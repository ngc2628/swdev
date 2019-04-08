
#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QFrame>
#include <QtGui/QColor>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFontDialog>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTabWidget>
#include <qt/simpleplot/chart2.h> 

namespace qtsimpleplot {

class QtDiagramXY;

class oswinexp GraphInterpolationOptions : public QDialog {

  Q_OBJECT

  protected:
    QGridLayout *m_layout;
    qtutil::CustomCheckBox **m_checkoption;
    mk_ulreal m_optionL;
    GraphInterpolationOptions(const GraphInterpolationOptions &) : QDialog(0) {
    }
    GraphInterpolationOptions &operator=(const GraphInterpolationOptions &) {
      return *this;
    }

  public:
    GraphInterpolationOptions(QWidget *);
    virtual ~GraphInterpolationOptions();
    void setOptionL(mk_ulreal);
  
  signals:
    void dismiss(mk_ulreal);

  protected slots:
    void slotOptionCheck(qtutil::CustomCheckBox*,int);   

};

class oswinexp GraphProps : public QFrame {

  Q_OBJECT

  protected:
    QtDiagramXY *m_chart;
    QGridLayout *m_layout;
    QComboBox *m_selector;
    qtutil::ColorButton *m_buttoncolorline;
    qtutil::ColorButton *m_buttoncolormarkO;
    qtutil::ColorButton *m_buttoncolormarkF;
    qtutil::ColorButton *m_buttoncolorfill;
    qtutil::ColorChooser *m_colorchooser;
    QComboBox *m_combolinestyle;
    QComboBox *m_combomarkstyle;
    QComboBox *m_combomarkstyleO;
    QComboBox *m_combomarkstyleF;
    QComboBox *m_combofillref;
    QComboBox *m_combofillstyle;
    QLineEdit *m_editlinewidth;
    QLineEdit *m_editmarkwidth;
    QComboBox *m_combointerpolation;
    QCheckBox *m_checkinterpolationoption;
    QComboBox *m_comboxax;
    QComboBox *m_comboyax;
    GraphInterpolationOptions *m_interpolationoptions;
    unsigned int m_linecolor;
    unsigned int m_markcolorO;
    unsigned int m_markcolorF;
    unsigned int m_fillcolor;
    mk_ulreal m_interpolationoptionL; 

    GraphProps(const GraphProps &) : QFrame(0) {
    }
    GraphProps &operator=(const GraphProps &) {
      return *this;
    }
    
  public:
    GraphProps(QWidget *,QtDiagramXY *);
    virtual ~GraphProps();
    
  public slots:
    void sendValues();
    void slotColorchooserDismiss() { 
      m_colorchooser=0; 
    }
    
  protected slots:
    void slotSelected(int);
    void slotColorLine();
    void slotColorFill();
    void slotColorMarkF();
    void slotColorMarkO();
    void slotInterpolationOptions(int);
    void slotInterpolationOptionsDismiss(mk_ulreal);
    void slotSetColorLine(unsigned int);
    void slotSetColorFill(unsigned int);
    void slotSetColorMarkF(unsigned int);
    void slotSetColorMarkO(unsigned int);
    
};

class oswinexp AxisProps : public QFrame {

  Q_OBJECT

  protected:
    QtDiagramXY *m_chart;
    QGridLayout *m_layout;
    QComboBox *m_selector;
    QFontDialog *m_fontdlg;
    QPushButton *m_buttonFontS;
    QPushButton *m_buttonFontL;
    QComboBox *m_combopos;
    QComboBox *m_comboscaletype;
    qtutil::ColorButton *m_buttoncolor;
    qtutil::ColorChooser *m_colorchooser;
    QComboBox *m_combostyle;
    QLineEdit *m_editwidth;
    QRadioButton *m_buttonAutoscaleOn;
    QRadioButton *m_buttonAutoscaleOff;
    QLineEdit *m_editMin;
    QLineEdit *m_editMax;
    unsigned int m_color;
    osix::xxFnt m_fntS;
    osix::xxFnt m_fntL;

    AxisProps(const AxisProps &) : QFrame(0) {
    }
    AxisProps &operator=(const AxisProps &) {
      return *this;
    }

  public:
    AxisProps(QWidget *,QtDiagramXY *);
    virtual ~AxisProps();
    
  public slots:
    void sendValues();
        
  protected slots:
    void slotSelected(int);
    void slotFontS();
    void slotFontL();
    void slotSetFontS();
    void slotSetFontL();
    void slotFontDismiss() { 
      m_fontdlg=0; 
    }
    void slotAutoscale(bool);
    void slotColor();
    void slotColorchooserDismiss() { 
      m_colorchooser=0; 
    }
    void slotSetColor(unsigned int);
   
};

class oswinexp Controls : public QDialog {

  Q_OBJECT

  protected:
    QTabWidget *m_tabwidget;
    AxisProps *m_tabAxis;
    GraphProps *m_tabGraph;
    QVBoxLayout *m_layout;
    QPushButton *m_buttonSend;

    Controls(const Controls &) : QDialog(0) {
    }
    Controls &operator=(const Controls &) {
      return *this;
    }
        
  public:
    Controls (QWidget *parent,QtDiagramXY *);
    virtual ~Controls ();
    
  signals:
    void dismiss();

  public slots:
    void slotCurrentChanged(int);
    void slotSendValues();
    void accept() { 
      emit dismiss();  
      QDialog::accept(); 
    }
    void reject() { 
      emit dismiss(); 
      QDialog::reject(); 
    }

};

} // namespace

#endif
