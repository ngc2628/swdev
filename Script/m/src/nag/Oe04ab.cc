
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
  Nag_Spline *splx;
  Nag_Spline *sply;
  double x0x;
  double x0y;
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

static void NAG_CALL xlsqfun(double xc,double *fc,Nag_Comm *comm) {

    
  char buf[2048];
  memset(buf,0,2048);

  NagError fail;
  INIT_FAIL(fail);

  double ycx=0.0,ycy=0.0;
  e02bbc(xc,&ycx,((struct nagcommobj*)comm->p)->splx,&fail);
  e02bbc(xc,&ycy,((struct nagcommobj*)comm->p)->sply,&fail);
  if (fail.code!=NE_NOERROR) 
    addmsg(__LINE__,((struct nagcommobj*)comm->p)->errmsg,fail.message,2048); 
  
  *fc=(ycx-((struct nagcommobj*)comm->p)->x0x)*(ycx-((struct nagcommobj*)comm->p)->x0x)+
      (ycy-((struct nagcommobj*)comm->p)->x0y)*(ycy-((struct nagcommobj*)comm->p)->x0y);


}

}

DEFUN_DLD (Oe04ab, args, cntout,"nag_opt_one_var_no_deriv\n")
{
  octave_value_list res;
  char buf[4096];
  memset(buf,0,4096);
  res(0)=octave_value(buf);
  res(1)=0;
  
  double ndlb=args(0).double_value();
  double ndub=args(1).double_value();
  NDArray ndx0=args(2).array_value();
  Integer nnx=args(3).int_value();
  NDArray ndlamdax=args(4).array_value();
  NDArray ndccx=args(5).array_value();
  Integer nny=args(6).int_value();
  NDArray ndlamday=args(7).array_value();
  NDArray ndccy=args(8).array_value();

  Integer ii=0,jj=0,kk=0;

  double *lamdax=NAG_ALLOC(nnx,double);
  double *ccx=NAG_ALLOC(nnx-4,double);
  for (ii=0;ii<nnx;ii++) {
    lamdax[ii]=ndlamdax.elem(ii);
    if (ii<nnx-4)
      ccx[ii]=ndccx.elem(ii);
  }
  double *lamday=NAG_ALLOC(nny,double);
  double *ccy=NAG_ALLOC(nny-4,double);
  for (ii=0;ii<nny;ii++) {
    lamday[ii]=ndlamday.elem(ii);
    if (ii<nny-4)
      ccy[ii]=ndccy.elem(ii);
  }  

  Nag_Spline splinex;
  splinex.n=nnx;
  splinex.c=ccx;
  splinex.lamda=lamdax;

  Nag_Spline spliney;
  spliney.n=nny;
  spliney.c=ccy;
  spliney.lamda=lamday;
  
  struct nagcommobj nagcomm;
  nagcomm.splx=&splinex;
  nagcomm.sply=&spliney;
  nagcomm.x0x=ndx0.elem(0);
  nagcomm.x0y=ndx0.elem(1);
  Nag_Comm comm;
  comm.flag=1;
  comm.first=Nag_TRUE;
  comm.nf=0;
  comm.user=0;
  comm.iuser=0;
  comm.p=(Pointer)&nagcomm;
  
  double xout=0.0,fout=0.0;

  NagError fail;
  INIT_FAIL(fail);
  
  e04abc(xlsqfun,0.000001,0.000000001,&ndlb,&ndub,30,&xout,&fout,&comm,&fail);
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

  res(1)=xout;
  res(2)=fout;

  NAG_FREE(lamday);
  NAG_FREE(ccy);
  NAG_FREE(lamdax);
  NAG_FREE(ccx);

  return res;

} 