
#include <stdio.h>
#include <stdlib.h>
#include <auxx/auxx.h>
#include <osix/xxstyle.h>

int usage() {

  printf ("rgb2int color|r,b,g[,a]\n");
  return 0;

}

int main(int argc,char **argv) { 

  if (argc!=2) 
    return usage();
  char *tok=strtok(argv[1],",");
  if (!tok)
    return usage();
  aux::Asciistr str1,str2;
  int base=-1;
  mk_ulreal cc=mk_a2ui(tok,&base);
  unsigned short rr=0,gg=0,bb=0,aa=255;
  tok=strtok(0,",");
  if (!tok) {
    aux::ui2a(cc,&str1,10);
    aux::ui2a(cc,&str2,16);
    osix::xxrgba((unsigned int)cc,&rr,&gg,&bb,&aa);
    printf ("color=%s[%s] r=%u g=%u b=%u a=%u\n",(const char*)str1,(const char*)str2,rr,gg,bb,aa);
    return 0;
  }
  rr=(unsigned short)cc;
  base=-1;
  gg=(unsigned short)mk_a2ui(tok,&base);
  tok=strtok(0,",");
  if (tok) {
    base=-1;
    bb=(unsigned short)mk_a2ui(tok,&base);
    tok=strtok(0,",");
    if (tok) {
      base=-1;
      aa=(unsigned short)mk_a2ui(tok,&base);
    }
  } 
  cc=osix::xxcolor(rr,gg,bb,aa);

  printf ("color=%lld r=%u g=%u b=%u a=%u\n",cc,rr,gg,bb,aa);
    
  return 0;

}

