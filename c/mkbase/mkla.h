
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

typedef double mk_vertex[4];
#define mk_vertexnan(name) mk_vertex name={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
#define mk_vertexzero(name) mk_vertex name={.0,.0,.0,.0};

struct oswinexp mk_matrix {
  int rows,cols;
  double **matrix;
};

struct oswinexp mk_polynomial {
  int len;
  mk_vertex *ctrlL;
  double **cc;
  double **dd;
};

#ifdef __cplusplus
/* #pragma message "cplusplus" */
extern "C" {
#endif

/*
  in vertex , return str , allocates memory
*/
xtern char * oswinexp mk_vertexdbg(const mk_vertex);

/*
  inout vertex-to , in vertex-from , return 0,1
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
  inout vertex-mod , in vertex-addend  , return 0,1
*/
xtern int oswinexp mk_vertexadd(mk_vertex,const mk_vertex);

/*
  inout vertex-mod , in vertexaddend  , return 0,1
*/
xtern int oswinexp mk_vertexsubs(mk_vertex,const mk_vertex);

/*
  inout vertex-mod , in multiplier  , return 0,1
*/
xtern int oswinexp mk_vertexmult(mk_vertex,double);

/*
  inout vertex-mod , in divisor  , return 0,1
*/
xtern int oswinexp mk_vertexdiv(mk_vertex,double);

/*
  in vertex1 , in vertex2  , return dotproduct
*/
xtern double oswinexp mk_vertexdot(const mk_vertex,const mk_vertex);

/*
  inout vertex-mod , return 0,1
*/
xtern int oswinexp mk_vertexnorm(mk_vertex);

/*
  inout mod-vertex , in multiplier  , return 0,1
*/
xtern int oswinexp mk_vertexcross(mk_vertex,const mk_vertex);

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
xtern double oswinexp mk_lineq(mk_vertex,mk_vertex,double);

/* 
  in hp1 , in hp2 , in xarg , return result 
*/
xtern double oswinexp mk_lgeq(mk_vertex,mk_vertex,double);

/*
  in line1.p1 , in line1.p2 , in line2.p1 , in line2.p2 ,
  out intersection.p , in prec , in box-restraint 0,1,2 , return 0,1
*/
xtern int oswinexp mk_linesintersection(mk_vertex,mk_vertex,mk_vertex,mk_vertex,mk_vertex,int,int);

/*
  in rect.topleft , in rect.bottomright , in line.p1 , in line.p2 ,
  out p.left , out p.top , out p.right , out p.bottom , 
  out (left,top,right,bottom) as 0,1 indicator line-cuts-rect , 
  in cliplinetorect 0,1
*/
xtern int oswinexp mk_intersectionpointslinerect(mk_vertex,mk_vertex,mk_vertex,mk_vertex,
  mk_vertex,mk_vertex,mk_vertex,mk_vertex,mk_vertex,int);

/*
  inout list-of-vertices* , in number-of-points , return 0,1
*/
xtern int oswinexp mk_ellipse(struct mk_list *,int);

/*
  in polygon1* , in polygon2* , out *intersection points , return numberintersectionpoints
*/
xtern int mk_polygonintersection(struct mk_list *,struct mk_list *,struct mk_list *);

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
  inout matrix* , return 0,size
*/
xtern int oswinexp mk_matrixalloc(struct mk_matrix *,int,int);

/*
  inout matrix* , return 0,1
*/
xtern int oswinexp mk_matrixfree(struct mk_matrix *);

/*
  in matrix* , in row , in col , return value at row,col | nan
*/
double mk_matrixget(struct mk_matrix *,int,int);

/*
  inout matrix* , in row , in col , in new value , return old value at row,col | nan
*/
double mk_matrixset(struct mk_matrix *,int,int,double);

/*
  in squarematrix* , out decomposited squarematrix ,
  out row permutation (which row goes where) , out row interchangeables even(1)/odd(-1) 
*/
xtern int oswinexp mk_matrixludecomposition(struct mk_matrix *,struct mk_matrix *,int *, double *);

/*
  in squarematrix* from ludecomposition, in row permutation* ,
  in right hand side column vector* , out result vector* a : a0+a1x+a2x**2 ... anx**n
*/
xtern int oswinexp mk_matrixlubacksubstitution(struct mk_matrix *,int *,double *,double *);

/*
  in squarematrix* , in right hand side column vector* , 
  out result vector* a : a0+a1x+a2x**2 ... anx**n
*/
xtern int oswinexp mk_matrixsolve(struct mk_matrix *,double *,double *);

/*
  inout polynomial* , return 0,length
*/
xtern int oswinexp mk_polynomialalloc(struct mk_polynomial *);

/*
  inout polynomial* , return 0,1
*/
xtern int oswinexp mk_polynomialfree(struct mk_polynomial *);

/*
  in polynomial* , interpolation at x-value , return 0,1
*/
xtern double mk_polynomialinterp(struct mk_polynomial *,double);

/*
  inout polynomial* , interpolation at x-value , intern slat 
*/
xtern int mk_polynomialcoeff(struct mk_polynomial *,double *);

#ifdef __cplusplus
}
#endif

#endif
