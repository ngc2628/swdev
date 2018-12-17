
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkbase.h>
#include <mkbase/mkla.h>

int usage() {

  printf ("\n");
  return 0;

}

/** ----------------------------------------------------------------------------------
calculate decomposition of square matrix 'm' (num rows and num colums)
into lower(alpha) and upper(beta) triangular matrices
e.g.(rows=cols=3) m={m[1,1],m[1,2],m[1,3],m[2,1],m[2,2],m[2,3],m[3,1],m[2,3],m[3,3]} into
alpha={alpha[2,1],alpha[3,1],alpha[3,2]} and
beta={beta[1,1],beta[1,2],beta[1,3],beta[2,2],beta[2,3],beta[3,3]}
out matrix is then m=
{beta[1,1],beta[1,2],beta[1,3],alpha[2,1],beta[2,2],beta[2,3],alpha[3,1],alpha[3,2],beta[3,3]}
where diagonal is set as beta[i,i] and alpha[i,i]=1.0 as free selectable coefficients
triangular matrices alpha+beta can be easily solved by forward/backward substitution
decomposed matrix will be returned in lum to preserve the original m
------------------------------------------------------------------------------------ */
int ludecomposition(int num, double **m, double **lum, int *rowperm, double *parity) {

  if (!m || !rowperm || num<=1)
    return -1;
  int ii=0,jj=0,kk=0,imax=0;
  double maxcoeff=.0,tmp=.0;
  for (ii=0;ii<num;ii++)
    rowperm[ii]=ii; // no row interchanging yet
  *parity=1.;
  // first find the largest element in every row (implicit pivoting)
  // also copy the original matrix since we do not want to destroy it
  double *rowscale=(double*)malloc(num*sizeof(double));
  for (ii=0;ii<num;ii++) {
    maxcoeff=.0;
    for (jj=0;jj<num;jj++) {
      lum[ii][jj]=m[ii][jj];
      tmp=fabs(lum[ii][jj]);
      if (tmp>maxcoeff)
        maxcoeff=tmp;
    }
    if (mk_deq(maxcoeff,.0))
      return -1;
    rowscale[ii]=maxcoeff;
  }
  // loop every column
  // used alphas and betas are already calculated by the time they are needed
  for (jj=0;jj<num;jj++) {
    // loop rows for 'u'pper triangular matrix
    for (ii=0;ii<jj;ii++) {
      // do the matrix multiplication
      // beta[i,j]=m[i,j]-sum(alpha[i,k]*beta[k,j])
      for (kk=0;kk<ii;kk++)
        lum[ii][jj]-=lum[ii][kk]*lum[kk][jj];
    }
    maxcoeff=.0;
    // loop rows for 'l'ower triangular matrix
    // and diagonal (denominators for lower matrix elements) inclusive
    for (ii=jj;ii<num;ii++) {
      // do the matrix multiplication
      // beta[j,j]*alpha[i,j]=m[i,j]-sum(alpha[i,k]*beta[k,j])
      for (kk=0;kk<jj;kk++)
        lum[ii][jj]-=lum[ii][kk]*lum[kk][jj];
      // beta[j,j] is to calculate as pivot(largest element)
      // from this row=i and (precalculated) =rowscale[i]
      tmp=fabs(lum[ii][jj])/rowscale[ii];
      if (tmp>=maxcoeff) {
        maxcoeff=tmp;
        imax=ii;
      }
    }
    // if index of precalculated rowscale!=scale[actrow]
    // rows must be interchanged and index table updated
    // for later dividing by pivot element (beta[j,j])
    // this is possible since columns<j are already determined, and
    // columns>j are not used yet, therefore row interchanging
    // does not destroy the solution only just scrambles the order
    // which means that the out-matrix may look queer but dissolves into
    // a rowwise permutation of m
    if (jj!=imax) {
      for (ii=0;ii<num;ii++)
        mk_swapf(&lum[imax][ii],&lum[jj][ii]);
      rowscale[imax]=rowscale[jj]; // rowscale[j] is not needed anymore
      mk_swapi(&rowperm[jj],&rowperm[imax]);
      *parity=-(*parity);
    }
    if (mk_deq(lum[jj][jj],.0))
      return -1;
    // finallly (for this column) divide all lower row elements
    // by the pivot
    if (jj<(num-1)) {
      for (ii=(jj+1);ii<num;ii++)
        lum[ii][jj]/=lum[jj][jj];
    }
  }
  free(rowscale);
  return 0;

}

/** ---------------------------------------------------------------------------------
calculate solution for matrix m and right hand side vector r when incoming matrix lum
is the lu-decomposition of the rowwise permutation of m
(arranged the same way as output of --> ludecomposition)
---------------------------------------------------------------------------------- */
int lubacksubstitution(int num, double **lum, int *lurowperm, double *r,double *x) {

  int ii=0,jj=0;
  if (!lum || !lurowperm || !r || !x || num<=1)
    return -1;
  // first adapt the row permutation for the right hand side vector r
  // also copy right hand side input (do not destroy)
  for (ii=0;ii<num;ii++)
    x[ii]=r[lurowperm[ii]];
  // do the forward substitution by solving for the lower triangular matrix (alpha)
  // e.g. for (rows=cols=3) lum(lowerpart)={lum[1,1]=1,lum[1,2]=0,lum[1,3]=0,
  // lum[2,1],lum[2,2]=1,lum[2,3]=0,lum[3,1],lum[3,2],lum[3,3]=1} as determined
  // in --> ludecomposition
  for (ii=0;ii<num;ii++) {
    for (jj=0;jj<ii;jj++)
      x[ii]-=lum[ii][jj]*x[jj];
  }
  // now do the backsubstitution by solving for the upper triangular matrix (beta)
  // e.g. for (rows=cols=3) lum(upperpart)={lum[1,1],lum[1,2],lum[1,3],
  // lum[2,1]=0,lum[2,2],lum[2,3],lum[3,1]=0,lum[3,2]=0,lum[3,3]} as determined
  // in --> ludecomposition
  // (since pivot is not ==1 here we have to do the dividing)
  double tmp=.0;
  for (ii=(num-1);ii>-1;ii--) {
    tmp=x[ii];
    for (jj=(ii+1);jj<num;jj++)
      tmp-=lum[ii][jj]*x[jj];
    x[ii]=tmp/lum[ii][ii];
  }
  return 0;

}

double mm44[4][4]={
  {10.,3.,13.,7.},{4.,2.,1.,11.},{6.,5.,15.,9.},{8.,16.,14.,12.}
};
double rr[4]={209.,141.,243.,338.};
double res[4]={3.,6.,7.,10.};

int no_mk_str() {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  double lum44[4][4]={
    {mk_dnan,mk_dnan,mk_dnan,mk_dnan},{mk_dnan,mk_dnan,mk_dnan,mk_dnan},
    {mk_dnan,mk_dnan,mk_dnan,mk_dnan},{mk_dnan,mk_dnan,mk_dnan,mk_dnan}
  };
  double **mm=(double**)malloc(4*sizeof(double*));
  for (ii=0;ii<4;ii++) {
    mm[ii]=(double*)malloc(4*sizeof(double));
    memcpy(&mm[ii][0],&mm44[ii][0],4*sizeof(double));
  }
  double **lum=(double**)malloc(4*sizeof(double*));
  for (ii=0;ii<4;ii++) {
    lum[ii]=(double*)malloc(4*sizeof(double));
    memcpy(&lum[ii][0],&lum44[ii][0],4*sizeof(double));
  }

  int rowperm[4]={0,0,0,0};
  double parity[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  double vrr[4]={rr[0],rr[1],rr[2],rr[3]};
  double xx[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};

  printf("%d [%d]\n",__LINE__,ludecomposition(4,mm,lum,&rowperm[0],&parity[0]));

  /*for (ii=0;ii<4;ii++) {
    for (jj=0;jj<4;jj++)
      printf("%d [%d,%d, %f]\n",__LINE__,ii,jj,lum[ii][jj]);
  }*/

  printf("%d [%d]\n",__LINE__,lubacksubstitution(4,lum,&rowperm[0],&vrr[0],&xx[0]));

  printf("%d [%f,%f,%f,%f]\n",__LINE__,xx[0],xx[1],xx[2],xx[3]);

  return 0;

}

int main(int argc,char **argv) {

  no_mk_str();

  int ii=0,jj=0;
  double zero=.0,one=1.;

  struct mk_matrix mat,lum;
  mk_matrixalloc(&mat,4,4);
  mk_matrixalloc(&lum,4,4);

  mk_matrixset(&mat,0,0,mm44[0][0]);
  mk_matrixset(&mat,0,1,mm44[0][1]);
  mk_matrixset(&mat,0,2,mm44[0][2]);
  mk_matrixset(&mat,0,3,mm44[0][3]);
  mk_matrixset(&mat,1,0,mm44[1][0]);
  mk_matrixset(&mat,1,1,mm44[1][1]);
  mk_matrixset(&mat,1,2,mm44[1][2]);
  mk_matrixset(&mat,1,3,mm44[1][3]);
  mk_matrixset(&mat,2,0,mm44[2][0]);
  mk_matrixset(&mat,2,1,mm44[2][1]);
  mk_matrixset(&mat,2,2,mm44[2][2]);
  mk_matrixset(&mat,2,3,mm44[2][3]);
  mk_matrixset(&mat,3,0,mm44[3][0]);
  mk_matrixset(&mat,3,1,mm44[3][1]);
  mk_matrixset(&mat,3,2,mm44[3][2]);
  mk_matrixset(&mat,3,3,mm44[3][3]);

  double vrr[4]={rr[0],rr[1],rr[2],rr[3]};
  double xx[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};

  printf("%d [%d]\n",__LINE__,mk_matrixsolve(&mat,&vrr[0],&xx[0]));

  printf("%d [%f,%f,%f,%f]\n",__LINE__,xx[0],xx[1],xx[2],xx[3]);

  return 0;

}

