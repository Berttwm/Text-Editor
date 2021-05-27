/* MAIN FILE */
#pragma once
#include"edit.h"
#include"search.h"
#include"file.h"
#include"EditorWindow.h"

// Define sizing of find/replace boxes
EditorWindow::EditorWindow(int w, int h, const char* t) : Fl_Double_Window(w, h, t) {
	replace_dlg = new Fl_Window(300, 105, "Replace");
	replace_find = new Fl_Input(80, 10, 210, 25, "Find:");
	replace_find->align(FL_ALIGN_LEFT);

	replace_with = new Fl_Input(80, 40, 210, 25, "Replace:");
	replace_with->align(FL_ALIGN_LEFT);

	replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
	replace_all->callback((Fl_Callback *)replall_cb, this);

	replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
	replace_next->callback((Fl_Callback *)replace2_cb, this);

	replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
	replace_cancel->callback((Fl_Callback *)replcan_cb, this);
	replace_dlg->end();
	replace_dlg->set_non_modal();
	editor = 0;
	*search = (char)0;
}

EditorWindow::~EditorWindow() {
	delete replace_dlg;
}
// Design of GUI
Fl_Window *replace_dlg = new Fl_Window(300, 105, "Replace");
Fl_Input *replace_find = new Fl_Input(70, 10, 200, 25, "Find:");
Fl_Input *replace_with = new Fl_Input(70, 40, 200, 25, "Replace:");
Fl_Button *replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
Fl_Button *replace_next = new Fl_Button(105, 70, 120, 25, "Replace Next");
Fl_Button *replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
// Main Method
int main(int argc, char **argv)
{
	textbuf = new Fl_Text_Buffer;
	style_init();
	fl_open_callback(cb);
	Fl_Window* window = new_view();

	window->show(1, argv);

	if (argc > 1) load_file(argv[1], -1);

	return Fl::run();

}

