#pragma once
#include <FL/Fl.h>

void new_cb(Fl_Widget*, void*);
void open_cb(Fl_Widget*, void*);
void insert_cb(Fl_Widget*, void*);
void save_cb();
void saveas_cb();
void view_cb(Fl_Widget*, void*);
void close_cb(Fl_Widget*, void*);
void quit_cb(Fl_Widget*, void*);
int check_save(void);
void save_file(const char *newfile);
void load_file(const char *newfile, int ipos);
void changed_cb(int, int nInserted, int nDeleted, int, const char*, void* v);
