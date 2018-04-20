
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char in[63]={48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,\
                 81,82,83,84,85,86,87,88,89,90,95,97,98,99,100,101,102,103,104,105,106,107,108,109,110,\
                 111,112,113,114,115,116,117,118,119,120,121,122};
const char inscram[63]={113,51,98,105,72,57,121,88,109,73,56,114,100,115,55,49,81,104,50,71,107,99,122,102,\
                  67,103,119,69,75,97,111,48,53,89,77,106,90,101,110,68,80,112,108,52,70,116,83,74,84,120,86,\
                  54,87,66,79,85,76,117,65,78,118,82,95};
char word[64];
char out[64];

int punchinelloDecrypt() {

  int ii=0,jj=0,len2=strlen(word),len=len2-2,idx1=0,idx2=0;
  if(len2==0 || len<=0) 
    return 1;
  char word2[64];
  memset(word2,0,64);

  for (ii=0;ii<63;ii++) {
    jj=ii+len;
    if (jj>62) 
      jj-=63;
    out[ii]=inscram[jj];
  }
  ii=jj=0;

  if (len<5) {
    idx1=len2-2;
    idx2=len2-1;
  }
  else {
    idx1=len/2;
    idx2=len-2;
  }
  for (ii=0;ii<len2;ii++) {
    if (ii==idx1 || ii==idx2) 
      continue;
    word2[jj++]=word[ii];
  }
  word2[jj]='\0';

  if (strlen(word2)!=len) 
    return 1;

  for (ii=0;ii<len;ii++) {
    for (jj=0;jj<63;jj++) {
      if (word2[ii]==out[jj]) {
        word2[ii]=in[jj];
        break;
      }
    }
  }

  printf ("word=%s\n",word2);
  return 0;

}

int punchinelloCrypt() {

  int ii=0,jj=0,len=strlen(word),idx1=0,idx2=0;
  if(len==0) 
    return 1;

  for (ii=0;ii<63;ii++) {
    jj=ii+len;
    if (jj>62) 
      jj-=63;
    out[ii]=inscram[jj];
  }

  for (ii=0;ii<len;ii++) {
    for (jj=0;jj<63;jj++) {
      if (word[ii]==in[jj]) {
        word[ii]=out[jj];
        break;
      }
    }
  }
    
  if (len<5) {
    word[len]=40+len;
    word[len+1]=127-len;
  }
  else {
    idx1=len/2;
    idx2=len-2;
    memmove(&word[idx1+1],&word[idx1],len-idx1);
    word[idx1]=40+len;
    memmove(&word[idx2+1],&word[idx2],len+1-idx2);
    word[idx2]=127-len;
  }
    
  word[len+2]='\0';

  printf ("word=%s\n",word);
  return 0; 
 
}

int main(int argc,char **argv) {

  memset(word,0,64);
  memset(out,0,64);
  if (argc<3) {
    printf ("usage: pun\n       -c word\n       -d word\n");
    return 1;
  }
  strncpy(word,argv[2],60);
  int res=1;
  if (strcmp(argv[1],"-c")==0) 
    res=punchinelloCrypt();
  else if (strcmp(argv[1],"-d")==0) 
    res=punchinelloDecrypt(); 
  if (res!=0) 
    printf ("usage: pun\n       -c word\n       -d word\n");
  return res;

}

