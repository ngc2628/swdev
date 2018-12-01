
#include <xt/gl/tst11/statics.h>
#include <xt/gl/tst11/dialogs.h>
#include <xt/gl/tst11/lights.h>
#include <xt/gl/tst11/glw.h>
#include <stdio.h>
#include <ctype.h>

static char strLightDlg[]="lightdlg";
static char strLightFrame[]="lightframe";
static char strOn[]="On";
static char strNone[]="None";
static char strNatureFrame[]="natureframe";
static char strNatureComboFrame[]="naturecomboframe";
static char strMaterialFrame[]="materialframe";
static char strMaterialComboFrame[]="materialcomboframe";
static char strButtonFrame[]="buttonframe";
static char strNature[]="Type";
static char strMaterial[]="Material";
static char strNatureColor[]="naturecolor";
static char strMaterialColor[]="materialcolor";
static char strAlpha[]="alpha";
static char strRed[]="red";
static char strGreen[]="green";
static char strBlue[]="blue";
static char strConfirmLightDlg[]="Confirm";
static char strQuitLightDlg[]="Quit";

static int spin_dec_off=3;

static XmString xms;
static XmString xmsL[lightspec_n];

Widget lightdlg_w[light_n]={0,0,0,0,0,0,0,0},lighton_w[light_n]={0,0,0,0,0,0,0,0},
  lightnature_w[light_n]={0,0,0,0,0,0,0,0},naturecolor_w[light_n]={0,0,0,0,0,0,0,0},
  material_w[light_n]={0,0,0,0,0,0,0,0},materialcolor_w[light_n]={0,0,0,0,0,0,0,0},
  naturealpha_w[light_n]={0,0,0,0,0,0,0,0},naturered_w[light_n]={0,0,0,0,0,0,0,0},
  naturegreen_w[light_n]={0,0,0,0,0,0,0,0},natureblue_w[light_n]={0,0,0,0,0,0,0,0},
  materialalpha_w[light_n]={0,0,0,0,0,0,0,0},materialred_w[light_n]={0,0,0,0,0,0,0,0},
  materialgreen_w[light_n]={0,0,0,0,0,0,0,0},materialblue_w[light_n]={0,0,0,0,0,0,0,0};

static void adjustRgba() {


}

static void cbLightDlgQuit(Widget ww,XtPointer userdata,XtPointer data) {

  int lightidx=(int)((long)userdata);
  XtDestroyWidget(lightdlg_w[lightidx]);
  lightdlg_w[lightidx]=0;
  lighton_w[lightidx]=0;
  lightnature_w[lightidx]=0;
  naturecolor_w[lightidx]=0;
  material_w[lightidx]=0;
  materialcolor_w[lightidx]=0;
  naturealpha_w[lightidx]=0;
  naturered_w[lightidx]=0;
  naturegreen_w[lightidx]=0;
  natureblue_w[lightidx]=0;
  materialalpha_w[lightidx]=0;
  materialred_w[lightidx]=0;
  materialgreen_w[lightidx]=0;
  materialblue_w[lightidx]=0;

}

static void cbLightDlgConfirm(Widget ww,XtPointer userdata,XtPointer data) {

  int lightidx=(int)((long)userdata);
  lightL[lightidx]=wrklightL[lightidx];

  redrawGl(glwidget_w,(void*)0,(void*)0);

}

static void cbLightOn(Widget ww,XtPointer userdata,XtPointer data) {

  int lightidx=(int)((long)userdata);
  struct LightSource *lightsource=&(wrklightL[lightidx]);
  XmToggleButtonCallbackStruct *tbdata=(XmToggleButtonCallbackStruct*)data;
  if (tbdata->set==1)
    lightsource->m_on|=light_on;
  else
    lightsource->m_on&=(~light_on);

}

static void spinValue(int lightidx,Widget ww,double value) {

  if (lightidx<0 || lightidx>=light_n)
    return;
  struct LightSource *lightsource=&(wrklightL[lightidx]);

  char *str=XtName(XtParent(XtParent(ww)));
  memset(&strBuf1[0],0,buflen);
  if (str)
    strcpy(strBuf1,str);
  int cmpnat=strcmp(strBuf1,strNatureFrame),cmpmat=strcmp(strBuf1,strMaterialFrame);
  if (cmpnat!=0 && cmpmat!=0)
    return;

  XtSetArg(xxargs[0],XmNselectedItem,&xms);
  XtGetValues(cmpnat==0 ? lightnature_w[lightidx] : material_w[lightidx],xxargs,1);
  XmStringGetLtoR(xms,XmSTRING_DEFAULT_CHARSET,&str);
  memset(&strBuf1[0],0,buflen);
  strncpy(strBuf1,str,buflen-1);
  XmStringFree(xms);

  struct LightSpec *rgba=
    (cmpnat==0 ? find_nature(strBuf1,lightsource,0) : find_material(strBuf1,lightsource,0));

  if (!rgba)
    return;

  if (strcmp(XtName(ww),strAlpha)==0)
    rgba->m_value[3]=value;
  else if (strcmp(XtName(ww),strRed)==0)
    rgba->m_value[0]=value;
  else if (strcmp(XtName(ww),strGreen)==0)
    rgba->m_value[1]=value;
  else if (strcmp(XtName(ww),strBlue)==0)
    rgba->m_value[2]=value;

}

static void cbSpinText(Widget ww,XtPointer userdata,XtPointer data) {

  int lightidx=(int)((long)userdata);
  if (lightidx<0 || lightidx>=light_n)
    return;

  memset(&strBuf1[0],0,buflen);
  char *currtxt=XmTextGetString(ww);
  if (currtxt) {
    strncpy(strBuf1,currtxt,buflen-1);
    XtFree(currtxt);
  }
  char *nptr=0;
  double txtval=strtod(&strBuf1[0],&nptr);

  int sbval=0;
  XtSetArg(xxargs[0],XmNposition,&sbval);
  XtGetValues(ww,xxargs,1);

  if (fabs(txtval)>simplepow10[spin_dec_off] || (nptr && strlen(nptr)>0)) {
    sprintf(strBuf1,"%.*f",spin_dec_off,(double)sbval/simplepow10[spin_dec_off]);
    XmTextSetString(ww,strBuf1);
  }
  else {
    XtSetArg(xxargs[0],XmNposition,(int)(txtval*simplepow10[spin_dec_off]));
    XtSetValues(ww,xxargs,1);
    spinValue(lightidx,ww,txtval);
  }

}

static void cbSpinPosition(Widget ww,XtPointer userdata,XtPointer data) {

  XmSpinBoxCallbackStruct *spdata=(XmSpinBoxCallbackStruct *)data;
  if (spdata->reason!=XmCR_OK)
    return;

  int lightidx=(int)((long)userdata);
  spinValue(lightidx,spdata->widget,(double)spdata->position/simplepow10[spin_dec_off]);

}

static void cbComboSelect(Widget ww,XtPointer userdata,XtPointer data) {

  memset(&strBuf1[0],0,buflen);
  int lightidx=(int)((long)userdata);
  if (lightidx<0 || lightidx>=light_n)
    return;
  struct LightSource *lightsource=&(wrklightL[lightidx]);
  XmComboBoxCallbackStruct *cbdata=(XmComboBoxCallbackStruct *)data;
  char *str=0;
  XmStringGetLtoR(cbdata->item_or_text,XmSTRING_DEFAULT_CHARSET,&str);
  strncpy(strBuf1,str,buflen-1);
  int natmat=0;
  struct LightSpec *specL=0;
  if (strcmp(XtName(ww),strNature)==0) {
    specL=&(lightsource->m_specL[0]);
    natmat=1;
  }
  else if (strcmp(XtName(ww),strMaterial)==0) {
    specL=&(lightsource->m_materialL[0]);
    natmat=2;
  }
  if (!specL)
    return;
  int ii=0,jj=0,specidx=-1,newspecidx=-1;
  for (ii=0;ii<lightspec_n;ii++) {
    if ((natmat==1 && strcmp(strBuf1,strLightSpecName[ii])==0) ||
         (natmat==2 && strcmp(strBuf1,strMaterialSpecName[ii])==0)) {
      for (jj=0;jj<lightspec_n;jj++) {
        if (strcmp(strBuf1,specL[jj].m_snature)==0) {
          specidx=jj;
          break;
        }
      }
      if (specidx<0) {
        for (jj=0;jj<lightspec_n;jj++) {
          if (strlen(specL[jj].m_snature)==0) {
            newspecidx=jj;
            break;
          }
        }
      }

      break;
    }
  }

  if (specidx<0 && newspecidx<0)
    return;

  double wxyz[4]={1.,0.,0.,0.};
  if (newspecidx<0) {
    wxyz[0]=specL[specidx].m_value[3];
    wxyz[1]=specL[specidx].m_value[0];
    wxyz[2]=specL[specidx].m_value[1];
    wxyz[3]=specL[specidx].m_value[2];
  }
  else {
    strcpy(specL[newspecidx].m_snature,strBuf1);
    specidx=newspecidx;
  }
  if (natmat==1)
    lightsource->m_sidx=specidx;
  else
    lightsource->m_midx=specidx;
  Widget wrgba=0;
  for (ii=0;ii<4;ii++) {
    if (ii==0)
      wrgba=(natmat==1 ? naturealpha_w[lightidx] : materialalpha_w[lightidx]);
    else if (ii==1)
      wrgba=(natmat==1 ? naturered_w[lightidx] : materialred_w[lightidx]);
    else if (ii==2)
      wrgba=(natmat==1 ? naturegreen_w[lightidx] : materialgreen_w[lightidx]);
    else if (ii==3)
      wrgba=(natmat==1 ? natureblue_w[lightidx] : materialblue_w[lightidx]);
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*wxyz[ii]));
    XtSetValues(wrgba,xxargs,1);
    sprintf(strBuf1,"%.*f",spin_dec_off,wxyz[ii]);
    XmTextSetString(wrgba,strBuf1);
  }

}

void createLightDialog(Widget parent,int lightidx) {

  if (lightdlg_w[lightidx])
    return;

  int ii=0;
  wrklightL[lightidx]=lightL[lightidx];
  struct LightSource *lightsource=&(wrklightL[lightidx]);
  struct LightSpec *lightspec=0;

  lightdlg_w[lightidx]=XmCreateTemplateDialog(parent,strLightDlg,(Arg*)0,0);

  XtSetArg(xxargs[0],XmNresizable,1);
  XtSetArg(xxargs[1],XmNfractionBase,8);
  Widget lightframe_w=XmCreateForm(lightdlg_w[lightidx],strLightFrame,xxargs,2);
  XtManageChild(lightframe_w);

  XtSetArg(xxargs[1],XmNleftOffset,5);
  XtSetArg(xxargs[2],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNtopOffset,5);
  XtSetArg(xxargs[4],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[5],XmNtopPosition,0);
  XtSetArg(xxargs[6],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[7],XmNbottomPosition,1);
  lighton_w[lightidx]=XmCreateToggleButton(lightframe_w,strOn,xxargs,8);
  if ((lightsource->m_on&light_on)>0)
    XtSetArg(xxargs[0],XmNset,1);
  else
    XtSetArg(xxargs[0],XmNset,0);
  XtSetValues(lighton_w[lightidx],xxargs,1);
  XtManageChild(lighton_w[lightidx]);

  XtSetArg(xxargs[0],XmNresizable,1);
  XtSetArg(xxargs[1],XmNorientation,XmVERTICAL);
  XtSetArg(xxargs[2],XmNleftOffset,5);
  XtSetArg(xxargs[3],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[4],XmNrightOffset,5);
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[6],XmNtopOffset,10);
  XtSetArg(xxargs[7],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[8],XmNtopPosition,1);
  XtSetArg(xxargs[9],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[10],XmNbottomPosition,4);
  XtSetArg(xxargs[11],XmNfractionBase,2);
  Widget natureframe=XmCreateForm(lightframe_w,strNatureFrame,xxargs,12);
  XtManageChild(natureframe);

  XtSetArg(xxargs[1],XmNorientation,XmHORIZONTAL);
  XtSetArg(xxargs[8],XmNtopPosition,0);
  XtSetArg(xxargs[10],XmNbottomPosition,1);
  XtSetArg(xxargs[11],XmNfractionBase,6);
  Widget naturecomboframe=XmCreateForm(natureframe,strNatureComboFrame,xxargs,12);
  XtManageChild(naturecomboframe);

  XtSetArg(xxargs[1],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[2],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNleftAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[4],XmNleftPosition,0);
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[6],XmNrightPosition,1);
  Widget naturelabel=XmCreateLabel(naturecomboframe,strNature,xxargs,7);
  XtManageChild(naturelabel);

  XtSetArg(xxargs[1],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[2],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNleftAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[4],XmNleftPosition,1);
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[6],XmNrightPosition,6);
  XtSetArg(xxargs[7],XmNcomboBoxType,XmDROP_DOWN_LIST);
  lightnature_w[lightidx]=XmCreateComboBox(naturecomboframe,strNature,xxargs,8);
  for (ii=0;ii<lightspec_n;ii++)
    xmsL[ii]=XmStringCreateSimple(strLightSpecName[ii]);
  XtSetArg(xxargs[0],XmNitems,xmsL);
  XtSetArg(xxargs[1],XmNitemCount,lightspec_n);
  XtSetValues(lightnature_w[lightidx],xxargs,2);
  for (ii=0;ii<lightspec_n;ii++)
    XmStringFree(xmsL[ii]);
  XtManageChild(lightnature_w[lightidx]);
  for (ii=0;ii<lightspec_n;ii++) {
    if (strlen(lightsource->m_specL[ii].m_snature)>0) {
      lightspec=&(lightsource->m_specL[ii]);
      break;
    }
  }
  if (lightspec) {
    xms=XmStringCreateSimple(lightspec->m_snature);
    XtSetArg(xxargs[0],XmNselectedItem,xms);
    XtSetValues(lightnature_w[lightidx],xxargs,1);
    XmStringFree(xms);
  }

  XtSetArg(xxargs[0],XmNresizable,1);
  XtSetArg(xxargs[1],XmNtopOffset,5);
  XtSetArg(xxargs[2],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[3],XmNtopPosition,1);
  XtSetArg(xxargs[4],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[5],XmNbottomPosition,2);
  XtSetArg(xxargs[6],XmNrightOffset,5);
  XtSetArg(xxargs[7],XmNrightAttachment,XmATTACH_FORM);
  naturecolor_w[lightidx]=XmCreateSpinBox(natureframe,strNatureColor,xxargs,8);

  XtSetArg(xxargs[1],XmNeditable,1);
  XtSetArg(xxargs[2],XmNspinBoxChildType,XmNUMERIC);
  XtSetArg(xxargs[3],XmNminimumValue,-(int)simplepow10[spin_dec_off]);
  XtSetArg(xxargs[4],XmNmaximumValue,(int)simplepow10[spin_dec_off]);
  XtSetArg(xxargs[5],XmNdecimalPoints,spin_dec_off);
  //XtSetArg(xxargs[6],XmNleadingZeroes,true);
  XtSetArg(xxargs[6],XmNincrementValue,1);
  XtSetArg(xxargs[7],XmNpositionType,XmPOSITION_VALUE);
  XtSetArg(xxargs[8],XtNwidth,60);
  naturealpha_w[lightidx]=XmCreateTextField(naturecolor_w[lightidx],strAlpha,xxargs,9);
  XtManageChild(naturealpha_w[lightidx]);
  if (lightspec) {
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*lightspec->m_value[3]));
    XtSetValues(naturealpha_w[lightidx],xxargs,1);
  }

  XtSetArg(xxargs[0],XmNresizable,1);
  naturered_w[lightidx]=XmCreateTextField(naturecolor_w[lightidx],strRed,xxargs,9);
  XtManageChild(naturered_w[lightidx]);
  if (lightspec) {
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*lightspec->m_value[0]));
    XtSetValues(naturered_w[lightidx],xxargs,1);
  }

  XtSetArg(xxargs[0],XmNresizable,1);
  naturegreen_w[lightidx]=XmCreateTextField(naturecolor_w[lightidx],strGreen,xxargs,9);
  XtManageChild(naturegreen_w[lightidx]);
  if (lightspec) {
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*lightspec->m_value[1]));
    XtSetValues(naturegreen_w[lightidx],xxargs,1);
  }

  XtSetArg(xxargs[0],XmNresizable,1);
  natureblue_w[lightidx]=XmCreateTextField(naturecolor_w[lightidx],strBlue,xxargs,9);
  XtManageChild(natureblue_w[lightidx]);
  if (lightspec) {
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*lightspec->m_value[2]));
    XtSetValues(natureblue_w[lightidx],xxargs,1);
  }

  XtManageChild(naturecolor_w[lightidx]);

  XtSetArg(xxargs[0],XmNresizable,1);
  XtSetArg(xxargs[1],XmNorientation,XmVERTICAL);
  XtSetArg(xxargs[2],XmNleftOffset,5);
  XtSetArg(xxargs[3],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[4],XmNrightOffset,5);
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[6],XmNtopOffset,10);
  XtSetArg(xxargs[7],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[8],XmNtopPosition,4);
  XtSetArg(xxargs[9],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[10],XmNbottomPosition,7);
  XtSetArg(xxargs[11],XmNfractionBase,2);
  Widget materialframe=XmCreateForm(lightframe_w,strMaterialFrame,xxargs,12);
  XtManageChild(materialframe);

  XtSetArg(xxargs[1],XmNorientation,XmHORIZONTAL);
  XtSetArg(xxargs[8],XmNtopPosition,0);
  XtSetArg(xxargs[10],XmNbottomPosition,1);
  XtSetArg(xxargs[11],XmNfractionBase,6);
  Widget materialcomboframe=XmCreateForm(materialframe,strMaterialComboFrame,xxargs,12);
  XtManageChild(materialcomboframe);

  XtSetArg(xxargs[1],XmNtopOffset,5);
  XtSetArg(xxargs[2],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[4],XmNleftAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[5],XmNleftPosition,0);
  XtSetArg(xxargs[6],XmNrightAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[7],XmNrightPosition,1);
  Widget materiallabel=XmCreateLabel(materialcomboframe,strMaterial,xxargs,8);
  XtManageChild(materiallabel);

  XtSetArg(xxargs[1],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[2],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[3],XmNleftAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[4],XmNleftPosition,1);
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[6],XmNrightPosition,6);
  XtSetArg(xxargs[7],XmNcomboBoxType,XmDROP_DOWN_LIST);
  material_w[lightidx]=XmCreateComboBox(materialcomboframe,strMaterial,xxargs,8);
  for (ii=0;ii<lightspec_n;ii++)
    xmsL[ii]=XmStringCreateSimple(strMaterialSpecName[ii]);
  XtSetArg(xxargs[0],XmNitems,xmsL);
  XtSetArg(xxargs[1],XmNitemCount,lightspec_n);
  XtSetValues(material_w[lightidx],xxargs,2);
  for (ii=0;ii<lightspec_n;ii++)
    XmStringFree(xmsL[ii]);
  XtManageChild(material_w[lightidx]);
  lightspec=0;
  for (ii=0;ii<lightspec_n;ii++) {
    if (strlen(lightsource->m_materialL[ii].m_snature)>0) {
      lightspec=&(lightsource->m_materialL[ii]);
      break;
    }
  }
  if (lightspec) {
    xms=XmStringCreateSimple(lightspec->m_snature);
    XtSetArg(xxargs[0],XmNselectedItem,xms);
    XtSetValues(material_w[lightidx],xxargs,1);
    XmStringFree(xms);
  }

  XtSetArg(xxargs[0],XmNresizable,1);
  XtSetArg(xxargs[1],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[2],XmNtopPosition,1);
  XtSetArg(xxargs[3],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[4],XmNbottomPosition,2);
  XtSetArg(xxargs[5],XmNrightOffset,5);
  XtSetArg(xxargs[6],XmNrightAttachment,XmATTACH_FORM);
  materialcolor_w[lightidx]=XmCreateSpinBox(materialframe,strMaterialColor,xxargs,7);

  XtSetArg(xxargs[1],XmNeditable,1);
  XtSetArg(xxargs[2],XmNspinBoxChildType,XmNUMERIC);
  XtSetArg(xxargs[3],XmNminimumValue,-(int)simplepow10[spin_dec_off]);
  XtSetArg(xxargs[4],XmNmaximumValue,(int)simplepow10[spin_dec_off]);
  XtSetArg(xxargs[5],XmNdecimalPoints,spin_dec_off);
  //XtSetArg(xxargs[6],XmNleadingZeroes,true);
  XtSetArg(xxargs[6],XmNincrementValue,1);
  XtSetArg(xxargs[7],XmNpositionType,XmPOSITION_VALUE);
  XtSetArg(xxargs[8],XtNwidth,60);
  materialalpha_w[lightidx]=XmCreateTextField(materialcolor_w[lightidx],strAlpha,xxargs,9);
  XtManageChild(materialalpha_w[lightidx]);
  if (lightspec) {
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*lightspec->m_value[3]));
    XtSetValues(materialalpha_w[lightidx],xxargs,1);
  }

  XtSetArg(xxargs[0],XmNresizable,1);
  materialred_w[lightidx]=XmCreateTextField(materialcolor_w[lightidx],strRed,xxargs,9);
  XtManageChild(materialred_w[lightidx]);
  if (lightspec) {
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*lightspec->m_value[0]));
    XtSetValues(materialred_w[lightidx],xxargs,1);
  }

  XtSetArg(xxargs[0],XmNresizable,1);
  materialgreen_w[lightidx]=XmCreateTextField(materialcolor_w[lightidx],strGreen,xxargs,9);
  XtManageChild(materialgreen_w[lightidx]);
  if (lightspec) {
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*lightspec->m_value[1]));
    XtSetValues(materialgreen_w[lightidx],xxargs,1);
  }

  XtSetArg(xxargs[0],XmNresizable,1);
  materialblue_w[lightidx]=XmCreateTextField(materialcolor_w[lightidx],strBlue,xxargs,9);
  XtManageChild(materialblue_w[lightidx]);
  if (lightspec) {
    XtSetArg(xxargs[0],XmNposition,(int)(simplepow10[spin_dec_off]*lightspec->m_value[2]));
    XtSetValues(materialblue_w[lightidx],xxargs,1);
  }

  XtManageChild(materialcolor_w[lightidx]);

  XtSetArg(xxargs[0],XmNresizable,1);
  XtSetArg(xxargs[1],XmNorientation,XmHORIZONTAL);
  XtSetArg(xxargs[2],XmNleftOffset,5);
  XtSetArg(xxargs[3],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[4],XmNrightOffset,5);
  XtSetArg(xxargs[5],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[6],XmNtopOffset,10);
  XtSetArg(xxargs[7],XmNtopAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[8],XmNtopPosition,7);
  XtSetArg(xxargs[9],XmNbottomAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[10],XmNbottomPosition,8);
  XtSetArg(xxargs[11],XmNfractionBase,3);
  Widget buttonframe=XmCreateForm(lightframe_w,strButtonFrame,xxargs,12);
  XtManageChild(buttonframe);

  XtSetArg(xxargs[1],XmNleftAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[2],XmNleftPosition,0);
  XtSetArg(xxargs[3],XmNrightAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[4],XmNrightPosition,1);
  XtSetArg(xxargs[5],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[6],XmNbottomAttachment,XmATTACH_FORM);
  Widget but_w=XmCreatePushButton(buttonframe,strConfirmLightDlg,xxargs,7);
  XtAddCallback(but_w,XmNactivateCallback,cbLightDlgConfirm,(void*)((long)lightidx));
  XtManageChild(but_w);

  XtSetArg(xxargs[1],XmNleftAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[2],XmNleftPosition,2);
  XtSetArg(xxargs[3],XmNrightAttachment,XmATTACH_POSITION);
  XtSetArg(xxargs[4],XmNrightPosition,3);
  XtSetArg(xxargs[5],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(xxargs[6],XmNbottomAttachment,XmATTACH_FORM);
  but_w=XmCreatePushButton(buttonframe,strQuitLightDlg,xxargs,7);
  XtAddCallback(but_w,XmNactivateCallback,cbLightDlgQuit,(void*)((long)lightidx));
  XtManageChild(but_w);

  Widget separatorwidget=XtNameToWidget(lightdlg_w[lightidx],"Separator");
  XtUnmanageChild(separatorwidget);
  XtManageChild(lightdlg_w[lightidx]);

  XtAddCallback(lighton_w[lightidx],XmNvalueChangedCallback,cbLightOn,(void*)((long)lightidx));
  XtAddCallback(lightnature_w[lightidx],XmNselectionCallback,cbComboSelect,(void*)((long)lightidx));
  XtAddCallback(naturealpha_w[lightidx],XmNvalueChangedCallback,cbSpinText,(void*)((long)lightidx));
  XtAddCallback(naturered_w[lightidx],XmNvalueChangedCallback,cbSpinText,(void*)((long)lightidx));
  XtAddCallback(naturegreen_w[lightidx],XmNvalueChangedCallback,cbSpinText,(void*)((long)lightidx));
  XtAddCallback(natureblue_w[lightidx],XmNvalueChangedCallback,cbSpinText,(void*)((long)lightidx));
  XtAddCallback(naturecolor_w[lightidx],XmNvalueChangedCallback,cbSpinPosition,(void*)((long)lightidx));
  XtAddCallback(material_w[lightidx],XmNselectionCallback,cbComboSelect,(void*)((long)lightidx));
  XtAddCallback(materialalpha_w[lightidx],XmNvalueChangedCallback,cbSpinText,(void*)((long)lightidx));
  XtAddCallback(materialred_w[lightidx],XmNvalueChangedCallback,cbSpinText,(void*)((long)lightidx));
  XtAddCallback(materialgreen_w[lightidx],XmNvalueChangedCallback,cbSpinText,(void*)((long)lightidx));
  XtAddCallback(materialblue_w[lightidx],XmNvalueChangedCallback,cbSpinText,(void*)((long)lightidx));
  XtAddCallback(materialcolor_w[lightidx],XmNvalueChangedCallback,cbSpinPosition,(void*)((long)lightidx));

}


