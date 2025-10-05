#include <X11/Xlib.h>
/* XPM */
#include <X11/xpm.h>
/* standard C headers */
#include <stdlib.h>
#include <stdio.h>
/* local definition */
#include "xpm_definition.h"

/* generate Pixmap from xpm */
Pixmap red_p, blue_p, black_p, white_p, rainbow_p;
Pixmap clipper, pattern;
XImage *xi_clipper, *xi_pattern;
XpmAttributes xpm_a;
static Status init = -1;

Status xpm_init()
{
    xpm_a.color_key = XPM_COLOR;
    xpm_a.valuemask = 0x0;
    xpm_a.valuemask |= XpmColorKey;
    xpm_a.valuemask |= XpmColorTable;
    xpm_a.valuemask |= XpmReturnPixels;
    xpm_a.valuemask |= XpmReturnExtensions;
    xpm_a.valuemask |= XpmReturnAllocPixels;

    return XpmSuccess;
}

Pixmap create_rainbow_pixmap(Display *dpy, Drawable parent)
{
    if (init != XpmSuccess)
	xpm_init();
    Status stat = XpmCreatePixmapFromData(dpy, parent, rainbow, &pattern, &clipper, &xpm_a);

    if (stat == Success)
    {
	return pattern;
    }
    else
    {
	fprintf(stderr, "Error in allocating Pixmap, exiting\n");
	exit(-1);
    }
}

XImage *create_rainbow_ximage(Display *dpy)
{
    if (init != XpmSuccess)
	xpm_init();
    Status stat = XpmCreateImageFromData(dpy, rainbow, &xi_pattern, &xi_clipper, &xpm_a);

    if (stat == Success)
    {
	return xi_pattern;
    }
    else
    {
	fprintf(stderr, "Error in allocating XImage, exiting\n");
	exit(-1);
    }
}
