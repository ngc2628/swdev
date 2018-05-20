
/*#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>*/

#include <ctype.h>
#include <mkbase/defs.h>
#include <mkbase/mkbase.h>
#include <txtcalc/txtcalc.h>

/*static const int maxintbase=36;
static const unsigned char basechar[maxintbase+1]={'0','1','2','3','4','5','6','7','8','9',
                                  'A','B','C','D','E','F','G','H','I','J',
                                  'K','L','M','N','O','P','Q','R','S','T',
                                  'U','V','W','X','Y','Z','0'};
static const unsigned char asciispace=32;
static const unsigned char asciihash=35;
static const unsigned char asciiplus=43;
static const unsigned char asciiminus=45;
static const unsigned char asciidec=46;
static const unsigned char asciizero=48;
static const unsigned char asciione=49;
static const unsigned char asciifive=53;
static const unsigned char asciinine=57;
static const unsigned char asciiE=69;
static const unsigned char asciiF=70;
static const unsigned char asciiX=88;
static const unsigned char asciib=98;
static const unsigned char asciie=101;
static const unsigned char asciif=102;
static const unsigned char asciix=120;
static const unsigned char asciidel=127;
*/

/** ********************** helpers *************************/

static void xchch(char *c1,char *c2) {

  char tmp=*c1;
  *c1=*c2;
  *c2=tmp;

}

static const int simpleibuflen=12;

static int txtsimplei102str(int number,char *num) {

  memset(&num[0],0,simpleibuflen);
  int pos=11,sgn=(number<0 ? 1 : 0);
  if (sgn==1)
    number=-number;
  do {
    num[--pos]=(char)mk_basechar[(int)(number%10)];
    number/=10;
  } while (number>0);
  memmove(&num[sgn],&num[pos],simpleibuflen-pos-1);
  if (sgn==1)
    num[0]=(char)mk_asciiminus;

  return 0;

}

static int txtstr2simplei10(const char *num) {

  int ii=0,jj=0,number=0,mult=1,idx=-1,ll=(num ? (int)strlen(num) : 0);
  unsigned char cc=0;

  for (ii=ll-1;ii>-1;ii--)  {
    cc=(unsigned char)num[ii];
    for (jj=0;jj<10;jj++) {
      if (cc==mk_basechar[jj]) {
        idx=jj;
        break;
      }
    }
    number+=idx*mult;
    mult*=10;
  }

  return number;

}

static int txtsqueeze(char *num,int maxlen) {

  if (maxlen==0 || !num)
    return 0;

  int ii=0,ll=maxlen,endpos=-1,dpos=-1,epos=-1,sgn=0;
  unsigned char cc=(unsigned char)num[0];

  while (cc==0 && ll>0) {
    memmove(&num[0],&num[1],--ll);
    num[ll]=0;
    cc=(unsigned char)num[0];
  }
  ll=(int)strlen(num);
  if (ll==0)
    return 0;

  cc=(unsigned char)num[0];
  while (ll>0 && cc!=mk_asciie && cc!=mk_asciiE && cc!=mk_asciiminus &&
         cc!=mk_asciiplus && cc!=mk_asciidec && (cc<mk_asciizero || cc>mk_asciinine)) {
    memmove(&num[0],&num[1],--ll);
    num[ll]=0;
    cc=(unsigned char)num[0];
  }

  cc=(unsigned char)num[0];
  if (cc==mk_asciiminus || cc==mk_asciiplus) {
    if (cc==mk_asciiminus)
      sgn=1;
    memmove(&num[0],&num[1],--ll);
    num[ll]=0;
  }
  if (ll==0)
    return 0;

  cc=(unsigned char)num[0];
  while (cc==mk_asciizero && ll>1) {
    cc=(unsigned char)num[1];
    if (cc<mk_asciizero || cc>mk_asciinine)
      break;
    memmove(&num[0],&num[1],--ll);
    num[ll]=0;
    cc=(unsigned char)num[0];
  }

  for (ii=0;ii<ll;ii++) {
    cc=(unsigned char)num[ii];
    if (dpos<0 && cc==mk_asciidec)
      dpos=ii;
    else if (epos<0 && (cc==mk_asciie || cc==mk_asciiE))
      epos=ii;
    if (dpos>=0 && epos>=0)
      break;
  }

  for (ii=ll-1;ii>-1;ii--) {
    cc=(unsigned char)num[ii];
    if (cc==mk_asciie || cc==mk_asciiE || cc==mk_asciiminus || cc==mk_asciiplus ||
         cc==mk_asciidec || (cc>=mk_asciizero && cc<=mk_asciinine)) {
      endpos=ii;
      break;
    }
  }

  if (endpos<(maxlen-1)) {
    memset(&num[endpos+1],0,maxlen-endpos-1);
    ll=endpos+1;
  }

  if (dpos>=0) {
    ii=(epos>=0 ? epos-1 : endpos);
    while (1==1) {
      cc=(unsigned char)num[ii];
      if (cc!=mk_asciizero && cc!=mk_asciidec)
        break;
      if (epos>=0)
        memmove(&num[ii],&num[ii+1],ll-ii-1);
      num[endpos--]=0;
      ll--;
      ii--;
      if (cc==mk_asciidec)
        break;
    }
  }

  if ((unsigned char)num[0]==mk_asciidec) {
    memmove(&num[1],&num[0],(ll<(maxlen-1) ? ll++ : ll-1));
    num[0]=(char)mk_asciizero;
  }

  if ((unsigned char)num[ll-1]==mk_asciidec)
    num[--ll]=0;

  if (sgn==1) {
    memmove(&num[1],&num[0],ll++);
    num[0]=(char)mk_asciiminus;
  }

  return ll;

}

static int txtabs(const char *str,char **num,int *sgn,int *sz,int *qq,unsigned char *ff) {

  int ii=0,jj=0,kk=0,ll=(str ? (int)strlen(str) : 0),llx=0,esgn=1,posdec=-1;
  unsigned char cc=0;
  char *xstr=0;
  *sgn=1;
  *sz=0;
  *qq=0;
  if (ff)
    *ff='f';

  if (ll==0 || !num)
    goto fail;

  xstr=(char*)malloc(ll+3);
  memset(&xstr[0],0,ll+3);
  strcpy(xstr,str);

  ll=txtsqueeze(xstr,ll+2);

  if (ll==0)
    goto fail;

  cc=(unsigned char)xstr[0];
  if (cc==mk_asciiminus || cc==mk_asciiplus) {
    if (cc==mk_asciiminus)
      *sgn=-1;
    memmove(&xstr[0],&xstr[1],--ll);
    xstr[ll]=0;
  }

  for (ii=0;ii<ll;ii++) {
    cc=(unsigned char)xstr[ii];
    if (cc==mk_asciie || cc==mk_asciiE) {
      if (ii<(ll-1))
        memmove(&xstr[ii],&xstr[ii+1],ll-ii-1);
      xstr[--ll]=0;
      cc=(unsigned char)xstr[ii];
      if (cc==mk_asciiminus || cc==mk_asciiplus) {
        if (cc==mk_asciiminus)
          esgn=-1;
        if (ii<(ll-1))
          memmove(&xstr[ii],&xstr[ii+1],ll-ii-1);
        xstr[--ll]=0;
      }
      kk=1;
      for (jj=ll-1;jj>=ii;jj--) {
        cc=(unsigned char)xstr[jj];
        if (cc<mk_asciizero || cc>mk_asciinine)
          goto fail;
        (*qq)+=kk*(int)(cc-mk_asciizero);
         kk*=10;
      }
      memset(&xstr[ii],0,ll-ii);
      ll=ii;
      if (esgn==-1)
        (*qq)=-(*qq);
      if (ff)
        *ff='e';
      break;
    }
    else if (cc==mk_asciidec) {
      if (posdec>=0)
        goto fail;
      else
        posdec=ii;
    }
    else if (cc<mk_asciizero || cc>mk_asciinine)
      goto fail;
  }
  *sz=*qq;

  if (ll>0 && posdec==ll-1)
    xstr[--ll]=0;
  else if (posdec>=0) {
    jj=ll;
    for (ii=ll-1;ii>posdec;ii--) {
      if (xstr[ii]!=(char)mk_asciizero)
        break;
      jj--;
    }
    if (jj<ll) {
      memset(&xstr[jj],0,ll-jj);
      ll=jj;
    }
    if (posdec==ll-1)
      xstr[--ll]=0;
    else {
      if (posdec>1)
        (*sz)+=(posdec-1);
      else {
        if (posdec==0 || xstr[posdec-1]==(char)mk_asciizero) {
          (*sz)--;
          jj=posdec+1;
          while (jj<ll && xstr[jj]==(char)mk_asciizero) {
            (*sz)--;
            jj++;
          }
        }
      }
      jj=ll-posdec-1;
      (*qq)-=jj;
      memmove(&xstr[posdec],&xstr[posdec+1],jj);
      xstr[--ll]=0;
    }
  }
  else if (posdec<0)
    (*sz)+=(ll-1);

  llx=ll;
  if ((*qq)>0)
    llx+=(*qq);

  *num=(char*)malloc(llx+3);
  memset(&(*num)[0],0,llx+3);
  memcpy(&(*num)[0],&xstr[0],ll);

  if ((*qq)>0) {
    for (ii=0;ii<(*qq);ii++)
      (*num)[ll++]=(char)mk_asciizero;
    *qq=0;
  }

  txtsqueeze(*num,llx+2);

  free(xstr);

  return 0;

fail:
  if (xstr)
    free(xstr);
  *num=(char*)malloc(2);
  (*num)[0]=(char)mk_asciizero;
  (*num)[1]=0;
  *sgn=1;
  *sz=0;
  *qq=0;
  if (ff)
    *ff='f';
  return -1;

}

static int txtcmp(const char *num1,const char *num2) {

  int ii=0,res=0,l1=(num1 ? strlen(num1) : 0),l2=(num2 ? strlen(num2) : 0);

  if (l2<l1)
    res=1;
  else if (l1<l2)
    res=-1;
  else if (l1>0 && l2>0) {
    unsigned char c1=0,c2=0;
    for (ii=0;ii<l1;ii++) {
      c1=(unsigned char)num1[ii];
      c2=(unsigned char)num2[ii];
      if (c1<mk_asciizero || c1>mk_asciinine || c2<mk_asciizero || c2>mk_asciinine)
        break;
      if (c1!=c2) {
        res=(c1<c2 ? -1 : 1);
        break;
      }
    }
  }

  return res;

}

static int txtadd(char *n1,char *n2,char *n3) {

  int ii=0,jj=0,kk=0,idx=0,c1=0,c2=0,c3=0,calc=0,carry=0,
      l1=strlen(n1),l2=strlen(n2);

  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    idx=ii+1;
    c1=(int)n1[ii]-(int)mk_asciizero;
    if (c1<0 || c1>9)
      return -1;
    c2=(jj>=0 ? (int)n2[jj]-(int)mk_asciizero : 0);
    if (c2<0 || c2>9)
      return -1;
    calc=c1+c2;
    if (calc>=10) {
      carry=1;
      calc-=10;
      kk=ii;
      while (carry>0 && kk>0) {
        kk--;
        c3=(int)n1[kk];
        if (c3>0)
          c3-=(int)mk_asciizero;
        c3+=carry;
        if (c3>=10)
          c3-=10;
        else
          carry=0;
        n1[kk]=(char)(c3+(int)mk_asciizero);
      }
    }
    n3[idx]=(char)(calc+(int)mk_asciizero);
  }
  if (carry>0)
    n3[--idx]=(char)mk_asciione;

  return 0;

}

static int txtsubs(char *n1,char *n2,char *n3) {

  int ii=0,jj=0,kk=0,idx=0,c1=0,c2=0,c3=0,calc=0,carry=0,
      l1=strlen(n1),l2=strlen(n2);

  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    idx=ii+1;
    c1=(int)n1[ii]-(int)mk_asciizero;
    if (c1<0 || c1>9)
      return -1;
    c2=(jj>=0 ? (int)n2[jj]-(int)mk_asciizero : 0);
    if (c2<0 || c2>9)
      return -1;
    calc=c1-c2;
    if (calc<0) {
      carry=1;
      calc+=10;
      kk=ii;
      while (carry>0) {
        kk--;
        c3=(int)n1[kk];
        if (c3>0)
          c3-=(int)mk_asciizero;
        c3-=carry;
        if (c3<0)
          c3+=10;
        else
          carry=0;
        n1[kk]=(char)(c3+(int)mk_asciizero);
      }
    }
    n3[idx]=(char)(calc+(int)mk_asciizero);
  }

  return 0;

}

static int txtmult(char *n1,char *n2,char *n3) {

  int ii=0,jj=0,kk=0,idx=0,c1=0,c2=0,c3=0,calc=0,carry=0,
      l1=strlen(n1),l2=strlen(n2);

  for (ii=l1-1;ii>-1;ii--) {
    c1=(int)n1[ii]-(int)mk_asciizero;
    if (c1<0 || c1>9)
      return -1;
    for (jj=l2-1;jj>-1;jj--) {
      idx=ii+jj+1;
      c2=(int)n2[jj]-(int)mk_asciizero;
      if (c2<0 || c2>9)
        return -1;
      c3=(int)n3[idx];
      if (c3>0)
        c3-=(int)mk_asciizero;
      calc=c1*c2+c3;
      if (calc>9) {
        carry=calc/10;
        calc-=(carry*10);
        kk=idx;
        while (carry>0) {
          kk--;
          c3=(int)n3[kk];
          if (c3>0)
            c3-=(int)mk_asciizero;
          c3+=carry;
          if (c3>9) {
            carry=c3/10;
            c3-=(carry*10);
          }
          else
            carry=0;
          n3[kk]=(char)(c3+(int)mk_asciizero);
        }
      }
      n3[idx]=(char)(calc+(int)mk_asciizero);
    }
  }

  return 0;

}

static int txtdiv(char *n1,char *n2,char *n3,int maxlen,int *corrsz) {

  int res=0,cmp=1,ii=0,jj=0,kk=0,l1=strlen(n1),l2=strlen(n2),l3=maxlen,chklow=0;
  *corrsz=0;

  char zcmp[2]={(char)mk_asciizero,0};
  cmp=txtcmp(n1,zcmp);
  if (cmp==0) {
    memset(&n3[0],0,maxlen);
    n3[0]=(char)mk_asciizero;
    return 0;
  }
  cmp=txtcmp(n2,zcmp);
  if (cmp==0) {
    memset(&n3[0],0,maxlen);
    n3[0]=(char)mk_asciizero;
    return -1;
  }

  char *slice=(char*)malloc(l3);
  memset(&slice[0],0,l3);
  char *inter=(char*)malloc(l3);
  memset(&inter[0],0,l3);
  inter[0]=(char)mk_asciizero;
  char *xxx=(char*)malloc(l3);
  memset(&xxx[0],0,l3);
  char mult[2]={(char)mk_asciizero,0};

  memcpy(&slice[0],&n1[0],l2<l1 ? l2 : l1);

  while (kk<l3) {
    if (chklow==0 && (int)strlen(slice)==l2) {
      cmp=txtcmp(slice,n2);
      if (cmp<0)
        *corrsz=1;
      chklow=1;
    }
    for (ii=1;ii<10;ii++) {
      jj=ii;
      memset(&xxx[0],0,l3);
      mult[0]=(char)(ii+(int)mk_asciizero);
      res=txtmult(n2,mult,xxx);
      if (res<0)
        goto cleanup;
      txtsqueeze(xxx,l3);
      cmp=txtcmp(slice,xxx);
      if (cmp<0) {
        jj--;
        break;
      }
      memcpy(&inter[0],&xxx[0],l3);
      if (cmp==0)
        break;
    }
    n3[kk]=(char)(jj+(int)mk_asciizero);
    memset(&xxx[0],0,l3);
    txtsubs(slice,inter,xxx);
    jj=txtsqueeze(xxx,l3);
    cmp=txtcmp(xxx,zcmp);
    if (cmp==0) {
      if (l2+kk<l1)
        jj=0;
      else
        break;
    }
    if (l2+kk<l1)
      xxx[jj]=n1[l2+kk];
    else
      xxx[jj]=(char)mk_asciizero;
    memcpy(&slice[0],&xxx[0],l3);
    memset(&inter[0],0,l3);
    inter[0]=(char)mk_asciizero;
    kk++;
  }

cleanup:
  free(xxx);
  free(inter);
  free(slice);

  return 0;

}

static int txtfixqq(unsigned char calctype,char **n1,int qq1,char **n2,int qq2) {

  int ii=0,ll=0,llx=0,dfqq=qq1-qq2;
  if (calctype=='*' || dfqq==0)
    return 0;
  char *mod=0;
  if (dfqq<0) {
    dfqq=abs(dfqq);
    ll=strlen(*n2);
    mod=*n2;
  }
  else if (dfqq>0) {
    ll=strlen(*n1);
    mod=*n1;
  }
  llx=ll+dfqq;

  mod=(char*)realloc(mod,llx+1);
  for (ii=ll;ii<llx;ii++)
    mod[ii]=(char)mk_asciizero;
  mod[llx]=0;

  return 0;

}

static int txtunfixqq(unsigned char calctype,char *n3,int sz1,int sz2,int qq1,int qq2) {

  int ii=0,ll=(int)strlen(n3),qq=0;
  if (calctype=='+' || calctype=='-')
    qq=abs(MIN(qq1,qq2)/*(qq1<qq2 ? qq1 : qq2)*/);
  else if (calctype=='*')
    qq=abs(qq1+qq2);
  else if (calctype=='/')
    qq=(ll-(sz1-sz2)-1);

  if (qq<=0)
    return 0;

  if (ll>qq) {
    memmove(&n3[ll-qq+1],&n3[ll-qq],qq);
    n3[ll-qq]=(char)mk_asciidec;
  }
  else {
    memmove(&n3[2],&n3[0],ll);
    ii=0;
    n3[ii++]=(char)mk_asciizero;
    n3[ii++]=(char)mk_asciidec;
    while (ll<qq) {
      memmove(&n3[ii+1],&n3[ii],ll);
      n3[ii++]=(char)mk_asciizero;
      qq--;
    }
  }

  return 0;

}

/** ********************** helpers *************************/

/* ########## */
int mk_txtchknum_(int cnt,...) {

  va_list valist;
  va_start(valist,cnt);
  const char *num=(cnt>0 ? va_arg(valist,char*) : 0);
  int fmt=(cnt>1 ? va_arg(valist,int) : -1);
  int *i10=(cnt>2 ? va_arg(valist,int*) : 0);
  va_end(valist);

  if (i10)
    *i10=0;
  int ii=0,jj=0,ll=(num ? (int)strlen(num) : 0);
  if (ll==0)
    return -1;

  if (fmt<2 || fmt>mk_maxintbase)
    fmt=10;

  unsigned char cc=(unsigned char)(toupper(num[0]));

  if (fmt!=10) {
    int jj=0,kk=-1,ss=0;
    if (fmt==8 && cc==mk_asciizero)
      ss=1;
    else if (fmt==16) {
      if (cc==mk_asciiX)
        ss=1;
      else if (cc==mk_asciizero && ll>1) {
        cc=(unsigned char)(toupper(num[1]));
        if (cc==mk_asciiX)
          ss=2;
      }
      if (cc==mk_asciiX && ll==1)
        return -1;
    }
    for (ii=ss;ii<ll;ii++) {
      cc=(unsigned char)(toupper(num[ii]));
      if (cc==mk_asciispace)
        continue;
      kk=-1;
      for (jj=0;jj<fmt;jj++) {
        if (cc==mk_basechar[jj]) {
          kk=jj;
          break;
        }
      }
      if (kk<0)
        return -1;
    }
    if (i10)
      *i10=1;
    return fmt;
  }

  int dec=-1,sgn=-1,exp=-1,expsgn=-1;

  for (ii=0;ii<ll;ii++) {
    cc=(unsigned char)num[ii];
    if (cc==mk_asciispace)
      continue;
    if (cc==mk_asciie || cc==mk_asciiE) {
      if (exp>=0)
        return -1;
      exp=ii;
    }
    else if (cc==mk_asciiminus || cc==mk_asciiplus) {
      if (exp>=0) {
        if (ii!=(exp+1) || expsgn>=0)
          return -1;
        expsgn=ii;
      }
      else {
        if (ii!=0 || sgn>=0)
          return -1;
        sgn=ii;
      }
    }
    else if (cc==mk_asciidec) {
      if (dec>=0 || ll==1)
        return -1;
      dec=ii;
    }
    else {
      if (cc<mk_asciizero || cc>mk_asciinine)
        return -1;
    }
  }

  if (i10) {
    char *lnum=0;
    int sz=0,qq=0;
    if (txtabs(num,&lnum,&sgn,&sz,&qq,0)==0 && sz>=0 && qq>=0)
      *i10=1;
    free(lnum);
  }

  return fmt;

}

/* ########## */
int mk_txtcalc(unsigned char calctype,const char *n1,const char *n2,char *n3,int maxlen) {

  if (!n3 || maxlen<4 || n3==n1 || n3==n2)
    return -1;

  memset(&n3[0],0,maxlen);

  int ii=0,res=0,cmp=0,sgn1=1,sgn2=1,sz1=0,qq1=0,sz2=0,qq2=0,corrdiv=0,
      l1=(n1 ? (int)strlen(n1) : 0),l2=(n2 ? (int)strlen(n2) : 0);
  char *num1=0;
  char *num2=0;
  unsigned char ff1='f',ff2='f',addsubstype=0;

  if ((calctype!='+' && calctype!='-' && calctype!='*' && calctype!='/') ||
       l1==0 || l2==0 || mk_txtchknum(n1)<0 || mk_txtchknum(n2)<0) {
    strcpy(n3,"nan");
    goto cleanup;
  }

  txtabs(n1,&num1,&sgn1,&sz1,&qq1,&ff1);
  txtabs(n2,&num2,&sgn2,&sz2,&qq2,&ff2);
  txtfixqq(calctype,&num1,qq1,&num2,qq2);
  cmp=txtcmp(num1,num2);

  if (calctype=='*') {
    if (maxlen<=(int)(strlen(num1)+strlen(num2))) {
      strcpy(n3,"nan");
      goto cleanup;
    }
    res=txtmult(num1,num2,n3);
  }
  else if (calctype=='/') {
    res=txtdiv(num1,num2,n3,maxlen,&corrdiv);
    sz1-=corrdiv;
  }
  else {
    addsubstype=(calctype=='+' ? (sgn1==sgn2 ? '+' : '-') : (sgn1==sgn2 ? '-' : '+'));
    if (cmp<0)
      mk_swapc(&num1,&num2);
    if (maxlen<=(int)(strlen(num1)+3)) {
      strcpy(n3,"nan");
      goto cleanup;
    }
    res=(addsubstype=='+' ? txtadd(num1,num2,n3) : txtsubs(num1,num2,n3));
  }

  txtsqueeze(n3,maxlen);
  txtunfixqq(calctype,n3,sz1,sz2,qq1,qq2);
  txtsqueeze(n3,maxlen);

  if ((addsubstype==0 && sgn1!=sgn2) || (addsubstype>0 && (
       (calctype=='+' &&
         ((sgn1<0 && sgn2<0) || (sgn1<0 && sgn2>0 && cmp>0) || (sgn2<0 && sgn1>0 && cmp<0))) ||
       (calctype=='-' &&
         ((sgn1<0 && sgn2>0) || (sgn1>0 && sgn2>0 && cmp<0) || (sgn1<0 && sgn2<0 && cmp<0)))))) {
    memmove(&n3[1],&n3[0],maxlen-1);
    n3[0]=(char)mk_asciiminus;
  }

cleanup:
  if (num2)
    free(num2);
  if (num1)
    free(num1);

  return strlen(n3);


}

/* ########## */
int mk_txtround(char *num,int maxlen,int pos) {

  int cc=0,ii=0,jj=0,ll=(num ? (int)strlen(num) : 0),qq=0,rdpos=0,sgn=1,sz=0;
  if (mk_txtchknum(num)<0)
    return ll;

  ll=txtsqueeze(num,maxlen);

  if (ll<2)
    return ll;

  unsigned char ff='f';
  char *xxnum=0;
  int res=txtabs(num,&xxnum,&sgn,&sz,&qq,&ff);

  if (res<0)
    goto fail;
  ll=(int)strlen(xxnum);
  rdpos=pos+sz;
  if (rdpos>(ll-2))
    goto fail;
  if (rdpos<-1) {
    memset(&num[0],0,maxlen);
    num[0]=(char)mk_asciizero;
    goto fail;
  }
  ii=rdpos+1;
  cc=(int)xxnum[ii]-(int)mk_asciizero+5;
  while (cc>9 && ii>0) {
    ii--;
    cc=(int)xxnum[ii]-(int)mk_asciizero+1;
    if (cc<10)
      xxnum[ii]=(char)(cc+(int)mk_asciizero);
    else
      xxnum[ii]=(char)mk_asciizero;
  }
  memset(&xxnum[rdpos+1],0,ll-rdpos-1);

  if (sz>0 && rdpos<sz) {
    for (ii=rdpos+1;ii<sz+1;ii++)
      xxnum[ii]=(char)mk_asciizero;
  }

  ll=rdpos+2;
  if (cc==10) {
    ll++;
    sz++;
  }
  if (sz<0)
    ll-=sz;
  if (sgn<0)
    ll++;
  ll=rdpos+1;
  if (maxlen<=(ll+abs(sz)+2+abs(sgn)))
    goto fail;
  memset(&num[0],0,maxlen);
  ii=0;
  if (sz<0) {
    num[ii++]=(char)mk_asciizero;
    num[ii++]=(char)mk_asciidec;
    for (jj=-1;jj>sz;jj--)
      num[ii++]=(char)mk_asciizero;
    if (cc==10)
      num[ii++]=(char)mk_asciione;
    else {
      for (jj=0;jj<ll;jj++)
        num[ii++]=xxnum[jj];
    }
  }
  else {
    if (cc==10) {
      num[ii++]=(char)mk_asciione;
      sz--;
    }
    for (jj=0;jj<=sz;jj++)
      num[ii++]=xxnum[jj];
    num[ii++]=(char)mk_asciidec;
    for (jj=sz+1;jj<ll;jj++)
      num[ii++]=xxnum[jj];
  }
  ll=ii;

  if (sgn<0) {
    memmove(&num[1],&num[0],ll++);
    num[0]=(char)mk_asciiminus;
  }
  ll=txtsqueeze(num,ll+1);

fail:
  if (xxnum)
    free(xxnum);

  return (int)strlen(num);

}

/* ########## */
int mk_txtsimple2exp(char *str,int maxlen) {

  int ii=0,jj=0,ll=(str ? strlen(str) : 0),dpos=-1,sc=0,scpos=-1;

  if (mk_txtchknum(str)<0 || maxlen<(ll+2))
    return ll;

  ll=txtsqueeze(str,maxlen);

  int sgn=(str[0]==(char)mk_asciiminus ? 1 : 0);
  unsigned char cc=0;

  for (ii=0;ii<ll;ii++) {
    cc=(unsigned char)str[ii];
    if (cc==mk_asciie || cc==mk_asciiE)
      return ll;
    if (cc==mk_asciidec)
      dpos=ii;
  }
  if (dpos<0)
    dpos=ll;

  if (dpos==(sgn+1) && (unsigned char)str[sgn]!=mk_asciizero)
    return ll;

  if (sgn>0) {
    memmove(&str[0],&str[1],--ll);
    str[ll]=0;
    dpos--;
  }

  if (dpos>1)
    sc=dpos-1;
  else {
    for (ii=dpos+1;ii<ll;ii++) {
      sc--;
      if ((unsigned char)str[ii]!=mk_asciizero) {
        scpos=ii;
        break;
      }
    }
  }

  if (sc==0) {
    if (sgn==1) {
      memmove(&str[1],&str[0],ll++);
      str[0]=(char)mk_asciiminus;
    }
    return ll;
  }

  char ebuf[simpleibuflen];
  memset(&ebuf[0],0,10);
  txtsimplei102str(abs(sc),ebuf);

  if (sc>0) {
    if (dpos<(ll-1)) {
      memmove(&str[dpos],&str[dpos+1],ll-dpos-1);
      str[--ll]=0;
    }
  }
  else {
    memmove(&str[0],&str[scpos],ll-scpos);
    memset(&str[ll-scpos],0,scpos);
    ll-=scpos;
  }

  if (ll>1) {
    memmove(&str[2],&str[1],ll-1);
    str[1]=(char)mk_asciidec;
    ll++;
  }
  ll=txtsqueeze(str,maxlen);

  if (maxlen<=(ll+(int)strlen(ebuf)+1+(sc<0 || dpos==ll ? 1 : 0)+sgn+1)) {
    if (sgn==1) {
      memmove(&str[1],&str[0],ll++);
      str[0]=(char)mk_asciiminus;
    }
    return ll;
  }

  str[ll++]=(char)mk_asciiE;
  if (sc<0)
    str[ll++]=(char)mk_asciiminus;
  for (ii=0;ii<(int)strlen(ebuf);ii++)
    str[ll++]=ebuf[ii];

  if (sgn==1) {
    memmove(&str[1],&str[0],ll++);
    str[0]=(char)mk_asciiminus;
  }

  return ll;

}

/* ########## */
int mk_txtexp2simple (char *str,int maxlen) {

  int ii=0,jj=0,idx=-1,ll=(str ? strlen(str) : 0),esgn=0,
      dpos=-1,epos=-1,sc=0,mult=1;
  unsigned char cc=0;

  if (ll==0 || mk_txtchknum(str)<0 || maxlen<(ll+2))
    return ll;

  ll=txtsqueeze(str,maxlen);

  if (ll<=1)
    return ll;

  for (ii=ll-1;ii>-1;ii--) {
    cc=(unsigned char)str[ii];
    if (cc==mk_asciie || cc==mk_asciiE)
      epos=ii;
    if (cc==mk_asciidec) {
      dpos=ii;
      break;
    }
  }

  if (epos<0)
    return ll;

  if (epos==(ll-1)) {
    str[--ll]=0;
    return ll;
  }

  char *xstr=(char*)malloc(ll+1);
  strcpy(xstr,str);

  int sgn=(str[0]==(char)mk_asciiminus ? 1 : 0);
  if (sgn==1) {
    memmove(&str[0],&str[1],ll--);
    str[ll]=0;
    epos--;
    if (dpos>0)
      dpos--;
  }

  if (dpos==1 && (unsigned char)str[0]==mk_asciizero) {
    sc--;
    cc=(unsigned char)str[dpos+1];
    while (cc==mk_asciizero) {
      memmove(&str[dpos+1],&str[dpos+2],ll-dpos-2);
      str[--ll]=0;
      sc--;
      cc=(unsigned char)str[dpos+1];
    }
    str[0]=str[dpos+1];
    cc=(unsigned char)str[dpos+2];
    if (cc==mk_asciie || cc==mk_asciiE)
      str[dpos+1]=(char)mk_asciizero;
    else {
      memmove(&str[dpos+1],&str[dpos+2],ll-dpos-2);
      str[--ll]=0;
    }
  }
  else {
    if (dpos>1) {
      sc+=(dpos-1);
      memmove(&str[dpos],&str[dpos+1],ll-dpos-1);
      str[ll-1]=0;
      memmove(&str[2],&str[1],ll-1);
      str[1]=mk_asciidec;
    }
    else if (dpos<0) {
      sc+=(epos-1);
      if (epos>1) {
        memmove(&str[2],&str[1],ll-1);
        ll++;
      }
      else {
        memmove(&str[epos+2],&str[epos],ll-epos);
        str[2]=(char)mk_asciizero;
        ll+=2;
      }
      str[1]=mk_asciidec;
    }
  }

  dpos=1;
  for (ii=ll-1;ii>-1;ii--) {
    cc=(unsigned char)str[ii];
    if (cc==mk_asciie || cc==mk_asciiE) {
      epos=ii;
      break;
    }
  }

  cc=(unsigned char)str[epos+1];
  if (cc==mk_asciiminus || cc==mk_asciiplus) {
    if (cc==mk_asciiminus)
      esgn=1;
    memmove(&str[epos+1],&str[epos+2],ll-epos-2);
    str[--ll]=0;
  }

  for (ii=ll-1;ii>epos;ii--) {
    cc=(unsigned char)str[ii];
    for (jj=0;jj<10;jj++) {
      if (cc==mk_basechar[jj]) {
        idx=jj;
        break;
      }
    }
    if (esgn==0)
      sc+=(idx*mult);
    else
      sc-=(idx*mult);
    mult*=10;
  }
  memset(&str[epos],0,ll-epos);
  ll=epos;

  while (sc>0 && dpos<(ll-1)) {
    xchch(&str[dpos],&str[dpos+1]);
    sc--;
    dpos++;
  }
  if ((unsigned char)str[ll-1]==mk_asciidec)
    str[--ll]=0;

  if (maxlen<=(ll+sgn+abs(sc)+2)) {
    strcpy(str,xstr);
    free(xstr);
    return (int)strlen(str);
  }
  free(xstr);

  while (sc>0) {
    str[ll++]=mk_asciizero;
    sc--;
  }
  while (sc<0) {
    xchch(&str[dpos-1],&str[dpos]);
    sc++;
    dpos--;
    if (dpos==0) {
      memmove(&str[1],&str[0],ll);
      str[0]=(char)mk_asciizero;
      ll++;
      dpos++;
      str[ll]=0;
    }
  }

  if (sgn==1) {
    memmove(&str[1],&str[0],ll++);
    str[0]=(char)mk_asciiminus;
  }

  return ll;

}

/* ########## */
int mk_txtmod(const char *n1,const char *n2,char *div,char *carry,int maxlen) {

  int ii=0,idx=-1,l1=(n1 ? (int)strlen(n1) : 0),l2=(n2 ? (int)strlen(n2) : 0),ll=0,sgn=0;
  unsigned char cc=0;

  if (maxlen>0) {
    memset(&div[0],0,maxlen);
    memset(&carry[0],0,maxlen);
  }

  if (maxlen<=l1 || maxlen<=l2)
    return -1;

  char *n3=(char*)malloc(maxlen);
  memset(&n3[0],0,maxlen);

  ll=mk_txtcalc('/',n1,n2,n3,maxlen);

  if (ll<1) {
    free(n3);
    return -1;
  }

  cc=(unsigned char)n3[0];
  if (cc==mk_asciiminus)
    cc=(unsigned char)n3[1];
  if (cc==mk_asciizero) {
    div[0]=(char)mk_asciizero;
    strcpy(carry,n1);
    free(n3);
    return 0;
  }

  for (ii=0;ii<ll;ii++) {
    cc=(unsigned char)n3[ii];
    if (cc==mk_asciidec) {
      idx=ii;
      break;
    }
  }

  if (idx<0) {
    carry[0]=(char)mk_asciizero;
    strcpy(div,n3);
    free(n3);
    return 0;
  }

  memset(&n3[idx],0,ll-idx+1);
  strcpy(div,n3);
  char *n4=(char*)malloc(maxlen);
  memset(&n4[0],0,maxlen);
  mk_txtcalc('*',n2,n3,n4,maxlen);
  memset(&n3[0],0,maxlen);
  mk_txtcalc('-',n1,n4,n3,maxlen);
  strcpy(carry,n3);

  free(n4);
  free(n3);

  return 0;

}

/* ########## */
int mk_txtconvfmt(char *num,int fmt1,int fmt2,int maxlen) {

  int ii=0,jj=0,idx=-1,ll=(num ? (int)strlen(num) : 0),sgn=0,cmp=0,fmt=fmt1;
  unsigned char cc=0;
  char *basenum=0;
  char *num10=0;
  char *n1=0;
  char *n2=0;
  char *n3=0;
  char cmpzero[2]={(char)mk_asciizero,0};

  if (ll==0)
    goto fail;

  if (fmt1<2 || fmt1>mk_maxintbase)
    fmt1=10;

  if (mk_txtchknum(num,fmt1)<0)
    goto fail;

  if (fmt2<2 || fmt2>mk_maxintbase)
    fmt2=10;
  if (fmt1==fmt2)
    goto fail;

  if (fmt1==10) {
    ll=mk_txtexp2simple(num,maxlen);
    for (ii=0;ii<ll;ii++) {
      cc=(unsigned char)(toupper(num[ii]));
      if (cc==mk_asciidec || cc==mk_asciiE)
        goto fail;
    }
  }

  if (fmt2<fmt1) {
    ii=1;
    float lchk=(float)fmt1/(float)fmt2;
    while (lchk>1.) {
      ii++;
      lchk/=(float)fmt2;
    }
    if (maxlen<=(ii*ll))
      goto fail;
  }

  cc=(unsigned char)num[0];
  if (cc==mk_asciiminus || cc==mk_asciiplus) {
    if (cc==mk_asciiminus)
      sgn=1;
    memmove(&num[0],&num[1],--ll);
    num[ll]=0;
    cc=(unsigned char)num[0];
  }

  while (cc==mk_asciizero) {
    memmove(&num[0],&num[1],--ll);
    num[ll]=0;
    cc=(unsigned char)num[0];
  }

  if (fmt1==16) {
    cc=(unsigned char)(toupper(num[0]));
    if (cc==mk_asciiX) {
      memmove(&num[0],&num[1],--ll);
      num[ll]=0;
    }
  }

  basenum=(char*)malloc(maxlen);
  memset(&basenum[0],0,maxlen);
  num10=(char*)malloc(maxlen);
  memset(&num10[0],0,maxlen);
  n1=(char*)malloc(maxlen);
  memset(&n1[0],0,maxlen);
  n2=(char*)malloc(maxlen);
  memset(&n2[0],0,maxlen);
  n3=(char*)malloc(maxlen);
  memset(&n3[0],0,maxlen);

  if (fmt1==10)
    strcpy(num10,num);
  else {
    txtsimplei102str(fmt1,basenum);
    n3[0]=(char)mk_asciione;
    num10[0]=(char)mk_asciizero;
    for (ii=ll-1;ii>-1;ii--)  {
      cc=(unsigned char)(toupper(num[ii]));
      for (jj=0;jj<fmt1;jj++) {
        if (cc==mk_basechar[jj]) {
          idx=jj;
          break;
        }
      }
      txtsimplei102str(idx,n1);
      mk_txtcalc('*',n1,n3,n2,maxlen-1);
      memset(&n1[0],0,maxlen);
      mk_txtcalc('+',num10,n2,n1,maxlen-1);
      memset(&n2[0],0,maxlen);
      memcpy(&num10[0],&n1[0],maxlen);
      memset(&n1[0],0,maxlen);
      mk_txtcalc('*',n3,basenum,n1,maxlen-1);
      memcpy(&n3[0],&n1[0],maxlen);
      memset(&n1[0],0,maxlen);
    }
  }

  memset(&basenum[0],0,simpleibuflen);
  txtsimplei102str(fmt2,basenum);
  memset(&n1[0],0,maxlen);
  memset(&n2[0],0,maxlen);
  memset(&n3[0],0,maxlen);
  idx=-1;

  do {
    if (mk_txtmod(num10,basenum,n1,n2,maxlen)<0 || txtcmp(basenum,n2)<0)
      goto restsgn;
    n3[++idx]=(char)mk_basechar[txtstr2simplei10(n2)];
    strcpy(num10,n1);
    cmp=txtcmp(num10,cmpzero);
  } while (cmp==1);

  for (ii=idx,jj=0;ii>-1;ii--,jj++)
    num[jj]=n3[ii];
  memset(&num[idx+1],0,maxlen-idx-1);
  fmt=fmt2;

restsgn:
  if (sgn==1) {
    memmove(&num[1],&num[0],ll-1);
    num[0]=(char)mk_asciizero;
  }

fail:
  if (n3)
    free(n3);
  if (n2)
    free(n2);
  if (n1)
    free(n1);
  if (num10)
    free(num10);
  if (basenum)
    free(basenum);

  return fmt;

}

