
#if defined (__linux__)
/* #include <QtX11Extras/QX11Info> */
#include <QtGui/6.4.0/QtGui/qpa/qplatformnativeinterface.h>
#endif
#include <QtWidgets/QStyle>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtGui/QClipboard>
#include <stdio.h>
#include <qt/util/qtutil.h>
#include <qt/spreadsheet/spreadsheet.h>
//#include <sys/param.h>

namespace qtspreadsheet {

SpreadsheetEditor::SpreadsheetEditor() : m_editwidget(0) {

  mk_stringset(m_editorType,0);    

}
    
bool SpreadsheetEditor::operator==(const SpreadsheetEditor &cmp) const {

  return (m_pos==cmp.m_pos);
  
}

bool SpreadsheetEditor::operator<(const SpreadsheetEditor &cmp) const {

  return (m_pos<cmp.m_pos);
  
}

int SpreadsheetEditor::active() const {

  return (m_editwidget ? 1 : 0);
  
}
    
SpreadsheetEditor::~SpreadsheetEditor() {

  if (m_editwidget)
    delete m_editwidget;

}

int SpreadsheetEditor::remove() {

  if (m_editwidget)
    delete m_editwidget;
  m_editwidget=0;
  m_pos.set(-1,-1);
  m_r.set();
  mk_stringset(m_editorType,0);
  return 0;

}

// **********
QtSpreadsheet::QtSpreadsheet(QFrame *parent,const char *name) : 
  QFrame(parent),spreadsheet::Spreadsheet(name) {

  setObjectName(name);
  connect(&m_scrollTimer,SIGNAL(expired(qtutil::QtTimer*)),this,SLOT(scrollTimeout(qtutil::QtTimer*)));
  qtutil::fromQBrush(&(palette().brush(QPalette::Base)),&m_bgStyle);
  m_layout=new QGridLayout(this);
  m_layout->setSpacing(0);
  m_layout->setContentsMargins(0,0,0,0);
  m_pixscr.m_t=osix::xx_pixmap;
  m_pixscr.m_w=(void*)(new QPixmap(10,10));
  m_view=new SpreadsheetView(this,"spreadsheetview");
  m_winscr.m_t=osix::xx_window;
  m_winscr.m_w=(void*)m_view;
  m_layout->addWidget(m_view,0,0);
  m_hsb=new QScrollBar(Qt::Horizontal,this);
  connect (m_hsb,SIGNAL(valueChanged(int)),this,SLOT(hsbValueChanged(int)));
  m_layout->addWidget(m_hsb,1,0);
  m_vsb=new QScrollBar(Qt::Vertical,this);
  connect (m_vsb,SIGNAL(valueChanged(int)),this,SLOT(vsbValueChanged(int)));
  m_layout->addWidget(m_vsb,0,1);
  m_vsb->setMaximum(0);
  m_hsb->setMaximum(0);
  setFocusProxy(m_view);

}

// **********
QtSpreadsheet::~QtSpreadsheet() {

  if (m_pixscr.m_w)
    delete (QPixmap*)m_pixscr.m_w;

}

void *QtSpreadsheet::findDisplay() {

#if defined (__linux__)
  /* return (void*)QX11Info::display(); */
  return QGuiApplication::platformNativeInterface()->nativeResourceForIntegration(QByteArray("display"));
#endif
    
  return 0;

}

int QtSpreadsheet::osUpdate(osix::xxRect rr) {

  if (m_view) {
    QRectF qrf;
    qtutil::toQRectF(&rr,&qrf);
    if (rr.empty()!=0)
      m_view->update();
    else
      m_view->update(qrf.toRect());
  }
  return 0;

}

// **********
int QtSpreadsheet::updateGeometry() {

  if (m_view)
    calcGeometry(osix::xxRectSize((double)m_view->size().width(),(double)m_view->size().height()));
  return 0;

}

// **********
QSize QtSpreadsheet::sizeHint() const {

  QSize sz=QFrame::sizeHint();
  //printf ("hint w=%d h=%d\n",sz.width(),sz.height());
  return QSize(1000,600);

}

// **********
void QtSpreadsheet::hsbValueChanged(int pos) {

  setPos(spreadsheet::Coords(-1,pos),1,0,true);

}

// **********
void QtSpreadsheet::vsbValueChanged(int pos) {

  setPos(spreadsheet::Coords(pos,-1),1,0,true);

}

int QtSpreadsheet::adjustSliders(int row,int col,int maxscrrows,int maxscrcols,int rowstep,int colstep) {

  m_hsb->blockSignals(true);
  m_vsb->blockSignals(true);
  if (row>=0)
    m_vsb->setValue(row);
  if (col>=0)
    m_hsb->setValue(col);
  if (maxscrrows>=0)
    m_vsb->setMaximum(maxscrrows);
  if (maxscrcols>=0)
    m_hsb->setMaximum(maxscrcols);
  if (rowstep>=0)
    m_vsb->setSingleStep(1);
  if (colstep>=0)
    m_hsb->setSingleStep(1);
  m_hsb->blockSignals(false);
  m_vsb->blockSignals(false);

  return 0;

}

// **********
int QtSpreadsheet::adjustEditor() {

  if (!m_editor.m_editwidget)
    return 0;
	spreadsheet::Coords edvpos;
  osix::xxRect edr=cellRect(m_editor.m_pos,&edvpos);
//printf ("edr=%f,%f,%f,%f vpos=%d,%d\n",edr.left(),edr.top(),edr.right(),edr.bottom(),edvpos.m_row,edvpos.m_col);
  if (edvpos.m_row<0 || edvpos.m_col<0)
    m_editor.m_editwidget->hide();
  else {
    m_editor.m_editwidget->move((int)edr.left(),(int)edr.top());
    if (m_editor.m_editwidget->isHidden())
      m_editor.m_editwidget->show();
  }

  return 0;

}

// **********
void QtSpreadsheet::scrollTimeout(qtutil::QtTimer *timer) {

  if (m_view)
    scrollAction(timer->timerId(),m_view->m_pointerPos);
  else
    stopScrolling();
  
}

// **********
int QtSpreadsheet::isScrolling() const {

  return (m_scrollTimer.isActive() ? spreadsheet::autoscroll : 0);

}

// **********
int QtSpreadsheet::startScrolling() {

  m_scrollTimer.start(spreadsheet::scrollInterval);
  return isScrolling();

}

// **********
int QtSpreadsheet::stopScrolling() {

  m_scrollTimer.stop();
  return isScrolling();

}

// **********
int QtSpreadsheet::isEditing(spreadsheet::Coords *) const {

  return (m_editor.m_editwidget ? 1 : 0);

}

// **********
int QtSpreadsheet::startEditCell(spreadsheet::Coords pos,int sym) {

  int rows=0,cols=0;
  if (!m_view || m_editor.m_editwidget || !validView(&rows,&cols) ||
      pos.m_row<0 || pos.m_row>=m_data->nrows() || pos.m_col<0 || pos.m_col>=m_data->ncols())
    return -1;
  spreadsheet::Coords vpos;
  osix::xxRect rr=cellRect(pos,&vpos);
//printf ("rl=%f rt=%f rw=%f rh=%f\n",rr.left(),rr.top(),rr.size().width(),rr.size().height());
  if (rr.empty())
    return -1;
  mk_string edtypedef;
  mk_stringset(edtypedef,editorDefault);
  mk_string edtypele;
  mk_stringset(edtypele,editorLineedit);
  mk_string editorCombobox;
  mk_stringset(editorCombobox,editorCombobox);
  mk::Ucsstr ucssym;
  ucssym.reserve(2);
  ucssym.append((unsigned short)sym);
  QFont fnt;
  QString edtxt;
  qtutil::toQString(&ucssym,&edtxt);
  spreadsheet::SpreadsheetDataItem *itm=m_data->data(pos.m_row,pos.m_col);
  if (itm) {
    if (mk_stringlength(itm->m_editorType)==0)
      return -1;
    if (itm->m_text.m_txt.length()>0)
      qtutil::toQString(&itm->m_text.m_txt,&edtxt);
    if (mk_stringcmp(itm->m_editorType,edtypedef)==0) {
      mk_stringset(m_editor.m_editorType,edtypedef);
      m_editor.m_editwidget=new QLineEdit(edtxt,m_view);
    }
    else if (mk_stringcmp(itm->m_editorType,edtypele)==0) {
      mk_stringset(m_editor.m_editorType,edtypele);
      m_editor.m_editwidget=new QLineEdit(edtxt,m_view);
    }
    else if (mk_stringcmp(itm->m_editorType,editorCombobox)==0) {
        
    }
    else
      return -1;
    qtutil::toQFont(&itm->m_text.m_fnt,&fnt);
  }
  else {
    qtutil::toQFont(&spreadsheet::defFnt,&fnt);
    mk_stringset(m_editor.m_editorType,edtypedef);
    m_editor.m_editwidget=new QLineEdit(edtxt,m_view);
  }
  m_editor.m_pos=pos;
  m_editor.m_editwidget->setFont(fnt);
  m_editor.m_editwidget->move((int)rr.left(),(int)rr.top());
  m_editor.m_editwidget->resize((int)rr.size().width(),(int)rr.size().height());
  m_editor.m_editwidget->installEventFilter(m_view);
  m_editor.m_editwidget->show();  
  m_editor.m_editwidget->setFocus(Qt::MouseFocusReason);

  return 0;

}

// **********
int QtSpreadsheet::stopEditCell(int res) {

  if (!m_editor.m_editwidget)
    return -1;
  spreadsheet::Coords pos=m_editor.m_pos;
  int rows=0,cols=0;
  if (!m_view || !validView(&rows,&cols) ||
      pos.m_row<0 || pos.m_row>=m_data->nrows() || 
      pos.m_col<0 || pos.m_col>=m_data->ncols())
    res=-1;
  mk_string edtypedef;
  mk_stringset(edtypedef,editorDefault);
  mk_string edtypele;
  mk_stringset(edtypele,editorLineedit);
  mk_string editorCombobox;
  mk_stringset(editorCombobox,editorCombobox);
  QString edtxt;
  if (mk_stringcmp(m_editor.m_editorType,edtypedef)==0)
    edtxt=((QLineEdit*)m_editor.m_editwidget)->text();
  else if (mk_stringcmp(m_editor.m_editorType,edtypele)==0)
    edtxt=((QLineEdit*)m_editor.m_editwidget)->text();
  else if (mk_stringcmp(m_editor.m_editorType,editorCombobox)==0) {
    res=-1;
  }
  else
    res=-1;
  m_editor.remove();
  if (res>=0) {
    mk::Ucsstr txt;
    qtutil::fromQString(&edtxt,&txt);
	  spreadsheet::SpreadsheetDataItem *itm=m_data->data(pos.m_row,pos.m_col);
    if (itm) {
      itm->m_text.m_txt=txt;
    }
    else {
      m_data->setData(pos.m_row,pos.m_col,new spreadsheet::SpreadsheetDataItem(txt));
    }
  }
  setFocus(Qt::MouseFocusReason);

  return 0;

}

// **********
// **********
// **********

// **********
SpreadsheetView::SpreadsheetView(QtSpreadsheet *spreadsheet,const char *name) : 
  QWidget((QWidget*)spreadsheet),m_spreadsheet(spreadsheet) {

  setObjectName(name);
  //setStyle(new QWindowsStyle());
  setAttribute(Qt::WA_NoSystemBackground,true);
  m_pointerPos[0]=m_pointerPos[1]=-1;
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  installEventFilter(this);//qApp->installEventFilter(this);

}

// **********
SpreadsheetView::~SpreadsheetView() {

}

// **********
bool SpreadsheetView::eventFilter(QObject *obj,QEvent *ev) {

  if (ev->type()==QEvent::KeyPress || ev->type()==QEvent::KeyRelease) {
    QKeyEvent *kev=(QKeyEvent*)ev;
    if (kev->key()==Qt::Key_Tab || kev->key()==Qt::Key_Backtab) {
      QApplication::postEvent(
        this,new QKeyEvent(ev->type(),(int)(kev->key()==Qt::Key_Tab ? Qt::Key_Right : Qt::Key_Left),
                           kev->modifiers(),QString(),kev->isAutoRepeat(),(unsigned short)kev->count()));
      return true;
    }
    if (m_spreadsheet && m_spreadsheet->m_editor.m_editwidget) {
      if (kev->key()==Qt::Key_Escape || kev->key()==Qt::Key_Return ||
          kev->key()==Qt::Key_Enter || kev->key()==Qt::Key_Up || kev->key()==Qt::Key_Down) {
        if (obj==m_spreadsheet->m_editor.m_editwidget) {
          QApplication::postEvent(this,new QKeyEvent(ev->type(),kev->key(),kev->modifiers(),
                                  kev->text(),kev->isAutoRepeat(),(unsigned short)kev->count()));
          return true;
        }
      }
      return (obj && obj!=this ? obj->eventFilter(obj,ev) : QWidget::eventFilter(obj,ev));
    }
  }
  else if (ev->type()==QEvent::MouseButtonPress ||
           ev->type()==QEvent::MouseButtonRelease ||
           ev->type()==QEvent::MouseMove) {  // return QWidget::eventFilter(obj,ev);
    if (m_spreadsheet && m_spreadsheet->m_editor.m_editwidget) {
      if (obj==m_spreadsheet->m_editor.m_editwidget) {
        QMouseEvent *mev=(QMouseEvent*)ev;
        if (m_spreadsheet->m_editor.m_editwidget->rect().contains(mev->pos()))
          return obj->eventFilter(obj,ev);
      }
    }
  }

  return QWidget::eventFilter(obj,ev);

}

// **********
bool SpreadsheetView::event(QEvent *qev) {

  QWidget::event(qev);
  if (qev->type()==QEvent::MouseButtonPress || qev->type()==QEvent::MouseMove) {
    m_pointerPos[0]=(double)((QMouseEvent*)qev)->pos().x();
    m_pointerPos[1]=(double)((QMouseEvent*)qev)->pos().y();
  }
  int consumed=0;
  if (m_spreadsheet)
    consumed=m_spreadsheet->osEvent(m_spreadsheet->findDisplay(),(void*)qev);
  if ((consumed&osix::xx_consumed)>0)
    return true;
  return false;

}

} // namespace qtspreadsheet
