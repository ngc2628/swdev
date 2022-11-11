
#include <stdio.h>
#include <mkbase/mkutil.h>
#include <mkbase/mkconv.h>

int usage(char *msg) {

  if (msg) 
    fprintf (stderr,"\n%s\n",msg);
  fprintf (stderr,"\nusage : \n  tstconvint ");
  fprintf (stderr,"\"number-string\" \"-ibase int-input-base\" \"-obase int-output-base\"\n\n");

  return 0;

}

int main(int argc,char **argv) {

  if (argc<2) 
    return usage("arg fehlt");

  mk_string strnum;
  mk_stringset(strnum,argv[1]);
  int ibase=-1,obase=-1;
  if (argc>2) {
    if (!strncmp(argv[2],"--ibase",7)) {
      if (argc>3)
        ibase=atoi(argv[3]);
    }
    else if (!strncmp(argv[2],"--obase",7)) {
      if (argc>3)
        obase=atoi(argv[3]);
    }
  }
  if (argc>4) {
    if (!strncmp(argv[4],"--ibase",7)) {
      if (argc>5)
        ibase=atoi(argv[5]);
    }
    else if (!strncmp(argv[4],"--obase",7)) {
      if (argc>5)
        obase=atoi(argv[5]);
    }
  }

  if (ibase<2 || ibase>mk_maxintbase)
    ibase=-1;
  if (obase<2 || obase>mk_maxintbase)
    obase=-1;
  
  mk_ulreal inum=mk_a2ui(strnum,&ibase);
  
  char basebuf[3]={0,0,0};
  mk_string msg;
  if (ibase<2) {
    mk_stringset(msg,"cannot parse \"");
    mk_stringappend(msg,&strnum[0]);
    mk_stringappend(msg,"\" to any integer base");
    return usage(&msg[0]);
  }

  mk_stringset(msg,&strnum[0]);
  basebuf[0]=mk_basechar[ibase/10];
  basebuf[1]=mk_basechar[ibase-10*(ibase/10)];
  mk_stringappend(msg," (ibase=");
  mk_stringappend(msg,&basebuf[0]);
  mk_stringappend(msg,") -> ");

  mk_stringset(strnum,0);
  obase=mk_ui2a(inum,strnum,obase);

  mk_stringappend(msg,&strnum[0]);
  basebuf[0]=mk_basechar[obase/10];
  basebuf[1]=mk_basechar[obase-10*(obase/10)];
  mk_stringappend(msg," (obase=");
  mk_stringappend(msg,&basebuf[0]);
  mk_stringappend(msg,")");

  printf("%s\n",&msg[0]);

  return 0;

}

