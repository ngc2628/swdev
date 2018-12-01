
#include <ipc/ipcutl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#ifdef WIN32
#include <wtypes.h>
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

int bufsz=4096;
char *inbuf=0;
char *outbuf=0;
char *errbuf=0;
char *hostbuf=0;
char *linebuf=0;
int dbgch=2;

int allocbuf() {

  if (!inbuf) 
    inbuf=(char *)malloc(bufsz);
  if (!outbuf) 
    outbuf=(char *)malloc(bufsz);
  if (!errbuf) 
    errbuf=(char *)malloc(bufsz);
  if (!hostbuf) 
    hostbuf=(char *)malloc(bufsz);
  if (!linebuf) 
    linebuf=(char *)malloc(bufsz);
  memset(&inbuf[0],0,bufsz);
  memset(&outbuf[0],0,bufsz);
  memset(&errbuf[0],0,bufsz);
  memset(&hostbuf[0],0,bufsz);
  memset(&linebuf[0],0,bufsz);

  return 0;

}

static char basechar[17]={
  '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','0'
};

static int bufpushint(int nn,int base,char *buf) {
  if (base!=2 && base!=8 && base!=16)
    base=10;
  int sgn=(nn<0 ? 1 : 0),ii=0,jj=0,ll=(int)strlen(buf),sp=ll+10,ep=sp;
  nn=abs(nn);
  do {
    buf[++ep]=basechar[(int)(nn%base)];
    nn/=base;
  } while (nn>0);
  if (sgn)
    buf[++ep]='-';
  for (ii=ep,jj=ll;ii>sp;ii--,jj++)
    buf[jj]=buf[ii];
  memset(&buf[sp+1],0,ep-sp);
  return jj;
}

int msgwrite(int fd,char *buf) {
  if (fd>0) {
#ifdef WIN32
    _write(fd,buf,strlen(buf));
#else
    write(fd,buf,strlen(buf));
#endif    
  }
  memset(buf,0,4096);
  return 0;
}

int msgpushint(int msg,char *buf) {
  return bufpushint(msg,10,buf);
}

int msgpusherrno(int ln) {
  msgpushint(ln,&errbuf[0]);
  strcat(errbuf," [");
#ifdef WIN32
  int errcode=WSAGetLastError();
  msgpushint(errcode,&errbuf[0]);
#else
  strncat(errbuf,strerror(errno),4000);
#endif
  strcat(errbuf,"]\n");
  return 0;
}

int closefd(int *fd) {
  if (!fd || (*fd)<0)
    return -1;
#ifdef WIN32
  shutdown(*fd,SD_BOTH);
  closesocket(*fd);
#else
  if (shutdown(*fd,SHUT_RDWR)<0)
    msgpusherrno(__LINE__);
  if (close(*fd)<0)
    msgpusherrno(__LINE__);
#endif
  if (strlen(errbuf)>0)
    msgwrite(2,&errbuf[0]);
  else {
    if (!outbuf) {
      outbuf=(char*)malloc(bufsz);
      memset(&outbuf[0],0,bufsz);
    }
    msgpushint(__LINE__,&outbuf[0]);
    strcat(&outbuf[0]," connection closed on [");
    msgpushint(*fd,&outbuf[0]);
    strcat(&outbuf[0],"]\n");
    msgwrite(2,&outbuf[0]);
  }
  *fd=-1;
  return 0;
}

unsigned short findnextport(unsigned short pp) {
  struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
  addr.sin_port=htons(pp);
  int ss=-1;
  while (pp<65535) {
    ss=socket(AF_INET,SOCK_STREAM,0);
    if (ss>0) {
      if (bind(ss,(struct sockaddr*)&addr,sizeof(addr))==0) 
        break;
      closefd(&ss);
    }
    addr.sin_port=htons(++pp);
  }
  closefd(&ss);
  return (pp>=65535 ? 0 : pp);
}

char *xinet_ntoa(unsigned int host,const char *order) {
  if (order && strlen(order)>0 && strncmp(order,"network",7)==0) {
    unsigned int hh=host;
    host=((hh>>24)&255);
    host+=(((hh>>16)&255)<<8);
    host+=(((hh>>8)&255)<<16);
    host+=((hh&255)<<24);
  }
  memset(&hostbuf[0],0,bufsz);
  bufpushint((host>>24)&255,10,hostbuf);
  strcat(hostbuf,".");
  bufpushint((host>>16)&255,10,hostbuf);
  strcat(hostbuf,".");
  bufpushint((host>>8)&255,10,hostbuf);
  strcat(hostbuf,".");
  bufpushint(host&255,10,hostbuf);
  return &hostbuf[0];
}
