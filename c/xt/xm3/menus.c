
#include <xt/xm3/globals.h>
#include <xt/xm3/menus.h>
#include <X11/Xm/RowColumn.h>
#include <X11/Xm/CascadeB.h>
#include <X11/Xm/PushB.h>
#include <X11/Xm/FileSB.h>
#include <xt/xm3/edit.h>

static char strMenubar[]="mainmenu";
static char strFilemenu[]="file";
static char strHelpmenu[]="help";
static char strFileentry[]="File";
static char strOpenentry[]="Open";
static char strSaveentry[]="Save";
static char strQuitentry[]="Quit";
static char strHelpentry[]="Help";
static char strAboutentry[]="About";
static char strSaveasdialog[]="saveasfile";

unsigned int bgmain=(240<<16)|(250<<8)|240;
unsigned int bgmenu=(240<<16)|(240<<8)|240;

Widget menubar;
Widget filebox;

void createMenuBar(Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  menubar=XmCreateMenuBar(parent,strMenubar,xxargs,1);
  XtManageChild(menubar);

}

void createFileMenu() {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  Widget filemenu=XmCreatePulldownMenu(menubar,strFilemenu,xxargs,1);
  Widget entry=XmCreatePushButton(filemenu,strOpenentry,xxargs,1);
  XtAddCallback(entry,XmNactivateCallback,cbOpen,NULL);
  XtManageChild(entry);
  entry=XmCreatePushButton(filemenu,strSaveentry,xxargs,1);
  XtAddCallback(entry,XmNactivateCallback,cbSave,NULL);
  XtManageChild(entry);
  entry=XmCreatePushButton(filemenu,strQuitentry,xxargs,1);
  XtAddCallback(entry,XmNactivateCallback,cbQuit,NULL);
  XtManageChild(entry);
  XtSetArg(xxargs[1],XmNsubMenuId,filemenu);
  Widget filesub=XmCreateCascadeButton(menubar,strFileentry,xxargs,2);
  XtManageChild(filesub);

}

void createHelpMenu() {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  Widget helpmenu=XmCreatePulldownMenu(menubar,strHelpmenu,xxargs,1);
  Widget entry=XmCreatePushButton(helpmenu,strAboutentry,xxargs,1);
  XtAddCallback(entry,XmNactivateCallback,cbAbout,NULL);
  XtManageChild(entry);
  XtSetArg(xxargs[1],XmNsubMenuId,helpmenu);
  Widget helpsub=XmCreateCascadeButton(menubar,strHelpentry,xxargs,2);
  XtManageChild(helpsub);

}

void cbOpen(Widget ww,XtPointer pp1,XtPointer pp2) {

  XtAppSetExitFlag(applContext);

}

void cbSave(Widget w,XtPointer client,XtPointer data) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  filebox=XmCreateFileSelectionDialog(mainFrame,strSaveasdialog,xxargs,1);
  XtManageChild(filebox);


  const char *txt=XmTextGetString(editor);
  if (txt)
	  printf ("%d txt=%s\n",__LINE__,txt);
  printf ("%d w=%ld client=%ld data=%ld\n",__LINE__,(long)w,(long)client,(long)data);

}

void cbQuit(Widget ww,XtPointer pp1,XtPointer pp2) {

  XtAppSetExitFlag(applContext);

}

void cbAbout(Widget ww,XtPointer pp1,XtPointer pp2) {

  XtAppSetExitFlag(applContext);

}
