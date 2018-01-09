
#include <octave/oct.h>
#define Complex NagComplex
#define MatrixType NagMatrixType
#include <nag.h>
#include <nag_types.h>
#include <nag_stdlib.h>
#include <nagg05.h>
#include <stdio.h>

DEFUN_DLD (Og05kg, args, cntout,"nag_rand_init_nonrepeatable\n")
{
  octave_value_list res;
  char buf[1024];
  memset(buf,0,1024);
  res(0)=octave_value(buf);
  res(1)=0;
  
  Integer nn1=args(0).int_value();
  Integer nn2=args(1).int_value();
 
  Integer ii=0,lstate=-1;
  Integer *state=0;
  Nag_BaseRNG genid=Nag_Basic;
  Integer subid=0;
  NagError fail;
  INIT_FAIL(fail);
  
  /* get lstate for alloc */
  g05kgc(genid,subid,state,&lstate,&fail);

  if (fail.code!=NE_NOERROR) {
    sprintf (buf,"%d %s\n",__LINE__,fail.message);
    res(0)=octave_value(buf);
    return res;
  }

  /* calc states[lstates] */
  state=NAG_ALLOC(lstate,Integer);
  g05kgc(genid,subid,state,&lstate,&fail);
  
  if (fail.code!=NE_NOERROR) {
    sprintf (buf,"%d %s\n",__LINE__,fail.message);
    res(0)=octave_value(buf);
  }

  NDArray nd1;
  nd1.resize1(lstate);
  for (ii=0;ii<lstate;ii++)
    nd1.elem(ii)=state[ii];
  res(1)=nd1;

  NAG_FREE(state);

  return res;

} 