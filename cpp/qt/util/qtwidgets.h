
#ifndef _QTWIDGETS_
#define _QTWIDGETS_

#include <QtCore/QObject>
#include <QtCore/QMetaObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QSize>
#include <QtGui/QColor>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLayout>
#include <stdio.h>
#include <auxx/auxx.h>
#include <osix/xxevent.h>
#include <osix/xxshape.h>
//#include <qt/util/qtcolortriangle.h>

namespace qtutil {

class oswinexp CustomPushButton : public QPushButton {

  Q_OBJECT

  public:
    aux::Asciistr m_info;
    CustomPushButton(const QString &,QWidget *parent=0,aux::Asciistr info=aux::Asciistr());
    virtual ~CustomPushButton() {
    }
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

  protected:
    CustomPushButton(const CustomPushButton &) : QPushButton() {
    }
    CustomPushButton &operator=(const CustomPushButton &) {
      return *this;
    }

  signals:
    void qclicked(qtutil::CustomPushButton*);

  public slots:
    void slotClicked() {
      emit qclicked(this);
    }
        
};

class oswinexp CustomRadioButton : public QRadioButton {

  Q_OBJECT

  public:
    aux::Asciistr m_info;
    CustomRadioButton(const QString &,QWidget *parent=0,aux::Asciistr info=aux::Asciistr());
    virtual ~CustomRadioButton() {
    }
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

  protected:
    CustomRadioButton(const CustomRadioButton &) : QRadioButton() {
    }
    CustomRadioButton &operator=(const CustomRadioButton &) {
      return *this;
    }
        
};

class oswinexp CustomCheckBox : public QCheckBox {

  Q_OBJECT

  public:
    aux::Asciistr m_info;
    CustomCheckBox(const QString &,QWidget *parent=0,aux::Asciistr info=aux::Asciistr());
    virtual ~CustomCheckBox() {
    }
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

  protected:
    CustomCheckBox(const CustomCheckBox &) : QCheckBox() {
    }
    CustomCheckBox &operator=(const CustomCheckBox &) {
      return *this;
    }

  signals:
    void qstatechanged(qtutil::CustomCheckBox*,int);

  public slots:
    void slotStateChanged(int check) {
      emit qstatechanged(this,check);
    }
        
};

class oswinexp ColorButton : public QFrame {

  Q_OBJECT

  public:
    QWidget *m_other;
    osix::xxEvent m_mousedown,m_mousemove;
    ColorButton(QWidget *parent,QWidget *other) : QFrame(parent),m_other(other) {
    }
    virtual ~ColorButton() {
    }
    void mousePressEvent (QMouseEvent*);
    void mouseMoveEvent (QMouseEvent*);
    void mouseReleaseEvent (QMouseEvent*);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void setColor(unsigned int,bool refresh=true);
    
  signals:
    void clicked();

};

class oswinexp ColorChooser : public QDialog {

  Q_OBJECT

  protected:
    QGridLayout *m_layout;
    QPushButton *m_buttonSend;
    ColorButton *m_colorview;
    QLineEdit *m_editR;
    QLineEdit *m_editG;
    QLineEdit *m_editB;
    QLineEdit *m_editA;
    //QtColorTriangle *m_colortriangle;
    void *m_colortriangle;
            
  public:
    ColorChooser(QWidget *,unsigned int);
    virtual ~ColorChooser();
    
  signals:
    void setColor(unsigned int);
    void dismiss();
    
  public slots:
    void accept();
    void reject();
    void sendValues();
    void slotColorChanged(const QColor &);
    void slotR();
    void slotG();
    void slotB();
    void slotA();

};

class oswinexp QtTimer : public QTimer {

  Q_OBJECT

  public:
    aux::TVList<aux::Asciistr> m_infoL;
    QtTimer() : QTimer() {
      connect (this,SIGNAL(timeout()),this,SLOT(relay()));
    }
    virtual ~QtTimer() {
    }
    bool operator==(const QtTimer &cmp) const {
      return (timerId()==cmp.timerId());
    }
    bool operator<(const QtTimer &cmp) const {
      return (timerId()<cmp.timerId());
    }
  signals:
    void expired(qtutil::QtTimer*);

  protected slots:
    void relay() {
      emit expired(this);
    }

  protected:
    QtTimer(const QtTimer &) : QTimer() {
    }
    QtTimer &operator=(const QtTimer &) {
      return *this;
    }

};

}

#endif
