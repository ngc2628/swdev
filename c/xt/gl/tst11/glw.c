
#include <xt/gl/tst11/glw.h>
#include <xt/gl/tst11/lights.h>
#include <xt/gl/tst11/objects.h>
#include <X11/Xm/DrawingA.h>
#include <stdio.h>

static char strGlwidget[]="glwidget";

XVisualInfo *visualinfo=0;
GLXContext glcontext;
Colormap colormap;
Widget glwidget_w;

mk_vertex transformV[4]={
  {.0,.0,-50.,.0},{.0,.0,.0,.0},{5.,5.,5.,.0},{.0,.0,.0,.0}
};

unsigned int drawstyle;

int setupGl() {

  GLint glvisualattr[5]={GLX_RGBA,GLX_DEPTH_SIZE,24,GLX_DOUBLEBUFFER,None};
  visualinfo=glXChooseVisual(dpy,DefaultScreen(dpy),glvisualattr);
  glcontext=glXCreateContext(dpy,visualinfo,0,GL_TRUE);
  colormap=XCreateColormap(dpy,RootWindow(dpy,visualinfo->screen),visualinfo->visual,AllocNone);

  return 0;

}

void createGl(Widget parent) {

  glwidget_w=XtCreateManagedWidget(strGlwidget,xmDrawingAreaWidgetClass,parent,(Arg*)0,0);

}

static void flushGl() {

  glFlush();
  glXSwapBuffers(dpy,XtWindow(glwidget_w));
  if(!glXIsDirect(dpy,glcontext))
    glFinish();

}

static void clearGl() {

  glClearDepth(1.0);
  glClearColor(0.26,0.28,0.24,0.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

}

void redrawGl(Widget ww,XtPointer userdata,XtPointer data) {

  glEnable(GL_DEPTH_TEST);
  clearGl();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  light_conf();
  draw_obj();
  flushGl();

}

void realizeGl() {

  glXMakeCurrent(dpy,XtWindow(glwidget_w),glcontext);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(-1.0,1.0,-1.0,1.0,1.,100.);
  gluPerspective(25.0,(float)initialgeometry[2]/(float)initialgeometry[3],1.,2500.);
  glMatrixMode(GL_MODELVIEW);
  redrawGl(glwidget_w,(void*)0,(void*)0);
  XtAddCallback(glwidget_w,XmNexposeCallback,(XtCallbackProc)redrawGl,(void*)0);
  XtAddCallback(glwidget_w,XmNresizeCallback,(XtCallbackProc)resizeGl,(void*)0);

  long inputs=ButtonPressMask|ButtonReleaseMask|PointerMotionMask|
              ButtonMotionMask|ExposureMask;
  XSelectInput(dpy,XtWindow(glwidget_w),inputs);

}

void resizeGl(Widget ww,XtPointer userdata,XtPointer data) {

  Dimension width=0,height=0;
  XtSetArg(xxargs[0],XmNwidth,&width);
  XtSetArg(xxargs[1],XmNheight,&height);
  XtGetValues(ww,xxargs,2);
  glXWaitX();
//printf ("%d resize [%d,%d]\n",__LINE__,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(-1.0,1.0,-1.0,1.0,1.,100.);
  gluPerspective(25.0,(float)width/(float)height,1.,2500.);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport(0,0,(GLint)width,(GLint)height);

}
