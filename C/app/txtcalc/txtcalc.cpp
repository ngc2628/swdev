
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const unsigned char asciispace=32;
static const unsigned char asciiplus=43;
static const unsigned char asciiminus=45;
static const unsigned char asciidec=46;
static const unsigned char asciizero=48;
static const unsigned char asciione=49;
static const unsigned char asciinine=57;
static const unsigned char asciiE=69;
static const unsigned char asciie=101;

static int xxsqueeze(char *num,int ll) {

  if (ll==0)
    return 0;

  int ii=0,llx=ll,start=-1,end=-1,dpos=-1,epos=-1;
  unsigned char cc=0;

  for (ii=0;ii<ll;ii++) {
    cc=(unsigned char)num[ii];
    if (start<0) {
      if (cc==asciie || cc==asciiE || cc==asciiminus || cc==asciiplus ||
           cc==asciidec || (cc>asciizero && cc<=asciinine))
        start=ii;
    }
    if (dpos<0 && cc==asciidec)
      dpos=ii;
    else if (epos<0 && (cc==asciie || cc==asciiE))
      epos=ii;
    if (dpos>=0 && epos>=0)
      break;
  }

  for (ii=ll-1;ii>=start;ii--) {
    cc=(ii<0 ? 0 : (unsigned char)num[ii]);
    if (cc==asciie || cc==asciiE || cc==asciiminus || cc==asciiplus ||
         cc==asciidec || (cc>=asciizero && cc<=asciinine)) {
      end=ii;
      break;
    }
  }

  if (start<0 || end<0) {
    num[0]=(char)asciizero;
    memset(&num[1],0,ll);
    return 1;
  }

  if (end<(ll-1)) {
    memset(&num[end+1],0,ll-end-1);
    ll=end+1;
  }

  if (dpos>=0) {
    ii=(epos>=0 ? epos-1 : end);
    while (num[ii]==(char)asciizero) {
      if (epos>=0)
        memmove(&num[ii],&num[ii+1],ll-ii-1);
      num[end--]=0;
      ll--;
      ii--;
    }
  }

  if (start>0) {
    memmove(&num[0],&num[start],ll-start);
    memset(&num[ll-start],0,start);
    ll-=start;
  }

  if (num[ll-1]==(char)asciidec) {
    num[ll-1]=0;
    ll--;
  }

  if (num[0]==(char)asciidec) {
    memmove(&num[1],&num[0],ll);
    num[0]=(char)asciizero;
    ll++;
  }

  if (ll==0) {
    num[0]=(char)asciizero;
    ll++;
  }

  if (ll<llx)
    memset(&num[ll],0,llx-ll);

  return ll;

}

static int xxabs(char *str,char **num,int *sgn,int *sz,int *qq,unsigned char *ff) {

  int ii=0,jj=0,kk=0,ll=(str ? (int)strlen(str) : 0),llx=0,esgn=1,posdec=-1;
  unsigned char cc=0;
  *sz=0;
  *qq=0;
  *ff='f';

  if (ll==0)
    goto fail;

  ll=xxsqueeze(str,ll);

  cc=(unsigned char)str[0];
  if (cc==asciiminus || cc==asciiplus) {
    if (cc==asciiminus)
      *sgn=-1;
    memmove(&str[0],&str[1],ll);
    str[ll-1]=0;
    ll--;
  }

  for (ii=0;ii<ll;ii++) {
    cc=(unsigned char)str[ii];
    if (cc==asciie || cc==asciiE) {
      if (ii<(ll-1))
        memmove(&str[ii],&str[ii+1],ll-ii-1);
      ll--;
      str[ll]=0;
      cc=(unsigned char)str[ii];
      if (cc==asciiminus || cc==asciiplus) {
        if (cc==asciiminus)
          esgn=-1;
        if (ii<(ll-1))
          memmove(&str[ii],&str[ii+1],ll-ii-1);
        ll--;
        str[ll]=0;
      }
      kk=1;
      for (jj=ll-1;jj>=ii;jj--) {
        cc=(unsigned char)str[jj];
        if (cc<asciizero || cc>asciinine)
          goto fail;
        (*qq)+=kk*(int)(cc-asciizero);
         kk*=10;
      }
      memset(&str[ii],0,ll-ii);
      ll=ii;
      if (esgn==-1)
        (*qq)=-(*qq);
      *ff='e';
      break;
    }
    else if (cc==asciidec) {
      if (posdec>=0)
        goto fail;
      else
        posdec=ii;
    }
    else if (cc<asciizero || cc>asciinine)
      goto fail;
  }
  *sz=*qq;

  if (posdec==ll-1) {
    str[ll-1]=0;
    ll--;
  }
  else if (posdec>=0) {
    jj=ll;
    for (ii=ll-1;ii>posdec;ii--) {
      if (str[ii]!=(char)asciizero)
        break;
      jj--;
    }
    if (jj<ll) {
      memset(&str[jj],0,ll-jj);
      ll=jj;
    }
    if (posdec==ll-1) {
      str[ll-1]=0;
      ll--;
    }
    else {
      if (posdec>1)
        (*sz)+=(posdec-1);
      else {
        if (str[posdec-1]==(char)asciizero)
          (*sz)--;
        jj=posdec+1;
        while (jj<ll && str[jj]==(char)asciizero) {
          (*sz)--;
          jj++;
        }
      }
      jj=ll-posdec-1;
      (*qq)-=jj;
      memmove(&str[posdec],&str[posdec+1],jj);
      ll--;
      str[ll]=0;
    }
  }
  else if (posdec<0)
    (*sz)+=(ll-1);

  llx=ll;
  if ((*qq)>0)
    llx+=(*qq);

  *num=(char*)malloc(llx+1);
  memset(&(*num)[0],0,llx+1);
  memcpy(&(*num)[0],&str[0],ll);

  if ((*qq)>0) {
    for (ii=0;ii<(*qq);ii++)
      (*num)[ll++]=(char)asciizero;
    *qq=0;
  }

  xxsqueeze(*num,llx+1);

  return 0;

fail:
  *num=(char*)malloc(2);
  (*num)[0]==(char)asciizero;
  (*num)[1]==0;
  *sgn=1;
  *sz=0;
  *qq=0;
  *ff='0';
  return -1;

}

static int xxcmp(const char *num1,const char *num2) {

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
      if (c1<asciizero || c1>asciinine || c2<asciizero || c2>asciinine)
        break;
      if (c1!=c2) {
        res=(c1<c2 ? -1 : 1);
        break;
      }
    }
  }

  return res;

}

static int xxadd(char *n1,char *n2,char *n3) {

  int ii=0,jj=0,kk=0,idx=0,c1=0,c2=0,c3=0,calc=0,carry=0,
      l1=strlen(n1),l2=strlen(n2);

  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    idx=ii+1;
    c1=(int)n1[ii]-(int)asciizero;
    if (c1<0 || c1>9)
      return -1;
    c2=(jj>=0 ? (int)n2[jj]-(int)asciizero : 0);
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
          c3-=(int)asciizero;
        c3+=carry;
        if (c3>=10)
          c3-=10;
        else
          carry=0;
        n1[kk]=(char)(c3+(int)asciizero);
      }
    }
    n3[idx]=(char)(calc+(int)asciizero);
  }
  if (carry>0)
    n3[--idx]=(char)asciione;

  return 0;

}

static int xxsubs(char *n1,char *n2,char *n3) {

  int ii=0,jj=0,kk=0,idx=0,c1=0,c2=0,c3=0,calc=0,carry=0,
      l1=strlen(n1),l2=strlen(n2);

  for (ii=l1-1,jj=l2-1;ii>=0;ii--,jj--) {
    idx=ii+1;
    c1=(int)n1[ii]-(int)asciizero;
    if (c1<0 || c1>9)
      return -1;
    c2=(jj>=0 ? (int)n2[jj]-(int)asciizero : 0);
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
          c3-=(int)asciizero;
        c3-=carry;
        if (c3<0)
          c3+=10;
        else
          carry=0;
        n1[kk]=(char)(c3+(int)asciizero);
      }
    }
    n3[idx]=(char)(calc+(int)asciizero);
  }

  return 0;

}

static int xxmult(char *n1,char *n2,char *n3) {

  int ii=0,jj=0,kk=0,idx=0,c1=0,c2=0,c3=0,calc=0,carry=0,
      l1=strlen(n1),l2=strlen(n2);

  for (ii=l1-1;ii>-1;ii--) {
    c1=(int)n1[ii]-(int)asciizero;
    if (c1<0 || c1>9)
      return -1;
    for (jj=l2-1;jj>-1;jj--) {
      idx=ii+jj+1;
      c2=(int)n2[jj]-(int)asciizero;
      if (c2<0 || c2>9)
        return -1;
      c3=(int)n3[idx];
      if (c3>0)
        c3-=(int)asciizero;
      calc=c1*c2+c3;
      if (calc>9) {
        carry=calc/10;
        calc-=(carry*10);
        kk=idx;
        while (carry>0) {
          kk--;
          c3=(int)n3[kk];
          if (c3>0)
            c3-=(int)asciizero;
          c3+=carry;
          if (c3>9) {
            carry=c3/10;
            c3-=(carry*10);
          }
          else
            carry=0;
          n3[kk]=(char)(c3+(int)asciizero);
        }
      }
      n3[idx]=(char)(calc+(int)asciizero);
    }
  }

  return 0;

}

static int xxdiv(char *n1,char *n2,char *n3,int maxlen,int *corrsz) {

  int res=0,cmp=1,ii=0,jj=0,kk=0,l1=strlen(n1),l2=strlen(n2),l3=maxlen,chklow=0;
  *corrsz=0;

  char zcmp[2]={(char)asciizero,0};
  cmp=xxcmp(n1,zcmp);
  if (cmp==0) {
    memset(&n3[0],0,maxlen);
    n3[0]=(char)asciizero;
    return 0;
  }
  cmp=xxcmp(n2,zcmp);
  if (cmp==0) {
    memset(&n3[0],0,maxlen);
    n3[0]=(char)asciizero;
    return -1;
  }

  char *slice=(char*)malloc(l3);
  memset(&slice[0],0,l3);
  char *inter=(char*)malloc(l3);
  memset(&inter[0],0,l3);
  inter[0]=(char)asciizero;
  char *xxx=(char*)malloc(l3);
  memset(&xxx[0],0,l3);
  char mult[2]={(char)asciizero,0};

  memcpy(&slice[0],&n1[0],l2<l1 ? l2 : l1);

  while (kk<l3) {
    if (chklow==0 && (int)strlen(slice)==l2) {
      cmp=xxcmp(slice,n2);
      if (cmp<0)
        *corrsz=1;
      chklow=1;
    }
    for (ii=1;ii<10;ii++) {
      jj=ii;
      memset(&xxx[0],0,l3);
      mult[0]=(char)(ii+(int)asciizero);
      res=xxmult(n2,mult,xxx);
      if (res<0)
        goto cleanup;
      xxsqueeze(xxx,l3);
      cmp=xxcmp(slice,xxx);
      if (cmp<0) {
        jj--;
        break;
      }
      memcpy(&inter[0],&xxx[0],l3);
      if (cmp==0)
        break;
    }
    n3[kk]=(char)(jj+(int)asciizero);
    memset(&xxx[0],0,l3);
    xxsubs(slice,inter,xxx);
    jj=xxsqueeze(xxx,l3);
    cmp=xxcmp(xxx,zcmp);
    if (cmp==0) {
      if (l2+kk<l1)
        jj=0;
      else
        break;
    }
    if (l2+kk<l1)
      xxx[jj]=n1[l2+kk];
    else
      xxx[jj]=(char)asciizero;
    memcpy(&slice[0],&xxx[0],l3);
    memset(&inter[0],0,l3);
    inter[0]=(char)asciizero;
    kk++;
  }

cleanup:
  free(inter);
  free(slice);
  free(xxx);

  return 0;

}

static int xxfixqq(unsigned char calctype,char **n1,int qq1,char **n2,int qq2) {

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
    mod[ii]=(char)asciizero;
  mod[llx]=0;

  return 0;

}

static int xxunfixqq(unsigned char calctype,char *n3,int sz1,int sz2,int qq1,int qq2) {

  int ii=0,ll=strlen(n3),qq=0;
  if (calctype=='+' || calctype=='-') {
    if (qq1<qq2)
      qq=abs(qq1);
    else
      qq=abs(qq2);
  }
  else if (calctype=='*')
    qq=abs(qq1+qq2);
  else if (calctype=='/')
    qq=(ll-(sz1-sz2)-1);

  if (qq==0)
    return 0;

  if (ll>qq) {
    memmove(&n3[ll-qq+1],&n3[ll-qq],qq);
    n3[ll-qq]=(char)asciidec;
  }
  else {
    memmove(&n3[2],&n3[0],ll);
    ii=0;
    n3[ii++]=(char)asciizero;
    n3[ii++]=(char)asciidec;
    while (ll<qq) {
      memmove(&n3[ii+1],&n3[ii],ll);
      n3[ii++]=(char)asciizero;
      qq--;
    }
  }

  return 0;

}

int xxcalc(unsigned char calctype,char *n1,char *n2,char *n3,int maxlen) {

  if (!n3 || maxlen<3 || (calctype!='+' && calctype!='-' && calctype!='*' && calctype!='/'))
    return -1;

  char *num1=0;
  char *num2=0;
  unsigned char ff1='f',ff2='f';
  int ii=0,res=0,cmp=0,sgn1=1,sgn2=1,l1=0,l2=0,sz1=0,qq1=0,sz2=0,qq2=0,corrdiv=0;
  xxabs(n1,&num1,&sgn1,&sz1,&qq1,&ff1);
  xxabs(n2,&num2,&sgn2,&sz2,&qq2,&ff2);
  xxfixqq(calctype,&num1,qq1,&num2,qq2);
//printf ("%d docalc : %s %c %s sgn=[%d,%d] sz=[%d,%d] qq=[%d,%d]\n",__LINE__,
//num1,calctype,num2,sgn1,sgn2,sz1,sz2,qq1,qq2);
  cmp=xxcmp(num1,num2);

  memset(&n3[0],0,maxlen);
  unsigned char addsubstype=0;

  if (calctype=='*') {
    res=xxmult(num1,num2,n3);
  }
  else if (calctype=='/') {
    res=xxdiv(num1,num2,n3,maxlen,&corrdiv);
    sz1-=corrdiv;
  }
  else {
    addsubstype=(calctype=='+' ? (sgn1==sgn2 ? '+' : '-') : (sgn1==sgn2 ? '-' : '+'));
    if (cmp<0) {
      char *swp=num1;
      num1=num2;
      num2=swp;
    }
    if ((l1+3)<=maxlen)
      res=(addsubstype=='+' ? xxadd(num1,num2,n3) : xxsubs(num1,num2,n3));
  }

  xxsqueeze(n3,maxlen);
  xxunfixqq(calctype,n3,sz1,sz2,qq1,qq2);
  xxsqueeze(n3,maxlen);

  if ((addsubstype==0 && sgn1!=sgn2) || (addsubstype>0 && (
       (calctype=='+' &&
         ((sgn1<0 && sgn2<0) || (sgn1<0 && sgn2>0 && cmp>0) || (sgn2<0 && sgn1>0 && cmp<0))) ||
       (calctype=='-' &&
         ((sgn1<0 && sgn2>0) || (sgn1>0 && sgn2>0 && cmp<0) || (sgn1<0 && sgn2<0 && cmp<0)))))) {
    memmove(&n3[1],&n3[0],maxlen-1);
    n3[0]=(char)asciiminus;
  }

  free(num2);
  free(num1);
  return strlen(n3);

}

int xxround(char *num,int maxlen,int pos) {

  int cc=0,ii=0,jj=0,llnum=(num ? strlen(num) : 0),ll=llnum,qq=0,rdpos=0,sgn=1,sz=0;
  if (ll<2)
    return 0;
  char *xnum=(char*)malloc(ll+1);
  memcpy(&xnum[0],&num[0],ll+1);
  unsigned char ff='f';
  char *xxnum=0;
  int res=xxabs(xnum,&xxnum,&sgn,&sz,&qq,&ff);
  if (res<0)
    goto fail;
  ll=strlen(xxnum);
  rdpos=pos+sz;
  if (rdpos>(ll-2))
    goto fail;
  if (rdpos<-1) {
    memset(&num[0],0,maxlen);
    if (sgn<0)
      num[ii++]=(char)asciiminus;
    num[ii]=(char)asciizero;
    goto fail;
  }
  ii=rdpos+1;
  cc=(int)xxnum[ii]-(int)asciizero+5;
  while (cc>9 && ii>0) {
    ii--;
    cc=(int)xxnum[ii]-(int)asciizero+1;
    if (cc<10)
      xxnum[ii]=(char)(cc+(int)asciizero);
    else
      xxnum[ii]=(char)asciizero;
  }
  memset(&xxnum[rdpos+1],0,ll-rdpos-1);
  ll=rdpos+2;
  if (cc==10) {
    ll++;
    sz++;
  }
  if (sz<0)
    ll-=sz;
  if (sgn<0)
    ll++;
  if (llnum<ll)
    xnum=(char*)realloc(xnum,ll+1);
  memset(&xnum[0],0,ll+1);
  ll=rdpos+1;
  ii=0;
  if (sz<0) {
    xnum[ii++]=(char)asciizero;
    xnum[ii++]=(char)asciidec;
    for (jj=-1;jj>sz;jj--)
      xnum[ii++]=(char)asciizero;
    if (cc==10)
      xnum[ii++]=(char)asciione;
    else {
      for (jj=0;jj<ll;jj++)
        xnum[ii++]=xxnum[jj];
    }
  }
  else {
    if (cc==10) {
      xnum[ii++]=(char)asciione;
      sz--;
    }
    for (jj=0;jj<=sz;jj++)
      xnum[ii++]=xxnum[jj];
    xnum[ii++]=(char)asciidec;
    for (jj=sz+1;jj<ll;jj++)
      xnum[ii++]=xxnum[jj];
  }
  ll=ii;

  if (sgn<0) {
    memmove(&xnum[1],&xnum[0],ll);
    xnum[0]=(char)asciiminus;
    ll++;
  }
  ll=xxsqueeze(xnum,ll);

  if (ll<maxlen) {
    memset(&num[0],0,maxlen);
    memcpy(&num[0],&xnum[0],ll);
  }

fail:
  free(xxnum);
  free(xnum);
  return 0;

}
