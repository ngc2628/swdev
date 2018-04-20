
#include <qt/simpleplot/controls.h>
#include <math.h>
#include <QtCore/QVariant>
#include <QtWidgets/QGroupBox>
#include <QtGui/QPainter>
#include <graphic/charts/simpleplot/graph.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/charts/simpleplot/scale.h>

namespace qtsimpleplot {

static const int numlinestyles=6;
static const char *const linestyles[]={"none","solid","dash","dot","dashdot","dashdotdot"};
static const int numfillstyles=15;
static const char *const fillstyles[]={"none","solid","dense1","dense2","dense3","dense4",
  "dense5","dense6","dense7","hor","ver","cross","bdiag","fdiag","crossdiag"};

GraphProps::GraphProps(QWidget *parent,QtDiagramXY *chart) : 
  QFrame(parent),m_chart(chart),m_layout(0),m_selector(0),m_buttoncolorline(0),m_buttoncolormarkO(0),
  m_buttoncolormarkF(0),m_buttoncolorfill(0),m_colorchooser(0),m_combolinestyle(0),
  m_combomarkstyle(0),m_combomarkstyleO(0),m_combomarkstyleF(0),m_combofillref(0),m_combofillstyle(0),
  m_editlinewidth(0),m_editmarkwidth(0),m_combointerpolation(0),m_checkinterpolationoption(0),
  m_comboxax(0),m_comboyax(0),m_interpolationoptions(0),m_linecolor(0),m_markcolorO(0),m_markcolorF(0) {
  
  m_interpolationoptionL.resize(num::numinerpolationoptions);
  m_layout=new QGridLayout(this);
  m_layout->setSpacing(0); 
  m_layout->setMargin(5);
  m_selector=new QComboBox(this);
  connect(m_selector,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSelected(int)));
  m_layout->addWidget(m_selector,0,0,1,3);
  QFrame *spacerH=new QFrame(this);
  spacerH->setFixedHeight(5);
  m_layout->addWidget(spacerH,1,0,1,3);
  QGroupBox *grouplinestyle=new QGroupBox("line",this);
  grouplinestyle->setFlat(true);
  QGridLayout *grouplinestylelayout=new QGridLayout(grouplinestyle);
  grouplinestylelayout->setSpacing(5); 
  grouplinestylelayout->setMargin(0);
  m_combolinestyle=new QComboBox(grouplinestyle);
  grouplinestylelayout->addWidget(m_combolinestyle,0,0);
  m_buttoncolorline=new qtutil::ColorButton(grouplinestyle,m_combolinestyle);
  m_buttoncolorline->setBackgroundRole(QPalette::Window);
  m_buttoncolorline->setAutoFillBackground(true);
  connect(m_buttoncolorline,SIGNAL(clicked()),this,SLOT(slotColorLine()));
  grouplinestylelayout->addWidget(m_buttoncolorline,1,0);
  m_editlinewidth=new QLineEdit(grouplinestyle);
  grouplinestylelayout->addWidget(m_editlinewidth,2,0);
  m_layout->addWidget(grouplinestyle,2,0,3,1);
  QGroupBox *groupfill=new QGroupBox("fill",this);
  groupfill->setFlat(true);
  QGridLayout *groupfilllayout=new QGridLayout(groupfill);
  groupfilllayout->setSpacing(5); 
  groupfilllayout->setMargin(0);
  QGroupBox *groupfillref=new QGroupBox("ref",groupfill);
  groupfillref->setFlat(true);
  QGridLayout *groupfillreflayout=new QGridLayout(groupfillref);
  groupfillreflayout->setSpacing(5); 
  groupfillreflayout->setMargin(0);
  m_combofillref=new QComboBox(groupfillref);
  groupfillreflayout->addWidget(m_combofillref,0,0);
  groupfilllayout->addWidget(groupfillref,0,0,2,1);
  QGroupBox *groupfillstyle=new QGroupBox("style",groupfill);
  groupfillstyle->setFlat(true);
  QGridLayout *groupfillstylelayout=new QGridLayout(groupfillstyle);
  groupfillstylelayout->setSpacing(5); 
  groupfillstylelayout->setMargin(0);
  m_combofillstyle=new QComboBox(groupfillstyle);
  groupfillstylelayout->addWidget(m_combofillstyle,0,0);
  m_buttoncolorfill=new qtutil::ColorButton(groupfillstyle,m_combofillstyle);
  m_buttoncolorfill->setBackgroundRole(QPalette::Window);
  m_buttoncolorfill->setAutoFillBackground(true);
  connect(m_buttoncolorfill,SIGNAL(clicked()),this,SLOT(slotColorFill()));
  groupfillstylelayout->addWidget(m_buttoncolorfill,1,0);
  groupfilllayout->addWidget(groupfillstyle,2,0,2,1);
  m_layout->addWidget(groupfill,5,0,3,1);
  QGroupBox *groupaxes=new QGroupBox("axes",this);
  groupaxes->setFlat(true);
  QGridLayout *groupaxeslayout=new QGridLayout(groupaxes);
  groupaxeslayout->setSpacing(5); 
  groupaxeslayout->setMargin(0);
  m_comboxax=new QComboBox(groupaxes);
  m_comboyax=new QComboBox(groupaxes);
  groupaxeslayout->addWidget(m_comboxax,0,0);
  groupaxeslayout->addWidget(m_comboyax,1,0);
  m_layout->addWidget(groupaxes,8,0,2,1);
  QFrame *spacerV=new QFrame(this);
  spacerV->setFixedWidth(5);
  m_layout->addWidget(spacerV,2,1,9,1);
  QGroupBox *groupmarkstyle=new QGroupBox("mark",this);
  groupmarkstyle->setFlat(true);
  QGridLayout *groupmarkstylelayout=new QGridLayout(groupmarkstyle);
  groupmarkstylelayout->setSpacing(5); 
  groupmarkstylelayout->setMargin(0);
  QGroupBox *groupmarkstyleshape=new QGroupBox("shape",groupmarkstyle);
  groupmarkstyleshape->setFlat(true);
  QGridLayout *groupmarkstyleshapelayout=new QGridLayout(groupmarkstyleshape);
  groupmarkstyleshapelayout->setSpacing(5); 
  groupmarkstyleshapelayout->setMargin(0);
  m_combomarkstyle=new QComboBox(groupmarkstyleshape);
  groupmarkstyleshapelayout->addWidget(m_combomarkstyle,0,0);
  m_editmarkwidth=new QLineEdit(groupmarkstyleshape);
  groupmarkstyleshapelayout->addWidget(m_editmarkwidth,1,0);
  groupmarkstylelayout->addWidget(groupmarkstyleshape,0,0,2,1);
  QGroupBox *groupmarkstyleF=new QGroupBox("fill",this);
  groupmarkstyleF->setFlat(true);
  QGridLayout *groupmarkstylelayoutF=new QGridLayout(groupmarkstyleF);
  groupmarkstylelayoutF->setSpacing(5); 
  groupmarkstylelayoutF->setMargin(0);
  m_combomarkstyleF=new QComboBox(groupmarkstyleF);
  groupmarkstylelayoutF->addWidget(m_combomarkstyleF,0,0);
  m_buttoncolormarkF=new qtutil::ColorButton(groupmarkstyleF,m_combomarkstyle);
  m_buttoncolormarkF->setBackgroundRole(QPalette::Window);
  m_buttoncolormarkF->setAutoFillBackground(true);
  connect(m_buttoncolormarkF,SIGNAL(clicked()),this,SLOT(slotColorMarkF()));
  groupmarkstylelayoutF->addWidget(m_buttoncolormarkF,1,0);
  groupmarkstylelayout->addWidget(groupmarkstyleF,2,0,2,1);
  QGroupBox *groupmarkstyleO=new QGroupBox("outline",this);
  groupmarkstyleO->setFlat(true);
  QGridLayout *groupmarkstylelayoutO=new QGridLayout(groupmarkstyleO);
  groupmarkstylelayoutO->setSpacing(5); 
  groupmarkstylelayoutO->setMargin(0);
  m_combomarkstyleO=new QComboBox(groupmarkstyleO);
  groupmarkstylelayoutO->addWidget(m_combomarkstyleO,0,0);
  m_buttoncolormarkO=new qtutil::ColorButton(groupmarkstyleO,m_combomarkstyle);
  m_buttoncolormarkO->setBackgroundRole(QPalette::Window);
  m_buttoncolormarkO->setAutoFillBackground(true);
  connect(m_buttoncolormarkO,SIGNAL(clicked()),this,SLOT(slotColorMarkO()));
  groupmarkstylelayoutO->addWidget(m_buttoncolormarkO,1,0);
  groupmarkstylelayout->addWidget(groupmarkstyleO,4,0,2,1);
  m_layout->addWidget(groupmarkstyle,2,2,6,1);
  QGroupBox *groupinterpolation=new QGroupBox("interpolation",this);
  groupinterpolation->setFlat(true);
  QGridLayout *groupinterpolationlayout=new QGridLayout(groupinterpolation);
  groupinterpolationlayout->setSpacing(5); 
  groupinterpolationlayout->setMargin(0);
  m_combointerpolation=new QComboBox(groupinterpolation);
  groupinterpolationlayout->addWidget(m_combointerpolation,0,0);
  m_checkinterpolationoption=new QCheckBox("options",groupinterpolation);
  connect(m_checkinterpolationoption,SIGNAL(stateChanged(int)),this,SLOT(slotInterpolationOptions(int)));
  groupinterpolationlayout->addWidget(m_checkinterpolationoption,1,0);
  m_layout->addWidget(groupinterpolation,8,2,2,1);
   
  aux::TPList<simpleplot::Graph> gridL;
  int ii=0,jj=0,ngr=(m_chart ? m_chart->typegraphs("graphxy",&gridL) : 0);
  if (ngr==0) {
    grouplinestyle->setEnabled(false);
    groupfill->setEnabled(false);
    groupmarkstyle->setEnabled(false);
    groupaxes->setEnabled(false);
    groupinterpolation->setEnabled(false);
    return;
  }
  aux::Asciistr str;
  aux::TypeId *typeidd=0; 
  QVariant qv;
  QString qvval;
  for (ii=0;ii<ngr;ii++) {
    typeidd=(aux::TypeId*)(gridL[ii]);
    typeidd->toString(&str);
    qtutil::toQString(&str,&qvval);
    qv.setValue(qvval);
    m_selector->addItem(qvval,qv);
    str=0;
  }
  
  aux::TPList<simpleplot::Axis> xaxL,yaxL;
  int nxax=(m_chart ? m_chart->typeaxes("xaxis",&xaxL) : 0),
      nyax=(m_chart ? m_chart->typeaxes("yaxis",&yaxL) : 0);
  for (ii=0;ii<nxax;ii++) {
    typeidd=(aux::TypeId*)(xaxL[ii]);
    typeidd->toString(&str);
    qtutil::toQString(&str,&qvval);
    qv.setValue(qvval);
    m_comboxax->addItem(qvval,qv);
    str=0;
  }
  for (ii=0;ii<nyax;ii++) {
    typeidd=(aux::TypeId*)(yaxL[ii]);
    typeidd->toString(&str);
    qtutil::toQString(&str,&qvval);
    qv.setValue(qvval);
    m_comboyax->addItem(qvval,qv);
    str=0;
  }
  
  m_combofillref->addItem("none");
  
  for (ii=0;ii<numlinestyles;ii++) {
    qv.setValue(ii);
    m_combolinestyle->addItem(linestyles[ii],qv);
    m_combomarkstyleO->addItem(linestyles[ii],qv);
  }
  
  for (ii=0;ii<numfillstyles;ii++) {
    qv.setValue(ii);
    m_combomarkstyleF->addItem(fillstyles[ii],qv);
    m_combofillstyle->addItem(fillstyles[ii],qv);
  }

  shapes::Shape2 *samplemark=0;
  aux::TVList<aux::Vector3> smL(600);
  QPointF smpL[120];
  QPixmap smpix(30,30);
  QPainter smp;
  int cnteval=0;
  for (ii=0;ii<simpleplot::nummarkshapes2d;ii++) {
    str=simpleplot::markshapes2d[ii];
    qtutil::toQString(&str,&qvval);
    qv.setValue(qvval);
    samplemark=simpleplot::buildMarkshape2(simpleplot::markshapes2d[ii],12.);
    if (samplemark) {
      samplemark->eval(&smL);
      cnteval=smL.count(); 
      for (jj=0;jj<cnteval;jj++) 
        smpL[jj]=QPointF(smL.at(jj)->x()+15.,smL.at(jj)->y()+15.);
      smp.begin(&smpix);
      smp.fillRect(0,0,30,30,m_combomarkstyle->palette().button());
      smp.setBrush(Qt::NoBrush);
      smp.setPen(QPen(QBrush(QColor(0,0,0)),3.));
      smp.drawPolygon(smpL,cnteval);
      smp.end();
      m_combomarkstyle->addItem(smpix.copy(),qvval,qv);
      delete samplemark;
      samplemark=0; 
    }
    else 
      m_combomarkstyle->addItem(qvval,qv);
  }
  
  for (ii=0;ii<num::numinterpolationtypes;ii++) {
    str=num::interpolationtypes[ii];
    qtutil::toQString(&str,&qvval);
    qv.setValue(qvval);
    m_combointerpolation->addItem(qvval,qv);
    str=0;  
  }
     
  slotSelected(0);
  
}

GraphProps::~GraphProps() {

  if (m_colorchooser) 
    delete m_colorchooser;
  if (m_interpolationoptions)
    delete m_interpolationoptions;

}

void GraphProps::slotSelected(int idx) {

  QVariant qv=m_selector->itemData(idx);
  QString qvval(qv.value<QString>()),qstr;
  aux::Asciistr str;
  qtutil::fromQString(&qvval,&str);
//printf ("%d grid=%s\n",__LINE__,(const char *)str);
  aux::TypeId grid;
  grid.fromString((const char *)str);
  simpleplot::GraphXY *gr=dynamic_cast<simpleplot::GraphXY*>(m_chart->graph(grid));
  
  m_buttoncolorline->setEnabled(gr!=0);
  m_buttoncolorfill->setEnabled(gr!=0);
  m_buttoncolormarkO->setEnabled(gr!=0);
  m_buttoncolormarkF->setEnabled(gr!=0);
  m_buttoncolormarkF->setEnabled(gr!=0);
  m_combolinestyle->setEnabled(gr!=0);
  m_combofillref->setEnabled(gr!=0);
  m_combofillstyle->setEnabled(gr!=0);
  m_editmarkwidth->setEnabled(gr!=0);
  m_combomarkstyleO->setEnabled(gr!=0);
  m_combomarkstyleF->setEnabled(gr!=0);
  m_editlinewidth->setEnabled(gr!=0);
  m_combointerpolation->setEnabled(gr!=0);
  m_comboxax->setEnabled(gr!=0);
  m_comboyax->setEnabled(gr!=0);
  if (!gr) 
    return;
  
  str=0;
  aux::i2a(gr->m_linestyle.m_width,&str);
  qtutil::toQString(&str,&qstr);
  str=0;
  m_linecolor=gr->m_linestyle.m_color;
  qv.setValue(gr->m_linestyle.m_style);
  m_combolinestyle->setCurrentIndex(m_combolinestyle->findData(qv));
  m_buttoncolorline->setColor(m_linecolor);
  m_editlinewidth->setText(qstr);
  
  shapes::Shape2 *grmark=(gr->m_graphdata ? gr->m_graphdata->mark(0) : 0);
  if (!grmark) {
    m_combomarkstyle->setCurrentIndex(0);
    m_buttoncolormarkF->setColor(0);
    m_combomarkstyleF->setCurrentIndex(0);
    m_buttoncolormarkO->setColor(0);
    m_combomarkstyleO->setCurrentIndex(0);
    m_editmarkwidth->setText("0");
  }
  else {
    m_markcolorO=grmark->m_styleO.m_color;
    m_markcolorF=grmark->m_styleF.m_color;
    m_combomarkstyle->setCurrentIndex(simpleplot::markshape2idx(grmark->descr()));
    m_editmarkwidth->setText(QString::number(grmark->circradius(),'f',2));
    m_buttoncolormarkF->setColor(m_markcolorF);
    m_combomarkstyleF->setCurrentIndex(grmark->m_styleF.m_style);
    m_buttoncolormarkO->setColor(m_markcolorO);
    m_combomarkstyleO->setCurrentIndex(grmark->m_styleO.m_style);
  }
   
  aux::TypeId axid;
  if (gr->m_axis[0]) {
    axid=*((aux::TypeId*)(gr->m_axis[0]));
    axid.toString(&str);
    qtutil::toQString(&str,&qvval);
    qv.setValue(qvval);
    m_comboxax->setCurrentIndex(m_comboxax->findData(qv));
    m_combofillref->addItem(qvval,qv);
    str=0;
  }
  if (gr->m_axis[1]) {
    axid=*((aux::TypeId*)(gr->m_axis[1]));
    axid.toString(&str);
    qtutil::toQString(&str,&qvval);
    qv.setValue(qvval);
    m_comboyax->setCurrentIndex(m_comboyax->findData(qv));
    str=0;
  }
     
  if (gr->m_interpolation) {
    gr->m_interpolation->type(&str);
    qtutil::toQString(&str,&qvval);
    qv.setValue(qvval);
    m_combointerpolation->setCurrentIndex(m_combointerpolation->findData(qv));
    gr->m_interpolation->options(&m_interpolationoptionL);
  }
  else { 
    m_combointerpolation->setCurrentIndex(0);
    m_interpolationoptionL.clear();
  }
  if (m_interpolationoptions)
    m_interpolationoptions->setOptionL(&m_interpolationoptionL);
  
}

void GraphProps::slotColorLine() {

  if (m_colorchooser) 
    return;
  m_colorchooser=new qtutil::ColorChooser(this,m_linecolor);
  connect(m_colorchooser,SIGNAL(dismiss()),this,SLOT(slotColorchooserDismiss()));
  connect(m_colorchooser,SIGNAL(setColor(unsigned int)),this,SLOT(slotSetColorLine(unsigned int)));
  m_colorchooser->show();

}

void GraphProps::slotColorFill() {

  if (m_colorchooser) 
    return;
  m_colorchooser=new qtutil::ColorChooser(this,m_fillcolor);
  connect(m_colorchooser,SIGNAL(dismiss()),this,SLOT(slotColorchooserDismiss()));
  connect(m_colorchooser,SIGNAL(setColor(unsigned int)),this,SLOT(slotSetColorFill(unsigned int)));
  m_colorchooser->show();

}
 
void GraphProps::slotColorMarkF() {

  if (m_colorchooser) 
    return;
  m_colorchooser=new qtutil::ColorChooser(this,m_markcolorF);
  connect(m_colorchooser,SIGNAL(dismiss()),this,SLOT(slotColorchooserDismiss()));
  connect(m_colorchooser,SIGNAL(setColor(unsigned int)),this,SLOT(slotSetColorMarkF(unsigned int)));
  m_colorchooser->show();

}

void GraphProps::slotColorMarkO() {

  if (m_colorchooser) 
    return;
  m_colorchooser=new qtutil::ColorChooser(this,m_markcolorO);
  connect(m_colorchooser,SIGNAL(dismiss()),this,SLOT(slotColorchooserDismiss()));
  connect(m_colorchooser,SIGNAL(setColor(unsigned int)),this,SLOT(slotSetColorMarkO(unsigned int)));
  m_colorchooser->show();

}

void GraphProps::slotSetColorLine(unsigned int color) {

  m_linecolor=color;
  m_buttoncolorline->setColor(color);

}

void GraphProps::slotSetColorFill(unsigned int color) {

  m_fillcolor=color;
  m_buttoncolorfill->setColor(color);

}

void GraphProps::slotSetColorMarkF(unsigned int color) {

  m_markcolorF=color;
  m_buttoncolormarkF->setColor(color);

}

void GraphProps::slotSetColorMarkO(unsigned int color) {

  m_markcolorO=color;
  m_buttoncolormarkO->setColor(color);

}

void GraphProps::slotInterpolationOptions(int check) {

  if (check==0) {
    if (m_interpolationoptions)
      delete m_interpolationoptions;
    m_interpolationoptions=0;
  }
  else if (check>0 && !m_interpolationoptions) {  
    m_interpolationoptions=new GraphInterpolationOptions(this);
    connect(m_interpolationoptions,SIGNAL(dismiss()),this,SLOT(slotInterpolationOptionsDismiss()));
    m_interpolationoptions->setOptionL(&m_interpolationoptionL);
    m_interpolationoptions->resize(m_interpolationoptions->sizeHint());
    m_interpolationoptions->show();
  }

}

void GraphProps::slotInterpolationOptionsDismiss() {
      
  m_interpolationoptions=0;
  m_checkinterpolationoption->setChecked(false);
  
}

void GraphProps::sendValues() {

  QVariant qv1=m_selector->itemData(m_selector->currentIndex()),qv2;
  aux::TypeId grid;
  QString qvval(qv1.value<QString>());
  aux::Asciistr str;
  qtutil::fromQString(&qvval,&str);
  grid.fromString(str);
  simpleplot::GraphXY *gr=dynamic_cast<simpleplot::GraphXY*>(m_chart->graph(grid));
  if (!gr) 
    return;
  str=0;
  if (gr->m_interpolation)
    gr->m_interpolation->type(&str);
  else
    str="none";  
  QString interptype;
  qtutil::toQString(&str,&interptype);
  str=0;
  qv1=m_combointerpolation->itemData(m_combointerpolation->currentIndex());
  QString combointerptype=qv1.toString();
  if (interptype!=combointerptype) {
    if (gr->m_interpolation) 
      delete gr->m_interpolation;
    qtutil::fromQString(&combointerptype,&str);
    gr->m_interpolation=num::buildInterpolation((const char *)str,&m_interpolationoptionL);
  }
  else if (gr->m_interpolation) 
    gr->m_interpolation->setOptions(&m_interpolationoptionL,1);
  
  osix::xxStyle linestyle(m_linecolor,(short)m_combolinestyle->currentIndex()),
                markstyleO(m_markcolorO,(short)m_combomarkstyleO->currentIndex(),1),
                markstyleF(m_markcolorF,(short)m_combomarkstyleF->currentIndex(),1);
  qvval=m_editlinewidth->text();
  qtutil::fromQString(&qvval,&str);
  linestyle.m_width=aux::a2d((const char *)str);
  str=0;
  gr->m_linestyle=linestyle;
  shapes::Shape2 *grmark=(gr->m_graphdata ? gr->m_graphdata->mark(0) : 0);
  if (grmark) {
    grmark->m_styleO=markstyleO;
    grmark->m_styleF=markstyleF;
  }
  int prec=2,base=-1,markstyle=simpleplot::markshape2idx(grmark ? grmark->descr() : 0),
      newmarkstyle=m_combomarkstyle->currentIndex();
  qvval=m_editmarkwidth->text();
  qtutil::fromQString(&qvval,&str);
  double markwidth=(grmark ? aux::round2(grmark->circradius(),prec) : .0),
         newmarkwidth=aux::round2(aux::a2d((const char *)str,&base),prec);
  str=0;
  
  if (base<0 || newmarkwidth<.0) 
    newmarkwidth=markwidth;
  if (markstyle!=newmarkstyle || markwidth!=newmarkwidth) {
    if (newmarkstyle==0 || newmarkwidth==.0) {
      gr->m_graphdata->setMark(0,0);
      m_combomarkstyle->setCurrentIndex(0);
      m_editmarkwidth->setText("0");
      m_buttoncolormarkF->setColor(0);
      m_buttoncolormarkO->setColor(0);
      m_combomarkstyleF->setCurrentIndex(0);
      m_combomarkstyleO->setCurrentIndex(0);
      m_markcolorO=m_markcolorF=0;
    }
    else {
      const char *markbuilder=(markstyle!=newmarkstyle ? simpleplot::idx2markshape2(newmarkstyle) : grmark->descr());
      grmark=simpleplot::buildMarkshape2(markbuilder,newmarkwidth);
      grmark->m_styleO=markstyleO;
      grmark->m_styleF=markstyleF;
      gr->m_graphdata->setMark(0,grmark);
    }
  }
  
  m_chart->redraw(simpleplot::redoScaleGraph|simpleplot::redoScr);

}

GraphInterpolationOptions::GraphInterpolationOptions(QWidget *parent) :
  QDialog(parent,Qt::WindowStaysOnTopHint),m_layout(0),m_checkoption(0),m_optionL(0) {

  setAttribute(Qt::WA_DeleteOnClose);
  m_layout=new QGridLayout(this);
  m_layout->setSpacing(0); 
  m_layout->setMargin(5);
  m_checkoption=new qtutil::CustomCheckBox *[num::numinerpolationoptions];
  int ii=0,cols=3,row=0,col=0;
  for (ii=0;ii<num::numinerpolationoptions;ii++) {
    m_checkoption[ii]=new qtutil::CustomCheckBox(num::interpolationoptions[ii],this);
    connect(m_checkoption[ii],SIGNAL(qstatechanged(qtutil::CustomCheckBox*,int)),
            this,SLOT(slotOptionCheck(qtutil::CustomCheckBox*,int)));  
    m_layout->addWidget(m_checkoption[ii],row,col++);
    if (col>=cols) {
      col=0;
      row++;
    }
  }
  
}
 
GraphInterpolationOptions::~GraphInterpolationOptions() {
  
  //printf ("%d destr\n",__LINE__);
  emit dismiss();
  if (m_checkoption)
    delete [] m_checkoption;

}

void GraphInterpolationOptions::setOptionL(aux::TVList<aux::Asciistr> *optionL) {

  if (optionL==m_optionL)
    return;
  m_optionL=optionL;
  aux::Asciistr str;
  int ii=0;
  for (ii=0;ii<num::numinerpolationoptions;ii++) {
    if (!m_optionL)
      m_checkoption[ii]->setChecked(false);
    else {
      str=num::interpolationoptions[ii];
      m_checkoption[ii]->setChecked(m_optionL->find(str)<0 ? false : true);
    }
  }

}

void GraphInterpolationOptions::slotOptionCheck(qtutil::CustomCheckBox *checkbox,int check) {

  aux::Asciistr str;
  int ii=0,idx=-1;
  for (ii=0;ii<num::numinerpolationoptions;ii++) {
    if (m_checkoption[ii]==checkbox) {
      if (!m_optionL)
        checkbox->setChecked(false);
      else {
        str=num::interpolationoptions[ii];
        idx=m_optionL->find(str);
        if (check>0 && idx<0)
          m_optionL->inSort(str);
        else if (check==0 && idx>=0)
          m_optionL->remove(idx);
      }
      break;
    }
  }

}

AxisProps::AxisProps(QWidget *parent,QtDiagramXY *chart) : 
  QFrame(parent),m_chart(chart),m_fontdlg(0),m_colorchooser(0),m_color(0) {

  m_layout=new QGridLayout(this);
  m_layout->setSpacing(0); 
  m_layout->setMargin(5);
  m_selector=new QComboBox(this);
  connect(m_selector,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSelected(int)));
  m_layout->addWidget(m_selector,0,0,1,5);
  QFrame *spacerH=new QFrame(this);
  spacerH->setFixedHeight(5);
  m_layout->addWidget(spacerH,1,0,1,5);
  QGroupBox *grouppos=new QGroupBox("position",this);
  grouppos->setFlat(true);
  QGridLayout *groupposlayout=new QGridLayout(grouppos);
  groupposlayout->setSpacing(0); 
  groupposlayout->setMargin(0);
  m_combopos=new QComboBox(grouppos);
  groupposlayout->addWidget(m_combopos,0,0);
  m_layout->addWidget(grouppos,2,0);
  QGroupBox *groupfont=new QGroupBox("font",this);
  groupfont->setFlat(true);
  QGridLayout *groupfontlayout=new QGridLayout(groupfont);
  groupfontlayout->setSpacing(5); 
  groupfontlayout->setMargin(0);
  m_buttonFontS=new QPushButton("font (S)",groupfont);
  m_buttonFontS->setAutoDefault(false);
  m_buttonFontS->setDefault(false);
  connect (m_buttonFontS,SIGNAL(clicked()),this,SLOT(slotFontS()));
  groupfontlayout->addWidget(m_buttonFontS,0,0);
  m_buttonFontL=new QPushButton("font (L)",groupfont);
  m_buttonFontL->setAutoDefault(false);
  m_buttonFontL->setDefault(false);
  connect (m_buttonFontL,SIGNAL(clicked()),this,SLOT(slotFontL()));
  groupfontlayout->addWidget(m_buttonFontL,1,0);
  m_layout->addWidget(groupfont,3,0);
  QGroupBox *groupstyle=new QGroupBox("style",this);
  groupstyle->setFlat(true);
  QGridLayout *groupstylelayout=new QGridLayout(groupstyle);
  groupstylelayout->setSpacing(5); 
  groupstylelayout->setMargin(0);
  m_combostyle=new QComboBox(groupstyle);
  groupstylelayout->addWidget(m_combostyle,0,0);
  m_buttoncolor=new qtutil::ColorButton(groupstyle,m_combopos);
  m_buttoncolor->setBackgroundRole(QPalette::Window);
  m_buttoncolor->setAutoFillBackground(true);
  connect(m_buttoncolor,SIGNAL(clicked()),this,SLOT(slotColor()));
  groupstylelayout->addWidget(m_buttoncolor,1,0);
  m_editwidth=new QLineEdit(groupstyle);
  groupstylelayout->addWidget(m_editwidth,2,0);
  m_layout->addWidget(groupstyle,4,0);
  QFrame *spacerV=new QFrame(this);
  spacerV->setFixedWidth(5);
  m_layout->addWidget(spacerV,2,1,5,1);
  QGroupBox *groupautoscale=new QGroupBox("autoscale",this);
  groupautoscale->setFlat(true);
  QGridLayout *groupautoscalelayout=new QGridLayout(groupautoscale);
  groupautoscalelayout->setSpacing(0); 
  groupautoscalelayout->setMargin(0);
  QButtonGroup *buttongroupautoscale=new QButtonGroup(groupautoscale);
  m_buttonAutoscaleOn=new QRadioButton("on",groupautoscale);
  connect(m_buttonAutoscaleOn,SIGNAL(toggled(bool)),this,SLOT(slotAutoscale(bool)));
  m_buttonAutoscaleOff=new QRadioButton("off",groupautoscale);
  connect(m_buttonAutoscaleOff,SIGNAL(toggled(bool)),this,SLOT(slotAutoscale(bool)));
  buttongroupautoscale->addButton(m_buttonAutoscaleOn);
  buttongroupautoscale->addButton(m_buttonAutoscaleOff);
  groupautoscalelayout->addWidget(m_buttonAutoscaleOn,0,0);
  groupautoscalelayout->addWidget(m_buttonAutoscaleOff,0,1);
  QLabel *minlabel=new QLabel("min : ",groupautoscale);
  groupautoscalelayout->addWidget(minlabel,1,0);
  m_editMin=new QLineEdit(groupautoscale);
  groupautoscalelayout->addWidget(m_editMin,1,1);
  QLabel *maxlabel=new QLabel("max : ",groupautoscale);
  groupautoscalelayout->addWidget(maxlabel,2,0);
  m_editMax=new QLineEdit(groupautoscale);
  groupautoscalelayout->addWidget(m_editMax,2,1);
  m_layout->addWidget(groupautoscale,2,2,2,3);
  QGroupBox *groupscaletype=new QGroupBox("scaletype",this);
  groupscaletype->setFlat(true);
  QGridLayout *groupscaletypelayout=new QGridLayout(groupscaletype);
  groupscaletypelayout->setSpacing(0); 
  groupscaletypelayout->setMargin(0);
  m_comboscaletype=new QComboBox(groupscaletype);
  groupscaletypelayout->addWidget(m_comboscaletype,0,0);
  m_layout->addWidget(groupscaletype,4,2,1,3);
  
  aux::TPList<simpleplot::Axis> xaxL,yaxL;
  int ii=0,nxax=(m_chart ? m_chart->typeaxes("xaxis",&xaxL) : 0),
      nyax=(m_chart ? m_chart->typeaxes("yaxis",&yaxL) : 0);
  if (nxax==0 && nyax==0) {
    groupscaletype->setEnabled(false);
    groupautoscale->setEnabled(false);
    groupstyle->setEnabled(false);
    groupfont->setEnabled(false);
    grouppos->setEnabled(false);
    return;
  }
  aux::Asciistr str;
  aux::TypeId *axid=0; 
  QVariant qv;
  QString qvval;
  for (ii=0;ii<nxax;ii++) {
    axid=(aux::TypeId*)(xaxL[ii]);
    axid->toString(&str);
    qvval=(const char *)str;
    qv.setValue(qvval);
    m_selector->addItem((const char *)str,qv);
    str=0;
  }
  for (ii=0;ii<nyax;ii++) {
    axid=(aux::TypeId*)(yaxL[ii]);
    axid->toString(&str);
    qvval=(const char *)str;
    qv.setValue(qvval);
    m_selector->addItem((const char *)str,qv);
    str=0;
  }
  
  for (ii=0;ii<numlinestyles;ii++) {
    qv.setValue(ii);
    m_combostyle->addItem(linestyles[ii],qv);
  }
        
  slotSelected(0);

}

AxisProps::~AxisProps() {

  if (m_colorchooser) delete m_colorchooser;

}

void AxisProps::slotSelected(int idx) {

  QVariant qv=m_selector->itemData(idx);
  aux::TypeId axid;
  QString qvval(qv.value<QString>());
  aux::Asciistr str;
  qtutil::fromQString(&qvval,&str);
  axid.fromString(str);
  simpleplot::Axis *ax=dynamic_cast<simpleplot::Axis*>(m_chart->axis(axid));
  m_combopos->setEnabled(ax!=0);
  m_buttonFontS->setEnabled(ax!=0);
  m_buttonFontL->setEnabled(ax!=0);
  m_buttoncolor->setEnabled(ax!=0);
  m_buttonAutoscaleOn->setEnabled(ax!=0);
  m_buttonAutoscaleOff->setEnabled(ax!=0);
  m_editMin->setEnabled(ax!=0);
  m_editMax->setEnabled(ax!=0);
  if (!ax) 
    return;
  
  m_combopos->clear();
  if (strcmp(axid.type(),"xaxis")==0) {
    qv.setValue(simpleplot::typeBottom);
    m_combopos->addItem("bottom",qv);
    qv.setValue(simpleplot::typeTop);
    m_combopos->addItem("top",qv);
  }
  else if (strcmp(axid.type(),"yaxis")==0) {
    qv.setValue(simpleplot::typeLeft);
    m_combopos->addItem("left",qv);
    qv.setValue(simpleplot::typeRight);
    m_combopos->addItem("right",qv);
  }
  qv.setValue(ax->pos());
  m_combopos->setCurrentIndex(m_combopos->findData(qv));
  
  m_fntS=ax->m_fnt[0];
  m_fntL=ax->m_fnt[1];
    
  m_color=ax->m_style.m_color;
  qv.setValue(ax->m_style.m_style);
  m_combostyle->setCurrentIndex(m_combostyle->findData(qv));
  m_buttoncolor->setColor(ax->m_style.m_color);
  m_editwidth->setText(QString::number(ax->m_style.m_width));
  double bb0=.0,bb1=1.;
  if (ax->scale()) 
    ax->scale()->effRange(&bb0,&bb1);
  aux::Asciistr numstr;
  aux::d2a(bb0,&numstr,aux::mag(bb0)+15);
  m_editMin->setText((const char *)numstr);
  numstr=0;
  aux::d2a(bb1,&numstr,aux::mag(bb0)+15);
  m_editMax->setText((const char *)numstr);
  if ((ax->scale()->rangeOption()&(simpleplot::typeBoundAutoMin|simpleplot::typeBoundAutoMax))>0) {
    m_buttonAutoscaleOn->toggle();
    m_editMin->setEnabled(false);
    m_editMax->setEnabled(false);
  }
  else 
    m_buttonAutoscaleOff->toggle();
  m_comboscaletype->clear();
  int ii=0;
  for (ii=0;ii<simpleplot::numscaletypes;ii++) {
    qv.setValue(QString(simpleplot::scaletypes[ii]));
    m_comboscaletype->addItem(simpleplot::scaletypes[ii],qv);
  }
  if (!ax->scale()) 
    m_comboscaletype->setCurrentIndex(0);
  else {
    qv.setValue(QString(ax->scale()->type()));
    m_comboscaletype->setCurrentIndex(m_comboscaletype->findData(qv));
  }

}

void AxisProps::slotFontS() {

  if (m_fontdlg) 
    return;
  m_fontdlg=new QFontDialog(this);
  m_fontdlg->setAttribute(Qt::WA_DeleteOnClose);
  m_fontdlg->setOptions(QFontDialog::DontUseNativeDialog);
  QFont qfnt((const char*)m_fntS.m_fam,(int)m_fntS.m_size);
  if ((m_fntS.m_style&2)>0) 
    qfnt.setBold(true);
  if ((m_fntS.m_style&1)>0) 
    qfnt.setItalic(true);
  m_fontdlg->setCurrentFont(qfnt);
  connect(m_fontdlg,SIGNAL(rejected()),this,SLOT(slotFontDismiss()));
  connect(m_fontdlg,SIGNAL(accepted()),this,SLOT(slotSetFontS()));
  m_fontdlg->show();

}

void AxisProps::slotFontL() {

  if (m_fontdlg) 
    return;
  m_fontdlg=new QFontDialog(this);
  m_fontdlg->setAttribute(Qt::WA_DeleteOnClose);
  m_fontdlg->setOptions(QFontDialog::DontUseNativeDialog);
  QFont qfnt((const char *)m_fntL.m_fam,(int)m_fntL.m_size);
  if ((m_fntL.m_style&2)>0) 
    qfnt.setBold(true);
  if ((m_fntL.m_style&1)>0) 
    qfnt.setItalic(true);
  m_fontdlg->setCurrentFont(qfnt);
  connect(m_fontdlg,SIGNAL(rejected()),this,SLOT(slotFontDismiss()));
  connect(m_fontdlg,SIGNAL(accepted()),this,SLOT(slotSetFontL()));
  m_fontdlg->show();

}

void AxisProps::slotSetFontS() {

  QFont qfnt=m_fontdlg->selectedFont();
  QString qstr(qfnt.family());
  aux::Asciistr str;
  qtutil::fromQString(&qstr,&str);
  m_fntS.m_fam=str;
  m_fntS.m_size=qfnt.pointSize();
  if (qfnt.weight()!=50) 
    m_fntS.m_style=2;
  if (qfnt.italic()) 
    m_fntS.m_style|=1;
  QFontMetrics metrics(qfnt);
  m_fntS.m_metric=osix::xxRectSize(metrics.width("0"),metrics.height());
  slotFontDismiss();

}

void AxisProps::slotSetFontL() {

  QFont qfnt=m_fontdlg->currentFont();
  QString qstr(qfnt.family());
  aux::Asciistr str;
  qtutil::fromQString(&qstr,&str);
  m_fntS.m_fam=str;
  m_fntL.m_size=qfnt.pointSize();
  if (qfnt.weight()!=50) 
    m_fntL.m_style=2;
  if (qfnt.italic()) 
    m_fntL.m_style|=1;
  QFontMetrics metrics(qfnt);
  m_fntL.m_metric=osix::xxRectSize(metrics.width("0"),metrics.height());
  slotFontDismiss();

}

void AxisProps::slotAutoscale(bool) {

  m_editMin->setEnabled(m_buttonAutoscaleOff->isChecked());
  m_editMax->setEnabled(m_buttonAutoscaleOff->isChecked());

}

void AxisProps::slotColor() {
 
  if (m_colorchooser) 
    return;
  m_colorchooser=new qtutil::ColorChooser(this,m_color);
  connect(m_colorchooser,SIGNAL(dismiss()),this,SLOT(slotColorchooserDismiss()));
  connect(m_colorchooser,SIGNAL(setColor(unsigned int)),this,SLOT(slotSetColor(unsigned int)));
  m_colorchooser->show();

}

void AxisProps::slotSetColor(unsigned int color) {

  m_color=color;
  m_buttoncolor->setColor(m_color);
    
}

void AxisProps::sendValues() {

  QVariant qv=m_selector->itemData(m_selector->currentIndex());
  QString qvval(qv.value<QString>());
  aux::Asciistr str;
  qtutil::fromQString(&qvval,&str);
  aux::TypeId axid;
  axid.fromString(str);
  simpleplot::Axis *ax=dynamic_cast<simpleplot::Axis*>(m_chart->axis(axid));
  if (!ax) 
    return;
  qv=m_combopos->itemData(m_combopos->currentIndex());
  ax->setPos(qv.value<int>());
  ax->m_fnt[0]=m_fntS;
  ax->m_fnt[1]=m_fntL;
  qv=m_combostyle->itemData(m_combostyle->currentIndex());
  ax->m_style.m_style=(short)qv.value<int>();
  ax->m_style.m_color=m_color;
  qvval=m_editwidth->text();
  qtutil::fromQString(&qvval,&str);
  ax->m_style.m_width=aux::a2d((const char *)str);
  qv=m_comboscaletype->itemData(m_comboscaletype->currentIndex());
  if (!ax->scale() || qv.toString()!=ax->scale()->type()) {
    qvval=qv.toString();
    qtutil::fromQString(&qvval,&str); 
    ax->setScale(simpleplot::buildScale((const char *)str));
  }
  int autoboundsflag=0;
  if (m_buttonAutoscaleOn->isChecked())
    autoboundsflag=simpleplot::typeBoundAutoMin|simpleplot::typeBoundAutoMax;
  if (ax->scale()) 
    ax->scale()->setRange(m_editMin->text().toDouble(),m_editMax->text().toDouble(),autoboundsflag);
  
  m_chart->redraw(simpleplot::redoScaleRect|simpleplot::redoDrawAxes|simpleplot::redoScr);

}


Controls::Controls(QWidget *parent,QtDiagramXY *chart) : 
  QDialog(parent,Qt::WindowStaysOnTopHint) {

  setAttribute(Qt::WA_DeleteOnClose);
  
  m_layout=new QVBoxLayout(this);
  m_layout->setSpacing(0); 
  m_layout->setMargin(5);
  m_tabwidget=new QTabWidget(this);
  m_tabGraph=new GraphProps(0,chart);
  m_tabwidget->addTab(m_tabGraph,"graph");
  m_tabAxis=new AxisProps(0,chart);
  m_tabwidget->addTab(m_tabAxis,"axis");
  connect (m_tabwidget,SIGNAL(currentChanged(int)),this,SLOT(slotCurrentChanged(int)));
  m_layout->addWidget(m_tabwidget);
  m_layout->addStretch(1);
  m_buttonSend=new QPushButton("apply",this);
  m_buttonSend->setAutoDefault(false);
  m_buttonSend->setDefault(false);
  connect(m_buttonSend,SIGNAL(clicked()),this,SLOT(slotSendValues()));
  m_layout->addWidget(m_buttonSend);
  slotCurrentChanged(0);
  
}

Controls::~Controls() {

  //printf ("%d destr\n",__LINE__);

}

void Controls::slotCurrentChanged(int curr) {

  QSize sz=(curr==0 ? m_tabGraph->sizeHint() : m_tabAxis->sizeHint());
  sz.setWidth(sz.width()+10);
  sz.setHeight(sz.height()+50);
  m_tabwidget->resize(sz);
  //resize(sizeHint());

}

void Controls::slotSendValues() {

  int idx=m_tabwidget->currentIndex();
  QWidget *ww=m_tabwidget->currentWidget();
  if (idx==0) 
    ((GraphProps*)ww)->sendValues();
  else if (idx==1) 
    ((AxisProps*)ww)->sendValues();
 
}

} // namespace
