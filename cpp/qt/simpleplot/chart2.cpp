
//#include <QtGui/QWindowsStyle>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#if defined (__linux__)
#include <QtX11Extras/QX11Info>
#endif
#include <qt/simpleplot/chart2.h>
#include <qt/simpleplot/controls.h>
#include <graphic/charts/simpleplot/scale.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/charts/simpleplot/graph.h>

namespace qtsimpleplot {

QtDiagramXY::QtDiagramXY(QWidget *parent,int sz) :
  QFrame(parent),simpleplot::DiagramXY(sz),
  m_layout(0),m_view(0),m_modX(0),m_modZ(0),m_modM(0),
  m_showcontrols(0),m_controlsDialog(0) {

  m_bg=osix::xxStyle(osix::xx_somecolors[osix::grey224],1,1);
  m_layout=new QGridLayout(this);
  m_layout->setSpacing(5);
  m_layout->setMargin(0);
  m_pixscr.m_t=osix::xx_pixmap;
  m_pixscr.m_w=(void*)(new QPixmap(10,10));
  m_pixplot.m_t=osix::xx_pixmap;
  m_pixplot.m_w=(void*)(new QPixmap(10,10));
  m_view=new PlotView(this,"plotview");
  m_winscr.m_t=osix::xx_window;
  m_winscr.m_w=(void*)m_view;
  m_layout->addWidget(m_view,0,0,20,20);
  m_modX=new QCheckBox("X",this);
  m_layout->addWidget(m_modX,0,21);
  connect(m_modX,SIGNAL(stateChanged(int)),this,SLOT(slotCheckX(int)));
  m_modZ=new QCheckBox("Z",this);
  m_layout->addWidget(m_modZ,1,21);
  connect(m_modZ,SIGNAL(stateChanged(int)),this,SLOT(slotCheckZ(int)));
  m_modM=new QCheckBox("M",this);
  m_layout->addWidget(m_modM,2,21);
  connect(m_modM,SIGNAL(stateChanged(int)),this,SLOT(slotCheckM(int)));
  m_showcontrols=new QCheckBox("C",this);
  m_layout->addWidget(m_showcontrols,3,21);
  connect(m_showcontrols,SIGNAL(stateChanged(int)),this,SLOT(slotControls(int)));
  setFocusProxy(m_view);
          	  
}

QtDiagramXY::QtDiagramXY(const QtDiagramXY &) : QFrame(),simpleplot::DiagramXY() {
    
}

QtDiagramXY::~QtDiagramXY() {

  if (m_pixplot.m_w)
    delete (QPixmap*)m_pixplot.m_w;
  if (m_pixscr.m_w)
    delete (QPixmap*)m_pixscr.m_w;

}

QtDiagramXY &QtDiagramXY::operator=(const QtDiagramXY &) {
      
  return *this;
  
}

void *QtDiagramXY::findDisplay() {

#if defined (__linux__)
  return (void*)QX11Info::display();
#endif
    
  return 0;

}

int QtDiagramXY::osUpdate(osix::xxRect rr) {

  QRectF qrf;
  qtutil::toQRectF(&rr,&qrf);
  if (rr.empty()!=0)
    m_view->update();
  else
    m_view->update(qrf.toRect());
  return 0;

} 

mk::TypeId QtDiagramXY::setAxis(simpleplot::Axis *axis) {

  mk::TypeId axid=DiagramXY::setAxis(axis);
  if (axid.busted()) 
    return axid;
  QFont fnt(font());
  fnt.setPointSizeF(12.);
  QString fntfam(fnt.family());
  QFontMetrics metrics(fnt);
  mk_string str;
  if (axis->m_fnt[1].m_metric.empty()) {
    qtutil::fromQString(&fntfam,str);
    axis->m_fnt[1]=osix::xxFnt(str,fnt.pointSizeF());
    axis->m_fnt[1].m_style=(fnt.weight()<<1);
    axis->m_fnt[1].m_style|=(int)(fnt.italic());
    axis->m_fnt[1].m_metric=osix::xxRectSize(metrics.width("0"),metrics.height());
  }
  if (axis->m_fnt[0].m_metric.empty()) {
    fnt.setPointSizeF(fnt.pointSizeF()-2.);
    axis->m_fnt[0]=osix::xxFnt(str,fnt.pointSizeF());
    axis->m_fnt[0].m_style=(fnt.weight()<<1);
    axis->m_fnt[0].m_style|=(int)(fnt.italic());
    metrics=QFontMetrics(fnt);
    axis->m_fnt[0].m_metric=osix::xxRectSize(metrics.width("0"),metrics.height());
  }
  return axid;
  
}

// *****
void QtDiagramXY::slotCheckX(int check) {

  int currmode=mouseMode();
  if (check)
    currmode|=simpleplot::mouseCrosshair;
  else
    currmode&=~simpleplot::mouseCrosshair;
  mouseMode(&currmode);

}

// *****
void QtDiagramXY::slotCheckZ(int check) {

  int currmode=mouseMode();
  if (check)
    currmode|=simpleplot::mouseZoom;
  else
    currmode&=~simpleplot::mouseZoom;
  mouseMode(&currmode);

}

// *****
void QtDiagramXY::slotCheckM(int check) {

  int currmode=mouseMode();
  if (check>0)
    currmode|=simpleplot::mouseModify;
  else
    currmode&=~simpleplot::mouseModify;
  mouseMode(&currmode);

}

void QtDiagramXY::slotControls(int check) {

  if (m_controlsDialog && check==0) {
    delete m_controlsDialog;
    m_controlsDialog=0; 
  }
  else if (!m_controlsDialog && check>0){
    m_controlsDialog=new Controls(this,this);
    connect(m_controlsDialog,SIGNAL(dismiss()),this,SLOT(slotControlsDismiss()));
    m_controlsDialog->show();
  }

}

void QtDiagramXY::slotControlsDismiss() {

  m_controlsDialog=0;
  m_showcontrols->setChecked(0);

} 

// **********
PlotView::PlotView(QtDiagramXY *diagram,const char *name) : 
  QWidget((QWidget*)diagram),m_diagram(diagram) {

  setObjectName(name);
  //setStyle(new QWindowsStyle());
  setAttribute(Qt::WA_NoSystemBackground,true);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  installEventFilter(this);//qApp->installEventFilter(this);

}

// **********
PlotView::PlotView(const PlotView &) : QWidget() {
    
}

// **********    
PlotView &PlotView::operator=(const PlotView &) {

  return *this;

}

// **********
PlotView::~PlotView() {

}

// **********
bool PlotView::operator==(const PlotView &cmp) const {
      
  return (m_diagram==cmp.m_diagram);
  
}

// **********
bool PlotView::operator<(const PlotView &cmp) const {
      
  return (m_diagram<cmp.m_diagram);
  
}

// **********
bool PlotView::eventFilter(QObject *obj,QEvent *ev) {

  return QWidget::eventFilter(obj,ev);

}

// **********
bool PlotView::event(QEvent *qev) {

  QWidget::event(qev);
  int consumed=0;
  if (m_diagram)
    consumed=m_diagram->osEvent(m_diagram->findDisplay(),(void*)qev);
  if ((consumed&osix::xx_consumed)>0)
    return true;
  return false;

}

} // namespace plot
