
#include <xt/gl/tst11/globals.h>
#include <xt/gl/tst11/form.h>
#include <xt/gl/tst11/dialogs.h>
#include <xt/gl/tst11/glw.h>
#include <xt/gl/tst11/objects.h>
#include <stdio.h>

static char strMainFrame[]="mainframe";
static char strLeftFrame[]="leftframe";
static char strTopFrame[]="topframe";
static char strRightFrame[]="rightframe";
static char strBottomFrame[]="bottomframe";
static char strSelect[4][10]={
{"translate"},{"rotate"},{"scale"},{"shear"}
};
static char strX[]="X";
static char strY[]="Y";
static char strZ[]="Z";
static char strDrawFrame[]="drawframe";
static char strLightButton[]="lightbutton";
static char strDum[]="dum";
static int selectidx=-1;

static Widget selectorbutton_w[4];
Widget mainframe_w,drawframe_w,lightbutton_w,xmod_w,ymod_w,zmod_w;

Widget createMainFrame(Widget parent) {

  mainframe_w=XmCreateForm(parent,strMainFrame,(Arg*)0,0);
  XtManageChild(mainframe_w);

  XtSetArg(xxargs[0],XmNresizable,TRUE);
  XtSetArg(xxargs[1],XmNorientation,XmVERTICAL);
  XtSetArg(xxargs[2],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[4],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[5],XmNleftOffset,5);
  XtSetArg(xxargs[6],XmNtopOffset,30);
  XtSetArg(xxargs[7],XmNbottomOffset,30);
  XtSetArg(xxargs[8],XmNfractionBase,3);
  Widget leftframe=XmCreateForm(mainframe_w,strLeftFrame,xxargs,9);
  XtManageChild(leftframe);

  XtSetArg(xxargs[1],XmNorientation,XmVERTICAL);
  XtSetArg(xxargs[2],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[4],XmNtopPosition,1);
  XtSetArg(xxargs[5],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[6],XmNbottomPosition,2);
  Widget but1=XmCreateToggleButton(leftframe,strDum,xxargs,7);
  XtManageChild(but1);

  XtSetArg(xxargs[0],XmNresizable,TRUE);
  XtSetArg(xxargs[1],XmNorientation,XmHORIZONTAL);
  XtSetArg(xxargs[2],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[4],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[5],XmNleftOffset,5);
  XtSetArg(xxargs[6],XmNtopOffset,5);
  XtSetArg(xxargs[7],XmNrightOffset,5);
  XtSetArg(xxargs[8],XmNfractionBase,3);
  Widget topframe=XmCreateForm(mainframe_w,strTopFrame,xxargs,9);
  XtManageChild(topframe);

  XtSetArg(xxargs[1],XmNorientation,XmHORIZONTAL);
  XtSetArg(xxargs[2],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNleftAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[4],XmNleftPosition,1);
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[6],XmNrightPosition,2);
  lightbutton_w=XmCreatePushButton(topframe,strLightButton,xxargs,7);
  XtManageChild(lightbutton_w);

  XtSetArg(xxargs[1],XmNorientation,XmVERTICAL);
  XtSetArg(xxargs[2],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[4],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[5],XmNrightOffset,5);
  XtSetArg(xxargs[6],XmNtopOffset,30);
  XtSetArg(xxargs[7],XmNbottomOffset,30);
  XtSetArg(xxargs[8],XmNfractionBase,3);
  Widget rightframe=XmCreateForm(mainframe_w,strRightFrame,xxargs,9);
  XtManageChild(rightframe);

  XtSetArg(xxargs[1],XmNorientation,XmVERTICAL);
  XtSetArg(xxargs[2],XmNshowArrows,XmEACH_SIDE);
  XtSetArg(xxargs[3],XmNshowValue,TRUE);
  XtSetArg(xxargs[4],XmNtitleString,XmStringCreateSimple(strX));
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[6],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[7],XmNtopPosition,0);
  XtSetArg(xxargs[8],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[9],XmNbottomPosition,1);
  xmod_w=XmCreateScale(rightframe,strX,xxargs,10);
  XtAddCallback(xmod_w,XmNdragCallback,cbModView,(void*)0);
  XtAddCallback(xmod_w,XmNvalueChangedCallback,cbModView,(void*)0);
  XtManageChild(xmod_w);

  XtSetArg(xxargs[4],XmNtitleString,XmStringCreateSimple(strY));
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[6],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[7],XmNtopPosition,1);
  XtSetArg(xxargs[8],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[9],XmNbottomPosition,2);
  ymod_w=XmCreateScale(rightframe,strY,xxargs,10);
  XtAddCallback(ymod_w,XmNdragCallback,cbModView,(void*)0);
  XtAddCallback(ymod_w,XmNvalueChangedCallback,cbModView,(void*)0);
  XtManageChild(ymod_w);

  XtSetArg(xxargs[4],XmNtitleString,XmStringCreateSimple(strZ));
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[6],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[7],XmNtopPosition,2);
  XtSetArg(xxargs[8],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[9],XmNbottomPosition,3);
  zmod_w=XmCreateScale(rightframe,strZ,xxargs,10);
  XtAddCallback(zmod_w,XmNdragCallback,cbModView,(void*)0);
  XtAddCallback(zmod_w,XmNvalueChangedCallback,cbModView,(void*)0);
  XtManageChild(zmod_w);

  XtSetArg(xxargs[1],XmNorientation,XmHORIZONTAL);
  XtSetArg(xxargs[2],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[4],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[5],XmNleftOffset,30);
  XtSetArg(xxargs[6],XmNrightOffset,30);
  XtSetArg(xxargs[7],XmNbottomOffset,5);
  XtSetArg(xxargs[8],XmNfractionBase,4);
  Widget bottomframe=XmCreateForm(mainframe_w,strBottomFrame,xxargs,9);
  XtManageChild(bottomframe);

  int ii=0;
  for (ii=0;ii<4;ii++) {
    XtSetArg(xxargs[1],XmNorientation,XmHORIZONTAL);
    XtSetArg(xxargs[2],XmNbottomAttachment,XmATTACH_FORM);
    XtSetArg(xxargs[3],XmNleftAttachment,XmATTACH_POSITION);
    XtSetArg(xxargs[4],XmNleftPosition,ii);
    XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_POSITION);
    XtSetArg(xxargs[6],XmNrightPosition,ii+1);
    selectorbutton_w[ii]=XmCreateToggleButton(bottomframe,strSelect[ii],xxargs,7);
    XtAddCallback(selectorbutton_w[ii],XmNvalueChangedCallback,cbModSel,NULL);
    XtManageChild(selectorbutton_w[ii]);
  }
  selectidx=0;
  XtSetArg(xxargs[0],XmNset,1);
  XtSetValues(selectorbutton_w[0],xxargs,1);
  XtSetArg(xxargs[0],XmNminimum,-50);
  XtSetArg(xxargs[1],XmNmaximum,50);
  XtSetArg(xxargs[2],XmNvalue,transformV[selectidx].xyzw[0]);
  XtSetValues(xmod_w,xxargs,3);
  XtSetArg(xxargs[2],XmNvalue,transformV[selectidx].xyzw[1]);
  XtSetValues(ymod_w,xxargs,3);
  XtSetArg(xxargs[0],XmNminimum,-2500);
  XtSetArg(xxargs[1],XmNmaximum,10);
  XtSetArg(xxargs[2],XmNvalue,transformV[selectidx].xyzw[2]);
  XtSetValues(zmod_w,xxargs,3);

  XtSetArg(xxargs[0],XtNbackground,bgchk);
  XtSetArg(xxargs[1],XmNleftAttachment,XmATTACH_WIDGET);
  XtSetArg(xxargs[2],XmNleftWidget,leftframe);
  XtSetArg(xxargs[3],XmNtopAttachment,XmATTACH_WIDGET);
  XtSetArg(xxargs[4],XmNtopWidget,topframe);
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_WIDGET);
  XtSetArg(xxargs[6],XmNrightWidget,rightframe);
  XtSetArg(xxargs[7],XmNbottomAttachment,XmATTACH_WIDGET);
  XtSetArg(xxargs[8],XmNbottomWidget,bottomframe);
  drawframe_w=XmCreateFrame(mainframe_w,strDrawFrame,xxargs,9);
  XtManageChild(drawframe_w);

  return drawframe_w;

}

void cbModSel(Widget ww,XtPointer,XtPointer data) {

  XmToggleButtonCallbackStruct *tbdata=(XmToggleButtonCallbackStruct*)data;
  //XmCR_VALUE_CHANGED

  char selector[10]={0,0,0,0,0,0,0,0,0,0};
  strncpy(selector,XtName(ww),9);
  int ii=0,idx=-1;
  for (ii=0;ii<4;ii++) {
    if (strcmp(selector,strSelect[ii])==0) {
      idx=ii;
      break;
    }
  }
  if (idx<0)
    return;

  if (tbdata->set==0) {
    if (idx==selectidx) {
      XtSetArg(xxargs[0],XmNset,1);
      XtSetValues(ww,xxargs,1);
    }
    return;
  }

  selectidx=idx;

  XtSetArg(xxargs[0],XmNset,0);
  XtSetValues(selectorbutton_w[(selectidx+1)%4],xxargs,1);
  XtSetValues(selectorbutton_w[(selectidx+2)%4],xxargs,1);
  XtSetValues(selectorbutton_w[(selectidx+3)%4],xxargs,1);

  if (selectidx==idxtr) {
    XtSetArg(xxargs[0],XmNminimum,-50);
    XtSetArg(xxargs[1],XmNmaximum,50);
  }
  else if (selectidx==idxrot) {
    XtSetArg(xxargs[0],XmNminimum,0);
    XtSetArg(xxargs[1],XmNmaximum,360);
  }
  else if (selectidx==idxsc) {
    XtSetArg(xxargs[0],XmNminimum,1);
    XtSetArg(xxargs[1],XmNmaximum,20);
  }
  else if (selectidx==idxsh) {
    XtSetArg(xxargs[0],XmNminimum,0);
    XtSetArg(xxargs[1],XmNmaximum,100);
  }

  XtSetArg(xxargs[2],XmNvalue,transformV[selectidx].xyzw[0]);
  XtSetValues(xmod_w,xxargs,3);
  XtSetArg(xxargs[2],XmNvalue,transformV[selectidx].xyzw[1]);
  XtSetValues(ymod_w,xxargs,3);
  if (selectidx==idxtr) {
    XtSetArg(xxargs[0],XmNminimum,-2500);
    XtSetArg(xxargs[1],XmNmaximum,10);
  }
  XtSetArg(xxargs[2],XmNvalue,transformV[selectidx].xyzw[2]);
  XtSetValues(zmod_w,xxargs,3);

}

void cbModView(Widget ww,XtPointer,XtPointer data) {

  XmScaleCallbackStruct *scdata=(XmScaleCallbackStruct*)data;

  // XmCR_VALUE_CHANGED, XmCR_DRAG
  if (scdata->reason==XmCR_VALUE_CHANGED &&
        scdata->event && scdata->event->type==ButtonRelease)
    return;

  if (strcmp(XtName(ww),strX)==0) {
    transformV[selectidx].xyzw[0]=(double)scdata->value;
  }
  else if (strcmp(XtName(ww),strY)==0) {
    transformV[selectidx].xyzw[1]=(double)scdata->value;
  }
  else if (strcmp(XtName(ww),strZ)==0) {
    transformV[selectidx].xyzw[2]=(double)scdata->value;
  }

  redrawGl(glwidget_w,(void*)0,(void*)0);

}

