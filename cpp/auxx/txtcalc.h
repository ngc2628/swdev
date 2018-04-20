
#ifndef _txtcalc_h_
#define _txtcalc_h_

#include <auxx/exportdef.h>

namespace aux {

extern int oswinexp txtchknum(const char *,int fmt=-1,int *i10=0);

extern int oswinexp txtcalc(unsigned char,const char *,const char *,char *,int);

extern int oswinexp txtmod(const char *,const char *,char *,char *,int);

extern int oswinexp txtconvfmt(char *,int,int,int);

extern int oswinexp txtround(char *,int,int);

extern int oswinexp txtsimple2exp(char *,int);

extern int oswinexp txtexp2simple(char *,int);

} // namespace

#endif
