
#include <octave/oct.h>
#define Complex NagComplex
#define MatrixType NagMatrixType
#include <nag.h>
#include <nag_types.h>
#include <nag_stdlib.h>
#include <nage02.h>
#include <stdio.h>

//[nX, lamdaX, cX] = e02be(start, s(k), x(k), w, smooth, n, lamda, wrk, iwrk);

DEFUN_DLD (Oe02be, args, cntout,"nag_1d_spline_fit\n")
{
  octave_value_list res;
  char buf[1024];
  memset(buf,0,1024);
  res(0)=octave_value(buf);
  res(1)=0;
  res(2)=0;
  res(3)=0;
  
  Integer nn=args(0).int_value();
  NDArray ndxx=args(1).array_value();
  NDArray ndyy=args(2).array_value();
  NDArray ndww=args(3).array_value();
  
  if (nn!= ndxx.length() || nn!= ndyy.length() || nn!= ndww.length()) {
    sprintf (buf,"%s:%d [%d %d %d %d] length mismatch\n",
      __FILE__,__LINE__,nn,ndxx.length(),ndyy.length(),ndww.length());
    res(0)=octave_value(buf);
    return res;
  }
  
  double ss=args(4).double_value(),sumsqres=0;
  if (ss<.0)
    ss=.0;
  
  double *xx=NAG_ALLOC(nn,double);
  double *yy=NAG_ALLOC(nn,double);
  double *ww=NAG_ALLOC(nn,double);
  Integer ii=0,nest=nn+4;
  for (ii=0;ii<nn;ii++) {
    xx[ii]=ndxx.elem(ii);
    yy[ii]=ndyy.elem(ii);
    ww[ii]=ndww.elem(ii);
  }
  
  Nag_Start start=Nag_Cold;
  NagError fail;
  INIT_FAIL(fail);
  Nag_Comm wwinf;
  wwinf.nag_w=0;
  wwinf.nag_iw=0;
  Nag_Spline spline;
  spline.n=0;
  spline.c=0;
  spline.lamda=0;
  
  e02bec(start,nn,xx,yy,ww,ss,nest,&sumsqres,&wwinf,&spline,&fail);
  
  if (fail.code!=NE_NOERROR) {
    sprintf (buf,"%s:%d %s\n",__FILE__,__LINE__,fail.message);
    res(0)=octave_value(buf);
  }
  
  res(1)=spline.n;
  ndxx.resize1(nest);
  for (ii=0;ii<nest;ii++) {
    ndxx.elem(ii)=spline.lamda[ii];
    if (ii<nn)
      ndyy.elem(ii)=spline.c[ii];
  }
  res(2)=ndxx;
  res(3)=ndyy;
   
  NAG_FREE(spline.lamda);
  NAG_FREE(spline.c);
  NAG_FREE(ww);
  NAG_FREE(yy);
  NAG_FREE(xx);
  
  return res;

} 