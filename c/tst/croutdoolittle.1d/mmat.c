
#include <tst/croutdoolittle.1d/mmat.h>
#include <mkbase/defs.h>

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
double mmatget(struct mmat *mat,int row,int col) {

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