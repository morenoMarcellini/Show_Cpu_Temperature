#ifndef XPMDEFINITION
#define XPMDEFINITION

#include "X11/Xlib.h"

#define XPM_BLACK	0
#define XPM_WHITE	1
#define XPM_RED		2	
#define XPM_GREEN	3
#define XPM_BLUE	4
#define XPM_CYAN	5
#define XPM_YELLOW	6
#define XPM_MAGENTA	7

/* XPM */
static char * rainbow[] = {
    "8 1 8 1",  /* Width 8, Height 1, 8 colors, 1 char/pixel */
    "a c #000000", /* 'a' represents a black pixel */
    "b c #FFFFFF", /* 'b' represents a black pixel */
    "c c #FF0000", /* 'c' represents a red pixel */
    "d c #00FF00", /* 'd' represents a green pixel */
    "e c #0000FF", /* 'd' represents a blue pixel */
    "f c #00FFFF", /* 'f' represents a cyan pixel */
    "g c #FF00FF", /* 'g' represents a yellow pixel */
    "h c #FFFF00", /* 'h' represents a magenta pixel */
    "abcdefgh" 	   /* First row: all white */
};


/* XPM */
static char * black_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #000000", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};

static char * white_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #FFFFFF", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};

static char * red_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 2 char/pixel */
    "b c #FF0000", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};

static char * green_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 2 char/pixel */
    "b c #00FF00", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};

static char * blue_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #0000FF", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};


static char * cyan_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #00FFFF", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};


static char * yellow_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #FFFF00", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};


static char * magenta_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #FF00FF", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};

/* EXAMPLE */
/* XPM */
static char * my_image[] = {
    "4 4 2 1",  /* Width 4, Height 4, 2 colors, 1 char/pixel */
    "a c #FFFFFF", /* 'a' represents a white pixel */
    "b c #000000", /* 'b' represents a black pixel */
    "aaaa",  /* First row: all white */
    "abba",  /* Second row: white, black, black, white */
    "abba",  /*/usr/share/emacs/28.2/etc/images/splash.xpm Third row: white, black, black, white */
    "aaaa"   /* Fourth row: all white */
};

extern Pixmap create_rainbow_pixmap();
extern XImage* create_rainbow_ximage();

#endif
