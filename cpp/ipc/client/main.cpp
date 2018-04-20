
#include <ipc/client/client.h>

int main(int argc, char** argv) {
    ipc::IpcClient client;
    client.newconn(0,1025);
    return 0;
}

