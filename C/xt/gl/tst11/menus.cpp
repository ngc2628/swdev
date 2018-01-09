
#include <X11/Xm/RowColumn.h>
#include <X11/Xm/CascadeB.h>
#include <X11/Xm/PushB.h>
#include <X11/Xm/ToggleB.h>
#include <X11/Xm/FileSB.h>

#include <xt/gl/tst11/globals.h>
#include <xt/gl/tst11/desktop.h>
#include <xt/gl/tst11/menus.h>
#include <xt/gl/tst11/edit.h>
#include <xt/gl/tst11/dialogs.h>
#include <xt/gl/tst11/glw.h>
#include <xt/gl/tst11/objects.h>
#include <xt/gl/tst11/lights.h>

#include <stdio.h>

static char strMenubar[]="mainmenu";

static char strFile[]="file";
static char strFileentry[]="File";
static char strOpenentry[]="Open";
static char strSaveentry[]="Save";
static char strSaveasdialog[]="saveasfile";
static char strQuitentry[]="Quit";

static char strObjects[]="objects";
static char strObjectsentry[]="Objects";
static char strSphere[]="sphere";
static char strSphereentry[]="Sphere";
static char strSpheresubentry[][7]={{"off"},{"20"},{"80"},{"320"},{"1280"},{"5120"},{"20480"}};
static char strCube[]="cube";
static char strCubeentry[]="Cube";

static char strDraw[]="draw";
static char strDrawentry[]="Draw Style";
static char strFillentry[]="Fill";
static char strWireframeentry[]="Wireframe";
static char strNormalsentry[]="Normals";

static char strLights[]="lights";
static char strLightsentry[]="Lights";
static char strLightsonentry[]="on";

static char strHelp[]="help";
static char strHelpentry[]="Help";
static char strAboutentry[]="About";


static void cbOpen(Widget,XtPointer,XtPointer) {


}

static void cbSave(Widget ww,XtPointer clientdata,XtPointer data) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  Widget filebox=XmCreateFileSelectionDialog((Widget)clientdata,strSaveasdialog,xxargs,1);
  XtManageChild(filebox);

  const char *txt=0;//XmTextGetString(editor);
  if (txt)
    printf ("%d txt=%s\n",__LINE__,txt);
  printf ("%d w=%ld client=%ld data=%ld\n",__LINE__,(long)ww,(long)clientdata,(long)data);

}

static void cbQuit(Widget,XtPointer,XtPointer) {

  run=0;

}

static void cbObjects(Widget ww,XtPointer userdata,XtPointer data) {

  XmPushButtonCallbackStruct *pbdata=0;
  XmToggleButtonCallbackStruct *tbdata=0;

  if (userdata==(void*)&(strSphere[0])) {
    pbdata=(XmPushButtonCallbackStruct*)data;
    sphere_level=atoi(XtName(ww));
    redrawGl(glwidget_w,(void*)0,(void*)0);
  }
  else if (userdata==(void*)&(strCube[0])) {
    tbdata=(XmToggleButtonCallbackStruct*)data;
    cube_level=tbdata->set;
    redrawGl(glwidget_w,(void*)0,(void*)0);
  }

}

static void cbDrawstyle(Widget ww,XtPointer,XtPointer data) {

  XmToggleButtonCallbackStruct *tbdata=(XmToggleButtonCallbackStruct*)data;

  if (strcmp(XtName(ww),strFillentry)==0) {
    if (tbdata->set==1)
      drawstyle|=draw_fill;
    else
      drawstyle&=(~draw_fill);
    redrawGl(glwidget_w,(void*)0,(void*)0);
  }
  else if (strcmp(XtName(ww),strWireframeentry)==0) {
    if (tbdata->set==1)
      drawstyle|=draw_wireframe;
    else
      drawstyle&=(~draw_wireframe);
    redrawGl(glwidget_w,(void*)0,(void*)0);
  }
  else if (strcmp(XtName(ww),strNormalsentry)==0) {
    if (tbdata->set==1)
      drawstyle|=draw_normals;
    else
      drawstyle&=(~draw_normals);
    redrawGl(glwidget_w,(void*)0,(void*)0);
  }

}

static void cbLight(Widget ww,XtPointer,XtPointer data) {

  XmPushButtonCallbackStruct *pbdata=0;
  XmToggleButtonCallbackStruct *tbdata=0;

  if (strcmp(XtName(ww),strLightsonentry)==0) {
    tbdata=(XmToggleButtonCallbackStruct*)data;
    if (tbdata->set==1)
      glEnable(GL_LIGHTING);
    else
      glDisable(GL_LIGHTING);
    redrawGl(glwidget_w,(void*)0,(void*)0);
    return;
  }
  int ii=0;
  for (ii=0;ii<light_n;ii++) {
    if (strcmp(XtName(ww),strNum09[ii])==0) {
      pbdata=(XmPushButtonCallbackStruct*)data;
      createLightDialog(mainwidget_w,ii);
      break;
    }
  }

}

static void cbAbout(Widget,XtPointer,XtPointer) {



}

static void createFileMenu(Widget menubar,Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  XtSetArg(xxargs[1],XmNvisual,visualinfo->visual);
  Widget filemenu=XmCreatePulldownMenu(menubar,strFile,xxargs,2);
  Widget entry=XmCreatePushButton(filemenu,strOpenentry,xxargs,1);
  XtAddCallback(entry,XmNactivateCallback,cbOpen,NULL);
  XtManageChild(entry);
  entry=XmCreatePushButton(filemenu,strSaveentry,xxargs,1);
  XtAddCallback(entry,XmNactivateCallback,cbSave,(XtPointer)parent);
  XtManageChild(entry);
  entry=XmCreatePushButton(filemenu,strQuitentry,xxargs,1);
  XtAddCallback(entry,XmNactivateCallback,cbQuit,NULL);
  XtManageChild(entry);
  XtSetArg(xxargs[1],XmNsubMenuId,filemenu);
  Widget filesub=XmCreateCascadeButton(menubar,strFileentry,xxargs,2);
  XtManageChild(filesub);

}

static void createObjectsMenu(Widget menubar,Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  XtSetArg(xxargs[1],XmNvisual,visualinfo->visual);
  Widget objectsmenu=XmCreatePulldownMenu(menubar,strObjects,xxargs,2);
  Widget spheremenu=XmCreatePulldownMenu(objectsmenu,strSphere,xxargs,2);
  Widget entry;
  int ii=0;
  for (ii=0;ii<7;ii++) {
    XtSetArg(xxargs[1],XmNlabelString,XmStringCreateSimple(strSpheresubentry[ii]));
    entry=XmCreatePushButton(spheremenu,strNum09[ii],xxargs,2);
    XtAddCallback(entry,XmNactivateCallback,cbObjects,(void*)(&strSphere[0]));
    XtManageChild(entry);
  }
  XtSetArg(xxargs[1],XmNsubMenuId,spheremenu);
  Widget spheresub=XmCreateCascadeButton(objectsmenu,strSphereentry,xxargs,2);
  XtManageChild(spheresub);
  entry=XmCreateToggleButton(objectsmenu,strCubeentry,xxargs,1);
  XtAddCallback(entry,XmNvalueChangedCallback,cbObjects,(void*)(&strCube[0]));
  XtManageChild(entry);
  XtSetArg(xxargs[1],XmNsubMenuId,objectsmenu);
  Widget objectssub=XmCreateCascadeButton(menubar,strObjectsentry,xxargs,2);
  XtManageChild(objectssub);

}

static void createDrawMenu(Widget menubar,Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  XtSetArg(xxargs[1],XmNvisual,visualinfo->visual);
  Widget drawmenu=XmCreatePulldownMenu(menubar,strDraw,xxargs,2);
  Widget entry=XmCreateToggleButton(drawmenu,strFillentry,xxargs,0);
  XtSetArg(xxargs[0],XmNset,1);
  XtSetValues(entry,xxargs,1);
  XtAddCallback(entry,XmNvalueChangedCallback,cbDrawstyle,NULL);
  XtManageChild(entry);
  entry=XmCreateToggleButton(drawmenu,strWireframeentry,xxargs,0);
  XtSetValues(entry,xxargs,1);
  XtAddCallback(entry,XmNvalueChangedCallback,cbDrawstyle,(XtPointer)parent);
  XtManageChild(entry);
  entry=XmCreateToggleButton(drawmenu,strNormalsentry,xxargs,0);
  XtAddCallback(entry,XmNvalueChangedCallback,cbDrawstyle,(XtPointer)parent);
  XtManageChild(entry);
  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  XtSetArg(xxargs[1],XmNsubMenuId,drawmenu);
  Widget filesub=XmCreateCascadeButton(menubar,strDrawentry,xxargs,2);
  XtManageChild(filesub);

}

static void createLightsMenu(Widget menubar,Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  XtSetArg(xxargs[1],XmNvisual,visualinfo->visual);
  Widget lightsmenu=XmCreatePulldownMenu(menubar,strLights,xxargs,2);
  Widget entry=XmCreateToggleButton(lightsmenu,strLightsonentry,xxargs,1);
  XtAddCallback(entry,XmNvalueChangedCallback,cbLight,(void*)0);
  XtManageChild(entry);
  unsigned char ison[1]={0};
  glGetBooleanv(GL_LIGHTING,&ison[0]);
  if (ison[0]) {
    XtSetArg(xxargs[0],XmNset,1);
    XtSetValues(entry,xxargs,1);
  }
  int ii=0;
  for (ii=0;ii<8;ii++) {
    entry=XmCreatePushButton(lightsmenu,strNum09[ii],xxargs,1);
    XtAddCallback(entry,XmNactivateCallback,cbLight,(void*)0);
    XtManageChild(entry);
  }
  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  XtSetArg(xxargs[1],XmNsubMenuId,lightsmenu);
  Widget filesub=XmCreateCascadeButton(menubar,strLightsentry,xxargs,2);
  XtManageChild(filesub);

}

static void createHelpMenu(Widget menubar,Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  XtSetArg(xxargs[1],XmNvisual,visualinfo->visual);
  Widget helpmenu=XmCreatePulldownMenu(menubar,strHelp,xxargs,2);
  Widget entry=XmCreatePushButton(helpmenu,strAboutentry,xxargs,1);
  XtAddCallback(entry,XmNactivateCallback,cbAbout,(void*)0);
  XtManageChild(entry);
  XtSetArg(xxargs[1],XmNsubMenuId,helpmenu);
  Widget helpsub=XmCreateCascadeButton(menubar,strHelpentry,xxargs,2);
  XtManageChild(helpsub);

}

void createMenus(Widget parent) {

  XtSetArg(xxargs[0],XtNbackground,bgmenu);
  Widget menubar=XmCreateMenuBar(parent,strMenubar,xxargs,1);
  XtManageChild(menubar);
  createFileMenu(menubar,parent);
  createObjectsMenu(menubar,parent);
  createDrawMenu(menubar,parent);
  createLightsMenu(menubar,parent);
  createHelpMenu(menubar,parent);

}
