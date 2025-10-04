#ifndef XPIXMAP
#define XPIXMAP
#include <X11/Xlib.h>
/* Copy&paste from XWindowAttributes */
typedef struct {
#if 0
    int x, y;                   /* location of window from where to copy */
#endif
    int w, h;          			/* width and height of pixmap */
#if 0
    int border_width;           /* border width of window ?? */
#endif
    int depth;                  /* depth of pixmap */
#if 0
    Visual *visual;             /* the associated visual structure ?? */
#endif
    Window root;                /* root of screen containing pixmap */
#if 0
#if defined(__cplusplus) || defined(c_plusplus)
    int c_class;                /* C++ InputOutput, InputOnly*/
#else
    int class;                  /* InputOutput, InputOnly*/
#endif
    int bit_gravity;            /* one of bit gravity values ?? */
    int win_gravity;            /* one of the window gravity values ?? */
    int backing_store;          /* NotUseful, WhenMapped, Always ?? */
    unsigned long backing_planes;/* planes to be preserved if possible ?? */
    unsigned long backing_pixel;/* value to be used when restoring planes ?? */
    Bool save_under;            /* boolean, should bits under be saved? ?? */
    Colormap colormap;          /* color map to be associated with pixmap */
    Bool map_installed;         /* boolean, is color map currently installed ??*/
    int map_state;              /* IsUnmapped, IsUnviewable, IsViewable ??*/
    long all_event_masks;       /* set of events all people have interest in ?? */
    long your_event_mask;       /* my event mask ?? */
    long do_not_propagate_mask; /* set of events that should not propagate ?? */
    Bool override_redirect;     /* boolean value for override-redirect ?? */
    Screen *screen;             /* back pointer to correct screen */
#endif
} XPixmapAttributes;

#if 0
typedef struct {
    int x, y;                   /* location of window */
    int width, height;          /* width and height of window */
    int border_width;           /* border width of window */
    int depth;                  /* depth of window */
    Visual *visual;             /* the associated visual structure */
    Window root;                /* root of screen containing window */
#if defined(__cplusplus) || defined(c_plusplus)
    int c_class;                /* C++ InputOutput, InputOnly*/
#else
    int class;                  /* InputOutput, InputOnly*/
#endif
    int bit_gravity;            /* one of bit gravity values */
    int win_gravity;            /* one of the window gravity values */
    int backing_store;          /* NotUseful, WhenMapped, Always */
    unsigned long backing_planes;/* planes to be preserved if possible */
    unsigned long backing_pixel;/* value to be used when restoring planes */
    Bool save_under;            /* boolean, should bits under be saved? */
    Colormap colormap;          /* color map to be associated with window */
    Bool map_installed;         /* boolean, is color map currently installed*/
    int map_state;              /* IsUnmapped, IsUnviewable, IsViewable */
    long all_event_masks;       /* set of events all people have interest in*/
    long your_event_mask;       /* my event mask */
    long do_not_propagate_mask; /* set of events that should not propagate */
    Bool override_redirect;     /* boolean value for override-redirect */
    Screen *screen;             /* back pointer to correct screen */
} XWindowAttributes;

typedef struct {
    Pixmap background_pixmap;   /* background or None or ParentRelative */
    unsigned long background_pixel;     /* background pixel */
    Pixmap border_pixmap;       /* border of the window */
    unsigned long border_pixel; /* border pixel value */
    int bit_gravity;            /* one of bit gravity values */
    int win_gravity;            /* one of the window gravity values */
    int backing_store;          /* NotUseful, WhenMapped, Always */
    unsigned long backing_planes;/* planes to be preserved if possible */
    unsigned long backing_pixel;/* value to use in restoring planes */
    Bool save_under;            /* should bits under be saved? (popups) */
    long event_mask;            /* set of events that should be saved */
    long do_not_propagate_mask; /* set of events that should not propagate */
    Bool override_redirect;     /* boolean value for override-redirect */
    Colormap colormap;          /* color map to be associated with window */
    Cursor cursor;              /* cursor to be displayed (or None) */
} XSetWindowAttributes;
#endif
#endif
