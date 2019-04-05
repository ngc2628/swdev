
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>

struct CharStruct1 {
  char buf1[64];
  int l1;
  char *buf2;
  int l2;
};

struct CharStruct2 {
  char buf1[64];
  int l1;
  char buf2[32];
  int l2;
};

int usage() {

  printf ("\n");
  return 0;

}

int cmpdbl(const void *dbl1,const void *dbl2) {

  int res=0;
  const double *vvv1=(const double*)dbl1;
  const double *vvv2=(const double*)dbl2;

  if ((*vvv1)<(*vvv2))
    res=-1;
  else if ((*vvv2)<(*vvv1))
    res=1;
  return res;

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  struct mk_list list;
  mk_listalloc(&list,sizeof(struct CharStruct1),3);

  struct CharStruct1 charstruct11,charstruct12;
  strcpy(&charstruct11.buf1[0],"the quick brown fox");
  charstruct11.buf2=(char*)malloc(32);
  strcpy(&charstruct11.buf2[0],"jumps over the lazy dog");
  mk_listappend(&list,(void*)&charstruct11);
  strcpy(&charstruct11.buf2[0],"too bad");
  mk_listat(&list,0,(void*)&charstruct12);
  printf("%d [%s,%s]\n",__LINE__,charstruct12.buf1,charstruct12.buf2);

  mk_listfree(&list);
  mk_listalloc(&list,sizeof(struct CharStruct2),3);

  struct CharStruct2 charstruct21,charstruct22;
  strcpy(&charstruct21.buf1[0],"the quick brown fox");
  strcpy(&charstruct21.buf2[0],"jumps over the lazy dog");
  mk_listappend(&list,(void*)&charstruct21);
  strcpy(&charstruct21.buf2[0],"too bad");
  mk_listat(&list,0,(void*)&charstruct22);
  printf("%d [%s,%s]\n",__LINE__,charstruct22.buf1,charstruct22.buf2);
  
  return 0;

}

