#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

/* modifizieren der Zugriffsrechte von Shared Memory Segmenten */

int usage(const char *msg) {

  if (msg) 
    fprintf(stderr,"%s\n",msg);
  fprintf(stderr,"%s\n","usage \"modshm\" \"shmid\" \"mode (oktal)\"");
  return 1; 

}

/*Start of main C language program*/
int main(int args, char **argv) {
 
  if (args<3) 
    return usage(0);

  int shmid=atoi(argv[1]),mode=0;
  sscanf(argv[2],"%o",&mode);
  struct ipc_perm ipcperm={0,0,0,0,0,0,0};
  struct shmid_ds shm={ipcperm,0,0,0,0,0,0,0};

  int ans=shmctl(shmid,IPC_STAT,&shm);
  if (ans<0)
    return usage("shmid ist ungueltig");

  printf("shm-key [0%x] , user [%d] , group [%d] permissions [%o]\n",shm.shm_perm.__key,shm.shm_perm.uid,shm.shm_perm.gid,shm.shm_perm.mode);
  shm.shm_perm.mode=mode;
  ans=shmctl(shmid,IPC_SET,&shm);
  if (ans<0)
    return usage("konnte Dateirechte nicht modifizieren");
  ans=shmctl(shmid,IPC_STAT,&shm);
  if (ans>=0)
    printf("shm-key [0%x] , user [%d] , group [%d] permissions [%o]\n",shm.shm_perm.__key,shm.shm_perm.uid,shm.shm_perm.gid,shm.shm_perm.mode);
  
  return (0);

}


