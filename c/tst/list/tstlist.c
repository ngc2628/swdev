
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>
#include <tst/list/list.h>


int cmpdbl(const void *dbl1,const void *dbl2) {

  int res=0;
  const double *vvv1=(const double*)dbl1;
  const double *vvv2=(const double*)dbl2;

  if ((*vvv1)<(*vvv2))
    res=-1;
  else if ((*vvv2)<(*vvv1))
    res=1;
  return res;

}

int cmpvertex(const void *vv1,const void *vv2) {

  int res=0;
  const mk_vertex *vvv1=(const mk_vertex*)vv1;
  const mk_vertex *vvv2=(const mk_vertex*)vv2;

  if ((*vvv1)[0]<(*vvv2)[0])
    res= -1;
  if ((*vvv2)[0]<(*vvv1)[0])
    res= 1;

/*printf("%d [%d] [%f,%f,%f,%f] [%f,%f,%f,%f]\n",__LINE__,res,
(*vvv1)[0],(*vvv1)[1],(*vvv1)[2],(*vvv1)[3],(*vvv2)[0],(*vvv2)[1],(*vvv2)[2],(*vvv2)[3]);*/

  return res;

}

int tstlist() {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  /*chklist();*/

  int rows=4,cols=4;
  struct tst_list vvL;
  tst_listalloc(&vvL,sizeof(mk_vertex),rows*cols);

  mk_vertexnan(vv);
  vv[0]=2.7;
  vv[1]=3.;
  vv[2]=11.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=3.7;
  vv[1]=4.;
  vv[2]=15.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=5.7;
  vv[1]=5.;
  vv[2]=14.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=6.7;
  vv[1]=7.;
  vv[2]=16.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=2.3;
  vv[1]=3.;
  vv[2]=17.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=3.3;
  vv[1]=4.;
  vv[2]=12.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=5.3;
  vv[1]=5.;
  vv[2]=13.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=6.3;
  vv[1]=7.;
  vv[2]=10.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=2.9;
  vv[1]=3.;
  vv[2]=15.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=3.9;
  vv[1]=4.;
  vv[2]=22.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=5.9;
  vv[1]=5.;
  vv[2]=20.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);
  vv[0]=6.9;
  vv[1]=7.;
  vv[2]=14.;
  tst_listsetat(&vvL,(void*)&vv,vvL.count,1);

  /*double chkL[12]={2.7,3.7,5.7,6.7,2.3,3.3,5.3,6.3,2.9,3.9,5.9,6.9};
  tst_heapsortvc(12,(void *)(&chkL[0]),cmpdbl);
  for (ii=0;ii<12;ii++) {
    printf("%d #%d [%f]\n",__LINE__,ii,chkL[ii]);
  }
  return 0;*/

  mk_vertexnan(vvidx);
  tst_listat(&vvL,1,(void*)&vvidx);
  printf("%d [%f,%f,%f,%f]\n",__LINE__,vvidx[0],vvidx[1],vvidx[2],vvidx[3]);

  vvL.cmp=cmpvertex;
  tst_listsort(&vvL);

  for (ii=0;ii<vvL.count;ii++) {
    tst_listat(&vvL,ii,(void*)&vvidx);
    printf("%d #%d [%f,%f,%f,%f]\n",__LINE__,ii,vvidx[0],vvidx[1],vvidx[2],vvidx[3]);
  }

  mk_vertexset(vvidx,5.3);
  ii=tst_listfind(&vvL,(void*)&vvidx);
  printf("%d [%d]\n",__LINE__,ii);

  vvidx[0]=3.8;
  vvidx[1]=77.;
  vvidx[2]=88.;
  ii=tst_listinsort(&vvL,(void*)&vvidx);
  printf("%d [%d]\n",__LINE__,ii);
  for (ii=0;ii<vvL.count;ii++) {
    tst_listat(&vvL,ii,(void*)&vvidx);
    printf("%d #%d [%f,%f,%f,%f]\n",__LINE__,ii,vvidx[0],vvidx[1],vvidx[2],vvidx[3]);
  }

  vvidx[0]=4.7;
  vvidx[1]=.1;
  vvidx[2]=.2;
  tst_listsetat(&vvL,(void*)&vvidx,3,0);
  tst_listsetat(&vvL,0,10,0);
  for (ii=0;ii<vvL.count;ii++) {
    tst_listat(&vvL,ii,(void*)&vvidx);
    printf("%d #%d [%f,%f,%f,%f]\n",__LINE__,ii,vvidx[0],vvidx[1],vvidx[2],vvidx[3]);
  }

  tst_listremove(&vvL,7,&vvidx);
  for (ii=0;ii<vvL.count;ii++) {
    tst_listat(&vvL,ii,(void*)&vvidx);
    printf("%d #%d [%f,%f,%f,%f]\n",__LINE__,ii,vvidx[0],vvidx[1],vvidx[2],vvidx[3]);
  }

  tst_listfree(&vvL);

  return 0;

}

