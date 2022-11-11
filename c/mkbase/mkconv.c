
#include <mkbase/defs.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>

#include <ctype.h>

unsigned char mk_basechar[38]={
  mk_asciizero,mk_asciione,mk_asciitwo,mk_asciithree,mk_asciifour,mk_asciifive,mk_asciisix,
  mk_asciiseven,mk_asciieight,mk_asciinine,mk_asciiA,mk_asciiB,mk_asciiC,mk_asciiD,mk_asciiE,
  mk_asciiF,mk_asciiG,mk_asciiH,mk_asciiI,mk_asciiJ,mk_asciiK,mk_asciiL,mk_asciiM,mk_asciiN,
  mk_asciiO,mk_asciiP,mk_asciiQ,mk_asciiR,mk_asciiS,mk_asciiT,mk_asciiU,mk_asciiV,mk_asciiW,
  mk_asciiX,mk_asciiY,mk_asciiZ,mk_ascii_,mk_asciizero
};

/* big endian
unsigned short _d0=0; 
unsigned short _d1=1;
unsigned short _d2=2;
unsigned short _d3=3; 
*/
/* little endian */
static unsigned short _d0=3; 
static unsigned short _d1=2;
static unsigned short _d2=1;
static unsigned short _d3=0;
static unsigned short dbias=1022;
static unsigned short doff=4;
static unsigned short dfrac=15;
static unsigned short dmask11=32752;
static unsigned short ndig=16;

/* ########## */
static int mk_strip4ascii(char *str) {

  int ih=(str ? (int)strlen(str) : 0),il=0,ic=0;
  if (ih==0)
    return 1;
  while (--ih>-1) {
    ic=(int)str[ih];
    if (mk_asciispace<ic && ic<mk_asciidel)
      break;
    str[ih]=0;
  }
  while (il<=ih) {
    ic=(int)str[il];
    if (mk_asciispace<ic && ic<mk_asciidel)
      break;
    il++;
  }
  if (il>0) {
    ih-=(il-1);
    memmove(&str[0],&str[il],(size_t)ih);
    memset(&str[ih],0,(size_t)il);
  }
  return 0;

}

/* ########## */
static int mk_separators(char *decsep,int *declen,char *groupsep,int *grouplen) {

  *declen=(decsep ? (int)strlen(decsep) : 0);
  *grouplen=(groupsep ? (int)strlen(groupsep) : 0);
  while ((*declen)>3)
    decsep[--(*declen)]=0;
  while ((*grouplen)>3)
    groupsep[--(*grouplen)]=0;
  return 0;
  
}

/* ########## */
int mk_removeseparators(char *numstr,char *decsep,char *groupsep) {

  int ii=0,jj=0,numlen=(numstr ? (int)strlen(numstr) : 0),skip=0,declen=0,grouplen=0;
  if (numlen<2)
    return 1;
  mk_separators(decsep,&declen,groupsep,&grouplen);
  if (grouplen==0 && (declen==0 || (declen==1 && decsep[0]==mk_asciidec)))
    return 0;

  char *res=(char*)malloc(numlen+1);
  memset(&res[0],0,numlen+1);
  char *substr=0;
  for (ii=0;ii<numlen;ii++) {
    substr=&numstr[ii];
    if (declen>0 && (ii+declen)<numlen && strncmp(substr,decsep,declen)==0) {
      ii+=(declen-1);
      res[jj++]=mk_asciidec;
      skip=1;
    }
    if (grouplen>0 && (ii+grouplen)<numlen && strncmp(substr,groupsep,grouplen)==0) {
      ii+=(grouplen-1);
      skip=1;
    }
    if (skip==0)
      res[jj++]=numstr[ii];
    skip=0;
  }

  memcpy(&numstr[0],&res[0],jj);
  free(res);
  if (numlen>jj)
    memset(&numstr[jj],0,(numlen-jj));
  return 0;

}

/* ########## */
int mk_insertseparators(char *numstr,char *decsep,char *groupsep) {

  int ii=0,jj=0,kk=0,numlen=(numstr ? (int)strlen(numstr) : 0),
    decpos=-1,declen=0,grouplen=0,grouptrack=0,sgn=0;
  if (numlen<2)
    return 1;
  mk_separators(decsep,&declen,groupsep,&grouplen);
  for (ii=numlen-1;ii>-1;ii--) {
    if (numstr[ii]==mk_asciie || numstr[ii]==mk_asciiE)
      grouplen=0;
    if (numstr[ii]==mk_asciidec) {
      decpos=ii;
      break;
    }
  }
  if (grouplen==0 && (decpos==0 || (declen==1 && decsep[0]==mk_asciidec)))
    return 0;

  sgn=(numstr[0]==mk_asciiminus ? -1 : (numstr[0]==mk_asciiplus ? 1 : 0));
  if (sgn!=0) {
    memmove(&numstr[0],&numstr[1],numlen-1);
    numstr[--numlen]=0;
    decpos--;
  }
  mk_string res;
  mk_stringset(res,0);
  int maxlen=mk_sz;
  kk=maxlen;
  for (ii=numlen-1;ii>-1;ii--) {
    if (decpos>=0 && ii>decpos) {
      if (kk==0)
        goto lenfail;
      res[kk--]=numstr[ii];
    }
    else if (ii==decpos) {
      if (declen>0) {
        for (jj=0;jj<declen;jj++) {
          if (kk==0)
            goto lenfail;
          res[kk--]=decsep[jj];
        }
      }
      else {
        if (kk==0)
          goto lenfail;
        res[kk--]=numstr[ii];
      }
    }
    else {
      if (grouplen==0) {
        if (kk==0)
          goto lenfail;
        res[kk--]=numstr[ii];
      }
      else if (decpos<0 || (decpos>=0 && ii<decpos)) {
        if (grouptrack==3) {
          for (jj=0;jj<grouplen;jj++) {
            if (kk==0)
              goto lenfail;
            res[kk--]=groupsep[jj];
          }
          grouptrack=0;
        }
        if (kk==0)
          goto lenfail;
        res[kk--]=numstr[ii];
        grouptrack++;
      }
    }
  }
  memcpy(&numstr[0],&res[kk+1],maxlen-kk-1);
  memset(&numstr[maxlen-kk-1],0,kk+1);

lenfail:
  if (sgn<0) {
    memmove(&numstr[1],&numstr[0],numlen);
    numstr[0]=mk_asciiminus;
  }

  return 0;

}

/* ########## */
int mk_exp2simple (char *numstr) {

  int numlen=(int)strlen(numstr);
  if (numlen<4)
    return 1;
  int ii=0,jj=1,expnum=0;
  mk_string bufman;
  mk_string bufexp;
  mk_stringset(bufman,0);
  mk_stringset(bufexp,0);
  for (ii=numlen-1;ii>-1;ii--) {
    if (numstr[ii]==mk_asciie || numstr[ii]==mk_asciiE) {
      strcpy(&bufman[0],&numstr[0]);
      strcpy(&bufexp[0],&numstr[ii+1]);
      break;
    }
  }
  int mantlen=mk_stringlength(bufman),explen=mk_stringlength(bufexp);
  if (mantlen==0 || explen==0)
    return 0;
  for (ii=explen-1;ii>-1;ii--) {
    if (ii==0 && (bufexp[ii]==mk_asciiminus || bufexp[ii]==mk_asciiplus)) {
      if (bufexp[ii]==mk_asciiminus)
        expnum=-expnum;
    }
    else {
      if (bufexp[ii]<mk_asciizero || bufexp[ii]>mk_asciinine)
        return 1;
      expnum+=jj*(bufexp[ii]-mk_asciizero);
      jj*=10;
      if (expnum>mk_dmag)
        return 1;
    }
  }
  if (expnum==0) {
    strcpy(numstr,&bufman[0]);
    numstr[mantlen]=0;
    return 0;
  }
  int decpos=-1,sgn=(bufman[0]==mk_asciiminus ? -1 : (bufman[0]==mk_asciiplus ? 1 : 0));
  if (sgn!=0) {
    memmove(&bufman[0],&bufman[1],mantlen-1);
    bufman[mantlen-1]=0;
    mantlen--;
  }
  while (bufman[0]==mk_asciizero) {
    memmove(&bufman[0],&bufman[1],mantlen-1);
    mantlen--;
  }
  if (mantlen==0)
    return 0;
  for (ii=0;ii<mantlen;ii++) {
    if (bufman[ii]==mk_asciidec) {
      decpos=ii;
      break;
    }
    else if (bufman[ii]<mk_asciizero || bufman[ii]>mk_asciinine)
      return 1;
  }
  if (expnum>0) {
    if (decpos<0) {
      for (ii=0;ii<expnum;ii++)
        bufman[mantlen++]=mk_asciizero;
    }
    else {
      for (ii=decpos;ii<mantlen;ii++) {
        if (expnum==0)
          break;
        if (ii==mantlen-1)
          bufman[ii]=mk_asciizero;
        else {
          bufman[ii]=bufman[ii+1];
          bufman[ii+1]=mk_asciidec;
        }
        expnum--;
      }
      while (expnum>0) {
        bufman[mantlen++]=mk_asciizero;
        expnum--;
      }
    }
  }
  else {
    expnum=-expnum;
    if (decpos<0) {
      bufman[mantlen]=mk_asciidec;
      decpos=mantlen;
      mantlen++;
    }
    for (ii=decpos-1;ii>-1;ii--) {
      if (expnum==0)
        break;
      bufman[ii+1]=bufman[ii];
      bufman[ii]=mk_asciidec;
      expnum--;
    }
    if (expnum>0) {
      memmove(&bufman[expnum+1],&bufman[1],mantlen);
      memset(&bufman[1],mk_asciizero,expnum);
      mantlen+=expnum;
    }
  }
  if (bufman[0]==mk_asciidec) {
    memmove(&bufman[1],&bufman[0],mantlen);
    bufman[0]=mk_asciizero;
    mantlen++;
  }
  else if (bufman[mantlen-1]==mk_asciidec) {
    bufman[mantlen-1]=0;
    mantlen--;
  }
  if (sgn==-1) {
    memmove(&bufman[1],&bufman[0],mantlen);
    bufman[0]=mk_asciiminus;
    mantlen++;
  }
  strcpy(numstr,&bufman[0]);
  numstr[mantlen]=0;
  return 0;

}

/* ########## */
mk_ulreal mk_parseint(char *estr,int *base,int *sgn,char *group) {

  int ii=0,jj=0,kk=0,mysgn=1,len=(estr ? (int)strlen(estr) : 0);
  if (len==0) {
    if (base)
      *base=-1;
    return 0;
  }
  int mybase=(base ? *base : 10);
  if (mybase<2 || mybase>mk_maxintbase)
    mybase=10;
  mk_string numstr;
  for (ii=0;ii<len;ii++)
    mk_stringsetat(numstr,ii,toupper(estr[ii]));
  mk_strip4ascii(&numstr[0]);
  mk_removeseparators(&numstr[0],0,group);
  mk_exp2simple(&numstr[0]);
  len=mk_stringlength(numstr);
  if (len==0) {
    if (base)
      *base=-1;
    return 0;
  }
  mk_string tmpstr;
  mk_stringset(tmpstr,0);
  int canparse=0;
  char cc=0;
  ii=len-1;
  while (ii>-1) {
    cc=numstr[ii--];
    if (mybase==16 && (cc==mk_asciix || cc==mk_asciiX || cc==mk_asciihash))
      break;
    if (ii==0) {
      if (cc==mk_asciiminus || cc==mk_asciiplus || (mybase==8 && cc==mk_asciizero)) {
        if (cc==mk_asciiminus)
          mysgn=-1;
        break;
      }
    }
    for(kk=0;kk<mybase;kk++) {
      if (cc==mk_basechar[kk]) {
        canparse=1;
        break;
      }
    }
    if (canparse==0) {
      if (base)
        *base=-1;
      if (sgn)
        *sgn=mysgn;
      return 0;
    }
    canparse=0;
    tmpstr[jj++]=cc;
  }
  if (sgn)
    *sgn=mysgn;
  len=jj;
  if (len==0) {
    if (base)
      *base=-1;
    return 0;
  }
  for (ii=len-1,jj=0;ii>-1;ii--,jj++)
    numstr[jj]=tmpstr[ii];
  numstr[jj]=0;
  /* extra check */
  static const char *ui64limitstr[5]={ /* 2,8,10,16,37 */
    "1111111111111111111111111111111111111111111111111111111111111111",
    "1777777777777777777777","18446744073709551615",
    "FFFFFFFFFFFFFFFF","2TP7TTSV9CSRB"
  };
  static const int chklen[5]={64,22,20,16,13};
  int lchk=(mybase>16 ? 4 : (mybase>10 ? 3 : (mybase>8 ? 2 : (mybase>2 ? 1 : 0))));
  if (len>chklen[lchk]) {
    if (base)
      *base=-1;
    return 0;
  }
  else if (len==chklen[lchk]) {
    for (ii=0;ii<len;ii++) {
      cc=numstr[ii];
      if (cc>ui64limitstr[lchk][ii]) {
        if (base)
          *base=-1;
        return 0;
      }
      else if (cc<ui64limitstr[lchk][ii])
        break;
    }
  }
  mk_ulreal number=0,mult=1;
  int idx=-1;
  for (ii=len-1;ii>-1;ii--)  {
    cc=numstr[ii];
    for (jj=0;jj<mybase;jj++) {
      if (cc==mk_basechar[jj]) {
        idx=jj;
        break;
      }
    }
    number+=idx*mult;
    mult*=mybase;
  }
  if (base)
    *base=mybase;
  return number;

}

/* ########## */
/* type - 0:fail,1:int,2:double */
double mk_parsefloat(char *str,int *type,int *prec,char *decsep,char *groupsep) {

  if (prec)
    *prec=0;
  if (type)
    *type=0;
  int ii=0,inplen=(str ? (int)strlen(str) : 0);
  if (inplen==0)
    return mk_dnan;
  mk_string estr;
  mk_stringset(estr,0);
  strcpy(&estr[0],str);
  mk_strip4ascii(&estr[0]);
  inplen=mk_stringlength(estr);
  if (inplen==0)
    return mk_dnan;
  for (ii=0;ii<inplen;ii++)
    estr[ii]=(char)tolower(estr[ii]);
  if (strcmp(&estr[0],"nan")==0 || strcmp(&estr[0],"-nan")==0 ||
      strcmp(&estr[0],"inf")==0 || strcmp(&estr[0],"-inf")==0) {
    if (type)
      *type=2;
    if (strcmp(&estr[0],"nan")==0)
      return mk_dnan;
    if (strcmp(&estr[0],"-nan")==0)
      return mk_dsnan;
    if (strcmp(&estr[0],"inf")==0)
      return mk_dinf;
    if (strcmp(&estr[0],"-inf")==0)
      return mk_dsinf;
  }
  mk_removeseparators(&estr[0],decsep,groupsep);
  inplen=mk_stringlength(estr);
  if (inplen==0)
    return mk_dnan;
  int sgn=(estr[0]==mk_asciiminus ? -1 : (estr[0]==mk_asciiplus ? 1 : 0));
  if (sgn!=0) {
    memmove(&estr[0],&estr[1],inplen-1);
    estr[inplen-1]=0;
    inplen--;
    if (inplen==0)
      return mk_dnan;
  }
  mk_string bufman;
  mk_stringset(bufman,0);
  mk_string bufexp;
  mk_stringset(bufexp,0);
  for (ii=inplen-1;ii>-1;ii--) {
    if (estr[ii]==mk_asciie) {
      strncpy(&bufman[0],&estr[0],ii);
      strncpy(&bufexp[0],&estr[ii+1],inplen-ii);
      break;
    }
  }
  int expnum=0,mantlen=mk_stringlength(bufman),explen=mk_stringlength(bufexp);
  if (mantlen==0) {
    strcpy(&bufman[0],&estr[0]);
    mantlen=inplen;
  }
  /* if mantissa length is zero something went wrong - return here */
  if (mantlen==0)
    return mk_dnan;
  int jj=1;
  for (ii=explen-1;ii>-1;ii--) {
    if (ii==0 && (bufexp[ii]==mk_asciiminus || bufexp[ii]==mk_asciiplus)) {
      if (bufexp[ii]==mk_asciiminus)
        expnum=-expnum;
    }
    else {
      if (!isdigit(bufexp[ii]))
        return mk_dnan;
      expnum+=jj*(bufexp[ii]-mk_asciizero);
      jj*=10;
    }
  }
  if (abs(expnum)>mk_dmag)
    return mk_dnan;
  /* now look for the radix in the mantissa */
  jj=0;
  int kk=0,ll=0,mm=0,radix=0,magnumant=0;
  double numant=.0;
  for (ii=0;ii<mantlen;ii++) {
    if (!isdigit(bufman[ii])) {
      if (bufman[ii]==mk_asciidec) {
        /* we can not handle if there is more than one decimal point - return here */
        if (radix==1)
          return mk_dnan;
        else {
          radix=1;
          continue;
        }
      }
      /* no character beside the '.' is allowed in the mantissa - return here */
      else
        return mk_dnan;
    }
    // ok found a digit - so append it to the mantissa number the right way
    if (radix==0) {
      if (ii>=(mk_dmag-1) && bufman[ii]!=mk_asciizero)
        return mk_dnan;
      numant=10.*numant+(double)(bufman[ii]-mk_asciizero);
      if (numant>.0) {
        magnumant++;
        ll++;
      }
    }
    else {
      if (numant==.0) {
        magnumant--;
        kk++;
      }
      else if (bufman[ii]==mk_asciizero)
        mm++;
      else {
        mm++;
        if (kk+mm<=mk_dprec || (magnumant<0 && kk+mm<=mk_dprec-magnumant))
          kk+=mm;
        mm=0;
      }
      numant+=(double)(bufman[ii]-mk_asciizero)*mk_ipow10(-(++jj));
    }
  }
  int acc=(magnumant<0 ? kk : (kk+ll>mk_dprec ? mk_dprec-ll : kk));
  /* if there is a decimal point or the number is too large we must admit it is a double value */
  if (type && (radix==1 || fabs(numant)>(double)mk_ilimit))
    *type=2;
  /* but we have not succeded yet - back again to the exponent */
  if ((magnumant+expnum)>mk_dmag+1)
    return mk_dnan;
  else if ((magnumant+expnum)<((-1)*mk_dmag))
    return .0;
  double dnumber=numant*mk_ipow10(expnum)*(sgn<0 ? -1. : 1.);
  acc-=expnum;
  if (mk_isbusted(dnumber)!=0)
    return mk_dnan;
  if (fabs(dnumber)<=(double)mk_ilimit) {
    if (mk_deq(fabs(dnumber),fabs((double)((int)dnumber))) && fabs(dnumber)>=1.) {
      if (type)
        *type=1;
      acc=0;
    }
    else if (type)
      *type=2;
  }
  else if (type)
    *type=2;
  if (prec)
    *prec=acc;

  return dnumber;

}

/* ########## */
mk_ulreal mk_str2ui(char *str,int *base,int *sgn,char *group) {

  int mysgn=1,mybase=-1;
  if (base)
    mybase=*base;
  mk_ulreal unum=0;
  if (mybase<0) {
    mybase=10;
    unum=mk_parseint(str,&mybase,&mysgn,group);
    if (mybase<0) {
      mybase=2;
      unum=mk_parseint(str,&mybase,&mysgn,0);
    }
    if (mybase<0) {
      mybase=16;
      unum=mk_parseint(str,&mybase,&mysgn,0);
    }
    if (mybase<0) {
      mybase=8;
      unum=mk_parseint(str,&mybase,&mysgn,0);
    }
    if (mybase<0) {
      mybase=mk_maxintbase;
      unum=mk_parseint(str,&mybase,&mysgn,0);
    }
  }
  else
    unum=mk_parseint(str,&mybase,&mysgn,group);
  if (base)
    *base=mybase;
  if (sgn)
    *sgn=mysgn;
  return unum;
  
}

/* ########## */
mk_ulreal mk_a2ui_(int cnt,...) {

  va_list valist;
  va_start(valist,cnt);
  char *str=(cnt>0 ? va_arg(valist,char *) : 0);
  int *base=(cnt>1 ? va_arg(valist,int *) : 0);
  int *sign=(cnt>2 ? va_arg(valist,int *) : 0);
  char *group=(cnt>3 ? va_arg(valist,char *) : 0);
  va_end(valist);
  return mk_str2ui(str,base,sign,group);

}

/* ########## */
mk_lreal mk_a2i_(int cnt,...) {

  va_list valist;
  va_start(valist,cnt);
  char *str=(cnt>0 ? va_arg(valist,char *) : 0);
  int *base=(cnt>1 ? va_arg(valist,int *) : 0);
  char *group=(cnt>2 ? va_arg(valist,char *) : 0);
  va_end(valist);

  int sgn=1;
  mk_lreal res=(mk_lreal)mk_str2ui(str,base,&sgn,group);
  return sgn*res;

}

/* ########## */
int mk_ui2str(mk_ulreal number,mk_string numstr,int base,int width,int fillzero,char *group) {

  mk_stringset(numstr,0);
  if (base<2 || base>mk_maxintbase)
    base=10;
  mk_ulreal num=number;
  int pos=-1;
  mk_string buf;
  mk_stringset(buf,0);
  do {
    buf[++pos]=(char)mk_basechar[(int)(num%base)];
    num/=base;
  } while (num>0);
  while ((pos+1)<width && pos<64+1 && fillzero==1)
    buf[++pos]=mk_asciizero;
  if (base==8)
    buf[++pos]=mk_asciizero;
  if (base==16) {
    buf[++pos]=mk_asciix;
    buf[++pos]=mk_asciizero;
  }
  while (fillzero==0 && (pos+1)<(width+(base==8 ? 1 : (base==16 ? 2 : 0))) && pos<64+1)
    buf[++pos]=mk_asciispace;
  int ii=0,jj=0;
  for (ii=pos,jj=0;ii>-1;ii--,jj++)
    numstr[jj]=buf[ii];
  numstr[pos+1]=0;
  if (base==10 && group && strlen(group)>0)
    mk_insertseparators(&numstr[0],0,group);
  return base;

}

/* ########## */
int mk_ui2a_(int cnt,...) {

  va_list valist;
  va_start(valist,cnt);
  mk_ulreal number=(cnt>0 ? va_arg(valist,mk_ulreal) : 0);
  char *str=(cnt>1 ? va_arg(valist,char *) : 0);
  int base=(cnt>2 ? va_arg(valist,int) : 10);
  int width=(cnt>3 ? va_arg(valist,int) : 0);
  int padzero=(cnt>4 ? va_arg(valist,int) : 1);
  char *group=(cnt>5 ? va_arg(valist,char *) : 0);
  va_end(valist);

  mk_string resx;
  base=mk_ui2str(number,resx,base,width,padzero,group);
  if (str)
    strcpy(str,&resx[0]);
  return base;
  
}

/* ########## */
int mk_i2str(mk_lreal number,mk_string numstr,int base,int width,int fillzero,char *group) {

  int sgn=(int)(number>>63);
  mk_ulreal unumber=(mk_ulreal)(number&mk_i64limit);
  base=mk_ui2str(unumber,numstr,base,width,fillzero,group);
  if (sgn>0) {
    char ss[2]={mk_asciiminus,0};
    mk_stringprepend(numstr,&ss[0]);
  }
  return base;

}

/* ########## */
int mk_i2a_(int cnt,...) {

  va_list valist;
  va_start(valist,cnt);
  mk_lreal number=(cnt>0 ? va_arg(valist,mk_lreal) : 0);
  char *str=(cnt>1 ? va_arg(valist,char *) : 0);
  int base=(cnt>2 ? va_arg(valist,int) : 10);
  int width=(cnt>3 ? va_arg(valist,int) : 0);
  int padzero=(cnt>4 ? va_arg(valist,int) : 1);
  char *group=(cnt>5 ? va_arg(valist,char *) : 0);
  va_end(valist);

  mk_string resx;
  base=mk_i2str(number,resx,base,width,padzero,group);
  if (str)
    strcpy(str,&resx[0]);
  return base;
  
}

/* ########## */
double mk_str2d(char *str,int *prec,char *dec,char *group) {

  int type=0;
  double res=mk_parsefloat(str,&type,prec,dec,group);
  return (type==0 ? mk_dnan : res);

}

/* ########## */
double mk_a2d_(int cnt,...) {

  va_list valist;
  va_start(valist,cnt);
  char *str=(cnt>0 ? va_arg(valist,char *) : 0);
  int *prec=(cnt>1 ? va_arg(valist,int*) : 0);
  char *dec=(cnt>2 ? va_arg(valist,char *) : ".");
  char *group=(cnt>3 ? va_arg(valist,char *) : 0);
  va_end(valist);
  return mk_str2d(str,prec,dec,group);

}

int failedcnt1=0,failedcnt2=0;
/* ########## */
int mk_dconvert (double d,mk_string str,int p,char fmt,int pad) {

  str[0]=0;
  int db=mk_isbusted(d);
  if (db!=0) {
    strcpy(&str[0],(mk_isinf(d) ? (db<0 ? "-INF" : "INF") : (db<0 ? "-NaN" : "NaN")));
    return 0;
  }
  union tp_dpur {
    double d;
    mk_ulreal ur;
  };
  union tp_ucur {
    unsigned char uc[8];
    mk_ulreal ur;
  };
  double dd=d,ddd=dd;

  /* resolve exponent should be concise ...
   unsigned short *ps=(unsigned short *)&d; // ps still points to double !!!
   short xchar=((ps[_d0]&dMask11)>>dOff),sg=(ps[_d0]>>15)&1;
   ps[_d0]=(((ps[_d0]&~dMask11)|dBias<<dOff)&sLimit);

   ... *** but if optimizing takes place gcc does not allow
   to manipulate d as subscript 0 of array of short so 64bits needed */
  union tp_ucur ur48l={{255,255,255,255,255,255,0,0}},ur16h={{0,0,0,0,0,0,255,255}};
  union tp_dpur tpddd={ddd};
  mk_ulreal ud=*(&tpddd.ur);
  unsigned short optps=(unsigned short)((ud&ur16h.ur)>>48),sg=(optps&(mk_slimit+1));
  short xchar=((optps&dmask11)>>doff);
  optps=(((optps&~dmask11)|dbias<<doff)&mk_slimit);
  tpddd.ur=((ud&ur48l.ur)|((mk_ulreal)optps<<48));
  ddd=*(&tpddd.d);

  short bexp=(xchar==0 ? 0 : xchar-dbias);
  double bexplog10=(double)bexp*mk_log210,tmp=1.,scl=.1,sch=10.,dsc=.0;
  short sc=(short)bexplog10,csc=sc;
  int maxlen=mk_sz,kk=0;
  double efrac=mk_log10e*(bexplog10-(double)sc);
  if (sc>=0 && efrac!=.0) {
    tmp=dsc=1.;
    while (++kk<25) {
      tmp*=(efrac/(double)kk);
      dsc+=tmp;
    }
    dsc*=ddd;
  }
  else {
    tmp=(ddd-1.)/(ddd+1.);
    if (bexp>.0)
      sc++;
    if ((short)(bexplog10+2.*mk_loge10*tmp*(1+tmp*(tmp*(1./3.+tmp*tmp/5.))))<csc)
      sc--;
    dsc=fabs(dd);
    kk=abs(sc);
    while (kk-->0)
      dsc*=(sc<0 ? sch : scl);
  }
  dsc+=mk_ipow10((-1)*ndig);
  if (1.<=dsc) {
    failedcnt2++;
    dsc*=scl;
    sc++;
  }
  if (dsc<.1) {
    failedcnt1++;
    dsc*=sch;
    sc--;
  }

  unsigned short pos=0,frac=0;
  if (sg>0)
    str[pos++]=mk_asciiminus;
  tmp=dsc;
  kk=ndig/4;
  while (kk-->0)
    tmp*=sch;
  frac=(unsigned short)tmp;
  if (frac==0 || (unsigned short)p<-sc) {
    str[pos++]=mk_asciizero;
    if (0<pad) {
      str[pos++]=mk_asciidec;
      while (0<pad--)
        str[pos++]=mk_asciizero;
    }
    return 0;
  }
  int ii=0,jj=-1,lastpos=maxlen-1,decpos=ndig-sc,rdpos=decpos-p;
  char c=0,co=0,lastc=0,carry=0;
  unsigned short ifrac[4]={frac,0,0,0};
  ifrac[3]=frac;
  for (ii=2;ii>-1;ii--) {
    tmp-=(double)ifrac[ii+1];
    kk=ndig/4;
    if (ii==0 && rdpos<=0)
      kk--;
    while (kk-->0)
      tmp*=sch;
    ifrac[ii]=(unsigned short)tmp;
    if (ii==0 && rdpos<=0)
      ifrac[ii]*=10;
  }
  frac=ifrac[0];
  if (fmt==0 || fmt==mk_asciif || fmt==mk_asciiF)
    fmt=0;
  if (fmt>0 || fmt==mk_asciie || fmt==mk_asciiE)
    fmt=1;
  if (fmt<0)
    fmt=((sc<-6 || 6<sc) ? 1 : 0);

  if (fmt==0) {
    for (ii=ndig;ii<sc;ii++)
      str[lastpos--]=mk_asciizero;
  }
  for (ii=0;ii<ndig;ii++) {
    if (ii>0 && (ii%4)==0)
      frac=ifrac[ii/4];
    c=(char)mk_basechar[(int)(frac%10)];
    frac/=10;
    if (ii<rdpos) {
      if (ii==rdpos-1 && c+5>mk_asciinine)
        carry=1;
      c=(ii<decpos ? 0 : mk_asciizero);
    }
    else if (carry==1) {
      c++;
      if (c<=mk_asciinine)
        carry=0;
      else
        c=(ii<decpos ? 0 : mk_asciizero);
    }
    if (fmt==0) {
      if (ii<decpos && c==mk_asciizero && lastc==0)
        c=0;
      if (ii==decpos && lastc>0)
        str[lastpos--]=mk_asciidec;
    }
    else {
      if (c==mk_asciizero && lastc==0)
        c=0;
      if (ii==ndig-1 && carry==0 && (lastc!=0 || 0<pad))
        str[lastpos--]=mk_asciidec;
    }
    lastc=c;
    str[lastpos--]=c;
    co|=c;
  }
  if (co==0 && carry==0) {
    pos=0;
    if (sg>0)
      str[pos++]=mk_asciiminus;
    str[pos++]=mk_asciizero;
    if (0<pad) {
      str[pos++]=mk_asciidec;
      while (0<pad--)
        str[pos++]=mk_asciizero;
    }
    str[pos++]=0;
    return 0;
  }
  if (fmt==0) {
    if (carry==1)
      str[lastpos--]=mk_asciione;
    if (sc<0 || (carry==0 && sc==0)) {
      str[pos++]=mk_asciizero;
      str[pos++]=mk_asciidec;
      for (ii=0;ii<abs(sc)-carry;ii++)
        str[pos++]=mk_asciizero;
    }
  }
  else {
    if (carry==1) {
      if (lastc!=0 || 0<pad)
        str[lastpos--]=mk_asciidec;
      str[lastpos--]=mk_asciione;
    }
    else sc--;
  }
  memmove(&str[pos],&str[lastpos+1],(size_t)(maxlen-lastpos-1));
  pos=(unsigned short)strlen(str);
  if (0<pad) {
    for (ii=pos-1;ii>-1;ii--) {
      if (str[ii]==mk_asciidec) {
        jj=pos-ii-1;
        break;
      }
    }
    if (fmt==0) {
      if (0<jj && pad<=jj)
        pad=0;
      else {
        if (jj<0)
          str[pos++]=mk_asciidec;
      }
    }
    else {
      if (ndig<pad)
        pad=ndig;
    }
    if (0<jj)
      pad-=(short)jj;
    for (ii=0;ii<pad;ii++)
      str[pos++]=mk_asciizero;
  }
  if (fmt==1) {
    str[pos++]=mk_asciie;
    if (sc<0) {
      sc=(short)abs((int)sc);
      str[pos++]=mk_asciiminus;
    }
    else
      str[pos++]=mk_asciiplus;
    ii=sc/100;
    if (0<ii) {
      sc-=(short)(ii*100);
      str[pos++]=(char)mk_basechar[ii];
    }
    jj=sc/10;
    if (0<jj || 0<ii) {
      sc-=(short)(jj*10);
      str[pos++]=(char)mk_basechar[jj];
    }
    str[pos++]=(char)mk_basechar[sc];
  }
  str[pos++]=0;

  return 0;

}

/* ########## */
int mk_d2str(double d,mk_string str,int p,char fmt,int pad,char *decsep,char *groupsep) {

  mk_stringset(str,0);
  mk_dconvert(d,str,p,fmt,pad);
  if ((decsep && strlen(decsep)>0 && strcmp(decsep,".")) ||
      (fmt==0 && groupsep && strlen(groupsep)>0))
    mk_insertseparators(&str[0],decsep,groupsep);
  return 0;

}

/* ########## */
int mk_d2a_(int cnt,...) {

  va_list valist;
  va_start(valist,cnt);
  double number=(cnt>0 ? va_arg(valist,double) : mk_dnan);
  char *str=(cnt>1 ? va_arg(valist,char *) : 0);
  int prec=(cnt>2 ? va_arg(valist,int) : 15);
  int fmt=(cnt>3 ? va_arg(valist,int) : -1);
  int pad=(cnt>4 ? va_arg(valist,int) : 0);
  char *dec=(cnt>5 ? va_arg(valist,char *) : ".");
  char *group=(cnt>6 ? va_arg(valist,char *) : 0);
  va_end(valist);

  mk_string resx;
  mk_d2str(number,resx,prec,fmt,pad,dec,group);
  if (str)
    strcpy(str,&resx[0]);
  return 0;

}

