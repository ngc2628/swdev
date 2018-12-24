
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  int rows=4,cols=4;
  struct mk_vertices vvL,der;
  mk_verticesalloc(&vvL,rows*cols);
  mk_verticesalloc(&der,rows*cols);

  mk_vertexnan(vv);
  vv[0]=2.;
  vv[1]=3.;
  vv[2]=11.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=3.;
  vv[1]=4.;
  vv[2]=15.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=5.;
  vv[1]=5.;
  vv[2]=14.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=6.;
  vv[1]=7.;
  vv[2]=16.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=2.;
  vv[1]=3.;
  vv[2]=17.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=3.;
  vv[1]=4.;
  vv[2]=12.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=5.;
  vv[1]=5.;
  vv[2]=13.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=6.;
  vv[1]=7.;
  vv[2]=10.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=2.;
  vv[1]=3.;
  vv[2]=15.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=3.;
  vv[1]=4.;
  vv[2]=22.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=5.;
  vv[1]=5.;
  vv[2]=20.;
  mk_verticesset(&vvL,ii++,vv);
  vv[0]=6.;
  vv[1]=7.;
  vv[2]=14.;
  mk_verticesset(&vvL,ii++,vv);
  
  mk_derivativesmixed(&vvL,&der,cols);
  for (ii=0;ii<rows*cols;ii+=cols) {
    printf("%d row#%d [%f,%f,%f,%f]\n",__LINE__,ii/cols,
      der.vertexL[ii][2],der.vertexL[ii+1][2],der.vertexL[ii+2][2],der.vertexL[ii+3][2]); 
  }

  return 0;

}

