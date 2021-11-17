
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>
#include <app/tst/list/list.h>

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
  mk_vertexnan(vv);
  struct tst_list vvloL;
  tst_listalloc(&vvloL,sizeof(struct tst_list*),lolcnt);
  tstlol(&vvloL);

  struct tst_list *chkL=0;
  tst_listat(&vvloL,1,(void *)&chkL);
  for (ii=0;ii<chkL->count;ii++) {
    tst_listat(chkL,ii,(void*)&vv);
    printf("%d #%d [%f,%f,%f,%f]\n",__LINE__,ii,vv[0],vv[1],vv[2],vv[3]);
  }

  tst_lolfree(&vvloL,1);

  return 0;

}

