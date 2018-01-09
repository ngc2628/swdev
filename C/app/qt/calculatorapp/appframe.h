#ifndef _qtcalculator_appframe_
#define _qtcalculator_appframe_

#include <auxx/auxx.h>
#include <qt/util/qtwidgets.h>
#include <qt/calculator/calculator.h>
#include <stdio.h>
#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTimer>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>

namespace qtcalculator {

class AppFrame : public QMainWindow {

  Q_OBJECT

  protected:
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolbar;
    QMenu *filemenu;
    QMenu *helpmenu;
    QMenu *miscmenu;
    QtCalculator *m_calculator;

  public:
    AppFrame(QWidget *parent=0);
    virtual ~AppFrame();
    bool eventFilter(QObject *,QEvent *);
    
  public slots:
    void slotDefResizeAction();
    void slotHelpAction();
    void slotOpenAction();
    void slotNewAction();
    void slotSaveAction();
    void slotPrintAction();
    void slotSaveAsAction();
    void slotQuitAction();
    void slotCutAction();
    void slotPasteAction();
    void slotCopyAction();
    void slotLFontAction();
    void slotSFontAction();
    void slotFgColorAction();
    void slotBgColorAction();

};

} // namespace

#endif

