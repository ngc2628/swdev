
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
#include <mkbase/mkbase.h>

typedef double mk_vertex[4];

struct oswinexp mk_vertices {
  int reserve,cnt;
  mk_vertex *vertexL;
};

struct oswinexp mk_matrix {
  int rows,cols;
  double **mm;
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

/* in xlow , in xhigh , in ylow , in yhigh , in xarg , return result */
xtern double oswinexp mk_lineq(double,double,double,double,double);

/* in xlow , in xhigh , in ylow , in yhigh , in xarg , return result */
xtern double oswinexp mk_lgeq(double,double,double,double,double);

/* in number , return result */
xtern double oswinexp mk_factorial(int);

/* in number set , in number subset , return result */
xtern double oswinexp mk_binomialCoeff(int,int);

/*
  in rect.left , in rect.right , in rect.bottom , in rect.top ,
  in line.p1.x , in line.p1.y ,  in line.p2.x , in line.p2.y ,
  out *p.bottom.x , out *p.bottom.y , out *p.left.x , out *p.left.y ,
  out *p.top.x , out *p.top.y , out *p.right.x , out *p.right.y ,
  out *cutbottom 0,1 , out *cutleft 0,1 , out *cuttop 0,1 , out *cutright 0,1 ,
  in cliptorect 0,1
*/
xtern void oswinexp mk_intersectionpointslinerect(
  double,double,double,double,double,double,double,double,double *,double *,double *,double *,
  double *,double *,double *,double *,int *,int *,int *,int *,int);

/*
  in polygon1.numbervertices , in *polygon1.px , in *polygon1.py ,
  in polygon2.numbervertices , in *polygon2.px , in *polygon2.py ,
  out *intersection.px , out *intersection.py , return numberintersectionpoints
*/
xtern int oswinexp mk_polygonintersection(
  int,double *,double *,int,double *,double *,double *,double *);

/*
  in line1.x1 , in line1.y1 , in line1.x2 , in line1.y2 ,
  in line2.x1 , in line2.y1 , in line2.x2 , in line2.y2 ,
  out *intersection.px , out *intersection.py ,
  in prec=3 , in xbox=0 0,1 , in ybox=0 0,1 , return 0,1
*/
xtern int oswinexp mk_linesintersection(
  double,double,double,double,double,double,double,double,double *,double *,int,int,int);

/*
  in *vertex-copy-from , out *vertex-copy-to , return 0,1
*/
xtern int oswinexp mk_vertexcopy(mk_vertex *,mk_vertex *);

/*
  in *vertex , return length
*/
xtern double oswinexp mk_vertexlen(mk_vertex *);

/*
  in *vertex-cmp1 , in *vertex-cmp2 , return -1,0,1
*/
xtern int oswinexp mk_vertexcmp(mk_vertex *,mk_vertex *);

/*
  inout *vertex-mod , in *vertex-addend  , return 0,1
*/
xtern int oswinexp mk_vertexadd(mk_vertex *,mk_vertex *);

/*
  inout *vertex-mod , in *vertexaddend  , return 0,1
*/
xtern int oswinexp mk_vertexsubs(mk_vertex *,mk_vertex *);

/*
  inout *vertex-mod , in multiplier  , return 0,1
*/
xtern int oswinexp mk_vertexmult(mk_vertex *,double);

/*
  inout *vertex-mod , in divisor  , return 0,1
*/
xtern int oswinexp mk_vertexdiv(mk_vertex *,double);

/*
  in *vertex1 , in *vertex2  , return dotproduct
*/
xtern double oswinexp mk_vertexdot(mk_vertex *,mk_vertex *);

/*
  inout *vertex-mod , return 0,1
*/
xtern int oswinexp mk_vertexnorm(mk_vertex *);

/*
  inout *mod-vertex , in *multiplier  , return 0,1
*/
xtern int oswinexp mk_vertexcross(mk_vertex *,mk_vertex *);

/*
  in *vertex1 , int *vertex2 , return angle(rad)-vertex1-vertex2
*/
xtern double oswinexp mk_vertexangrad(mk_vertex *,mk_vertex *);

/*
  in *vertex1 , in *vertex2 , return angle(deg)-vertex1-vertex2
*/
xtern double oswinexp mk_vertexangdeg(mk_vertex *,mk_vertex *);

/*
  inout list-of-vertices* , return 0,length
*/
xtern int oswinexp mk_verticesalloc(struct mk_vertices *);

/*
  inout list-of-vertices* , return 0,1
*/
xtern int oswinexp mk_verticesfree(struct mk_vertices *);

/*
  in list-of-vertices* , in arrindex , return item-at-index*
*/
xtern mk_vertex *oswinexp mk_verticesget(struct mk_vertices *,int);

/*
  inout list-of-*vertices , in *vertex , in maxcntvertices , return list-index
*/
xtern int oswinexp mk_verticesappend(struct mk_vertices *,mk_vertex *);

/*
  in list-of-*vertices , out list of derivatives (df/dx)[i], (df/dy)[i] , return 0,1
*/
xtern int oswinexp mk_der1(struct mk_vertices *,double *,double *);

/*
  inout list-of-*vertices , (df/dy)[i]=vertex[i]->xyzw[3] , return 0,1
*/
xtern int oswinexp mk_der1y(struct mk_vertices *);

/*
  inout list-of-*vertices , (d2f/dxdy)[i]=vertex[i]->xyzw[3] , return 0,1
*/
xtern int oswinexp mk_der2xy(struct mk_vertices *);

/*
  inout matrix* , return 0,size
*/
xtern int oswinexp mk_matrixalloc(struct mk_matrix *);

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
  in squarematrix , out decomposited squarematrix ,
  out row permutation (which row goes where) , out row interchangeables even(1)/odd(-1) 
*/
xtern int oswinexp mk_matrixludecomposition(struct mk_matrix *,struct mk_matrix *,int *, double *);

/*
  in squarematrix from ludecomposition, in row permutation ,
  in right hand side column vector , out result vector a : a0+a1x+a2x**2 ... anx**n
*/
xtern int oswinexp mk_matrixlubacksubstitution(struct mk_matrix *,int *,double *,double *);

/*
  in squarematrix , in right hand side column vector , 
  out result vector a : a0+a1x+a2x**2 ... anx**n
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
