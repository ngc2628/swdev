
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>
#include <mkbase/mkconv.h>
#include <app/tst/list/list.h>

static int alnumlen=13;

int tst_typeid2string(const struct tst_typeid *tid,mk_string str) {

  mk_stringset(str,&tid->type[0]);
  if (mk_stringlength(str)==0)
    return 1;
  mk_string numstr;
  mk_ui2str(tid->idd,numstr,mk_maxintbase,alnumlen,1,0);  
  mk_stringappend(str,&numstr[0]);
  return 0;  

}

/* ########## */
int tst_typeid4string(struct tst_typeid *tid,mk_string str) {

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

int tst_cmptypeidtype(const void *tid1,const void *tid2) {

  const struct tst_typeid *typeid1=(const struct tst_typeid *)tid1;
  const struct tst_typeid *typeid2=(const struct tst_typeid *)tid2;
  int typecmp=strcmp(typeid1->type,typeid2->type);
  if (typecmp<0)
    typecmp=-1;
  else if (0<typecmp)
    typecmp=1;
  return typecmp;

}

int tst_cmptypeid(const void *tid1,const void *tid2) {

  int typecmp=tst_cmptypeidtype(tid1,tid2);
  if (typecmp!=0)
    return typecmp;
  const struct tst_typeid *typeid1=(const struct tst_typeid *)tid1;
  const struct tst_typeid *typeid2=(const struct tst_typeid *)tid2;
  typecmp=(typeid1->idd<typeid2->idd ? -1 : (typeid2->idd<typeid1->idd ? 1 : 0));
  return typecmp;

}

int tst_cmptypeidref(const void *tid1,const void *tid2) {

  const struct tst_typeid **typeid1=(const struct tst_typeid**)tid1;
  const struct tst_typeid **typeid2=(const struct tst_typeid**)tid2;
  int typecmp=tst_cmptypeidtype((const void*)(*typeid1),(const void*)(*typeid2));
  if (typecmp!=0)
    return typecmp;
  typecmp=((*typeid1)->idd<(*typeid2)->idd ? -1 : ((*typeid2)->idd<(*typeid1)->idd ? 1 : 0));  
  return typecmp;

}

int tst_cmptypeidrefi(const void *tid1,const void *tid2) {

  const struct tst_typeid **typeid1=(const struct tst_typeid**)tid1;
  const struct tst_typeid *typeid2=(const struct tst_typeid*)tid2;
  int typecmp=mk_cmptypeidtype((const void*)(*typeid1),(const void*)typeid2);
  if (typecmp!=0)
    return typecmp;
  typecmp=((*typeid1)->idd<typeid2->idd ? -1 : (typeid2->idd<(*typeid1)->idd ? 1 : 0));  
  return typecmp;

}

int tst_heapsortvc(
  int nn, void *arr_, int (*comp)(const void *itm1,const void *itm2)) {
  if (nn<2 || !arr_ || !comp)
    return 1;
  int ii=0,jj=0,ub=nn-1,mb=nn/2;
  char *tmp1=(char*)malloc(8);
  char *tmp2=(char*)malloc(8);
  char *tmp3=(char*)malloc(8);
  char *arr=(char*)arr_;
  while (mb>-1) {
    if (mb==0) {
      memcpy(tmp1,arr+8*ub,8);
      memcpy(arr+8*ub,arr,8);
      if (--ub==0) {
        memcpy(arr,tmp1,8);
        break;
      }
    }
    else {
      mb--;
      memcpy(tmp1,arr+8*mb,8);
    }
    ii=mb;jj=ii+ii+1;
    while(jj<=ub) {
      if (jj<ub) {
        memcpy(tmp2,arr+8*jj,8);
        memcpy(tmp3,arr+8*(jj+1),8);
        if (comp && comp((const void*)tmp2,(const void*)tmp3)<0)
          jj++;
      }
      memcpy(tmp2,arr+8*jj,8);
      if (comp && comp((const void*)(tmp1),(const void*)tmp2)<0) {
        memcpy(arr+8*ii,arr+8*jj,8);
        ii=jj;
        jj+=(ii+1);
      }
      else
        break; 
    }
    memcpy(arr+8*ii,tmp1,8);
  }
  return 0;
}

int tst_heapsort(int cnt,int typesize,void *arr_,int (*comp)(const void *,const void *)) {

  if (cnt<2 || typesize<1 || !arr_ || !comp)
    return 1;
  int ii=0,jj=0,ub=cnt-1,mb=cnt/2;
  char *arr=(char*)arr_;
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
int tst_binsearch(
  void *xx,int typesize,int cnt,void *arr_,int (*comp)(const void *,const void *),int guess) {
  if (!xx || cnt<1 || typesize<1 || !arr_ || !comp)
    return -1;
  char *arr=(char*)arr_;
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
int tst_listalloc(struct tst_list *list,int typesize_,int reserved_) {

  if (!list || typesize_<=0)
    return 0;
  int ii=0,jj=0;
  list->typesize=typesize_;
  list->reserved=1;
  while (list->reserved<MAX(reserved_,1) && (ii++)<32)
    list->reserved*=2;
  list->count=0;
  list->sorted=0;
  list->cmp=0;
  list->arr=(void *)malloc(list->reserved*list->typesize);
  memset(list->arr,0,list->reserved*list->typesize);
  return list->reserved;

}

/* ########## */
int tst_listfree(struct tst_list *list) {

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
int tst_listsort(struct tst_list *list) {

  int res=0;
  if (list && list->arr && list->count>1 && list->cmp) {
    tst_heapsort(list->count,list->typesize,list->arr,list->cmp);
    list->sorted=1;
  }
  else
    res=1;
  return res;

}

/* ########## */
int tst_listfindnextindex(struct tst_list *list,void *itm) {

  if (!list || !list->arr)
    return -1;
  if (list->count==0)
    return 0;
  if (list->sorted==0 || !itm)
    return list->count;
  int lb=-1,mb=0,mblast=-1,ub=list->count,cmpres=0,sz=list->typesize;
  while ((ub-lb)>1) {
    mb=(ub+lb)/2;
    if (mb==mblast)
      break;
    cmpres=list->cmp((const void *)((char*)(list->arr)+mb*sz),(const void *)itm);
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
int tst_listfind(struct tst_list *list,void *itm) {

  if (!list || !list->arr || list->count==0 || !itm)
    return -1;
  if ((list->sorted&1)>0)
    return tst_binsearch(itm,list->count,list->typesize,list->arr,list->cmp,0);
  if (list->cmp==0)
    return -1;
  int ii=0;
  for (ii=0;ii<list->count;ii++) {
    if (list->cmp((const void *)((char*)(list->arr)+ii*list->typesize),(const void *)itm)==0)
      return ii;
  }
  return -1;

}

/* ########## */
int tst_listfind2(const struct tst_list *list,void *itm,int *idxl,int *idxh) {

  if (idxl)
    *idxl=-1;
  if (idxh)
    *idxh=-1;
  if (!list || !list->arr || list->count==0 || list->cmp==0 || !itm)
    return 0;
  int ii=0,il=-1,ih=-1,cntitm=0;
  if ((list->sorted&1)==0) {
    for (ii=0;ii<list->count;ii++) {
      if (list->cmp((const void *)((char*)(list->arr)+ii*list->typesize),(const void *)itm)==0) {
        if (il<0)
          il=ii;
        else
          ih=ii;
        cntitm++;
      }
    }
  }
  else {
    ih=il=tst_binsearch(itm,list->typesize,list->count,list->arr,list->cmp,0);
    if (ih>=0) {
      for (ii=il-1;ii>-1;ii--) {
        if (list->cmp((const void *)((char*)(list->arr)+ii*list->typesize),(const void *)itm)!=0) 
          break;
        il=ii;
      }
      for (ii=ih+1;ii<list->count;ii++) {
        if (list->cmp((const void *)((char*)(list->arr)+ii*list->typesize),(const void *)itm)!=0) 
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
int tst_listat(struct tst_list *list,int idx,void *itm) {

  if (!list || !list->arr || idx<0 || idx>=list->count || !itm)
    return 1;
  memcpy(itm,(const void*)((char*)(list->arr)+idx*list->typesize),list->typesize);
  return 0;

}

/* ########## */
int tst_listsetat(struct tst_list *list,void *itm,int idx,int insert) {

  if (!list || !list->arr || idx<0 || idx>list->count || 
       (list->count==list->reserved && (insert&1)>0))
    return 1;
  int sz=list->typesize;
  if ((insert&1)>0 && idx<list->count) 
    memmove((void*)((char*)(list->arr)+(idx+1)*sz),
            (const void*)((char*)(list->arr)+idx*sz),(list->count-idx)*sz);
  if (itm)  
    memcpy((void*)((char*)(list->arr)+idx*sz),itm,sz);
  else
    memset((void*)((char*)(list->arr)+idx*sz),0,sz);
  if ((insert&1)>0)
    list->count++;
  list->sorted=0;



  return 0;

}

/* ########## */
int tst_listappend(struct tst_list *list,void *itm) {

  return (list ? tst_listsetat(list,itm,list->count,1) : 1);

}

/* ########## */
int tst_listprepend(struct tst_list *list,void *itm) {

  return (list ? tst_listsetat(list,itm,0,1) : 1);

}

/* ########## */
int tst_listinsort(struct tst_list *list,void *itm) {

  if (!list || !list->arr || !list->cmp || !itm || list->count==list->reserved)
    return -1;
  if ((list->sorted&1)==0)
    tst_listsort(list);
  int idx=tst_listfindnextindex(list,itm);
  tst_listsetat(list,itm,idx,1);  
  return tst_listfind(list,itm);

}

/* ########## */
int tst_listremove(struct tst_list *list,int idx,void *itm) {

  tst_listat(list,idx,itm);
  if (!list || !list->arr || idx<0 || idx>=list->count)
    return 1;
  int sz=list->typesize;
  if (idx<list->count-1)
    memmove((void*)((char*)(list->arr)+idx*sz),
            (const void*)((char*)(list->arr)+(idx+1)*sz),(list->count-idx-1)*sz);
  memset((void*)((char*)(list->arr)+(list->count-1)*sz),0,sz);
  list->count--;
  return 0;

}

/* ########## */
int tst_listclear(struct tst_list *list) {

  if (list->reserved>0)
    memset(list->arr,0,list->reserved);
  list->count=0;
  return 0;

}

/* ########## */
int tst_listshallowcopy(struct tst_list *cplist,struct tst_list *list) {

  if (!list || !cplist)
    return 1;
  cplist->typesize=list->typesize;
  cplist->reserved=(list->reserved<2 ? list->reserved : list->reserved-1);
  cplist->count=list->count;
  cplist->sorted=list->sorted;
  cplist->cmp=list->cmp;
  cplist->arr=0;
  if (cplist->reserved>0)
    tst_listalloc(cplist,cplist->typesize,cplist->reserved);
  void *cpitm=(void*)malloc(cplist->typesize);
  int ii=0;
  for (ii=0;ii<list->count;ii++) {
    tst_listat(list,ii,cpitm);
    tst_listappend(cplist,cpitm);
  }
  free(cpitm);
  return 0;

}

/* ########## */
int tst_lolfree(struct tst_list *list) {

  struct tst_list *rmL=0;
  while (list->count>0) {
    tst_listremove(list,list->count-1,(void *)&rmL);
    tst_listfree(rmL);
    free(rmL);
  }
  return 0;

}

TypeId::TypeId(const char *name,unsigned int idd)  {

  memset(&m_typeid,0,sizeof(struct tst_typeid));
  mk_stringset(m_typeid.type,name);
  m_typeid.idd=(idd==0 ? mk_nextcnt() : idd);

}

TypeId::TypeId(const TypeId &ass) {

  if (&ass!=this) {
    memcpy(&m_typeid,&ass.m_typeid,sizeof(struct tst_typeid));
  }

}

TypeId::~TypeId() {

}

TypeId &TypeId::operator=(const TypeId &ass) {

  if (this!=&ass) {
    memcpy(&m_typeid,&ass.m_typeid,sizeof(struct tst_typeid));
  }
  return *this;

}

int TypeId::toString(mk_string str) const {

  tst_typeid2string(&m_typeid,str);
  return 0;

}

int TypeId::fromString(const char *ss) {

  mk_string str;
  mk_stringset(str,ss);
  return tst_typeid4string(&m_typeid,str);

}

int tst_cmpTypeIdRef(const void *t1,const void *t2) {

  const TypeId **tt1=(const TypeId**)t1;
  const TypeId **tt2=(const TypeId**)t2;
//printf("%d  %p %p %s\n",__LINE__,tt1,tt2,&((*tt1)->m_typeid).type[0]);
  return tst_cmptypeid((const void *)&(*tt1)->m_typeid,(const void *)&(*tt2)->m_typeid);

}

int tst_cmpTypeIdRefiType(const void *t1,const void *t2) {

  const TypeId **tt1=(const TypeId**)t1;
  const TypeId *tt2=(const TypeId*)t2;
  return tst_cmptypeidtype((const void *)&((*tt1)->m_typeid),(const void *)&tt2->m_typeid);

}

DerTypeId::DerTypeId(const char *type,unsigned int idd) : 
  TypeId(type,idd) {

  tst_listalloc(&vertexL,sizeof(mk_vertex),17);

}

DerTypeId::~DerTypeId() {

  //printf("%d dtor\n",__LINE__);
  tst_listfree(&vertexL);

}

DerTypeId *Mgr::dertypeid(TypeId typeidd_) {

  TypeId typeidd(typeidd_);
  TypeId *res=0;
  int idxl=-1;
  dertypeidL.cmp=tst_cmpTypeIdRefiType;
  tst_listfind2(&dertypeidL,(void*)&typeidd,&idxl,0);
  dertypeidL.cmp=tst_cmpTypeIdRef;
  if (idxl>=0)
    tst_listat(&dertypeidL,idxl,(void*)&res);
  return (DerTypeId*)res;
 
}




