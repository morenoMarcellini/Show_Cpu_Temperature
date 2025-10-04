#ifndef SHOWTEMPERATURE
#define SHOWTEMPERATURE
/* Xcms */
//#include <X11/Xcms.h> /* For XcmsColor */

struct WindowTopology {
    int pos_x;
    int pos_y;
    int size_x;
    int size_y;
    int border_size;
};

typedef struct WindowTopology WindowTopology;

struct ArcAttr {
    int rad_x;
    int rad_y;
    int three;
    int length;
};
#endif
