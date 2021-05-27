#include "EditorWindow.h"

#include <FL/FL_Menu_Bar.H>

/*
	Styling functions
	1) style_init() - initialize styling parameters
	2) style_parse() - parse styling options
	3) style_updates() - positional styling updates
*/

int num_windows = 0;
int changed = 0;
int loading = 0;
char filename[256] = "";
char title[256];
Fl_Text_Buffer *textbuf = 0;
Fl_Text_Buffer *stylebuf = 0;
Fl_Text_Display::Style_Table_Entry
styletable[] = {	// Style table
{ FL_BLACK,      FL_COURIER,           TS }, // A - Plain
{ FL_DARK_GREEN, FL_HELVETICA_ITALIC,  TS }, // B - Line comments
{ FL_DARK_GREEN, FL_HELVETICA_ITALIC,  TS }, // C - Block comments
{ FL_BLUE,       FL_COURIER,           TS }, // D - Strings
{ FL_DARK_RED,   FL_COURIER,           TS }, // E - Directives
{ FL_DARK_RED,   FL_COURIER_BOLD,      TS }, // F - Types
{ FL_BLUE,       FL_COURIER_BOLD,      TS }, // G - Keywords
};


const char  *code_keywords[] = {	// List of known C/C++ keywords...
			 "and",
			 "and_eq",
			 "asm",
			 "bitand",
			 "bitor",
			 "break",
			 "case",
			 "catch",
			 "compl",
			 "continue",
			 "default",
			 "delete",
			 "do",
			 "else",
			 "false",
			 "for",
			 "goto",
			 "if",
			 "new",
			 "not",
			 "not_eq",
			 "operator",
			 "or",
			 "or_eq",
			 "return",
			 "switch",
			 "template",
			 "this",
			 "throw",
			 "true",
			 "try",
			 "while",
			 "xor",
			 "xor_eq"
};
const char  *code_types[] = {	// List of known C/C++ types...
			 "auto",
			 "bool",
			 "char",
			 "class",
			 "const",
			 "const_cast",
			 "double",
			 "dynamic_cast",
			 "enum",
			 "explicit",
			 "extern",
			 "float",
			 "friend",
			 "inline",
			 "int",
			 "long",
			 "mutable",
			 "namespace",
			 "private",
			 "protected",
			 "public",
			 "register",
			 "short",
			 "signed",
			 "sizeof",
			 "static",
			 "static_cast",
			 "struct",
			 "template",
			 "typedef",
			 "typename",
			 "union",
			 "unsigned",
			 "virtual",
			 "void",
			 "volatile"
};
Fl_Menu_Item menuitems[] = {
  { "&File",              0, 0, 0, FL_SUBMENU },
	{ "&New File",        0, (Fl_Callback *)new_cb },
	{ "&Open File...",    FL_CTRL + 'o', (Fl_Callback *)open_cb },
	{ "&Insert File...",  FL_CTRL + 'i', (Fl_Callback *)insert_cb, 0, FL_MENU_DIVIDER },
	{ "&Save File",       FL_CTRL + 's', (Fl_Callback *)save_cb },
	{ "Save File &As...", FL_CTRL + FL_SHIFT + 's', (Fl_Callback *)saveas_cb, 0, FL_MENU_DIVIDER },
	{ "New &View", FL_ALT + 'v', (Fl_Callback *)view_cb, 0 },
	{ "&Close View", FL_CTRL + 'w', (Fl_Callback *)close_cb, 0, FL_MENU_DIVIDER },
	{ "E&xit", FL_CTRL + 'q', (Fl_Callback *)quit_cb, 0 },
	{ 0 },

  { "&Edit", 0, 0, 0, FL_SUBMENU },
	{ "&Undo",       FL_CTRL + 'z', (Fl_Callback *)undo_cb, 0, FL_MENU_DIVIDER },
	{ "Cu&t",        FL_CTRL + 'x', (Fl_Callback *)cut_cb },
	{ "&Copy",       FL_CTRL + 'c', (Fl_Callback *)copy_cb },
	{ "&Paste",      FL_CTRL + 'v', (Fl_Callback *)paste_cb },
	{ "&Delete",     0, (Fl_Callback *)delete_cb },
	{ 0 },

  { "&Search", 0, 0, 0, FL_SUBMENU },
	{ "&Find...",       FL_CTRL + 'f', (Fl_Callback *)find_cb },
	{ "F&ind Again",    FL_CTRL + 'g', find2_cb },
	{ "&Replace...",    FL_CTRL + 'r', replace_cb },
	{ "Re&place Again", FL_CTRL + 't', replace2_cb },
	{ 0 },

  { 0 }
};
extern "C" {
	int
		compare_keywords(const void *a,
			const void *b) {
		return (strcmp(*((const char **)a), *((const char **)b)));
	}
}
void style_init(void) {
	char *style = new char[textbuf->length() + 1];
	char *text = textbuf->text();

	memset(style, 'A', textbuf->length());
	style[textbuf->length()] = '\0';

	if (!stylebuf) stylebuf = new Fl_Text_Buffer(textbuf->length());

	style_parse(text, style, textbuf->length());

	stylebuf->text(style);
	delete[] style;
	free(text);
}
void cb(const char *fname) {
	load_file(fname, -1);
}

void style_unfinished_cb(int, void*) {
}

void style_parse(const char *text, char *style, int length) {
	char current;
	int	col;
	int	last;
	char buf[255],
		*bufptr;
	const char *temp;

	// Style letters:
	//
	// A - Plain
	// B - Line comments
	// C - Block comments
	// D - Strings
	// E - Directives
	// F - Types
	// G - Keywords

	for (current = *style, col = 0, last = 0; length > 0; length--, text++) {
		if (current == 'B' || current == 'F' || current == 'G') current = 'A';
		if (current == 'A') {
			// Check for directives, comments, strings, and keywords...
			if (col == 0 && *text == '#') {
				// Set style to directive
				current = 'E';
			}
			else if (strncmp(text, "//", 2) == 0) {
				current = 'B';
				for (; length > 0 && *text != '\n'; length--, text++) *style++ = 'B';

				if (length == 0) break;
			}
			else if (strncmp(text, "/*", 2) == 0) {
				current = 'C';
			}
			else if (strncmp(text, "\\\"", 2) == 0) {
				// Quoted quote...
				*style++ = current;
				*style++ = current;
				text++;
				length--;
				col += 2;
				continue;
			}
			else if (*text == '\"') {
				current = 'D';
			}
			else if (!last && (islower((*text) & 255) || *text == '_')) {
				// Might be a keyword...
				for (temp = text, bufptr = buf;
					(islower((*temp) & 255) || *temp == '_') && bufptr < (buf + sizeof(buf) - 1);
					*bufptr++ = *temp++) {
					// nothing
				}

				if (!islower((*temp) & 255) && *temp != '_') {
					*bufptr = '\0';

					bufptr = buf;

					if (bsearch(&bufptr, code_types,
						sizeof(code_types) / sizeof(code_types[0]),
						sizeof(code_types[0]), compare_keywords)) {
						while (text < temp) {
							*style++ = 'F';
							text++;
							length--;
							col++;
						}

						text--;
						length++;
						last = 1;
						continue;
					}
					else if (bsearch(&bufptr, code_keywords,
						sizeof(code_keywords) / sizeof(code_keywords[0]),
						sizeof(code_keywords[0]), compare_keywords)) {
						while (text < temp) {
							*style++ = 'G';
							text++;
							length--;
							col++;
						}

						text--;
						length++;
						last = 1;
						continue;
					}
				}
			}
		}
		else if (current == 'C' && strncmp(text, "*/", 2) == 0) {
			// Close a C comment...
			*style++ = current;
			*style++ = current;
			text++;
			length--;
			current = 'A';
			col += 2;
			continue;
		}
		else if (current == 'D') {
			// Continuing in string...
			if (strncmp(text, "\\\"", 2) == 0) {
				// Quoted end quote...
				*style++ = current;
				*style++ = current;
				text++;
				length--;
				col += 2;
				continue;
			}
			else if (*text == '\"') {
				// End quote...
				*style++ = current;
				col++;
				current = 'A';
				continue;
			}
		}

		// Copy style info...
		if (current == 'A' && (*text == '{' || *text == '}')) *style++ = 'G';
		else *style++ = current;
		col++;

		last = isalnum((*text) & 255) || *text == '_' || *text == '.';

		if (*text == '\n') {
			// Reset column and possibly reset the style
			col = 0;
			if (current == 'B' || current == 'E') current = 'A';
		}
	}
}
void style_update(int pos,		// I - Position of update
	int nInserted,	// I - Number of inserted chars
	int nDeleted,	// I - Number of deleted chars
	int /*nRestyled*/,	// I - Number of restyled chars
	const char * /*deletedText*/,// I - Text that was deleted
	void *cbArg) {	// I - Callback data

	int	start,				// Start of text
		end;				// End of text
	char last,				// Last style on line
		*style,				// Style data
		*text;				// Text data


	 // If this is just a selection change, just unselect the style buffer...
	if (nInserted == 0 && nDeleted == 0) {
		stylebuf->unselect();
		return;
	}

	// Track changes in the text buffer...
	if (nInserted > 0) {
		// Insert characters into the style buffer...
		style = new char[nInserted + 1];
		memset(style, 'A', nInserted);
		style[nInserted] = '\0';

		stylebuf->replace(pos, pos + nDeleted, style);
		delete[] style;
	}
	else {
		// Just delete characters in the style buffer...
		stylebuf->remove(pos, pos + nDeleted);
	}

	// Select the area that was just updated to avoid unnecessary
	// callbacks...
	stylebuf->select(pos, pos + nInserted - nDeleted);

	// Re-parse the changed region; we do this by parsing from the
	// beginning of the previous line of the changed region to the end of
	// the line of the changed region...  Then we check the last
	// style character and keep updating if we have a multi-line
	// comment character...
	start = textbuf->line_start(pos);
	//  if (start > 0) start = textbuf->line_start(start - 1);
	end = textbuf->line_end(pos + nInserted);
	text = textbuf->text_range(start, end);
	style = stylebuf->text_range(start, end);
	if (start == end)
		last = 0;
	else
		last = style[end - start - 1];

	//  printf("start = %d, end = %d, text = \"%s\", style = \"%s\", last='%c'...\n",
	//         start, end, text, style, last);

	style_parse(text, style, end - start);

	//  printf("new style = \"%s\", new last='%c'...\n",
	//         style, style[end - start - 1]);

	stylebuf->replace(start, end, style);
	((Fl_Text_Editor *)cbArg)->redisplay_range(start, end);

	if (start == end || last != style[end - start - 1]) {
		//    printf("Recalculate the rest of the buffer style\n");
			// Either the user deleted some text, or the last character
			// on the line changed styles, so reparse the
			// remainder of the buffer...
		free(text);
		free(style);

		end = textbuf->length();
		text = textbuf->text_range(start, end);
		style = stylebuf->text_range(start, end);

		style_parse(text, style, end - start);

		stylebuf->replace(start, end, style);
		((Fl_Text_Editor *)cbArg)->redisplay_range(start, end);
	}

	free(text);
	free(style);
}
/*
	1) new_view() - initialize a new view window
	2) set_title() - change filename title
*/
Fl_Window* new_view() {

	EditorWindow* w = new EditorWindow(660, 400, title);

	w->begin();
	Fl_Menu_Bar* m = new Fl_Menu_Bar(0, 0, 660, 30);
	m->copy(menuitems, w);
	w->editor = new Fl_Text_Editor(0, 30, 660, 370);
	w->editor->textfont(FL_COURIER);
	w->editor->textsize(TS);
	//w->editor->wrap_mode(Fl_Text_Editor::WRAP_AT_BOUNDS, 250);
	w->editor->buffer(textbuf);
	w->editor->highlight_data(stylebuf, styletable,
		sizeof(styletable) / sizeof(styletable[0]),
		'A', style_unfinished_cb, 0);


	w->end();
	w->resizable(w->editor);
	w->size_range(300, 200);
	w->callback((Fl_Callback *)close_cb, w);

	textbuf->add_modify_callback(style_update, w->editor);
	textbuf->add_modify_callback(changed_cb, w);
	textbuf->call_modify_callbacks();
	num_windows++;
	return w;
}

void set_title(Fl_Window* w) {
	if (filename[0] == '\0') strcpy(title, "Untitled");
	else {
		char *slash;
		slash = strrchr(filename, '/');
#ifdef WIN32
		if (slash == NULL) slash = strrchr(filename, '\\');
#endif
		if (slash != NULL) strcpy(title, slash + 1);
		else strcpy(title, filename);
	}

	if (changed) strcat(title, " (modified)");

	w->label(title);
}
