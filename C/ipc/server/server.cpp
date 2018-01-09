
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <ipc/ipcutl.h>
#include <ipc/server/server.h>

namespace ipc {

IpcServer::IpcServer(unsigned int hh,unsigned short pp) : 
  connthread(0),sockfd(-1),sockfdaccepted(-1),host(hh),port(pp) {
#ifdef WIN32
  WSADATA wsaData;
  int iResult=WSAStartup(MAKEWORD(2,2),&wsaData);
  if (iResult!=NO_ERROR) {
  }
#endif  
}

IpcServer::~IpcServer() {
  closeconn();
  closefd(&sockfd);
#ifdef WIN32
  WSACleanup();
#endif 
}

static void writeclient(int fd,const char *str,int len) {
  if (!str || len<0) 
    send (fd,"\n",1,0);    
  else 
    send(fd,str,len,0);
}

static int readclient(int fd) {
  int len=0;
  char line[1024]; 
  memset (line,0,1024);
  do {
    len=recv(fd,line,1023,0); 
    //parsecommand(fd,line,len);
  } while (len>0);
  return len;
}

static void *runner(void *arg) {
  IpcServer *ipcserver=(IpcServer*)arg;
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
    sprintf(outbuf,"%d connected on fd,port [%d,%d]\n",__LINE__,ipcserver->sockfdaccepted,ntohs(sin.sin_port));
    msgwrite(1,&outbuf[0]);
    if (ipcserver->sockfdaccepted>0) {
      readclient(ipcserver->sockfdaccepted);
      closefd(&ipcserver->sockfdaccepted);
    }
  }
  return 0;
}

int IpcServer::closeconn() {
  if (connthread==0)
    return -1;
  pthread_cancel(connthread);
  pthread_join(connthread,0);
  connthread=0;
  closefd(&sockfdaccepted);
  return 0;
}

int IpcServer::newconn(unsigned int hh,unsigned short pp) {
  closeconn();
  closefd(&sockfd);
  host=(hh==0 ? (host==0 ? 2130706433 : host) : hh);
  port=(pp==0 ? (port==0 ? findnextport(1025) : port) : pp);
  if (port==0)
    return -1;
  struct in_addr inaddr;
  inaddr.s_addr=ntohl(host);
  char *hostbuf=inet_ntoa(inaddr);
  //char *hostbuf=xinet_ntoa(host,0);
  int ans=socket(AF_INET,SOCK_STREAM,0);
  if (ans<0)
    goto bailout;
  sockfd=ans;
  struct sockaddr_in sin;
  sin.sin_family=AF_INET;
  sin.sin_port=htons(port);
  sin.sin_addr.s_addr=INADDR_ANY;
  ans=bind(sockfd,(struct sockaddr*)(&sin),sizeof(sin));
  if (ans<0) 
    goto bailout;
  ans=listen(sockfd,2);
  if (ans<0) 
    goto bailout;
  sprintf(outbuf,"%d accept connections on host,fd,port [%s,%d,%d]\n",__LINE__,hostbuf,sockfd,port);
  msgwrite(1,&outbuf[0]);
  ans=pthread_create(&connthread,0,&runner,(void*)this);
  if (ans==0)
    ans=pthread_join(connthread,0);
bailout:
  if (ans<0) {
    msgpusherrno(__LINE__);
    msgwrite(2,&errbuf[0]);
  }
  closefd(&sockfd);
  return ans;
} 

}
