
#ifndef _ipc_server_h_
#define _ipc_server_h_

#include <sys/types.h>
#ifdef WIN32
#include <wtypes.h>
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#endif
#include <mkbase/exportdefs.h>
#include <mkbase/defs.h>

struct oswinexp IpcServer {
  pthread_t connthread;
  int sockfd,sockfdaccepted;
  unsigned int host;
  unsigned short port;
};

xtern int oswinexp setupconn(struct IpcServer *);
xtern int oswinexp newconn(struct IpcServer *,unsigned int,unsigned short);
xtern int oswinexp closeconn(struct IpcServer *);
xtern int oswinexp cleanupconn(struct IpcServer *);

#endif

