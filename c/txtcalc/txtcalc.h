
#ifndef _txtcalc_h_
#define _txtcalc_h_

#include <mkbase/exportdefs.h>
#include <mkbase/defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* in number , in base , out *isint , return base */
xtern int oswinexp mk_txtchknum_(int,...);

/* 
  in type=+,-,*,/ , in str1 , in str2 , out str-result , 
  in result-maxlength , return result-length 
*/
xtern int oswinexp mk_txtcalc(unsigned char,const char *,const char *,char *,int);

/* in str1 , in str2 , out str-result , in result-maxlength , return result-length */
xtern int oswinexp mk_txtmod(const char *,const char *,char *,char *,int);

/* inout str , in fmt-in , in fmt-out , in result-maxlength , return result-fmt */
xtern int oswinexp mk_txtconvfmt(char *,int,int,int);

/* inout str , in round-pos , in result-maxlength , return result-length */
xtern int oswinexp mk_txtround(char *,int,int);

/* inout str , in result-maxlength , return result-length */
xtern int oswinexp mk_txtsimple2exp(char *,int);

/* inout str , in result-maxlength , return result-length */
xtern int oswinexp mk_txtexp2simple(char *,int);

#ifdef __cplusplus
}
#endif

#endif
