#include"edit.h"
#include"search.h"
#include"file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Text_Editor.H>

#define TS 14 // default editor textsize
extern int num_windows;
extern int changed;
extern int loading;
extern char filename[256];
extern char title[256];
extern Fl_Text_Buffer *textbuf;
extern Fl_Text_Buffer *stylebuf;
extern Fl_Text_Display::Style_Table_Entry
styletable[];

class EditorWindow : public Fl_Double_Window {
public:
	EditorWindow(int w, int h, const char* t);
	~EditorWindow();

	Fl_Window          *replace_dlg;
	Fl_Input           *replace_find;
	Fl_Input           *replace_with;
	Fl_Button          *replace_all;
	Fl_Return_Button   *replace_next;
	Fl_Button          *replace_cancel;

	Fl_Text_Editor     *editor;
	char               search[256];
};

Fl_Menu_Item menuitems[];

Fl_Window* new_view();
void set_title(Fl_Window* w);
void style_init(void);
void style_update(int pos, int nInserted, int nDeleted, int, const char *, void *cbArg);
void style_parse(const char *text, char *style, int length);
void style_unfinished_cb(int, void*);
void cb(const char *fname);