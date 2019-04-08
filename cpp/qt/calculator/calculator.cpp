
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStyle>
#include <QtGui/QKeyEvent>
#include <QtX11Extras/QX11Info>
#include <QtGui/QPainter>

#include <math.h>

#include <mkbase/mkconv.h>
#include <osix/xxstyle.h>
#include <osix/xxevent.h>
#include <qt/util/qtutil.h>
#include <qt/calculator/calculator.h>

namespace qtcalculator {

static const char* const init_radiobuttons[numradiobuttons][2] = {
{"dec",calculator::fn_dec},{"bin",calculator::fn_bin},{"oct",calculator::fn_oct},
{"hex",calculator::fn_hex},{"deg",calculator::fn_deg},{"rad",calculator::fn_rad}
};

static const char* const init_pushbuttons[numpushbuttons][2] = { 
{"inv",calculator::fn_inv},{"1/x",calculator::fn_reci},{"Mrcl",calculator::fn_memrcl},
{"Msto",calculator::fn_memput},{"C",calculator::fn_clearall},{"CE",calculator::fn_clear},
{"ln",calculator::fn_ln},{"log",calculator::fn_log},{"sqrt",calculator::fn_sqrt},
{"x^2",calculator::fn_sqr},{"x^y",calculator::fn_pow},{"exp",calculator::fn_exp},
{"sin",calculator::fn_sin},{"cos",calculator::fn_cos},{"tan",calculator::fn_tan},
{"x!",calculator::fn_fac},{"e",calculator::fn_euler},{"pi",calculator::fn_pi},
{"7",calculator::fn_7},{"8",calculator::fn_8},{"9",calculator::fn_9},{"/",calculator::fn_div},
{"A",calculator::fn_A},{"B",calculator::fn_B},{"4",calculator::fn_4},{"5",calculator::fn_5},
{"6",calculator::fn_6},{"*",calculator::fn_mult},{"C",calculator::fn_C},{"D",calculator::fn_D},
{"1",calculator::fn_1},{"2",calculator::fn_2},{"3",calculator::fn_3},{"+",calculator::fn_plus},
{"E",calculator::fn_E},{"F",calculator::fn_F},{"0",calculator::fn_0},{"+/-",calculator::fn_chsgn},
{".",calculator::fn_decsep},{"-",calculator::fn_minus},{"(",calculator::fn_parenl},
{")",calculator::fn_parenr},{"=",calculator::fn_equals}
};

static int cmpCustomPushButton(const void *b1,const void *b2) {

  if (((const qtutil::CustomPushButton*)b1)->m_info<((const qtutil::CustomPushButton*)b2)->m_info)
    return -1;
  if (((const qtutil::CustomPushButton*)b2)->m_info<((const qtutil::CustomPushButton*)b1)->m_info)
    return 1;
  return 0;

}

static int cmpCustomRadioButton(const void *b1,const void *b2) {

  if (((const qtutil::CustomRadioButton*)b1)->m_info<((const qtutil::CustomRadioButton*)b2)->m_info)
    return -1;
  if (((const qtutil::CustomRadioButton*)b2)->m_info<((const qtutil::CustomRadioButton*)b1)->m_info)
    return 1;
  return 0;

}

QtCalculator::QtCalculator(QWidget *parent) :
  QFrame(parent),calculator::Calculator(),m_layout(0),wwoutput(0),fmtgroup(0),triggroup(0),wwinfo(0) {

  int ii=0,rows=10,cols=6,row=0,col=0;

  m_layout=new QGridLayout(this);
  m_layout->setContentsMargins(4,2,4,2);
  m_layout->setSpacing(4);  
  wwoutput=new CalculatorOutput(this,this);
  wwoutput->setMinimumHeight(20);
  m_layout->addWidget(wwoutput,row,col,1,cols);
  m_outputwin.m_t=osix::xx_window;
  m_outputwin.m_w=(void*)wwoutput;
  row++;

  memset(&m_radiobuttons[0],0,numradiobuttons*sizeof(QRadioButton*));
  for (ii=0;ii<numradiobuttons;ii++) {
    m_radiobuttons[ii]=
      new qtutil::CustomRadioButton(init_radiobuttons[ii][0],this,init_radiobuttons[ii][1]);
    if (col<4)
      m_layout->addWidget(m_radiobuttons[ii],row,col,Qt::AlignLeft);
    else
      m_layout->addWidget(m_radiobuttons[ii],row,col,Qt::AlignRight);
    col++;
  }
  row++;col=0;
  fmtgroup=new QButtonGroup(this);
  for (ii=0;ii<numradiobuttons-2;ii++)
    fmtgroup->addButton(m_radiobuttons[ii]);
  connect(fmtgroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(slotFmt(QAbstractButton*)));
  triggroup=new QButtonGroup(this);
  for (ii=4;ii<numradiobuttons;ii++)
    triggroup->addButton(m_radiobuttons[ii]);
  connect(triggroup,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(slotTrig(QAbstractButton*)));
  mk::heapsortpc(numradiobuttons,m_radiobuttons,cmpCustomRadioButton);
  
  memset(&m_pushbuttons[0],0,numpushbuttons*sizeof(qtutil::CustomPushButton*));
  for (ii=0;ii<numpushbuttons;ii++) {
    m_pushbuttons[ii]=new qtutil::CustomPushButton(init_pushbuttons[ii][0],this,init_pushbuttons[ii][1]);
    connect(m_pushbuttons[ii],SIGNAL(qclicked(qtutil::CustomPushButton*)),
      this,SLOT(slotPushButton(qtutil::CustomPushButton*)));
    if (ii<numpushbuttons-1)
      m_layout->addWidget(m_pushbuttons[ii],row,col);
    else 
      m_layout->addWidget(m_pushbuttons[ii],row,col,1,4);
    col++;
    if (col==cols) {
      row++;
      col=0;
    } 
  }
  col=4;
  mk::heapsortpc(numpushbuttons,m_pushbuttons,cmpCustomPushButton);
  
  wwinfo=new CalculatorOutput(this,this);
  wwinfo->setMinimumHeight(20);
  m_layout->addWidget(wwinfo,row,col,1,2);
  m_showinfowin.m_t=osix::xx_window;
  m_showinfowin.m_w=(void*)wwinfo;

  connect(&m_timer,SIGNAL(expired(qtutil::QtTimer*)),this,SLOT(slotTimeout(qtutil::QtTimer*)));

  doFont(13.,0);
  updateOutput(0);
  updateInfo(0);
  qtutil::CustomPushButton *pb=findCustomPushButton(calculator::fn_parenr);
  if (pb)
    pb->setEnabled(false);
  pb=findCustomPushButton("memrcl");
  if (pb)
    pb->setEnabled(false);
  qtutil::CustomRadioButton *rb=findCustomRadioButton(calculator::fn_dec);
  if (rb)
    rb->setChecked(true);
  rb=findCustomRadioButton(calculator::fn_deg);
  if (rb)
    rb->setChecked(true);

}

QtCalculator::~QtCalculator() {

}

qtutil::CustomPushButton *QtCalculator::findCustomPushButton(calculator::Asciistr info) {

  int lb=-1,mb=0,ub=numpushbuttons;
  while ((ub-lb>1)) {
    mb=(ub+lb)/2;
    if (m_pushbuttons[mb]->m_info==info)
      return m_pushbuttons[mb];
    if (m_pushbuttons[mb]->m_info<info)
      lb=mb;
    else
      ub=mb;
  }
    
  return 0;

}

qtutil::CustomRadioButton *QtCalculator::findCustomRadioButton(calculator::Asciistr info) {

  int lb=-1,mb=0,ub=numradiobuttons;
  while ((ub-lb>1)) {
    mb=(ub+lb)/2;
    if (m_radiobuttons[mb]->m_info==info)
      return m_radiobuttons[mb];
    if (m_radiobuttons[mb]->m_info<info)
      lb=mb;
    else
      ub=mb;
  }
    
  return 0;

}

void QtCalculator::slotPushButton(qtutil::CustomPushButton *bb) {

  const char *info=(const char*)(bb->m_info);
  if (!info || strlen(info)==0)
    return;
  if (isFnNumber(info)>0)
    appendNumber(bb->m_info);
  else if (isFnOperator(info)>0)
    appendOperator(bb->m_info);
  else if (isFnExtraNumber(info)>0)
    setExtraNumber(bb->m_info); 
  else { 
    if (strcmp(info,calculator::fn_memput)==0)
      pushStore();
    else if (strcmp(info,calculator::fn_memrcl)==0)
      popStore();
    else if (strcmp(info,calculator::fn_parenl)==0)
      pushParen();
    else if (strcmp(info,calculator::fn_parenr)==0)
      popParen();
    else if (strcmp(info,calculator::fn_chsgn)==0)
      chgSgn();
    else if (strcmp(info,calculator::fn_clear)==0)
      clear();
    else if (strcmp(info,calculator::fn_clearall)==0)
      clean();
    else if (strcmp(info,calculator::fn_equals)==0)
      equals();
    else if (strcmp(info,calculator::fn_inv)==0)
      opFmt(calculator::fmtToggle);
  }

}

void QtCalculator::slotTrig(QAbstractButton *button) {

  qtutil::CustomRadioButton *rb=findCustomRadioButton(calculator::fn_deg);
  if (button == rb)
    trigFmt(calculator::fmtDeg);
  else {
    rb=findCustomRadioButton(calculator::fn_rad);
    if (button == rb)
      trigFmt(calculator::fmtRad);
  }

}

void QtCalculator::slotFmt(QAbstractButton *button) {

  qtutil::CustomRadioButton *rb=findCustomRadioButton(calculator::fn_bin);
  if (button == rb)
    doFmt(calculator::fmtBin);
  else {
    rb=findCustomRadioButton(calculator::fn_oct);
    if (button == rb)
      doFmt(calculator::fmtOct);
    else {
      rb=findCustomRadioButton(calculator::fn_dec);
      if (button == rb)
        doFmt(calculator::fmtDec);
      else {
        rb=findCustomRadioButton(calculator::fn_hex);
        if (button == rb)
          doFmt(calculator::fmtHex);
      }
    }
  }

}

unsigned int QtCalculator::delayedChgFmt(unsigned int chgfmt) {

  m_timer.m_qtinfoL.clear();
  m_timer.m_qtinfoL.append("chgfmt");
  m_timer.m_qtinfoL.append("");
  calculator::ui2a((mk_ulreal)chgfmt,m_timer.m_qtinfoL.at(1));
  m_timer.setSingleShot(true);
  m_timer.start(500);
  return chgfmt;

}

void QtCalculator::slotTimeout(qtutil::QtTimer *qttimer) {

  static calculator::Asciistr reason_chgfmt("chgfmt");
  QtCalculatorTimer *timer=dynamic_cast<QtCalculatorTimer*>(qttimer);
  if (timer->m_qtinfoL[0] == reason_chgfmt) {
    int fmt=(int)calculator::fmtDec;
    unsigned int ff=(unsigned int)mk_a2ui((const char *)timer->m_qtinfoL[1],&fmt);
    if (fmt >= 0) {
      chgFmt(ff);
      fmtgroup->blockSignals(true);
      qtutil::CustomRadioButton *rb=0;
      if (m_numfmt == calculator::fmtBin) {
        rb=findCustomRadioButton(calculator::fn_bin);
        if (rb)
          rb->setChecked(true);
      }
      else if (m_numfmt == calculator::fmtOct) {
        rb=findCustomRadioButton(calculator::fn_oct);
        if (rb)
          rb->setChecked(true);
      }
      else if (m_numfmt == calculator::fmtDec) {
        rb=findCustomRadioButton(calculator::fn_dec);
        if (rb)
          rb->setChecked(true); 
      }
      else if (m_numfmt == calculator::fmtHex) {
        rb=findCustomRadioButton(calculator::fn_hex);
        if (rb)
          rb->setChecked(true);
      }
      fmtgroup->blockSignals(false);
    }
  }

}

void QtCalculator::showHelp() {

  QMessageBox::information(this,"About","calculator",(int)QMessageBox::Ok);
  
}

int QtCalculator::osUpdate(void *win) {

  if (win==m_outputwin.m_w)
    ((QFrame*)m_outputwin.m_w)->update();
  else if (win==m_showinfowin.m_w)
    ((QFrame*)m_showinfowin.m_w)->update();
  ((QFrame*)m_outputwin.m_w)->setFocus();
  return 0;

}

void *QtCalculator::findDisplay() {

#if !defined (OSWIN)
  return (void*)QX11Info::display();
#endif
    
  return 0;

}

bool QtCalculator::eventFilter(QObject *obj,QEvent *ev) {

  int consumer=osEvent(findDisplay(),0,(void*)ev);
  return ((consumer&osix::xx_consumed)>0 ? true : false);

}

void QtCalculator::resizeEvent(QResizeEvent *qre) {
   
}

void QtCalculator::doFont(float fntsz,int how) { // how - 0:as is 1:incr 2: decr

  if (how==1)
    m_outputfnt.m_size+=fntsz;
  else if (how==2)
    m_outputfnt.m_size-=fntsz;
  else
    m_outputfnt.m_size=fntsz;
  QFont qfnt=QApplication::font();
  qtutil::toQFont(&m_outputfnt,&qfnt);  
  QFontMetrics metrics(qfnt);
  int ii=0,fntw=metrics.width("0"),fnth=metrics.height();
  m_infofnt.m_size=m_outputfnt.m_size-1.;
  qfnt.setPointSize((int)m_infofnt.m_size);
  QApplication::setFont(qfnt);
  QWidgetList wL=QApplication::allWidgets();
  for (ii=0;ii < wL.count();ii++) {
    if (wL.at(ii)!=wwoutput)
      wL.at(ii)->setFont(qfnt);
    // ?? wL.at(i)->setContentsMargins(0,0,0,0);
  }
  wwoutput->setFixedSize(QSize(65 * fntw,fnth));
  setFixedWidth(65 * fntw+8);
  resize(minimumSizeHint());

}

int QtCalculator::fnActive(const char *fn,int active) {

  if (!fn || strlen(fn)==0)
    return 0;
  qtutil::CustomPushButton *pb=findCustomPushButton(fn);
  if (!pb)
    return 0;
  active&=(calculator::fn_Active|calculator::fn_Inactive);
  if ((active&calculator::fn_Active)>0)
    pb->setEnabled(true);
  else if ((active&calculator::fn_Inactive)>0)
    pb->setEnabled(false);
  return (pb->isEnabled() ? 1 : 2);

}

// **********
CalculatorOutput::CalculatorOutput(QtCalculator *calculator,QWidget *parent) : 
  QFrame(parent),m_calculator(calculator) {

  //setObjectName(name);
  //setStyle(new QWindowsStyle());
  setAttribute(Qt::WA_NoSystemBackground,true);
  setFocusPolicy(Qt::StrongFocus);
  installEventFilter(this);

}

// **********
CalculatorOutput::~CalculatorOutput() {

}

// **********
bool CalculatorOutput::eventFilter(QObject *obj,QEvent *ev) {

  return QFrame::eventFilter(obj,ev);

}

// **********
bool CalculatorOutput::event(QEvent *qev) {

  QFrame::event(qev);
  int consumed=0;
  if (m_calculator) {
    consumed=m_calculator->osEvent(m_calculator->findDisplay(),(void*)this,(void*)qev);
  }
  if ((consumed&osix::xx_consumed)>0)
    return true;
  return false;

}

} //namesapce
