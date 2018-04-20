
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

namespace ipc {

#ifdef WIN32 
class __declspec(dllexport) IpcServer  {
#else 
class IpcServer  {
#endif
  public:
    pthread_t connthread;
    int sockfd,sockfdaccepted;
    unsigned int host;
    unsigned short port;
    
    IpcServer(unsigned int hh=0,unsigned short pp=0);
    ~IpcServer();
    int newconn(unsigned int hh=0,unsigned short pp=0);
    int closeconn();
    
  private: // disable
    IpcServer(const IpcServer &) { }
    IpcServer operator=(const IpcServer &) { return *this; }
    bool operator==(const IpcServer &) const { return false; }
    bool operator<(const IpcServer &) const { return false; }
};

}

#endif

