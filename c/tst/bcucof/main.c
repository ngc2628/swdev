
#include <stdio.h>
#include <stdlib.h>
#include <tst/bcucof/bcu.h>
#include <mkbase/mkla.h>

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  struct mk_matrix cc;
  mk_matrixalloc(&cc,4,4);
  mk_vertex vv[]={{3.,4.,13.,.0},{8.,4.,21.,.0},{8.,11.,19.,.0},{3.,11.,23.,.0}};
  double dzx[]={1.,2.,3.,4.};
  double dzy[]={4.,3.,2.,1.};
  double dzxy[]={2.,1.,4.,3.};

  bcucof(&vv[0],&dzx[0],&dzy[0],&dzxy[0],&cc);
  
  mk_string strdbg;
  mk_stringset(strdbg,0);
  mk_matrixdbg(&cc,strdbg);
  printf("%s\n",&strdbg[0]);

  return 0;

}

