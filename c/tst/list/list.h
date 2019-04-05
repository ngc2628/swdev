
#ifndef _tst_list_h_
#define _tst_list_h_

struct tst_list {
  int typesize,reserved,count,sorted;
  int (*cmp)(const void *,const void *);
  void *arr;
};

int tst_heapsortvc(int,void *,int (*)(const void *,const void *));

int tst_heapsort(int,int,void *,int (*)(const void *,const void *));

int tst_binsearch(void *,int,int,void *,int (*)(const void *,const void *),int);

int tst_listalloc(struct tst_list *,int,int);

int tst_listfree(struct tst_list *);

int tst_listsort(struct tst_list *);

int tst_listfind(struct tst_list *,void *);

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

int tst_cmptypeidrefi(const void *,const void *);

#endif
