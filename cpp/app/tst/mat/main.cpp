
#include <stdio.h>
#include <stdlib.h>
#include <tools/tlist.h>
#include <osix/xxstyle.h>
#include <numeric/matrix.h>

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  double mm44[4][4]={
    {10.,3.,13.,7.},{4.,2.,1.,11.},{6.,5.,15.,9.},{8.,16.,14.,12.}
  };
  double mm44inv[4][4]={
    {.19208,.00733,-.18704,.02152},{-.02061,-.03297,-.06456,.09066},
    {-.03205,-.05128,.12179,-.02564},{-.06319,.09890,.06868,-.02198}
  };
  double mm44det=-8736.;

  num::SquareMatrix mm(4);
  for (ii=0;ii<mm.rows();ii++) {
    for (jj=0;jj<mm.cols();jj++)
      mm.setValue(ii,jj,mm44[ii][jj]);
  }

  printf("%d det [%f]\n",__LINE__,mm.determinant());
  
  printf("%d [%d]\n",__LINE__,mm.invert());

  for (ii=0;ii<mm.rows();ii++) {
    for (jj=0;jj<mm.cols();jj++)
      printf("%9.5f",mm.getValue(ii,jj));
    printf("\n");
  }
  printf("\n");

  for (ii=0;ii<4;ii++) {
    for (jj=0;jj<4;jj++)
      printf("%9.5f",mm44inv[ii][jj]);
    printf("\n");
  }
  printf("\n");

  return 0;

}

