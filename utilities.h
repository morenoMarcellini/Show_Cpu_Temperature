#ifndef UTILITIES
#define UTILITIES

#include "X11/Xlib.h"

extern int make_unresizable_window(Display*, Window);
extern int generate_index(char *);
extern int set_window_name(Display*, Window);
#endif
