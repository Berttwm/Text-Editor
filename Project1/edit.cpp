#include"EditorWindow.h"

/*
	Menu Bar Text Section Functions
	1) undo_cb - Undo last change
	2) cut_cb - Remove current section
	3) copy_cb - Copy current section
	4) paste_cb - Paste last copied section
	5) delete_cb - Delete section
*/

void undo_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	Fl_Text_Editor::kf_undo(0, e->editor);
}
void cut_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	Fl_Text_Editor::kf_cut(0, e->editor);
}
void copy_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	Fl_Text_Editor::kf_copy(0, e->editor);
}
void paste_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	Fl_Text_Editor::kf_paste(0, e->editor);
}
void delete_cb(Fl_Widget*, void*) {
	textbuf->remove_selection();
}