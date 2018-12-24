
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>

int usage() {

  printf ("\n");
  return 0;

}

int linesintersection(
  double line1x1,double line1y1,double line1x2,double line1y2,double line2x1,double line2y1,
  double line2x2,double line2y2,double *interx,double *intery,int prec,int xbox,int ybox) {

  /* line equation :
   y=m*x+b
   m=(y2-y1)/(x2-x1)
   b=y1-m*x1
   condition : m1x+b1==m2x+b2 ->
   xintersect=(b2-b1)/(m1-m2) ; yintersect=(b2*m1-b1*m2)/(m1-m2) */
  double x11=line1x1,x12=line1x2,y11=line1y1,y12=line1y2,
         x21=line2x1,x22=line2x2,y21=line2y1,y22=line2y2;
  double eps=mk_ipow10(-prec);
  if (mk_diff(x11,x12,eps)>.0) {
    mk_swapf(&x11,&x12);
    mk_swapf(&y11,&y12);
  }
  if (mk_diff(x21,x22,eps)>.0) {
    mk_swapf(&x21,&x22);
    mk_swapf(&y21,&y22);
  }
  double m1=.0,m2=.0;
  int vert1=0,vert2=0;
  if (mk_diff(x11,x12,eps)==.0)
    vert1=1;
  else
    m1=(y12-y11)/(x12-x11);
  if (mk_isfinite(m1)==0) {
    vert1=1;
    m1=.0;
  }
  if (vert1==1 && mk_diff(y11,y12,eps)>.0)
    mk_swapf(&y11,&y12);
  if (mk_diff(x21,x22,eps)==.0)
    vert2=1;
  else
    m2=(y22-y21)/(x22-x21);
  if (mk_isfinite(m2)==0) {
    vert2=1;
    m2=.0;
  }
  if (vert2==1 && mk_diff(y21,y22,eps)>0.0)
    mk_swapf(&y21,&y22);
  double b1=y11-m1*x11,b2=y21-m2*x21,mdiff=mk_diff(m1,m2);
  if (mdiff==.0) {
    if (vert1==1 && vert2==0) {
      *interx=x11;
      *intery=y21;
    }
    else if (vert1==0 && vert2==1) {
      *interx=x21;
      *intery=y11;
    }
    else if ((vert1==1 && vert2==1 && mk_diff(x11,x21,eps)==.0) ||
             (vert1==0 && vert2==0 && mk_diff(y11,y21,eps)==.0) ||
             (y21==(b1+m1*x21))) {  //degenerated case (do not know .... to be precised)
      *interx=x11;
      *intery=y11;
      if (xbox || ybox)
        return 0;
      return 1;
    }
    else {
      *interx=0.0;
      *intery=0.0;
      return 0;
    }
  }
  else {
    if (vert1) {
      *interx=x11;
      *intery=b2+m2*x11;
      if (mk_diff(y11,y12,eps)==.0) {
        if (mk_diff(y11,m2*x11+b2,eps)==.0)
          return 1;
        return 0;
      }
    }
    else if (vert2) {
      *interx=x21;
      *intery=b1+m1*x21;
      if (mk_diff(y21,y22,eps)==.0) {
        if (mk_diff(y21,m1*x21+b1,eps)==.0)
          return 1;
        return 0;
      }
    }
    else {
      *interx=(b2-b1)/mdiff;
      *intery=(b2*m1-b1*m2)/mdiff;
    }
  }
  if (xbox==1 && (mk_diff(*interx,x11,eps)<.0 || mk_diff(*interx,x21,eps)<.0 ||
      mk_diff(*interx,x12,eps)>.0 || mk_diff(*interx,x22,eps)>.0))
    return 0;
  if (ybox==1) {
    if (mdiff==.0) {
      if (vert1==1 && vert2==0 && (mk_diff(*intery,y11,eps)<.0 || mk_diff(*intery,y12,eps)>.0))
        return 0;
      if (vert1==0 && vert2==1 && (mk_diff(*intery,y21,eps)<.0 || mk_diff(*intery,y22,eps)>.0))
        return 0;
      return 1;
    }
    if ((m1>.0 && (mk_diff(*intery,y11,eps)<.0 || mk_diff(*intery,y12,eps)>.0)) ||
        (m1<.0 && (mk_diff(*intery,y11,eps)>.0 || mk_diff(*intery,y12,eps)<0.0)))
      return 0;
    if ((m2>.0 && (mk_diff(*intery,y21,eps)<.0 || mk_diff(*intery,y22,eps)>.0)) ||
        (m2<.0 && (mk_diff(*intery,y21,eps)>.0 || mk_diff(*intery,y22,eps)<0.0)))
      return 0;
  }
  return 1;

}

void intersectionpointslinerect(
  double rl_,double rr_,double rb_,double rt_,double p1x_,double p1y_,double p2x_,double p2y_,
  double *piBx,double *piBy,double *piLx,double *piLy,double *piTx,double *piTy,double *piRx,
  double *piRy,int *cutsB,int *cutsL,int *cutsT,int *cutsR,int clip) {

  double m=.0,rl=rl_,rr=rr_,rt=rt_,rb=rb_,p1x=p1x_,p1y=p1y_,p2x=p2x_,p2y=p2y_;
  *piBx=*piBy=*piLx=*piLy=*piTx=*piTy=*piRx=*piRy=.0;
  int isLineHor=0,isLineVer=0,isPdoubled=0;
  *cutsB=1;*cutsL=1;*cutsT=1;*cutsR=1;
  if (rl>rr)
    mk_swapf(&rl,&rr);
  if (rb>rt)
    mk_swapf(&rb,&rt);
  if (p1x>p2x) {
    mk_swapf(&p1x,&p2x);
    mk_swapf(&p1y,&p2y);
  }
  if (mk_deq(p2y,p1y))
    isLineHor=1;
  if (mk_deq(p2x,p1x))
    isLineVer=1;
  /* make line equation and calc intersection points for regular line */
  if (isLineHor==0 && isLineVer==0) {
    /* special values for infinite (non hor/ver) line */
    if (p1x==-mk_dlimit)
      p1x=rl;
    else if (p1x==mk_dlimit)
      p1x=rr;
    if (p2x==-mk_dlimit)
      p2x=rl;
    else if (p2x==mk_dlimit)
      p2x=rr;
    if (p1y==-mk_dlimit)
      p1y=rb;
    else if (p1y==mk_dlimit)
      p1y=rt;
    if (p2y==-mk_dlimit)
      p2y=rb;
    else if (p2y==mk_dlimit)
      p2y=rt;
    m=(p2y-p1y)/(p2x-p1x);
    *piTx=p1x+(rt-p1y)/m;
    *piTy=rt;
    *piBx=p1x+(rb-p1y)/m;
    *piBy=rb;
    *piLx=rl;
    *piLy=p1y+m*(rl-p1x);
    *piRx=rr;
    *piRy=p1y+m*(rr-p1x);
    if (*piRy<rb || *piRy>rt)
      *cutsR=0;
    if (*piLy<rb || *piLy>rt)
      *cutsL=0;
    if (*piBx<rl || *piBx>rr)
      *cutsB=0;
    if (*piTx<rl || *piTx>rr)
      *cutsT=0;
  }
  else if(isLineHor==1) {
    *piLx=rl;
    *piLy=p1y;
    *piRx=rr;
    *piRy=p1y;
    *cutsB=*cutsT=0;
    if (*piLy<rb || *piLy>rt)
      *cutsL=0;
    if (*piRy<rb || *piRy>rt)
      *cutsR=0;
  }
  else if(isLineVer==1) {
    *piBx=p1x;
    *piBy=rb;
    *piTx=p1x;
    *piTy=rt;
    *cutsL=*cutsR=0;
    if (*piBx<rl || *piBx>rr)
      *cutsB=0;
    if (*piTx<rl || *piTx>rr)
      *cutsT=0;
  }
  else {
    isPdoubled=1;
    *cutsB=*cutsL=*cutsT=*cutsR=0;
  }
  if (clip==1) {
    if ( (p1x<rl && p2x<rl) || (p1x>rr && p2x>rr) || (p1y<rb && p2y<rb) || (p1y>rt && p2y>rt) )
      *cutsB=*cutsL=*cutsT=*cutsR=0;
    if (*cutsL==1) {
      if ( (*piLy<p1y && *piLy<p2y) || (*piLy>p1y && *piLy>p2y) || (*piLx<p1x && *piLx<p2x) )
        *cutsL=0;
    }
    if (*cutsR==1) {
      if ( (*piRy<p1y && *piRy<p2y) || (*piRy>p1y && *piRy>p2y) || (*piRx>p1x && *piRx>p2x) )
        *cutsR=0;
    }
    if (*cutsT==1) {
      if ( (*piTx<p1x && *piTx<p2x) || (*piTx>p1x && *piTx>p2x) || (*piTy>p1y && *piTy>p2y) )
        *cutsT=0;
    }
    if (*cutsB==1) {
      if ( (*piBx<p1x && *piBx<p2x) || (*piBx>p1x && *piBx>p2x) || (*piBy<p1y && *piBy<p2y) )
        *cutsB=0;
    }
  }
  if (*cutsB==1) {
    if ( (mk_deq(rb,p1y)) || (mk_deq(rb,p2y)) )
      *cutsB=0;
  }
  if (*cutsL==1) {
    if ( (mk_deq(rl,p1x)) || (mk_deq(rl,p2x)) )
      *cutsL=0;
  }
  if (*cutsT==1) {
    if ( (mk_deq(rt,p1y)) || (mk_deq(rt,p2y)) )
      *cutsT=0;
  }
  if (*cutsR==1) {
    if ( (mk_deq(rr,p1x)) || (mk_deq(rr,p2x)) )
      *cutsR=0;
  }

}

/* ########## */
int polygonintersection(
  int n1,double *poly1x,double *poly1y,
  int n2,double *poly2x,double *poly2y,
  double *interx,double *intery) {

  double m1=.0,m2=.0,mdiff=.0,b1=.0,b2=.0;
  int ii=0,jj=0,kk=0;
  double p1x1=.0,p1x2=.0,p1y1=.0,p2x1=.0,p2y1=.0,p2x2=.0,xinter=.0;
  /* line equation :
   y=m*x+b
   m=(y2-y1)/(x2-x1)
   b=y1-m*x1
   condition : m1x+b1==m2x+b2 ->
   xintersect=(b2-b1)/(m1-m2) ; yintersect=(b2*m1-b1*m2)/(m1-m2) */
  for (ii=1;ii<n1;ii++) {
    p1x1=poly1x[ii-1];
    p1y1=poly1y[ii-1];
    p1x2=poly1x[ii];
    m1=(poly1y[ii]-p1y1)/(p1x2-p1x1);
    b1=p1y1-m1*p1x1;
    for (jj=1;jj<n2;jj++) {
      p2x1=poly2x[jj-1];
      p2y1=poly2y[jj-1];
      p2x2=poly2x[jj];
      m2=(poly2y[jj]-p2y1)/(p2x2-p2x1);
      b2=p2y1-m2*p2x1;
      if ((mdiff=mk_diff(m1,m2))==.0)
        continue;
      xinter=(b2-b1)/mdiff;
      if (mk_isfinite(xinter)==0)
        continue;
      if (xinter<p1x1 || xinter>p1x2 || xinter<p2x1 || xinter>p2x2)
        continue;
      interx[kk]=xinter;
      intery[kk++]=(b2*m1-b1*m2)/mdiff;
    }
  }
  return kk;

}

double lineq(double xlow,double xhigh,double ylow,double yhigh,double xarg) {

  int dfx=0,dbx=mk_dbusted(xhigh,xlow,&dfx),dfy=0,dby=mk_dbusted(yhigh,ylow,&dfy),
      dfa=0,dba=mk_dbusted(xarg,xlow,&dfa),dbsgxl=mk_dsgn(xlow),dbsg=(dfx==0 ? dbsgxl : dfx);
  dbsg*=((dfy==0 ? mk_dsgn(yhigh) : dfy)*(dfa==0 ? dbsgxl : dfa));
  if (dfx==0 || dba!=0 || dby!=0)
    return (dbsg<0 ? mk_dsnan : mk_dnan);
  if (dbx!=0)
    return ylow;
  double res=ylow+(xarg-xlow)*(yhigh-ylow)/(xhigh-xlow);
  dba=mk_isbusted(res);
  if (dba==0)
    return res;
  res=(mk_isinf(res) ? (dba<0 ? mk_dsinf : mk_dinf) : (dba<0 ? mk_dsnan : mk_dnan));
  return res;

}

/* ########## */
double lgeq(double xlow,double xhigh,double ylow,double yhigh,double xarg) {

  if (mk_isbusted(xlow)!=0 || mk_isbusted(xhigh)!=0 || mk_isbusted(ylow)!=0 ||
      mk_isbusted(yhigh)!=0 || mk_isbusted(xarg)!=0)
    return mk_dnan;
  double dy=yhigh-ylow;
  if (xarg<=.0 || xlow<.0 || xhigh<.0 || dy==.0)
    return mk_dnan;
  int vv=0;
  if (xhigh<xlow) {
    mk_swapf(&xlow,&xhigh);
    vv=1;
  }
  if (yhigh<ylow) {
    mk_swapf(&ylow,&yhigh);
    vv=~vv;
  }
  if (xlow<=.0)
    xlow=mk_ipow10((-1)*mk_dprec); /* i do not like this - but what would the left edge be otherwise ? */
  double m=1.*pow(xhigh/xlow,1./(yhigh-ylow));
  double n=ylow-mk_logm(xlow,m);
  double res=mk_logm(xarg,m)+n;
  return ((vv&1)>0 ? yhigh-res+ylow : res);

}

int main(int argc,char **argv) {

  int ii=0,jj=0;
  double zero=.0,one=1.;

  double rl=7.,rr=11.,rb=4.,rt=15.,p1x=5.,p1y=5.,p2x=13.,p2y=7.,
    piBx=mk_dnan,piBy=mk_dnan,piLx=mk_dnan,piLy=mk_dnan,piTx=mk_dnan,piTy=mk_dnan,
    piRx=mk_dnan,piRy=mk_dnan;
  int cutsB=0,cutsL=0,cutsT=0,cutsR=0,clip=0;

  intersectionpointslinerect(rl,rr,rb,rt,p1x,p1y,p2x,p2y,
    &piBx,&piBy,&piLx,&piLy,&piTx,&piTy,&piRx,&piRy,&cutsB,&cutsL,&cutsT,&cutsR,1);

  printf("%d [%f,%f %f,%f %f,%f %f,%f] [%d,%d,%d,%d]\n",__LINE__,
    piLx,piLy,piTx,piTy,piRx,piRy,piBx,piBy,cutsL,cutsT,cutsR,cutsB);

  /* for (ii=0;ii<1;ii++) {
    for (jj=0;jj<4;jj++)
      printf ("%d [%d,%d] [%f] [%f]\n",__LINE__,ii,jj,vv1[jj],vv2[jj]);
  } */

  mk_vertex rtl={7.,15.,1.,mk_dnan},rbr={11.,4.,1.,mk_dnan},
    lp1={5.,5.,mk_dnan,mk_dnan},lp2={13.,7.,mk_dnan,mk_dnan},
    pl={mk_dnan,mk_dnan,mk_dnan,mk_dnan},pt={mk_dnan,mk_dnan,mk_dnan,mk_dnan},
    pr={mk_dnan,mk_dnan,mk_dnan,mk_dnan},pb={mk_dnan,mk_dnan,mk_dnan,mk_dnan},
    cut_ltrb={1.,1.,1.,1.};

  mk_intersectionpointslinerect(rtl,rbr,lp1,lp2,pl,pt,pr,pb,cut_ltrb,1);

  printf("%d [%f,%f %f,%f %f,%f %f,%f] [%0.f,%0.f,%0.f,%0.f]\n",__LINE__,
    pl[0],pl[1],pt[0],pt[1],pr[0],pr[1],pb[0],pb[1],
    cut_ltrb[0],cut_ltrb[1],cut_ltrb[2],cut_ltrb[3]);

  double poly1x[6]={-6.,-3.,.0,4.,2.,4.};
  double poly1y[6]={6.,6.,-3.,1.,3.,7.};
  double poly2x[5]={-7.,-7.,-3.,1.,6.};
  double poly2y[5]={-1.,2.,1.,6.,4.};
  double interx[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  double intery[4]={mk_dnan,mk_dnan,mk_dnan,mk_dnan};

  int npinter=
    polygonintersection(6,&poly1x[0],&poly1y[0],5,&poly2x[0],&poly2y[0],&interx[0],&intery[0]);
  printf("%d [%d] [%f,%f %f,%f %f,%f %f,%f]\n",__LINE__,
    npinter,interx[0],intery[0],interx[1],intery[1],interx[2],intery[2],interx[3],intery[3]);

  struct mk_vertices poly1,poly2,pinter;
  mk_verticesalloc(&poly1,6);
  mk_verticesalloc(&poly2,5);
  mk_verticesalloc(&pinter,4);
  mk_vertex poly1p[6]={
    {-6.,6.,mk_dnan,mk_dnan},{-3.,6.,mk_dnan,mk_dnan},{0.,-3.,mk_dnan,mk_dnan},
    {4.,1.,mk_dnan,mk_dnan},{2.,3.,mk_dnan,mk_dnan},{4.,7.,mk_dnan,mk_dnan}
  };
  mk_vertex poly2p[5]={
    {-7.,-1.,mk_dnan,mk_dnan},{-7.,2.,mk_dnan,mk_dnan},{-3.,1.,mk_dnan,mk_dnan},
    {1.,6.,mk_dnan,mk_dnan},{6.,4.,mk_dnan,mk_dnan}
  };
  for (ii=0;ii<6;ii++)
    mk_verticesset(&poly1,-1,poly1p[ii]);
  for (ii=0;ii<5;ii++)
    mk_verticesset(&poly2,-1,poly2p[ii]);

  char *dbgstr=0;
  npinter=mk_polygonintersection(&poly1,&poly2,&pinter);
  for (ii=0;ii<npinter;ii++)
    printf("%d #%d %s\n",__LINE__,ii,mk_vertexdbg(pinter.vertexL[ii]));

  double plineq=lineq(p1x,p2x,p1y,p2y,8.);
  printf("%d [%f]\n",__LINE__,plineq);

  plineq=mk_lineq(lp1,lp2,8.);
  printf("%d [%f]\n",__LINE__,plineq);

  double plgeq=lgeq(p1x,p2x,p1y,p2y,8.);
  printf("%d [%f]\n",__LINE__,plgeq);

  plgeq=mk_lgeq(lp1,lp2,8.);
  printf("%d [%f]\n",__LINE__,plgeq);

  mk_vertexnan(chkv);
  printf("%d [%f,%f,%f,%f]\n",__LINE__,chkv[0],chkv[1],chkv[2],chkv[3]); 

  return 0;

}

