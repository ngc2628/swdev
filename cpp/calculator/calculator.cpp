
#include <calculator/calculator.h>
#include <math.h>
#include <ctype.h>

namespace calculator {

static void dbgOp(aux::Asciistr input,const aux::TVList<Entry> &opL,int line) {

  aux::Asciistr dbg,numstr;
  dbg.reserve(128);
  int i=0;
  for (i=0;i<opL.count();i++) {
    dbg.append("  str:");
    dbg.append(opL.at(i)->m_str);
    dbg.append(" opt:");
    aux::ui2a(opL.at(i)->m_option,&numstr);
    dbg.append(numstr);
    dbg.append(" inv:");
    dbg.append(opL.at(i)->m_invstr);
    dbg.append("\n");
  }
  printf ("%d :\n  input:%s\n%s\n",line,(const char *)input,(const char *)dbg);

}

static int esplitanum(aux::Asciistr str,aux::Asciistr *strmant,aux::Asciistr *strexp) {

  if (strmant)
    *strmant=0;
  if (strexp)
    *strexp=0;
  if (str.len()==0)
    return -1;
  int idx=str.find(mk_asciie,-1,mk_asciib,mk_asciii);
  if (idx<0) {
    if (strmant)
      *strmant=str;
  }
  else {
    if (strmant)
      *strmant=str.substr(0,idx);
    if (strexp)
      *strexp=str.substr(idx+1,-1);
  }

  return idx;

}

Calculator::Calculator () :
  m_outputfg(fgColor,1,1),m_outputbg(bgColor,1,1),
  m_infofg(ebuttonfgColor,1,1),m_infobg(ebuttonbgColor,1,1),
  m_numfmt(fmtDec),m_trigfmt(fmtDeg),m_opfmt(fmtDirect) {

  m_input.reserve(128);
  m_info.reserve(128);
  m_store.resize(128);
  m_ops.resize(128);
  m_paren.resize(128);
  m_actions.resize(20);
  m_actions.inSort(Entry(fn_plus,fmtAddaction,fn_minus));
  m_actions.inSort(Entry(fn_minus,fmtAddaction,fn_plus));
  m_actions.inSort(Entry(fn_mult,fmtMultaction,fn_div));
  m_actions.inSort(Entry(fn_div,fmtMultaction,fn_mult));
  m_actions.inSort(Entry(fn_pow,fmtMultaction,fn_invpow));
  m_actions.inSort(Entry(fn_invpow,fmtMultaction,fn_pow));
  m_actions.inSort(Entry(fn_sqrt,fmtUnaction,fn_sqr));
  m_actions.inSort(Entry(fn_sqr,fmtUnaction,fn_sqrt));
  m_actions.inSort(Entry(fn_sin,fmtUnaction,fn_asin));
  m_actions.inSort(Entry(fn_asin,fmtUnaction,fn_sin));
  m_actions.inSort(Entry(fn_cos,fmtUnaction,fn_acos));
  m_actions.inSort(Entry(fn_acos,fmtUnaction,fn_cos));
  m_actions.inSort(Entry(fn_tan,fmtUnaction,fn_atan));
  m_actions.inSort(Entry(fn_atan,fmtUnaction,fn_tan));
  m_actions.inSort(Entry(fn_ln,fmtUnaction,fn_powe));
  m_actions.inSort(Entry(fn_powe,fmtUnaction,fn_ln));
  m_actions.inSort(Entry(fn_log,fmtUnaction,fn_pow10));
  m_actions.inSort(Entry(fn_pow10,fmtUnaction,fn_log));
  m_actions.inSort(Entry(fn_fac,fmtUnaction));
  m_actions.inSort(Entry(fn_reci,fmtUnaction));
  m_actions.inSort(Entry(fn_chsgn,fmtUnaction));

}

Calculator::~Calculator() {

}

int Calculator::isFnNumber(const char *info) {

  if (!info || strlen(info)==0)
    return 0;
  int ii=0;
  for (ii=0;ii<numfn_number;ii++) {
    if (strcmp(info,fn_number[ii])==0)
      return 1;
  }
  return 0;

}

int Calculator::isFnOperator(const char *info) {

  if (!info || strlen(info)==0)
    return 0;
  int ii=0;
  for (ii=0;ii<numfn_operator;ii++) {
    if (strcmp(info,fn_operator[ii])==0)
      return 1;
  }
  return 0;

}

int Calculator::isFnExtraNumber(const char *info) {

  if (!info || strlen(info)==0)
    return 0;
  if (strcmp(info,fn_exp)==0 || strcmp(info,fn_euler)==0 || strcmp(info,fn_pi)==0)
    return 1;
  return 0;

}

aux::Asciistr Calculator::setInput(aux::Asciistr input) {

  m_input=input;
  return m_input;

}

aux::Asciistr Calculator::setInfo(aux::Asciistr info) {

  m_info=info;
  return m_info;

}

int Calculator::clear() {

  m_input=0;
  updateOutput(m_input);
  return 0;

}

int Calculator::clean() {

  m_ops.clear();
  m_paren.clear();
  m_opfmt=fmtDirect;
  m_input=0;
  m_info=0;
  updateOutput(m_input);
  updateInfo(m_info);
  return 0;

}

int Calculator::errComp(aux::Asciistr msg) {

  m_ops.clear();
  m_paren.clear();
  m_opfmt=fmtDirect;
  m_input=0;
  m_info=0;
  updateOutput(msg.len()==0 ? "Err" : msg);
  updateInfo(m_info);
  return 0;

}

unsigned int Calculator::chgFmt(unsigned int chgfmt) {

  chgfmt&=fmtNum;
  if (chgfmt==0)
    return m_numfmt;
  int ii=0;
  for (ii=0;ii<m_ops.count();ii++)
    m_ops.at(ii)->m_option=chgfmt;
  unsigned int f=m_numfmt;
  m_numfmt=chgfmt;
  checkFnActive();
  return f;

}

unsigned int Calculator::trigFmt(unsigned int trigfmt) {

  trigfmt&=fmtTrig;
  if (trigfmt==0)
    return m_trigfmt;
  unsigned int f=m_trigfmt;
  m_trigfmt=trigfmt;
  return f;

}

unsigned int Calculator::opFmt(unsigned int opfmt) {

  unsigned int f=m_opfmt,toggle=(opfmt&fmtToggle);
  opfmt&=fmtDir;
  if (toggle>0) {
    if ((f&fmtDirect)>0)
      m_opfmt=fmtInvert;
    else if ((f&fmtInvert)>0)
      m_opfmt=fmtDirect;
  }
  else {
    if (opfmt==0)
      return f;
    m_opfmt=opfmt;
  }
  updateInfo(doInfo());
  return f;

}

unsigned int Calculator::doFmt(unsigned int numfmt) {

  numfmt&=fmtNum;
  if (numfmt==0 || m_numfmt==numfmt)
    return m_numfmt;
  int actfmt=m_numfmt;
  m_numfmt=numfmt;
  int ok=(int)actfmt,sgn=1;
  aux::Asciistr buf=m_input;
  mk_ulreal val=0;
  if (m_input.len()>0) {
    val=mk_a2ui((const char *)m_input,&ok,&sgn,0);
    if (ok<0 || (sgn<0 && (mk_lreal)val>(mk_lreal)mk_i64limit)) {
      errComp(aux::Asciistr("err : cannot convert to integer (o)"));
      return m_numfmt;
    }
    else {
      if (sgn<0)
        aux::i2a(val,&buf,m_numfmt);
      else
        aux::ui2a(val,&buf,m_numfmt);
      setInput(buf);
    }
  }
  else if (m_ops.count()>0) {
    Entry *lastentry=m_ops.at(m_ops.count()-1);
    int a=isAction(*lastentry);
    if ((a&fmtAction)==0 && (a&fmtUnaction)==0) {
      val=mk_a2ui((const char *)lastentry->m_str,&ok,&sgn,0);
      if (ok<0 || (sgn<0 && val>(mk_ulreal)mk_i64limit)) {
        errComp(aux::Asciistr("err : cannot convert to integer (n)"));
        return m_numfmt;
      }
      else {
        if (sgn<0)
          aux::i2a(val,&buf,m_numfmt);
        else
          aux::ui2a(val,&buf,m_numfmt);
        lastentry->m_str=buf;
        lastentry->m_option=m_numfmt;
      }
    }
  }

  updateOutput(buf);
  return m_numfmt;

}

int Calculator::chgSgn() {

  unsigned int opfmt=opFmt(fmtDirect);
  if ((opfmt&fmtInvert)>0)
    return -1;

  if (m_input.len()==0) {
    calcUnary(fn_chsgn);
    return 0;
  }
  char sgn=0;
  aux::Asciistr strmant,strexp;
  int idx=esplitanum(m_input,&strmant,&strexp);
  if (idx>=0) {
    sgn=(strexp.len()>0 ? strexp[0] : 0);
    if (sgn==0)
      strexp.append(mk_asciiminus);
    else if (sgn==mk_asciiplus)
      strexp.set(0,mk_asciiminus);
    else if (sgn==mk_asciiminus)
      strexp.set(0,mk_asciiplus);
    else
      strexp.prepend(mk_asciiminus);
    strmant.append(mk_asciiE);
    strmant.append(strexp);
  }
  else {
    sgn=strmant[0];
    if (sgn==mk_asciiplus)
      strmant.set(0,mk_asciiminus);
    else if (sgn==mk_asciiminus)
      strmant=strmant.substr(1,-1);
    else
      strmant.prepend(mk_asciiminus);
  }

  setInput(strmant);
  updateOutput(m_input);
  return 0;

}

unsigned int Calculator::isAction(Entry action) {

  Entry *fnd=m_actions.at(m_actions.find(action));
  return (fnd ? fnd->m_option : 0);

}

int Calculator::setOp(int idx,Entry op) {

  if (op.m_str.len()==0)
    return -1;
  if (idx<0 || idx>=m_ops.count())
    m_ops.append(op);
  else
    m_ops.replace(idx,op);
  return m_ops.count();

}

int Calculator::pushStore() {

  unsigned int opfmt=opFmt(fmtDirect);
  if (opfmt==fmtInvert)
    return popStore();
  aux::Asciistr store;
  Entry *lastop=m_ops.at(m_ops.count()-1);
  if (m_input.len()==0 && lastop) {
    unsigned int isaction=isAction(*lastop);
    if ((isaction&fmtAction)==0 && (isaction&fmtUnaction)==0)
      store=lastop->m_str;
  }
  else
    store=m_input;
  if (store.len()>0)
    m_store.push(store);
  else
    m_store.pop();
  return updateInfo(doInfo());

}

int Calculator::popStore() {

  unsigned int opfmt=opFmt(fmtDirect);
  if (opfmt==fmtInvert)
    return pushStore();
  setInput(0);
  Entry m(m_store.pop());
  setOp(-1,m);
  updateOutput(m.m_str);
  return updateInfo(doInfo());

}

aux::Asciistr Calculator::doInfo() {

  aux::Asciistr infostr;
  infostr.reserve(64);
  if ((opFmt(0)&fmtInvert)>0)
    infostr.append("Inv ");
  if (m_store.count()>0)
    infostr.append("Mem ");
  int i=0,cnt=m_paren.count();
  if (cnt>0)
    infostr.append("(");
  for (i=1;i<m_paren.count();i++)
    infostr.append(",(");
  return setInfo(infostr);

}

int Calculator::pushParen() {

  unsigned int opfmt=opFmt(fmtDirect);
  if (opfmt==fmtInvert)
    return popParen();
  validateHist();
  int opscnt=m_ops.count();
  if (opscnt > 0) {
    if ((isAction(m_ops[opscnt - 1])&fmtAction)==0) {
      errComp("err : prev must be operator");
      return -1;
    }
  }
  m_paren.push(m_ops);
  m_ops.clear();
  setInput(0);
  return updateInfo(doInfo());

}

int Calculator::popParen() {

  unsigned int opfmt=opFmt(fmtDirect);
  if (opfmt==fmtInvert)
    return pushParen();
  if (m_paren.count() < 1) {
    errComp("unmatched paren");
    return -1;
  }
  equals(true);
  int opscnt=m_ops.count();
  if (opscnt == 0) {
    errComp(0);
    return -1;
  }
  Entry lastnum=m_ops[opscnt - 1];
  m_ops=m_paren.pop();
  m_ops.append(lastnum);
  return updateInfo(doInfo());

}

int Calculator::appendNumber(aux::Asciistr num) {

  if (num.len()==0)
    return -1;
  opFmt(fmtDirect);
  aux::Asciistr input(m_input),strmant,strexp;
  int ok=-1,inplen=input.len(),idx=esplitanum(input,&strmant,&strexp);
  if (idx>=0 && m_numfmt!=fmtHex) {
    if (!isdigit(num[0]))
      return -1;
    char sgn=strexp[0];
    aux::Asciistr strexpa(sgn==mk_asciiplus || sgn==mk_asciiminus ? strexp.substr(1,-1) : strexp);
    int nexp=(int)mk_a2i((const char *)strexpa,&ok,0);
    if (ok<0)
      return -1;
    aux::i2a((mk_lreal)nexp,&strexpa,10,3,true);
    strexpa.set(0,strexpa[1]);
    strexpa.set(1,strexpa[2]);
    strexpa.set(2,num[0]);
    if(strexpa[0]==mk_asciizero)
      strexpa=strexpa.substr(1,-1);
    if(strexpa[0]==mk_asciizero)
      strexpa=strexpa.substr(1,-1);
    input=strmant;
    input.append(mk_asciiE);
    if (sgn==mk_asciiplus || sgn==mk_asciiminus)
      input.append(sgn);
    input.append(strexpa);
    if (mk_isnan(mk_a2d((const char *)input))!=0)
      return -1;
  }
  else {
    if (num[0]==mk_asciidec && m_numfmt!=fmtDec)
      return -1;
    if ((m_numfmt==fmtBin && inplen>63) ||
        (m_numfmt==fmtOct && inplen>20) ||
        (m_numfmt==fmtHex && inplen>15))
      return -1;
    idx=input.find(mk_asciidec,0,mk_asciif);
    if (idx>=0 && (inplen>15 || num[0]==mk_asciidec))
      return -1;
    if ((num[0]==mk_asciie || num[0]==mk_asciiE) && (
        (m_numfmt!=fmtHex && m_numfmt!=fmtDec) || (m_numfmt==fmtDec && inplen==0)))
      return -1;
    input.append(num);
    if (m_numfmt!=fmtDec) {
      ok=(int)m_numfmt;
      mk_a2ui((const char *)input,&ok);
    }
    else if (mk_isnan(mk_a2d((const char *)input))!=0)
      return -1;
  }
  setInput(input);
  updateOutput(m_input);

  return 0;

}

int Calculator::appendOperator(aux::Asciistr op) {

  Entry fnd(op);
  Entry *vop=m_actions.at(m_actions.find(fnd));
  if (!vop || vop->m_str.len()==0) {
    errComp("err : unknown operator");
    return -1;
  }
  fnd=*vop;
  unsigned int opfmt=opFmt(fmtDirect);
  if (opfmt==fmtInvert)
    fnd.invert();
  if (fnd.m_str.len()==0) {
    errComp("err : unknown operator");
    return -1;
  }
  if ((fnd.m_option&fmtUnaction)==0) {
    if (m_input.len()>0)
      setOp(-1,Entry(m_input,m_numfmt));
    setOp(-1,fnd);
    updateAction();
    m_input=0;
  }
  else
    calcUnary(fnd);

  //dbgOp(m_input,m_ops,__LINE__);

  return 0;

}

int Calculator::setExtraNumber(aux::Asciistr numtype) {

  aux::Asciistr input,known(fn_exp);
  if (numtype==known)
    return appendNumber("e+000");
  known=fn_euler;
  if (numtype==known) {
    aux::d2a(mk_euler,&input,15,0);
    return updateOutput(setInput(input));
  }
  known=fn_pi;
  if (numtype==known) {
    aux::d2a(mk_pi,&input,15,0);
    return updateOutput(setInput(input));
  }
  return 0;

}

int Calculator::validateHist() {

  int i=0,opcnt=m_ops.count();
  if (opcnt==0)
    return -1;

  aux::TVList<Entry> nums;
  aux::TVList<Entry> acts;
  Entry actentry,nextentry;
  if (opcnt==1) {
    actentry=m_ops[0];
    if (isAction(actentry))
      acts.append(actentry);
    else
      nums.append(actentry);
  }
  for (i=0;i<(opcnt-1);i++) {
    actentry=m_ops[i];
    nextentry=m_ops[i+1];
    if (actentry.m_str.len()>0) {
      if (isAction(actentry)==0) {
        if (isAction(nextentry)==0) {
          nums.clear();
          acts.clear();
        }
        else
          nums.append(actentry);
      }
      else {
        if (i==0)
          nums.append("0");
        if (isAction(nextentry)==0)
          acts.append(actentry);
      }
    }
    if (i==(opcnt-2) && nextentry.m_str.len()>0) {
      if (isAction(nextentry)==0)
        nums.append(nextentry);
      else
        acts.append(nextentry);
    }
  }
  m_ops.clear();
  int numcnt=nums.count();
  opcnt=acts.count();

/*printf ("collected: numcnt=%d actcnt=%d\n",numcnt,opcnt);
for (i=0;i<numcnt;i++)
printf ("i=%d numi=%s\n",i,qx(nums[i].str));
for (i=0;i<opcnt;i++)
printf("i=%d opi=%s\n",i,qx(acts[i]));*/

  if (numcnt==0 && opcnt==0)
    return -1;
  if (numcnt==0 || opcnt==0) {
    if (numcnt==0)
      actentry=acts[0];
    else
      actentry=nums[0];
    if (actentry.m_str.len()>0) {
      if (numcnt==0)
        setOp(-1,"0");
      setOp(-1,actentry);
    }
    return -1;
  }
  Entry num1,num2,num3,op1,op2,op3;
  if (numcnt>2)
    num3=nums[2];
  if (numcnt>1)
    num2=nums[1];
  num1=nums[0];
  if (opcnt>2)
    op3=acts[2];
  if (opcnt>1)
    op2=acts[1];
  op1=acts[0];
  if (num1.m_str.len()==0)
    return -1;
  if (op1.m_str.len()==0) {
    setOp(-1,num1);
    return -1;
  }
  else if (num2.m_str.len()==0) {
    setOp(-1,num1);
    setOp(-1,op1);
    return -1;
  }
  if (op2.m_str.len()==0) {
    setOp(-1,num1);
    setOp(-1,op1);
    if (num2.m_str.len()>0)
      setOp(-1,num2);
    return -1;
  }
  else if (num2.m_str.len()==0) {
    setOp(-1,num1);
    setOp(-1,op1);
    return -1;
  }

  unsigned int a1=isAction(op1),a2=isAction(op2);
  if ((a1&fmtAddaction)>0 && (a2&fmtMultaction)>0 && op3.m_str.len()>0) {
    setOp(-1,num1);
    setOp(-1,op1);
    setOp(-1,num2);
    setOp(-1,op2);
    if (num3.m_str.len()>0)
      setOp(-1,num3);
    return -1;
  }
  setOp(-1,num1);
  setOp(-1,op1);
  setOp(-1,num2);
  setOp(-1,op2);
  if (num3.m_str.len()>0)
    setOp(-1,num3);
  if (op3.m_str.len()>0)
    setOp(-1,op3);

  return 0;

}

int Calculator::updateAction() {

  int validated=validateHist();
//for (int i=0;i<(int)ops.count();i++) printf("validated %d i=%d opi=%s\n",validated,i,qx(ops[i].str));
  if (validated<0)
    return -1;
  int numcnt=0,opcnt=0,opscnt=m_ops.count();
//for (i=0;i<opscnt;i++) printf("updAction i=%d opi=%s\n",i,qx(ops[i].str));
  if (opscnt<4)
    return -1;
  Entry num1=m_ops[0];
  numcnt++;
  Entry op1=m_ops[1];
  opcnt++;
  Entry num2=m_ops[2];
  numcnt++;
  Entry op2=m_ops[3];
  opcnt++;
  Entry num3;
  Entry op3;
  if (opscnt>4) {
    num3=m_ops[4];
    numcnt++;
  }
  if (opscnt>5) {
    op3=m_ops[5];
    opcnt++;
  }
  m_ops.clear();
  if (numcnt<3) {
    num1=compute(num1,num2,op1);
    if (num1.m_option!=m_numfmt)
      delayedChgFmt(num1.m_option);
    if (num1.m_str.len()==0) {
      errComp(0);
      return -1;
    }
    setOp(-1,num1);
    setOp(-1,op2);
    updateOutput(num1.m_str);
    return -1;
  }
  if (opcnt<3)
    return -1;
  num2=compute(num2,num3,op2);
  if (num2.m_option!=m_numfmt)
    delayedChgFmt(num2.m_option);
  if (num1.m_str.len()==0) {
    errComp(0);
    return -1;
  }
  if (op3==fn_mult || op3==fn_pow || op3==fn_invpow || op3==fn_div) {
    setOp(-1,num1);
    setOp(-1,op1);
    setOp(-1,num2);
    setOp(-1,op3);
    updateOutput(num2.m_str);
    return -1;
  }
  num1=compute(num1,num2,op1);
  if (num1.m_option!=m_numfmt)
    delayedChgFmt(num1.m_option);
  if (num1.m_str.len()) {
    errComp(0);
    return -1;
  }
  setOp(-1,num1);
  setOp(-1,op3);
  updateOutput(num1.m_str);

  return 0;

}

Entry Calculator::compute(Entry num1_,Entry num2_,Entry action) {

  Entry res;
  if (num1_.m_str.len()==0 || num2_.m_str.len()==0 || action.m_str.len()==0)
    return res;
  if (isAction(action)==0)
    return res;
  Entry num1=num1_,num2=num2_;
  int fmt1=num1.m_option,fmt2=num2.m_option;
  int oki1=fmt1,oki2=fmt2,i=0,prec1=0,prec2=0,sgnr=1,sgni1=1,sgni2=1;
  mk_ulreal nl1=mk_a2ui((const char *)num1.m_str,&oki1,&sgni1),
                nl2=mk_a2ui((const char *)num2.m_str,&oki2,&sgni2);

  if (oki1<0 && (fmt1==2 || fmt1==8 || fmt1==16))
    return res;
  if (oki2<0 && (fmt2==2 || fmt2==8 || fmt2==16))
    return res;
  bool isintcalc=(oki1>0 && oki2>0);
  mk_ulreal rl=0;
  double nd1=.0,nd2=.0,rd=.0;
  if (oki1>0)
    nd1=(double)sgni1*double(nl1);
  else
    nd1=mk_a2d((const char *)num1.m_str,&prec1);
  if (oki2>0)
    nd2=(double)sgni2*double(nl2);
  else
    nd2=mk_a2d((const char *)num2.m_str,&prec2);
  if (mk_isnan(nd1)!=0 || mk_isnan(nd2)!=0)
    return res;
  if (action==fn_plus)
    rd=nd1+nd2;
  else if (action==fn_minus)
    rd=nd1-nd2;
  else if (action==fn_mult)
    rd=nd1*nd2;
  else if (action==fn_div)
    rd=nd1/nd2;
  else if (action==fn_pow)
    rd=pow(nd1,nd2);
  else if (action==fn_invpow)
    rd=pow(nd1,1.0/nd2);
  else
    return res;
  if (!mk_isfinite(rd))
    return res;
  int magrd=mk_mag(rd);
  if (isintcalc && (
    magrd>mk_ui64mag ||
    (action==fn_pow && sgni2<0) ||
     action==fn_invpow ||
     (action==fn_div && (nl1<nl2 || (nl1%nl2)!=0))))
    isintcalc=false;
  if (isintcalc) {
    Entry iaction=action;
    int overflow=0;
    if (iaction==fn_plus && sgni1!=sgni2)
      iaction=fn_minus;
    else if (iaction==fn_minus && sgni1!=sgni2)
      iaction=fn_plus;
    if (iaction==fn_plus) {
      rl=mk_binadd(nl1,nl2,&overflow);
      if (overflow&1)
        isintcalc=false;
      else if (sgni1<0)
        sgnr=-1;
    }
    else if (iaction==fn_minus) {
      if (nl1>nl2) {
        rl=nl1-nl2;
        if (sgni1<0)
          sgnr=-1;
      }
      else {
        rl=nl2-nl1;
        if (sgni1>0)
          sgnr=-1;
      }
    }
    else if (iaction==fn_mult) {
      rl=mk_binmult(nl1,nl2,&overflow);
      if (overflow&1)
        isintcalc=false;
      else if ((sgni1<0 && sgni2>0) || (sgni1>0 && sgni2<0))
        sgnr=-1;
    }
    else if (iaction==fn_div) {
      rl=nl1/nl2;
      if ((sgni1<0 && sgni2>0) || (sgni1>0 && sgni2<0))
        sgnr=-1;
    }
    else if (iaction==fn_pow) {
      rl=1;
      for (i=0;i<(int)nl2;i++) {
        rl=mk_binmult(rl,nl1,&overflow);
        if (overflow&1) {
          isintcalc=false;
          break;
        }
      }
      if (sgni1<0 && nl2!=0 && nl2%2)
        sgnr=-1;
    }
    if (isintcalc && rl>(mk_ulreal)mk_i64limit && sgnr<0)
      isintcalc=false;
  }
  aux::Asciistr buf;
  if (isintcalc) {
    if (sgnr<0)
      aux::i2a(sgnr*rl,&buf,fmt2);
    else
      aux::ui2a(rl,&buf,fmt2);
    res.m_str=buf;
    res.m_option=fmt2;
  }
  else {
    if (action==fn_plus || action==fn_minus)
      aux::d2a(rd,&buf,prec1>prec2 ? prec1 : prec2,-1);
    else
      aux::d2a(rd,&buf,-magrd+mk_dprec,-1);
    res.m_str=buf;
    res.m_option=fmtDec;
    //doFmt(11);
  }

  return res;

}

Entry Calculator::calcUnary(Entry unop) {

  Entry res;
  if ((isAction(unop)&fmtUnaction)==0)
    return res;
  Entry numstr;
  aux::Asciistr strDbg;
  if (m_input.len()>0)
    numstr.m_str=m_input;
  else {
    validateHist();
    int opscnt=m_ops.count();
    if (opscnt==0)
      return res;
    while (opscnt>0) {
      numstr=m_ops[--opscnt];
      if (numstr.m_str.len()==0 || (isAction(numstr)&fmtAction)>0)
        continue;
      else
        break;
    }
    if (numstr.m_str.len()==0 || (isAction(numstr)&fmtAction)>0)
      return res;
  }
  bool isintcalc=false;
  int ok=(int)m_numfmt,sgni=1,prec=0;
  double n=0.;
  mk_ulreal ni=mk_a2ui((const char *)numstr.m_str,&ok,&sgni);
  if (ok<0) {
    if (m_numfmt==fmtBin || m_numfmt==fmtOct || m_numfmt==fmtHex) {
      errComp(0);
      return res;
    }
  }
  else
    isintcalc=true;
  if (isintcalc)
    n=double(ni);
  else {
    n=mk_a2d((const char *)numstr.m_str,&prec);
    if (mk_isnan(n)!=0) {
      strDbg="err : cannot parse number ";
      strDbg.append((const char *)numstr.m_str);
      errComp(strDbg);
      return res;
    }
  }
  if (!(unop==fn_sqr))
    isintcalc=false;
  double trignum=((m_trigfmt&fmtRad)>0 ? 1. : mk_rad);
  if (unop==fn_sqrt) {
    if (n<.0) {
      errComp("err : cannot compute transcendent number");
      return res;
    }
    n=sqrt(n);
  }
  else if (unop==fn_sqr) {
    if (isintcalc) {
      mk_ulreal tstni=ni;
      ni=ni*ni;
      if (ni<tstni)
        isintcalc=false;
    }
    n=n*n;
  }
  else if (unop==fn_sin)
    n=mk_round2(sin(n*trignum),14);
  else if (unop==fn_asin)
    n=asin(n)/trignum;
  else if (unop==fn_cos)
    n=mk_round2(cos(n*trignum),14);
  else if (unop==fn_acos)
    n=acos(n)/trignum;
  else if (unop==fn_tan) {
    if (mk_deq(cos(n*trignum),.0)) {
      errComp("err : cannot divide by \"0\"");
      return res;
    }
    n=tan(n*trignum);
  }
  else if (unop==fn_atan)
    n=atan(n)/trignum;
  else if (unop==fn_ln) {
    if (n<=.0) {
      errComp("err : cannot compute transcendent number");
      return res;
    }
    n=log(n);
  }
  else if (unop==fn_powe) {
    if (n>log(mk_dlimit)) {
      errComp("err : input too large");
      return res;
    }
    n=exp(n);
  }
  else if (unop==fn_log) {
    if (n<=0.0) {
      errComp("err : cannot compute transcendent number");
      return res;
    }
    n=log10(n);
  }
  else if (unop==fn_pow10) {
    if (n>(double)mk_mag(mk_dlimit)) {
      errComp("err : input too large");
      return res;
    }
    n=pow(10.,n);
  }
  else if (unop==fn_fac) {
    ok=0;
    int in=(int)mk_a2i((const char *)numstr.m_str,&ok,0);
    if (ok<0) {
      errComp("err : input must be integer");
      return res;
    }
    n=mk_factorial(in);
    if (n<1.0) {
      errComp(0);
      return res;
    }
  }
  else if (unop==fn_reci) {
    if (mk_deq(n,.0)) {
      errComp("err : cannot divide by \"0\"");
      return res;
    }
    n=1./n;
  }
  else if (unop==fn_chsgn) {
    aux::Asciistr strmant,strexp;
    int idx=esplitanum(numstr.m_str,&strmant,&strexp);
    if (idx>=0) {
      char sgn=strexp[0];
      strmant.append(mk_asciiE);
      if (sgn!=mk_asciiminus)
        strmant.append(mk_asciiminus);
      if (sgn==mk_asciiplus || sgn==mk_asciiminus)
        strmant.append(strexp.substr(1,-1));
      else
        strmant.append(strexp);
      n=mk_a2d((const char *)strmant,&prec);
      if (mk_isnan(n)!=0) {
        errComp(0);
        return res;
      }
    }
    else n=-n;
  }
  aux::Asciistr buf;
  if (isintcalc)
    aux::ui2a(ni,&buf,m_numfmt);
  else {
    aux::d2a(n,&buf,-mk_mag(n)+mk_dprec,-1);
    //doFmt(11);
  }
  numstr.m_str=buf;

  if (numstr.m_str.len()==0) {
    errComp(0);
    return res;
  }
  setOp(-1,numstr);
  setInput(0);
  updateOutput(numstr.m_str);

  return numstr;

}

int Calculator::equals(bool calcparen) {

  opFmt(fmtDirect);
  if (!calcparen && m_paren.count()>0) {
    errComp("err : unmatched paren");
    return -1;
  }
  if (m_input.len()>0)
    setOp(-1,Entry(m_input,m_numfmt));
  setInput(0);

  validateHist();
  int numcnt=0,opcnt=0,opscnt=m_ops.count();
  if (opscnt==0)
    return -1;
  Entry num1,num2,num3,op1,op2,op3;
  if (opscnt>0) {
    num1=m_ops[0];
    numcnt++;
  }
  if (opscnt>1) {
    op1=m_ops[1];
    opcnt++;
  }
  if (opscnt>2) {
    num2=m_ops[2];
    numcnt++;
  }
  if (opscnt>3) {
    op2=m_ops[3];
    opcnt++;
  }
  if (opscnt>4) {
    num3=m_ops[4];
    numcnt++;
  }
  if (opscnt>5) {
    op3=m_ops[5];
    opcnt++;
  }
  m_ops.clear();

  if (opcnt<1) {
    setOp(-1,num1);
    return -1;
  }

  if (numcnt<3 || opcnt<2) {
    num1=compute(num1,numcnt<2 ? num1 : num2,op1);
    if (num1.m_option!=m_numfmt)
      delayedChgFmt(num1.m_option);
    if (num1.m_str.len()==0) {
      errComp(0);
      return -1;
    }
    setOp(-1,num1);
    updateOutput(num1.m_str);
    return 0;
  }
  num2=compute(num2,num3,op2);
  if (num2.m_option!=m_numfmt)
    delayedChgFmt(num2.m_option);
  if (num2.m_str.len()==0) {
    errComp(0);
    return -1;
  }
  num1=compute(num1,num2,op1);
  if (num1.m_option!=m_numfmt)
    delayedChgFmt(num1.m_option);
  if (num1.m_str.len()==0) {
    errComp(0);
    return -1;
  }
  setOp(-1,num1);
  updateOutput(num1.m_str);

  //dbgOp(m_input,m_ops,__LINE__);

  return 0;

}

int Calculator::checkFnActive() {

  int ii=0,fnuifmt=((m_numfmt==2 || m_numfmt==8 || m_numfmt==16) ? fn_Inactive : fn_Active);

  for (ii=0;ii<numfn_operator;ii++) {
    if (fn_operator[ii]!=fn_div && fn_operator[ii]!=fn_minus &&
        fn_operator[ii]!=fn_mult && fn_operator[ii]!=fn_plus)
      fnActive(fn_operator[ii],fnuifmt);
  }
  fnActive(fn_exp,fnuifmt);
  fnActive(fn_chsgn,fnuifmt);
  fnActive(fn_euler,fnuifmt);
  fnActive(fn_pi,fnuifmt);
  fnActive(fn_decsep,fnuifmt);
  for (ii=1;ii<numfn_number;ii++)
    fnActive(fn_number[ii],ii>(int)m_numfmt ? fn_Inactive : fn_Active);
  fnActive(fn_parenr,m_paren.count()>0 ? fn_Active : fn_Inactive);
  fnActive(fn_memrcl,m_store.count()>0 ? fn_Active : fn_Inactive);

  return 0;

}

int Calculator::fnActive(const char *,int) {

  return 0;

}

static void key2calcstr(osix::xxEvent *keyInput,aux::Ucsstr *str) {

  if (keyInput->m_xxk==osix::xxk_comma)
    str->append(fn_decsep);
  else if (keyInput->m_xxk==osix::xxk_plus)
    str->append(fn_plus);
  else if (keyInput->m_xxk==osix::xxk_minus)
    str->append(fn_minus);
  else if (keyInput->m_xxk==osix::xxk_asterisk)
    str->append(fn_mult);
  else if (keyInput->m_xxk==osix::xxk_slash)
    str->append(fn_div);
  else if (keyInput->m_xxk>=osix::xxk_a && keyInput->m_xxk<=osix::xxk_f)
    str->append((unsigned short)toupper((int)keyInput->m_xxk));
  else
    str->append((unsigned short)keyInput->m_xxk);

}

// **********

int Calculator::updateOutput(aux::Asciistr msg) {

  if (msg.len() == 0)
    msg=m_input;
  if (msg.len() == 0)
    msg="0";
  m_output=msg;
  osUpdate(m_outputwin.m_w);
  return 0;

}

int Calculator::updateInfo(aux::Asciistr msg) {

  if (msg.len() == 0)
    msg=m_info;
  if (msg.len() == 0)
    msg=" ";
  m_showinfo=msg;
  osUpdate(m_showinfowin.m_w);
  return 0;

}

int Calculator::osEvent(void *disp,void *win,void *osev) {

  osix::xxEvent xxev;
  osix::xxtranslateEvent(osev,&xxev);
  osix::xxGC gc;
  aux::Ucsstr str;
  if (xxev.m_type==osix::xx_resize) {
    if (win==m_outputwin.m_w)
      m_outputwin.m_r=xxev.m_r;
    else if (win==m_showinfowin.m_w)
      m_showinfowin.m_r=xxev.m_r;
  }
  else if (xxev.m_type==osix::xx_paint) {
    if (win==m_outputwin.m_w) {
      gc.m_fg=m_outputfg;
      gc.m_bg=m_outputbg;
      gc.m_fnt=m_outputfnt;
      gc.m_r=m_outputwin.m_r;
      str=(const char *)m_output;
      osix::xxpredraw(disp,&m_outputwin,&gc);
      osix::xxfillRect(disp,&m_outputwin,&gc);
      osix::xxdrawText(disp,&m_outputwin,&gc,&str,osix::xx_alignright|osix::xx_alignvcenter);
      osix::xxpostdraw(disp,&m_outputwin,&gc);
    }
    else if (win==m_showinfowin.m_w) {
      gc.m_fg=m_infofg;
      gc.m_bg=m_infobg;
      gc.m_fnt=m_infofnt;
      gc.m_r=m_showinfowin.m_r;
      str=(const char *)m_showinfo;
      osix::xxpredraw(disp,&m_showinfowin,&gc);
      osix::xxfillRect(disp,&m_showinfowin,&gc);
      osix::xxdrawText(disp,&m_showinfowin,&gc,&str,osix::xx_alignright|osix::xx_alignvcenter);
      osix::xxpostdraw(disp,&m_showinfowin,&gc);
    }
    checkFnActive();
  }
  else if (osix::xxTypeInput(&xxev)>0)
    inputEvent(&xxev);

  return xxev.m_consumer;

}

int Calculator::inputEvent(osix::xxEvent *xxev) {

  if (osix::xxTypeMouse(xxev)>0)
    return xxev->m_consumer;

  if (xxev->m_type==osix::xx_keyPressed)
    xxev->m_consumer|=osix::xx_consumed;
  else if (xxev->m_type==osix::xx_keyReleased) {
    if ((m_xxlastinputev.m_consumer&osix::xx_consumed)>0)
      xxev->m_consumer|=osix::xx_consumed;
    if ((m_xxlastinputev.m_consumer&osix::xx_processed)==0)
      return xxev->m_consumer;
  }
  xxev->m_xxk=osix::lowerkey(xxev);
  int twkey=osix::typewriterkey(xxev);
  aux::Ucsstr str;
  key2calcstr(xxev,&str);
  if ((twkey&osix::xxk_ascii)>0) {
    if ((xxev->m_mods&osix::xx_modCtrl)>0) {
      if (xxev->m_xxk==osix::xxk_c) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          doCopy();
      }
      if (xxev->m_xxk==osix::xxk_x) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          doCut();
      }
      if (xxev->m_xxk==osix::xxk_v) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          doPaste();
      }
    }
    else if (xxev->m_mods==0 || (xxev->m_mods&osix::xx_modShift)>0) {
      if ((xxev->m_xxk>=osix::xxk_0 && xxev->m_xxk<=osix::xxk_9) ||
          (xxev->m_xxk>=osix::xxk_a && xxev->m_xxk<=osix::xxk_f) ||
          xxev->m_xxk==osix::xxk_period) {
        if (xxev->m_type==osix::xx_keyPressed)
          xxev->m_consumer|=osix::xx_processed;
        else {
          if (
            (m_numfmt==fmtBin &&
              (xxev->m_xxk==osix::xxk_0 || xxev->m_xxk==osix::xxk_1))
            ||
            (m_numfmt==fmtOct &&
              (xxev->m_xxk>=osix::xxk_0 && xxev->m_xxk<=osix::xxk_8))
            ||
            (m_numfmt==fmtDec &&
              ((xxev->m_xxk>=osix::xxk_0 && xxev->m_xxk<=osix::xxk_9) ||
                xxev->m_xxk==osix::xxk_period))
            ||
            (m_numfmt==fmtHex &&
              ((xxev->m_xxk>=osix::xxk_0 && xxev->m_xxk<=osix::xxk_9) ||
                (xxev->m_xxk>=osix::xxk_a && xxev->m_xxk<=osix::xxk_f)))
          ) {
            xxev->m_consumer|=osix::xx_processed;
            appendNumber(str.asciistr());
          }
        }
      }
      else if (xxev->m_xxk==osix::xxk_asterisk || xxev->m_xxk==osix::xxk_plus ||
               xxev->m_xxk==osix::xxk_minus || xxev->m_xxk==osix::xxk_slash) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          appendOperator(str.asciistr());
      }
      else if (xxev->m_xxk==osix::xxk_parenleft) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          pushParen();
      }
      else if (xxev->m_xxk==osix::xxk_parenright) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          popParen();
      }
      else if (xxev->m_xxk==osix::xxk_equal) {
        xxev->m_consumer|=osix::xx_processed;
        if (xxev->m_type==osix::xx_keyReleased)
          equals();
      }
    }
  }
  else if (twkey==0) {
    if (xxev->m_xxk==osix::xxk_Enter || xxev->m_xxk==osix::xxk_Return) {
      xxev->m_consumer|=osix::xx_processed;
      if (xxev->m_type==osix::xx_keyReleased)
        equals();
    }
    else if (xxev->m_xxk==osix::xxk_Delete) {
      xxev->m_consumer|=osix::xx_processed;
      if (xxev->m_type==osix::xx_keyReleased)
        clear();
    }
    else if (xxev->m_xxk==osix::xxk_BackSpace) {
      xxev->m_consumer|=osix::xx_processed;
      if (xxev->m_type==osix::xx_keyReleased) {
        opFmt(calculator::fmtDirect);
        if (m_input.len()>0) {
          m_input=m_input.substr(0,m_input.len()-1);
          updateOutput(m_input);
        }
        updateInfo(doInfo());
      }
    }
    else if (xxev->m_xxk==osix::xxk_Shift_L || xxev->m_xxk==osix::xxk_Super_L)
      xxev->m_consumer|=osix::xx_processed;
  }

  m_xxlastinputev=*xxev;

  return xxev->m_consumer;

}

int Calculator::doCopy() {

  aux::Ucsstr str;
  str=(const char *)m_output;
  osix::xxtoClipboard(&str);
  return 0;

}

int Calculator::doCut() {

  aux::Ucsstr str;
  str=(const char *)m_output;
  osix::xxtoClipboard(&str);
  int opscnt=m_ops.count();
  if (m_input.len() > 0) {
    setInput("");
  }
  else if (opscnt > 0) {
    Entry lastentry=m_ops[opscnt - 1];
    unsigned int isaction=isAction(lastentry);
    if ((isaction&fmtAction)==0 && (isaction&fmtUnaction) == 0)
      m_ops.remove(opscnt - 1);
  }
  updateOutput(m_input);
  return 0;

}

int Calculator::doPaste() {

  aux::Ucsstr str;
  osix::xxfromClipboard(&str);
  setInput(str.asciistr());
  updateOutput(m_input);
  return 0;

}

} // namespace

