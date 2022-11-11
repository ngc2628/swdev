
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int printuserinfo(char *prg) {

  prg ? printf ("execute \"%s\" as ",prg) : printf ("execute as user.group : ");

  char buf[256];
  memset(&buf[0],0,256);

  FILE *fp=popen("/usr/bin/id -zun","r");
  while (fgets(&buf[0],256,fp))
    printf("%s.",buf);
  pclose(fp);
  fp=popen("/usr/bin/id -zgn","r");
  while (fgets(&buf[0],256,fp))
    printf("%s\n",buf);
  pclose(fp);

  return 0;

}

int main(int argc, char **argv) {

  if (argc<3)
    return 1;

  if (!argv[1] || strlen(argv[1])<1 || !argv[2] || strlen(argv[2])<1)
    return 1;

  uid_t uid=atoi(argv[1]);
  gid_t gid=atoi(argv[2]);

	setgid(gid);
	setuid(uid);

  if (argc<4) 
    return printuserinfo(0);

  char **cmdargs=(char**)malloc((argc-2)*sizeof(char*));
  memset(&cmdargs[0],0,(argc-2)*sizeof(char*));
  int ii=0,jj=0,ll=0,larg=0;
  for (ii=3;ii<argc;ii++,jj++) {
    larg=strlen(argv[ii])+1;
    ll+=larg;
    cmdargs[jj]=(char*)malloc(larg);
    memset(&cmdargs[jj][0],0,larg);
    strcpy(cmdargs[jj],argv[ii]);
  }
  char *buf=(char*)malloc(ll+1);
  memset(&buf[0],0,ll+1);
  for (ii=0;ii<argc-3;ii++) {
    strcat(buf,cmdargs[ii]); 
    strcat(buf," ");
  }

  printuserinfo(buf);
  execvp(cmdargs[0],cmdargs);

  return 0;

}
