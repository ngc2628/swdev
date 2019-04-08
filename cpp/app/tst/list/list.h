
#ifndef _tst_list_h_
#define _tst_list_h_

struct tst_list {
  int typesize,reserved,count,sorted;
  int (*cmp)(const void *,const void *);
  void *arr;
};

struct tst_typeid {
  unsigned int idd;
  mk_string type;
};

int tst_typeid2string(const struct tst_typeid *,mk_string);

int tst_typeid4string(struct tst_typeid *,mk_string);

int tst_cmptypeidtype(const void *,const void *);

int tst_cmptypeid(const void *,const void *);

int tst_cmptypeidref(const void *,const void *);

int tst_cmptypeidrefi(const void *,const void *);

int tst_cmptypeidrefi(const void *,const void *);

class TypeId {

  public:
    struct tst_typeid m_typeid;
    TypeId(const char *name=0,unsigned int idd=0);
    TypeId(const TypeId &);
    virtual ~TypeId();
    TypeId &operator=(const TypeId &);
    virtual int toString(mk_string) const;
    virtual int fromString(const char *);

};

int tst_cmpTypeIdRef(const void *,const void *);

int tst_cmpTypeIdRefiType(const void *,const void *);

class DerTypeId : public TypeId {

  DerTypeId(const DerTypeId &) : TypeId() { }
  DerTypeId &operator=(const DerTypeId &) { return *this; }

  public:
    DerTypeId(const char *name=0,unsigned int idd=0);
    virtual ~DerTypeId();
    struct tst_list vertexL;

};

int tst_heapsortvc(int,void *,int (*)(const void *,const void *));

int tst_heapsort(int,int,void *,int (*)(const void *,const void *));

int tst_binsearch(void *,int,int,void *,int (*)(const void *,const void *),int);

int tst_listalloc(struct tst_list *,int,int);

int tst_listfree(struct tst_list *);

int tst_listsort(struct tst_list *);

int tst_listfind(struct tst_list *,void *);

int tst_listfind2(const struct tst_list *,void *,int *,int *);

int tst_listfindnextindex(struct tst_list *,void *);

int tst_listat(struct tst_list *,int,void *);

int tst_listsetat(struct tst_list *list,void *itm,int idx,int insert);

int tst_listappend(struct tst_list *,void *);

int tst_listprepend(struct tst_list *,void *);

int tst_listinsort(struct tst_list *,void *);

int tst_listremove(struct tst_list *,int,void *);

int tst_listclear(struct tst_list *);

int tst_listshallowcopy(struct tst_list *,struct tst_list *);

int tst_lolfree(struct tst_list *);

class Mgr {

  public:
    tst_list dertypeidL;
    Mgr(int cnt) { 
      tst_listalloc(&dertypeidL,sizeof(TypeId*),cnt);
    }
    ~Mgr() {
      tst_listfree(&dertypeidL); 
    }
    DerTypeId *dertypeid(TypeId);

};

#endif
