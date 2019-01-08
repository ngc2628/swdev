
#ifndef _mkutil_h_
#define _mkutil_h_

#include <mkbase/defs.h>
#include <mkbase/exportdefs.h>

/* list of consecutive items , typesize:mem-size of stored type , reserve:num-max-items, 
   cnt:num-current-items , sorted:0,1 , cmp:item-compare-function , arr:array of items
*/
struct oswinexp mk_list {
  int typesize,reserve,count,sorted;
  int (*cmp)(const void *,const void *);
  void *arr;
};

/* in numberitems , in array , out indexarray */
xtern void oswinexp mk_indextab1(int,double[],int []);

/* in numberitems , in array , out indexarray */
xtern void oswinexp mk_indextab2(int,double [],int []);

/* in numberitems , in mem-size of item , inout array , in cmp-function */
xtern int oswinexp mk_heapsort(int,int,void *,int (*)(const void *,const void *));

/*
  in *look-for , in mem-size of item , in numberitems , in array , 
  in item-compare-function , in guess-likely-index=-1 , return index-in-array
*/
xtern int oswinexp mk_binsearch(void *,int,int,void *,int (*)(const void *,const void *),int);

/* inout list* , in mem-size of item , in number items */
xtern int oswinexp mk_listalloc(struct mk_list *,int,int);

/* inout list* */
xtern int oswinexp mk_listfree(struct mk_list *);

/* inout list* in itm-to-clear-with|0 */
xtern int oswinexp mk_listclear(struct mk_list *,void *);

/* inout list* */
xtern int oswinexp mk_listsort(struct mk_list *);

/* in list* , in item* , return index|-1 */
xtern int oswinexp mk_listfind(struct mk_list *,void *);

/* in list* , in item* , return 0|list-cnt|index */
xtern int oswinexp mk_listfindnextindex(struct mk_list *,void *);

/* in list* , in index , out item-at-index* , return 0,1 */
xtern int oswinexp mk_listat(struct mk_list *,int,void *);

/* inout list* , in itm* , in index , in replace|insert 0|1 , return 0,1 */
int mk_listsetat(struct mk_list *,void *,int,int);

/* inout list* , in new item* , return 0|1  */
xtern int oswinexp mk_listappend(struct mk_list *,void *);

/* inout list* , in new item* , return 0|1 */
xtern int oswinexp mk_listprepend(struct mk_list *,void *);

/* inout list* , in new item* , return new-item-index|-1 */
xtern int oswinexp mk_listinsort(struct mk_list *,void *);

#endif
