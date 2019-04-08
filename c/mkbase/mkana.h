
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

struct oswinexp mk_polynomial {
  struct mk_list ctrlL;
  struct mk_matrix cc;
  struct mk_matrix dd;
};

#ifdef __cplusplus
/* #pragma message "cplusplus" */
extern "C" {
#endif

/*
  inout polynomial* , return 0,length
*/
xtern int oswinexp mk_polynomialalloc(struct mk_polynomial *,int);

/*
  inout polynomial* , return 0,1
*/
xtern int oswinexp mk_polynomialfree(struct mk_polynomial *);

/*
  in polynomial* , interpolation at x-value , return 0,1
*/
xtern double oswinexp mk_polynomialinterp(struct mk_polynomial *,double);

/*
  inout polynomial* , interpolation at x-value , intern slat 
*/
xtern int oswinexp mk_polynomialcoeff(struct mk_polynomial *,double *);

#ifdef __cplusplus
}
#endif

#endif
