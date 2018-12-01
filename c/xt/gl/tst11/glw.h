
#ifndef GLW_H
#define GLW_H

#include <xt/gl/tst11/statics.h>
#include <mkbase/mkla.h>

#define idxtr 0
#define idxrot 1
#define idxsc 2
#define idxsh 3
#define transform_n 4

extern mk_vertex transformV[transform_n];
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
