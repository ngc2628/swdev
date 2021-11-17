
//..end "File Description"

//..begin "Ifdef"
#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_
//..end "Ifdef"

#include <stdio.h>
#include <tools/misc.h>
#include <tools/tlist.h>
#include <osix/xxevent.h>
#include <osix/xxpaint.h>
#include <txtcalc/txtcalc.h>

namespace calculator {

const unsigned int fgColor=4278848010U; // ~black
const unsigned int bgColor=4293519855U; // almost white
const unsigned int dbuttonfgColor=4286090370U;
const unsigned int dbuttonbgColor=4288722090U;
const unsigned int ebuttonfgColor=4278848010U;
const unsigned int ebuttonbgColor=4292137160U;

const unsigned int fmtBin=2;
const unsigned int fmtOct=8;
const unsigned int fmtDec=10;
const unsigned int fmtHex=16;
const unsigned int fmtNum=26;
const unsigned int fmtDeg=128;
const unsigned int fmtRad=256;
const unsigned int fmtTrig=384;
const unsigned int fmtDirect=512;
const unsigned int fmtInvert=1024;
const unsigned int fmtDir=1536;
const unsigned int fmtAddaction=2048;
const unsigned int fmtMultaction=4096;
const unsigned int fmtAction=8191;
const unsigned int fmtUnaction=8192;
const unsigned int fmtToggle=65536;

const char* const fn_inv="inv";
const char* const fn_reci="reci";
const char* const fn_memrcl="memrcl";
const char* const fn_memput="memput";
const char* const fn_clearall="clearall";
const char* const fn_clear="clear";
const char* const fn_ln="ln";
const char* const fn_log="log";
const char* const fn_sqrt="sqrt";
const char* const fn_sqr="sqr";
const char* const fn_pow="pow";
const char* const fn_invpow="invpow";
const char* const fn_pow10="pow10";
const char* const fn_powe="powe";
const char* const fn_exp="exp";
const char* const fn_sin="sin";
const char* const fn_asin="asin";
const char* const fn_cos="cos";
const char* const fn_acos="acos";
const char* const fn_tan="tan";
const char* const fn_atan="atan";
const char* const fn_fac="fac";
const char* const fn_euler="euler";
const char* const fn_pi="pi";
const char* const fn_7="7";
const char* const fn_8="8";
const char* const fn_9="9";
const char* const fn_div="div";
const char* const fn_A="A";
const char* const fn_B="B";
const char* const fn_4="4";
const char* const fn_5="5";
const char* const fn_6="6";
const char* const fn_mult="mult";
const char* const fn_C="C";
const char* const fn_D="D";
const char* const fn_1="1";
const char* const fn_2="2";
const char* const fn_3="3";
const char* const fn_plus="plus";
const char* const fn_E="E";
const char* const fn_F="F";
const char* const fn_0="0";
const char* const fn_chsgn="chsgn";
const char* const fn_decsep=".";
const char* const fn_minus="minus";
const char* const fn_parenl="parenl";
const char* const fn_parenr="parenr";
const char* const fn_equals="equals";
const char* const fn_dec="dec";
const char* const fn_bin="bin";
const char* const fn_oct="oct";
const char* const fn_hex="hex";
const char* const fn_deg="deg";
const char* const fn_rad="rad";

const int numfn_number=17;
const char* const fn_number[numfn_number]={
fn_decsep,fn_0,fn_1,fn_2,fn_3,fn_4,fn_5,fn_6,
fn_7,fn_8,fn_9,fn_A,fn_B,fn_C,fn_D,fn_E,fn_F
};

const int numfn_operator=14;
const char* const fn_operator[numfn_operator]={
fn_cos,fn_div,fn_fac,fn_ln,fn_log,fn_minus,fn_mult,
fn_plus,fn_pow,fn_reci,fn_sin,fn_sqr,fn_sqrt,fn_tan
};

const int fn_Active=1;
const int fn_Inactive=2;

const int maxlenstr=256;

class oswinexp Asciistr {

  protected:
    char *m_str;
    int m_sz;

  public:
    Asciistr(const char *str=0);
    Asciistr(const Asciistr &);
    ~Asciistr();
    Asciistr &operator=(const Asciistr &);
    Asciistr &operator=(const char *);
    operator const char *() const;
    char operator[](int) const;
    bool operator==(const Asciistr &) const;
    bool operator<(const Asciistr &) const;
    const char *data() const;
    char *rawdata();
    int len() const;
    int reserve(int);
    int cut(int newl=0);
    int set(int,char);
    int find(char,int idx=0,unsigned char dir='f',unsigned char ci=0) const;
    Asciistr substr(int,int) const;
    Asciistr &append(char);
    Asciistr &append(const char *);
    Asciistr &prepend(char);
    Asciistr &prepend(const char *);
    Asciistr &lower();
    Asciistr &upper();
    Asciistr &strip();

};

extern int oswinexp ui2a(
  mk_ulreal,Asciistr*,int base=10,int width=0,int padzero=1,const char *group=0);
   
extern int oswinexp i2a(
  mk_lreal,Asciistr*,int base=10,int width=0,int padzero=1,const char *group=0);

extern int oswinexp d2a(
  double,Asciistr*,int prec=15,char fmt=-1,int pad=-1,const char *dec=".",const char *group=0);

class oswinexp Entry {

  public:
    Asciistr m_str;
    unsigned int m_option;
    Asciistr m_invstr;
    Entry(const char *str=0,unsigned int option=fmtDec,const char *invstr=0) :
      m_str(str),m_option(option),m_invstr(invstr) {
    }
    Entry(const Entry &ass) : m_str(ass.m_str),m_option(ass.m_option),m_invstr(ass.m_invstr) {
    }
    ~Entry() {
    }
    Entry &operator=(const Entry &ass) {
      if (this==&ass)
    	return *this;
      m_str=ass.m_str;
      m_option=ass.m_option;
      m_invstr=ass.m_invstr;
      return *this;
    }
    bool operator==(const Entry &cmp) const {
      return (m_str==cmp.m_str);
    }
    bool operator<(const Entry &cmp) const {
      return (m_str<cmp.m_str);
    }
    int invert() {
      if (m_invstr.len()>0) {
        mk::swap(&m_str,&m_invstr);
        return 0;
      }
      return -1;
    }

};

class oswinexp Calculator {

  public:
    osix::xxDrawable m_outputwin,m_showinfowin;
    osix::xxStyle m_outputfg,m_outputbg,m_infofg,m_infobg;
    osix::xxFnt m_outputfnt,m_infofnt;
    Calculator ();
    virtual ~Calculator ();
    virtual int isFnNumber(const char *);
    virtual int isFnOperator(const char *);
    virtual int isFnExtraNumber(const char *);
    virtual int doCopy();
    virtual int doCut();
    virtual int doPaste();

  protected:
    osix::xxEvent m_xxlastinputev;
    unsigned int m_numfmt,m_trigfmt,m_opfmt;
    Asciistr m_input,m_info,m_output,m_showinfo;
    mk::TVStack<Asciistr> m_store;
    mk::TVList<Entry> m_ops;
    mk::TVStack<mk::TVList<Entry> > m_paren;
    mk::TVList<Entry> m_actions;

    Calculator(const Calculator &) {
    }
    Calculator &operator=(const Calculator &) {
      return *this;
    }

    virtual Asciistr setInput(Asciistr);
    virtual Asciistr setInfo(Asciistr);
    virtual int clear();
    virtual int clean();
    virtual int errComp(Asciistr);
    virtual unsigned int chgFmt(unsigned int);
    virtual unsigned int trigFmt(unsigned int);
    virtual unsigned int opFmt(unsigned int);
    virtual unsigned int doFmt(unsigned int);
    virtual int chgSgn();
    virtual unsigned int isAction(Entry);
    virtual int setOp(int,Entry);
    virtual int pushStore();
    virtual int popStore();
    virtual Asciistr doInfo();
    virtual int pushParen();
    virtual int popParen();
    virtual int appendNumber(Asciistr);
    virtual int appendOperator(Asciistr);
    virtual int setExtraNumber(Asciistr);
    virtual int validateHist();
    virtual int updateAction();
    virtual Entry compute(Entry,Entry,Entry);
    virtual Entry calcUnary(Entry);
    virtual int equals(bool calcparen=false);
    virtual unsigned int delayedChgFmt(unsigned int chgfmt) {
      return chgFmt(chgfmt);
    }
    virtual int checkFnActive();
    virtual int fnActive(const char *,int);
    virtual int updateOutput(Asciistr);
    virtual int updateInfo(Asciistr);
    virtual void *findDisplay() {
      return 0;
    }
    virtual int osEvent(void *,void *,void *);
    virtual int inputEvent(osix::xxEvent *);
    virtual int osUpdate(void*) {
      return 0;
    }

};

} // namespace

#endif
