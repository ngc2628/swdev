
#ifndef _auxx_h_
#define _auxx_h_

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <auxx/exportdef.h>

namespace aux {

// ***** integers
#if defined (_MSC_VER)
#include <basetsd.h>
#if defined (_WIN64)
typedef int64_t lreal;
typedef uint64_t ulreal;
#else
typedef __int64__ lreal;
typedef __uint64__ ulreal;
#endif
const lreal i64Limit=9223372036854775807i64;
const ulreal ui64Limit=18446744073709551615ui64 ;
const unsigned int uiLimit=4294967295;
#elif defined (__WATCOMC__)
typedef int64_t lreal;
typedef uint64_t ulreal;
const lreal i64Limit=9223372036854775807LL;
const ulreal ui64Limit=18446744073709551615ULL;
const unsigned int uiLimit=4294967295U;
#define finite(x) isfinite(x)
#elif defined (__MACH__)
#include <inttypes.h>
typedef long long int lreal;
typedef unsigned long long int  ulreal;
const ulreal i64Limit=9223372036854775807LL;
const ulreal ui64Limit=18446744073709551615ULL;
const unsigned int uiLimit=4294967295U;
#else
#include <inttypes.h>
typedef long long int lreal;
typedef unsigned long long int  ulreal;
const lreal i64Limit=9223372036854775807LL;
const ulreal ui64Limit=18446744073709551615ULL;
const unsigned int uiLimit=4294967295U;
#endif
const int iLimit=2147483647;
const unsigned short usLimit=65535;
const short sLimit=32767;


// ***** floatings

/* const unsigned short _d0=0; // big endian
const unsigned short _d1=1;
const unsigned short _d2=2;
const unsigned short _d3=3; */
const unsigned short _d0=3; // little endian
const unsigned short _d1=2;
const unsigned short _d2=1;
const unsigned short _d3=0;
const unsigned short dBias=1022;
const unsigned short dOff=4;
const unsigned short dFrac=15;
const unsigned short dMask11=32752;
const unsigned short ndig=16;
union tp_ucpd {
  unsigned char ucd[8];
  double d;
};
//const union tp_ucpd defdnan={{127,248,0,0,0,0,0,0}}; // big endian
const union tp_ucpd defdnan={{0,0,0,0,0,0,248,127}}; // little endian
const double dnan=*(&defdnan.d);
//const union tp_ucpd defdsnan={{255,248,0,0,0,0,0,0}}; // big endian
const union tp_ucpd defdsnan={{0,0,0,0,0,0,248,255}}; // little endian
const double dsnan=*(&defdsnan.d);
//const union tp_ucpd defdinf={{127,240,0,0,0,0,0,0}}; // big endian
const union tp_ucpd defdinf={{0,0,0,0,0,0,240,127}}; // little endian
const double dinf=*(&defdinf.d);
//const union tp_ucpd defdsinf={{255,240,0,0,0,0,0,0}}; // big endian
const union tp_ucpd defdsinf={{0,0,0,0,0,0,240,255}}; // little endian
const double dsinf=*(&defdsinf.d);

union tp_ucpf {
  unsigned char ucf[4];
  float f;
};
//const union tp_ucpf deffnan={{127,192,0,0}}; // big endian
const union tp_ucpf deffnan={{0,0,192,127}}; // little endian
const float fnan=*(&deffnan.f);
//const union tp_ucpf deffsnan={{255,192,0,0}}; // big endian
const union tp_ucpf deffsnan={{0,0,192,255}}; // little endian
const float fsnan=*(&deffsnan.f);
//const union tp_ucpf deffinf={{127,128,0,0}}; // big endian
const union tp_ucpf deffinf={{0,0,128,127}}; // little endian
const float finf=*(&deffinf.f);
//const union tp_ucpf deffsinf={{255,128,0,0}}; // big endian
const union tp_ucpf deffsinf={{0,0,128,255}}; // little endian
const float fsinf=*(&deffsinf.f);

// ***** static defaults
const double dLimit=1.7976931348623157e308;
const double dErr=0.000000000000001;
const double euler=2.718281828459045235;
const double pi=3.1415926535897932385;
const double rad=0.0174532925199432957694;
const double log210=0.3010299956639812;
const double loge10=0.4342944819032518;
const double log10e=2.3025850929940457;
const unsigned short dMag=308;
const unsigned short i64Mag=18;
const unsigned short ui64Mag=19;
const unsigned short dPrec=15;
const unsigned short ddPrec=3;
const unsigned short emptyUcsstr[1]={0};
const unsigned short nullUcsstr[7]={91,78,85,76,76,93,0};
const unsigned short blen=8;
const unsigned short wlen=16;
const unsigned short dwlen=32;
const unsigned short qwlen=64;
const unsigned short dqwlen=128;
const unsigned short idlen=256;
const unsigned short klen=1024;
const unsigned short k4len=4096;
const unsigned short separatorGroupLen=3;
const unsigned short maxSeparatorLen=6;
const unsigned short maxArrSz=20;
const unsigned short maxintbase=36;
const unsigned short maxNumstrLen=316;
const unsigned int maxStrLen=1073741824;
const unsigned char basechar[maxintbase+1]={'0','1','2','3','4','5','6','7','8','9',
                                  'A','B','C','D','E','F','G','H','I','J',
                                  'K','L','M','N','O','P','Q','R','S','T',
                                  'U','V','W','X','Y','Z','0'};
const unsigned char asciispace=32;
const unsigned char asciihash=35;
const unsigned char asciiplus=43;
const unsigned char asciiminus=45;
const unsigned char asciidec=46;
const unsigned char asciizero=48;
const unsigned char asciione=49;
const unsigned char asciifive=53;
const unsigned char asciinine=57;
const unsigned char asciiE=69;
const unsigned char asciiF=70;
const unsigned char asciiI=73;
const unsigned char asciiX=88;
const unsigned char asciib=98;
const unsigned char asciie=101;
const unsigned char asciif=102;
const unsigned char asciii=105;
const unsigned char asciix=120;
const unsigned char asciidel=127;
const char * const emptyString="";
const char *const nullAsciistr="[NULL]";

#if !defined MIN
#define MIN(x,y) ((x)<(y) ? (x) : (y))
#endif
#if !defined MAX
#define MAX(x,y) ((x)<(y) ? (y) : (x))
#endif

inline int oswinexp dsgn(double d) {
  return (((((const unsigned char *)&d)[7])>>7)>0 ? -1 : 1);
} // little endian 0 -> 7

inline double oswinexp dsign (double d) {
  return (double)dsgn(d);
}

inline int oswinexp isInf(double d) {

#if defined (_MSC_VER)
  return (_fpclass(d)==_FPCLASS_NINF ? -1 : (_fpclass(d)==_FPCLASS_PINF ? 1 : 0));
#else
  return (finite(d) ? 0 : dsgn(d));
#endif
}

inline int oswinexp isNan(double d) {
#if defined (_MSC_VER)
  return (_isnan(d) ? dsgn(d) : 0);
#else
  return (isnan(d) ? dsgn(d) : 0);
#endif
}

inline int oswinexp isFinite(double d) {
#if defined (_MSC_VER)
  return (_finite(d) ? dsgn(d) : 0);
#else
  return (finite(d) ? dsgn(d) : 0);
#endif
}

inline int oswinexp dbusted(double d) {
#if defined (_MSC_VER)
  return (!_isnan(d) && _finite(d) ? 0 : dsgn(d));
#else
  return (!isnan(d) && finite(d) ? 0 : dsgn(d));
#endif
}

extern ulreal oswinexp nextCnt();

extern ulreal oswinexp nextT();

extern double oswinexp ipow10(int);

extern double oswinexp ipow2(int);

extern int oswinexp exp2simple (char numstr[klen]);

extern int oswinexp removeSeparators(char *,const char * const,const char * const);

extern int oswinexp insertSeparators(char *,int,const char * const,const char * const);

extern ulreal oswinexp parseInt(const char *,int *,int *sign=0,const char *group=0);

extern double oswinexp parseFloat(const char *,int *type=0,int *acc=0,const char *decsep=".",const char *groupsep=0);

extern ulreal oswinexp a2ui(const char *,int *base=0,int *sign=0,const char *group=0);

extern lreal oswinexp a2i(const char *,int *base=0,const char *group=0);

extern int oswinexp ui2a(ulreal,char [klen],int base=10,int width=0,int padzero=1,const char *group=0);

extern int oswinexp i2a(lreal,char [klen],int base=10,int width=0,int padzero=1,const char *group=0);

extern ulreal oswinexp binAdd(ulreal,ulreal,int *overflow=0);

extern ulreal oswinexp binMult(ulreal,ulreal,int *overflow=0);

extern double oswinexp a2d(const char *,int *prec=0,const char *dec=".",const char *group=0);

extern int oswinexp dconvert (double,char [klen],int,char fmt=-1,int pad=-1);

extern int oswinexp d2a(double,char [klen],int prec=15,char fmt=-1,int pad=-1,const char *dec=".",const char *group=0);

extern int oswinexp exp2simple(char [klen]);

extern int oswinexp mag(double,int incacc=1);

extern int oswinexp dbusted(double,double,int *cmp=0);

extern int oswinexp deq(double,double);

extern int oswinexp dlt(double,double);

extern int oswinexp dgt(double,double);

extern double oswinexp diff(double,double,double eps=0.000000000000001,double tolerance=0.000001);

extern double oswinexp linEq(double,double,double,double,double);

extern double oswinexp lgEq(double,double,double,double,double);

extern double oswinexp round2(double,int roundPos=0,double errmag=0.0001);

extern double oswinexp roundUp(double,int,int trailing=3);

extern double oswinexp roundDown (double,int,int trailing=3);

extern double oswinexp logm(double,double);

extern double oswinexp powrat(double,float);

extern void oswinexp indextab1(int,const double[],int []);

extern void oswinexp iindextab1(int,const int [],int []);

extern void oswinexp indextab2(int,const double [],int []);

extern void oswinexp iindextab2(int,const int [],int []);

extern int oswinexp isPrim(int);

extern int oswinexp isPrim3(int);

extern int oswinexp nextLargerPrim(int);

extern int oswinexp lcm(int,int);

extern int oswinexp gcd(int,int);

extern double oswinexp doubleMod(double,double);

extern double oswinexp factorial(int);

extern double oswinexp binomialCoeff(int,int);

extern void oswinexp intersectionpointsLineRect(double,double,double,double,double,double,double,double,
                                double *,double *,double *,double *,double *,double *,double *,double *,
                                int *,int *,int *,int *,int);

extern int oswinexp polygonintersection(int,double *,double *,int,double *,double *[],double *,double *);

extern int oswinexp linesintersection(double,double,double,double,double,double,double,double,
                       double *,double *,int prec=ddPrec,int xbox=0,int ybox=0);

class oswinexp Asciistr {

  private:
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

extern int oswinexp ui2a(ulreal,Asciistr*,int base=10,int width=0,int padzero=1,const char *group=0);

extern int oswinexp i2a(lreal,Asciistr*,int base=10,int width=0,int padzero=1,const char *group=0);

extern int oswinexp d2a(double,Asciistr*,int prec=15,char fmt=-1,int pad=-1,const char *dec=".",const char *group=0);

extern ulreal oswinexp nextT(Asciistr *);

class oswinexp Ucsstr {

  private:
    unsigned short *m_str;
    int m_sz;

  public:
    Ucsstr(int l=0,const unsigned short *str=0);
    Ucsstr(const Ucsstr &);
    ~Ucsstr();
    Ucsstr &operator=(const Ucsstr &);
    Ucsstr &operator=(const char *);
    operator const unsigned short *() const;
    unsigned short operator[](int) const;
    bool operator==(const Ucsstr &) const;
    bool operator<(const Ucsstr &) const;
    const unsigned short *data() const;
    int len() const;
    int reserve(int);
    int set(int,unsigned short);
    int find(unsigned short,int idx=0,unsigned char dir='f') const;
    Ucsstr substr(int,int) const;
    Ucsstr &append(const char *);
    Ucsstr &append(unsigned short);
    Ucsstr &append(const Ucsstr &);
    Ucsstr &prepend(const char *);
    Ucsstr &prepend(unsigned short);
    Ucsstr &prepend(const Ucsstr &);
    Asciistr asciistr() const;

};

class oswinexp TypeId {

  protected:
    ulreal m_idd;
    char m_type[idlen];

  public:
    TypeId(const char *name=0,ulreal idd=0);
    TypeId(const TypeId &);
    virtual ~TypeId();
    TypeId &operator=(const TypeId &);
    bool operator==(const TypeId &cmp) const;
    bool operator<(const TypeId &cmp) const;
    virtual TypeId *clone() const;
    int busted() const;
    const char *type() const;
    ulreal idd() const;
    virtual void toString(Asciistr *) const;
    virtual int fromString(const char *);

};

extern int oswinexp cmpType(const void *,const void *);
extern int oswinexp cmpIdd(const void *,const void *);

class oswinexp Rounded {

  public:
    double m_d;
    int m_a;
    Rounded(double d=dnan,int a=sLimit) : m_d(d),m_a(a) {
    }
    Rounded(const Rounded &ass) : m_d(ass.m_d),m_a(ass.m_a) {
    }
    ~Rounded() {
    }
    Rounded &operator=(const Rounded &);
    bool operator==(const Rounded &) const;
    bool operator<(const Rounded &) const;
    int cmprd(const Rounded &) const;
    double rounded() const;
    Asciistr toString(char fmt=-1,int pad=-1,const char *dec=".",const char *group=0) const;

};

template <class X> inline void bubblesort(int n, X arr[]) {
  if (n<2 || !arr)
    return;
  if (n>20) {
    heapsortv(n,arr,0);
    return;
  }
  int ii=0,jj=0;
  X tmp;
  for (ii=1;ii<(int)n;ii++) {
    tmp=arr[ii];
    jj=ii-1;
    while (jj>-1 && arr[jj]>tmp) {
      arr[jj+1]=arr[jj];
      jj--;
    }
    arr[jj+1]=tmp;
  }
}

template <class X> inline void heapsortv(int nn, X arr[]) {
  if (nn<2 || !arr)
    return;
  int ii=0,jj=0,ub=nn-1,mb=nn/2;
  X tmp;
  while (mb>-1) {
    if (mb==0) { // heap creation phase has ended - start reordering subknots
      tmp=arr[ub]; // make auxillary from last subknot to fill in
      arr[ub]=arr[0];
      if (--ub==0) { // all done - put smallest into start position
        arr[0]=tmp;
        break;
      }
    }
    else
      tmp=arr[--mb]; // start at last knot which has subknots and create bintree (knot>subknots)
    ii=mb;jj=ii+ii+1; // j is subknot index
    while(jj<=ub) {
      if (jj<ub && arr[jj]<arr[jj+1])
        jj++; // subknot is ok to its successor
      if (tmp<arr[jj]) {  // promote j-pos until it is better than auxillary ...
        arr[ii]=arr[jj];
        ii=jj;
        jj+=(ii+1); // ... and make i=j as new knot with subknots from 2*j
      }
      else
        break; //
    }
    arr[ii]=tmp; // put the auxillary (which was small) into act subknot position
  }
}

template <class X> inline void heapsortvc(int nn, X arr[], int (*comp)(const void *itm1,const void *itm2)) {
  if (nn<2 || !arr || !comp)
    return;
  int ii=0,jj=0,ub=nn-1,mb=nn/2;
  X tmp;
  while (mb>-1) {
    if (mb==0) { // heap creation phase has ended - start reordering subknots
      tmp=arr[ub]; // make auxillary from last subknot to fill in
      arr[ub]=arr[0];
      if (--ub==0) { // all done - put smallest into start position
        arr[0]=tmp;
        break;
      }
    }
    else
      tmp=arr[--mb]; // start at last knot which has subknots and create bintree (knot>subknots)
    ii=mb;jj=ii+ii+1; // j is subknot index
    while(jj<=ub) {
      if (jj<ub) { // subknot is ok to its successor
        if (comp && comp((const void*)(&arr[jj]),(const void*)(&arr[jj+1]))<0)
          jj++;
      }
      if (comp && comp((const void*)(&tmp),(const void*)(&arr[jj]))<0) {  // promote j-pos until it is better than auxillary ...
        arr[ii]=arr[jj];
        ii=jj;
        jj+=(ii+1); // ... and make i=j as new knot with subknots from 2*j
      }
      else
        break; //
    }
    arr[ii]=tmp; // put the auxillary (which was small) into act subknot position
  }
}

template <class X> inline void heapsortp(int nn, X **arr) {
  if (nn<2 || !arr)
    return;
  int ii=0,jj=0,ub=nn-1,mb=nn/2;
  X *tmp=0;
  while (mb>-1) {
    if (mb==0) { // heap creation phase has ended - start reordering subknots
      tmp=arr[ub]; // make auxillary from last subknot to fill in
      arr[ub]=arr[0];
      if (--ub==0) { // all done - put smallest into start position
        arr[0]=tmp;
        break;
      }
    }
    else
      tmp=arr[--mb]; // start at last knot which has subknots and create bintree (knot>subknots)
    ii=mb;jj=ii+ii+1; // j is subknot index
    while(jj<=ub) {
      if (jj<ub && *arr[jj]<*arr[jj+1])
        jj++; // subknot is ok to its successor
      if (*tmp<*arr[jj]) {// promote j-pos until it is better than auxillary ...
        arr[ii]=arr[jj];
        ii=jj;
        jj+=(ii+1); // ... and make i=j as new knot with subknots from 2*j
      }
      else
        break; //
    }
    arr[ii]=tmp; // put the auxillary (which was small) into act subknot position
  }
}

template <class X> inline void heapsortpc(int nn, X **arr, int (*comp)(const void *itm1,const void *itm2)) {
  if (nn<2 || !arr || !comp)
    return;
  int ii=0,jj=0,ub=nn-1,mb=nn/2;
  X *tmp=0;
  while (mb>-1) {
    if (mb==0) { // heap creation phase has ended - start reordering subknots
      tmp=arr[ub]; // make auxillary from last subknot to fill in
      arr[ub]=arr[0];
      if (--ub==0) { // all done - put smallest into start position
        arr[0]=tmp;
        break;
      }
    }
    else
      tmp=arr[--mb]; // start at last knot which has subknots and create bintree (knot>subknots)
    ii=mb;jj=ii+ii+1; // j is subknot index
    while(jj<=ub) {
      if (jj<ub && comp((const void *)arr[jj],(const void *)arr[jj+1])<0)
        jj++; // subknot is ok to its successor
      if (comp((const void *)tmp,(const void *)arr[jj])<0) { // promote j-pos until it is better than auxillary ...
        arr[ii]=arr[jj];
        ii=jj;
        jj+=(ii+1); // ... and make i=j as new knot with subknots from 2*j
      }
      else
        break; //
    }
    arr[ii]=tmp; // put the auxillary (which was small) into act subknot position
  }
}

template < class X > inline void swap (X * obj1, X * obj2) {
  if (!obj1 || !obj2)
    return;
  X tmp=*obj1;
  *obj1=*obj2;
  *obj2=tmp;
}

template <class X> inline int binsearchv(const X *x,int cnt,const X arr[],int guess=-1) {
  int lb=-1,mb=0,ub=cnt,cmp=0,inc=1;
  const X *tmp=0;
  if (guess<=0 || guess>=cnt-1) {
    while ((ub-lb>1)) {
      mb=(ub+lb)/2;
      tmp=&arr[mb];
      if (*tmp==*x)
        return mb;
      if (*tmp<*x)
        lb=mb;
      else
        ub=mb;
    }
    return -1;
  }
  mb=guess;
  tmp=&arr[mb];
  if (*tmp==*x)
    return mb;
  if (*tmp<*x) {
    mb+=inc;
    while (mb<ub) {
      inc<<=1;
      tmp=&arr[mb];
      if (*tmp==*x)
        return mb;
      if (*tmp<*x) {
        lb=mb;
        mb+=inc;
      }
      else {
        ub=mb;
        break;
      }
    }
  }
  else {
    mb-=inc;
    while (mb>lb) {
      tmp=&arr[mb];
      if (*tmp==*x)
        return mb;
      if (*x<*tmp) {
        ub=mb;
        mb-=inc;
      }
      else {
        lb=mb;
        break;
      }
      inc<<=1;
    }
  }
  while ((ub-lb>1)) {
    mb=(ub+lb)/2;
    tmp=&arr[mb];
    if (*tmp==*x)
      return mb;
    if (*x<*tmp)
      lb=mb;
    else
      ub=mb;
  }
  return -1;
}

template <class X> inline int binsearchvc(const X *x,int cnt,const X arr[],int (*comp)(const void *x1,const void *x2),int guess=-1) {
  if (!comp)
    return -1;
  int lb=-1,mb=0,ub=cnt,cmp=0,inc=1;
  const X *tmp=0;
  if (guess<=0 || guess>=cnt-1) {
    while ((ub-lb>1)) {
      mb=(ub+lb)/2;
      tmp=&arr[mb];
      cmp=comp((const void*)tmp,(const void*)x);
      if (cmp==0)
        return mb;
      if (cmp<0)
        lb=mb;
      else
        ub=mb;
    }
    return -1;
  }
  mb=guess;
  tmp=&arr[mb];
  cmp=comp((const void*)tmp,(const void*)x);
  if (cmp==0)
    return mb;
  if (cmp<0) {
    mb+=inc;
    while (mb<ub) {
      inc<<=1;
      tmp=&arr[mb];
      cmp=comp((const void*)tmp,(const void*)x);
      if (cmp==0)
        return mb;
      if (cmp<0) {
        lb=mb;
        mb+=inc;
      }
      else {
        ub=mb;
        break;
      }
    }
  }
  else {
    mb-=inc;
    while (mb>lb) {
      tmp=&arr[mb];
      cmp=comp((const void*)tmp,(const void*)x);
      if (cmp==0)
        return mb;
      if (cmp>0) {
        ub=mb;
        mb-=inc;
      }
      else {
        lb=mb;
        break;
      }
      inc<<=1;
    }
  }
  while ((ub-lb>1)) {
    mb=(ub+lb)/2;
    tmp=&arr[mb];
    cmp=comp((const void*)tmp,(const void*)x);
    if (cmp==0)
      return mb;
    if (cmp<0)
      lb=mb;
    else
      ub=mb;
  }
  return -1;
}

template <class X> inline int binsearchp(const X *x,int cnt,const X **arr,int guess=-1) {
  int lb=-1,mb=0,ub=cnt,cmp=0,inc=1;
  const X *tmp=0;
  if (guess<=0 || guess>=cnt-1) {
    while ((ub-lb>1)) {
      mb=(ub+lb)/2;
      tmp=arr[mb];
      if (*tmp==*x)
        return mb;
      if (*tmp<*x)
        lb=mb;
      else
        ub=mb;
    }
    return -1;
  }
  mb=guess;
  tmp=arr[mb];
  if (*tmp==*x)
    return mb;
  if (cmp<0) {
    mb+=inc;
    while (mb<ub) {
      inc<<=1;
      tmp=arr[mb];
      if (*tmp==*x)
        return mb;
      if (*tmp<*x) {
        lb=mb;
        mb+=inc;
      }
      else {
        ub=mb;
        break;
      }
    }
  }
  else {
    mb-=inc;
    while (mb>lb) {
      tmp=arr[mb];
      if (*tmp==*x)
        return mb;
      if (*x<*tmp) {
        ub=mb;
        mb-=inc;
      }
      else {
        lb=mb;
        break;
      }
      inc<<=1;
    }
  }
  while ((ub-lb>1)) {
    mb=(ub+lb)/2;
    tmp=arr[mb];
    if (*tmp==*x)
      return mb;
    if (*tmp<*x)
      lb=mb;
    else
      ub=mb;
  }
  return -1;
}

template <class X> inline int binsearchpc(const X *x,int cnt,const X **arr,int (*comp)(const void *x1,const void *x2),int guess=-1) {
  if (!comp)
    return -1;
  int lb=-1,mb=0,ub=cnt,cmp=0,inc=1;
  const X *tmp=0;
  if (guess<=0 || guess>=cnt-1) {
    while ((ub-lb>1)) {
      mb=(ub+lb)/2;
      tmp=arr[mb];
      cmp=comp((const void*)tmp,(const void*)x);
      if (cmp==0)
        return mb;
      if (cmp<0)
        lb=mb;
      else
        ub=mb;
    }
    return -1;
  }
  mb=guess;
  tmp=arr[mb];
  cmp=comp((const void*)tmp,(const void*)x);
  if (cmp==0)
    return mb;
  if (cmp<0) {
    mb+=inc;
    while (mb<ub) {
      inc<<=1;
      tmp=arr[mb];
      cmp=comp((const void*)tmp,(const void*)x);
      if (cmp==0)
        return mb;
      if (cmp<0) {
        lb=mb;
        mb+=inc;
      }
      else {
        ub=mb;
        break;
      }
    }
  }
  else {
    mb-=inc;
    while (mb>lb) {
      tmp=arr[mb];
      cmp=comp((const void*)tmp,(const void*)x);
      if (cmp==0)
        return mb;
      if (cmp>0) {
        ub=mb;
        mb-=inc;
      }
      else {
        lb=mb;
        break;
      }
      inc<<=1;
    }
  }
  while ((ub-lb>1)) {
    mb=(ub+lb)/2;
    tmp=arr[mb];
    cmp=comp((const void*)tmp,(const void*)x);
    if (cmp==0)
      return mb;
    if (cmp<0)
      lb=mb;
    else
      ub=mb;
  }
  return -1;
}

template <class X> inline int binsearchInterval(const X *x,int cnt,const X arr[],int *idxl,int *idxh,int sortedrev=0) {

  int ii=0,ih=cnt,il=-1;
  while ((ih-il)>1) {
    ii=(ih+il)/2;
    if (sortedrev>0 ? *x>arr[ii] : arr[ii]>*x)
      ih=ii;
    else
      il=ii;
  }
  int ret=1;
  if (ih>=cnt) {
    ih=(cnt==0 ? 0 : cnt-1);
    il=(cnt==0 ? 0 : (cnt>1 ? cnt-2 : cnt-1));
    if (cnt==0 || *x>arr[cnt-1])
      ret=0;
  }
  if (il<0) {
    il=0;
    ih=(cnt>1 ? 1 : 0);
    if (cnt==0 || *x<arr[0])
      ret=0;
  }
  if (idxl)
    *idxl=il;
  if (idxh)
    *idxh=ih;
  return ret;

}

template <class X> inline int binsearchInterval(const X *x,int cnt,const X arr[],int (*comp)(const void *x1,const void *x2),int *idxl,int *idxh,int sortedrev=0) {

  int ii=0,ih=cnt,il=-1,cmp=0;
  X *tmp=0;
  while ((ih-il)>1) {
    ii=(ih+il)/2;
    tmp=&arr[ii];
    cmp=comp((const void*)tmp,(const void*)x);
    if (sortedrev>0 ? cmp<0 : cmp>0)
      ih=ii;
    else
      il=ii;
  }
  int ret=1;
  if (ih>=cnt) {
    ih=(cnt==0 ? 0 : cnt-1);
    il=(cnt==0 ? 0 : (cnt>1 ? cnt-2 : cnt-1));
    if (cnt==0 || *x>arr[cnt-1])
      ret=0;
  }
  if (il<0) {
    il=0;
    ih=(cnt>1 ? 1 : 0);
    if (cnt==0 || *x<arr[0])
      ret=0;
  }
  if (idxl)
    *idxl=il;
  if (idxh)
    *idxh=ih;
  return ret;

}

// *******************************

template <class X> class TPArr {

  protected:
    int m_sz;
    int m_last;
    X **m_arr;

  public:
    int m_autoGrow;
    TPArr(int sz=0) : m_sz(sz<0 ? 0 : (sz>=(int)ipow2(maxArrSz) ? (int)ipow2(maxArrSz)-1 : sz)),
      m_last(-1),m_arr(0),m_autoGrow(1) {
      if (m_sz>0) {
        m_arr=new X*[(size_t)m_sz];
        memset(&m_arr[0],0,m_sz*sizeof(X*));
      }
    }
    ~TPArr() {
      resize(0);
    }
    X *operator[](int idx) {
      return (idx<0 || idx>=m_sz ? 0 : m_arr[idx]);
    }
    const X *operator[](int idx) const {
      return (idx<0 || idx>=m_sz ? 0 : (const X*)(m_arr[idx]));
    }
    X *at(int idx) {
      return (idx<0 || idx>=m_sz ? 0 : m_arr[idx]);
    }
    X **rawData() {
      return m_arr;
    }
    int last() const {
      return m_last;
    }
    int size() const {
      return m_sz;
    }
    int resize(int sz,int destr=0) {
      if (sz<0)
        sz=0;
      else if (sz>=ipow2(maxArrSz))
        sz=(int)ipow2(maxArrSz)-1;
      if (sz==m_sz)
        return m_sz;
      int ii=0;
      if (sz<m_sz) {
        for (ii=sz;ii<m_sz;ii++) {
          X *x=m_arr[ii];
          if (x && destr>0)
            delete x;
          m_arr[ii]=0;
        }
        if (m_last>=sz)
          m_last=sz-1;
        m_sz=sz;
      }
      X **arr=(m_sz>0 ? new X*[(size_t)m_sz] : 0);
      for (ii=0;ii<m_sz;ii++)
        arr[ii]=m_arr[ii];
      if (m_arr)
        delete [] m_arr;
      m_arr=(sz>0 ? new X*[(size_t)sz] : 0);
      if (sz>0)
        memset(&m_arr[0],0,sz*sizeof(X*));
      if (m_sz>0) {
        for (ii=0;ii<m_sz;ii++)
          m_arr[ii]=arr[ii];
        delete [] arr;
      }
      m_sz=sz;
      return m_sz;
    }
    void clear(int destr=0) {
      int ii=0;
      X *x=0;
      for (ii=0;ii<=m_last;ii++) {
        x=m_arr[ii];
        if (x && destr)
          delete x;
        m_arr[ii]=0;
      }
      m_last=-1;
    }
    void sort(int (*cmp)(const void *,const void *)) {
      heapsortpc(m_last+1,m_arr,cmp);
    }
    X *set(int idx,X *x) {
      if (idx>=m_sz && m_autoGrow>0)
        resize(idx+1);
      if (idx<0 || idx>=m_sz)
        return 0;
      X *xx=m_arr[idx];
      m_arr[idx]=x;
      if (idx>m_last)
        m_last=idx;
      return xx;
    }

  protected:
    TPArr(const TPArr &)  : m_sz(0),m_last(-1),m_arr(0),m_autoGrow(1) {
    }
    TPArr &operator=(const TPArr &) {
      return *this;
    }

};

// *******************************

inline int cmpint(const void *a1,const void *a2)  {

  if (!a1 || !a2)
    return 0;
  if ( *((int*)a1) > *((int*)a2) )
    return 1;
  if ( *((int*)a1) < *((int*)a2) )
    return -1;
  return 0;

}

template <class X> class TVArr {

  protected:
    int m_sz;
    int m_last;
    X *m_arr;
    X dum;

  public:
    int m_autoGrow;
    TVArr(int sz=0) : m_sz(sz<0 ? 0 : (sz>=(int)ipow2(maxArrSz) ? (int)ipow2(maxArrSz)-1 : sz)),
      m_last(-1),m_arr(0),dum(X()),m_autoGrow(true) {
      if (m_sz>0) {
        m_arr=new X[(size_t)m_sz];
        int ii=0;
        for (ii=0;ii<m_sz;ii++)
          m_arr[ii]=dum;
      }
    }
    TVArr(const TVArr &ass) : m_sz(ass.m_sz),m_last(ass.m_last),m_arr(0),dum(X()),m_autoGrow(ass.m_autoGrow) {
      if (m_sz>0) {
        m_arr=new X[(size_t)m_sz];
        memcpy(&m_arr[0],&ass.m_arr[0],m_sz*sizeof(X));
      }
    }
    ~TVArr() {
      resize(0);
    }
    TVArr &operator=(const TVArr &ass) {
      if (m_sz!=ass.m_sz) {
        if (m_arr)
          delete [] m_arr;
        m_sz=ass.m_sz;
        m_arr=(m_sz>0 ? new X[(size_t)m_sz] : 0);

      }
      m_last=ass.m_last;
      m_autoGrow=ass.m_autoGrow;
      if (m_sz>0)
        memcpy(&m_arr[0],&ass.m_arr[0],m_sz*sizeof(X));
      return *this;
    }
    X operator[](int idx) {
      return (idx<0 || idx>=m_sz ? dum : m_arr[idx]);
    }
    const X *operator[](int idx) const  {
      return (idx<0 || idx>=m_sz ? dum : m_arr[idx]);
    }
    X *at(int idx) {
      return (idx<0 || idx>=m_sz ? 0 : &m_arr[idx]);
    }
    X *rawData() {
      return &m_arr[0];
    }
    int last() const {
      return m_last;
    }
    int size() const {
      return m_sz;
    }
    int resize(int sz) {
      if (sz<0)
        sz=0;
      else if (sz>=ipow2(maxArrSz))
        sz=(int)ipow2(maxArrSz)-1;
      if (sz==m_sz)
        return m_sz;
      X *arr=0;
      int cpsz=(sz<m_sz ? sz : m_sz);
      m_sz=sz;
      if (cpsz>0) {
        arr=new X[(size_t)cpsz];
        memcpy(&arr[0],&m_arr[0],cpsz*sizeof(X));
      }
      if (m_arr)
        delete [] m_arr;
      m_arr=(m_sz>0 ? new X[(size_t)m_sz] : 0);
      if (cpsz>0) {
        memcpy(&m_arr[0],&arr[0],cpsz*sizeof(X));
        delete [] arr;
      }
      for (int ii=cpsz;ii<m_sz;ii++)
        m_arr[ii]=dum;
      if (m_last>=m_sz)
        m_last=m_sz-1;
      return m_sz;
    }
    void clear() {
      for (int ii=0;ii<m_sz;ii++)
        m_arr[ii]=dum;
      m_last=-1;
    }
    void sort(int (*cmp)(const void *,const void *)) {
      (cmp ? heapsortvc(m_last+1,m_arr,cmp) : heapsortv(m_last+1,m_arr));
    }
    X set(int idx,const X &x) {
      if (idx>=m_sz && m_autoGrow>0)
        resize(idx+1);
      if (idx<0 || idx>=m_sz)
        return dum;
      X xx=m_arr[idx];
      m_arr[idx]=x;
      if (idx>m_last)
        m_last=idx;
      return xx;
    }

};

// *******************************

template <class X> class TPList {

  protected:
    int m_sz;
    int m_cnt;
    int m_idx;
    int m_sorted;
    X **m_list;

  public:
    int m_autoGrow;
    int (*cmp)(const void *,const void *);
    TPList(int sz=0) : m_sz(0),m_cnt(0),m_idx(-1),m_sorted(0),m_list(0),m_autoGrow(1),cmp(0) {
      int ii=0;
      while (ii<maxArrSz && ipow2(ii)<=sz)
        ii++;
      m_sz=(int)ipow2(ii)-1;
      if (m_sz>0) {
        m_list=new X*[(size_t)m_sz];
        memset(&m_list[0],0,m_sz*sizeof(X*));
      }
    }
    ~TPList() {
      resize(0);
    }
    bool operator==(const TPList &cmp) const {
      return false;
    }
    bool operator<(const TPList &cmp) const {
      return (m_sz<cmp.m_sz);
    }
    X *operator[](int idx) {
      return (idx<0 || idx>=m_cnt ? 0 : m_list[idx]);
    }
    const X *operator[](int idx) const {
      return (idx<0 || idx>=m_cnt ? 0 : (const X*)(m_list[idx]));
    }
    X *at(int idx) {
      return (idx<0 || idx>=m_cnt ? 0 : m_list[idx]);
    }
    X **rawData() {
      return m_list;
    }
    int count() const {
      return m_cnt;
    }
    int size() const {
      return m_sz;
    }
    int sorted() const {
      return m_sorted;
    }
    int setSorted(int sorted) {
      m_sorted=(sorted<=0 ? 0 : sorted);
      return m_sorted;
    }
    void sort(int doit=0) {
      if (m_sorted>0 && doit==0)
        return;
      if (m_cnt>1)
        (cmp ? heapsortpc(m_cnt,m_list,cmp) : heapsortp(m_cnt,m_list));
      m_sorted=1;
    }
    int resize(int sz,int destr=0) {
      int ii=0;
      while (ii<maxArrSz && ipow2(ii)<=sz)
        ii++;
      sz=(int)ipow2(ii)-1;
      if (sz==m_sz)
        return m_sz;
      m_sz=sz;
      if (m_sz<m_cnt) {
        for (ii=m_sz;ii<m_cnt;ii++) {
          if (destr>0)
            delete m_list[ii];
          m_list[ii]=0;
        }
        m_cnt=m_sz;
      }
      X **list=0;
      if (m_cnt>0) {
        list=new X*[(size_t)m_cnt];
        memcpy(&list[0],&m_list[0],m_cnt*sizeof(X*));
      }
      if (m_list)
        delete [] m_list;
      m_list=(m_sz>0 ? new X*[(size_t)m_sz] : 0);
      if (m_list)
        memset(&m_list[0],0,m_sz*sizeof(X*));
      if (list) {
        memcpy(&m_list[0],&list[0],m_cnt*sizeof(X*));
        delete [] list;
      }
      m_idx=-1;
      if (m_cnt==0)
        m_sorted=0;
      return m_sz;
    }
    void clear(int destr=0) {
      if (destr>0) {
        int ii=0;
        for (ii=0;ii<m_cnt;ii++)
          delete m_list[ii];
      }
      memset(&m_list[0],0,m_cnt*sizeof(X*));
      m_sorted=0;
      m_idx=-1;
      m_cnt=0;
    }
    int append(X *x) {
      if (!x)
        return -1;
      if (m_cnt>=m_sz) {
        if (m_autoGrow>0)
          resize(m_sz==0 ? 1 : 2*m_sz);
        if (m_cnt>=m_sz)
          return -1;
      }
      if (m_sorted>0 && m_cnt>0) {
        if ((cmp && cmp((const void *)x,(const void *)m_list[m_cnt-1])<0) ||
            (!cmp && (*x)<(*(m_list[m_cnt-1]))))
          m_sorted=0;
      }
      m_list[m_cnt]=x;
      m_cnt++;
      return m_cnt-1;
    }
    int prepend(X *x) {
      if (!x)
        return -1;
      if (m_cnt>=m_sz) {
        if (m_autoGrow>0)
          resize(m_sz==0 ? 1 : 2*m_sz);
        if (m_cnt>=m_sz)
          return -1;
      }
      if (m_cnt==0)
        return append(x);
      if (m_sorted>0) {
        if ((cmp && cmp((const void *)x,(const void *)m_list[0])>0) ||
            (!cmp && (*(m_list[0]))<(*x)))
          m_sorted=0;
      }
      if (m_cnt>1)
        memmove(&m_list[1],&m_list[0],m_cnt*sizeof(X*));
      else
        m_list[1]=m_list[0];
      m_list[0]=x;
      m_cnt++;
      return 0;
    }
    X *remove(int idx) {
      if (idx<0 || idx>=m_cnt)
        return 0;
      X *x=m_list[idx];
      if (idx<m_cnt-1)
        memmove(&m_list[idx],&m_list[idx+1],(m_cnt-idx-1)*sizeof(X*));
      m_list[m_cnt-1]=0;
      m_cnt--;
      if (m_cnt==0)
        m_idx=-1;
      else if (m_idx>=m_cnt)
        m_idx=m_cnt;
      return x;
    }
    int remove(int startidx,int endidx,TPList<X> *rmL) {
      if (startidx<0 || startidx>=m_cnt || endidx<startidx)
        return 0;
      int ii=0,backidx=endidx+1;
      if (backidx>=m_cnt)
        backidx=m_cnt;
      if (rmL->size()<backidx-startidx)
        rmL->resize(backidx-startidx);
      for (ii=startidx;ii<backidx;ii++)
        rmL->append(m_list[ii]);
      if (backidx<m_cnt)
        memmove(&m_list[startidx],&m_list[backidx],(m_cnt-backidx)*sizeof(X*));
      for (ii=startidx+m_cnt-backidx;ii<m_cnt;ii++)
        m_list[ii]=0;
      m_cnt-=(backidx-startidx);
      if (m_cnt==0)
        m_idx=-1;
      else if (m_idx>=m_cnt)
        m_idx=m_cnt;
      return (backidx-startidx);
    }
    X *remove(X *x) {
      return remove(findRef(x));
    }
    int inSort(X *x,int unique=0) {
      if (!x)
        return -1;
      if (m_cnt>=m_sz) {
        if (m_autoGrow>0)
          resize(m_sz==0 ? 1 : 2*m_sz);
        if (m_cnt>=m_sz)
          return -1;
      }
      if (m_cnt==0) {
        if (append(x)>=0) {
          m_sorted=1;
          return 0;
        }
        return -1;
      }
      if (m_sorted==0) {
        if (m_cnt>1)
          (cmp ? heapsortpc(m_cnt,m_list,cmp) : heapsortp(m_cnt,m_list));
        m_sorted=1;
      }
      int avail=0,idx=findNextIndex(x,&avail);
      if (avail && unique>0)
        return -1;
      if (idx<=0) {
        if (prepend(x)<0)
          return -1;
        return 0;
      }
      else if (idx>=m_cnt) {
        if (append(x)<0)
          return -1;
        return m_cnt-1;
      }
      memmove(&m_list[idx+1],&m_list[idx],(m_cnt-idx)*sizeof(X*));
      m_list[idx]=x;
      m_cnt++;
      return idx;
    }
    int insert(int idx,X *x) {
      if (!x)
        return -1;
      if (idx<0)
        return prepend(x);
      if (idx>=m_cnt)
        return append(x);
      if (m_cnt>=m_sz) {
        if (m_autoGrow>0)
          resize(m_sz==0 ? 1 : 2*m_sz);
        if (m_cnt>=m_sz)
          return -1;
      }
      if (m_sorted>0) {
        if (idx<m_cnt-1) {
          if ((!cmp && (*(m_list[idx+1]))<(*x)) ||
              (cmp && cmp((const void *)x,(const void *)m_list[idx+1])>0))
            m_sorted=0;
        }
        if (idx>0) {
          if ((!cmp && (*x)<(*(m_list[idx-1]))) ||
              (cmp && cmp((const void *)x,(const void *)m_list[idx-1])<0))
            m_sorted=0;
        }
      }
      memmove(&m_list[idx+1],&m_list[idx],(m_cnt-idx)*sizeof(X));
      m_list[idx]=x;
      m_cnt++;
      return idx;
    }
    X *replace(int idx,X *x,int *ok=0) {
      if (ok)
        *ok=0;
      if (!x || idx<0 || idx>=m_cnt)
        return 0;
      if (ok)
        *ok=1;
      if (m_sorted>0) {
        if (idx<m_cnt-1) {
          if ((!cmp && (*(m_list[idx+1]))<(*x)) ||
              (cmp && cmp((const void *)x,(const void *)m_list[idx+1])>0))
            m_sorted=0;
        }
        if (idx>0) {
          if ((!cmp && (*x)<(*(m_list[idx-1]))) ||
              (cmp && cmp((const void *)x,(const void *)m_list[idx-1])<0))
            m_sorted=0;
        }
      }
      X *xin=m_list[idx];
      m_list[idx]=x;
      return xin;
    }
    int find(const X *x) const {
      if (!x)
        return -1;
      if (m_sorted==0) {
        int i=0;
        for (i=0;i<m_cnt;i++) {
          if ((cmp && cmp((const void *)x,(const void *)m_list[i])==0) ||
              (!cmp && (*x)==(*(m_list[i]))))
            return i;
        }
        return -1;
      }
      else
        return (cmp ? binsearchpc(x,m_cnt,(const X**)m_list,cmp) : binsearchp(x,m_cnt,(const X**)m_list));
    }
    int findAll(const X *x,TVArr<int> *idxL) const {
      if (!x)
        return -1;
      int ii=0,cnt=0;
      if (idxL) {
        idxL->clear();
        if (idxL->size()<m_cnt)
          idxL->resize(m_cnt);
      }
      if (m_sorted==0) {
        for (ii=0;ii<m_cnt;ii++) {
          if ((cmp && cmp((const void *)x,(const void *)m_list[ii])==0) ||
              (!cmp && (*x)==(*(m_list[ii])))) {
            if (idxL)
              idxL->set(cnt,ii);
            cnt++;
          }
        }
      }
      else {
        int idx=find(x);
        if (idx<0)
          return cnt;
        if (idxL)
          idxL->set(cnt,idx);
        cnt++;
        for (ii=idx-1;ii>-1;ii--) {
          if ((cmp && cmp((const void *)x,(const void *)m_list[ii])==0) ||
              (!cmp && (*x)==(*(m_list[ii])))) {
            if (idxL)
              idxL->set(cnt,ii);
            cnt++;
          }
          else break;
        }
        for (ii=idx+1;ii<m_cnt;ii++) {
          if ((cmp && cmp((const void *)x,(const void *)m_list[ii])==0) ||
              (!cmp && (*x)==(*(m_list[ii])))) {
            if (idxL)
              idxL->set(cnt,ii);
            cnt++;
          }
          else break;
        }
      }
      if (idxL)
        idxL->sort(cmpint);
      return cnt;
    }
    int findRef(const X *x) const {
      if (!x)
        return -1;
      int ii=0;
      for (ii=0;ii<m_cnt;ii++) {
        if (m_list[ii]==x)
          return ii;
      }
      return -1;
    }
    int findNextIndex(const X *x,int *avail=0) const {
      if (m_sorted==0) {
        if (avail)
          *avail=0;
        return -1;
      }
      int lb=-1,mb=0,mblast=-1,ub=m_cnt,cmpres=0;
      while ((ub-lb>1)) {
        mb=(ub+lb)/2;
        if (mb==mblast)
          break;
        if (cmp)
          cmpres=cmp((const void *)m_list[mb],(const void *)x);
        else
          cmpres=((*(m_list[mb]))<(*x) ? -1 : ((*(m_list[mb]))==(*x) ? 0 : 1));
        if (cmpres==0) {
          if (avail)
            *avail=1;
          if (mb>=(m_cnt-1))
            return m_cnt;
          return (mb+1);
        }
        else if (cmpres<0)
          lb=mb;
        else
          ub=mb;
        mblast=mb;
      }
      if (ub<=0)
        return 0;
      return (ub>=m_cnt ? m_cnt : ub);
    }
    X *first() {
      if (m_cnt==0) {
        m_idx=-1;
        return 0;
      }
      m_idx=0;
      return m_list[m_idx];
    }
    X *next() {
      if (m_idx<m_cnt)
        m_idx++;
      return (m_idx<0 || m_idx>=m_cnt ? 0 : m_list[m_idx]);
    }
    X *last() {
      if (m_cnt==0) {
        m_idx=-1;
        return 0;
      }
      m_idx=m_cnt-1;
      return m_list[m_idx];
    }
    X *prev() {
      if (m_idx>-1)
        m_idx--;
      return (m_idx<0 || m_idx>=m_cnt ? 0 : m_list[m_idx]);
    }
    X *current() {
      return (m_idx<0 || m_idx>=m_cnt ? 0 : m_list[m_idx]);
    }
    int currentIdx() const {
      return m_idx;
    }

  protected:
    TPList(const TPList &) : m_sz(0),m_cnt(0),m_idx(-1),m_sorted(0),m_list(0),m_autoGrow(1),cmp(0) {
    }
    TPList &operator=(const TPList &) {
      return *this;
    }

};

template <class X> class TVList {

  protected:
    TPList<X> m_list;
    X dum;

  public:
    TVList(int sz=0) : m_list(sz),dum(X()) {
    }
    TVList(const TVList &ass) : m_list(0),dum(X()) {
      m_list.resize(ass.m_list.size());
      int ii=0;
      for (ii=0;ii<ass.m_list.count();ii++)
        m_list.append(new X(*ass.m_list[ii]));
      m_list.setSorted(ass.m_list.sorted());
      m_list.m_autoGrow=ass.m_list.m_autoGrow;
      m_list.cmp=ass.m_list.cmp;
    }
    ~TVList() {
      resize(0);
    }
    TVList &operator=(const TVList &ass) {
      m_list.clear(1);
      m_list.resize(ass.m_list.size());
      int ii=0;
      for (ii=0;ii<ass.m_list.count();ii++)
        m_list.append(new X(*ass.m_list[ii]));
      m_list.setSorted(ass.m_list.sorted());
      m_list.m_autoGrow=ass.m_list.m_autoGrow;
      m_list.cmp=ass.m_list.cmp;
      return *this;
    }
    bool operator==(const TVList &) const {
      return false;
    }
    bool operator<(const TVList &cmp) const {
      return (m_list.size()<cmp.m_list.size());
    }
    X operator[](int idx) {
      return (m_list[idx] ? *m_list[idx] : dum);
    }
    const X operator[](int idx) const {
      return (m_list[idx] ? *m_list[idx] : dum);
    }
    X *at(int idx) {
      return m_list.at(idx);
    }
    const X *at(int idx) const {
      return m_list.operator[](idx);
    }
    X **rawData() {
      return m_list.rawData();
    }
    int count() const {
      return m_list.count();
    }
    int size() const {
      return m_list.size();
    }
    int resize(int sz) {
      return m_list.resize(sz,true);
    }
    void setAutoGrow(int autoGrow) {
      m_list.m_autoGrow=autoGrow;
    }
    void setCmp(int (*cmp)(const void *,const void *)) {
      m_list.cmp=cmp;
    }
    int (*getCmp())(const void *,const void *) {
      return m_list.cmp;
    }
    int sorted() const {
      return m_list.sorted();
    }
    int setSorted(int sorted) {
      return m_list.setSorted(sorted);
    }
    void sort(int doit=false) {
      m_list.sort(doit);
    }
    void clear() {
      m_list.clear(1);
    }
    int append(const X &x) {
      X *xx=new X(x);
      int idx=m_list.append(xx);
      if (idx<0)
        delete xx;
      return idx;
    }
    int prepend(const X &x) {
      X *xx=new X(x);
      int idx=m_list.prepend(xx);
      if (idx<0)
        delete xx;
      return idx;
    }
    int remove(int idx) {
      X *x=m_list.remove(idx);
      if (x)
        delete x;
      return (x ? idx : -1);
    }
    int remove(int startidx,int endidx) {
      TPList<X> rmL;
      int ret=m_list.remove(startidx,endidx,&rmL);
      rmL.clear(true);
      return ret;
    }
    int inSort(const X &x,int unique=0) {
      X *xx=new X(x);
      int res=m_list.inSort(xx,unique);
      if (res<0)
        delete xx;
      return res;
    }
    int insert(int idx,const X &x) {
      X *xx=new X(x);
      int res=m_list.insert(idx,xx);
      if (res<0)
        delete xx;
      return res;
    }
    X replace(int idx,const X &x) {
      X *xx=new X(x);
      int ok=0;
      X *repl=m_list.replace(idx,xx,&ok);
      X xxx;
      if (ok==0)
        delete xx;
      else if (repl) {
        xxx=*repl;
        delete repl;
      }
      return xxx;
    }
    int find(const X &x) const {
      return m_list.find(&x);
    }
    int findAll(const X &x,TVArr<int> *idxL) const {
      return m_list.findAll(&x,idxL);
    }
    int findNextIndex(const X &x,int *avail=0) const {
      return m_list.findNextIndex(&x,avail);
    }

};

template <class X> class TVStack {

  protected:
    TVList<X> m_list;
    X dum;

  public:
    TVStack(int sz=0) : m_list(sz),dum(X()) {
    }
    TVStack(const TVStack &ass) : m_list(ass.m_list),dum(X()) {
    }
    ~TVStack() {
    }
    TVStack &operator=(const TVStack &ass) {
      if (this==&ass)
        return *this;
      m_list=ass.m_list;
      return *this;
    }
    bool operator==(const TVStack &cmp) const {
      return false;
    }
    bool operator<(const TVStack &cmp) const {
      return (m_list.size()<cmp.m_list.size());
    }
    X operator[](int idx) {
      return m_list[idx];
    }
    const X operator[](int idx) const {
      return m_list[idx];
    }
    X top() {
      return m_list[m_list.count()-1];
    }
    const X top() const {
      return m_list[m_list.count()-1];
    }
    TVList<X> *rawData() {
      return &m_list;
    }
    int count() const {
      return m_list.count();
    }
    int size() const {
      return m_list.size();
    }
    int resize(int sz) {
      return m_list.resize(sz);
    }
    void setAutoGrow(int autoGrow) {
      m_list.setAutoGrow(autoGrow);
    }
    void clear() {
      m_list.clear();
    }
    int push(const X &x) {
      return m_list.append(x);
    }
    X pop() {
      X x=m_list[m_list.count()-1];
      m_list.remove(m_list.count()-1);
      return x;
    }

};

// *********************************

template <class X> class TPArr2d {

  protected:
    int m_rows;
    int m_cols;
    X ***m_arr;

  public:
    int m_autoGrow;
    TPArr2d(int rows=0,int cols=0) : m_rows(0),m_cols(0),m_arr(0),m_autoGrow(1) {
      int ii=0;
      if (rows>0 && cols>0) {
        m_rows=(rows>=(int)ipow2(maxArrSz) ? (int)ipow2(maxArrSz)-1 : rows);
        m_cols=(cols>=(int)ipow2(maxArrSz) ? (int)ipow2(maxArrSz)-1 : cols);
        m_arr=new X**[(size_t)m_rows];
        for (ii=0;ii<m_rows;ii++) {
          m_arr[ii]=new X*[(size_t)m_cols];
          memset(&m_arr[ii][0],0,m_cols*sizeof(X*));
        }
      }
    }
    ~TPArr2d() {
      resize(0,0);
    }
    int rows() const {
      return m_rows;
    }
    int cols() const {
      return m_cols;
    }
    X *at(int row,int col) {
      if (row<0 || row>=m_rows || col<0 || col>=m_cols)
        return 0;
      return m_arr[row][col];
    }
    const X *at(int row,int col) const {
      if (row<0 || row>=m_rows || col<0 || col>=m_cols)
        return 0;
      return (const X*)(m_arr[row][col]);
    }
    int row(int r,TPArr<X> *rL) {
      if (r<0 || r>=m_rows || !rL)
        return 0;
      rL->resize(m_cols);
      X **rawL=rL->rawData();
      int ii=0;
      for (ii=0;ii<m_cols;ii++)
        rawL[ii]=m_arr[r][ii];
      rL->set(m_cols-1,m_arr[r][m_cols-1]); // last
      return m_cols;
    }
    int col(int c,TPArr<X> *cL) {
      if (c<0 || c>=m_cols || !cL)
        return 0;
      cL->resize(m_rows);
      X **rawL=cL->rawData();
      int ii=0;
      for (ii=0;ii<m_rows;ii++)
        rawL[ii]=m_arr[ii][c];
      cL->set(m_rows-1,m_arr[m_rows-1][c]); // last
      return m_rows;
    }
    int resize(int r,int c,int destr=0) {
      if (r<=0 || c<=0)
        r=c=0;
      if (r>=ipow2(maxArrSz))
        r=(int)ipow2(maxArrSz)-1;
      if (c>=ipow2(maxArrSz))
        c=(int)ipow2(maxArrSz)-1;
      if (r==m_rows && c==m_cols)
        return m_rows*m_cols;
      int ii=0,jj=0;
      if (destr>0 && m_arr && (r<m_rows || c<m_cols)) {
        if (c<m_cols) {
          for (ii=0;ii<m_rows;ii++) {
            for (jj=c;jj<m_cols;jj++) {
              if (m_arr[ii][jj])
                delete m_arr[ii][jj];
              m_arr[ii][jj]=0;
            }
          }
        }
        if (r<m_rows) {
          for (ii=0;ii<m_cols;ii++) {
            for (jj=r;jj<m_rows;jj++) {
              if (m_arr[jj][ii])
                delete m_arr[jj][ii];
              m_arr[jj][ii]=0;
            }
          }
        }
      }
      if (r==0 || c==0) {
        if (m_arr) {
          for (ii=0;ii<m_rows;ii++)
            delete [] m_arr[ii];
          delete [] m_arr;
          m_arr=0;
        }
        m_rows=m_cols=0;
        return 0;
      }
      int saverows=(r<m_rows ? r : m_rows),savecols=(c<m_cols ? c : m_cols);
      X ***arr=0;
      if (m_arr) {
        if (saverows>0 && savecols>0)
          arr=new X**[(size_t)saverows];
        for (ii=0;ii<saverows;ii++) {
          arr[ii]=new X*[(size_t)savecols];
          for (jj=0;jj<savecols;jj++)
            arr[ii][jj]=m_arr[ii][jj];
        }
        for (ii=0;ii<m_rows;ii++)
          delete [] m_arr[ii];
        delete [] m_arr;
      }
      m_rows=r;
      m_cols=c;
      m_arr=new X**[(size_t)m_rows];
      for (ii=0;ii<m_rows;ii++) {
        m_arr[ii]=new X*[(size_t)m_cols];
        memset(&m_arr[ii][0],0,m_cols*sizeof(X*));
      }
      if (arr) {
        for (ii=0;ii<saverows;ii++) {
          for (jj=0;jj<savecols;jj++)
            m_arr[ii][jj]=arr[ii][jj];
          delete [] arr[ii];
        }
        delete [] arr;
      }
      return m_rows*m_cols;
    }
    void clear(int destr=0) {
      int ii=0,jj=0;
      for (ii=0;ii<m_rows;ii++) {
        for (jj=0;jj<m_cols;jj++) {
          if (destr>0 &&  m_arr[ii][jj])
            delete m_arr[ii][jj];
          m_arr[ii][jj]=0;
        }
      }
    }
    X *set(int row,int col,X *x) {
      if (row<0 || col<0)
        return 0;
      int r=m_rows,c=m_cols;
      if (row>=m_rows) {
        if (!m_autoGrow)
          return 0;
        r=row+1;
      }
      if (col>=m_cols) {
        if (!m_autoGrow)
          return 0;
        c=col+1;
      }
      resize(r,c);
      X *xx=m_arr[row][col];
      m_arr[row][col]=x;
      return xx;
    }

  protected:
    TPArr2d(const TPArr2d &)  : m_rows(0),m_cols(0),m_arr(0),m_autoGrow(1) {
    }
    TPArr2d &operator=(const TPArr2d &) {
      return *this;
    }
    bool operator==(const TPArr2d &) const {
      return false;
    }
    bool operator<(const TPArr2d &) const {
      return false;
    }

};

// *********************************

template <class X> class TVArr2d {

  protected:
    int m_rows;
    int m_cols;
    X **m_arr;
    X dum;

  public:
    int m_autoGrow;
    TVArr2d(int rows=0,int cols=0,X xdum=X()) : m_rows(0),m_cols(0),m_arr(0),dum(xdum),m_autoGrow(1) {
      int ii=0,jj=0;
      if (rows>0 && cols>0) {
        m_rows=(rows>=ipow2(maxArrSz) ? (int)ipow2(maxArrSz)-1 : rows);
        m_cols=(cols>=ipow2(maxArrSz) ? (int)ipow2(maxArrSz)-1 : cols);
        m_arr=new X*[m_rows];
        for (ii=0;ii<m_rows;ii++) {
          m_arr[ii]=new X[m_cols];
          for (jj=0;jj<m_cols;jj++)
            m_arr[ii][jj]=dum;
        }
      }
    }
    TVArr2d(const TVArr2d &ass) : m_rows(ass.m_rows),m_cols(ass.m_cols),m_arr(0),m_autoGrow(ass.m_autoGrow) {
      int ii=0,jj=0;
      if (m_rows>0 && m_cols>0) {
        m_arr=new X*[m_rows];
        for (ii=0;ii<m_rows;ii++) {
          m_arr[ii]=new X[m_cols];
          for (jj=0;jj<m_cols;jj++)
            m_arr[ii][jj]=ass.m_arr[ii][jj];
        }
      }
    }
    ~TVArr2d() {
      resize(0,0);
    }
    TVArr2d &operator=(const TVArr2d &ass) {
      resize(ass.m_rows,ass.m_cols);
      int ii=0,jj=0;
      for (ii=0;ii<m_rows;ii++) {
        for (jj=0;jj<m_cols;jj++)
          m_arr[ii][jj]=ass.m_arr[ii][jj];
      }
      return *this;
    }
    X *operator[](int row) {
      return (row<0 || row>=m_rows ? 0 : &m_arr[row][0]);
    }
    const X *operator[](int row) const {
      return (row<0 || row>=m_rows ? 0 : &m_arr[row][0]);
    }
    X *at(int row,int col) {
      if (row<0 || row>=m_rows || col<0 || col>=m_cols)
        return 0;
      return &m_arr[row][col];
    }
    const X *at(int row,int col) const {
      if (row<0 || row>=m_rows || col<0 || col>=m_cols)
        return 0;
      return (const X*)(&m_arr[row][col]);
    }
    int row(int r,TVArr<X> *rL) const {
      if (r<0 || r>=m_rows || !rL)
        return 0;
      rL->resize(m_cols);
      X *rawL=rL->rawData();
      memcpy(&rawL[0],&m_arr[r],m_cols*sizeof(X));
      rL->set(m_cols-1,m_arr[r][m_cols-1]); // last
      return m_cols;
    }
    int col(int c,TPArr<X> *cL) const {
      if (c<0 || c>=m_cols || !cL)
        return 0;
      cL->resize(m_rows);
      X *rawL=cL->rawData();
      int ii=0;
      for (ii=0;ii<m_rows;ii++)
        rawL[ii]=m_arr[ii][c];
      cL->set(m_rows-1,m_arr[m_rows-1][c]); // last
      return m_rows;
    }
    int rows() const {
      return m_rows;
    }
    int cols() const {
      return m_cols;
    }
    int resize(int r,int c,X xdum=X()) {
      dum=xdum;
      if (r<=0 || c<=0)
        r=c=0;
      if (r>=ipow2(maxArrSz))
        r=(int)ipow2(maxArrSz)-1;
      if (c>=ipow2(maxArrSz))
        c=(int)ipow2(maxArrSz)-1;
      if (r==m_rows && c==m_cols)
        return m_rows*m_cols;
      int ii=0,jj=0;
      X **arr=0;
      int cpr=(r<m_rows ? r : m_rows),cpc=(c<m_cols ? c : m_cols);
      if (cpr>0 && cpc>0) {
        arr=new X*[cpr];
        for (ii=0;ii<cpr;ii++) {
          arr[ii]=new X[cpc];
          for (jj=0;jj<cpc;jj++)
            arr[ii][jj]=m_arr[ii][jj];
        }
      }
      if (m_arr) {
        for (ii=0;ii<m_rows;ii++)
          delete [] m_arr[ii];
        delete [] m_arr;
        m_arr=0;
      }
      m_rows=r;
      m_cols=c;
      if (m_rows>0 && m_cols>0) {
        m_arr=new X*[m_rows];
        for (ii=0;ii<m_rows;ii++) {
          m_arr[ii]=new X[m_cols];
          for (jj=0;jj<m_cols;jj++)
            m_arr[ii][jj]=((arr && ii<cpr && jj<cpc) ? arr[ii][jj] : dum);
        }
        if (arr) {
          for (ii=0;ii<cpr;ii++)
            delete [] arr[ii];
          delete [] arr;
        }
      }
      return m_rows*m_cols;
    }
    void clear() {
      int ii=0,jj=0;
      for (ii=0;ii<m_rows;ii++) {
        for (jj=0;jj<m_cols;jj++)
          m_arr[ii][jj]=dum;
      }
    }
    X set(int row,int col,const X &x) {
      if (row<0 || col<0)
        return X();
      int r=m_rows,c=m_cols;
      if (row>=m_rows) {
        if (m_autoGrow==0)
          return 0;
        r=row+1;
      }
      if (col>=m_cols) {
        if (m_autoGrow==0)
          return 0;
        c=col+1;
      }
      resize(r,c);
      X xx=m_arr[row][col];
      m_arr[row][col]=x;
      return xx;
    }

};

} // namespace

#endif
