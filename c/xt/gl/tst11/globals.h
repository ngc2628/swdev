
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

const unsigned int designator_off=10;
const unsigned int draw_fill=(1<<designator_off);
const unsigned int draw_wireframe=(2<<designator_off);
const unsigned int draw_normals=(4<<designator_off);
const unsigned int light_on=(8<<designator_off);

const int buflen=1024;
const int initialgeometry[4]={400,200,1000,800};
const unsigned int bgmain=(240<<16)|(250<<8)|240;
const unsigned int bgmenu=(240<<16)|(240<<8)|240;
const unsigned int bgchk=(200<<16)|(200<<8)|50;
const double simplepow10[10]={
  1.,10.,100.,1000.,10000.,100000.,1000000.,10000000.,100000000.,1000000000.
};

extern char strBuf1[buflen];
extern char strBuf2[buflen];
extern char strNum09[10][2];
extern XtAppContext applcontext;
extern Widget applshell;
extern Display *dpy;
extern Arg xxargs[32];

extern unsigned int run;
extern int runloop();

#endif
