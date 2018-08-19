
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Shell.h>
#include <X11/Intrinsic.h>
#include <X11/Xm/MainW.h>
#include <xt/xm3/menus.h>
#include <xt/xm3/edit.h>
#include <xt/xm3/globals.h>

XtAppContext applContext;
Display *applDisp=0;
Widget mainFrame;
Arg xxargs[32];

int main (int argc, char **argv) {

  XtSetArg(xxargs[0],XtNwidth,800);
  XtSetArg(xxargs[1],XtNheight,600);
  XtSetArg(xxargs[2],XtNx,(Position)400);
  XtSetArg(xxargs[3],XtNy,(Position)200);
	Widget applShell=XtAppInitialize(&applContext,
                            "xm3",
                            (XrmOptionDescRec*)NULL,
                            0,
                            &argc,argv,
                            (char**)NULL,
                            xxargs,4);
  applDisp=XtDisplay(applShell);
  XtSetArg(xxargs[0],XtNbackground,bgmain);
  mainFrame=XtCreateManagedWidget("mainframe",xmMainWindowWidgetClass,applShell,xxargs,1);
  createMenuBar(mainFrame);
  createFileMenu();
  createHelpMenu();
  createEditor(mainFrame);
  XtRealizeWidget(applShell);
  XtAppMainLoop(applContext);

  return 0;

}
