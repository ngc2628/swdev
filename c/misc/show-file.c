/*
  entfernt nicht-druckbare Zeichen (Steuerzeichen) aus Dateien die im Binaerformat 
  geschrieben wurden.
  In der Voreinstellung wird versucht das Format Sam zu decodieren
  Sam : Datensaetze mit variabler Laenge welche in den ersten beiden Byte (big) der 
  ersten vier Bytes des Datensatzes als unsigned short codiert sind   
  23.02.2021 martin.kloss@drv-berlin-brandenburg.de
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static const int de_arr[]={9,128,196,199,214,220,223,228,246,252};

unsigned char reverse(unsigned char ch) {

  return (unsigned char)(((ch*0x0802U&0x22110U)|(ch*0x8020U&0x88440U))*0x10101U>>16);

}

int resizeBuf(unsigned char **buf,int oldsz,int newsz) {

  if (newsz<=oldsz) {
    memset(&((*buf)[0]),0,oldsz);
    return oldsz;
  }

  if (*buf)
    free(*buf);

  int sz=1;
  while (sz<newsz)
    sz*=2;

  *buf=(unsigned char*)malloc(sz);
  memset(&((*buf)[0]),0,sz);

/* printf("%d bufresize %d\n",__LINE__,sz); */

  return sz;

}

int shiftArgs(int args,char **argv) {

  if (args<2)
    return args;
  int ii=0;
  for (ii=2;ii<args;ii++) {
    argv[ii-1]=argv[ii];
  }
  return args-1;

}

int clearCtrl(unsigned char **buf,int len,int mode) {

  int ii=0,jj=0,cntctrl=0;
  for (ii=0;ii<len;ii++) {
    if (mode=='d' && (*buf)[ii]==10)
      continue;
    for (jj=0;jj<sizeof(de_arr)/sizeof(int);jj++) {
      if ((*buf)[ii]==de_arr[jj]) 
        goto breakfor;
    }
    if (((*buf)[ii]<32 || (*buf)[ii]>128)) {
      (*buf)[ii]=32;
      cntctrl++;
    }
breakfor: 
  continue;
  }
  return cntctrl;

}

int main (int args, char **argv) {

  if (args<3) {
    fprintf(stderr,"usage \"show-file -if datei [-m sam|direct] [-of datei]\"\n");
    return 1;
  }

  int buflen=256,mode='s',ii=0;
  unsigned char *ibuf=(unsigned char *)malloc(buflen);
  memset(&ibuf[0],0,buflen);
  char obuf[256];
  memset(&obuf[0],0,256);

  while (args>1) {
    if (strncmp(argv[1],"-if",3)==0) {
      args=shiftArgs(args,argv);
      if (args>1) 
        strncpy(ibuf,argv[1],buflen-1);
      args=shiftArgs(args,argv);
    }
    else if (strncmp(argv[1],"-m",2)==0) {
      args=shiftArgs(args,argv);
      if (args>1) {
        if (strncmp(argv[1],"direct",3)==0)
          mode='d';
      }
      args=shiftArgs(args,argv);
    }
    else if (strncmp(argv[1],"-of",3)==0) {
      args=shiftArgs(args,argv);
      if (args>1) 
        strncpy(obuf,argv[2],buflen-1);
      args=shiftArgs(args,argv);
    }
    else
      args=shiftArgs(args,argv);
  }

  int ifd=open(&ibuf[0],O_RDONLY);
  if (ifd<0) {
    fprintf(stderr,"kann \"%s\" nicht lesen\n",ibuf);
    return 1;
  }
  FILE *ifp=0;
  if (mode=='d') {
    ifp=fdopen(ifd,"r");
    if (!ifp) {
      fprintf(stderr,"kann \"%s\" nicht lesen\n",ibuf);
      return 1;
    }
  }
  int ofd=-1;
  if (strlen(obuf)==0)
    ofd=STDOUT_FILENO;
  else {
    ofd=open(&obuf[0],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
    if (ofd<0) {
      fprintf(stderr,"kann \"%s\" nicht schreiben\n",obuf);
      return 1;
    }
  }
  
  setlocale(LC_ALL,"C");
  memset(&ibuf[0],0,buflen);
  unsigned short setlen=0;
  unsigned char lenbuf[]={0,0,0,0};
  unsigned char ch=0;
  int setno=0,written=0,cntb=0;
  if (mode=='d') {
    fseek(ifp,0,SEEK_END);
    cntb=ftell(ifp);
    fseek(ifp,0,SEEK_SET);
    buflen=resizeBuf(&ibuf,buflen,cntb);
    cntb=fread(&ibuf[0],1,cntb,ifp);
    clearCtrl(&ibuf,cntb,mode);
    written=write(ofd,&ibuf[0],cntb);  
  }
  else {
    cntb=(int)read(ifd,&lenbuf[0],4);
    while (cntb>0) {
      setno++;
      setlen=*((unsigned short*)(&lenbuf[0])); 
      setlen=((setlen&255)<<8)|(setlen>>8); /* big -> little */
/*      fprintf(stderr,"Datensatz #%d mit Laenge %d\n",setno,setlen);*/
      buflen=resizeBuf(&ibuf,buflen,setlen);
      cntb=read(ifd,&ibuf[0],setlen-4);
      clearCtrl(&ibuf,cntb,mode);
      ibuf[cntb]=10;
      written=write(ofd,&ibuf[0],cntb+1);
      cntb=(int)read(ifd,&lenbuf[0],4);
    }
  }  

  close(ifd);
  if (ofd>2)
    close(ofd);

  free(ibuf);

  return 0;

}
