#include "utilities.h"
/* system includes */
#include "X11/Xlib.h"

/* make unresizable window */
int make_unresizable_window(Display *dpy, Window w){

	XWindowAttributes wAtt;
	
	XSizeHints *size_hints = XAllocSizeHints();
	size_hints->flags = PMinSize | PMaxSize;
	size_hints->min_width = size_hints->max_width = watt.size_x;
	size_hints->min_height = size_hints->max_height = watt.size_y;
	XSetWMNormalHints(dpy, window, size_hints);
	XFree(size_hints);
}
