
#include <octave/oct.h>
#include <sys/time.h>
#include <stdio.h>

DEFUN_DLD (Xchk11, args, cntout,"chk11\n")
{
  octave_value_list res;
  
  int ii=0;
  NDArray nd1,nd2;
  nd1.resize1(11);
  for (ii=0;ii<11;ii++)
    nd1.elem(ii)=2.*ii+1.;
  res(0)=nd1;
  nd2.resize1(13);
  for (ii=0;ii<13;ii++)
    nd2.elem(ii)=3*ii-1.;
  res(1)=nd2;

  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv,&tz);

  printf ("%d t=%s\n",__LINE__,ctime(&tv.tv_sec));

  res(2)=octave_value("hello");

  return res;
} 
