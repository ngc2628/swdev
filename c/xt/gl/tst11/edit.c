
#include <xt/gl/tst11/globals.h>
#include <xt/gl/tst11/edit.h>

static char strEditor[]="editor";

Widget editor;

void createEditor(Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmain);
  XtSetArg(xxargs[1],XmNeditMode,XmMULTI_LINE_EDIT);
  editor=XmCreateText(parent,strEditor,xxargs,2);
  XtManageChild(editor);

}
