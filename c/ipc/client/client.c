#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <ipc/client/client.h>
#include <ipc/ipcutl.h>

int setupconn(struct IpcClient *client) {

  if (!client)
    return 1;
  client->connthread=0;
  client->sockfd=-1;
  client->host=0;
  client->port=0;
#ifdef WIN32
  WSADATA wsaData;
  int iResult=WSAStartup(MAKEWORD(2,2),&wsaData);
  if (iResult!=NO_ERROR) {
  }
#endif
  return 0;

}

int cleanupconn(struct IpcClient *client) {

  if (!client)
    return 1;
  closeconn(client);
  closefd(&client->sockfd);
#ifdef WIN32
  WSACleanup();
#endif
  return 0;

}

int closeconn(struct IpcClient *client) {

  if (!client || client->connthread==0)
    return -1;
  pthread_cancel(client->connthread);
  pthread_join(client->connthread,0);
  client->connthread=0;
  closefd(&client->sockfd);
  return 0;

}

static void *runner(void *arg) {
  struct IpcClient *ipcclient=(struct IpcClient*)arg;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,0);
  struct sockaddr_in sin;
  sin.sin_family=AF_INET;
  sin.sin_port=htons(ipcclient->port);
  sin.sin_addr.s_addr=htonl(ipcclient->host);
  unsigned int len=sizeof(sin);
  int ll=0,ans=socket(AF_INET,SOCK_STREAM,0);
  if (ans<0) {
    ll=__LINE__;
    goto bailout;
  }
  ipcclient->sockfd=ans;
  ans=connect(ipcclient->sockfd,(struct sockaddr*)(&sin),sizeof(sin));
  if (ans<0) {
    ll=__LINE__;
    goto bailout;
  }
  sprintf(outbuf,"%d connected on host,fd,port [%s,%d,%d]\n",__LINE__,
    xinet_ntoa(ipcclient->host,0),ipcclient->sockfd,ipcclient->port);
  msgwrite(1,&outbuf[0]);
  // send/recv loop
  strcpy(&outbuf[0],"enter command (\"bye\" or ctrl-c to quit)\n> ");
  msgwrite(1,&outbuf[0]);
  memset(&inbuf[0],0,bufsz);
  ans=read(0,&inbuf[0],bufsz);
  while (ans>=0 && strncmp(&inbuf[0],"bye",3)) {
    if (strlen(inbuf)==0)
      strcpy(&inbuf[0]," ");
    send(ipcclient->sockfd,&inbuf[0],strlen(inbuf),0);
    ll=recv(ipcclient->sockfd,&outbuf[0],bufsz-1,0);
    if (ll>0)
      msgwrite(1,&outbuf[0]);
    strcpy(&outbuf[0],"> ");
    msgwrite(1,&outbuf[0]);
    memset(&inbuf[0],0,bufsz);
    ans=read(0,&inbuf[0],bufsz-1);
  }

bailout:
  if (ans<0) {
    msgpusherrno(ll);
    msgwrite(2,&errbuf[0]);
  }
  closefd(&ipcclient->sockfd);
  return 0;
}



int newconn(struct IpcClient *client,unsigned int hh,unsigned short pp) {

  closeconn(client);
  client->host=(hh==0 ? (client->host==0 ? 2130706433 : client->host) : hh);
  client->port=(pp==0 ? (client->port==0 ? 0 : client->port) : pp);
  if (client->port==0)
    return -1;
  int ans=pthread_create(&client->connthread,0,&runner,(void*)client);
  if (ans==0)
    ans=pthread_join(client->connthread,0);
  closefd(&client->sockfd);
  return ans;

}

