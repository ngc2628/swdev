
#include <QtCore/QVariant>
#include <QtWidgets/QMessageBox>
#include <QtCore/QTextCodec>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
//#include <QtGui/QWindowsStyle>
//#include <QtGui/QCDEStyle>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFontDialog>
#include <math.h>
#include <numeric/interpolation.h>
#include <graphic/shapes/shape.h>
#include <graphic/shapes/ellipse.h>
#include <graphic/shapes/rect.h>
#include <graphic/charts/simpleplot/scale.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/charts/simpleplot/graph.h>
#include <app/qt/simpleplotapp/appframe.h>

Q_DECLARE_METATYPE(mk::TypeId)

namespace qtsimpleplot {


// *****
// AppFrame :: AppFrame (QWidget *parent) : QFrame(parent,Qt::Widget|Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint) {
AppFrame :: AppFrame (QWidget *parent,const char *datafile) : QFrame(parent,Qt::Window),
  m_menubar(0),m_statusbar(0),m_toolbar(0),m_sizegrip(0),m_layout(0),
  m_tabwidget(0),m_control(0),m_controlLayout(0),m_buttonDismiss(0),m_filemenu(0),m_helpmenu(0),
  m_miscmenu(0),m_chartmenu2(0),m_chartmenuInteractive(0) {

  mk_stringset(m_datafile,datafile);
  memset(&m_tab[0],0,ntabs*sizeof(QWidget*));
  mk_listalloc(&m_xaxes,sizeof(mk::TypeId),31);
  mk_listalloc(&m_yaxes,sizeof(mk::TypeId),31);
  mk_listalloc(&m_graphs,sizeof(mk::TypeId),31); 
  //qApp->setStyle(new QCDEStyle());    
  //tstfunc();
  
  m_layout=new QGridLayout(this);
  m_layout->setVerticalSpacing(0);
  
  m_menubar = new QMenuBar(this);
  m_layout->setMenuBar(m_menubar);
  m_filemenu = new QMenu("File",m_menubar);
  m_miscmenu = new QMenu("Misc",m_menubar);
  m_chartmenu2 = new QMenu("Chart",m_menubar);
  m_chartmenuInteractive = new QMenu("Interactive",m_menubar);
  m_helpmenu = new QMenu("Help",m_menubar);

  m_filemenu->addAction("&New",this,SLOT (slotNewAction()));
  m_filemenu->addAction("&Open",this,SLOT (slotOpenAction()));
  m_filemenu->addAction("&Save",this,SLOT (slotSaveAction()));
  m_filemenu->addAction("Save &As",this,SLOT (slotSaveAsAction()));
  m_filemenu->addAction("&Print",this,SLOT (slotPrintAction()));
  m_filemenu->addAction("&Quit",this,SLOT (slotQuitAction()));

  m_miscmenu->addAction("&cut",this,SLOT(slotCutAction()));
  m_miscmenu->addAction("&paste",this,SLOT(slotPasteAction()));
  m_miscmenu->addAction("&copy",this,SLOT(slotCopyAction()));
  
  m_chartmenu2->addAction("chart2",this,SLOT(slotChart2Action()));
  m_chartmenu2->addAction("load",this,SLOT(slotLoadChartAction()));
  m_chartmenu2->addAction("start1",this,SLOT(slotStart1Action()));
  m_chartmenu2->addAction("stop1",this,SLOT(slotStop1Action()));
  m_chartmenu2->addAction("start2",this,SLOT(slotStart2Action()));
  m_chartmenu2->addAction("stop2",this,SLOT(slotStop2Action()));
  m_chartmenu2->addAction("clear",this,SLOT(slotClearAction()));
  
  m_chartmenuInteractive->addAction("create",this,SLOT(slotChartInteractive()));

  m_helpmenu->addAction("&About",this,SLOT (slotHelpAction()));

  m_menubar->addMenu(m_filemenu);
  m_menubar->addMenu(m_miscmenu);
  m_menubar->addMenu(m_chartmenu2);
  m_menubar->addMenu(m_chartmenuInteractive);
  m_menubar->addMenu(m_helpmenu);

  m_tabwidget=new QTabWidget(this);
  m_layout->addWidget(m_tabwidget,0,0,10,20);
  int i=0;
  for (i=0;i<ntabs;i++) {
    m_tab[i]=new QWidget;
    m_tablayout[i]=new QGridLayout(m_tab[i]);
    m_tablayout[i]->setSpacing(0); 
    m_tablayout[i]->setMargin(20);
    m_tabwidget->addTab(m_tab[i],i==0 ? "plot" : (i==1 ? "draw" : "tab"+QString::number(i)));
    m_chart2[i]=0;
  }
  m_control=new QFrame(this);
  m_controlLayout=new QHBoxLayout(m_control);
  m_buttonDismiss=new QPushButton("dismiss",m_control);
  connect(m_buttonDismiss,SIGNAL(clicked()),this,SLOT(slotQuit()));
  m_controlLayout->addWidget(m_buttonDismiss);
  m_layout->addWidget(m_control,11,18,1,2);

  m_sizegrip=new QSizeGrip(0);
  //m_layout->addWidget(m_sizegrip);
  resize(minimumSizeHint());
  
  connect (&m_t1,SIGNAL(timeout()),this,SLOT(slotT1Action()));
  connect (&m_t2,SIGNAL(timeout()),this,SLOT(slotT2Action()));

}

// *****
AppFrame :: ~AppFrame()
{
    mk_listfree(&m_xaxes);
    mk_listfree(&m_yaxes);
    mk_listfree(&m_graphs);
}

// *****
bool AppFrame::eventFilter(QObject *obj,QEvent *ev) {

  QEvent::Type evType=ev->type();
  QMouseEvent *mev=0;
  QKeyEvent *kev=0;
  Qt::Key key=Qt::Key_unknown;

  switch (evType) {
    case QEvent::FocusIn:
      break;
    case QEvent::FocusOut:
      break;
    case QEvent::MouseButtonPress:
      mev=(QMouseEvent*)ev;
      break;
    case QEvent::MouseMove:
      break;
    case QEvent::MouseButtonRelease:
      break;
    case QEvent::KeyPress:
      kev=(QKeyEvent*)ev;
      key=(Qt::Key)(kev->key());
      break;
    case QEvent::KeyRelease:
      kev=(QKeyEvent*)ev;
      key=(Qt::Key)(kev->key());
      switch (key) {
        case Qt::Key_C:
          if (kev->modifiers() & Qt::ControlModifier) {
            slotCopyAction();
            return true;
          }
          break;
        case Qt::Key_V:
          if (kev->modifiers() & Qt::ControlModifier) {
            slotPasteAction();
            return true;
          }
          break;
        case Qt::Key_X:
          if (kev->modifiers() & Qt::ControlModifier) {
            slotCutAction();
            return true;
          }
          break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

  return QFrame::eventFilter(obj,ev);;

}

// *****
void AppFrame::slotNewAction() {

  QFontDialog *fntdlg=new QFontDialog(this);
  fntdlg->setOptions(QFontDialog::DontUseNativeDialog);
  fntdlg->exec();

}

// *****
void AppFrame::slotOpenAction()
{

}

// *****
void AppFrame::slotSaveAction()
{

}

// *****
void AppFrame::slotSaveAsAction()
{

}

// *****
void AppFrame::slotPrintAction()
{

}

// *****
void AppFrame::slotQuitAction()
{
  qApp->quit();
}

// *****
void AppFrame::slotHelpAction()
{
  QMessageBox::information(this,"About","plot",(int)QMessageBox::Ok);

}

// *****
void AppFrame::resizeEvent(QResizeEvent *rev) {

  //printf ("resize %s %d,%d\n",qqx(objectName()),rev->size().width(),rev->size().height());
  QFrame::resizeEvent(rev);

}

// *****
void AppFrame::slotCopyAction() {

  QClipboard *clip=QApplication::clipboard();
  if (!clip) 
    return;
  clip->setText("copy");

}

// *****
void AppFrame::slotCutAction() {

  QClipboard *clip=QApplication::clipboard();
  if (!clip) 
    return;
  clip->setText("cut");

}

// *****
void AppFrame::slotPasteAction() {

  QClipboard *clip=QApplication::clipboard();
  if (!clip) 
    return;

}

// *****************************************************************

void AppFrame::slotLoadChartAction() {

  int ii=0,jj=0,idx=m_tabwidget->currentIndex();
  if (idx<0 || idx>=ntabs || m_chart2[idx]) 
    return;

printf ("%d datafile=%s\n",__LINE__,(const char *)m_datafile);

  if (mk_stringlength(m_datafile)==0)
    return;

  double xx[256];
  memset(&xx[0],0,256*sizeof(double));
  double yy[256];
  memset(&yy[0],0,256*sizeof(double));
  double dd[10];
  memset(&dd[0],0,10*sizeof(double));
  char descr[16];
  memset(&descr[0],0,16);
  char buf[256];
  memset(&buf[0],0,256);
  char *tok=0;
  int cntx=0,cnty=0,cntplot=0;
  double minx=mk_dnan,maxx=mk_dnan,miny=mk_dnan,maxy=mk_dnan;

  FILE *fp=fopen((const char *)m_datafile,"r");
  if (!fp)
    return;
  while (fgets(buf,255,fp)) {
    memset(&descr[0],0,16);
    tok=strtok(buf," ");
    while (tok) {
      if (strlen(descr)==0)
        strcpy(descr,tok);
      else if (strcmp(descr,"x:")==0)
        xx[cntx++]=atof(tok);
      else if (strcmp(descr,"y:")==0)
        yy[cnty++]=atof(tok);
      else if (strcmp(descr,"minx:")==0) 
        minx=atof(tok);
      else if (strcmp(descr,"miny:")==0) 
        miny=atof(tok);
      else if (strcmp(descr,"maxx:")==0) 
        maxx=atof(tok);
      else if (strcmp(descr,"maxy:")==0) 
        maxy=atof(tok);
      else if (strcmp(descr,"cntplot:")==0) 
        cntplot=atoi(tok);
      tok=strtok(0," ");
    }
  }
  fclose(fp);

  /*for (ii=0;ii<cntx;ii++)
    printf ("%d x#%d=%f\n",__LINE__,ii,xx[ii]);
  for (ii=0;ii<cnty;ii++)
    printf ("%d y#%d=%f\n",__LINE__,ii,yy[ii]);
  printf ("%d minx=%f maxx=%f miny=%f maxy=%f cntplot=%d\n",__LINE__,minx,maxx,miny,maxy,cntplot);*/

  m_chart2[idx]=new QtDiagramXY(m_tab[idx]);
  m_tablayout[idx]->addWidget(m_chart2[idx],0,0,100,100);

  int boundsoption=(simpleplot::typeBoundStaticMin|simpleplot::typeBoundStaticMax);
  
  simpleplot::Xaxis *xax=new simpleplot::Xaxis();
  simpleplot::Scale *sc=new simpleplot::Scale();
  sc->setRange(minx,maxx,boundsoption);
  xax->setScale(sc);
  mk::TypeId tid=m_chart2[idx]->setAxis(xax);
  mk_listinsort(&m_xaxes,(void*)&tid);
  simpleplot::Yaxis *yax=new simpleplot::Yaxis();
  sc=new simpleplot::Scale();
  sc->setRange(miny,maxy,boundsoption);
  yax->setScale(sc);
  tid=m_chart2[idx]->setAxis(yax);
  mk_listinsort(&m_yaxes,(void*)&tid);
    
  int cntctrl=MIN(cntx,cnty);

  simpleplot::GraphData2 *graphdata=new simpleplot::GraphData2(cntctrl);
  graphdata->setSortype(0);
  mk_list vvL;
  mk_listalloc(&vvL,sizeof(mk_vertex),4*cntctrl);
  mk_vertexnan(vv);
  for (ii=0;ii<cntctrl;ii++) {
    vv[0]=xx[ii];
    vv[1]=yy[ii];
    jj=graphdata->setData(-1,vv);
    mk_listappend(&vvL,(void*)&vv);
  }

  shapes::Shape2 *mark=simpleplot::buildMarkshape2("triangle",5.);
  mark->setStyleO(osix::xxStyle(osix::xx_somecolors[osix::magenta],1,1));
  mark->setStyleF(osix::xxStyle(osix::xx_somecolors[osix::yellow],1,1));
  graphdata->setMark(0,mark);
  simpleplot::GraphXY *graph=new simpleplot::GraphXY();
  graph->m_graphdata=graphdata;

  mk_ulreal interpoloptL=num::interpolation_ctrl;
  graph->m_interpolation=new num::Polynomial(interpoloptL);
  graph->m_linestyle=osix::xxStyle(osix::xx_somecolors[osix::darkgreen],1,1);
  xax->assignGraph(graph);
  yax->assignGraph(graph);
  tid=m_chart2[idx]->setGraph(graph);
  mk_listinsort(&m_graphs,(void*)&tid);
 
  num::Polynomial polyinter1(interpoloptL);
  polyinter1.setCtrl(&vvL);
  mk_listclear(&vvL,0);
  polyinter1.coeff(.0,&vvL);
  for (ii=0;ii<vvL.count;ii++) {
    mk_listat(&vvL,ii,(void*)&vv);
    xx[ii]=vv[1];
    printf ("%d coeff#%d=%.15f\n",__LINE__,ii,xx[ii]);
  }

  interpoloptL=num::interpolation_eq;
  num::Polynomial polyinter2(interpoloptL);
  mk_listclear(&vvL,0);
  for (ii=1;ii<vvL.count;ii++) {
    xx[ii-1]=(double)ii*xx[ii];
    vv[0]=xx[ii-1];
    mk_listappend(&vvL,(void*)&vv);
  }
  
  polyinter2.setCtrl(&vvL);
  
  jj=polyinter2.rootsBrute(xx,minx,maxx);
  for (ii=0;ii<jj;ii++)
    printf ("%d root-%d=%.15f\n",__LINE__,ii,xx[ii]);
  
  m_chart2[idx]->show();

  mk_listfree(&vvL);


}

static int cnttstdata=19;

/* ***** */
void AppFrame::slotChart2Action() {

  int ii=0,jj=0,idx=m_tabwidget->currentIndex();
  if (idx<0 || idx>=ntabs || m_chart2[idx]) 
    return;
  mk_string str;
  mk_stringset(str,0);
  m_chart2[idx]=new QtDiagramXY(m_tab[idx]);
  m_tablayout[idx]->addWidget(m_chart2[idx],0,0,100,100);
  
  simpleplot::Xaxis *xax=new simpleplot::Xaxis();
  simpleplot::Scale *sc=new simpleplot::Scale();
  xax->setScale(sc);
  mk::TypeId tid=m_chart2[idx]->setAxis(xax);
  mk_listinsort(&m_xaxes,(void*)&tid);
  simpleplot::Yaxis *yax=new simpleplot::Yaxis();
  sc=new simpleplot::Scale();
  int boundsoption=(simpleplot::typeBoundStaticMin|simpleplot::typeBoundStaticMax);
  sc->setRange(-101.,101.,boundsoption);
  yax->setScale(sc);
  tid=m_chart2[idx]->setAxis(yax);
  mk_listinsort(&m_yaxes,(void*)&tid);
    
  simpleplot::GraphData2 *graphdata=new simpleplot::GraphData2(500);
  graphdata->setSortype(0);
  mk_vertex vv;
  for (ii=0;ii<cnttstdata;ii++) {
    vv[0]=(double)(ii*360/(cnttstdata-1));
    vv[1]=100.*sin((double)(ii*360/(cnttstdata-1))*mk_rad);
    if (ii==cnttstdata/2-1) 
      vv[1]=vv[1]-9.;
    jj=graphdata->setData(-1,vv);
  }

//mk::TVList<num::Vector3> dbgdata;
//graphdata->data(&dbgdata);
//for (i=0;i<dbgdata.count();i++) printf ("data-in %d  x=%f y=%f\n",i,dbgdata[i][0],dbgdata[i][1]);

  shapes::Shape2 *mark=0;
  mark=simpleplot::buildMarkshape2("triangle",5.);
  mark->setStyleO(osix::xxStyle(osix::xx_somecolors[osix::magenta],1,1));
  mark->setStyleF(osix::xxStyle(osix::xx_somecolors[osix::yellow],1,1));
    //mark->rotate(45.);
  graphdata->setMark(0,mark);
  simpleplot::GraphXY *graph=new simpleplot::GraphXY();
  graph->m_graphdata=graphdata;
  graph->m_interpolation=new num::CubicSpline(num::interpolation_solve2nd);
  graph->m_linestyle=osix::xxStyle(osix::xx_somecolors[osix::darkgreen],1,1);
  xax->assignGraph(graph);
  yax->assignGraph(graph);
  tid=m_chart2[idx]->setGraph(graph);
  mk_listinsort(&m_graphs,(void*)&tid);

  //m_chart2[idx]->show();
  //return;
  
  xax=new simpleplot::Xaxis();
  xax->setScale(new simpleplot::Scale());
  //xax->setPos(2);
  tid=m_chart2[idx]->setAxis(xax);
  mk_listinsort(&m_xaxes,(void*)&tid);
  yax=new simpleplot::Yaxis();
  yax->setScale(new simpleplot::Scale());
  //yax->setPos(8);
  tid=m_chart2[idx]->setAxis(yax);
  mk_listinsort(&m_yaxes,(void*)&tid);
    
  graphdata=new simpleplot::GraphData2(500);
  graphdata->setSortype(0);
  for (ii=0;ii<cnttstdata;ii++) {
    vv[0]=(double)(ii*360/(cnttstdata-1));
    vv[1]=50.*cos((double)(ii*360/(cnttstdata-1))*mk_rad);
    jj=graphdata->setData(-1,vv);
  }
  
//graphdata->data(&dbgdata);
//for (i=0;i<dbgdata.count();i++) printf ("data-in %d  x=%f y=%f\n",i,dbgdata[i][0],dbgdata[i][1]);
  
  mark=simpleplot::buildMarkshape2("circle",5.);
  mark->setStyleO(osix::xxStyle(osix::xx_somecolors[osix::darkgreen],1,1));
  mark->setStyleF(osix::xxStyle(osix::xx_somecolors[osix::orange],1,1));
  graphdata->setMark(0,mark);
  graph=new simpleplot::GraphXY();
  graph->m_graphdata=graphdata;
  graph->m_interpolation=new num::CubicSpline(num::interpolation_solve2nd);
  graph->m_linestyle=osix::xxStyle(osix::xx_somecolors[osix::blue],1,1);
  xax->assignGraph(graph);
  yax->assignGraph(graph);
  tid=m_chart2[idx]->setGraph(graph);
  mk_listinsort(&m_graphs,(void*)&tid);
    
  m_chart2[idx]->show();
  //m_t1.start(500);
  //m_t2.start(500);
  
  
}

// *****

static int t1cnt=0,t2cnt=0;

void AppFrame::slotT1Action() {

  int ii=0,jj=0,idx=m_tabwidget->currentIndex();
  if (idx<0 || idx>=ntabs || !m_chart2[idx]) 
    return;
  mk_list grL;
  mk_listalloc(&grL,sizeof(mk::TypeId),0);
  mk::TypeId tid;
  simpleplot::Graph *gr=0;
  mk_vertexnan(vv);
  int ngr=m_chart2[idx]->entities(&grL,"graphxy"),modbounds=0,modb=0;
  for (ii=0;ii<ngr;ii++) {
    mk_listat(&grL,ii,(void*)&tid);
    gr=m_chart2[idx]->graph(tid);
    for (jj=0;jj<cnttstdata;jj++) {
      vv[0]=(double)(t1cnt+jj*360/(cnttstdata-1));
      vv[1]=100.*(ii%2==1 ? cos((double)(t1cnt+jj*360/(cnttstdata-1))*mk_rad) : sin((double)(t1cnt+jj*360/(cnttstdata-1))*mk_rad));
      if (gr && t1cnt==20) {
        gr->setValue(jj,vv,&modb);
        if (modb>modbounds) 
          modbounds=modb;
      }
      else if (gr && gr->m_graphdata) 
        gr->m_graphdata->setData(jj,vv);
    } 
  }
  if (modbounds==0) {
    for (ii=0;ii<ngr;ii++) {
      mk_listat(&grL,ii,(void*)&tid);
      gr=m_chart2[idx]->graph(tid);
      if (gr)
        gr->rescale();
    }
  }
  else 
    ;//m_chart2[idx]->resetRect();
  m_chart2[idx]->redraw(simpleplot::redoDrawAxes|simpleplot::redoDrawGraph|simpleplot::redoScr);
  t1cnt++;
  mk_listfree(&grL);

}

void AppFrame::slotT2Action() {

  int idx=m_tabwidget->currentIndex();
  if (idx<0 || idx>=ntabs || !m_chart2[idx]) 
    return;
  mk_list xaxL;
  mk_listalloc(&xaxL,sizeof(mk::TypeId),0);
  mk_list yaxL;
  mk_listalloc(&yaxL,sizeof(mk::TypeId),0);
  mk::TypeId tid;
  simpleplot::Axis *ax=0;
  int ii=0,nxax=m_chart2[idx]->entities(&xaxL,"xaxis"),
      nyax=m_chart2[idx]->entities(&yaxL,"yaxis"),nn=nxax+nyax;
  for (ii=0;ii<nxax;ii++) {
    mk_listat(&xaxL,ii,(void*)&tid);
    ax=m_chart2[idx]->axis(tid);
    if (ax && t2cnt%2) 
      ax->m_style.m_color=osix::xx_somecolors[ii%osix::xx_cntcolors];
    else if (ax)
      ax->m_style.m_color=osix::xx_somecolors[(ii+1)%osix::xx_cntcolors];
  }
  for (ii=0;ii<nyax;ii++) {
    mk_listat(&yaxL,ii,(void*)&tid);
    ax=m_chart2[idx]->axis(tid);
    if (ax && t2cnt%2)
      ax->m_style.m_color=osix::xx_somecolors[(ii+nn)%osix::xx_cntcolors];
    else if (ax)
      ax->m_style.m_color=osix::xx_somecolors[(ii+nn+1)%osix::xx_cntcolors];
  }
  mk_list grL;
  mk_listalloc(&grL,sizeof(mk::TypeId),0);
  simpleplot::Graph *gr=0;
  int ngr=m_chart2[idx]->entities(&grL,"graphxy");
  for (ii=0;ii<ngr;ii++) {
    mk_listat(&grL,ii,(void*)&tid);
    gr=m_chart2[idx]->graph(tid);
    if (gr)
      gr->m_linestyle.m_color=osix::xx_somecolors[t2cnt%osix::xx_cntcolors];
  }
  
  m_chart2[idx]->redraw(simpleplot::redoDrawAxes|simpleplot::redoDrawGraph|simpleplot::redoScr);
  t2cnt++;
  mk_listfree(&xaxL);
  mk_listfree(&yaxL);
  mk_listfree(&grL);

}

void AppFrame::slotStart1Action() {
  m_t1.start(500);
}

void AppFrame::slotStop1Action() {
  m_t1.stop();

}

void AppFrame::slotStart2Action() {
  m_t2.start(500);
}

void AppFrame::slotStop2Action() {
  m_t2.stop();
}

// *****
void AppFrame::slotClearAction() {

  m_t1.stop();
  m_t2.stop();
  t1cnt=t2cnt=0;
  int idx=m_tabwidget->currentIndex();
  if (idx<0 || idx>=ntabs || !m_chart2[idx]) 
    return;
  mk::TypeId tid;
  int ii=0;
  simpleplot::Graph *gr=0;
  for (ii=m_graphs.count-1;ii>-1;ii--) {
    mk_listremove(&m_graphs,ii,(void*)&tid);
    gr=m_chart2[idx]->removeGraph(tid);
    if (gr)
      delete gr;
  }
  simpleplot::Axis *ax=0;
  for (ii=m_xaxes.count-1;ii>-1;ii--) {
    mk_listremove(&m_xaxes,ii,(void*)&tid);
    ax=m_chart2[idx]->removeAxis(tid);
    if (ax)
      delete ax;
  }
  for (ii=m_yaxes.count-1;ii>-1;ii--) {
    mk_listremove(&m_yaxes,ii,(void*)&tid);
    ax=m_chart2[idx]->removeAxis(tid);
    if (ax)
      delete ax;
  }
  delete m_chart2[idx];
  m_chart2[idx]=0;

}

/* ***** */
void AppFrame::slotChartInteractive() {

  int idx=m_tabwidget->currentIndex();
  if (idx<0 || idx>=ntabs || m_chart2[idx]) 
    return;
  m_chart2[idx]=new QtDiagramXY(this);
  
  int boundsoption=0;
  simpleplot::Xaxis *xax=new simpleplot::Xaxis();
  simpleplot::Scale *sc=new simpleplot::Scale();
  sc->setRange(0.,20.,boundsoption);
  xax->setScale(sc);
  mk::TypeId tid=m_chart2[idx]->setAxis(xax);
  mk_listinsort(&m_xaxes,(void*)&tid);
  simpleplot::Yaxis *yax=new simpleplot::Yaxis();
  sc=new simpleplot::Scale();
  sc->setRange(0.,10.,boundsoption);
  yax->setScale(sc);
  tid=m_chart2[idx]->setAxis(yax);
  mk_listinsort(&m_yaxes,(void*)&tid);
  
  simpleplot::GraphData2 *graphdata=new simpleplot::GraphData2(500);
  graphdata->setSortype(0);
  mk_vertexnan(vv);
  vv[0]=3.;
  vv[1]=4.;
  graphdata->setData(-1,vv);
  vv[0]=7.;
  vv[1]=6.;
  graphdata->setData(-1,vv);
  vv[0]=9.;
  vv[1]=5.;
  graphdata->setData(-1,vv);
  vv[0]=11.;
  vv[1]=2.;
  graphdata->setData(-1,vv);
  vv[0]=15.;
  vv[1]=9.;
  graphdata->setData(-1,vv);
  shapes::Shape2 *mark=simpleplot::buildMarkshape2("circle",5.);
  mark->setStyleO(osix::xxStyle(osix::xx_somecolors[osix::darkgreen],1,1));
  mark->setStyleF(osix::xxStyle(osix::xx_somecolors[osix::orange],1,1));
  graphdata->setMark(0,mark);
  simpleplot::GraphXY *graph=new simpleplot::GraphXY();
  graph->m_graphdata=graphdata;
  graph->m_interpolation=new num::CubicSpline;
  graph->m_linestyle=osix::xxStyle(osix::xx_somecolors[osix::blue],1,1);
  xax->assignGraph(graph);
  yax->assignGraph(graph);
  tid=m_chart2[idx]->setGraph(graph);
  mk_listinsort(&m_graphs,(void*)&tid);
    
  m_chart2[idx]->redraw(simpleplot::redoDrawAxes|simpleplot::redoDrawGraph|simpleplot::redoScr);
  
}

TstFrame :: TstFrame (QWidget *parent) : QFrame(parent,Qt::Window) {

  //qApp->setStyle(new QCDEStyle());    
  tstfunc();
  
  m_layout=new QVBoxLayout(this);
  m_buttonDismiss=new QPushButton("dismiss",this);
  connect(m_buttonDismiss,SIGNAL(clicked()),this,SLOT(slotQuit()));
  m_layout->addWidget(m_buttonDismiss);
  
  m_menubar = new QMenuBar(this);
  m_filemenu = new QMenu("tst1",m_menubar);
  m_miscmenu = new QMenu("tst2",m_menubar);
  m_helpmenu = new QMenu("tst3",m_menubar);

  m_filemenu->addAction("&jnk",this,SLOT (slotQuit()));
  
  m_miscmenu->addAction("&misc",this,SLOT(slotQuit()));
   
  m_helpmenu->addAction("&AboutTst",this,SLOT (slotQuit()));

  m_menubar->addMenu(m_filemenu);
  m_menubar->addMenu(m_miscmenu);
  m_menubar->addMenu(m_helpmenu);

  m_sizegrip=new QSizeGrip(0);
  //m_layout->addWidget(m_sizegrip);
  resize(minimumSizeHint());
  
}

// *****
TstFrame :: ~TstFrame()
{
    // destroySomeThing
}

// *****

void tstfunc() {

  const char * const tstchar="hello-martinworld0004060019";
  mk::TypeId tstid;
  bool ok=tstid.fromString(tstchar);
  mk_string str;
  tstid.toString(str);
  printf ("ok=%d tstid=%s\n",ok,&str[0]);

  QFont f("Arial");
  QFontMetrics metrics(f);
  int i=0;
  for (i=1;i<101;i++) {
    f.setPixelSize(i);
    metrics=QFontMetrics(f);
    printf ("sz=%d h=%d w=%d\n",i,metrics.size(Qt::TextSingleLine,"0").height(),metrics.size(Qt::TextSingleLine,"0").width());
  }

}

} // namespace qtsimpleplot
