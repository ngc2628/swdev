
#include <ipc/ipcutl.h>
#include <ipc/server/server.h>

int main(int argc,char** argv) {
  allocbuf();
  struct IpcServer server={0,-1,-1,0,0};
  setupconn(&server);
  newconn(&server,0,0);
  closeconn(&server);
  cleanupconn(&server);
  return 0;
}

