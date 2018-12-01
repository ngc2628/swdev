
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkbase.h>
#include <mkbase/mkla.h>

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  double zero=.0,one=1.;
  double coeff[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  // y=f(x)=9+7*x-5*x*x+3*x*x*x
  mk_vertex vv[4]={
    {2.,27.,mk_dnan,mk_dnan},{4.,149.,mk_dnan,mk_dnan},
    {5.,294.,mk_dnan,mk_dnan},{6.,519.,mk_dnan,mk_dnan}
  };
  struct mk_polynomial poly;
  poly.len=0;
  poly.ctrlL=0;
  poly.cc=0;
  poly.dd=0;

  poly.len=4;
  mk_polynomialalloc(&poly);
  memcpy(&poly.ctrlL[0],&vv[0],4*sizeof(mk_vertex));
  mk_polynomialcoeff(&poly,&coeff[0]);
  for(ii=0;ii<4;ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeff[ii]);

  mk_polynomialfree(&poly);
  poly.len=3;
  mk_polynomialalloc(&poly);
  vv[0][1]=35.;
  vv[1][1]=57.;
  vv[2][1]=85.;
  memcpy(&poly.ctrlL[0],&vv[0],3*sizeof(mk_vertex));
  mk_polynomialcoeff(&poly,&coeff[0]);
  for(ii=0;ii<3;ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeff[ii]);

  mk_polynomialfree(&poly);
  poly.len=2;
  mk_polynomialalloc(&poly);
  vv[0][1]=10.;
  vv[1][1]=13.;
  memcpy(&poly.ctrlL[0],&vv[0],2*sizeof(mk_vertex));
  mk_polynomialcoeff(&poly,&coeff[0]);
  for(ii=0;ii<2;ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeff[ii]);

  mk_polynomialfree(&poly);
  poly.len=1;
  mk_polynomialalloc(&poly);
  vv[0][1]=3.;
  memcpy(&poly.ctrlL[0],&vv[0],sizeof(mk_vertex));
  mk_polynomialcoeff(&poly,&coeff[0]);
  for(ii=0;ii<1;ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeff[ii]);

  return 0;

}

