
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

  if (argc<3)
    return usage();

  double arg1=strtod(argv[1],0);
  double arg2=strtod(argv[2],0);

  int ii=0,jj=0;
  double zero=.0,one=1.;

  int gcd=mk_gcd((int)arg1,(int)arg2);

  printf("%d [%d] [%.f,%.f]\n",__LINE__,gcd,arg1/gcd,arg2/gcd);

  struct s1 {
    unsigned int xx:2;
  };
  struct s1 xx={2};
  printf("%d [%d]\n",__LINE__,xx.xx);

  xx.xx=7;

  printf("%d [%d]\n",__LINE__,xx.xx);

  return 0;

}

