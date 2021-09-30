
#ifndef _mkana_h_
#define _mkana_h_

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
#include <mkbase/mkla.h>

/*
  polynomial as a set of control-vertex , ctrlL: list of vertex , cc,dd: intermediates, up-down 
  matrix as in walk-through neville-aitken tableau for calculation of interpolated value 
*/
struct oswinexp mk_polynomial {
  struct mk_list ctrlL;
  struct mk_matrix cc;
  struct mk_matrix dd;
};

/*
  cubic spline as a set of control-vertex , ctrlL: list of vertex , derL: intermediates, arr of 
  derivatives (after solving equations simultaniously) for calculation of interpolated value 
*/
struct oswinexp mk_spline {
  struct mk_list ctrlL;
  mk_ulreal options;
  double *derL;
};

#define mk_spline_solve1st MK_SH(1,30)
#define mk_spline_solve2nd MK_SH(1,31)
#define mk_splineboundary_notaknot MK_SH(1,33)
#define mk_splineboundary_natural MK_SH(1,34)
#define mk_splineboundary_periodic MK_SH(1,35)
#define mk_splineboundary_der1st MK_SH(1,36)
#define mk_spline_monotonic MK_SH(1,38)

#ifdef __cplusplus
/* #pragma message "cplusplus" */
extern "C" {
#endif

/*
  inout polynomial* , in length to be allocated (ctrlL) , return length
*/
xtern int oswinexp mk_polynomialalloc(struct mk_polynomial *,int);

/*
  inout polynomial* , return 0|1
*/
xtern int oswinexp mk_polynomialfree(struct mk_polynomial *);

/*
  in polynomial* , inout vertex: calc y4x from ctrlL with neville-aitken (cc,dd) , return 0|1
*/
xtern int oswinexp mk_polynomialinterpol(struct mk_polynomial *,mk_vertex);

/*
  in polynomial* , out arr of coeff (already allocated length|degree+1) , return 0|1
*/
xtern int oswinexp mk_polynomialcoeff(struct mk_polynomial *,double *);

/*
  in number coeff , in arr of coeff , inout vertex: calc y4x from list of coeff , return 0|1
*/
xtern int oswinexp mk_polynomialeval(int,double *,mk_vertex);

/*
  in polynomial* , in 0<degree<length , out arr of coeff (already allocated degree+1) ,
  out coefficient of determination* , return 0|1
*/
xtern int oswinexp mk_polynomialfitdegr(struct mk_polynomial *,int,double *,double *);

/*
  in number coeff , in arr of coeff , in interval-left , in interval-right ,
  out arr[number-of-coeff] of roots (already allocated) , return number roots
*/
int mk_polynomialrootscrude(int,double *,double,double,double *);

/*
  inout spline* , in length ctrlL to be allocated , return length
*/
xtern int oswinexp mk_cubicsplinealloc(struct mk_spline *,int);

/*
  inout spline* , return 0|1
*/
xtern int oswinexp mk_cubicsplinefree(struct mk_spline *);

/*
  inout spline* , return 0|1
*/
xtern int oswinexp mk_cubicsplineder(struct mk_spline *);

/*
  in x: x(i-1)<x<x(i) , in spline* , out arr[4] of coeff (already allocated) , return 0|1
*/
xtern int oswinexp mk_cubicsplinecoeff(double,struct mk_spline *spline,double *);

/*
  in spline* , inout vertex: calc y4x , return 0|1
*/
xtern int oswinexp mk_cubicsplineinterpol(struct mk_spline *,mk_vertex);

/*
  in spline* , inout vertex: calc y4x , return 0|1
*/
xtern int oswinexp mk_cubicsplineextrapol(struct mk_spline *,mk_vertex);

/*
  in list vertex* , in number interpol vertex , out list vertex*: calc y4x , return 0|1
*/
xtern int oswinexp mk_bezierinterpol(struct mk_list *,int,struct mk_list *);

#ifdef __cplusplus
}
#endif

#endif
