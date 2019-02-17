
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>

int usage() {

  printf ("\n");
  return 0;

}

int cmpvertex(const void *vv1,const void *vv2) {

  int res=0;
  const mk_vertex *vvv1=(const mk_vertex*)vv1;
  const mk_vertex *vvv2=(const mk_vertex*)vv2;

  if ((*vvv1)[0]<(*vvv2)[0])
    res= -1;
  if ((*vvv2)[0]<(*vvv1)[0])
    res= 1;

/*printf("%d [%d] [%f,%f,%f,%f] [%f,%f,%f,%f]\n",__LINE__,res,
(*vvv1)[0],(*vvv1)[1],(*vvv1)[2],(*vvv1)[3],(*vvv2)[0],(*vvv2)[1],(*vvv2)[2],(*vvv2)[3]);*/

  return res;

}

int chklist() {

  struct mk_list list;
  int ii=0,jj=20,idxl=-1,idxh=-1,nitm=17,reserved=mk_listalloc(&list,sizeof(mk_vertex),nitm);
  printf("%d reserved [%d]\n",__LINE__,reserved); 
  list.cmp=cmpvertex;
  mk_vertex *vv=0;
  
  for (ii=0;ii<nitm;ii++) {
    vv=(mk_vertex*)malloc(sizeof(mk_vertex));
    (*vv)[0]=(double)jj--;
    (*vv)[1]=((double)jj)/2.;
    (*vv)[2]=(double)(jj*jj);
    (*vv)[3]=mk_dnan;
    if (mk_listappend(&list,vv)!=0)
      free(vv);
  } 
  mk_listsort(&list); 
  
  vv=(mk_vertex*)malloc(sizeof(mk_vertex));
  (*vv)[0]=15.8;
  (*vv)[1]=4.7;
  (*vv)[2]=3.6;
  (*vv)[3]=mk_dnan;
  idxl=mk_listinsort(&list,vv);
  idxl=mk_listinsort(&list,vv);

  printf("%d inserted at #%d\n",__LINE__,idxl);

  if (idxl<0)
    free(vv);

  vv=(mk_vertex*)malloc(sizeof(mk_vertex));
  (*vv)[0]=6.1;
  (*vv)[1]=7.2;
  (*vv)[2]=8.3;
  (*vv)[3]=mk_dnan;
  idxl=mk_listinsort(&list,vv);
  idxl=mk_listinsort(&list,vv);

  printf("%d inserted at #%d\n",__LINE__,idxl);

  if (idxl<0)
    free(vv);

  mk_vertexnan(look);
  look[0]=15.8;
  ii=mk_listfind(&list,&look,&idxl,&idxh);
  mk_listat(&list,idxl,vv);
  printf("%d #%d,%d,%d [%f,%f,%f,%f]\n",__LINE__,ii,idxl,idxh,(*vv)[0],(*vv)[1],(*vv)[2],(*vv)[3]);

  for (ii=0;ii<reserved;ii++) {
    mk_listat(&list,ii,vv);
    if (vv)
      printf("%d #%d [%f,%f,%f,%f]\n",__LINE__,ii,(*vv)[0],(*vv)[1],(*vv)[2],(*vv)[3]); 
    else 
      printf("%d #%d(%d) no item\n",__LINE__,ii,list.count);
  }

  mk_listfree(&list);
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  chklist();

  int rows=4,cols=4;
  struct mk_list vvL,der;
  mk_listalloc(&vvL,sizeof(mk_vertex),rows*cols);
  mk_listalloc(&der,sizeof(mk_vertex),rows*cols);

  mk_vertexnan(vv);
  vv[0]=2.;
  vv[1]=3.;
  vv[2]=11.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=3.;
  vv[1]=4.;
  vv[2]=15.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=5.;
  vv[1]=5.;
  vv[2]=14.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=6.;
  vv[1]=7.;
  vv[2]=16.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=2.;
  vv[1]=3.;
  vv[2]=17.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=3.;
  vv[1]=4.;
  vv[2]=12.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=5.;
  vv[1]=5.;
  vv[2]=13.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=6.;
  vv[1]=7.;
  vv[2]=10.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=2.;
  vv[1]=3.;
  vv[2]=15.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=3.;
  vv[1]=4.;
  vv[2]=22.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=5.;
  vv[1]=5.;
  vv[2]=20.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);
  vv[0]=6.;
  vv[1]=7.;
  vv[2]=14.;
  mk_listsetat(&vvL,(void*)&vv,ii++,1);

  vvL.cmp=mk_vertexcmpx;
  mk_listsort(&vvL);
  for (ii=0;ii<vvL.count;ii++) {
    mk_listat(&vvL,ii,&vv);
    printf("%d [%f,%f,%f,%f]\n",__LINE__,vv[0],vv[1],vv[2],vv[3]); 
  }
  
  /*mk_derivativesmixed(&vvL,&der,cols);
  for (ii=0;ii<rows*cols;ii+=cols) {
    printf("%d row#%d [%f,%f,%f,%f]\n",__LINE__,ii/cols,
      der.vertexL[ii][2],der.vertexL[ii+1][2],der.vertexL[ii+2][2],der.vertexL[ii+3][2]); 
  }*/

  mk_listfree(&der);
  mk_listfree(&vvL);

  return 0;

}

