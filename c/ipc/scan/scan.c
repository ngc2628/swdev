
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#ifdef WIN32
#include <wtypes.h>
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#endif

int usage (int ll,const char *msg1,const char *msg2,const char *msg3,const char *msg4) {

  char buf[1025];
  memset(&buf[0],0,1025);
  sprintf(buf,"\n%d: ",ll);
  msg1 ? strcat(buf,msg1) : strcat(buf,"error ");
  if (msg2)
    strcat(buf,msg2);
  if (msg3)
    strcat(buf,msg3);
  if (msg4)
    strcat(buf,msg4);
  strcat(buf,"\n\nusage : \"ipcscan\" \"host\" \"port\"");
  fprintf(stderr,"%s\n\n",buf);
  return 1;

} 

int simplea2i(const char *str) {

  int ll=(int)(!str ? 0 : strlen(str));
  if(ll==0)
    return -1;
  char basechar[10]={'0','1','2','3','4','5','6','7','8','9'};
  int ans=0;
  int ii=0,jj=-1,mult=1;
  while (--ll>=0) {
    for (ii=0;ii<10;ii++) {
      if (basechar[ii]==str[ll]) {
        jj=ii;
        break;
      }
    }
    if (jj<0)
      return -1;
    ans+=(jj*mult);
    mult*=10;
    jj=-1;
  }

  return ans;
    
}

int main (int argc, char **argv) {

  if(argc<3)
    return usage(__LINE__,"arguments missing",0,0,0);

  /* get ipv4 host address - just the first one */
  char hostname[255];
  memset(&hostname[0],0,255);
  strncpy(hostname,argv[1],254);
  int ans=simplea2i(argv[2]);
  if (ans<0)
    return usage(__LINE__,hostname," bad port : ",argv[2],0);
  
  unsigned short port=(unsigned short)ans;

  char hostaddr[255];
  memset(&hostaddr[0],0,255);
  struct addrinfo info;
  struct addrinfo *info_out=0;
  struct addrinfo *cursor=0;
  struct sockaddr_in *info_sin=0;
  memset(&info,0,sizeof(struct addrinfo));
  info.ai_family=AF_INET;
  ans=getaddrinfo(hostname,0,&info,&info_out);
  if (ans) 
    return usage(__LINE__,"host not found",0,0,0);
  info_sin=(struct sockaddr_in *)info_out->ai_addr;
  inet_ntop(AF_INET,&info_sin->sin_addr,hostaddr,254);
  freeaddrinfo(info_out);
  
  unsigned int host=0;
  char *tok=strtok(hostaddr,".");
  ans=simplea2i(tok);
  if (ans<0)
    return usage(__LINE__,hostname," bad ",hostaddr,0);
  host=(ans<<24);
  tok=strtok(0,".");
  ans=simplea2i(tok);
  if (ans<0)
    return usage(__LINE__,hostname," bad ",hostaddr,0);
  host+=(ans<<16);
  tok=strtok(0,".");
  ans=simplea2i(tok);
  if (ans<0)
    return usage(__LINE__,hostname," bad ",hostaddr,0);
  host+=(ans<<8);
  tok=strtok(0,".");
  ans=simplea2i(tok);
  if (ans<0)
    return usage(__LINE__,hostname," bad ",hostaddr,0);
  host+=ans;

  int sockfd=-1;
  struct sockaddr_in sin;
  sin.sin_family=AF_INET;
  sin.sin_port=htons(port);
  sin.sin_addr.s_addr=htonl(host);
  unsigned int len=sizeof(sin);
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  if (sockfd<0) {
    fprintf(stderr,"%d cannot create socket",__LINE__);
    return 1;
  }
  struct timeval timeout;      
  timeout.tv_sec=1;
  timeout.tv_usec=0;
  ans=setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
  if (ans<0) { 
    fprintf(stderr,"%d cannot set read timeout on fd \"%d\"\n",__LINE__,sockfd);
    return 1;
  }
  ans=setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));
  if (ans<0) { 
    fprintf(stderr,"%d cannot set send timeout on fd \"%d\"\n",__LINE__,sockfd);
    return 1;
  }
  ans=connect(sockfd,(struct sockaddr*)(&sin),sizeof(sin));
  if (ans<0) { 
    fprintf(stderr,"%d cannot connect to %s (%u) on port %d\n",__LINE__,hostname,host,port);
    return 1;
  }
  fprintf(stdout,"connected on host,fd,port [%s,%d,%u]\n",hostname,sockfd,port);
  ans=shutdown(sockfd,SHUT_RDWR);

  return ans;

}



