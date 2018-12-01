
#ifndef _ipc_utl_h_
#define _ipc_utl_h_

#include <mkbase/exportdefs.h>
#include <mkbase/defs.h>

xtern int oswinexp bufsz;
xtern char * oswinexp inbuf;
xtern char * oswinexp outbuf;
xtern char * oswinexp errbuf;
xtern char * oswinexp hostbuf;
xtern char * oswinexp linebuf;
xtern int oswinexp allocbuf();
xtern int oswinexp msgwrite(int,char *);
xtern int oswinexp msgpusherrno(int);
xtern int oswinexp msgpushint(int,char *);
xtern int oswinexp closefd(int*);
xtern unsigned short oswinexp findnextport(unsigned short);
xtern char * oswinexp xinet_ntoa(unsigned int,const char *); 

#endif
