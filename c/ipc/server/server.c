
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#if defined (__BORLANDC__)
#include <windows.h>
#include <io.h>
#else
#ifndef WIN32
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#endif
#include <ipc/ipcutl.h>
#include <ipc/server/server.h>

int setupconn(struct IpcServer *server) {

  if (!server)
    return 1;
  server->connthread=0;
  server->sockfd=-1;
  server->sockfdaccepted=-1;
  server->host=0;
  server->port=0;
#ifdef WIN32
  WSADATA wsaData;
  int iResult=WSAStartup(MAKEWORD(2,2),&wsaData);
  if (iResult!=NO_ERROR) {
  }
#endif 
  return 0;  

}

int cleanupconn(struct IpcServer *server) {

  if (!server)
    return 1;
  closeconn(server);
  closefd(&server->sockfd);
#ifdef WIN32
  WSACleanup();
#endif 
  return 0;

}

static void writeclient(int fd,const char *str,int len) {

  if (!str || len<0) 
    send (fd,"\n",1,0);    
  else 
    send(fd,str,len,0);

}

static int readclient(int fd) {

  int len=0;
  memset (linebuf,0,bufsz);
  do {
    len=recv(fd,linebuf,bufsz-1,0); 
    //parsecommand(fd,line,len);
  } while (len>0);
  return len;

}

static void *runner(void *arg) {

  struct IpcServer *ipcserver=(struct IpcServer*)arg;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,0);
  struct sockaddr_in sin;
  sin.sin_family=AF_INET;
  sin.sin_port=0;
  sin.sin_addr.s_addr=INADDR_ANY;
  unsigned int len=sizeof(sin),rec=0;
  while (1) {
    sprintf(outbuf,"%d waiting on fd [%d]\n",__LINE__,ipcserver->sockfd);
    msgwrite(1,&outbuf[0]);
    ipcserver->sockfdaccepted=accept(ipcserver->sockfd,(struct sockaddr *)(&sin),&len);
    sprintf(outbuf,"%d connected on fd,port [%d,%d]\n",
      __LINE__,ipcserver->sockfdaccepted,ntohs(sin.sin_port));
    msgwrite(1,&outbuf[0]);
    if (ipcserver->sockfdaccepted>0) {
      readclient(ipcserver->sockfdaccepted);
      closefd(&ipcserver->sockfdaccepted);
    }
  }
  return 0;

}

int closeconn(struct IpcServer *server) {

  if (!server || server->connthread==0)
    return -1;
  pthread_cancel(server->connthread);
  pthread_join(server->connthread,0);
  server->connthread=0;
  closefd(&server->sockfdaccepted);
  return 0;

}

int newconn(struct IpcServer *server,unsigned int hh,unsigned short pp) {
  closeconn(server);
  closefd(&server->sockfd);
  server->host=(hh==0 ? (server->host==0 ? 2130706433 : server->host) : hh);
  server->port=(pp==0 ? (server->port==0 ? findnextport(1025) : server->port) : pp);
  if (server->port==0)
    return -1;
  struct in_addr inaddr;
  inaddr.s_addr=ntohl(server->host);
  char *hhbuf=inet_ntoa(inaddr);
  //char *hhbuf=xinet_ntoa(host,0);
  int ans=socket(AF_INET,SOCK_STREAM,0);
  if (ans<0)
    goto bailout;
  server->sockfd=ans;
  struct sockaddr_in sin;
  sin.sin_family=AF_INET;
  sin.sin_port=htons(server->port);
  sin.sin_addr.s_addr=INADDR_ANY;
  ans=bind(server->sockfd,(struct sockaddr*)(&sin),sizeof(sin));
  if (ans<0) 
    goto bailout;
  ans=listen(server->sockfd,2);
  if (ans<0) 
    goto bailout;
  sprintf(outbuf,"%d accept connections on host,fd,port [%s,%d,%d]\n",
    __LINE__,hhbuf,server->sockfd,server->port);
  msgwrite(1,&outbuf[0]);
  ans=pthread_create(&server->connthread,0,&runner,(void*)server);
  if (ans==0)
    ans=pthread_join(server->connthread,0);
bailout:
  if (ans<0) {
    msgpusherrno(__LINE__);
    msgwrite(2,&errbuf[0]);
  }
  closefd(&server->sockfd);
  return ans;

} 

