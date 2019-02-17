
#ifndef _mkutil_h_
#define _mkutil_h_

#include <mkbase/defs.h>
#include <mkbase/exportdefs.h>

/* int-pair */
struct oswinexp mk_ij {
  int ii,jj;
};

/* int-float-pair */
struct oswinexp mk_if {
  int ii;
  double ff;
};

typedef char mk_string[mk_sz+1];

/* id-of-item , idd:unique-id , type:unique-type */
struct oswinexp mk_typeid {
  mk_ulreal idd;
  mk_string type;
};

/* list of consecutive items , typesize:mem-size of stored type , reserve:num-max-items, 
   cnt:num-current-items , sorted:0,1 , cmp:item-compare-function , arr:array of items
*/
struct oswinexp mk_list {
  int typesize,reserved,count,sorted;
  int (*cmp)(const void *,const void *);
  void *arr;
};

#ifdef __cplusplus
/* #pragma message "cplusplus" */
extern "C" {
#endif

/* return global-cnt */
xtern mk_ulreal oswinexp mk_nextcnt();

/* return time-cnt */
xtern mk_ulreal oswinexp mk_nextt();

/* in string , in string* , return length */
xtern int oswinexp mk_stringset(mk_string,const char *);

/* in string* , in string* , return -1|0|1 */
xtern int oswinexp mk_stringcmp(const void *,const void *);

/* in string* , return length */
xtern int oswinexp mk_stringlength(const mk_string);

/* in string* , int new-length , return length */
xtern int oswinexp mk_stringsetlength(mk_string,int);

/* in string , in string* , return length */
xtern int oswinexp mk_stringappend(mk_string,const char *);

/* in string , in string* , return length */
xtern int oswinexp mk_stringprepend(mk_string,const char *);

/* in string , in index , in letter , return 0|1 */
xtern int oswinexp mk_stringsetat(mk_string,int,char);

/* in string , in index-start , in length , inout substring , return substr-length */
xtern int oswinexp mk_stringsubstr(const mk_string,int,int,mk_string);

/* in string , in char-to-find , in from-index , 
   in direction=fwd|bwd , in case-sensitivity , return index 
*/
xtern int oswinexp mk_stringfind(const mk_string,char,int,unsigned char,unsigned char); 

/* in typeid* , in typeid* , return -1|0|1 */
xtern int oswinexp mk_cmptypeid(const void *,const void *);

/* in typeid* , in typeid* , return -1|0|1 */
xtern int oswinexp mk_cmptypeidtype(const void *,const void *);

/* in typeid* , inout string , return 0|1 */
xtern int oswinexp mk_typeid2string(const struct mk_typeid *,mk_string);

/* inout typeid* , in string , return 0|1 */
xtern int oswinexp mk_typeid4string(struct mk_typeid *,mk_string);

/* in numberitems , in array , out indexarray */
xtern void oswinexp mk_indextab1(int,double[],int []);

/* in numberitems , in array , out indexarray */
xtern void oswinexp mk_indextab2(int,double [],int []);

/* in numberitems , in mem-size of item , inout array , in cmp-function */
xtern int oswinexp mk_heapsort(int,int,void *,int (*)(const void *,const void *));

/*
  in *look-for , in mem-size of item , in numberitems , in array , 
  in item-compare-function , in guess-likely-index -1=none , return index-in-array
*/
xtern int oswinexp mk_binsearch(void *,int,int,void *,int (*)(const void *,const void *),int);

/*
  in *look-for , in mem-size of item , in numberitems , in array , in item-compare-function , 
  out left-index , out right-index , in sortedreverse 0|1 , return 0|1
*/
xtern int oswinexp mk_binsearchinterval(
  void *,int,int,void *,int (*)(const void *,const void *),int *,int *,int);

/* inout list* , in mem-size of item , in number items */
xtern int oswinexp mk_listalloc(struct mk_list *,int,int);

/* inout list* */
xtern int oswinexp mk_listfree(struct mk_list *);

/* inout list* , in new size* , return new size */
xtern int oswinexp mk_listrealloc(struct mk_list *,int);

/* inout list* in itm-to-clear-with|0 */
xtern int oswinexp mk_listclear(struct mk_list *,void *);

/* inout list* */
xtern int oswinexp mk_listsort(struct mk_list *);

/* in list* , in item* , out index-left* , out index-right* , return cnt-items */
xtern int oswinexp mk_listfind(const struct mk_list *,void *,int *,int *);

/* in list* , in item* , return 0|list-cnt|index */
xtern int oswinexp mk_listfindnextindex(const struct mk_list *,void *);

/* in list* , in index , out item-at-index* , return 0,1 */
xtern int oswinexp mk_listat(const struct mk_list *,int,void *);

/* inout list* , in itm* , in index , in replace|insert 0|1 , return 0,1 */
xtern int oswinexp mk_listsetat(struct mk_list *,void *,int,int);

/* inout list* , in new item* , return 0|1  */
xtern int oswinexp mk_listappend(struct mk_list *,void *);

/* inout list* , in new item* , return 0|1 */
xtern int oswinexp mk_listprepend(struct mk_list *,void *);

/* inout list* , in new item* , return new-item-index|-1 */
xtern int oswinexp mk_listinsort(struct mk_list *,void *);

/* inout list* , in index , out item-at-index* , return 0|1 */
xtern int oswinexp mk_listremove(struct mk_list *,int,void *);

/* inout list* , in new item* , return 0|1 */
xtern int oswinexp mk_listpush(struct mk_list *,void *);

/* inout list* , out item* , return 0|1 */
xtern int oswinexp mk_listpop(struct mk_list *,void *);

#ifdef __cplusplus
}
#endif

#endif
