
#include <tst/cr2d/cr2d.h>
#include <mkbase/mkmath.h>
#include <mkbase/defs.h>

/* ########## */
int mmatalloc(struct mmat *mat,int rows_,int cols_) {

  int ii=0,jj=0;
  mat->rows=(rows_<=0 ? 1 : rows_);
  mat->cols=(cols_<=0 ? 1 : cols_);
  mat->matrix=(double **)malloc(mat->rows*sizeof(double *));
  for (ii=0;ii<mat->rows;ii++) {
    mat->matrix[ii]=(double *)malloc(mat->cols*sizeof(double));
    for (jj=0;jj<mat->cols;jj++)
      mat->matrix[ii][jj]=(ii==jj ? 1. : .0);
  }
  return mat->rows*mat->cols; 

}

/* ########## */
int mmatfree(struct mmat *mat) {

  if (!mat || !mat->matrix)
    return 1;
  int ii=0;
  for (ii=0;ii<mat->rows;ii++)
    free(mat->matrix[ii]);
  free(mat->matrix);
  mat->matrix=0;
  return 0;

}

/* ########## */
double mmatget(struct mmat *mat,int row,int col) {

  if (!mat || row<0 || row>=mat->rows || col<0 || col>=mat->cols)
    return mk_dnan;
  return mat->matrix[row][col];

}

/* ########## */
double mmatset(struct mmat *mat,int row,int col,double val) {

  if (!mat || row<0 || row>=mat->rows || col<0 || col>=mat->cols)
    return mk_dnan;
  double oldval=mat->matrix[row][col];
  mat->matrix[row][col]=val;
  return oldval;

}
