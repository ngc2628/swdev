
#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdlib.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Shell.h>
#include <X11/Intrinsic.h>
#include <X11/Xm/Xm.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#define buflen 1024
#define designator_off 10

extern unsigned int draw_fill;
extern unsigned int draw_wireframe;
extern unsigned int draw_normals;
extern unsigned int light_on;

extern int initialgeometry[4];
extern unsigned int bgmain;
extern unsigned int bgmenu;
extern unsigned int bgchk;
extern double simplepow10[10];

extern char strBuf1[buflen];
extern char strBuf2[buflen];
extern char strNum09[10][2];
extern XtAppContext applcontext;
extern Widget applshell;
extern Display *dpy;
extern Arg xxargs[32];

extern unsigned int run;
extern int runloop();
extern int initialize_statics();

#endif
