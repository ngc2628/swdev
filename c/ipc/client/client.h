

#ifndef _ipc_client_h_
#define _ipc_client_h_

#include <sys/types.h>
#include <stdio.h>
#ifdef WIN32
#include <wtypes.h>
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#endif
#include <sys/types.h>
#include <mkbase/exportdefs.h>
#include <mkbase/defs.h>

struct oswinexp IpcClient {
  pthread_t connthread;
  int sockfd;
  unsigned int host;
  unsigned short port;
};

xtern int oswinexp setupconn(struct IpcClient *);
xtern int oswinexp newconn(struct IpcClient *,unsigned int,unsigned short);
xtern int oswinexp closeconn(struct IpcClient *);
xtern int oswinexp cleanupconn(struct IpcClient *);

#endif

