#ifndef SYNC
#define SYNC
#include <X11/Xlib.h>

extern int sync_XAlarm(Display *);
extern int reload_XAlarm(Display *);
extern int destroy_XAlarm(Display *);

#endif
