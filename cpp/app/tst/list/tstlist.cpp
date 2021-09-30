
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>
#include <app/tst/list/list.h>


int tstlist(struct tst_list *dertypeL) {

  int ii=0,jj=0;
  time_t tt=0;
  double zero=.0,one=1.;
  char buf[16];
  memset(&buf[0],0,16);

  mk_vertexnan(vv);
  DerTypeId *dertype=0;
  srand(time(&tt));

  for (ii=0;ii<10;ii++) {
    sprintf(&buf[0],"[%s%d]","type",1+rand()%9);
    dertype=new DerTypeId(&buf[0]);
    vv[0]=(double)ii;
    tst_listsetat(&dertype->vertexL,(void*)&vv[0],dertype->vertexL.count,1);
    tst_listsetat(dertypeL,(void*)&dertype,dertypeL->count,1);
  }
  tst_listsort(dertypeL);

  return 0;

}

