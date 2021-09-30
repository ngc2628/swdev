
#include <octave/oct.h>
#define Complex NagComplex
#define MatrixType NagMatrixType
#include <nag.h>
#include <nag_types.h>
#include <nag_stdlib.h>
#include <nage02.h>
#include <stdio.h>
#include <math.h>

DEFUN_DLD (Oe02dd, args, cntout,"nag_2d_spline_fit_scat\n")
{
  octave_value_list res;
  char buf[1024];
  memset(buf,0,1024);
  res(0)=octave_value(buf);
  res(1)=0;
  res(2)=0;
  res(3)=0;
  res(4)=0;
  res(5)=0;
  res(6)=0;

  Integer nn=args(0).int_value();
  NDArray ndxx=args(1).array_value();
  NDArray ndyy=args(2).array_value();
  NDArray ndzz=args(3).array_value();
  NDArray ndww=args(4).array_value();
  
  if (nn!= ndxx.length() ||
      nn!= ndyy.length() ||
      nn!= ndzz.length() ||
      nn!= ndww.length()) {
    sprintf (buf,"%s%d [%d - %d %d %d %d] length mismatch\n",
      __FILE__,__LINE__,nn,ndxx.length(),ndyy.length(),ndzz.length(),ndww.length());
    res(0)=octave_value(buf);
    return res;
  }
  
  double ss=args(5).double_value();
  if (ss<.0)
    ss=.0;
  
  double *xx=NAG_ALLOC(nn,double);
  double *yy=NAG_ALLOC(nn,double);
  double *zz=NAG_ALLOC(nn,double);
  double *ww=NAG_ALLOC(nn,double);
  Integer ii=0;
  for (ii=0;ii<nn;ii++) {
    xx[ii]=ndxx.elem(ii);
    yy[ii]=ndyy.elem(ii);
    zz[ii]=ndzz.elem(ii);
    ww[ii]=ndww.elem(ii);
  }
  
  Nag_Start start=Nag_Cold;
  NagError fail;
  INIT_FAIL(fail);
  Integer nxest=5+(int)(sqrt((double)nn)),
    nyest=nxest,splineclen=(nxest-4)*(nyest-4),rank=0;
  double fp=.0,wwinf=.0;
  Nag_2dSpline spline2d;
  spline2d.nx=0;
  spline2d.lamda=0;
  spline2d.ny=0;
  spline2d.mu=0;
  spline2d.c=0;

  e02ddc(start,nn,xx,yy,zz,ww,ss,nxest,nyest,&fp,&rank,&wwinf,&spline2d,&fail);
  
  if (fail.code!=NE_NOERROR) {
    sprintf (buf,"%s:%d %s\n",__FILE__,__LINE__,fail.message);
    res(0)=octave_value(buf);
  }
  
  res(1)=spline2d.nx;
  ndxx.resize1(nxest);
  for (ii=0;ii<nxest;ii++) 
    ndxx.elem(ii)=spline2d.lamda[ii];
  res(2)=ndxx;
  res(3)=spline2d.ny;
  ndyy.resize1(nyest);
  for (ii=0;ii<nyest;ii++) 
    ndyy.elem(ii)=spline2d.mu[ii];
  res(4)=ndyy;
  ndzz.resize1(splineclen);
  for (ii=0;ii<splineclen;ii++) 
    ndzz.elem(ii)=spline2d.c[ii];
  res(5)=ndzz;
  res(6)=fp;
  
  NAG_FREE(spline2d.c); 
  NAG_FREE(spline2d.mu);
  NAG_FREE(spline2d.lamda);
  NAG_FREE(ww);
  NAG_FREE(zz);
  NAG_FREE(yy);
  NAG_FREE(xx);
  
  return res;

} 
