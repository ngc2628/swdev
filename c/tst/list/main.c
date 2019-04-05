
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>
#include <tst/list/list.h>


int usage() {

  printf ("\n");
  return 0;

}

extern int tstlist();
extern int tstlol(struct tst_list *);
extern int tstlolfree(struct tst_list *);

int main(int argc,char **argv) {

  int ii=0,jj=0,lolcnt=7;
  double zero=.0,one=1.;
  mk_string strdbg;
  mk_vertexnan(vv1);
  mk_vertexnan(vv2);
  mk_vertexnan(vv3);
  struct tst_list vvloL;
  tst_listalloc(&vvloL,sizeof(struct tst_list*),lolcnt);
  tstlol(&vvloL);
  struct tst_list cplist;
  tst_listshallowcopy(&cplist,&vvloL);
  tst_listfree(&vvloL);
  
  struct tst_list *chkL=0;
  tst_listat(&cplist,1,(void *)&chkL);
  
  for (ii=0;ii<chkL->count;ii++) {
    tst_listat(chkL,ii,(void*)&vv1);
    printf("%d #%d [%f,%f,%f,%f]\n",__LINE__,ii,vv1[0],vv1[1],vv1[2],vv1[3]);
    mk_vertexmult(vv1,3.3,vv3);
    tst_listsetat(chkL,(void*)&vv3,ii,0);
  }
  vv1[0]=-1.;
  vv1[1]=-2.;
  vv1[2]=-3.;
  tst_listsetat(chkL,(void*)&vv1,chkL->count,1);
  /*tst_listsetat(&cplist,(void*)&chkL,1,0);*/

  tst_listshallowcopy(&vvloL,&cplist);

  tst_listfree(&cplist);

  tst_listat(&vvloL,1,(void *)&chkL);
  
  for (ii=0;ii<chkL->count;ii++) {
    tst_listat(chkL,ii,(void*)&vv1);
    printf("%d #%d [%f,%f,%f,%f]\n",__LINE__,ii,vv1[0],vv1[1],vv1[2],vv1[3]);
  }

  tst_lolfree(&vvloL);
  tst_listfree(&vvloL);

  tst_listalloc(&vvloL,sizeof(struct mk_typeid **),7);
  struct mk_typeid *mkobj1=(struct mk_typeid *)malloc(sizeof(struct mk_typeid));
  mkobj1->idd=8;
  mk_stringset(mkobj1->type,"type1");
  struct mk_typeid *mkobj2=(struct mk_typeid *)malloc(sizeof(struct mk_typeid));
  mkobj2->idd=17;
  mk_stringset(mkobj2->type,"type2");
  struct mk_typeid *mkobj3=(struct mk_typeid *)malloc(sizeof(struct mk_typeid));
  mkobj3->idd=13;
  mk_stringset(mkobj3->type,"type3");
  struct mk_typeid *mkobj4=(struct mk_typeid *)malloc(sizeof(struct mk_typeid));
  mkobj4->idd=11;
  mk_stringset(mkobj4->type,"type2");
  
  tst_listsetat(&vvloL,(void*)&mkobj4,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj3,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj2,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj1,vvloL.count,1);
  vvloL.cmp=mk_cmptypeidref;
  tst_listsort(&vvloL);

  struct mk_typeid *mkobj6=0;
  for (ii=0;ii<4;ii++) {
    tst_listat(&vvloL,ii,(void *)&mkobj6); 
    mk_stringset(strdbg,0); 
    mk_typeid2string(mkobj6,strdbg);
    printf("%d #%d [%s]\n",__LINE__,ii,&strdbg[0]);
  }

  vvloL.cmp=tst_cmptypeidrefi;
  struct mk_typeid mkobj5;
  mkobj5.idd=17;
  mk_stringset(mkobj5.type,"type2");
  int idx=tst_listfind(&vvloL,(void *)&mkobj5);
  if (idx>=0) {
    tst_listat(&vvloL,idx,(void *)&mkobj6); 
    mk_stringset(strdbg,0); 
    mk_typeid2string(mkobj6,strdbg);
    printf("%d [%d] [%s]\n",__LINE__,idx,&strdbg[0]);
  }

  while (vvloL.count>0) {
    tst_listremove(&vvloL,vvloL.count-1,(void *)&mkobj6);
    free(mkobj6);
  }
  tst_listfree(&vvloL);

  return 0;

}

