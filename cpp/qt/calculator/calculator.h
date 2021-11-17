#ifndef _calculator_qt_
#define _calculator_qt_

#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTimer>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QWidget>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGroupBox>
#include <stdio.h>
#include <qt/util/qtwidgets.h>
#include <calculator/calculator.h>

namespace qtcalculator {

const int numradiobuttons=6;
const int numpushbuttons=43;

class QtCalculator;

class oswinexp CalculatorOutput : public QFrame {

  Q_OBJECT

  public:
    QtCalculator *m_calculator;
    CalculatorOutput(QtCalculator *, QWidget*);
    virtual ~CalculatorOutput();
    bool operator==(const CalculatorOutput &cmp) const {
      return (m_calculator==cmp.m_calculator);
    }
    bool operator<(const CalculatorOutput &cmp) const {
      return (m_calculator<cmp.m_calculator);
    }
    bool event (QEvent *);
    bool eventFilter(QObject*,QEvent*);

  protected:
    CalculatorOutput(const CalculatorOutput &) : QFrame() {
    }
    CalculatorOutput &operator=(const CalculatorOutput &) {
      return *this;
    }

};

class oswinexp QtCalculatorTimer : public qtutil::QtTimer {

  public:
    mk::TVList<calculator::Asciistr> m_qtinfoL;
    QtCalculatorTimer() : qtutil::QtTimer() {
    }
    virtual ~QtCalculatorTimer() {
    }

};

class oswinexp QtCalculator : public QFrame,public calculator::Calculator  {

  Q_OBJECT

  friend class CalculatorOutput;

  protected:
    QtCalculatorTimer m_timer;
    QGridLayout *m_layout;
    CalculatorOutput *wwoutput;
    QButtonGroup *fmtgroup;
    QButtonGroup *triggroup;
    qtutil::CustomRadioButton *m_radiobuttons[numradiobuttons];
    qtutil::CustomPushButton *m_pushbuttons[numpushbuttons];
    CalculatorOutput *wwinfo;
       
  public:
    QtCalculator(QWidget *parent=0);
    virtual ~QtCalculator();
    virtual void *findDisplay();
    bool eventFilter(QObject *,QEvent *);
    void doFont(float,int);

  public slots:
    void slotPushButton(qtutil::CustomPushButton*);
    void slotTimeout(qtutil::QtTimer *);
    void slotTrig(QAbstractButton*);
    void slotFmt(QAbstractButton*);
    void showHelp();

  protected:
    int osUpdate(void*);
    void resizeEvent(QResizeEvent *);
    int fnActive(const char *,int);
    unsigned int delayedChgFmt(unsigned int);
    qtutil::CustomPushButton *findCustomPushButton(calculator::Asciistr);
    qtutil::CustomRadioButton *findCustomRadioButton(calculator::Asciistr);

};

} // namespace

#endif

