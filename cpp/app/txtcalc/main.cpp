
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <auxx/txtcalc.h>

//extern int xxcalc(unsigned char,char *,char *,char *,int);
//extern int xxround(char *,int,int);

static const int ncmdopt=7;
static const char *cmdopt[ncmdopt]={"chk","e2s","s2e","mod","fmt","quit","exit"};
static const int ntok=5;
static const int ltok=1000;
static const int maxlen=1024;

int hout(int once) {

  char optbuf[ltok];
  memset(&optbuf[0],0,ltok);
  optbuf[0]='\"';
  if (once==1) {
    printf ("usage \"txtcalc :\n");
    optbuf[once]='-';
  }
  strcat(optbuf,"chk num [fmt]\"\n");
  printf(optbuf);
  memset(&optbuf[1+once],0,ltok-1-once);
  strcat(optbuf,"e2s num\"\n");
  printf(optbuf);
  memset(&optbuf[1+once],0,ltok-1-once);
  strcat(optbuf,"s2e num\"\n");
  printf(optbuf);
  memset(&optbuf[1+once],0,ltok-1-once);
  strcat(optbuf,"mod num1 num2\"\n");
  printf(optbuf);
  memset(&optbuf[1+once],0,ltok-1-once);
  strcat(optbuf,"fmt num fmt1 fmt2\"\n");
  printf(optbuf);
  memset(&optbuf[1],0,ltok-1);
  strcpy(optbuf,"num1 op num2 [rd] [bin|oct|hex|dec*|exp]\"\n");
  printf(optbuf);

  return 0;

}

int chkcmd(char *cmdline,char *tokL[ntok]) {

  if (cmdline && strlen(cmdline)>0 && cmdline[strlen(cmdline)-1]=='\n')
    cmdline[strlen(cmdline)-1]=0;
  int ii=0,ll=0,cmp=0,idx=-1;
  char *tok=strtok(cmdline," ");
  while (tok && ll<ntok) {
    if (strlen(tok)>0)
      strncpy(tokL[ll++],tok,ltok-1);
    tok=strtok(0," \t");
  }

  if (ll==0)
    return -1;

  for (ii=0;ii<ncmdopt;ii++) {
    cmp=strcmp(cmdopt[ii],tokL[0]);
    if (cmp==0) {
      idx=ii;
      break;
    }
  }

  if (idx==0) {
    if (ll<2) {
      printf ("chk num [fmt]\n");
      printf ("check for input as a number in common style ");
      printf ("(integer base 2-36, float simple or exp)\n");
      idx|=16;
    }
  }

  else if (idx==1) {
    if (ll<2) {
      printf ("e2s num\n");
      printf ("convert floating number with 10-exp format to simple format\n");
      idx|=16;
    }
  }

  else if (idx==2) {
    if (ll<2) {
      printf ("s2e num\n");
      printf ("convert floating number simple format to 10-exp format\n");
      idx|=16;
    }
  }

  else if (idx==3) {
    if (ll<3) {
      printf ("mod num1 num2\n");
      printf ("int-modulo num1/num2\n");
      idx|=16;
    }
  }

  else if (idx==4) {
    if (ll<4) {
      printf ("fmt num fmt1 fmt2\n");
      printf ("format convertion, fmt1 and fmt2 integer base 2-36\n");
      idx|=16;
    }
  }

  else if (idx!=5 && idx!=6) {
    if (ll<3) {
      printf ("num1 op num2 [rd]  [bin|oct|hex|dec*|exp]\n");
      printf ("simple base-10 calculation, op=[+|-|*|/], optional position round (int) and/or optional format\n");
    }
    else
      idx=7;
  }

  return idx;

}

int docalc(char *cmd[ntok],int ll) {

  int ret=-1;

  if (strcmp(cmd[0],cmdopt[0])==0) {
    int fmt=-1,i10=0;
    if (strlen(cmd[2])>0)
      fmt=atoi(cmd[2]);
    ret=aux::txtchknum(cmd[1],fmt,&i10);
    printf ("%s\n",ret<0 ? "nan" : (i10==1 ? "int" : "float"));
  }
  else if (strcmp(cmd[0],cmdopt[1])==0) {
    ret=aux::txtexp2simple(cmd[1],ll-1);
    printf ("%s\n",cmd[1]);
  }
  else if (strcmp(cmd[0],cmdopt[2])==0) {
    ret=aux::txtsimple2exp(cmd[1],ll-1);
    printf ("%s\n",cmd[1]);
  }
  else if (strcmp(cmd[0],cmdopt[3])==0) {
    char *div=(char*)malloc(maxlen);
    memset(&div[0],0,maxlen);
    char *carry=(char*)malloc(maxlen);
    memset(&carry[0],0,maxlen);
    ret=aux::txtmod(cmd[1],cmd[2],div,carry,ll-1);
    printf ("%s [%s]\n",carry,div);
    free(carry);
    free(div);
  }
  else if (strcmp(cmd[0],cmdopt[4])==0) {
    int fmt1=atoi(cmd[2]),fmt2=atoi(cmd[3]);
    ret=aux::txtconvfmt(cmd[1],fmt1,fmt2,ll-1);
    printf ("%s\n",cmd[1]);
  }
  else {
    char *res=(char*)malloc(maxlen);
    memset(&res[0],0,maxlen);
    ret=aux::txtcalc((unsigned char)cmd[1][0],cmd[0],cmd[2],res,ll-1);
    int chkfmt=0;
    if (strlen(cmd[3])>0) {
      if (aux::txtchknum(cmd[3],10)==10) {
        aux::txtround(res,ltok,atoi(cmd[3]));
        if (strlen(cmd[4])>0)
          chkfmt=4;
      }
      else
        chkfmt=3;
    }
    if (chkfmt>0) {
      if (strcmp(cmd[chkfmt],"bin")==0)
        aux::txtconvfmt(res,10,2,ll-1);
      else if (strcmp(cmd[chkfmt],"oct")==0)
        aux::txtconvfmt(res,10,8,ll-1);
      else if (strcmp(cmd[chkfmt],"hex")==0)
        aux::txtconvfmt(res,10,16,ll-1);
      else if (strcmp(cmd[chkfmt],"exp")==0) {
        aux::txtsimple2exp(res,ll-1);
      }
    }
    printf ("%s\n",res);
  }

  return ret;

}

int main(int argc, char **argv) {

  int ii=0,ll=0,once=1;
  if (argc<2)
    once=0;

  if (once==1) {
    ll=(int)strlen(argv[1]);
    if (ll<2)
      return hout(1);
    memmove(&argv[1][0],&argv[1][1],--ll);
    argv[1][ll]=0;
  }

  char *cmdL[ntok]={0,0,0,0};
  for (ii=0;ii<ntok;ii++) {
    cmdL[ii]=(char*)malloc(maxlen);
    memset(&cmdL[ii][0],0,maxlen);
  }
  int lmaxinp=ntok*ltok+4,linp=0;
  char *input=(char*)malloc(lmaxinp);
  memset(&input[0],0,lmaxinp);

  if (once==1) {
    for (ii=1;ii<ntok+1 && ii<argc;ii++) {
      strncat(input,argv[ii],ltok-1);
      if (ii<argc-1 && ii<ntok)
        strcat(input," ");
    }
  }
  else {
    printf (">");
    fgets(input,lmaxinp-1,stdin);
  }

  int res=chkcmd(input,cmdL);
  if (res>=0) {
    if (once==0 && (res==5 || res==6))
      return 0;
    if ((res&16)==0)
      docalc(cmdL,ltok);
  }
  else
    hout(once);

  while (once==0) {
    memset(&input[0],0,lmaxinp);
    printf (">");
    fgets(input,lmaxinp-1,stdin);
    res=chkcmd(input,cmdL);
    if (res==5 || res==6)
      once=1;
    else if (res>=0 && (res&16)==0)
      docalc(cmdL,maxlen);
    else
      hout(once);
    for (ii=0;ii<ntok;ii++)
      memset(&cmdL[ii][0],0,ltok);
  }

  return 0;

}
