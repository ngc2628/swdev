
#ifndef _misc_h_
#define _misc_h_

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <mkbase/defs.h>
#include <mkbase/exportdefs.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkutil.h>

namespace mk {

template <class X> inline void swap (X *obj1, X *obj2) {
  if (!obj1 || !obj2)
    return;
  X tmp=*obj1;
  *obj1=*obj2;
  *obj2=tmp;
}

class oswinexp Rounded {

  public:
    double m_d;
    int m_a;
    Rounded(double d=mk_dnan,int a=mk_slimit) : m_d(d),m_a(a) {
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
    int toString(mk_string,char fmt=-1,int pad=-1,const char *dec=".",const char *group=0) const;

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

template <class X> inline void heapsortvc(
  int nn, X arr[], int (*comp)(const void *itm1,const void *itm2)) {
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
      // promote j-pos until it is better than auxillary ...
      if (comp && comp((const void*)(&tmp),(const void*)(&arr[jj]))<0) {  
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

template <class X> inline void heapsortpc(
  int nn, X **arr, int (*comp)(const void *itm1,const void *itm2)) {
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
      // promote j-pos until it is better than auxillary ...
      if (comp((const void *)tmp,(const void *)arr[jj])<0) { 
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

template <class X> inline int binsearchvc(
  const X *x,int cnt,const X arr[],int (*comp)(const void *x1,const void *x2),int guess=-1) {
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

template <class X> inline int binsearchpc(
  const X *x,int cnt,const X **arr,int (*comp)(const void *x1,const void *x2),int guess=-1) {
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

template <class X> inline int binsearchInterval(
  const X *x,int cnt,const X arr[],int *idxl,int *idxh,int sortedrev=0) {

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

template <class X> inline int binsearchInterval(
  const X *x,int cnt,const X arr[],
  int (*comp)(const void *x1,const void *x2),int *idxl,int *idxh,int sortedrev=0) {

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

} // namespace

#endif
