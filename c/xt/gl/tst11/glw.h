
#ifndef GLW_H
#define GLW_H

#include <xt/gl/tst11/globals.h>
#include <mkbase/mkbase.h>

const int idxtr=0;
const int idxrot=1;
const int idxsc=2;
const int idxsh=3;
const int transform_n=4;

extern struct mk_vertex transformV[transform_n];
extern unsigned int drawstyle;

extern XVisualInfo *visualinfo;
extern GLXContext glcontext;
extern Colormap colormap;
extern Widget glwidget_w;

extern int setupGl();
extern void createGl(Widget);
extern void realizeGl();
extern void resizeGl(Widget,XtPointer,XtPointer);
extern void redrawGl(Widget,XtPointer,XtPointer);

#endif
