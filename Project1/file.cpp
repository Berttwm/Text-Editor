#include"EditorWindow.h"

#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Menu_Item.H>

extern Fl_Text_Buffer *textbuf;
/*
	Menu Bar Text File Access Functions
	1) new_cb - Create blank new file
	2) save_cb - Save current file
	3) saveas_cb - Save current file as file name
	4) open_cb - Open a file
	5) insert_cb - Insert a file's contents into current position
*/
void new_cb(Fl_Widget*, void*) {
	if (!check_save()) return;

	filename[0] = '\0';
	textbuf->select(0, textbuf->length());
	textbuf->remove_selection();
	changed = 0;
	textbuf->call_modify_callbacks();
}
void save_cb(void) {
	if (filename[0] == '\0') {
		// No filename - get one!
		saveas_cb();
		return;
	}
	else save_file(filename);
}
void saveas_cb() {
	Fl_Native_File_Chooser fnfc;
	fnfc.title("What would you like to name your save file?");
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	if (fnfc.show()) return;
	save_file(fnfc.filename());
}
void open_cb(Fl_Widget*, void*) {
	if (!check_save()) return;
	Fl_Native_File_Chooser fnfc;
	fnfc.title("Open file");
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	if (fnfc.show()) return;
	load_file(fnfc.filename(), -1);
}
void insert_cb(Fl_Widget*, void* v) {
	Fl_Native_File_Chooser fnfc;
	fnfc.title("Insert File");
	fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
	if (fnfc.show()) return;
	EditorWindow *w = (EditorWindow *)v;
	load_file(fnfc.filename(), w->editor->insert_position());

}
void view_cb(Fl_Widget*, void*) {
	Fl_Window* w = new_view();
	w->show();
}
void close_cb(Fl_Widget*, void* v) {
	EditorWindow* w = (EditorWindow*)v;

	if (num_windows == 1) {
		if (!check_save())
			return;
	}

	w->hide();
	w->editor->buffer(0);
	textbuf->remove_modify_callback(style_update, w->editor);
	textbuf->remove_modify_callback(changed_cb, w);
	Fl::delete_widget(w);

	num_windows--;
	if (!num_windows) exit(0);
}
void quit_cb(Fl_Widget*, void*) {
	if (changed && !check_save())
		return;

	exit(0);
}
/*
	File Manipulation Functions
	1) check_save - check if file has been saved, if not, cancel operation, save or discard file
	2) save_file - save the file
	3) load_file - load the file
	4) changed_cb - check if file has been changed
*/
int check_save(void) {
	if (!changed) return 1;

	int r = fl_choice("The current file has not been saved.\n"
		"Would you like to save it now?",
		"Cancel", "Save", "Discard");

	if (r == 1) {
		save_cb(); // Save the file...
		return !changed;
	}

	return (r == 2) ? 1 : 0;
}
void save_file(const char *newfile) {
	if (textbuf->savefile(newfile))
		fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
	else
		strcpy(filename, newfile);
	changed = 0;
	textbuf->call_modify_callbacks();
}
void load_file(const char *newfile, int ipos) {
	loading = 1;
	int insert = (ipos != -1);
	changed = insert;
	if (!insert) strcpy(filename, "");
	int r;
	if (!insert) r = textbuf->loadfile(newfile);
	else r = textbuf->insertfile(newfile, ipos);
	if (r)
		fl_alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
	else
		if (!insert) strcpy(filename, newfile);
	loading = 0;
	textbuf->call_modify_callbacks();
}

void changed_cb(int, int nInserted, int nDeleted, int, const char*, void* v) {
	if ((nInserted || nDeleted) && !loading) changed = 1;
	EditorWindow *w = (EditorWindow *)v;
	set_title(w);
	if (loading) w->editor->show_insert_position();
}