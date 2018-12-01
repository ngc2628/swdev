
#include <ipc/ipcutl.h>
#include <ipc/client/client.h>

int main(int argc,char** argv) {

allocbuf();
  struct IpcClient client={0,-1,-1,0};
  int ans=setupconn(&client);
  ans=newconn(&client,0,1025);
  closeconn(&client);
  cleanupconn(&client);
  return 0;

}


