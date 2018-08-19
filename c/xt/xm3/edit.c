
#include <xt/xm3/globals.h>
#include <xt/xm3/edit.h>

Widget editor;

void createEditor(Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmain);
  XtSetArg(xxargs[1],XmNeditMode,XmMULTI_LINE_EDIT);
  editor=XmCreateText(parent,"editor",xxargs,2);
  XtManageChild(editor);

}



