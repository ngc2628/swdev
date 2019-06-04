
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#if defined (__MACH__)
#include <libkern/OSAtomic.h>
#endif
#if defined (_MSC_VER) || defined (__WATCOMC__) || defined (__BORLANDC__)
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include <time.h>
#include <mkbase/mkutil.h>

extern int mk_ui2str(mk_ulreal,char *,int,int,int,char *);
extern mk_ulreal mk_str2ui(char *,int *,int *,char *);
static int alnumlen=13;
static mk_ulreal mk_globalcnt=0;

/* ########## */
mk_ulreal mk_nextcnt() {
#if defined (__MACH__)
  OSAtomicAdd32(1,&globalcnt);
#else
  mk_globalcnt++;
#endif
  return mk_globalcnt;
}

/* ########## */
mk_ulreal mk_nextt() {

  mk_ulreal tt=0;

#if defined (_MSC_VER)
  struct __timeb64 tsec;
  _ftime64(&tsec);
  tt=(mk_ulreal)1000000*tsec.time+tsec.millitm;
#elif defined (__WATCOMC__) || defined (__BORLANDC__)
  struct timeb tsec;
  ftime(&tsec);
  tt=(mk_ulreal)1000000*tsec.time+tsec.millitm;
#else
  struct timezone tzp;
  struct timeval tsec;
  gettimeofday(&tsec,&tzp);
  tt=(mk_ulreal)1000000*tsec.tv_sec+tsec.tv_usec;
#endif
  return tt;

}

/* ########## */
int mk_stringlength(const mk_string mkstr) {

  return MIN(mk_sz,MAX(0,(int)strlen(&mkstr[0])));

}

/* ########## */
int mk_stringset(mk_string mkstr,const char *str) {

  memset(&mkstr[0],0,mk_sz+1);
  if (str && strlen(str)>0)
    strncpy(&mkstr[0],str,mk_sz);
  return mk_stringlength(mkstr);

}

/* ########## */
int mk_stringcmp(const void *mkstr1,const void *mkstr2) {

  const mk_string *m1=(const mk_string*)mkstr1;
  const mk_string *m2=(const mk_string*)mkstr2;
  int cmp=strcmp(&((*m1)[0]),&((*m2)[0]));
  return (cmp<0 ? -1 : (0<cmp ? 1 : 0));

}

/* ########## */
int mk_stringsetlength(mk_string mkstr,int length) {

  length=MIN(mk_sz,MAX(0,length));
  memset(&mkstr[length],0,mk_sz-length);
  return mk_stringlength(mkstr);

}

/* ########## */
int mk_stringappend(mk_string mkstr,const char *str) {

  int ii=0,l1=mk_stringlength(mkstr),l2=(str ? strlen(str) : 0);
  for (ii=0;ii<l2 && l1+ii<mk_sz;ii++)
    mkstr[l1+ii]=str[ii];
  return mk_stringlength(mkstr);

}

/* ########## */
int mk_stringprepend(mk_string mkstr,const char *str) {

  int ii=0,l2=MIN(mk_sz,(str ? strlen(str) : 0));
  if (l2>0 && l2<mk_sz)
    memmove(&mkstr[l2],&mkstr[0],mk_sz-l2);
  for (ii=0;ii<l2;ii++)
    mkstr[ii]=str[ii]; 
  return mk_stringlength(mkstr);

}

/* ########## */
int mk_stringsetat(mk_string mkstr,int idx,char cc) {

  if (cc==0)
    return mk_stringsetlength(mkstr,idx);
  int ll=mk_stringlength(mkstr);
  if (idx<=0) {
    if (ll<mk_sz) {
      memmove(&mkstr[1],&mkstr[0],mk_sz-1);
      mkstr[0]=cc;
    }
  }
  else if (idx>=mk_sz) {
    if (ll<mk_sz) 
      mkstr[ll]=cc;
  }
  else
    mkstr[idx]=cc;  
  return mk_stringlength(mkstr);

}

/* ########## */
int mk_stringsubstr(const mk_string mkstr,int idx,int cnt,mk_string mksubstr) {

  mk_stringset(mksubstr,0);
  int ii=0,ll=mk_stringlength(mkstr);
  if (idx<0)
    idx=ll-idx;
  if (idx>=ll || idx<0 || cnt<=0)
    return 0;
  if (idx+cnt>=ll)
    cnt=ll-idx;
  for (ii=0;ii<cnt;ii++)
    mksubstr[ii]=mkstr[idx+ii];
  return mk_stringlength(mksubstr);

}

/* ########## */
int mk_stringfind(const mk_string mkstr,char cc,int idx,unsigned char dir,unsigned char cs) {
 
   char cci=0;
   int ll=mk_stringlength(mkstr),ss=(idx<0 ? ll+idx : idx),ee=(dir=='b' ? -1 : ll);
   if (ll==0 || ss<0 || ss>=ll)
     return -1;
   if (cs=='i')
     cc=(char)tolower(cc);
   do {
     cci=(cs=='i' ? tolower(mkstr[ss]) : mkstr[ss]);
     if (cc==cci)
       return ss;
     ss+=(dir=='b' ? -1 : 1);
   } while (ss!=ee);
   return -1;
 
}

/* ########## */
int mk_cmptypeidtype(const void *tid1,const void *tid2) {

  const struct mk_typeid *typeid1=(const struct mk_typeid *)tid1;
  const struct mk_typeid *typeid2=(const struct mk_typeid *)tid2;
  int typecmp=strcmp(typeid1->type,typeid2->type);
  if (typecmp<0)
    typecmp=-1;
  else if (0<typecmp)
    typecmp=1;
  return typecmp;

}

/* ########## */
int mk_cmptypeid(const void *tid1,const void *tid2) {

  int typecmp=mk_cmptypeidtype(tid1,tid2);
  if (typecmp!=0)
    return typecmp;
  const struct mk_typeid *typeid1=(const struct mk_typeid *)tid1;
  const struct mk_typeid *typeid2=(const struct mk_typeid *)tid2;
  typecmp=(typeid1->idd<typeid2->idd ? -1 : (typeid2->idd<typeid1->idd ? 1 : 0));
  return typecmp;

}

/* ########## */
int mk_cmptypeidref(const void *tid1,const void *tid2) {

  const struct mk_typeid **typeid1=(const struct mk_typeid**)tid1;
  const struct mk_typeid **typeid2=(const struct mk_typeid**)tid2;
  int typecmp=mk_cmptypeidtype((const void*)(*typeid1),(const void*)(*typeid2));
  if (typecmp!=0)
    return typecmp;
  typecmp=((*typeid1)->idd<(*typeid2)->idd ? -1 : ((*typeid2)->idd<(*typeid1)->idd ? 1 : 0));  
  return typecmp;

}

/* ########## */
int mk_cmptypeidrefi(const void *tid1,const void *tid2) {

  const struct mk_typeid **typeid1=(const struct mk_typeid**)tid1;
  const struct mk_typeid *typeid2=(const struct mk_typeid*)tid2;
  int typecmp=mk_cmptypeidtype((const void*)(*typeid1),(const void*)typeid2);
  if (typecmp!=0)
    return typecmp;
  typecmp=((*typeid1)->idd<typeid2->idd ? -1 : (typeid2->idd<(*typeid1)->idd ? 1 : 0));  
  return typecmp;

}

/* ########## */
int mk_typeid2string(const struct mk_typeid *tid,mk_string str) {

  mk_stringset(str,&tid->type[0]);
  if (mk_stringlength(str)==0)
    return 1;
  mk_string numstr;
  mk_ui2str(tid->idd,numstr,mk_maxintbase,alnumlen,1,0);  
  mk_stringappend(str,&numstr[0]);
  return 0;  

}

/* ########## */
int mk_typeid4string(struct mk_typeid *tid,mk_string str) {

  if (!tid)
    return 1;
  memset(tid,0,sizeof(struct mk_typeid));
  int ii=0,jj=0,base=-1,len=mk_stringlength(str);
  if (len<=alnumlen)
    return 1;
  mk_string stridd;
  mk_stringsubstr(str,len-alnumlen,mk_sz,stridd);
  tid->idd=mk_str2ui(stridd,&base,0,0);
  if (base<0)
    return 1;
  mk_stringsubstr(str,0,len-alnumlen,tid->type);
  return 0;

}

/* ########## */
static int mk_cmpif(const void *if1, const void *if2) {

  if (((struct mk_if*)if1)->ff<((struct mk_if*)if2)->ff)
    return -1;
  if (((struct mk_if*)if2)->ff<((struct mk_if*)if1)->ff)
    return 1;
  return 0;

}

/* ########## */
void mk_indextab1(int num,double arr[],int indx[]) {

  int jj=0;
  struct mk_if *tmp=(struct mk_if*)malloc(num*sizeof(struct mk_if));
  for (jj=0;jj<num;jj++) {
    tmp[jj].ii=jj;
    tmp[jj].ff=arr[jj];
  }
  qsort((void*)tmp,(size_t)num,sizeof(struct mk_if),mk_cmpif);
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
      memcpy(tmp,(const void*)((char*)arr+ub*typesize),typesize);
      memcpy((void*)((char*)arr+ub*typesize),arr,typesize);
      if (--ub==0) {
        memcpy(arr,tmp,typesize);
        break;
      }
    }
    else {
      mb--;
      memcpy(tmp,(const void*)((char*)arr+mb*typesize),typesize);
    }
    ii=mb;
    jj=ii+ii+1;
    while(jj<=ub) {
      if (jj<ub && 
          comp((const void*)((char*)arr+jj*typesize),(const void*)((char*)arr+(jj+1)*typesize))<0)
        jj++;
      if (comp((const void*)tmp,(const void*)((char*)arr+jj*typesize))<0) {
        memcpy((void*)((char*)arr+ii*typesize),(const void*)((char*)arr+jj*typesize),typesize);
        ii=jj;
        jj+=(ii+1);
      }
      else
        break;
    }
    memcpy((void*)((char*)arr+ii*typesize),tmp,typesize);
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
      cmp=comp((const void*)((char*)arr+mb*typesize),(const void*)xx);
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
  cmp=comp((const void*)((char*)arr+mb*typesize),(const void*)xx);
  if (cmp==0)
    return mb;
  if (cmp<0) {
    mb+=inc;
    while (mb<ub) {
      inc<<=1;
      cmp=comp((const void*)((char*)arr+mb*typesize),(const void*)xx);
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
      cmp=comp((const void*)((char*)arr+mb*typesize),(const void*)xx);
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
    cmp=comp((const void*)((char*)arr+mb*typesize),(const void*)xx);
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
  int ii=0,ih=cnt,il=-1,cmp=0,res=0;
  while ((ih-il)>1) {
    ii=(ih+il)/2;
    cmp=comp((const void*)((char*)arr+ii*typesize),(const void*)xx);
    if (sortedrev>0 ? cmp<0 : cmp>0)
      ih=ii;
    else
      il=ii;
  }
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
  return res;

}

/* ########## */
int mk_listalloc(struct mk_list *list,int typesize_,int reserve) {

  if (!list || typesize_<1)
    return 0;
  int ii=0,jj=0;
  list->typesize=typesize_;
  list->reserved=(reserve<=0 ? 0 : 1);
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
        memcpy((void*)((char*)list->arr+ii*list->typesize),itm,list->typesize);
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
int mk_listfind(const struct mk_list *list,void *itm,int *idxl,int *idxh) {

  if (idxl)
    *idxl=-1;
  if (idxh)
    *idxh=-1;
  if (!list || !list->arr || list->count==0 || list->cmp==0 || !itm)
    return 0;
  int ii=0,il=-1,ih=-1,cntitm=0;
  if ((list->sorted&1)==0) {
    for (ii=0;ii<list->count;ii++) {
      if (list->cmp((const void*)((char*)list->arr+ii*list->typesize),(const void*)itm)==0) {
        if (il<0)
          il=ii;
        else
          ih=ii;
        cntitm++;
      }
    }
  }
  else {
    ih=il=mk_binsearch(itm,list->typesize,list->count,list->arr,list->cmp,0);
    if (ih>=0) {
      for (ii=il-1;ii>-1;ii--) {
        if (list->cmp((const void*)((char*)list->arr+ii*list->typesize),(const void*)itm)!=0) 
          break;
        il=ii;
      }
      for (ii=ih+1;ii<list->count;ii++) {
        if (list->cmp((const void*)((char*)list->arr+ii*list->typesize),(const void*)itm)!=0) 
          break;
        ih=ii;
      }
      cntitm=ih-il+1;
    }
  }
  if (idxl)
    *idxl=il;
  if (idxh)
    *idxh=(ih<0 && cntitm>0 ? il : ih);
  return cntitm;

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
    cmpres=list->cmp((const void*)((char*)list->arr+mb*sz),(const void*)itm);
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
    memcpy(itm,(const void*)((char*)list->arr+idx*list->typesize),list->typesize);
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
    memmove((void*)((char*)list->arr+(idx+1)*sz),
            (const void*)((char*)list->arr+idx*sz),
            (list->count-idx)*sz);
  if (itm)  
    memcpy((void*)((char*)list->arr+idx*sz),itm,sz);
  else
    memset((void*)((char*)list->arr+idx*sz),0,sz);
  if ((insert&1)>0) {
    list->count++;
    list->sorted=0;
  }
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
  int idxl=mk_listfindnextindex(list,itm),idxh=-1;
  mk_listsetat(list,itm,idxl,1); 
  list->sorted=1;
  mk_listfind(list,itm,&idxl,&idxh);
  return idxl;

}

/* ########## */
int mk_listremove(struct mk_list *list,int idx,void *itm) {

  mk_listat(list,idx,itm);
  if (!list || !list->arr || idx<0 || idx>=list->count)
    return 1;
  int sz=list->typesize;
  if (idx<list->count-1)
    memmove((void*)((char*)list->arr+idx*sz),
            (const void*)((char*)list->arr+(idx+1)*sz),
            (list->count-idx-1)*sz);
  memset((void*)((char*)list->arr+(list->count-1)*sz),0,sz);
  list->count--;
  return 0;

}

/* ########## */
int mk_listpush(struct mk_list *list,void *itm) {

  return mk_listappend(list,itm);

}

/* ########## */
int mk_listpop(struct mk_list *list,void *itm) {

  return mk_listremove(list,list ? list->count-1 : -1,itm);

}

/* ########## */
int mk_listcopy(struct mk_list *dest,const struct mk_list *src) {

  if (!src || !dest)
    return 1;
  mk_listfree(dest);
  dest->typesize=src->typesize;
  dest->reserved=(src->reserved<4 ? src->reserved : src->reserved-1);
  dest->count=src->count;
  dest->sorted=src->sorted;
  dest->cmp=src->cmp;
  dest->arr=0;
  if (dest->reserved>0)
    mk_listalloc(dest,dest->typesize,dest->reserved);
  void *itm=(void*)malloc(dest->typesize);
  int ii=0;
  for (ii=0;ii<src->count;ii++) {
    mk_listat(src,ii,itm);
    mk_listappend(dest,itm);
  }
  free(itm);
  return 0;

}


