
#include <octave/oct.h>
#include <numeric/interpolation.h>
#include <stdio.h>

DEFUN_DLD (xxspline, args, cntout,"xxspline\n")
{
  octave_value_list res;
  
  NDArray ndxx=args(0).array_value();
  NDArray ndyy=args(1).array_value();
  charNDArray ndopt1=args(2).char_array_value();
  charNDArray ndopt2=args(3).char_array_value();
  charNDArray ndopt3=args(4).char_array_value();
  int ii=0,nnxx=ndxx.length(),nnyy=ndyy.length(),
    nnopt1=ndopt1.length(),nnopt2=ndopt2.length(),nnopt3=ndopt3.length();
  
  if (nnxx!=nnyy) {
    printf ("%d [%d %d] length mismatch\n",__LINE__,nnxx,nnyy);
    return res;
  }
  
  double *xx=(double*)malloc(nnxx*sizeof(double));
  double *yy=(double*)malloc(nnxx*sizeof(double));
  for (ii=0;ii<nnxx;ii++) {
    xx[ii]=ndxx.elem(ii);
    yy[ii]=ndyy.elem(ii);
  }
  aux::TVList<aux::Asciistr> optL(3);
  aux::Asciistr opt;
  opt.reserve(nnopt1+1);
  for (ii=0;ii<nnopt1;ii++)
    opt.append(ndopt1.elem(ii));
  optL.append(opt);
  opt=(char*)0;
  opt.reserve(nnopt2+1);
  for (ii=0;ii<nnopt2;ii++)
    opt.append(ndopt2.elem(ii));
  optL.append(opt);
  opt=(char*)0;
  opt.reserve(nnopt3+1);
  for (ii=0;ii<nnopt3;ii++)
    opt.append(ndopt3.elem(ii));
  optL.append(opt);
      
  double *der=(double*)malloc(nnxx*sizeof(double));
  memset(&der[0],0,nnxx*sizeof(double));
  
  num::Spline sp(&optL);
  sp.setCtrl(nnxx,xx,yy);
  sp.makeSpline(der);
  
  NDArray ndder;
  ndder.resize1(nnxx);
  for (ii=0;ii<nnxx;ii++)
    ndder.elem(ii)=der[ii];
  res(0)=ndder;
  
  free(der);
  free(yy);
  free(xx);
  
  return res;

} 