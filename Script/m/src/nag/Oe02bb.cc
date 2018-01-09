
#include <octave/oct.h>
#define Complex NagComplex
#define MatrixType NagMatrixType
#include <nag.h>
#include <nag_types.h>
#include <nag_stdlib.h>
#include <nage02.h>
#include <stdio.h>

DEFUN_DLD (Oe02bb, args, cntout,"nag_1d_spline_evaluate\n")
{
  octave_value_list res;
  char buf[1024];
  memset(buf,0,1024);
  res(0)=octave_value(buf);
  res(1)=0;
  
  Integer nn=args(0).int_value();
  NDArray ndlamda=args(1).array_value();
  NDArray ndcc=args(2).array_value();
  double xx=args(3).double_value(),yy=.0;
  
  if (nn!=ndlamda.length() || nn-4!=ndcc.length()) {
    sprintf (buf,"%s:%d %d length mismatch [%d %d]\n",
      __FILE__,__LINE__,nn,ndlamda.length(),ndcc.length());
    res(0)=octave_value(buf);
    return res;
  }
  
  double *lamda=NAG_ALLOC(nn,double);
  double *cc=NAG_ALLOC(nn-4,double);
  Integer ii=0;
  for (ii=0;ii<nn;ii++) {
    lamda[ii]=ndlamda.elem(ii);
    if (ii<nn-4)
      cc[ii]=ndcc.elem(ii);
  }
  
  NagError fail;
  INIT_FAIL(fail);
  Nag_Spline spline;
  spline.n=nn;
  spline.c=cc;
  spline.lamda=lamda;
  
  e02bbc(xx,&yy,&spline,&fail);
  
  if (fail.code!=NE_NOERROR) {
    sprintf (buf,"%s:%d %s\n",__FILE__,__LINE__,fail.message);
    res(0)=octave_value(buf);
  }
  
  res(1)=yy;
     
  NAG_FREE(lamda);
  NAG_FREE(cc);
  
  return res;

} 