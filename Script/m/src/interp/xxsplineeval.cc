
#include <octave/oct.h>
#include <numeric/interpolation.h>
#include <stdio.h>

DEFUN_DLD (xxsplineeval, args, cntout,"xxsplineeval\n")
{
  octave_value_list res;

  double xxeval=args(0).double_value();
  NDArray ndxx=args(1).array_value();
  NDArray ndyy=args(2).array_value();
  NDArray ndder=args(3).array_value();
  charNDArray ndopt1=args(4).char_array_value();
  charNDArray ndopt2=args(5).char_array_value();
  charNDArray ndopt3=args(6).char_array_value();
  int ii=0,nnxx=ndxx.length(),
    nnopt1=ndopt1.length(),nnopt2=ndopt2.length(),nnopt3=ndopt3.length();

  if (nnxx!=ndyy.length() || nnxx!=ndder.length()) {
    printf ("%d [%d %d %d] length mismatch\n",__LINE__,
      nnxx,ndyy.length(),ndder.length());
    return res;
  }

  double *xx=(double*)malloc(nnxx*sizeof(double));
  double *yy=(double*)malloc(nnxx*sizeof(double));
  double *der=(double*)malloc(nnxx*sizeof(double));
  for (ii=0;ii<nnxx;ii++) {
    xx[ii]=ndxx.elem(ii);
    yy[ii]=ndyy.elem(ii);
    der[ii]=ndder.elem(ii);
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

  num::Spline sp(&optL);
  sp.setCtrl(nnxx,xx,yy);
  int ok=sp.setSpline(der);

  if (ok!=0) {
    printf ("%d spline failed\n",__LINE__);
    return res;
  }

  res(0)=sp.extrap(xxeval);

  free(der);
  free(yy);
  free(xx);

  return res;

}
