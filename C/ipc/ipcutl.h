
#ifndef _ipc_utl_h_
#define _ipc_utl_h_

namespace ipc {

extern char inbuf[4096];
extern char outbuf[4096];
extern char errbuf[4096];
extern int msgwrite(int,char *);
extern int msgpusherrno(int);
extern int msgpushint(int);
extern int closefd(int*);
extern unsigned short findnextport(unsigned short);
extern char *xinet_ntoa(unsigned int,const char *); 

}

#endif
