
#include <tst/cr1d/cr1d.h>
#include <mkbase/defs.h>

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
int ludecomposition(int num,const struct mmat *mm,struct mmat *lum,int *rowperm,double *parity) {

  if (!mm || !rowperm || num<=1)
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
      mmatset(lum,ii,jj,mmatget(mm,ii,jj));
      tmp=fabs(mmatget(lum,ii,jj));
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
      for (kk=0;kk<ii;kk++) {
        tmp=mmatget(lum,ii,jj)-mmatget(lum,ii,kk)*mmatget(lum,kk,jj);
        mmatset(lum,ii,jj,tmp);
      }
    }
    maxcoeff=.0;
    // loop rows for 'l'ower triangular matrix
    // and diagonal (denominators for lower matrix elements) inclusive
    for (ii=jj;ii<num;ii++) {
      // do the matrix multiplication
      // beta[j,j]*alpha[i,j]=m[i,j]-sum(alpha[i,k]*beta[k,j])
      for (kk=0;kk<jj;kk++) {
        tmp=mmatget(lum,ii,jj)-mmatget(lum,ii,kk)*mmatget(lum,kk,jj);
        mmatset(lum,ii,jj,tmp);
      }
      // beta[j,j] is to calculate as pivot(largest element)
      // from this row=i and (precalculated) =rowscale[i]
      tmp=fabs(mmatget(lum,ii,jj))/rowscale[ii];
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
      for (ii=0;ii<num;ii++) {
        tmp=mmatget(lum,imax,ii);
        mmatset(lum,imax,ii,mmatget(lum,jj,ii));
        mmatset(lum,jj,ii,tmp);
      }
      rowscale[imax]=rowscale[jj]; // rowscale[j] is not needed anymore
      mk_swapi(&rowperm[jj],&rowperm[imax]);
      *parity=-(*parity);
    }
    if (mk_deq(mmatget(lum,jj,jj),.0))
      return -1;
    // finallly (for this column) divide all lower row elements
    // by the pivot
    if (jj<(num-1)) {
      for (ii=(jj+1);ii<num;ii++) {
        tmp=mmatget(lum,ii,jj)/mmatget(lum,jj,jj);
        mmatset(lum,ii,jj,tmp);
      }
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
int lubacksubstitution(int num,struct mmat *lum,int *lurowperm,double *rr,double *xx) {

  int ii=0,jj=0;
  if (!lum || !lurowperm || !rr || !xx || num<=1)
    return -1;
  // first adapt the row permutation for the right hand side vector r
  // also copy right hand side input (do not destroy)
  for (ii=0;ii<num;ii++)
    xx[ii]=rr[lurowperm[ii]];
  // do the forward substitution by solving for the lower triangular matrix (alpha)
  // e.g. for (rows=cols=3) lum(lowerpart)={lum[1,1]=1,lum[1,2]=0,lum[1,3]=0,
  // lum[2,1],lum[2,2]=1,lum[2,3]=0,lum[3,1],lum[3,2],lum[3,3]=1} as determined
  // in --> ludecomposition
  for (ii=0;ii<num;ii++) {
    for (jj=0;jj<ii;jj++)
      xx[ii]-=mmatget(lum,ii,jj)*xx[jj];
  }
  // now do the backsubstitution by solving for the upper triangular matrix (beta)
  // e.g. for (rows=cols=3) lum(upperpart)={lum[1,1],lum[1,2],lum[1,3],
  // lum[2,1]=0,lum[2,2],lum[2,3],lum[3,1]=0,lum[3,2]=0,lum[3,3]} as determined
  // in --> ludecomposition
  // (since pivot is not ==1 here we have to do the dividing)
  double tmp=.0;
  for (ii=(num-1);ii>-1;ii--) {
    tmp=xx[ii];
    for (jj=(ii+1);jj<num;jj++)
      tmp-=mmatget(lum,ii,jj)*xx[jj];
    xx[ii]=tmp/mmatget(lum,ii,ii);
  }
  return 0;

}

int mmatalloc(struct mmat *mat,int rows_,int cols_) {

  int ii=0,jj=0;
  mat->rows=(rows_<=0 ? 1 : rows_);
  mat->cols=(cols_<=0 ? 1 : cols_);
  mat->matrix=(double *)malloc(mat->rows*mat->cols*sizeof(double));
  for (ii=0;ii<mat->rows;ii++) {
    for (jj=0;jj<mat->cols;jj++)
      *(mat->matrix+(ii*mat->cols+jj))=(ii==jj ? 1. : .0);
  }
  return mat->rows*mat->cols; 

}

/* ########## */
int mmatfree(struct mmat *mat) {

  if (mat && mat->matrix)
    free(mat->matrix);
  mat->matrix=0;
  return 0;

}

/* ########## */
double mmatget(const struct mmat *mat,int row,int col) {

  if (!mat || row<0 || row>=mat->rows || col<0 || col>=mat->cols)
    return mk_dnan;
  return *(mat->matrix+(row*mat->cols+col));

}

/* ########## */
double mmatset(struct mmat *mat,int row,int col,double val) {

  if (!mat || row<0 || row>=mat->rows || col<0 || col>=mat->cols)
    return mk_dnan;
  double oldval=*(mat->matrix+(row*mat->cols+col));
  *(mat->matrix+(row*mat->cols+col))=val;
  return oldval;

}

int mmatreset(struct mmat *mat,int identity) {

  if (!mat || mat->rows*mat->cols==0)
    return 1;
  memset((void*)mat->matrix,0,mat->rows*mat->cols*sizeof(double));
  if (identity==0)
    return 0;
  int ii=0,jj=0;
  for (ii=0;ii<mat->rows;ii++) {
    for (jj=0;jj<mat->cols;jj++) {
      if (ii==jj)
        mmatset(mat,ii,jj,1.);
    }
  }
  return 0;  

}

int mmatcopy(struct mmat *dest,const struct mmat *src) {

  if (!dest)
    return 1;
  int sz=(src ? src->rows*src->cols : 0);
  if (sz==0) {
    mmatfree(dest);
    return 0;
  }
  if (sz!=dest->rows*dest->cols) {
    mmatfree(dest);
    mmatalloc(dest,src->rows,src->cols);
  }
  memcpy((void*)dest->matrix,(void*)src->matrix,sz*sizeof(double));
  return sz;

}

/* ########## */
int mmattranspose(struct mmat *trmat) {

  if (!trmat || !trmat->matrix || trmat->rows*trmat->cols==0)
    return 1;
  int row=0,col=0,sz=trmat->rows*trmat->cols;
  struct mmat mat;
  mmatalloc(&mat,trmat->rows,trmat->cols);
  mmatcopy(&mat,trmat);
  mk_swapi(&trmat->rows,&trmat->cols);
  for (row=0;row<trmat->rows;row++) {
    for (col=0;col<trmat->cols;col++) {
      *(trmat->matrix+(row*trmat->cols+col))=*(mat.matrix+(col*mat.cols+row));
    }
  }
  mmatfree(&mat);
  return 0;

}

int mmatmult(struct mmat *mumat,const struct mmat *mat) {

  int ii=0,jj=0,kk=0,rr=(mumat ? mumat->rows: 0),cc=(mumat ? mumat->cols : 0);
  if (rr*cc==0 || !mat || mat->rows*mat->cols==0 || cc!=mat->rows)
    return 1;
  double *rmat=(double*)malloc(rr*cc*sizeof(double));
  memcpy((void*)&rmat[0],(void*)&mumat->matrix[0],rr*cc*sizeof(double));
  mmatfree(mumat);
  mmatalloc(mumat,rr,mat->cols);
  memset((void*)mumat->matrix,0,mumat->rows*mumat->cols*sizeof(double));
  double tmp=mk_dnan;
  for (ii=0;ii<rr;ii++) {
    for (jj=0;jj<mat->cols;jj++) {
      for (kk=0;kk<cc;kk++) {
        tmp=mmatget(mumat,ii,jj)+(*(rmat+(ii*cc+kk)))*mmatget(mat,kk,jj);
        mmatset(mumat,ii,jj,tmp);
      }
    }
  }
  free(rmat);
  return 0;  

}

double mmatdet(struct mmat *dmat) {
 
  int ii=0,rr=(dmat ? dmat->rows : 0),cc=(dmat ? dmat->cols : 0);
  if (rr*cc==0 || rr!=cc)
    return 1;
  struct mmat lumat;
  mmatalloc(&lumat,rr,cc);
  int *rowperm=(int*)malloc(rr*sizeof(int));
  for (ii=0;ii<rr;ii++)
    rowperm[ii]=ii;
  double parity=1.;
  if (ludecomposition(4,dmat,&lumat,rowperm,&parity)!=0)
    return .0;
  double det=parity;
  for (ii=0;ii<rr;ii++)
    det*=mmatget(&lumat,ii,ii);
  return det;

}

int mmatinvert(struct mmat *imat) {

  int ii=0,jj=0,len=imat->rows;
  struct mmat lumat;
  mmatalloc(&lumat,len,len);
  int *rowperm=(int*)malloc(len*sizeof(int));
  for (ii=0;ii<len;ii++)
    rowperm[ii]=ii;
  double *identity=(double*)malloc(len*sizeof(double));
  double *inverted=(double*)malloc(len*sizeof(double));
  double parity=1.;
  int chk=ludecomposition(len,imat,&lumat,rowperm,&parity);
  for (ii=0;ii<len;ii++) {
    for (jj=0;jj<len;jj++)
      identity[jj]=inverted[jj]=(ii==jj ? 1. : .0);
    // solve column by column the equation M*M(-1)=1
    chk=lubacksubstitution(len,&lumat,rowperm,identity,inverted);
    if (chk==0) {
      // set the new elements ...
      for (jj=0;jj<len;jj++)
        mmatset(imat,jj,ii,inverted[jj]);
    }
  }
  
  return 0;

}




