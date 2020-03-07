
#ifndef _mkla_h_
#define _mkla_h_

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <mkbase/defs.h>
#include <mkbase/exportdefs.h>
#include <mkbase/mkutil.h>
#include <mkbase/mkmath.h>

typedef double mk_vertex[4];
#define mk_vertexnan(name) mk_vertex name={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
#define mk_vertexzero(name) mk_vertex name={.0,.0,.0,1.};

/* 
  matrix of double values , rows: number of rows , cols: number of columns , 
  matrix: flat arr of values - element[ii][jj]=matrix[ii*cols+jj]
*/
struct oswinexp mk_matrix {
  int rows,cols;
  double *matrix;
};

#ifdef __cplusplus
/* #pragma message "cplusplus" */
extern "C" {
#endif

/*
  in vertex , out string , return 0|1
*/
xtern int oswinexp mk_vertexdbg(const mk_vertex,mk_string);

/*
  out vertex-to , in vertex-from , return 0,1
*/
xtern int oswinexp mk_vertexcopy(mk_vertex,const mk_vertex);

/*
  inout vertex , in value , return 0,1
*/
xtern int oswinexp mk_vertexset(mk_vertex,double);

/*
  inout vertex-1 , inout vertex-2 , return 0,1
*/
xtern int oswinexp mk_vertexswap(mk_vertex,mk_vertex);

/*
  in vertex , return length
*/
xtern double oswinexp mk_vertexlen(const mk_vertex);

/*
  in *vertex-cmp1 , in *vertex-cmp2 , in arr-idx , return -1,0,1
*/
xtern int oswinexp mk_vertexcmpidx(const void *,const void *,int);

/*
  in *vertex-cmp1 , in *vertex-cmp2 , return -1,0,1
*/
xtern int oswinexp mk_vertexcmpx(const void *,const void *);

/*
  in *vertex-cmp1 , in *vertex-cmp2 , return -1,0,1
*/
xtern int oswinexp mk_vertexcmpy(const void *,const void *);

/*
  in *vertex-cmp1 , in *vertex-cmp2 , return -1,0,1
*/
xtern int oswinexp mk_vertexcmpz(const void *,const void *);

/*
  in *vertex-cmp1 , in *vertex-cmp2 , return -1,0,1
*/
xtern int oswinexp mk_vertexcmpw(const void *,const void *);

/*
  cmp-function array
*/
typedef int (*mk_vertexcmpf)(const void *,const void *);
xtern oswinexp mk_vertexcmpf mk_vertexcmp[4];

/*
  in vertex1 , in vertex2  , out vertex-result , return 0,1
*/
xtern int oswinexp mk_vertexadd(const mk_vertex,const mk_vertex,mk_vertex);

/*
  in vertex1 , in vertex2 , out vertex-result , return 0,1
*/
xtern int oswinexp mk_vertexsubs(const mk_vertex,const mk_vertex,mk_vertex);

/*
  in vertex , in multiplier  , out vertex-result , return 0,1
*/
xtern int oswinexp mk_vertexmult(const mk_vertex,double,mk_vertex);

/*
  in vertex , in divisor  , out vertex-result , return 0,1
*/
xtern int oswinexp mk_vertexdiv(const mk_vertex,double,mk_vertex);

/*
  in vertex1 , in vertex2  , return dotproduct
*/
xtern double oswinexp mk_vertexdot(const mk_vertex,const mk_vertex);

/*
  in vertex , out vertex-result , return 0,1
*/
xtern int oswinexp mk_vertexnorm(const mk_vertex,mk_vertex);

/*
  in vertex1 , in vertex2  , out vertex-result , return 0,1
*/
xtern int oswinexp mk_vertexcross(const mk_vertex,const mk_vertex,mk_vertex);

/*
  in vertex1 , int vertex2 , return angle(rad)-vertex1-vertex2
*/
xtern double oswinexp mk_vertexangrad(const mk_vertex,const mk_vertex);

/*
  in vertex1 , in vertex2 , return angle(deg)-vertex1-vertex2
*/
xtern double oswinexp mk_vertexangdeg(const mk_vertex,const mk_vertex);

/*
  in linep1 , in linep2 , in xarg , return result
*/
xtern double oswinexp mk_lineq(const mk_vertex,const mk_vertex,double);

/*
  in hp1 , in hp2 , in xarg , return result
*/
xtern double oswinexp mk_lgeq(const mk_vertex,const mk_vertex,double);

/*
  in line1.p1 , in line1.p2 , in line2.p1 , in line2.p2 ,
  out intersection.p , in prec , in box-restraint 0,1,2 , return 0,1
*/
xtern int oswinexp mk_linesintersection(
  const mk_vertex,const mk_vertex,const mk_vertex,const mk_vertex,mk_vertex,int,int);

/*
  in rect.topleft , in rect.bottomright , in line.p1 , in line.p2 ,
  out p.left , out p.top , out p.right , out p.bottom ,
  out (left,top,right,bottom) as 0,1 indicator line-cuts-rect ,
  in cliplinetorect 0,1
*/
xtern int oswinexp mk_intersectionpointslinerect(const mk_vertex,const mk_vertex,const mk_vertex,
  const mk_vertex,mk_vertex,mk_vertex,mk_vertex,mk_vertex,mk_vertex,int);

/*
  out list-of-vertices* , in number-of-points , return 0,1
*/
xtern int oswinexp mk_ellipse(struct mk_list *,int);

/*
  in polygon1* , in polygon2* , out *intersection points , return numberintersectionpoints
*/
xtern int oswinexp mk_polygonintersection(struct mk_list *,struct mk_list *,struct mk_list *);

/*
  in list-of-vertices* , out list-of-1st-derivatives* (df/dx)[i][0], (df/dy)[i][1] ,
  out list-of-2nd-derivatives* (df2/dxdx)[i][0], (d2f/dydy)[i][1] , return 0,1
*/
xtern int oswinexp mk_derivatives(struct mk_list *,struct mk_list *,struct mk_list *);

/*
  in list-of-vertices* , out list-of-2nd-derivatives* (d2f/dxdy)[i][2] ,
  in number grid columns , return 0,1
*/
xtern int oswinexp mk_derivativesmixed(struct mk_list *,struct mk_list *,int);

/*
  inout matrix* , in rows , in cols , return 0,size
*/
xtern int oswinexp mk_matrixalloc(struct mk_matrix *,int,int);

/*
  inout matrix* , return 0,1
*/
xtern int oswinexp mk_matrixfree(struct mk_matrix *);

/*
  in matrix* , in row , in col , return value at row,col | nan
*/
double oswinexp mk_matrixget(const struct mk_matrix *,int,int);

/*
  inout matrix* , in row , in col , in new value , return 0|1
*/
int oswinexp mk_matrixset(struct mk_matrix *,int,int,double);

/*
  out matrix-to* , in matrix-from* , return 0|1
*/
int oswinexp mk_matrixcopy(struct mk_matrix *,const struct mk_matrix *);

/*
  inout matrix* , in identity 0|1 , return 0|1
*/
int oswinexp mk_matrixreset(struct mk_matrix *,int);

/*
  in matrix* , in diff-check , return 0|1
*/
int mk_matrixisidentity(struct mk_matrix *,double);

/*
  inout matrix* , return 0|1
*/
int oswinexp mk_matrixtranspose(struct mk_matrix *);

/*
  in matrix* , in matrix* , out matrix* , return 0|1
*/
int oswinexp mk_matrixmult(const struct mk_matrix *,const struct mk_matrix *,struct mk_matrix *);

/*
  in squarematrix* , out decomposited squarematrix ,
  out row permutation (which row goes where) , out row interchangeables even(1)/odd(-1)
*/
xtern int oswinexp mk_matrixludecomposition(struct mk_matrix *,struct mk_matrix *,int *, double *);

/*
  in squarematrix* from ludecomposition , in row permutation* from ludecomposition ,
  in right hand side column vector* , out result vector* a : a0+a1x+a2x**2 ... anx**n
*/
xtern int oswinexp mk_matrixlubacksubstitution(struct mk_matrix *,int *,double *,double *);

/*
  in matrix* , return determinant
*/
double oswinexp mk_matrixdet(struct mk_matrix *);

/*
  inout matrix* , return 0|1
*/
int oswinexp mk_matrixinvert(struct mk_matrix *);

/*
  in squarematrix* , in right hand side column vector* ,
  out result vector* a : a0+a1*x+a2*x**2 ... an*x**n
*/
xtern int oswinexp mk_matrixsolve(struct mk_matrix *,double *,double *);

/*
  in matrix* , out dbgstring , return 0|1
*/
xtern int mk_matrixdbg(struct mk_matrix *,mk_string);

#ifdef __cplusplus
}
#endif

#endif
