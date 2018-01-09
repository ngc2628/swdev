
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
 
namespace ipc {

#ifdef WIN32 
class __declspec(dllexport) IpcClient {
#else 
class IpcClient {
#endif
  public:
    pthread_t connthread;
    int sockfd;
    unsigned int host;
    unsigned short port;
    
    IpcClient(unsigned int hh=0,unsigned short pp=0);
    ~IpcClient();
    int newconn(unsigned int hh=0,unsigned short pp=0);
    int closeconn();
    
  private: // disable
    IpcClient(const IpcClient &) { }
    IpcClient operator=(const IpcClient &) { return *this; }
    bool operator==(const IpcClient &) const { return false; }
    bool operator<(const IpcClient &) const { return false; }

};

}

#endif

