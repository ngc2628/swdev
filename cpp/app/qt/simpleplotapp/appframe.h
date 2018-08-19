
#ifndef _APPFRAME_H_
#define _APPFRAME_H_

#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>
#include <QtWidgets/QLayout>
#include <QtCore/QTimer>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QSizeGrip>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QColor>
#include <QtCore/QStack>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QDialog>
#include <auxx/auxx.h>
#include <qt/simpleplot/chart2.h> 
#include <qt/simpleplot/controls.h>

namespace qtsimpleplot {

const int ntabs=3; 

class AppFrame : public QFrame {
    
  Q_OBJECT
    
  public:
    AppFrame (QWidget *parent=0,aux::Asciistr datafile=0);
    virtual ~AppFrame ();
    bool eventFilter(QObject *,QEvent *);
    void resizeEvent(QResizeEvent *);

  public slots:
    void slotQuit() { qApp->quit(); }
    void slotOpenAction();
    void slotNewAction();
    void slotSaveAction();
    void slotPrintAction();
    void slotSaveAsAction();
    void slotQuitAction();
    void slotCopyAction();
    void slotCutAction();
    void slotPasteAction();
    void slotHelpAction();
    void slotChart2Action();
    void slotLoadChartAction();
    void slotClearAction();
    void slotT1Action();
    void slotT2Action();
    void slotStart1Action();
    void slotStop1Action();
    void slotStart2Action();
    void slotStop2Action();
    
    void slotChartInteractive();
           
  protected:
    aux::Asciistr m_datafile;

    QMenuBar *m_menubar;
    QStatusBar *m_statusbar;
    QToolBar *m_toolbar;
    QSizeGrip *m_sizegrip;
    QGridLayout *m_layout;
    QTabWidget *m_tabwidget;
    QWidget *m_tab[ntabs];
    QFrame *m_control;
    QHBoxLayout *m_controlLayout;
    QPushButton *m_buttonDismiss;
    QGridLayout *m_tablayout[ntabs];
    QMenu *m_filemenu;
    QMenu *m_helpmenu;
    QMenu *m_miscmenu;
    QMenu *m_chartmenu2;
    QMenu *m_chartmenuInteractive;
    
    QTimer m_t1;
    QTimer m_t2;
    
    QtDiagramXY *m_chart2[ntabs];
    aux::TVList<aux::TypeId> m_xaxes;
    aux::TVList<aux::TypeId> m_yaxes;
    aux::TVList<aux::TypeId> m_graphs;
    
};

class TstFrame : public QFrame {
    
  Q_OBJECT
    
  public:
    TstFrame (QWidget *parent=0);
    virtual ~TstFrame ();
    
  public slots:
    void slotQuit() { qApp->quit(); }
       
  protected:
    QMenuBar *m_menubar;
    QSizeGrip *m_sizegrip;
    QVBoxLayout *m_layout;
    QPushButton *m_buttonDismiss;
    QMenu *m_filemenu;
    QMenu *m_helpmenu;
    QMenu *m_miscmenu;
    
};

void tstfunc();

} // namespace qtsimpleplot

#endif
