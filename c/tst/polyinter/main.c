
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

static mk_vertex vvL1[4]={
  {2.,27.,.0,.0},{4.,149.,.0,.0},
  {5.,294.,.0,.0},{6.,519.,.0,.0}
};
static double tabledcoeff1[]={3.,-5.,7.,9.}; /* 3x*x*x+-5*x*x+7*x+9 */

static mk_vertex vvL2[3]={
  {2.,35.,.0,.0},{4.,57.,.0,.0},{5.,85.,.0,.0}
};
static double tabledcoeff2[]={5.666667,-23.,58.33333}; /* 5.6666*x*x-23*x+58.33333 */

static mk_vertex vvL3[2]={
  {2.,10.,.0,.0},{4.,13.,.0,.0}
};
static double tabledcoeff3[]={1.5,7.}; /* 1.5*x+7. */

static mk_vertex vvL4[1]={
  {2.,3.,.0,.0}
};
static double tabledcoeff4[]={3.}; /* 3. */

mk_vertex vvregr[8]={
  {-1.,-1.,.0,.0},{.0,3.,.0,.0},{1.,2.5,.0,.0},
  {2.,5.,.0,.0},{3.,4.,.0,.0},{5.,2.,.0,.0},
  {7.,5.,.0,.0},{9.,4.,.0,.0}
};
static int degree=4; 
/* -.01182*x*x*x*x+.2169*x*x*x+-1.233*x*x+2.301*x+2.686 */
static double tabledcoeffregr[]={-.01182,.2169,-1.233,2.301,2.686}; 

int fregr(struct mk_polynomial *poly) {

  double coeffL[5]={mk_dnan,mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  double cod=.0;
  int ii=0,len=8;
  mk_vertexzero(vv);
  mk_polynomialalloc(poly,len);
  for(ii=0;ii<len;ii++)
    mk_listsetat(&poly->ctrlL,(void*)vvregr[ii],ii,0);
  mk_polynomialfitdegr(poly,4,&coeffL[0],&cod);
  for(ii=0;ii<5;ii++) {
    printf ("[%9.5f]",coeffL[ii]);
  }
  printf("\n%d cod [%9.5f]\n",__LINE__,cod); 
  return 0;

}

int tstpolyeval(int len,double *coeffL,mk_vertex vertex) {

  vertex[1]=.0;
  while (--len>-1)
    vertex[1]=vertex[0]*vertex[1]+coeffL[len];
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0,len=4;
  double zero=.0,one=1.;
  mk_vertexzero(vv);
  double coeffL[6]={mk_dnan,mk_dnan,mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  // y=f(x)=9+7*x-5*x*x+3*x*x*x
  
  struct mk_polynomial poly;

  mk_polynomialalloc(&poly,len);
  for(ii=0;ii<len;ii++)
    mk_listsetat(&poly.ctrlL,(void*)vvL1[ii],ii,0);
  mk_polynomialcoeff(&poly,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%9.5f]",coeffL[ii]);
  }
  printf("\n%d\n",__LINE__);
  memset(&coeffL[0],0,6*sizeof(double));

  mk_polynomialfree(&poly);
  len=3;
  mk_polynomialalloc(&poly,len);
  for(ii=0;ii<len;ii++)
    mk_listsetat(&poly.ctrlL,(void*)vvL2[ii],ii,0);
  mk_polynomialcoeff(&poly,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%9.5f]",coeffL[ii]);
  }
  printf("\n%d\n",__LINE__);
  memset(&coeffL[0],0,6*sizeof(double));

  mk_polynomialfree(&poly);
  len=2;
  mk_polynomialalloc(&poly,len);
  for(ii=0;ii<len;ii++)
    mk_listsetat(&poly.ctrlL,(void*)vvL3[ii],ii,0);
  mk_polynomialcoeff(&poly,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%9.5f]",coeffL[ii]);
  }
  printf("\n%d\n",__LINE__);
  memset(&coeffL[0],0,6*sizeof(double));
  
  mk_polynomialfree(&poly);
  len=1;
  mk_polynomialalloc(&poly,len);
  mk_listsetat(&poly.ctrlL,(void*)vvL4[0],0,0);
  mk_polynomialcoeff(&poly,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%9.5f]",coeffL[ii]);
  }
  printf("\n%d\n",__LINE__);
  memset(&coeffL[0],0,6*sizeof(double));

  mk_polynomialfree(&poly);
  fregr(&poly);

  mk_polynomialfree(&poly);

  mk_vertex veval={11.,mk_dnan,mk_dnan,mk_dnan};
  tstpolyeval(4,&tabledcoeff1[0],veval);
  printf("%d [%9.5f %9.5f]\n",__LINE__,veval[0],veval[1]); 

  return 0;

}

