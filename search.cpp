#include"EditorWindow.h"
#include <FL/fl_ask.H>

/*
	Menu Bar Text Search/Replace Functions
	1) find_cb - Find first occurance of search string
	2) find2_cb - Find second occurance of search string
	3) replace_cb - Replace first occurance of search string
	4) replace2_cb - Replace next occurance of search string
	5) replall_cb - Replace all occurences of search string
	6) replcan_cb - Hides replace dialog
*/
void find_cb(Fl_Widget* w, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	const char *val;

	val = fl_input("Search String:", e->search);
	if (val != NULL) {
		// User entered a string - go find it!
		strcpy(e->search, val);
		find2_cb(w, v);
	}
	else {
		fl_alert("Find field cannot be empty!");
	}
}
void find2_cb(Fl_Widget* w, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	if (e->search[0] == '\0') {
		// Search string is blank; get a new one...
		find_cb(w, v);
		return;
	}

	int pos = e->editor->insert_position();
	int found = textbuf->search_forward(pos, e->search, &pos);
	if (found) {
		// Found a match; select and update the position...
		textbuf->select(pos, pos + strlen(e->search));
		e->editor->insert_position(pos + strlen(e->search));
		e->editor->show_insert_position();
	}
	else fl_alert("No occurrences of \'%s\' found!", e->search);
}
void replace_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	e->replace_dlg->show();
}
void replace2_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	const char *find = e->replace_find->value();
	const char *replace = e->replace_with->value();

	if (find[0] == '\0') {
		// Search string is blank; get a new one...
		e->replace_dlg->show();
		return;
	}

	e->replace_dlg->hide();

	int pos = e->editor->insert_position();
	int found = textbuf->search_forward(pos, find, &pos);

	if (found) {
		// Found a match; update the position and replace text...
		textbuf->select(pos, pos + strlen(find));
		textbuf->remove_selection();
		textbuf->insert(pos, replace);
		textbuf->select(pos, pos + strlen(replace));
		e->editor->insert_position(pos + strlen(replace));
		e->editor->show_insert_position();
	}
	else fl_alert("No occurrences of \'%s\' found!", find);
}
void replall_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	const char *find = e->replace_find->value();
	const char *replace = e->replace_with->value();

	find = e->replace_find->value();
	if (find[0] == '\0') {
		// Search string is blank; get a new one...
		e->replace_dlg->show();
		return;
	}

	e->replace_dlg->hide();

	e->editor->insert_position(0);
	int times = 0;

	// Loop through the whole string
	for (int found = 1; found;) {
		int pos = e->editor->insert_position();
		found = textbuf->search_forward(pos, find, &pos);

		if (found) {
			// Found a match; update the position and replace text...
			textbuf->select(pos, pos + strlen(find));
			textbuf->remove_selection();
			textbuf->insert(pos, replace);
			e->editor->insert_position(pos + strlen(replace));
			e->editor->show_insert_position();
			times++;
		}
	}
	if (times) fl_message("Replaced %d occurrences.", times);
	else fl_alert("No occurrences of \'%s\' found!", find);
}
void replcan_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	e->replace_dlg->hide();
}