#ifndef XPMDEFINITION
#define XPMDEFINITION
/* XPM */
static char * black_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #000000", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};

static char * red_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 2 char/pixel */
    "b c #FF0000", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};

static char * blue_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #0000FF", /* 'b' represents a black pixel */
    "bb",  /* First row: all white */
    "bb"  /* Second row: white, black, black, white */
};

static char * white_dot[] = {
    "2 2 1 1",  /* Width 2, Height 2, 1 colors, 1 char/pixel */
    "b c #FFFFFF", /* 'b' represents a black pixel */
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

#endif
