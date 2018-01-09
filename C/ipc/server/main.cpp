
#include <ipc/server/server.h>

int main(int argc,char** argv ) {
  ipc::IpcServer server;
  server.newconn(0,0);
  return 0;
}

