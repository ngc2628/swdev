
#include <octave/oct.h>
#define Complex NagComplex
#define MatrixType NagMatrixType
#include <nag.h>
#include <nag_types.h>
#include <nag_stdlib.h>
#include <nagg05.h>
#include <stdio.h>

DEFUN_DLD (Og05sa, args, cntout,"nag_rand_basic\n")
{
  octave_value_list res;
  char buf[1024];
  memset(buf,0,1024);
  res(0)=octave_value(buf);
  res(1)=0;
  
  Integer nn=args(0).int_value();
  NDArray nd1=args(1).array_value();
  Integer ii=0,nstate=nd1.length();

  Integer *state=NAG_ALLOC(nstate,Integer);
  for (ii=0;ii<nstate;ii++)
    state[ii]=nd1.elem(ii);
  double *values=NAG_ALLOC(nn,double);
  memset(&values[0],0,nn*sizeof(double));
  NagError fail;
  INIT_FAIL(fail);
  g05sac(nn,state,values,&fail);
  
  if (fail.code!=NE_NOERROR) {
    sprintf (buf,"%d %s\n",__LINE__,fail.message);
    res(0)=octave_value(buf);
  }

  nd1.resize1(nn);
  for (ii=0;ii<nn;ii++)
    nd1.elem(ii)=values[ii];
  res(1)=nd1;

  NAG_FREE(values);
  NAG_FREE(state);

  return res;

} 