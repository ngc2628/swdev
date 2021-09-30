
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

  int ii=0,jj=0,lolcnt=15;
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

  tst_listalloc(&vvloL,sizeof(struct mk_typeid **),lolcnt);
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
  struct mk_typeid *mkobj5=(struct mk_typeid *)malloc(sizeof(struct mk_typeid));
  mkobj5->idd=15;
  mk_stringset(mkobj5->type,"type4");
  struct mk_typeid *mkobj6=(struct mk_typeid *)malloc(sizeof(struct mk_typeid));
  mkobj6->idd=12;
  mk_stringset(mkobj6->type,"type5");
  struct mk_typeid *mkobj7=(struct mk_typeid *)malloc(sizeof(struct mk_typeid));
  mkobj7->idd=10;
  mk_stringset(mkobj7->type,"type4");
  struct mk_typeid *mkobj8=(struct mk_typeid *)malloc(sizeof(struct mk_typeid));
  mkobj8->idd=19;
  mk_stringset(mkobj8->type,"type5");
  
  tst_listsetat(&vvloL,(void*)&mkobj8,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj7,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj6,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj5,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj4,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj3,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj2,vvloL.count,1);
  tst_listsetat(&vvloL,(void*)&mkobj1,vvloL.count,1);
  vvloL.cmp=mk_cmptypeidref;
  tst_listsort(&vvloL);

  struct mk_typeid *mkobj19=0;
  for (ii=0;ii<vvloL.count;ii++) {
    tst_listat(&vvloL,ii,(void *)&mkobj19); 
    mk_stringset(strdbg,0); 
    mk_typeid2string(mkobj19,strdbg);
    printf("%d #%d [%s]\n",__LINE__,ii,&strdbg[0]);
  }

  vvloL.cmp=tst_cmptypeidrefi;
  struct mk_typeid mkobj17;
  mkobj17.idd=15;
  mk_stringset(mkobj17.type,"type4");
  int idxl=tst_listfind(&vvloL,(void *)&mkobj17),idxh=-1;
  if (idxl>=0) {
    tst_listat(&vvloL,idxl,(void *)&mkobj19); 
    mk_stringset(strdbg,0); 
    mk_typeid2string(mkobj19,strdbg);
    printf("%d [%d] [%s]\n",__LINE__,idxl,&strdbg[0]);
  }

  mkobj17.idd=12;
  idxl=idxh=-1;
  ii=tst_binsearchinterval(
    (void *)&mkobj17,sizeof(struct mk_typeid **),
    vvloL.count,vvloL.arr,tst_cmptypeidrefi,&idxl,&idxh,0);
  printf("%d [%d,%d,%d]\n",__LINE__,ii,idxl,idxh); 

  while (vvloL.count>0) {
    tst_listremove(&vvloL,vvloL.count-1,(void *)&mkobj19);
    free(mkobj19);
  }
  tst_listfree(&vvloL);

#if defined (_MSC_VER) || defined (__MINGW32__)
  printf("%d [%I64d]\n",__LINE__,sh133);
#else
  printf("%d [%lld]\n",__LINE__,sh133);
#endif

  return 0;

}

