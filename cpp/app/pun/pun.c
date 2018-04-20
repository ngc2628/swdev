
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char in[94]={
33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,112,
113,114,115,116,117,118,119,120,121,122,123,124,125,126
};
const char inscram[94]={
94,101,115, 93,107,116, 77,126, 62,125, 57, 44, 72, 38,118, 68,112,100, 91, 86,
55,108, 59,121, 82, 39, 87, 96, 88, 67, 52,117,109,113, 83, 90, 33, 47, 50, 80,
48, 99,105, 65,110, 56, 70, 69, 92, 41, 97,124, 43, 95, 42, 61,119, 35,104, 51,
123, 45, 34, 98, 54,102,111, 37, 81,122, 75, 79, 58, 66,114, 40,120, 84,103, 89,
78, 63, 53, 73, 60, 71, 49, 64, 46,106, 76, 36, 85, 74
};
static const int scramlen=94;
static const int wordlen=128;

int punchinelloDecrypt(const char *word) {

  int ii=0,jj=0,len2=(word ? strlen(word) : 0),len=len2-2,idx1=0,idx2=0;
  if(len2==0 || len<=0 || len2>=wordlen)
    return 1;

  char word2[wordlen];
  char out[wordlen];
  memset(word2,0,wordlen);
  memset(out,0,wordlen);

  for (ii=0;ii<scramlen;ii++) {
    jj=ii+len;
    if (jj>=scramlen)
      jj-=scramlen;
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

  if ((int)strlen(word2)!=len)
    return 1;

  for (ii=0;ii<len;ii++) {
    for (jj=0;jj<scramlen;jj++) {
      if (word2[ii]==out[jj]) {
        word2[ii]=in[jj];
        break;
      }
    }
  }

  printf ("word=%s\n",word2);
  return 0;

}

int punchinelloCrypt(const char *word) {

  int ii=0,jj=0,len=(word ? strlen(word) : 0),idx1=0,idx2=0;
  if(len==0 || len>wordlen-3)
    return 1;

  char word2[wordlen];
  char out[wordlen];
  memset(word2,0,wordlen);
  memset(out,0,wordlen);

  strncpy(word2,word,len);

  for (ii=0;ii<scramlen;ii++) {
    jj=ii+len;
    if (jj>=scramlen)
      jj-=scramlen;
    out[ii]=inscram[jj];
  }

  for (ii=0;ii<len;ii++) {
    for (jj=0;jj<scramlen;jj++) {
      if (word2[ii]==in[jj]) {
        word2[ii]=out[jj];
        break;
      }
    }
  }

  if (len<5) {
    word2[len]=40+len/2;
    word2[len+1]=127-len/2;
  }
  else {
    idx1=len/2;
    idx2=len-2;
    memmove(&word2[idx1+1],&word2[idx1],len-idx1);
    word2[idx1]=40+len/2;
    memmove(&word2[idx2+1],&word2[idx2],len+1-idx2);
    word2[idx2]=127-len/2;
  }

  word2[len+2]='\0';

  printf ("word=%s\n",word2);
  return 0;

}

int main(int argc,char **argv) {

  if (argc<2) {
    printf ("usage: pun\n       -c word\n       -d word\n");
    return 1;
  }
  char inp[129];
  memset(&inp[0],0,129);
  if (argc<3) {
    char iinp[256];
    memset(&iinp[0],0,256);
    fgets(iinp,255,stdin);
    int ii=0,jj=0;
    for (ii=0;ii<(int)strlen(iinp);ii++) {
      if ((int)iinp[ii]>32 && (int)iinp[ii]<127)
        inp[jj++]=iinp[ii];
    }
  }
  else
    strncpy(inp,argv[2],128);

  int res=1;
  if (strcmp(argv[1],"-c")==0)
    res=punchinelloCrypt(inp);
  else if (strcmp(argv[1],"-d")==0)
    res=punchinelloDecrypt(inp);
  if (res!=0)
    printf ("usage: pun\n       -c word\n       -d word\n");
  return res;

}

