
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkana.h>

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0,len=4;
  double zero=.0,one=1.;
  double coeff[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  // y=f(x)=9+7*x-5*x*x+3*x*x*x
  mk_vertex vvL[4]={
    {2.,27.,mk_dnan,mk_dnan},{4.,149.,mk_dnan,mk_dnan},
    {5.,294.,mk_dnan,mk_dnan},{6.,519.,mk_dnan,mk_dnan}
  };
  double tabledcoeff1[]={3.,-5.,7.,9.}; /* 3x*x*x+-5*x*x+7*x+9 */
  struct mk_polynomial poly;
  mk_polynomialalloc(&poly,len);
  for(ii=0;ii<len;ii++)
    mk_listsetat(&poly.ctrlL,(void*)vvL[ii],ii,0);
  mk_polynomialcoeff(&poly,&coeff[0]);
  for(ii=0;ii<len;ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeff[ii]);

  mk_polynomialfree(&poly);
  len=3;
  mk_polynomialalloc(&poly,len);
  vvL[0][1]=35.;
  vvL[1][1]=57.;
  vvL[2][1]=85.;
  double tabledcoeff2[]={5.666667,-23.,58.33333}; /* 5.6666*x*x-23*x+58.33333 */
  for(ii=0;ii<len;ii++)
    mk_listsetat(&poly.ctrlL,(void*)vvL[ii],ii,0);
  mk_polynomialcoeff(&poly,&coeff[0]);
  for(ii=0;ii<len;ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeff[ii]);

  mk_polynomialfree(&poly);
  len=2;
  mk_polynomialalloc(&poly,len);
  vvL[0][1]=10.;
  vvL[1][1]=13.;
  double tabledcoeff3[]={1.5,7.}; /* 1.5*x+7. */
  for(ii=0;ii<len;ii++)
    mk_listsetat(&poly.ctrlL,(void*)vvL[ii],ii,0);
  mk_polynomialcoeff(&poly,&coeff[0]);
  for(ii=0;ii<len;ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeff[ii]);
  
  mk_polynomialfree(&poly);
  len=1;
  mk_polynomialalloc(&poly,len);
  vvL[0][1]=3.;
  double tabledcoeff4[]={3.}; /* 3. */
  for(ii=0;ii<len;ii++)
    mk_listappend(&poly.ctrlL,(void*)vvL[ii]);
  mk_polynomialcoeff(&poly,&coeff[0]);
  for(ii=0;ii<len;ii++)
    printf ("%d #%d [%f]\n",__LINE__,ii,coeff[ii]);

  mk_polynomialfree(&poly);

  return 0;

}

