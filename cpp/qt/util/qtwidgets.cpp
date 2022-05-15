
#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QRect>
#include <QtWidgets/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QResizeEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QStyle>

#include <qt/util/qtutil.h>
#include <qt/util/qtwidgets.h>
#include <osix/xxstyle.h>
#include <mkbase/mkconv.h>

namespace qtutil {

CustomPushButton::CustomPushButton(const QString &text,QWidget *parent,const char *info) :
  QPushButton(text,parent) {
  
  mk_stringset(m_info,info);
  connect(this,SIGNAL(clicked()),this,SLOT(slotClicked()));
  
}

// *****
QSize CustomPushButton::sizeHint() const {

  QSize sz=QPushButton::sizeHint();
  QFontMetrics metrics(font());
  sz.setWidth(2*metrics.horizontalAdvance(text()));
//printf ("sz w=%d h=%d\n",sz.width(),sz.height());
  return sz;

}

// *****
QSize CustomPushButton::minimumSizeHint() const {

  QSize sz=QPushButton::sizeHint();
  QFontMetrics metrics(font());
  sz.setWidth(2*metrics.horizontalAdvance(text()));
//printf ("minsz w=%d h=%d\n",sz.width(),sz.height());
  return sz;

}

CustomRadioButton::CustomRadioButton(const QString &text,QWidget *parent,const char *info) :
  QRadioButton(text,parent) {

  mk_stringset(m_info,info);
  
}

// *****
QSize CustomRadioButton::sizeHint() const {

  return QRadioButton::sizeHint();

}

// *****
QSize CustomRadioButton::minimumSizeHint() const {

  return QRadioButton::minimumSizeHint();

}

CustomCheckBox::CustomCheckBox(const QString &text,QWidget *parent,const char *info) :
  QCheckBox(text,parent) {
  
  mk_stringset(m_info,info);
  connect(this,SIGNAL(stateChanged(int)),this,SLOT(slotStateChanged(int)));
  
}

// *****
QSize CustomCheckBox::sizeHint() const {

  return QCheckBox::sizeHint();

}

// *****
QSize CustomCheckBox::minimumSizeHint() const {

  return QCheckBox::minimumSizeHint();

}

void ColorButton::mousePressEvent(QMouseEvent *me) {

  xxtranslateQevent((void*)me,&m_mousedown);
  if (rect().contains(me->pos())) 
    m_mousedown.m_consumer|=osix::xx_consumed;
  if ((m_mousedown.m_consumer&osix::xx_consumed)>0)
    me->accept();

}

void ColorButton::mouseMoveEvent(QMouseEvent *me) {

  xxtranslateQevent((void*)me,&m_mousemove);
    
}

void ColorButton::mouseReleaseEvent(QMouseEvent *me) {

  osix::xxEvent mdown=m_mousedown;
  m_mousedown.clear();
  m_mousemove.clear();
  if (mdown.m_consumer>1 && rect().contains(me->pos()))
    emit clicked(); 
  if ((mdown.m_consumer&osix::xx_consumed)>0)
    me->accept();

}

QSize ColorButton::sizeHint() const {

  return (m_other ? m_other->sizeHint() : QFrame::sizeHint());

}

QSize ColorButton::minimumSizeHint() const {

  return (m_other ? m_other->minimumSizeHint() : QFrame::minimumSizeHint());

}

void ColorButton::setColor(unsigned int color,bool refresh) {

  QColor cc((QRgb)color);
  cc.setAlpha(osix::xxalpha(color));
  QPalette pal=palette();
  pal.setBrush(QPalette::Window,cc);
  setPalette(pal);
  if (refresh) 
    update();
  
}

ColorChooser::ColorChooser(QWidget *parent,unsigned int color) : 
  QDialog(parent,Qt::WindowStaysOnTopHint),m_colortriangle(0) {

  mk_string numstr;
  setAttribute(Qt::WA_DeleteOnClose);
  
  QFontMetrics metrics(font());
  int ww=metrics.horizontalAdvance("0");

  m_layout=new QGridLayout(this);
  m_layout->setSpacing(0); 
  m_layout->setContentsMargins(QMargins(0,0,0,0));
  
  //m_colortriangle=new QtColorTriangle(this);
  //m_colortriangle->setColor(QColor((QRgb)color));
  //connect(m_colortriangle,SIGNAL(colorChanged(const QColor&)),this,SLOT(slotColorChanged(const QColor&)));
  //m_layout->addWidget(m_colortriangle,0,0,8,8);
  QFrame *spacerV=new QFrame(this);
  spacerV->setFixedWidth(5);
  m_layout->addWidget(spacerV,0,8,8,1);
  QLabel *lr=new QLabel("r:",this);
  lr->setFixedWidth(2*ww);
  m_layout->addWidget(lr,0,9,Qt::AlignRight);
  mk_ui2a(osix::xxred(color),numstr);
  m_editR=new QLineEdit(&numstr[0],this);
  m_editR->setMaxLength(3);
  m_editR->setFixedWidth(4*ww);
  m_layout->addWidget(m_editR,0,10,Qt::AlignRight);
  connect (m_editR,SIGNAL(editingFinished()),this,SLOT(slotR()));
  QLabel *lg=new QLabel("g:",this);
  lg->setFixedWidth(2*ww);
  m_layout->addWidget(lg,1,9,Qt::AlignRight);
  mk_ui2a(osix::xxgreen(color),numstr);
  m_editG=new QLineEdit(&numstr[0],this);
  m_editG->setMaxLength(3);
  m_editG->setFixedWidth(4*ww);
  m_layout->addWidget(m_editG,1,10,Qt::AlignRight);
  connect (m_editG,SIGNAL(editingFinished()),this,SLOT(slotG()));
  QLabel *lb=new QLabel("b:",this);
  lb->setFixedWidth(2*ww);
  m_layout->addWidget(lb,2,9,Qt::AlignRight);
  mk_ui2a(osix::xxblue(color),numstr);
  m_editB=new QLineEdit(&numstr[0],this);
  m_editB->setMaxLength(3);
  m_editB->setFixedWidth(4*ww);
  m_layout->addWidget(m_editB,2,10,Qt::AlignRight);
  connect (m_editB,SIGNAL(editingFinished()),this,SLOT(slotB()));
  QLabel *la=new QLabel("a:",this);
  la->setFixedWidth(2*ww);
  m_layout->addWidget(la,3,9,Qt::AlignRight);
  mk_ui2a(osix::xxalpha(color),numstr);
  m_editA=new QLineEdit(&numstr[0],this);
  m_editA->setMaxLength(3);
  m_editA->setFixedWidth(4*ww);
  m_layout->addWidget(m_editA,3,10,Qt::AlignRight);
  connect (m_editA,SIGNAL(editingFinished()),this,SLOT(slotA()));
  m_colorview=new ColorButton(this,0);
  //m_colorview->setMinimumHeight(6*w);
  m_colorview->setAutoFillBackground(true);
  m_colorview->setBackgroundRole(QPalette::Window);
  m_colorview->setFrameShape(QFrame::Box);
  m_colorview->setColor(color,false);
  m_layout->addWidget(m_colorview,4,9,4,2);
  m_buttonSend=new QPushButton("ok",this);
  m_buttonSend->setAutoDefault(false);
  m_buttonSend->setDefault(false);
  connect(m_buttonSend,SIGNAL(clicked()),this,SLOT(sendValues()));
  m_layout->addWidget(m_buttonSend,8,0,1,11);
    
}

ColorChooser::~ColorChooser() {

}

void ColorChooser::accept() {

  emit dismiss();
  QDialog::accept();

}


void ColorChooser::reject() {

  emit dismiss();
  QDialog::reject();

}

void ColorChooser::slotR() {

  QColor color;//=m_colortriangle->color();
  QString txt=m_editR->text();
  const char *str=qasciistr(&txt);
  color.setRed((int)mk_a2ui(str));
  //m_colortriangle->setColor(color);
  
}

void ColorChooser::slotG() {

  QColor color;//=m_colortriangle->color();
  QString txt=m_editG->text();
  const char *str=qasciistr(&txt);
  color.setGreen((int)mk_a2ui(str));
  //m_colortriangle->setColor(color);

}

void ColorChooser::slotB() {

  QColor color;//=m_colortriangle->color();
  QString txt=m_editB->text();
  const char *str=qasciistr(&txt);
  color.setBlue((int)mk_a2ui(str));
  //m_colortriangle->setColor(color);

}

void ColorChooser::slotA() {

  QString txt=m_editA->text();
  const char *str=qasciistr(&txt);
  int base=-1;
  mk_ulreal aa=mk_a2ui(str,&base);
  if (base<0 || aa>255) {
    aa=255;
    m_editA->setText("255");
  }
  unsigned int cc=0;//(unsigned int)m_colortriangle->color().rgba();
  cc=osix::xxcolor(osix::xxred(cc),osix::xxgreen(cc),osix::xxblue(cc),aa);
  m_colorview->setColor(cc);

}

void ColorChooser::slotColorChanged(const QColor &) {

  unsigned int cc=0;//(unsigned int)m_colortriangle->color().rgba();
  QString txt=m_editA->text();
  const char *str=qasciistr(&txt);
  int base=-1;
  mk_lreal aa=mk_a2ui(str,&base);
  if (base<0 || aa>255)
    aa=255;
  cc=osix::xxcolor(osix::xxred(cc),osix::xxgreen(cc),osix::xxblue(cc),aa);
  m_colorview->setColor(cc);
  mk_string numstr;
  mk_ui2a(osix::xxred(cc),numstr);
  m_editR->setText(&numstr[0]);
  mk_ui2a(osix::xxgreen(cc),numstr);
  m_editG->setText(&numstr[0]);
  mk_ui2a(osix::xxblue(cc),numstr);
  m_editB->setText(&numstr[0]);
  
}

void ColorChooser::sendValues() { 

  unsigned int cc=0;//(unsigned int)m_colortriangle->color().rgba();
  QString txt=m_editA->text();
  const char *str=qasciistr(&txt);
  int base=-1;
  mk_lreal aa=mk_a2ui(str,&base);
  if (base<0 || aa>255)
    aa=255;
  cc=osix::xxcolor(osix::xxred(cc),osix::xxgreen(cc),osix::xxblue(cc),aa);
  emit setColor(cc);
  accept(); 

}

} // namespace plot
