
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <tst/string/decl.h>

typedef unsigned long long int tst_ulreal;
#define tst_sz 128
typedef char tst_string[tst_sz];

int tst_ui2a_(int cnt,...) {

  va_list valist;
  va_start(valist,cnt);
  tst_ulreal number=(cnt>0 ? va_arg(valist,tst_ulreal) : 0);
  char *str=(cnt>1 ? va_arg(valist,char*) : 0);
  int base=(cnt>2 ? va_arg(valist,int) : 10);
  int width=(cnt>3 ? va_arg(valist,int) : 0);
  int padzero=(cnt>4 ? va_arg(valist,int) : 1);
  char *group=(cnt>5 ? va_arg(valist,char *) : 0);
  va_end(valist);

  tst_string tststr;
  strcpy(&tststr[0],"jumps over the lazy dog");

  if (str) {
#if defined (_MSC_VER) || defined (__MINGW32__)
    sprintf(str,"%I64d",number);
#else
    sprintf(str,"%lld",number);
#endif
    strcat(str,"[");
    strcat(str,&tststr[0]);
    strcat(str,"]");
  }
  printf("%d [%d]\n",__LINE__,base);
    
  return 0;

}

#define _ARG6(_0,_1,_2,_3,_4,_5,_6,...) _6
#define NARG6(...) _ARG6(__VA_ARGS__,6,5,4,3,2,1,0)
#define tst_ui2a(...) tst_ui2a_(NARG6(__VA_ARGS__),__VA_ARGS__)

/* #define tst_pundnan */

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  printf("%d [%f] [%f]\n",__LINE__,tst_dnann,tst_dinff);
#if defined (_MSC_VER) || defined (__MINGW32__)
  printf("%d [%f] [%I64d]\n",__LINE__,tst_dnan,*((unsigned long long int*)&tst_dnan));
#else
  printf("%d [%f] [%lld]\n",__LINE__,tst_dnan,*((unsigned long long int*)&tst_dnan));
#endif
  printf("%d [%f]\n",__LINE__,tst_dsnan);
#if defined (_MSC_VER) || defined (__MINGW32__)
  printf("%d [%f] [%I64d]\n",__LINE__,tst_dinf,*((unsigned long long int*)&tst_dinf));
#else
  printf("%d [%f] [%lld]\n",__LINE__,tst_dinf,*((unsigned long long int*)&tst_dinf));
#endif
  printf("%d [%f]\n",__LINE__,tst_dsinf);

  tst_ulreal tstnum=1234;
  tst_string tststr;
  memset(&tststr[0],0,tst_sz);
  strcpy(&tststr[0],"the quick brown fox");

  tst_ui2a(1234,&tststr[0],33);

  printf ("%d [%s]\n",__LINE__,&tststr[0]);


  

  return 0;

}

