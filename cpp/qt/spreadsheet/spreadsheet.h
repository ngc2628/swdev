
#ifndef _spreadsheet_qt_
#define _spreadsheet_qt_

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QRect>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLayout>
#include <QtWidgets/QScrollBar>
#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>
#include <qt/util/qtwidgets.h>
#include <spreadsheet/spreadsheetdata.h>
#include <spreadsheet/spreadsheet.h>

namespace qtspreadsheet {

const char *const editorDefault="default";
const char *const editorLineedit="lineedit";
const char *const editorCombobox="combobox";

class QtSpreadsheet;

class oswinexp SpreadsheetView : public QWidget {

  Q_OBJECT

  friend class QtSpreadsheet;

  public:
    QtSpreadsheet *m_spreadsheet;
    mk_vertex m_pointerPos;
    SpreadsheetView(QtSpreadsheet *, const char *);
    virtual ~SpreadsheetView();
    bool operator==(const SpreadsheetView &cmp) const {
      return (m_spreadsheet==cmp.m_spreadsheet);
    }
    bool operator<(const SpreadsheetView &cmp) const {
      return (m_spreadsheet<cmp.m_spreadsheet);
    }
    bool event (QEvent *);
    bool eventFilter(QObject*,QEvent*);

  protected:
    SpreadsheetView(const SpreadsheetView &) : QWidget() {
    }
    SpreadsheetView &operator=(const SpreadsheetView &) {
      return *this;
    }

};

class oswinexp SpreadsheetEditor {

  public:
    spreadsheet::Coords m_pos;
    osix::xxRect m_r;
    mk_string m_editorType;
    QWidget *m_editwidget;
    SpreadsheetEditor();
    virtual ~SpreadsheetEditor();
    bool operator==(const SpreadsheetEditor &cmp) const;
    bool operator<(const SpreadsheetEditor &cmp) const;
    int active() const;
    virtual int remove();

  protected:
    SpreadsheetEditor(const SpreadsheetEditor &) {
    }
    SpreadsheetEditor &operator=(const SpreadsheetEditor &) {
      return *this;
    }

};

class oswinexp QtSpreadsheet : public QFrame,public spreadsheet::Spreadsheet {

  Q_OBJECT

  friend class SpreadsheetView;

  public:
    QtSpreadsheet(QFrame *,const char *name=0);
    virtual ~QtSpreadsheet();
    virtual QSize sizeHint() const;
    virtual int isScrolling() const;
    virtual int isEditing(spreadsheet::Coords *pos=0) const;
    virtual int updateGeometry();
    virtual int osUpdate(osix::xxRect=osix::xxRect());
    virtual void *findDisplay();

  protected:
    QGridLayout *m_layout;
    QScrollBar *m_hsb;
    QScrollBar *m_vsb;
    SpreadsheetView *m_view;
    SpreadsheetEditor m_editor;
    qtutil::QtTimer m_scrollTimer;

    virtual int adjustSliders(int,int,int,int,int,int);
    virtual int adjustEditor();
    virtual int stopScrolling();
    virtual int startScrolling();
    virtual int startEditCell(spreadsheet::Coords,int sym=0);
    virtual int stopEditCell(int);
    
    QtSpreadsheet(const QtSpreadsheet &) : QFrame(),Spreadsheet() {
    }
    QtSpreadsheet &operator=(const QtSpreadsheet &) {
      return *this;
    }

  protected slots:
    void hsbValueChanged(int);
    void vsbValueChanged(int);
    void scrollTimeout(qtutil::QtTimer*);

  signals:
    void dataChanged(int,int);


};

} // namespace qtspreadsheet


#endif
