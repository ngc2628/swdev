
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <auxx/auxx.h>

extern int xxcalc(unsigned char,char *,char *,char *,int);
extern int xxround(char *,int,int);

int main(int argc, char **argv) {

  char num3[1024];
  memset(&num3[0],0,1024);
  int chklen=100;

  if (argc<4) {
    printf ("%d usage \"txtcalc num1 op num2\" [rd]\n",__LINE__);
    return 0;
  }

  printf ("%d calc : %s %s %s\n",__LINE__,argv[1],argv[2],argv[3]);
  //aux::xxcalc(argv[2][0],argv[1],argv[3],&num3[0],100);
  xxcalc(argv[2][0],argv[1],argv[3],&num3[0],chklen);
  printf ("%d res=[%d] %s\n",__LINE__,(int)strlen(num3),num3);
  if (argc>4) {
    xxround(num3,chklen,atoi(argv[4]));
    printf ("%d rnd=[%d] %s\n",__LINE__,(int)strlen(num3),num3);
  }


  printf ("%d ------------------------ \n",__LINE__);
  double chk1=0.85/3600.0;
  sprintf (num3,"%.16f",chk1);
  printf ("%d chk=%s\n",__LINE__,num3);
  double chk2=3600.0*atof(num3);
  printf ("%d chk=%.16f\n",__LINE__,chk2);
  
  return (0);

}
