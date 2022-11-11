
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int obsolete_call(const char *name) {

  struct hostent *hp=gethostbyname(name);

  if (!hp) {
    printf("gethostbyname() failed\n");
  } 
  else {
    printf("%s = ", hp->h_name);
    unsigned int ii=0;
    while (hp->h_addr_list[ii]) {
      printf("%s ",inet_ntoa(*(struct in_addr*)(hp->h_addr_list[ii])));
      ii++;
    }
    printf ("\n");
  }

  return 0;

}

/* gethostbyname -> getaddrinfo */
int curr_call(const char *name) {

  struct addrinfo info;
  struct addrinfo *info_out=0;
  struct addrinfo *cursor=0;
  struct sockaddr_in *info_sin=0;
  char hostaddr[1025];
  
  memset(&hostaddr[0],0,1025);
  memset(&info,0,sizeof(struct addrinfo));
  info.ai_family=AF_INET;

  int res=getaddrinfo(name,0,&info,&info_out);
  if (res) {
    printf("getaddrinfo() failed\n");
  }
  else {
    cursor=info_out;
    while (cursor) {
      info_sin=(struct sockaddr_in *)cursor->ai_addr;
      inet_ntop(AF_INET,&info_sin->sin_addr,hostaddr,1024);
      printf("%d - %s\n",__LINE__,hostaddr);
      cursor=cursor->ai_next;
    }
    freeaddrinfo(info_out);
  }

  return 0;

}

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("Usage: %s hostname\n", argv[0]);
    return 1;
  }

  int ll=(int)strlen(argv[1]);
  char *name=(char*)malloc(ll+1);
  memset(&name[0],0,ll+1);
  strcpy(name,argv[1]);

  obsolete_call(name);

  curr_call(name); 


  free(name);
  
  return 0;

}





