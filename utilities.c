#include "utilities.h"
/* X11 headers */
#include "X11/Xlib.h"
#include <X11/Xutil.h>
/* std headers */
#include <stdlib.h>
#include <sys/param.h>


/* make unresizable window */
int make_unresizable_window(Display *dpy, Window w){

	XWindowAttributes wAtt;
	
	Status stat = XGetWindowAttributes(dpy, w, &wAtt);
	
	if (stat == Success) { 
	XSizeHints *size_hints = XAllocSizeHints();
	size_hints->flags = PMinSize | PMaxSize;
	size_hints->min_width = size_hints->max_width = wAtt.width;
	size_hints->min_height = size_hints->max_height = wAtt.height;
	XSetWMNormalHints(dpy, w, size_hints);
	XFree(size_hints);
	return 1;
	}
	else 
		return 0;
}

int generate_index(char *t){
	
		float Celsius = (float) atof(t);
		size_t index = 0;
		const float tick = 80.0f / 127.0f;
		float test = 19.9f;
		while (test < Celsius) {
		    index++;
		    test += tick;
		}
		index = MIN(index, 127);
		return index;
		}

int set_window_name(Display *dpy, Window w){
	 /* set the window name, app name, and class */
    char *appname = "Cpu(s) Temperature(s)";
    XClassHint *classHint;

    /* set the titlebar name */
    XStoreName(dpy, w, appname);

    /* set the name and class hints for the window manager to use */
    classHint = XAllocClassHint();
    if (classHint)
    {
	classHint->res_name = appname;
	classHint->res_class = "CpuTemperature";
    }
    XSetClassHint(dpy, w, classHint);
    XFree(classHint);
    
    return 1;
	}
