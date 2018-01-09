
#include <osix/xxstyle.h>
#include <spreadsheet/spreadsheetdata.h>
#include <qt/util/qtutil.h>
#include <qt/spreadsheet/spreadsheet.h>
#include <app/qt/spreadsheetapp/appframe.h>
#include <math.h>

#include <QtCore/QEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QStyle>
#include <QtWidgets/QMessageBox>
#include <QtGui/QClipboard>

namespace qtspreadsheet {

// *****
QSize CustomButton::sizeHint() const {

  QSize sz=QPushButton::sizeHint();
  QFontMetrics metrics(font());
  sz.setWidth(2*metrics.width(text()));
//printf ("sz w=%d h=%d\n",sz.width(),sz.height());
  return sz;

}

// *****
QSize CustomButton::minimumSizeHint() const {

  QSize sz=QPushButton::sizeHint();
  QFontMetrics metrics(font());
  sz.setWidth(2*metrics.width(text()));
//printf ("minsz w=%d h=%d\n",sz.width(),sz.height());
  return sz;

}

// *****
void Canvas::resizeEvent(QResizeEvent *rev) {

  //printf ("canvas (%d,%d)\n",rev->size().width(),rev->size().height());
  if (table)
    table->resize(rev->size().width(),rev->size().height());

}

// *****
AppFrame :: AppFrame (QWidget *parent) : QMainWindow(parent) {

  //menubar = new QMenuBar(this);
  //toolbar = new QToolBar((QMainWindow *)this);
  //statusbar = new QStatusBar((QMainWindow *)this,"statusbar");

  //qApp->setStyle(new QWindowsStyle());

  central = new QTabWidget(this);
  int ii=0;
  for (ii=0;ii<ntabs;ii++) {
    tab[ii]=new QWidget;
    tablayout[ii]=new QGridLayout(tab[ii]);
    tablayout[ii]->setSpacing(0);
    tablayout[ii]->setMargin(0);
    canvas[ii]=new Canvas(tab[ii]);
    tablayout[ii]->addWidget(canvas[ii],0,0);
    central->addTab(tab[ii],ii==0 ? "spreadsheet" : "tab"+QString::number(ii));
    spreadsheets[ii]=0;
    spreadsheetsdata[ii]=0;
  }

  menubar=menuBar();
  filemenu = new QMenu("File",menubar);
  miscmenu = new QMenu("Misc",menubar);
  helpmenu = new QMenu("Help",menubar);
  spreadsheetmenu = new QMenu("spreadsheet",menubar);

  filemenu->addAction("&New",this,SLOT (slotNewAction()));
  filemenu->addAction("&Open",this,SLOT (slotOpenAction()));
  filemenu->addAction("&Save",this,SLOT (slotSaveAction()));
  filemenu->addAction("Save &As",this,SLOT (slotSaveAsAction()));
  filemenu->addAction("&Print",this,SLOT (slotPrintAction()));
  filemenu->addAction("&Quit",this,SLOT (slotQuitAction()));

  miscmenu->addAction("&cut",this,SLOT(slotCutAction()));
  miscmenu->addAction("&paste",this,SLOT(slotPasteAction()));
  miscmenu->addAction("&copy",this,SLOT(slotCopyAction()));

  helpmenu->addAction("&About",this,SLOT (slotHelpAction()));

  spreadsheetmenu->addAction("spreadsheet1",this,SLOT (slotSpreadsheetAction()));
  spreadsheetmenu->addAction("dataresize",this,SLOT (slotSpreadsheetDataResizeAction()));
  spreadsheetmenu->addAction("setpos",this,SLOT (slotSetPosAction()));
  spreadsheetmenu->addAction("clear",this,SLOT (slotClearAction()));

  menubar->addMenu(filemenu);
  menubar->addMenu(miscmenu);
  menubar->addMenu(spreadsheetmenu);
  menubar->addMenu(helpmenu);

  setCentralWidget(central);
  resize(minimumSizeHint());

}

// *****
AppFrame :: ~AppFrame() {

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

  return QMainWindow::eventFilter(obj,ev);;

}

// *****
void AppFrame::slotNewAction() {

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
    QMessageBox::information(this,"About","spreadsheet",(int)QMessageBox::Ok);

}

// *****
void AppFrame::resizeEvent(QResizeEvent *rev) {

  QString objname=objectName();
  printf ("%d resize %s %d,%d\n",__LINE__,qtutil::qasciistr(&objname),rev->size().width(),rev->size().height());
  QMainWindow::resizeEvent(rev);

}

// *****
void AppFrame::slotCopyAction() {

    QClipboard *clip=QApplication::clipboard();
    if (clip)
      clip->setText("copy");

}

// *****
void AppFrame::slotCutAction() {

    QClipboard *clip=QApplication::clipboard();
    if (clip)
      clip->setText("cut");

}

// *****
void AppFrame::slotPasteAction() {

    QClipboard *clip=QApplication::clipboard();
    if (!clip) return;

}

// *****************************************************************

// *****
void AppFrame::slotClearAction() {


}

// *****
void AppFrame::slotSpreadsheetAction() {

  if (spreadsheets[0])
    return;
  spreadsheets[0]=new QtSpreadsheet((QFrame*)canvas[0]);
  slotSpreadsheetDataResizeAction();
  canvas[0]->table=spreadsheets[0];
  spreadsheets[0]->resize(canvas[0]->size());
  spreadsheets[0]->show();

}

// *****
void AppFrame::slotSpreadsheetDataResizeAction() {

  int oldrows=0,oldcols=0;
  bool initial=(spreadsheetsdata[0]==0);
  if (!initial) {
    oldrows=spreadsheetsdata[0]->nrows();
    oldcols=spreadsheetsdata[0]->ncols();
  }
  //spreadsheets[0]->setSpreadsheetData(0);
  //printf ("nodata\n");

  QDialog resizedialog(spreadsheets[0]);
  QGridLayout rdlayout(&resizedialog);
  rdlayout.setSpacing(0);
  rdlayout.setMargin(0);
  QLabel labrows("rows",&resizedialog);
  rdlayout.addWidget(&labrows,0,0);
  QLineEdit lerows(&resizedialog);
  rdlayout.addWidget(&lerows,0,1);
  QLabel labcols("cols",&resizedialog);
  rdlayout.addWidget(&labcols,1,0);
  QLineEdit lecols(&resizedialog);
  rdlayout.addWidget(&lecols,1,1);
  QPushButton bok("ok",&resizedialog);
  connect(&bok,SIGNAL(clicked()),&resizedialog,SLOT(accept()));
  rdlayout.addWidget(&bok,2,0);
  QPushButton bcancel("cancel",&resizedialog);
  connect(&bcancel,SIGNAL(clicked()),&resizedialog,SLOT(reject()));
  rdlayout.addWidget(&bcancel,2,1);
  resizedialog.resize(100,100);
  int res=resizedialog.exec();
  if (!res)
    return;
  int rows=lerows.text().toInt();
  int cols=lecols.text().toInt();
  
  int ii=0,jj=0;
  if (initial)
    spreadsheetsdata[0]=new spreadsheet::SpreadsheetData();
  if (spreadsheetsdata[0]->setDimension(rows,cols)<0)
    return;
  
  int idxtype=(spreadsheet::sectiontypeCol|spreadsheet::sectiontypeHeader|spreadsheet::sectiontypeStatic);
  spreadsheet::SpreadsheetIndex section(idxtype,0,aux::dnan);
  spreadsheetsdata[0]->setIndexDescr(section);
  idxtype=(spreadsheet::sectiontypeCol|spreadsheet::sectiontypeStatic);
  section.set(idxtype,1,aux::dnan);
  spreadsheetsdata[0]->setIndexDescr(section);
  section.set(idxtype,cols-1,135.);
  spreadsheetsdata[0]->setIndexDescr(section);
  //spreadsheetsdata[0]->setColDescr(cols-2,-1.,1,"col "+QString::number(cols-2));
  idxtype=(spreadsheet::sectiontypeRow|spreadsheet::sectiontypeHeader|spreadsheet::sectiontypeStatic);
  section.set(idxtype,0,aux::dnan);
  spreadsheetsdata[0]->setIndexDescr(section);
  idxtype=(spreadsheet::sectiontypeRow|spreadsheet::sectiontypeStatic);
  section.set(idxtype,1,aux::dnan);
  spreadsheetsdata[0]->setIndexDescr(section);
  section.set(idxtype,rows-1,aux::dnan);
  spreadsheetsdata[0]->setIndexDescr(section);
  //spreadsheetsdata[0]->setRowDescr(rows-2,-1.,1,"row "+QString::number(rows-2));
  
  idxtype=spreadsheet::sectiontypeRow;
  section.set(idxtype,rows/2);
  spreadsheetsdata[0]->indexDescr(&section);
  section.m_gridstyle.set(osix::xx_somecolors[osix::green],1,3);
  spreadsheetsdata[0]->setIndexDescr(section);
  idxtype=spreadsheet::sectiontypeCol;
  section.set(idxtype,cols/2);
  spreadsheetsdata[0]->indexDescr(&section);
  section.m_gridstyle.set(osix::xx_somecolors[osix::blue],1,5);
  spreadsheetsdata[0]->setIndexDescr(section);
  
  aux::Ucsstr txt;
  txt.reserve(128);
  aux::Asciistr strnum;
  strnum.reserve(65);
  spreadsheet::SpreadsheetDataItem *itm=0;
  for(ii=oldrows;ii<rows;ii++) {
    for (jj=0;jj<cols;jj++) {
      txt=(const char *)0;
      if (ii==0 && jj==0)
        txt="Tiger";
      else if (ii==0) {
        txt="col ";
        aux::i2a(jj,&strnum);
        txt.append((const char *)strnum);
      }
      else if (jj==0) {
        txt="row ";
        aux::i2a(ii,&strnum);
        txt.append((const char *)strnum);
      }
      else {
        txt="(";
        aux::i2a(ii,&strnum);
        txt.append((const char *)strnum);
        txt.append(",");
        aux::i2a(jj,&strnum);
        txt.append((const char *)strnum);
        txt.append(")");
      }
      if (ii>0 && jj>0 && (ii+jj)%5==0)
        continue;
      itm=new spreadsheet::SpreadsheetDataItem(txt);
      if (ii==5 && jj==4) {
        itm->m_style[2]=osix::xxStyle(osix::xx_somecolors[osix::red],1,3);
        itm->m_style[3]=osix::xxStyle(osix::xx_somecolors[osix::blue],1,3);
        itm->m_style[4]=osix::xxStyle(osix::xx_somecolors[osix::green],1,3);
        itm->m_style[5]=osix::xxStyle(osix::xx_somecolors[osix::magenta],1,3);
      }
      spreadsheetsdata[0]->setData(ii,jj,itm);
    }
  }
  for(ii=oldcols;ii<cols;ii++) {
    for (jj=0;jj<oldrows;jj++) {
      if (jj==0) {
        txt="col ";
        aux::i2a(ii,&strnum);
        txt.append((const char *)strnum);
      }
      else {
        txt="(";
        aux::i2a(jj,&strnum);
        txt.append((const char *)strnum);
        txt.append(",");
        aux::i2a(ii,&strnum);
        txt.append((const char *)strnum);
        txt.append(")");
      }
      itm=new spreadsheet::SpreadsheetDataItem(txt);
      spreadsheetsdata[0]->setData(jj,ii,itm);
      txt=(const char *)0;
    }
  }
  
  if (initial)
    spreadsheets[0]->setSpreadsheetData(spreadsheetsdata[0]);
  spreadsheets[0]->updateGeometry();
  
/*aux::Asciistr dbgstr;
spreadsheetsdata[0]->toString(&dbgstr);
printf ("%d data : \n%s\n",__LINE__,(const char *)dbgstr);*/

}

// *****
void AppFrame::slotSetPosAction() {

  QDialog setposdialog(spreadsheets[0]);
  QGridLayout rdlayout(&setposdialog);
  rdlayout.setSpacing(0);
  rdlayout.setMargin(0);
  QLabel labrow("row",&setposdialog);
  rdlayout.addWidget(&labrow,0,0);
  QLineEdit lerow(&setposdialog);
  rdlayout.addWidget(&lerow,0,1);
  QLabel labcol("col",&setposdialog);
  rdlayout.addWidget(&labcol,1,0);
  QLineEdit lecol(&setposdialog);
  rdlayout.addWidget(&lecol,1,1);
  QPushButton bok("ok",&setposdialog);
  connect(&bok,SIGNAL(clicked()),&setposdialog,SLOT(accept()));
  rdlayout.addWidget(&bok,2,0);
  QPushButton bcancel("cancel",&setposdialog);
  connect(&bcancel,SIGNAL(clicked()),&setposdialog,SLOT(reject()));
  rdlayout.addWidget(&bcancel,2,1);
  setposdialog.resize(100,100);
  int res=setposdialog.exec();
  if (!res)
    return;
  int row=lerow.text().toInt();
  int col=lecol.text().toInt();
  spreadsheets[0]->cellVisible(spreadsheet::Coords(row,col));

}

} // namespace qtspreadsheet
