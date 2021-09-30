
#include <stdio.h>

#include <xt/gl/tst11/statics.h>
#include <xt/gl/tst11/desktop.h>

static char strPrg[]="gltst11";

XtAppContext applcontext;
Widget applshell;
Display *dpy=0;
Arg xxargs[32];
unsigned int run=0;
char strBuf1[1024];
char strBuf2[1024];
char strNum09[10][2]={{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"}};

int main (int argc, char **argv) {

  initialize_statics();

  XtSetArg(xxargs[0],XtNwidth,initialgeometry[2]);
  XtSetArg(xxargs[1],XtNheight,initialgeometry[3]);
  XtSetArg(xxargs[2],XtNx,(Position)initialgeometry[0]);
  XtSetArg(xxargs[3],XtNy,(Position)initialgeometry[1]);
  applshell=XtAppInitialize(&applcontext,strPrg,
    (XrmOptionDescRec*)NULL,0,&argc,argv,(char**)NULL,xxargs,4);
  dpy=XtDisplay(applshell);

  // get visuals
  if (setupGl()!=0)
    return 1;

  // no glw - set visual, depth, colormap on top-level
  XtSetArg(xxargs[0],XtNvisual,visualinfo->visual);
  XtSetArg(xxargs[1],XtNdepth,visualinfo->depth);
  XtSetArg(xxargs[2],XtNcolormap,colormap);
  XtSetValues(applshell,xxargs,3);

  createDesktop(applshell);
  XtRealizeWidget(applshell);

  realizeGl();
  run=1;
  int res=runloop();

  return res;

}
