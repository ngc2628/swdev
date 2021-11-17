
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>

int usage() {

  printf ("rgb2int color|[a,]r,g,b\n");
  return 0;

}

int main(int argc,char **argv) { 

  if (argc!=2) 
    return usage();
  char *tok=strtok(argv[1],",");
  if (!tok)
    return usage();
  mk_string str1;
  mk_string str2;
  mk_stringset(str1,0);
  mk_stringset(str2,0);
  int base=-1;
  mk_ulreal cc=mk_a2ui(tok,&base);
  unsigned short rr=0,gg=0,bb=0,aa=255;
  tok=strtok(0,",");
  if (!tok) {
    mk_ui2str(cc,str1,10,0,1,0);
    mk_ui2str(cc,str2,16,0,1,0);
    if ((cc>>24)>0)
      aa=(unsigned short)((cc>>24)&255);
    rr=(unsigned short)((cc>>16)&255);
    gg=(unsigned short)((cc>>8)&255);
    bb=(unsigned short)(cc&255);
    printf ("%s[%s] a,r,g,b [%u,%u,%u,%u]\n",
      (const char*)&str1[0],(const char*)&str2[0],aa,rr,gg,bb);
    return 0;
  }
  aa=(unsigned short)cc;
  base=-1;
  rr=(unsigned short)mk_a2ui(tok,&base);
  tok=strtok(0,",");
  int cnt=2;
  if (tok) {
    base=-1;
    gg=(unsigned short)mk_a2ui(tok,&base);
    cnt++;
    tok=strtok(0,",");
    if (tok) {
      base=-1;
      bb=(unsigned short)mk_a2ui(tok,&base);
      cnt++;
    }
  } 
  if (cnt==2) {
    bb=0;
    gg=rr;
    rr=aa;
    aa=255;
  }
  if (cnt==3) {
    bb=gg;
    gg=rr;
    rr=aa;
    aa=255;
  }
  cc=(unsigned int)(bb+(gg<<8)+(rr<<16)+(aa<<24));
  mk_ulreal ccc=(unsigned int)(bb+(gg<<8)+(rr<<16));
#if defined (_MSC_VER) || defined (__MINGW32__)
  printf ("%I64d[%I64d] a,r,g,b [%u,%u,%u,%u]\n",cc,ccc,aa,rr,gg,bb);
#else
  printf ("%lld[%lld] a,r,g,b [%u,%u,%u,%u]\n",cc,ccc,aa,rr,gg,bb);
#endif
    
  return 0;

}

