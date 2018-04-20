
#include <app/qt/txtcalculatorapp/appframe.h>
#include <QtWidgets/QMessageBox>

namespace qtcalculator {

AppFrame::AppFrame(QWidget *parent) : QMainWindow(parent),menubar(0), statusbar(0), toolbar(0) {

  menubar=menuBar();
  filemenu=new QMenu("File",menubar);
  miscmenu=new QMenu("Misc",menubar);
  helpmenu=new QMenu("Help",menubar);

  filemenu->addAction("&New",this,SLOT (slotNewAction()));
  filemenu->addAction("&Open",this,SLOT (slotOpenAction()));
  filemenu->addAction("&Save",this,SLOT (slotSaveAction()));
  filemenu->addAction("Save &As",this,SLOT (slotSaveAsAction()));
  filemenu->addAction("&Print",this,SLOT (slotPrintAction()));
  filemenu->addAction("&Quit",this,SLOT (slotQuitAction()));

  miscmenu->addAction("&cut",this,SLOT(slotCutAction()));
  miscmenu->addAction("&paste",this,SLOT(slotPasteAction()));
  miscmenu->addAction("&copy",this,SLOT(slotCopyAction()));
  miscmenu->addAction("&font+",this,SLOT(slotLFontAction()));
  miscmenu->addAction("&font-",this,SLOT(slotSFontAction()));
  miscmenu->addAction("&fgcolor(rgb)",this,SLOT(slotFgColorAction()));
  miscmenu->addAction("&bgcolor(rgb)",this,SLOT(slotBgColorAction()));
  helpmenu->addAction("&About",this,SLOT (slotHelpAction()));

  menubar->addMenu(filemenu);
  menubar->addMenu(miscmenu);
  menubar->addMenu(helpmenu);

  m_calculator=new QtCalculator(this);
  setCentralWidget(m_calculator);

}

AppFrame::~AppFrame() {

}

void AppFrame::slotNewAction() {

}

void AppFrame::slotOpenAction() {

}

void AppFrame::slotSaveAction() {

}

void AppFrame::slotSaveAsAction() {

}

void AppFrame::slotPrintAction() {

}

void AppFrame::slotQuitAction() {

  qApp->quit();

}

void AppFrame::slotHelpAction() {

  m_calculator->showHelp();

}

bool AppFrame::eventFilter(QObject *obj,QEvent *ev) {

  return false;

}

void AppFrame::slotCopyAction() {

  m_calculator->doCopy();

}

void AppFrame::slotCutAction() {

  m_calculator->doCut();

}

void AppFrame::slotPasteAction() {

  m_calculator->doPaste();

}

void AppFrame::slotFgColorAction() {


}

void AppFrame::slotBgColorAction() {


}

void AppFrame::slotLFontAction() {

  m_calculator->doFont(1.,1);
  QTimer::singleShot(50,this,SLOT(slotDefResizeAction()));

}

void AppFrame::slotSFontAction() {

  m_calculator->doFont(1.,2);
  QTimer::singleShot(50,this,SLOT(slotDefResizeAction()));

}

void AppFrame::slotDefResizeAction() {

  resize(minimumSizeHint());

}

} //namesapce
