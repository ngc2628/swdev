
#include <stdlib.h>

#include <mkbase/mkutil.h>
#include <mkbase/mkmath.h>

struct strfi {
  double ff;
  int ii;
};

/* ########## */
static int cmpFI(const void *a1, const void *a2) {

  int ret=0;
  if (!a1 || !a2)
    return ret;
  if (((struct strfi*)a1)->ff>((struct strfi*)a2)->ff)
    ret=1;
  if (((struct strfi*)a1)->ff<((struct strfi*)a2)->ff)
    ret=-1;
  return ret;

}

/* ########## */
void mk_indextab1(int num,double arr[],int indx[]) {

  int jj=0;
  struct strfi *tmp=(struct strfi*)malloc(num*sizeof(struct strfi));
  for (jj=0;jj<num;jj++) {
    tmp[jj].ff=arr[jj];
    tmp[jj].ii=jj;
  }
  qsort((void*)tmp,(size_t)num,sizeof(struct strfi),cmpFI);
  for (jj=0;jj<num;jj++)
    indx[tmp[jj].ii]=jj;
  free(tmp);
  /* or ...
  int ii=0,jj=0,cnt=0;
  char *done=(char *)malloc(num);
  for (ii=0;ii<num;ii++) {
    done[ii]=0;
    indx[ii]=ii;
  }
  for (jj=0;jj<num;jj++) {
    cnt=0;
    for (ii=0;ii<num;ii++) {
      if (ii!=jj && arr[jj]>arr[ii]) 
        cnt++;
    }
    while(done[cnt]==1 && cnt<num) 
      cnt++;
    indx[jj]=cnt;
    done[cnt]=1;
  }
  free(done);
  ... */

}

/* ########## */
void mk_indextab2(int num,double arr[],int indx[]) {

  int ii=0;
  int *tmp=(int*)malloc(num*sizeof(int));
  mk_indextab1(num,arr,tmp);
  for (ii=0;ii<num;ii++)
    indx[tmp[ii]]=ii;
  free(tmp);

}

/* ########## */
int mk_heapsort(int typesize,int cnt,void *arr,int (*comp)(const void *,const void *)) {

  if (typesize<1 || !arr || !comp)
    return 1;
  if (cnt<2)
    return 0;
  int ii=0,jj=0,ub=cnt-1,mb=cnt/2;
  void *tmp=(void*)malloc(typesize);
  while (mb>-1) {
    if (mb==0) {
      memcpy(tmp,arr+ub*typesize,typesize);
      memcpy(arr+ub*typesize,arr,typesize);
      if (--ub==0) {
        memcpy(arr,tmp,typesize);
        break;
      }
    }
    else {
      mb--;
      memcpy(tmp,arr+mb*typesize,typesize);
    }
    ii=mb;
    jj=ii+ii+1;
    while(jj<=ub) {
      if (jj<ub && comp((const void *)(arr+jj*typesize),(const void *)(arr+(jj+1)*typesize))<0)
        jj++;
      if (comp((const void *)tmp,(const void *)(arr+jj*typesize))<0) {
        memcpy(arr+ii*typesize,arr+jj*typesize,typesize);
        ii=jj;
        jj+=(ii+1);
      }
      else
        break;
    }
    memcpy(arr+ii*typesize,tmp,typesize);
  }
  free(tmp);
  return 0;

}

/* ########## */
int mk_binsearch(
  void *xx,int typesize,int cnt,void *arr,int (*comp)(const void *,const void *),int guess) {
  if (!xx || cnt<1 || typesize<1 || !arr || !comp)
    return -1;
  int lb=-1,mb=0,ub=cnt,cmp=0,inc=1;
  if (guess<=0 || guess>=cnt-1) {
    while ((ub-lb>1)) {
      mb=(ub+lb)/2;
      cmp=comp((const void*)(arr+mb*typesize),(const void*)xx);
      if (cmp==0)
        return mb;
      if (cmp<0)
        lb=mb;
      else
        ub=mb;
    }
    return -1;
  }
  /* if a guess is given, look neighbours or narrow the interval */ 
  mb=guess;
  cmp=comp((const void*)(arr+mb*typesize),(const void*)xx);
  if (cmp==0)
    return mb;
  if (cmp<0) {
    mb+=inc;
    while (mb<ub) {
      inc<<=1;
      cmp=comp((const void*)(arr+mb*typesize),(const void*)xx);
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
      cmp=comp((const void*)(arr+mb*typesize),(const void*)xx);
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
    cmp=comp((const void*)(arr+mb*typesize),(const void*)xx);
    if (cmp==0)
      return mb;
    if (cmp<0)
      lb=mb;
    else
      ub=mb;
  }
  return -1;
}

/* ########## */
int mk_binsearchinterval(
  void *xx,int typesize,int cnt,void *arr,int (*comp)(const void *,const void *),
  int *idxl,int *idxh,int sortedrev) {

  if (!xx || cnt<1 || typesize<1 || !arr || !comp)
    return 1;
  int ii=0,ih=cnt,il=-1,cmp=0;
  while ((ih-il)>1) {
    ii=(ih+il)/2;
    cmp=comp((const void*)(arr+ii*typesize),(const void*)xx);
    if (sortedrev>0 ? cmp<0 : cmp>0)
      ih=ii;
    else
      il=ii;
  }
  int ret=0;
  if (ih>=cnt) {
    ih=(cnt==0 ? 0 : cnt-1);
    il=(cnt==0 ? 0 : (cnt>1 ? cnt-2 : cnt-1));
  }
  if (il<0) {
    il=0;
    ih=(cnt>1 ? 1 : 0);
  }
  if (idxl)
    *idxl=il;
  if (idxh)
    *idxh=ih;
  return ret;

}

/* ########## */
int mk_listalloc(struct mk_list *list,int typesize_,int reserve) {

  if (!list || typesize_<1)
    return 0;
  int ii=0,jj=0;
  list->typesize=typesize_;
  list->reserved=(reserve==0 ? 0 : 1);
  while (list->reserved<MAX(reserve,0) && (ii++)<32)
    list->reserved*=2;
  list->count=0;
  list->sorted=0;
  list->cmp=0;
  list->arr=0;
  if (list->reserved>0) {
    list->arr=(void *)malloc(list->reserved*list->typesize);
    memset(list->arr,0,list->reserved*list->typesize);
  }
  return list->reserved;

}

/* ########## */
int mk_listfree(struct mk_list *list) {

  if (!list)
    return 1;
  list->count=0;
  list->sorted=0;
  list->cmp=0;
  if (list->arr)
    free(list->arr);
  list->arr=0;
  return 0;

}

/* ########## */
int mk_listrealloc(struct mk_list *list,int reserve) {

  if (!list)
    return 0;
  int ii=0,len=list->count,sz=list->typesize;
  if (sz==0)
    return 0;
  list->reserved=(reserve>0 ? 1 : 0);
  while (list->reserved<MAX(reserve,0) && (ii++)<32)
    list->reserved*=2;
  void *cparr=0;
  if (list->arr) {
    if (len>0 && list->reserved>0) {
      cparr=(void *)malloc(len*sz);
      memcpy(cparr,list->arr,len*sz);
    }
    free(list->arr);
    list->arr=0;
    list->count=0;
  }
  if (list->reserved>0) {
    list->arr=(void *)malloc(list->reserved*sz);
    memset(list->arr,0,list->reserved*sz);
  }
  if (cparr) {
    list->count=(len<list->reserved ? len : list->reserved);
    if (list->count>0)
      memcpy(list->arr,cparr,list->count*sz); 
    free(cparr);
  }
  return list->reserved;

}

/* ########## */
int mk_listclear(struct mk_list *list,void *itm) {

  if (!list)
    return 1;
  if (list->arr) {
    if (!itm)
      memset(list->arr,0,list->count*list->typesize);
    else {
      int ii=0;
      for (ii=0;ii<list->count;ii++)
        memcpy(list->arr+ii*list->typesize,itm,list->typesize);
    }
  }
  list->count=0;
  list->sorted=0;
  return 0;

}

/* ########## */
int mk_listsort(struct mk_list *list) {

  list->sorted=0;
  if (!list || !list->cmp)
    return 1;
  if (!list->arr || list->count<2 || 
    mk_heapsort(list->typesize,list->count,list->arr,list->cmp)==0)
    list->sorted=1;
  return 0;

}

/* ########## */
int mk_listfind(const struct mk_list *list,void *itm) {

  if (!list || !list->arr || list->count==0 || !itm)
    return -1;
  if ((list->sorted&1)>0)
    return mk_binsearch(itm,list->typesize,list->count,list->arr,list->cmp,0);
  if (list->cmp==0)
    return -1;
  int ii=0;
  for (ii=0;ii<list->count;ii++) {
    if (list->cmp((const void *)(list->arr+ii*list->typesize),(const void *)itm)==0)
      return ii;
  }
  return -1;

}

/* ########## */
int mk_listfindnextindex(const struct mk_list *list,void *itm) {

  if (!list)
    return -1;
  if (!list->arr || list->count==0)
    return 0;
  if (list->sorted==0 || !list->cmp || !itm)
    return list->count;
  int lb=-1,mb=0,mblast=-1,ub=list->count,cmpres=0,sz=list->typesize;
  while ((ub-lb)>1) {
    mb=(ub+lb)/2;
    if (mb==mblast)
      break;
    cmpres=list->cmp((const void *)(list->arr+mb*sz),(const void *)itm);
    if (cmpres==0) {
      if (mb>=(list->count-1))
        return list->count;
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
  return (ub>=list->count ? list->count : ub);

}

/* ########## */
int mk_listat(const struct mk_list *list,int idx,void *itm) {

  if (itm)
    memset(itm,0,list->typesize);
  if (!list || !list->arr || !itm)
    return 1;
  if (idx>=0 && idx<list->count)
    memcpy(itm,list->arr+idx*list->typesize,list->typesize);
  return (idx>=0 && idx<list->count ? 0 : 1);

}

/* ########## */
int mk_listsetat(struct mk_list *list,void *itm,int idx,int insert) {

  if (!list || !list->arr || idx<0 || idx>list->count)
    return 1;
  if (((insert&1)>0 && list->count==list->reserved) || ((insert&1)==0 && list->count==idx))
    return 1;
  int sz=list->typesize;
  if ((insert&1)>0 && idx<list->count) 
    memmove(list->arr+(idx+1)*sz,list->arr+idx*sz,(list->count-idx)*sz);
  if (itm)  
    memcpy(list->arr+idx*sz,itm,sz);
  else
    memset(list->arr+idx*sz,0,sz);
  if ((insert&1)>0)
    list->count++;
  list->sorted=0;
  return 0;

}

/* ########## */
int mk_listappend(struct mk_list *list,void *itm) {

  return (list ? mk_listsetat(list,itm,list->count,1) : 1);

}

/* ########## */
int mk_listprepend(struct mk_list *list,void *itm) {

  return (list ? mk_listsetat(list,itm,0,1) : 1);

}

/* ########## */
int mk_listinsort(struct mk_list *list,void *itm) {

  if (!list || !list->arr || !list->cmp || !itm || list->count==list->reserved)
    return -1;
  if ((list->sorted&1)==0)
    mk_listsort(list);
  int idx=mk_listfindnextindex(list,itm);
  mk_listsetat(list,itm,idx,1);  
  return mk_listfind(list,itm);

}

/* ########## */
int oswinexp mk_listremove(struct mk_list *list,int idx) {

  if (!list || !list->arr || idx<0 || idx>=list->count)
    return 1;
  int sz=list->typesize;
  if (idx<list->count-1)
    memmove(list->arr+idx*sz,list->arr+(idx+1)*sz,(list->count-idx-1)*sz);
  memset(list->arr+(list->count-1)*sz,0,sz);
  list->count--;
  return 0;

}
