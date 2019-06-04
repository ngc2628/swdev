
#include <stdio.h>
#include <stdlib.h>
#include <tst/croutdoolittle.1d/mmat.h>

int usage() {

  printf ("\n");
  return 0;

}

double mm44[4][4]={
  {10.,3.,13.,7.},{4.,2.,1.,11.},{6.,5.,15.,9.},{8.,16.,14.,12.}
};
double mm44inv[4][4]={
  {.19208,.00733,-.18704,.02152},{-.02061,-.03297,-.06456,.09066},
  {-.03205,-.05128,.12179,-.02564},{-.06319,.09890,.06868,-.02198}
};
double mm44det=-8736.;
double rr[4]={209.,141.,243.,338.};
double res[4]={3.,6.,7.,10.};
double mm57[5][7]={
  {.0,.1,.2,.3,.4,.5,.6},{1.0,1.1,1.2,1.3,1.4,1.5,1.6},
  {2.0,2.1,2.2,2.3,2.4,2.5,2.6},{3.0,3.1,3.2,3.3,3.4,3.5,3.6},
  {4.0,4.1,4.2,4.3,4.4,4.5,4.6}
};

int mk_str1() {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  double lum44[4][4]={
    {mk_dnan,mk_dnan,mk_dnan,mk_dnan},{mk_dnan,mk_dnan,mk_dnan,mk_dnan},
    {mk_dnan,mk_dnan,mk_dnan,mk_dnan},{mk_dnan,mk_dnan,mk_dnan,mk_dnan}
  };
  
  struct mk_matrix mm;
  mk_matrixalloc(&mm,4,4);
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      mk_matrixset(&mm,ii,jj,mm44[ii][jj]);
  }
  struct mk_matrix lum;
  mk_matrixalloc(&lum,4,4);
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      mk_matrixset(&lum,ii,jj,lum44[ii][jj]);
  }
  struct mk_matrix matres;
  mk_matrixalloc(&matres,0,0);

  int rowperm[4]={0,0,0,0};
  double parity[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  double vrr[4]={rr[0],rr[1],rr[2],rr[3]};
  double xx[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};

  printf("%d [%d]\n",__LINE__,mk_matrixludecomposition(&mm,&lum,&rowperm[0],&parity[0]));

  /*for (ii=0;ii<4;ii++) {
    for (jj=0;jj<4;jj++)
      printf("%d [%d,%d, %f]\n",__LINE__,ii,jj,lum[ii][jj]);
  }*/

  printf("%d [%d]\n",__LINE__,mk_matrixlubacksubstitution(&lum,&rowperm[0],&vrr[0],&xx[0]));

  printf("%d [%f,%f,%f,%f]\n",__LINE__,xx[0],xx[1],xx[2],xx[3]);

  mk_matrixfree(&mm);
  mk_matrixfree(&lum);

  mk_matrixalloc(&mm,4,4);
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      mk_matrixset(&mm,ii,jj,mm44[ii][jj]);
  }

  printf("%d det [%f]\n",__LINE__,mk_matrixdet(&mm));

  mk_matrixfree(&mm);

  mk_matrixalloc(&mm,4,4);
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      mk_matrixset(&mm,ii,jj,mm44[ii][jj]);
  }

  mk_matrixinvert(&mm);

  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      printf("%9.5f",mk_matrixget(&mm,ii,jj));
    printf("\n");
  }
  printf("\n");

  mk_matrixalloc(&lum,4,4);
  for (ii=0;ii<lum.rows;ii++) {
    for (jj=0;jj<lum.cols;jj++)
      mk_matrixset(&lum,ii,jj,mm44[ii][jj]);
  }

  printf("%d %d\n",__LINE__,mk_matrixmult(&mm,&lum,&matres));
  for (ii=0;ii<matres.rows;ii++) {
    for (jj=0;jj<matres.cols;jj++)
      printf("%9.5f",mk_matrixget(&matres,ii,jj));
    printf("\n");
  }
  printf("\n");

  return 0;

}

int no_mk_str1() {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  double lum44[4][4]={
    {mk_dnan,mk_dnan,mk_dnan,mk_dnan},{mk_dnan,mk_dnan,mk_dnan,mk_dnan},
    {mk_dnan,mk_dnan,mk_dnan,mk_dnan},{mk_dnan,mk_dnan,mk_dnan,mk_dnan}
  };
  
  struct mmat mm;
  mmatalloc(&mm,4,4);
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      mmatset(&mm,ii,jj,mm44[ii][jj]);
  }
  struct mmat lum;
  mmatalloc(&lum,4,4);
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      mmatset(&lum,ii,jj,lum44[ii][jj]);
  }

  int rowperm[4]={0,0,0,0};
  double parity[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  double vrr[4]={rr[0],rr[1],rr[2],rr[3]};
  double xx[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};

  printf("%d [%d]\n",__LINE__,ludecomposition(4,&mm,&lum,&rowperm[0],&parity[0]));

  /*for (ii=0;ii<4;ii++) {
    for (jj=0;jj<4;jj++)
      printf("%d [%d,%d, %f]\n",__LINE__,ii,jj,lum[ii][jj]);
  }*/

  printf("%d [%d]\n",__LINE__,lubacksubstitution(4,&lum,&rowperm[0],&vrr[0],&xx[0]));

  printf("%d [%f,%f,%f,%f]\n",__LINE__,xx[0],xx[1],xx[2],xx[3]);

  mmatfree(&mm);
  mmatfree(&lum);

  mmatalloc(&mm,4,4);
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      mmatset(&mm,ii,jj,mm44[ii][jj]);
  }

  printf("%d det [%f]\n",__LINE__,mmatdet(&mm));

  mmatfree(&mm);

  mmatalloc(&mm,4,4);
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      mmatset(&mm,ii,jj,mm44[ii][jj]);
  }

  mmatinvert(&mm);

  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      printf("%9.5f",mmatget(&mm,ii,jj));
    printf("\n");
  }
  printf("\n");

  mmatalloc(&lum,4,4);
  for (ii=0;ii<lum.rows;ii++) {
    for (jj=0;jj<lum.cols;jj++)
      mmatset(&lum,ii,jj,mm44[ii][jj]);
  }

  printf("%d %d\n",__LINE__,mmatmult(&mm,&lum));
  for (ii=0;ii<mm.rows;ii++) {
    for (jj=0;jj<mm.cols;jj++)
      printf("%9.5f",mmatget(&mm,ii,jj));
    printf("\n");
  }
  printf("\n");

  return 0;

}

int mk_str2() {

  int ii=0,jj=0;
  struct mk_matrix matres;
  mk_matrixalloc(&matres,0,0);
  struct mk_matrix mat1;
  mk_matrixalloc(&mat1,5,7);
  for (ii=0;ii<mat1.rows;ii++) {
    for (jj=0;jj<mat1.cols;jj++)
      mk_matrixset(&mat1,ii,jj,mm57[ii][jj]);
  }
  printf ("%d transpose\n",__LINE__);
  for (ii=0;ii<mat1.rows;ii++) {
    for (jj=0;jj<mat1.cols;jj++) {
      printf("%7.3f",mk_matrixget(&mat1,ii,jj));
    }
    printf("\n");
  }
  printf("\n");
  mk_matrixtranspose(&mat1);
  for (ii=0;ii<mat1.rows;ii++) {
    for (jj=0;jj<mat1.cols;jj++) {
      printf("%7.3f",mk_matrixget(&mat1,ii,jj));
    }
    printf("\n");
  }
  printf("\n");
  mk_matrixfree(&mat1);

  mk_matrixalloc(&mat1,3,4);
  for (ii=0;ii<mat1.rows;ii++) {
    for (jj=0;jj<mat1.cols;jj++)
      mk_matrixset(&mat1,ii,jj,mm57[ii][jj]);
  }
  for (ii=0;ii<mat1.rows;ii++) {
    for (jj=0;jj<mat1.cols;jj++)
      printf("%7.3f",mk_matrixget(&mat1,ii,jj));
    printf("\n");
  }
  printf("\n");

  struct mk_matrix mat2;
  mk_matrixalloc(&mat2,4,2);
  for (ii=0;ii<mat2.rows;ii++) {
    for (jj=0;jj<mat2.cols;jj++)
      mk_matrixset(&mat2,ii,jj,mm44[ii][jj]);
  }
  for (ii=0;ii<mat1.rows;ii++) {
    for (jj=0;jj<mat1.cols;jj++)
      printf("%7.3f",mk_matrixget(&mat1,ii,jj));
    printf("\n");
  }
  printf("\n");

  double multres[3][2]={{4.,6.},{32.,32.},{60.,58.}};
  mk_matrixmult(&mat1,&mat2,&matres);
  for (ii=0;ii<matres.rows;ii++) {
    for (jj=0;jj<matres.cols;jj++)
      printf("%7.3f",mk_matrixget(&matres,ii,jj));
    printf("\n");
  }
  printf("%d\n",__LINE__);

  double vrr[4]={rr[0],rr[1],rr[2],rr[3]};
  double xx[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};

  /*printf("%d [%d]\n",__LINE__,mk_matrixsolve(&mat,&vrr[0],&xx[0]));

  printf("%d [%f,%f,%f,%f]\n",__LINE__,xx[0],xx[1],xx[2],xx[3]);*/

  mk_matrixfree(&mat1);
  mk_matrixfree(&mat2);

  return 0;

}

int no_mk_str2() {

  int ii=0,jj=0;
  struct mmat tmm1;
  struct mmat tmm2;
  mmatalloc(&tmm1,5,7);
  for (ii=0;ii<tmm1.rows;ii++) {
    for (jj=0;jj<tmm1.cols;jj++)
      mmatset(&tmm1,ii,jj,mm57[ii][jj]);
  }
  printf ("%d transpose\n",__LINE__);
  for (ii=0;ii<tmm1.rows;ii++) {
    for (jj=0;jj<tmm1.cols;jj++) {
      printf("%7.3f",mmatget(&tmm1,ii,jj));
    }
    printf("\n");
  }
  printf("\n");
  
  mmattranspose(&tmm1);
  for (ii=0;ii<tmm1.rows;ii++) {
    for (jj=0;jj<tmm1.cols;jj++) {
      printf("%7.3f",mmatget(&tmm1,ii,jj));
    }
    printf("\n");
  }
  printf("\n");
  mmatfree(&tmm1);
  
  printf ("%d mult\n",__LINE__);
  mmatalloc(&tmm1,3,4);
  for (ii=0;ii<tmm1.rows;ii++) {
    for (jj=0;jj<tmm1.cols;jj++)
      mmatset(&tmm1,ii,jj,mm57[ii][jj]);
  }
  for (ii=0;ii<tmm1.rows;ii++) {
    for (jj=0;jj<tmm1.cols;jj++)
      printf("%7.3f",mmatget(&tmm1,ii,jj));
    printf("\n");
  }
  printf("\n");

  mmatalloc(&tmm2,4,2);
  for (ii=0;ii<tmm2.rows;ii++) {
    for (jj=0;jj<tmm2.cols;jj++)
      mmatset(&tmm2,ii,jj,mm44[ii][jj]);
  }
  for (ii=0;ii<tmm2.rows;ii++) {
    for (jj=0;jj<tmm2.cols;jj++)
      printf("%7.3f",mmatget(&tmm2,ii,jj));
    printf("\n");
  }
  printf("\n");

  double multres[3][2]={{4.,6.},{32.,32.},{60.,58.}};
  mmatmult(&tmm1,&tmm2);
  for (ii=0;ii<tmm1.rows;ii++) {
    for (jj=0;jj<tmm1.cols;jj++)
      printf("%7.3f",mmatget(&tmm1,ii,jj));
    printf("\n");
  }
  printf("%d\n",__LINE__);

  return 0;

}

int main(int argc,char **argv) {

  no_mk_str2();
  printf("\n");
  mk_str2();

  int ii=0,jj=0;
  double zero=.0,one=1.;

  
  return 0;

}

