
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

  int ii=0,jj=0,len=4;
  double zero=.0,one=1.;

  // y=f(x)=9+7*x-5*x*x+3*x*x*x
  double xx[4]={2.,4.,5.,6.};
  double yy[4]={27,149,294,519};
  double yyy[3]={35.,57.,85.};
  double yyyy[2]={10.,13.};
  double yyyyy[1]={3.};
  mk_vertexnan(vv);
  double coeff=.0;
  double coeffL[8]={mk_dnan,mk_dnan,mk_dnan,mk_dnan,mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  mk_list ctrlL;
  mk_listalloc(&ctrlL,sizeof(mk_vertex),len);
  num::Polynomial pp;

  for(ii=0;ii<len;ii++) {
    vv[0]=xx[ii];
    vv[1]=yy[ii];
    mk_listsetat(&ctrlL,(void*)&vv[0],ii,1);
  }
  pp.setCtrl(&ctrlL);
  pp.coeff(.0,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%7.3f] ",coeffL[ii]);
  }
  printf("%d\n",__LINE__);

  len--;
  memset(&coeffL[0],0,len*sizeof(double));
  mk_listclear(&ctrlL,0);
  for(ii=0;ii<len;ii++) {
    vv[0]=xx[ii];
    vv[1]=yyy[ii];
    mk_listsetat(&ctrlL,(void*)&vv[0],ii,1);
  }
  pp.setCtrl(&ctrlL);
  pp.coeff(.0,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%7.3f] ",coeffL[ii]);
  }
  printf("%d\n",__LINE__);

  len--;
  memset(&coeffL[0],0,len*sizeof(double));
  mk_listclear(&ctrlL,0);
  for(ii=0;ii<len;ii++) {
    vv[0]=xx[ii];
    vv[1]=yyyy[ii];
    mk_listsetat(&ctrlL,(void*)&vv[0],ii,1);
  }
  pp.setCtrl(&ctrlL);
  pp.coeff(.0,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%7.3f] ",coeffL[ii]);
  }
  printf("%d\n",__LINE__);

  len--;
  memset(&coeffL[0],0,len*sizeof(double));
  mk_listclear(&ctrlL,0);
  for(ii=0;ii<len;ii++) {
    vv[0]=xx[ii];
    vv[1]=yyyyy[ii];
    mk_listsetat(&ctrlL,(void*)&vv[0],ii,1);
  }
  pp.setCtrl(&ctrlL);
  pp.coeff(.0,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%7.3f] ",coeffL[ii]);
  }
  printf("%d\n",__LINE__);

  memset(&coeffL[0],0,len*sizeof(double));
  mk_listclear(&ctrlL,0);

  len=8;
  mk_vertex vvregr[8]={
    {-1.,-1.,.0,.0},{.0,3.,.0,.0},{1.,2.5,.0,.0},{2.,5.,.0,.0},
    {3.,4.,.0,.0},{5.,2.,.0,.0},{7.,5.,.0,.0},{9.,4.,.0,.0}
  };    
  mk_listrealloc(&ctrlL,len);
  for(ii=0;ii<len;ii++)
    mk_listsetat(&ctrlL,(void*)&vvregr[ii][0],ii,1);
  pp.setCtrl(&ctrlL);
  pp.setOptions(num::interpol_polynomial|num::interpol_regr|4);
  pp.coeff(.0,&coeffL[0]);
  for(ii=0;ii<len;ii++) {
    printf ("[%7.3f] ",coeffL[ii]);
  }
  printf("%d\n",__LINE__);

  mk_listfree(&ctrlL);

  return 0;

}

