
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>
#include <app/tst/list/list.h>

int tstlol(struct tst_list *vvloL) {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  int vcnt1=5,vcnt2=3,lolcnt=7;
  struct tst_list *vv1L=(struct tst_list *)malloc(sizeof(struct tst_list));
  struct tst_list *vv2L=(struct tst_list *)malloc(sizeof(struct tst_list));

printf("%d [%p,%p]\n",__LINE__,(void*)vv1L,(void*)vv2L);
  
  mk_vertexnan(vv);
  
  tst_listalloc(vv1L,sizeof(mk_vertex),vcnt1);
  vv[0]=2.7;
  vv[1]=3.;
  vv[2]=11.;
  tst_listsetat(vv1L,(void*)&vv,vv1L->count,1);
  vv[0]=3.7;
  vv[1]=4.;
  vv[2]=15.;
  tst_listsetat(vv1L,(void*)&vv,vv1L->count,1);
  vv[0]=5.7;
  vv[1]=5.;
  vv[2]=14.;
  tst_listsetat(vv1L,(void*)&vv,vv1L->count,1);
  vv[0]=6.7;
  vv[1]=7.;
  vv[2]=16.;
  tst_listsetat(vv1L,(void*)&vv,vv1L->count,1);
  vv[0]=2.3;
  vv[1]=3.;
  vv[2]=17.;
  tst_listsetat(vv1L,(void*)&vv,vv1L->count,1);

  tst_listalloc(vv2L,sizeof(mk_vertex),vcnt2);
  vv[0]=3.3;
  vv[1]=4.;
  vv[2]=12.;
  tst_listsetat(vv2L,(void*)&vv,vv2L->count,1);
  vv[0]=5.3;
  vv[1]=5.;
  vv[2]=13.;
  tst_listsetat(vv2L,(void*)&vv,vv2L->count,1);
  vv[0]=6.3;
  vv[1]=7.;
  vv[2]=10.;
  tst_listsetat(vv2L,(void*)&vv,vv2L->count,1);

  tst_listsetat(vvloL,(void*)&vv1L,vvloL->count,1);
  tst_listsetat(vvloL,(void*)&vv2L,vvloL->count,1);

  return 0;

}

