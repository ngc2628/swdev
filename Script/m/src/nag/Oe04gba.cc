
#include <octave/oct.h>
#define Complex NagComplex
#define MatrixType NagMatrixType
#include <nag.h>
#include <nag_types.h>
#include <nag_stdlib.h>
#include <nage04.h>
#include <nage02.h>
#include <stdio.h>

extern "C" {

struct nagcommobj {
  Nag_2dSpline *splx;
  Nag_2dSpline *sply;
  Nag_2dSpline *splz;
  double x0[3];
  char errmsg[2048];
};

static void addmsg(int line,char *msg,const char *add,int maxsz) {
  
  if (strlen(add)==0)
    return;
  char lnbuf[8];
  sprintf(lnbuf,"%d ",line);
  int msglen=strlen(msg),lnlen=strlen(lnbuf);
  if (maxsz<msglen+lnlen+2)
    return;
  strcat(msg,lnbuf);
  if (maxsz-msglen-2>0)
    strncat(msg,add,maxsz-msglen-2);
  strcat(msg,"\n");

}

static void NAG_CALL xlsqfun(Integer mm,Integer nn, const double x0[],double fvec[],double fjac[],
  Integer tdfjac,Nag_Comm *comm) {

  double xx[]={x0[0]};
  double yy[]={x0[1]};
  double der[]={0.0};
    
  char buf[2048];
  memset(buf,0,2048);

  NagError fail;
  INIT_FAIL(fail);

  e02dhc(1,1,xx,yy,1,0,der,((struct nagcommobj*)comm->p)->splx,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fjac[0]=der[0];
  der[0]=.0;
  e02dhc(1,1,xx,yy,1,0,der,((struct nagcommobj*)comm->p)->sply,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fjac[2]=der[0];
  der[0]=.0;
  e02dhc(1,1,xx,yy,1,0,der,((struct nagcommobj*)comm->p)->splz,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fjac[4]=der[0];
  der[0]=.0;
  e02dhc(1,1,xx,yy,0,1,der,((struct nagcommobj*)comm->p)->splx,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fjac[1]=der[0];
  der[0]=.0;
  e02dhc(1,1,xx,yy,0,1,der,((struct nagcommobj*)comm->p)->sply,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fjac[3]=der[0];
  der[0]=.0;
  e02dhc(1,1,xx,yy,0,1,der,((struct nagcommobj*)comm->p)->splz,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fjac[5]=der[0];
  der[0]=.0;
  
  e02dec(1,xx,yy,der,((struct nagcommobj*)comm->p)->splx,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fvec[0]=der[0]-((struct nagcommobj*)comm->p)->x0[0];
  der[0]=.0;
  e02dec(1,xx,yy,der,((struct nagcommobj*)comm->p)->sply,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fvec[1]=der[0]-((struct nagcommobj*)comm->p)->x0[1];
  der[0]=.0;
  e02dec(1,xx,yy,der,((struct nagcommobj*)comm->p)->splz,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  fvec[2]=der[0]-((struct nagcommobj*)comm->p)->x0[2];
  der[0]=.0;

}

}

DEFUN_DLD (Oe04gba, args, cntout,"nag_opt_lsq_deriv\n")
{
  octave_value_list res;
  char buf[4096];
  memset(buf,0,4096);
  res(0)=octave_value(buf);
  res(1)=0;
  
  NDArray ndx0=args(0).array_value();
  NDArray ndst=args(1).array_value();
  Integer nxspx=args(2).int_value();
  Integer nyspx=args(3).int_value();
  NDArray ndlamdax=args(4).array_value();
  NDArray ndmux=args(5).array_value();
  NDArray ndcx=args(6).array_value();
  Integer nxspy=args(7).int_value();
  Integer nyspy=args(8).int_value();
  NDArray ndlamday=args(9).array_value();
  NDArray ndmuy=args(10).array_value();
  NDArray ndcy=args(11).array_value();
  Integer nxspz=args(12).int_value();
  Integer nyspz=args(13).int_value();
  NDArray ndlamdaz=args(14).array_value();
  NDArray ndmuz=args(15).array_value();
  NDArray ndcz=args(16).array_value();

  Integer ii=0,jj=0,kk=0;

  Nag_2dSpline xspl;
  xspl.nx=nxspx;
  xspl.lamda=NAG_ALLOC(nxspx,double);
  for (ii=0;ii<nxspx;ii++)
    xspl.lamda[ii]=ndlamdax.elem(ii);
  xspl.ny=nyspx;
  xspl.mu=NAG_ALLOC(nyspx,double);
  for (ii=0;ii<nyspx;ii++)
    xspl.mu[ii]=ndmux.elem(ii);
  Integer splineclen=(nxspx-4)*(nyspx-4);
  xspl.c=NAG_ALLOC(splineclen,double);
  for (ii=0;ii<nxspx-4;ii++) {
    for (jj=0;jj<nyspx-4;jj++) 
      xspl.c[(nyspx-4)*ii+jj]=ndcx.elem(kk++);
  } 
  kk=0; 

  Nag_2dSpline yspl;
  yspl.nx=nxspy;
  yspl.lamda=NAG_ALLOC(nxspy,double);
  for (ii=0;ii<nxspy;ii++)
    yspl.lamda[ii]=ndlamday.elem(ii);
  yspl.ny=nyspy;
  yspl.mu=NAG_ALLOC(nyspy,double);
  for (ii=0;ii<nyspy;ii++)
    yspl.mu[ii]=ndmuy.elem(ii);
  splineclen=(nxspy-4)*(nyspy-4);
  yspl.c=NAG_ALLOC(splineclen,double);
  for (ii=0;ii<nxspy-4;ii++) {
    for (jj=0;jj<nyspy-4;jj++) 
      yspl.c[(nyspy-4)*ii+jj]=ndcy.elem(kk++);
  }
  kk=0;

  Nag_2dSpline zspl;
  zspl.nx=nxspz;
  zspl.lamda=NAG_ALLOC(nxspz,double);
  for (ii=0;ii<nxspz;ii++)
    zspl.lamda[ii]=ndlamdaz.elem(ii);
  zspl.ny=nyspz;
  zspl.mu=NAG_ALLOC(nyspz,double);
  for (ii=0;ii<nyspz;ii++)
    zspl.mu[ii]=ndmuz.elem(ii);
  splineclen=(nxspz-4)*(nyspz-4);
  zspl.c=NAG_ALLOC(splineclen,double);
  for (ii=0;ii<nxspz-4;ii++) {
    for (jj=0;jj<nyspz-4;jj++) 
      zspl.c[(nyspz-4)*ii+jj]=ndcz.elem(kk++);
  }

  double fsumsq=0;
  double fvec[3]={0.0,0.0,0.0};
  Integer tdfjac=2;
  double fjac[6]={0.0,0.0,0.0,0.0,0.0,0.0};
  double st[2]={ndst.elem(0),ndst.elem(1)};

  struct nagcommobj nagcomm;
  nagcomm.splx=&xspl;
  nagcomm.sply=&yspl;
  nagcomm.splz=&zspl;
  nagcomm.x0[0]=ndx0.elem(0);
  nagcomm.x0[1]=ndx0.elem(1);
  nagcomm.x0[2]=ndx0.elem(2);
  memset(&nagcomm.errmsg[0],0,512);
  Nag_Comm comm;
  comm.flag=1;
  comm.first=Nag_TRUE;
  comm.nf=0;
  comm.user=0;
  comm.iuser=0;
  comm.p=(Pointer)&nagcomm;
  
  Nag_E04_Opt opt;
  nag_opt_init (&opt);
  opt.optim_tol=0.01;
  opt.list=Nag_FALSE;
  opt.print_level=Nag_Soln_Iter;
  /*Nag_NoPrint 	       No output.
  Nag_Soln 	           The final solution.
  Nag_Iter 	           One line of output for each iteration.
  Nag_Iter_Long 	     A longer line of output for each iteration with more information (line exceeds 80 characters).
  Nag_Soln_Iter 	     The final solution and one line of output for each iteration.
  Nag_Soln_Iter_Long 	 The final solution and one long line of output for each iteration (line exceeds 80 characters).
  Nag_Soln_Iter_Const  As Nag_Soln_Iter_Long with the Lagrange multipliers, the variables x , 
                       the constraint values Ax  and the constraint status also printed at each iteration.
  Nag_Soln_Iter_Full 	 As Nag_Soln_Iter_Const with the diagonal elements of the upper triangular matrix T  
                       associated with the TQ  factorization (3) of the working set, and the diagonal elements 
                       of the upper triangular matrix R  printed at each iteration.*/

  NagError fail;
  INIT_FAIL(fail);
  
  e04gbc(3,2,xlsqfun,st,&fsumsq,fvec,fjac,tdfjac,&opt,&comm,&fail);
  //xlsqfun(3,2,st,fvec,fjac,tdfjac,&comm);
  //printf ("%d fvec=%f,%f,%f\n",__LINE__,fvec[0],fvec[1],fvec[2]);
  //printf ("%d fjac=%f,%f,%f,%f,%f,%f\n",__LINE__,fjac[0],fjac[1],fjac[2],fjac[3],fjac[4],fjac[5]);

  if (strlen(nagcomm.errmsg)>0) {
    addmsg(__LINE__,buf,nagcomm.errmsg,4096); 
    res(0)=octave_value(buf);
  }
  
  if (fail.code!=NE_NOERROR) {
    addmsg(__LINE__,buf,fail.message,4096); 
    res(0)=octave_value(buf);
  }

  ndst.elem(0)=st[0];
  ndst.elem(1)=st[1];
  res(1)=ndst;

  NAG_FREE(zspl.c); 
  NAG_FREE(zspl.mu);
  NAG_FREE(zspl.lamda);
  NAG_FREE(yspl.c); 
  NAG_FREE(yspl.mu);
  NAG_FREE(yspl.lamda);
  NAG_FREE(xspl.c); 
  NAG_FREE(xspl.mu);
  NAG_FREE(xspl.lamda);

  return res;

} 