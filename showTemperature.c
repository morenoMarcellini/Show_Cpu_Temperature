#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
/* XExtension */
#include <X11/extensions/sync.h>
/* Xcms */
#include <X11/Xcms.h>
/* XPM */
#include <X11/xpm.h>
/* C headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <sys/param.h>
/* Local */
#include "jetColor.h"
#include "showTemperature.h"
#include "xpm_definition.h"
#include "read_temperature.h"
#include "get_host_name.h"
#include "XPixmap.h"
#include "sync_XAlarm.h"

int main(int argc, char **argv)
{
    /* X11 stuff */
    Display *dpy;
    Window window, win;
    /* XSetWindowAttributes needed to set the initial attributes */
    XSetWindowAttributes wset;
    /* XWindowAttributes are readable attributes */
    // XWindowAttributes win_a;
    
    XEvent event;
    Atom wmDeleteMessage;
    GC gc;
    int parent;	// the root window;
    int defaultScreen;
    Pixmap p = None;
    XPixmapAttributes p_att;

    /*
     * Other stuff
     * window dimensions
     */
    WindowTopology watt;

    watt.pos_x = 10;
    watt.pos_y = watt.pos_x;
    watt.size_x = 256;
    watt.size_y = watt.size_x;
    watt.border_size = 2;
	
	
	size_t ticker = 0;
	float previous = 0.0f;
	int up = 0, down = 0;
	
    /* easy way to get hostname */
    char hostname[1024];
    get_host_name_IP4(hostname);
    printf("%s\n", hostname);
    /* we try with XThreads */
    if (XInitThreads() == 0)
    {
	fprintf(stderr, "Error in XInitThreads\n");
	return 1;
    }
    // Open the display
    dpy = XOpenDisplay(NULL);
    if (!dpy)
    {
	fprintf(stderr, "Failed to open dpy\n");
	return 1;
    }
    printf("Protocol Version %d, Protocol Revision %d\n", XProtocolVersion(dpy), XProtocolRevision(dpy));

    parent = DefaultRootWindow(dpy);
    defaultScreen = DefaultScreen(dpy);

    /*
     * We get some info on VisualInfo
     * Variable
     */
    XVisualInfo visInfo;
    XVisualInfo visTemplate;
    XVisualInfo *vis;
    int nxvisuals;

    visTemplate.screen = DefaultScreen(dpy);

    vis = XGetVisualInfo(dpy, VisualScreenMask, &visTemplate, &nxvisuals);
    if (!XMatchVisualInfo(dpy, defaultScreen, 32, TrueColor, &visInfo))
    {
	fprintf(stderr, "no such visual\n");
	return 1;
    }
    printf("Matched visual 0x%lx class %d (%s) depth %d\n", visInfo.visualid,
		visInfo.class, (visInfo.class == TrueColor ? "TrueColor" : "unknown"), 
		visInfo.depth);

    XSync(dpy, True);                   // clear out the event queue

	/* After connect to display, got root, screen, and visual, we can define 
	 * the main window */

	/* Define few attributes */
	wset.bit_gravity = StaticGravity; //ForgetGravity;
	wset.win_gravity = NorthWestGravity;
	wset.backing_store = WhenMapped;
	wset.background_pixel = WhitePixel(dpy, defaultScreen);
	wset.border_pixel = BlackPixel(dpy, defaultScreen);
	wset.event_mask = ExposureMask;
	wset.save_under = True;
	
	unsigned int w_mask;
	w_mask = CWBackPixel;
	w_mask |= CWBorderPixel;
	w_mask |= CWEventMask;
	w_mask |= CWSaveUnder;
	w_mask |= CWBackingStore;
	w_mask |= CWBitGravity;
	w_mask |= CWWinGravity;
#if 0
    // Create a window
    window = XCreateSimpleWindow(dpy,   // Display
	RootWindow(dpy, 0),             // Parent window
	watt.pos_x, watt.pos_y,         // X and Y coordinates
	watt.size_x, watt.size_y,       // Width and height
	1,                              // Border width
	BlackPixel(dpy, defaultScreen), // Border color
	WhitePixel(dpy, defaultScreen)  // Background color
	);
#else
	window = XCreateWindow(dpy, parent, 
	watt.pos_x, watt.pos_y, watt.size_x, watt.size_y, watt.border_size, 
	DefaultDepth(dpy, defaultScreen),
	InputOutput,
	DefaultVisual(dpy, defaultScreen), 
	w_mask, &wset);
#endif
    if (!window)
    {
	fprintf(stderr, "Failed to create simple window\n");
	return 1;
    }
    // Get the WM_DELETE_WINDOW atom
    wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, window, &wmDeleteMessage, 1);

    // Map the window (make it visible)
    wset.backing_store = WhenMapped;
    XChangeWindowAttributes(dpy, window, CWBackingStore, &wset);
    XMapWindow(dpy, window);

    // Select events to receive
    XSelectInput(dpy, window, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);
    XFlush(dpy);

    /* To be able to draw anything we need to create the graphic context GC *
     * Create Graphic Context
     *
     * GC XCreateGC(
     *  Display *display,
     *  Drawable d,
     *  unsigned long valuemask,
     *  XGCValues *values
     * );
     */
    gc = XCreateGC(dpy, window, 0, NULL);
    if (!gc)
    {
	fprintf(stderr, "Failed to create graphic context\n");
	return 1;
    }
    /* we set the pencil colour */
    XSetForeground(dpy, gc, BlackPixel(dpy, defaultScreen));

    /* set the window name, app name, and class */
    char *appname = "Cpu(s) Temperature(s)";
    XClassHint *classHint;

    /* set the titlebar name */
    XStoreName(dpy, window, appname);

    /* set the name and class hints for the window manager to use */
    classHint = XAllocClassHint();
    if (classHint)
    {
	classHint->res_name = appname;
	classHint->res_class = "CpuTemperature";
    }
    XSetClassHint(dpy, window, classHint);
    XFree(classHint);

    /* Example for Colours */
    Colormap cmap = DefaultColormap(dpy, 0);

    /*
     * XColor near_color, true_color;
     * XAllocNamedColor(dpy, cmap, "red", &near_color, &true_color);
     */
    /* Allocate colors with Xcms */
    XcmsColor cms_exact, *cms_screen;

    /* we use only the cms_screen */
    cms_screen = (XcmsColor *)malloc(128 * sizeof (XcmsColor));
    for (size_t i = 0, j = 0; i < 256; i += 2, j++)
    {
	Status status = XcmsLookupColor(dpy, cmap, jetColor[i], &cms_exact, 
		&cms_screen[j], XcmsRGBiFormat);

	if (status == XcmsFailure)
	{
	    fprintf(stderr, "Failure in XcmsLookupColor: %ld\n", i);
	    return -1;
	}
    }
#ifdef XCMSDEBUG
    XcmsFloat red, green, blue;

    red = cms_exact.spec.RGBi.red;
    green = cms_exact.spec.RGBi.green;
    blue = cms_exact.spec.RGBi.blue;
    switch (status)
    {
    case XcmsSuccess:
	printf("Success : red = %f green = %f blue = %f\n", red, green, blue);
	break;
    case XcmsSuccessWithCompression:
	printf("Compressed : red = %f green = %f blue = %f\n", red, green, blue);
	break;
    case XcmsFailure:
	printf("Xcms failure\n");
	break;
    default:
	printf("This should never happen\n");
    }
#endif
    /* Alloc the color */
    XcmsAllocColor(dpy, cmap, &cms_exact, XcmsRGBFormat);
    for (size_t i = 0; i < 128; i++)
    {
	Status status = XcmsAllocColor(dpy, cmap, &cms_screen[i], XcmsRGBFormat);
#if 1
	if (status != XcmsSuccess)
	{
	    fprintf(stderr, "Error in XcmsAllocColor: %ld \n", i);
	    return -1;
	}
#else
	XcmsFloat red, green, blue;

	red = cms_screen[i].spec.RGBi.red;
	green = cms_screen[i].spec.RGBi.green;
	blue = cms_screen[i].spec.RGBi.blue;
	switch (status)
	{
	case XcmsSuccess:
	    printf("Success : red = %f green = %f blue = %f\n", red, green, blue);
	    break;
	case XcmsSuccessWithCompression:
	    printf("Compressed : red = %f green = %f blue = %f\n", red, green, blue);
	    break;
	case XcmsFailure:
	    printf("Xcms failure\n");
	    break;
	default:
	    printf("This should never happen\n");
	}
#endif
    }
    /* Wait for the MapNotify event */
    for (;;)
    {
	XEvent e;
	XNextEvent(dpy, &e);
	if (e.type == MapNotify)
	    break;
    }
    
#if 0
   /* define few parameter for the simple arc*/
    struct ArcAttr arcatt;
    arcatt.rad_x = 128;
    arcatt.rad_y = 128;
    arcatt.three = 64 * 90;
    arcatt.length = 64 * 360;

    XDrawArc(dpy, window, gc, (watt.size_x - arcatt.rad_x) >> 1, 
    (watt.size_y - arcatt.rad_y) >> 1, arcatt.rad_x,
	arcatt.rad_y, arcatt.three, arcatt.length);
#else
	/* Simpler way */
	XArc circle;
    circle.width = circle.height = 128;
    circle.angle1 = 64 * 90;
    circle.angle2 = 64 *360;
    circle.x = (watt.size_x - circle.width) >> 1;
    circle.y = (watt.size_y - circle.height) >> 1;

	XDrawArcs(dpy, window, gc, &circle, 1);
#endif
    // Flush the drawing commands
    XFlush(dpy);

    /* Font methods */
    XFontStruct *font_info;             /* Font structure, used for drawing text */
    // char *font_name = "-*-*-*-*-*-*-34-*-*-*-*-*-*-*";
    char *font_name = "-*-*-demibold-r-*-*-34-*-*-*-*-*-*-*";
    /* try to load the given font. */
    font_info = XLoadQueryFont(dpy, font_name);
    if (!font_info)
    {
	fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
	exit(-1);
    }
    /* assign the given font to our GC. */
    XSetFont(dpy, gc, font_info->fid);

    /* variables used for drawing the text strings. */
    int x, y;
    char text_string[128];
    int string_width;
    int font_height;

    memset(text_string, '\0', 128);
    /* find the height of the characters drawn using this font.        */
    font_height = font_info->ascent + font_info->descent;
    /* Write the name of the host on top of the window */
    x = (watt.size_x - XTextWidth(font_info, hostname, strlen(hostname))) >> 1;
    y = font_height;
    XDrawString(dpy, window, gc, x, y, hostname, strlen(hostname));

    read_temperature(text_string);

    /*
     * find the width, in pixels, of the text that will be drawn using
     * the given font.
     */
    string_width = XTextWidth(font_info, text_string, strlen(text_string));
    x = (watt.size_x - string_width) >> 1;
    y = ((watt.size_y + font_height) >> 1) - 8;
    
    /* Add rectangle to draw the relative variation of temperature */
    /* change to subwindow */
#if 0
    int sub_x = 32;
    int sub_y = (watt.size_y - 48);
    unsigned int sub_width = watt.size_x - 64;
    unsigned int sub_height = 16;
    XDrawRectangle(dpy, window, gc, sub_x, sub_y, sub_width, sub_height);
#else
	/* we recycle the original XSetWindowAttributes */
	struct WindowTopology sub_w;
	sub_w.pos_x = 32;
	sub_w.pos_y = (watt.size_y - 48);
	sub_w.size_x = watt.size_x - 64;
	sub_w.size_y = 32;
	sub_w.border_size = 1;
	wset.background_pixel = BlackPixel(dpy, defaultScreen);
	
	win = XCreateWindow(dpy, window,
		sub_w.pos_x, sub_w.pos_y, sub_w.size_x, sub_w.size_y, sub_w.border_size,
		DefaultDepth(dpy, defaultScreen),
		InputOutput,
		DefaultVisual(dpy, defaultScreen),
		w_mask, &wset);
	if (win != 0) XMapWindow(dpy, win);
		else 
		return -1;
#endif
    /* we change the gc.foreground */
    XGCValues xgc;
    xgc.foreground = cms_exact.pixel;
    XChangeGC(dpy, gc, GCForeground, &xgc);
#if 0
	struct ArcAttr arcatt;
    arcatt.rad_x = 128;
    arcatt.rad_y = 128;
    arcatt.three = 64 * 90;
    arcatt.length = 64 * 360;
    XFillArc(dpy, window, gc, (watt.size_x - arcatt.rad_x) >> 1, 
		(watt.size_y >> 1) - (arcatt.rad_y >> 1), arcatt.rad_x,
		arcatt.rad_y, arcatt.three, arcatt.length);
#else
	XFillArcs(dpy, window, gc, &circle, 1);
#endif
    /* back to black pencil */
    xgc.foreground = BlackPixel(dpy, defaultScreen);
    XChangeGC(dpy, gc, GCForeground, &xgc);
    XDrawString(dpy, window, gc, x, y, text_string, strlen(text_string));
#if 0    
    /* XSync */
/*
https://nrk.neocities.org/articles/x11-timeout-with-xsyncalarm
*/
    int sync_event, sync_error, major, minor;
    if (!XSyncQueryExtension(dpy, &sync_event, &sync_error))
    {
	fprintf(stderr, "XSync extension not available");
	return 1;
    }
    if (!XSyncInitialize(dpy, &major, &minor))
    {
	fprintf(stderr, "failed to initialize XSync extension");
	return 1;
    }
    printf("XSync major %d minor %d\n", major, minor);

    int ncounter;
    XSyncSystemCounter *counters;
    XSyncCounter servertime = None;
    if ((counters = XSyncListSystemCounters(dpy, &ncounter)) != NULL)
    {
	for (int i = 0; i < ncounter; i++)
	{
	    // fprintf(stderr, "%s\n", counters[i].name);
	    if (strcmp(counters[i].name, "SERVERTIME") == 0)
	    {
		servertime = counters[i].counter;
		break;
	    }
	}
	XSyncFreeSystemCounterList(counters);
    }
    if (servertime == None)
    {
	fprintf(stderr, "SERVERTIME counter not found");
	return 1;
    }
    XSyncAlarmAttributes al_attr;
    long unsigned int flags = 0;
    al_attr.trigger.counter = servertime;
    flags |= XSyncCACounter;
#ifdef DEBUG
    XSyncIntToValue(&al_attr.trigger.wait_value, 100);
#else
	XSyncIntToValue(&al_attr.trigger.wait_value, 1000);
#endif
    flags |= XSyncCAValue;
    al_attr.trigger.value_type = XSyncRelative;
    flags |= XSyncCAValueType;
    al_attr.trigger.test_type = XSyncPositiveComparison;
    flags |= XSyncCATestType;
/* set the delta to 0, this will deactivate the alarm after firing once. */
    XSyncIntToValue(&al_attr.delta, 0);
    flags |= XSyncCADelta;

    XSyncAlarm alarm = XSyncCreateAlarm(dpy, flags, &al_attr);

    /*
     * end of alarm
     */
#else
	int sync_event = sync_XAlarm(dpy);
#endif
    /* generate Pixmap from xpm */
    Pixmap red_p, blue_p, black_p, white_p;
    Pixmap clipper, pattern;
    XpmAttributes xpm_a;
    Status stat;
    
    xpm_a.color_key = XPM_COLOR;
    
    xpm_a.valuemask = XpmColorKey | XpmColorTable | XpmReturnPixels | XpmReturnExtensions;
	xpm_a.valuemask |= XpmReturnAllocPixels;
	
    stat = XpmCreatePixmapFromData(dpy, parent, red_dot, &red_p, &clipper, &xpm_a);
    printf("%s\n", XpmGetErrorString(stat));
    if (stat < 0)
		return -1;
	
    stat = XpmCreatePixmapFromData(dpy, parent, blue_dot, &blue_p, &clipper, &xpm_a);
    printf("%s\n", XpmGetErrorString(stat));
    if (stat < 0)
		return -1;
	
    stat = XpmCreatePixmapFromData(dpy, parent, black_dot, &black_p, &clipper, &xpm_a);
    printf("%s\n", XpmGetErrorString(stat));
    if (stat < 0)
		return -1;
	
	stat = XpmCreatePixmapFromData(dpy, parent, white_dot, &white_p, &clipper, &xpm_a);
    printf("%s\n", XpmGetErrorString(stat));
    if (stat < 0)
		return -1;
		
	/* make unresizable window */
	XSizeHints *size_hints = XAllocSizeHints();
	size_hints->flags = PMinSize | PMaxSize;
	size_hints->min_width = size_hints->max_width = watt.size_x;
	size_hints->min_height = size_hints->max_height = watt.size_y;
	XSetWMNormalHints(dpy, window, size_hints);
	XFree(size_hints);
	
    /*
     * Main loop
     */
    while (1) {
	XNextEvent(dpy, &event);
	switch (event.type)
	{
	case Expose:
	    printf("Expose event\n");
	    break;
	case KeyPress:
	    printf("KeyPress event\n");
	    if (event.xkey.keycode == XKeysymToKeycode(dpy, XK_Escape))
	    {
		printf("Escape key pressed, exiting...\n");
		goto cleanup;
	    }
	    break;
	case ButtonPress:
	    printf("ButtonPress event\n");

	    /*
	     * if (event.xbutton.button == Button1) {
	     * Window root_return, child_return;
	     *  int root_x_return, root_y_return;
	     *  int win_x_return, win_y_return;
	     *  unsigned int mask_return;
	     *  bool result;
	     *  result = XQueryPointer(dpy, window, &root_return, &child_return,
	     *  &root_x_return, &root_y_return, &win_x_return, &win_y_return,
	     *  &mask_return);
	     *  //printf("%d %d\n", win_x_return, win_y_return);
	     *  XFillRectangle(dpy, window, gc, win_x_return, win_y_return, 10, 10);
	     *  }
	     *  if (event.xbutton.button == Button2) {
	     * Window root_return, child_return;
	     *  int root_x_return, root_y_return;
	     *  int win_x_return, win_y_return;
	     *  unsigned int mask_return;
	     *  bool result;
	     *  result = XQueryPointer(dpy, window, &root_return, &child_return,
	     *  &root_x_return, &root_y_return, &win_x_return, &win_y_return,
	     *  &mask_return);
	     *  //printf("%d %d\n", win_x_return, win_y_return);
	     *  XFillArc(dpy, window, gc, win_x_return, win_y_return, 10, 10, 64*45, 64*360);
	     *  }
	     */
	    break;
	case ClientMessage:
	    if ((Atom)event.xclient.data.l[0] == wmDeleteMessage)
	    {
		printf("Close button pressed, exiting...\n");
		goto cleanup;
	    }
	    break;
	default:
	    if (event.type == (sync_event + XSyncAlarmNotify))
	    {
		/* got an alarm */
		read_temperature(text_string);
		float Celsius = (float)atof(text_string);
		size_t index = 0;
		const float tick = 80.0f / 127.0f;
		float test = 19.9f;
		while (test < Celsius) {
		    index++;
		    test += tick;
		}
		index = MIN(index, 127);

		XSetForeground(dpy, gc, cms_screen[index].pixel);
#if 0
		struct ArcAttr arcatt;
		arcatt.rad_x = 128;
		arcatt.rad_y = 128;
		arcatt.three = 64 * 90;
		arcatt.length = 64 * 360;
		XFillArc(dpy, window, gc, (watt.size_x - arcatt.rad_x) >> 1, 
			(watt.size_y - arcatt.rad_y) >> 1,
		    arcatt.rad_x, arcatt.rad_y, arcatt.three, arcatt.length);
#else
		XFillArcs(dpy, window, gc, &circle, 1);
#endif
		if (p != None){
#if 1
			XCopyArea(dpy, p, win, gc, 0, 0, (watt.size_x-64), 14, 0, 0);
#else
			XCopyArea(dpy, p, window, gc, 2, 0, (watt.size_x-64-2), 14,
			33, (watt.size_y-48+1));
#endif
			}
		
		if (ticker == 0){
#if 0
			XCopyArea(dpy, black_p, window, gc, 0, 0, 2, 2, sub_x + 1, sub_y + 7);
#else
			XCopyArea(dpy, white_p, win, gc, 0, 0, 2, 2, 1, 7);
#endif
			previous = Celsius;
			ticker += 2;
		}
		else {
			if (Celsius > previous){
				up -= 2;
				up = MAX(up, -6);
				down = 0;
#if 0
				XCopyArea(dpy, red_p, window, gc, 0, 0, 2, 2, sub_x + 1 + ticker, 
				sub_y + 7 + up);
#else
				XCopyArea(dpy, red_p, win, gc, 0, 0, 2, 2, 1+ticker, 7+ up);
#endif
				previous = Celsius;
			}
			else if (Celsius == previous) {
#if 0
				XCopyArea(dpy, black_p, window, gc, 0, 0, 2, 2, sub_x + 1 + ticker, 
				sub_y + 7);
#else
				XCopyArea(dpy, white_p, win, gc, 0, 0, 2, 2, 1+ticker, 7);
#endif
				up = 0;
				down = 0;
				}
			else {
				down += 2;
				down = MIN(down, 6);
				up = 0;
#if 0
				XCopyArea(dpy, blue_p, window, gc, 0, 0, 2, 2, sub_x + 1 + ticker, 
				sub_y + 7 + down);
#else
				XCopyArea(dpy, blue_p, win, gc, 0, 0, 2, 2, 1+ticker, 7+ down);
#endif
				previous = Celsius;
				}
			}

			ticker += 2;
			if (ticker >= (watt.size_x - 64 - 2)){
				ticker -= 2;
				if (p == None){
					p_att.w = sub_w.size_x;
					p_att.h = sub_w.size_y;
					p_att.depth = DefaultDepth(dpy, defaultScreen);
					p_att.root = win;
					/* Pixmap XCreatePixmap(Display *display, Drawable d, 
					 * 	unsigned int width, unsigned int height, 
					 * 	unsigned int depth); */
					/*
					p = XCreatePixmap(dpy, parent, (watt.size_x-64), 14,
					DefaultDepth(dpy, defaultScreen)); */
					p = XCreatePixmap(dpy, p_att.root, p_att.w, p_att.h, p_att.depth);
					if (p != None) {
						XSetForeground(dpy, gc, WhitePixel(dpy, defaultScreen));
						XFillRectangle(dpy, p, gc, 0, 0, (watt.size_x-64), 14);
						XSetForeground(dpy, gc, BlackPixel(dpy, defaultScreen));
					}
				}
				XCopyArea(dpy, window, p, gc, (32+1), (watt.size_y-48+1), 
				(watt.size_x-64-2), 14, 0, 0);
			}
		/*
		 * find the width, in pixels, of the text that will be drawn using
		 * the given font.
		 */

		string_width = XTextWidth(font_info, text_string, strlen(text_string));
		x = (watt.size_x - string_width) >> 1;
		y = ((watt.size_y + font_height) >> 1) - 8;
		XSetForeground(dpy, gc, BlackPixel(dpy, defaultScreen));
		XDrawString(dpy, window, gc, x, y, text_string, strlen(text_string));
	    }
#if 0
	    else
	    {
		printf("Unknown event type: %d\n", event.type);
	    }
#endif
	    break;
	}                               /* End XNextEvent switch */
#if 0	/* restart alarm */
	XSyncChangeAlarm(dpy, alarm, flags, &al_attr);
#else
	reload_XAlarm(dpy);
#endif
    } /* End of Loop */
    
cleanup: 
    // Clean up
    /* all the Pixmap stored in XServer */
    XFreePixmap(dpy, red_p);
    XFreePixmap(dpy, black_p);
    XFreePixmap(dpy, blue_p);
    XFreePixmap(dpy, white_p);
    //XFreePixmap(dpy, clipper);
    //XFreePixmap(dpy, pattern);
    if (p!=None)
		XFreePixmap(dpy, p);
    /* the rest*/
    XSync(dpy, True);
    #if 0
    XSyncDestroyAlarm(dpy, alarm);
    #else
    destroy_XAlarm(dpy);
    #endif
    XUnmapWindow(dpy, win);
    XUnmapWindow(dpy, window);
    XDestroyWindow(dpy, win);
    XDestroyWindow(dpy, window);
    XCloseDisplay(dpy);

    return 0;
}
