
#include <stdio.h>
#include <stdlib.h>
#include <tst/bcucof/bcu.h>

  double ccnr[16][16]={
    {1. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
    {0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,1. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
    {-3.,0. ,0. ,3. ,0. ,0. ,0. ,0. ,-2.,0. ,0. ,-1.,0. ,0. ,0. ,0. },
    {2. ,0. ,0. ,-2.,0. ,0. ,0. ,0. ,1. ,0. ,0. ,1. ,0. ,0. ,0. ,0. },
    {0. ,0. ,0. ,0. ,1. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
    {0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,1. ,0. ,0. ,0. },
    {0. ,0. ,0. ,0. ,-3.,0. ,0. ,3. ,0. ,0. ,0. ,0. ,-2.,0. ,0. ,-1.},
    {0. ,0. ,0. ,0. ,2. ,0. ,0. ,-2.,0. ,0. ,0. ,0. ,1. ,0. ,0. ,1. },
    {-3.,3. ,0. ,0. ,-2.,-1.,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
    {0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,-3.,3. ,0. ,0. ,-2.,-1.,0. ,0. },
    {9. ,-9.,9. ,-9.,6. ,3. ,-3.,-6.,6. ,-6.,-3.,3. ,4. ,2. ,1. ,2. },
    {-6.,6. ,-6.,6. ,-4.,-2.,2. ,4. ,-3.,3. ,3. ,-3.,-2.,-1.,-1.,-2.},
    {2. ,-2.,0. ,0. ,1. ,1. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
    {0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,2. ,-2.,0. ,0. ,1. ,1. ,0. ,0. },
    {-6.,6. ,-6.,6. ,-3.,-3.,3. ,3. ,-4.,4. ,2. ,-2.,-2.,-2.,-1.,-1.},
    {4. ,-4.,4. ,-4.,2. ,2. ,-2.,-2.,2. ,-2.,-2.,2. ,1. ,1. ,1. ,1. }
  };

  double ccwp[16][16]={
    { 1., .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0},
    { .0, .0, .0, .0, 1., .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0},
    {-3., 3., .0, .0,-2.,-1., .0, .0, .0, .0, .0, .0, .0, .0, .0, .0},
    { 2.,-2., .0, .0, 1., 1., .0, .0, .0, .0, .0, .0, .0, .0, .0, .0},
    { .0, .0, .0, .0, .0, .0, .0, .0, 1., .0, .0, .0, .0, .0, .0, .0},
    { .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, .0, 1., .0, .0, .0},
    { .0, .0, .0, .0, .0, .0, .0, .0,-3., 3., .0, .0,-2.,-1., .0, .0},
    { .0, .0, .0, .0, .0, .0, .0, .0, 2.,-2., .0, .0, 1., 1., .0, .0},
    {-3., .0, 3., .0, .0, .0, .0, .0,-2., .0,-1., .0, .0, .0, .0, .0},
    { .0, .0, .0, .0,-3., .0, 3., .0, .0, .0, .0, .0,-2., .0,-1., .0},
    { 9.,-9.,-9., 9., 6., 3.,-6.,-3., 6.,-6., 3.,-3., 4., 2., 2., 1.},
    {-6., 6., 6.,-6.,-3.,-3., 3., 3.,-4., 4.,-2., 2.,-2.,-2.,-1.,-1.},
    { 2., .0,-2., .0, .0, .0, .0, .0, 1., .0, 1., .0, .0, .0, .0, .0},
    { .0, .0, .0, .0, 2., .0,-2., .0, .0, .0, .0, .0, 1., .0, 1., .0},
    {-6., 6., 6.,-6.,-4.,-2., 4., 2.,-3., 3.,-3., 3.,-2.,-1.,-2.,-1.},
    { 4.,-4.,-4., 4., 2., 2.,-2.,-2., 2.,-2., 2.,-2., 1., 1., 1., 1.}
  };

int bcucof(mk_vertex *vv,double *dzx,double *dzy,double *dzxy,struct mk_matrix *cc) {

  struct mk_matrix mat1,mat2,mat3;
  mk_matrixalloc(&mat1,16,16);
  mk_matrixalloc(&mat2,16,1);
  mk_matrixalloc(&mat3,16,1);
  int ii=0,jj=0;
  for (ii=0;ii<16;ii++) {
    for (jj=0;jj<16;jj++)
      mk_matrixset(&mat1,ii,jj,ccnr[ii][jj]);
  }
  double dx=(vv[2][0]-vv[3][0]+vv[1][0]-vv[0][0])/2.;
  double dy=(vv[2][1]-vv[1][1]+vv[3][1]-vv[0][1])/2.;
  for (ii=0;ii<4;ii++) {
    mk_matrixset(&mat2,ii,0,vv[ii][2]);
    mk_matrixset(&mat2,ii+4,0,dzx[ii]*dx);
    mk_matrixset(&mat2,ii+8,0,dzy[ii]*dy);
    mk_matrixset(&mat2,ii+12,0,dzxy[ii]*dx*dy);
  }
  
  mk_matrixmult(&mat1,&mat2,&mat3);
  for (ii=0;ii<4;ii++) {
    for (jj=0;jj<4;jj++)
      mk_matrixset(cc,ii,jj,mk_matrixget(&mat3,ii*4+jj,0));
  }
  return 0;

}



  
