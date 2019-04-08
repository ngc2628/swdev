
#ifndef _APPFRAME_H_
#define _APPFRAME_H_

#include <QtCore/QSize>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>

class QMenu;
class QMenuBar;
class QToolBar;
class QStatusbar;
class QTabWidget;
class QGridLayout;

class QtSpreadsheet;
namespace spreadsheet {
class SpreadsheetData;
}

namespace qtspreadsheet {

const int ntabs=3;

class CustomButton : public QPushButton {

  public:
    CustomButton(const QString &text,QWidget *parent=0) : QPushButton(text,parent) {
    }
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
        
}; 

class Canvas : public QFrame {

  public:
    QtSpreadsheet *table;
    Canvas(QWidget *parent) : QFrame(parent),table(0) {
    }
    virtual ~Canvas() {
    }
    void resizeEvent(QResizeEvent*);

  protected:
    Canvas(const Canvas &) : QFrame(0) {
    }
    Canvas &operator=(const Canvas &) {
      return *this;
    }

};

class AppFrame : public QMainWindow {
    
  Q_OBJECT
    
  public:
    AppFrame (QWidget *parent=0);
    virtual ~AppFrame ();
    bool eventFilter(QObject *,QEvent *);
    void resizeEvent(QResizeEvent *);

  public slots:
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
    
    void slotClearAction();
    void slotSpreadsheetAction();
    void slotSpreadsheetDataResizeAction();
    void slotSetPosAction();

  protected:
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolbar;
    QTabWidget *central;
    QWidget *tab[ntabs];
    QGridLayout *tablayout[ntabs];
    QMenu *filemenu;
    QMenu *helpmenu;
    QMenu *miscmenu;
    QMenu *spreadsheetmenu;
    
    Canvas *canvas[ntabs];
    QtSpreadsheet *spreadsheets[ntabs];
    spreadsheet::SpreadsheetData *spreadsheetsdata[ntabs];
    
};

} // namespace qtspreadsheet

#endif
