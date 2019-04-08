
#include <stdio.h>
#include <stdlib.h>
#include <tools/tlist.h>
#include <osix/xxstyle.h>
#include <numeric/matrix.h>
#include <numeric/interpolation.h>

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  double zero=.0,one=1.;
  mk::TVList<double> coeffL;

  // y=f(x)=9+7*x-5*x*x+3*x*x*x
  double xx[4]={2.,4.,5.,6.};
  double yy[4]={27,149,294,519};

  double yyy[3]={35.,57.,85.};
  double yyyy[2]={10.,13.};
  double yyyyy[1]={3.};

  num::Polynomial pp;
  pp.setCtrl(4,&xx[0],&yy[0]);
  pp.coeff(.0,&coeffL);
  for(ii=0;ii<coeffL.count();ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeffL[ii]);

  coeffL.clear();
  pp.invalidate();
  pp.setCtrl(3,&xx[1],&yyy[0]);
  pp.coeff(.0,&coeffL);
  for(ii=0;ii<coeffL.count();ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeffL[ii]);

  coeffL.clear();
  pp.invalidate();
  pp.setCtrl(2,&xx[2],&yyyy[0]);
  pp.coeff(.0,&coeffL);
  for(ii=0;ii<coeffL.count();ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeffL[ii]);

  coeffL.clear();
  pp.invalidate();
  pp.setCtrl(1,&xx[3],&yyyyy[0]);
  pp.coeff(.0,&coeffL);
  for(ii=0;ii<coeffL.count();ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeffL[ii]);

  return 0;

}

