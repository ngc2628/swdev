
#include <octave/oct.h>
#define Complex NagComplex
#define MatrixType NagMatrixType
#include <nag.h>
#include <nag_types.h>
#include <nag_stdlib.h>
#include <nage02.h>
#include <stdio.h>
#include <math.h>

DEFUN_DLD (Oe02de, args, cntout,"nag_2d_spline_eval\n")
{
  octave_value_list res;
  char buf[1024];
  memset(buf,0,1024);
  res(0)=octave_value(buf);
  res(1)=0;
  
  Integer nn=args(0).int_value();
  NDArray ndxx=args(1).array_value();
  NDArray ndyy=args(2).array_value();
  NDArray ndlamda=args(3).array_value();
  NDArray ndmu=args(4).array_value();
  NDArray ndc=args(5).array_value();
  Integer nx=args(6).int_value();
  Integer ny=args(7).int_value();
  
  if (nn!= ndxx.length() || nn!= ndyy.length()) {
    sprintf (buf,"%s:%d [%d - %d %d] length mismatch\n",
      __FILE__,__LINE__,nn,ndxx.length(),ndyy.length());
    res(0)=octave_value(buf);
    return res;
  }
  
  double *xx=NAG_ALLOC(nn,double);
  double *yy=NAG_ALLOC(nn,double);
  double *zz=NAG_ALLOC(nn,double);
  Integer ii=0,jj=0,kk=0,splineclen=(nx-4)*(ny-4);
  for (ii=0;ii<nn;ii++) {
    xx[ii]=ndxx.elem(ii);
    yy[ii]=ndyy.elem(ii);
    zz[ii]=.0;
  }

  NagError fail;
  INIT_FAIL(fail);
  Nag_2dSpline spline2d;
  spline2d.nx=nx;
  spline2d.lamda=NAG_ALLOC(nx,double);
  for (ii=0;ii<nx;ii++)
    spline2d.lamda[ii]=ndlamda.elem(ii);
  spline2d.ny=ny;
  spline2d.mu=NAG_ALLOC(ny,double);
  for (ii=0;ii<ny;ii++)
    spline2d.mu[ii]=ndmu.elem(ii);
  spline2d.c=NAG_ALLOC(splineclen,double);
  for (ii=0;ii<nx-4;ii++) {
    for (jj=0;jj<ny-4;jj++) 
      spline2d.c[(ny-4)*ii+jj]=ndc.elem(kk++);
  }
  
  e02dec(nn,xx,yy,zz,&spline2d,&fail);
  
  if (fail.code!=NE_NOERROR) {
    sprintf (buf,"%d %s\n",__LINE__,fail.message);
    res(0)=octave_value(buf);
  }
  
  for (ii=0;ii<nn;ii++)
    ndxx.elem(ii)=zz[ii];
  res(1)=ndxx;
    
  NAG_FREE(spline2d.c); 
  NAG_FREE(spline2d.mu);
  NAG_FREE(spline2d.lamda);
  NAG_FREE(zz);
  NAG_FREE(yy);
  NAG_FREE(xx);
  
  return res;

} 