
#include <xt/gl/tst11/desktop.h>
#include <X11/Xm/MainW.h>

static char strMainwidget[]="mainwidget";

Widget mainwidget_w=0;

void createDesktop(Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmain);
  mainwidget_w=XtCreateManagedWidget(strMainwidget,xmMainWindowWidgetClass,parent,xxargs,1);
  XtManageChild(mainwidget_w);
  createMenus(mainwidget_w);
  createMainFrame(mainwidget_w);
  createGl(drawframe_w);

}


