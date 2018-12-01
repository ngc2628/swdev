
#include <mkbase/defs.h>
#include <mkbase/mkbase.h>
#include <mkbase/mkla.h>

#include <ctype.h>

/* ########## */
double mk_factorial(int nn) {

  if (nn>170 || nn<0)
    return .0;
  static double factab[171]={
    1.0 /*=0!*/,1.0 /*=1!*/,2.0 /*=2!*/,6.0 /*=3!*/,24.0 /*=4!*/,
    120.0 /*=5!*/,720.0 /*=6!*/,5040.0 /*=7!*/,40320.0 /*=8!*/,
    362880.0 /*=9!*/,3628800.0 /*=10!*/,39916800.0 /*=11!*/,
    479001600.0 /*=12!*/,6227020800.0 /*=13!*/, 87178291200.0 /*=14!*/,
    1307674368000.0 /*=15!*/,20922789888000.0 /*=16!*/,355687428096000.0 /*=17!*/};
  static int calced=17;
  if (nn<=calced)
    return factab[nn];
  int ii=0;
  /* double ret=factab[calced]; */
  for (ii=(calced+1);ii<(nn+1);ii++)
    factab[ii]=(double)ii*factab[ii-1];
  calced=nn;
  return factab[calced];

}

/* ########## */
double mk_binomialCoeff(int up,int down) {

  return mk_factorial(up)/(mk_factorial(down)*mk_factorial(up-down));

}

/* ########## */
double mk_lineq(double xlow,double xhigh,double ylow,double yhigh,double xarg) {

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
double mk_lgeq(double xlow,double xhigh,double ylow,double yhigh,double xarg) {

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

/* ########## */
void mk_intersectionpointslinerect(
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
int mk_polygonintersection(
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

/* ########## */
int mk_linesintersection(
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

/* ########## */
int mk_vertexcopy(mk_vertex *vvto,mk_vertex *vvfrom) {

  if (!vvto || !vvfrom)
    return 1;
  int ii=0;
  for (ii=0;ii<4;ii++)
    (*vvto)[ii]=(*vvfrom)[ii];
  return 0;

}

double mk_vertexlen(mk_vertex *vertex) {

  if (!vertex)
    return .0;
  double pp[3]={mk_isbusted((*vertex)[0])==0 ? (*vertex)[0] : .0,
                mk_isbusted((*vertex)[1])==0 ? (*vertex)[1] : .0,
                mk_isbusted((*vertex)[2])==0 ? (*vertex)[2] : .0};
  return sqrt(pp[0]*pp[0]+pp[1]*pp[1]+pp[2]*pp[2]);

}

/* ########## */
int mk_vertexcmp(mk_vertex *cmp1,mk_vertex *cmp2) {

  double ll1=mk_vertexlen(cmp1),ll2=mk_vertexlen(cmp2);
  return (ll1<ll2 ? -1 : (ll2<ll1 ? 1 : 0));

}

/* ########## */
int mk_vertexadd(mk_vertex *vertex,mk_vertex *addend) {

  if (!vertex || !addend)
    return 1;
  int ii=0;
  for (ii=0;ii<4;ii++) {
    if (mk_isbusted((*vertex)[ii])==0 && mk_isbusted((*addend)[ii])==0)
      (*vertex)[ii]+=(*addend)[ii];
    else
      (*vertex)[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
int mk_vertexsubs(mk_vertex *vertex,mk_vertex *addend) {

  if (!vertex || !addend)
    return 1;
  int ii=0;
  for (ii=0;ii<4;ii++) {
    if (mk_isbusted((*vertex)[ii])==0 && mk_isbusted((*addend)[ii])==0)
      (*vertex)[ii]-=(*addend)[ii];
    else
      (*vertex)[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
int mk_vertexmult(mk_vertex *vertex,double sc) {

  if (!vertex)
    return 1;
  int bsc=mk_isbusted(sc),ii=0;
  for (ii=0;ii<4;ii++) {
    if (mk_isbusted((*vertex)[ii])==0 && bsc==0)
      (*vertex)[ii]*=sc;
    else
      (*vertex)[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
int mk_vertexdiv(mk_vertex *vertex,double sc) {

  if (!vertex)
    return 1;
  int bsc=(mk_isbusted(sc) && sc!=.0 ? 0 : 1),ii=0;
  for (ii=0;ii<4;ii++) {
    if (mk_isbusted((*vertex)[ii])==0 && bsc==0)
      (*vertex)[ii]/=sc;
    else
      (*vertex)[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
double mk_vertexdot(mk_vertex *vertex1,mk_vertex *vertex2) {

  if (!vertex1 || !vertex2)
    return .0;
  mk_vertex *vertex[2]={vertex1,vertex2};
  double pp[2][3];
  int ii=0,jj=0;
  for (jj=0;jj<2;jj++) {
    for (ii=0;ii<3;ii++) {
      if (mk_isbusted((*vertex[jj])[ii])==0)
        pp[jj][ii]=(*vertex[jj])[ii];
      else
        pp[jj][ii]=mk_dnan;
    }
  }
  return (pp[0][0]*pp[1][0]+pp[0][1]*pp[1][1]+pp[0][2]*pp[1][2]);

}

/* ########## */
int mk_vertexnorm(mk_vertex *vertex) {

  if (!vertex)
    return 1;
  double ll=mk_vertexlen(vertex);
  int ii=0;
  for (ii=0;ii<3;ii++) {
    if (mk_isbusted((*vertex)[ii])==0 && ll>.0)
      (*vertex)[ii]/=ll;
    else
      (*vertex)[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
int mk_vertexcross(mk_vertex *vertex1,mk_vertex *vertex2) {

  if (!vertex1 || !vertex2)
    return 1;
  double pp[3]={mk_dnan,mk_dnan,mk_dnan};
  if (mk_isbusted((*vertex1)[1])==0 && mk_isbusted((*vertex1)[2])==0 &&
      mk_isbusted((*vertex2)[1])==0 && mk_isbusted((*vertex2)[2])==0)
    pp[0]=(*vertex1)[1]*(*vertex2)[2]-(*vertex1)[2]*(*vertex2)[1];
  if (mk_isbusted((*vertex1)[0])==0 && mk_isbusted((*vertex1)[2])==0 &&
      mk_isbusted((*vertex2)[0])==0 && mk_isbusted((*vertex2)[2])==0)
    pp[1]=(*vertex1)[2]*(*vertex2)[0]-(*vertex1)[0]*(*vertex2)[2];
  if (mk_isbusted((*vertex1)[0])==0 && mk_isbusted((*vertex1)[1])==0 &&
      mk_isbusted((*vertex2)[0])==0 && mk_isbusted((*vertex2)[1])==0)
    pp[2]=(*vertex1)[0]*(*vertex2)[1]-(*vertex1)[1]*(*vertex2)[0];
  int ii=0;
  for (ii=0;ii<3;ii++)
    (*vertex1)[ii]=pp[ii];
  return 0;

}

/* ########## */
double mk_vertexangrad(mk_vertex *vertex1,mk_vertex *vertex2) {

  if (!vertex1 || !vertex2)
    return .0;
  double den=mk_vertexlen(vertex1)*mk_vertexlen(vertex2);
  if (den==.0)
    return .0;
  return acos(mk_vertexdot(vertex1,vertex2)/den);

}

/* ########## */
double mk_vertexangdeg(mk_vertex *vertex1,mk_vertex *vertex2) {

  if (!vertex1 || !vertex2)
    return .0;
  double den=mk_vertexlen(vertex1)*mk_vertexlen(vertex2);
  if (den==.0)
    return .0;
  double arg=mk_vertexdot(vertex1,vertex2)/den;
  return acos(arg)/mk_rad;

}

/* ########## */
int mk_verticesalloc(struct mk_vertices *vertices) {

  if (vertices->vertexL)
    return 0;
  int ii=0,jj=0,sz=1;
  while (sz<vertices->reserve && (ii++)<32)
    sz*=2;
  vertices->reserve=sz;
  vertices->vertexL=(mk_vertex *)malloc(vertices->reserve*sizeof(mk_vertex));
  for (ii=0;ii<vertices->reserve;ii++) {
    for (jj=0;jj<4;jj++)
      vertices->vertexL[ii][jj]=mk_dnan;
  }
  vertices->cnt=0;
  return sz;  

}

/* ########## */
int mk_verticesfree(struct mk_vertices *vertices) {

  if (!vertices)
    return 1;
  if (vertices->vertexL)
    free(vertices->vertexL);
  vertices->vertexL=0;
  vertices->cnt=0;
  return 0;

}

/* ########## */
mk_vertex *mk_verticesget(struct mk_vertices *vertices,int idx) {

  if (!vertices || idx<0 || idx>=vertices->cnt)
    return 0;
  return &(vertices->vertexL[idx]);

}

/* ########## */
int mk_verticesappend(struct mk_vertices *vertices,mk_vertex *vertex) {

  if (!vertices || !vertex)
    return 0;
  int res=mk_verticesalloc(vertices);
  if (res>0) {
    memcpy(&(vertices->vertexL[0][0]),*vertex,sizeof(mk_vertex));
    vertices->cnt=1;
    return 1;
  }
  if (vertices->reserve==vertices->cnt) {
    mk_vertex *cpL=(mk_vertex *)malloc(vertices->cnt*sizeof(mk_vertex));
    memcpy(&(cpL[0][0]),&(vertices->vertexL[0][0]),vertices->cnt*sizeof(mk_vertex));
    free(vertices->vertexL);
    vertices->reserve*=2;
    vertices->vertexL=(mk_vertex *)malloc(vertices->reserve*sizeof(mk_vertex));
    memcpy(&(vertices->vertexL[0][0]),&(cpL[0][0]),vertices->cnt*sizeof(mk_vertex));
    free(cpL);
    int ii=0,jj=0;
    for (ii=vertices->cnt;ii<vertices->reserve;ii++) {
      for (jj=0;jj<4;jj++)
        vertices->vertexL[ii][jj]=mk_dnan;
    }
  }
  memcpy(&(vertices->vertexL[vertices->cnt][0]),&(vertex[0]),sizeof(mk_vertex));
  vertices->cnt++;
  return vertices->cnt;

}

/* ########## */
int oswinexp mk_der1(struct mk_vertices *vertices,double *derx,double *dery) {

  if (!vertices)
    return 1;

  double cc=mk_dnan,dh=mk_dnan,zdh=mk_dnan;
  if (derx) {
  
  }

  int ii=0;
  for (ii=0;ii<vertices->cnt;ii++) {
    
  }

  return 0;

}

/* ########## */
int oswinexp mk_der2xy(struct mk_vertices *vertices) {

  if (!vertices)
    return 1;

  

  

  int ii=0;
  for (ii=0;ii<vertices->cnt;ii++) {

  }

  return 0;

}

/* ########## */
int mk_matrixalloc(struct mk_matrix *matrix) {

  if (matrix->mm)
    return 0;
  int ii=0,jj=0;
  matrix->mm=(double **)malloc(matrix->rows*sizeof(double *));
  for (ii=0;ii<matrix->rows;ii++) {
    matrix->mm[ii]=(double *)malloc(matrix->cols*sizeof(double));
    for (jj=0;jj<matrix->cols;jj++)
      matrix->mm[ii][jj]=(ii==jj ? 1. : .0);
  }
  return matrix->rows*matrix->cols; 

}

/* ########## */
int mk_matrixfree(struct mk_matrix *matrix) {

  if (!matrix || !matrix->mm)
    return 1;
  int ii=0;
  for (ii=0;ii<matrix->rows;ii++)
    free(matrix->mm[ii]);
  free(matrix->mm);
  matrix->mm=0;
  return 0;

}

/* ########## */
double mk_matrixget(struct mk_matrix *matrix,int row,int col) {

  if (!matrix || row<0 || row>=matrix->rows || col<0 || col>=matrix->cols)
    return mk_dnan;
  if (!matrix->mm)
    mk_matrixalloc(matrix);
  return matrix->mm[row][col];

}

/* ########## */
double mk_matrixset(struct mk_matrix *matrix,int row,int col,double val) {

  if (!matrix || row<0 || row>=matrix->rows || col<0 || col>=matrix->cols)
    return mk_dnan;
  if (!matrix->mm)
    mk_matrixalloc(matrix);
  double oldval=matrix->mm[row][col];
  matrix->mm[row][col]=val;
  return oldval;

}

/* ----------------------------------------------------------------------------------
calculate decomposition of square matrix 'm' (num rows and num colums)
into lower(alpha) and upper(beta) triangular matrices
e.g.(rows=cols=3) m={m[1,1],m[1,2],m[1,3],m[2,1],m[2,2],m[2,3],m[3,1],m[2,3],m[3,3]} into
alpha={alpha[2,1],alpha[3,1],alpha[3,2]} and
beta={beta[1,1],beta[1,2],beta[1,3],beta[2,2],beta[2,3],beta[3,3]}
out matrix is then m=
{beta[1,1],beta[1,2],beta[1,3],alpha[2,1],beta[2,2],beta[2,3],alpha[3,1],alpha[3,2],beta[3,3]}
where diagonal is set as beta[i,i] and alpha[i,i]=1.0 as free selectable coefficients
triangular matrices alpha+beta can be easily solved by forward/backward substitution
decomposed matrix will be returned in lum to preserve the original m
------------------------------------------------------------------------------------ */
int mk_matrixludecomposition(struct mk_matrix *mat,struct mk_matrix *lumat,int *rowperm,double *parity) {

  if (!mat || !rowperm || mat->rows<=0 || mat->rows!=mat->cols)
    return -1;
  int ii=0,jj=0,kk=0,imax=0,num=mat->rows;
  double maxcoeff=.0,tmp=.0;
  for (ii=0;ii<num;ii++)
    rowperm[ii]=ii; // no row interchanging yet
  *parity=1.;
  /* first find the largest element in every row (implicit pivoting)
   also copy the original matrix since we do not want to destroy it */
  double *rowscale=(double*)malloc((size_t)num*sizeof(double));
  for (ii=0;ii<num;ii++) {
    maxcoeff=.0;
    for (jj=0;jj<num;jj++) {
      lumat->mm[ii][jj]=mat->mm[ii][jj];
      tmp=fabs(lumat->mm[ii][jj]);
      if (tmp>maxcoeff)
        maxcoeff=tmp;
    }
    if (mk_deq(maxcoeff,.0))
      return -1;
    rowscale[ii]=maxcoeff;
  }
  /* loop every column
   used alphas and betas are already calculated by the time they are needed */
  for (jj=0;jj<num;jj++) {
    /* loop rows for 'u'pper triangular matrix */
    for (ii=0;ii<jj;ii++) {
      /* do the matrix multiplication
       beta[i,j]=m[i,j]-sum(alpha[i,k]*beta[k,j]) */
      for (kk=0;kk<ii;kk++)
        lumat->mm[ii][jj]-=lumat->mm[ii][kk]*lumat->mm[kk][jj];     
    }
    maxcoeff=.0;
    /* loop rows for 'l'ower triangular matrix
     and diagonal (denominators for lower matrix elements) inclusive */
    for (ii=jj;ii<num;ii++) {
      /* do the matrix multiplication
       beta[j,j]*alpha[i,j]=m[i,j]-sum(alpha[i,k]*beta[k,j]) */
      for (kk=0;kk<jj;kk++)
        lumat->mm[ii][jj]-=lumat->mm[ii][kk]*lumat->mm[kk][jj];
      /* beta[j,j] is to calculate as pivot(largest element)
       from this row=i and (precalculated) =rowscale[i] */
      tmp=fabs(lumat->mm[ii][jj])/rowscale[ii];
      if (tmp>=maxcoeff) {
        maxcoeff=tmp;
        imax=ii;
      }
    }
    /* if index of precalculated rowscale!=scale[actrow]
     rows must be interchanged and index table updated
     for later dividing by pivot element (beta[j,j])
     this is possible since columns<j are already determined, and
     columns>j are not used yet, therefore row interchanging
     does not destroy the solution only just scrambles the order
     which means that the out-matrix may look queer but dissolves into
     a rowwise permutation of m */
    if (jj!=imax) {
      for (ii=0;ii<num;ii++)
        mk_swapf(&lumat->mm[imax][ii],&lumat->mm[jj][ii]);
      rowscale[imax]=rowscale[jj]; /* rowscale[j] is not needed anymore */
      mk_swapi(&rowperm[jj],&rowperm[imax]);
      *parity=-(*parity);
    }
    if (mk_deq(lumat->mm[jj][jj],.0))
      return -1;
    /* finallly (for this column) divide all lower row elements by the pivot */
    if (jj<(num-1)) {
      for (ii=(jj+1);ii<num;ii++)
        lumat->mm[ii][jj]/=lumat->mm[jj][jj];
    }
  }
  free(rowscale);
  return 0;

}

/* ---------------------------------------------------------------------------------
calculate solution for matrix m and right hand side vector r when incoming matrix lum
is the lu-decomposition of the rowwise permutation of m
(arranged the same way as output of --> ludecomposition)
---------------------------------------------------------------------------------- */
int mk_matrixlubacksubstitution(struct mk_matrix *lumat,int *lurowperm,double *rr,double *xx) {

  if (!lumat || lumat->rows<=0 || lumat->rows!=lumat->cols || !lurowperm || !rr || !xx)
    return -1;
  /* first adapt the row permutation for the right hand side vector r
   also copy right hand side input (do not destroy) */
  int ii=0,jj=0,kk=0,num=lumat->rows;
  for (ii=0;ii<num;ii++)
    xx[ii]=rr[lurowperm[ii]];
  /* do the forward substitution by solving for the lower triangular matrix (alpha)
   e.g. for (rows=cols=3) lum(lowerpart)={lum[1,1]=1,lum[1,2]=0,lum[1,3]=0,
   lum[2,1],lum[2,2]=1,lum[2,3]=0,lum[3,1],lum[3,2],lum[3,3]=1} as determined
   in --> ludecomposition */
  for (ii=0;ii<num;ii++) {
    for (jj=0;jj<ii;jj++)
      xx[ii]-=lumat->mm[ii][jj]*xx[jj];
  }
  /* now do the backsubstitution by solving for the upper triangular matrix (beta)
   e.g. for (rows=cols=3) lum(upperpart)={lum[1,1],lum[1,2],lum[1,3],
   lum[2,1]=0,lum[2,2],lum[2,3],lum[3,1]=0,lum[3,2]=0,lum[3,3]} as determined
   in --> ludecomposition
   (since pivot is not ==1 here we have to do the dividing) */
  double tmp=.0;
  for (ii=(num-1);ii>-1;ii--) {
    tmp=xx[ii];
    for (jj=(ii+1);jj<num;jj++) {
      tmp-=lumat->mm[ii][jj]*xx[jj];
    }
    xx[ii]=tmp/lumat->mm[ii][ii];
  }
  return 0;

}

/* ########## */
int mk_matrixsolve(struct mk_matrix *mat,double *rr,double *xx) {

  if (!mat || !rr || !xx || mat->rows<=0 || mat->rows!=mat->cols)
    return -1;
  int num=mat->rows;
  struct mk_matrix lumat;
  lumat.mm=0;
  lumat.rows=lumat.cols=num;
  mk_matrixalloc(&lumat);
  int *rowperm=(int*)malloc(num*sizeof(int));
  memset(&rowperm[0],0,num*sizeof(int));
  double *parity=(double*)malloc(num*sizeof(double));
  memset(&parity[0],0,num*sizeof(double));
  int res=mk_matrixludecomposition(mat,&lumat,&rowperm[0],&parity[0]);
  if (res==0)
    res=mk_matrixlubacksubstitution(&lumat,&rowperm[0],rr,xx);
  free(parity);
  free(rowperm);
  mk_matrixfree(&lumat);
  return res;

}

/* ########## */
int mk_polynomialalloc(struct mk_polynomial *polynomial) {

  if (mk_polynomialfree(polynomial)==1)
    return 1;
  int ii=0,jj=0,ll=polynomial->len;
  polynomial->ctrlL=(mk_vertex*)malloc(polynomial->len*sizeof(mk_vertex));
  polynomial->cc=(double**)malloc(polynomial->len*sizeof(double *));
  polynomial->dd=(double**)malloc(polynomial->len*sizeof(double *));
  for (ii=0;ii<polynomial->len;ii++) {
    memset(&polynomial->ctrlL[ii],0,sizeof(mk_vertex));
    polynomial->cc[ii]=(double*)malloc(ll*sizeof(double));
    polynomial->dd[ii]=(double*)malloc(ll*sizeof(double));
    for (jj=0;jj<ll;jj++)
      polynomial->cc[ii][jj]=polynomial->dd[ii][jj]=.0;
    ll--;
  }
  return 0;
 
}

/* ########## */
int mk_polynomialfree(struct mk_polynomial *polynomial) {

  if (!polynomial)
    return 1;
  if (polynomial->ctrlL)
    free(polynomial->ctrlL);
  polynomial->ctrlL=0;
  if (polynomial->cc)
    free(polynomial->cc);
  polynomial->cc=0;
  if (polynomial->dd)
    free(polynomial->dd);
  polynomial->dd=0;
  return 0;

}

/* ########## */
double mk_polynomialinterp(struct mk_polynomial *polynomial,double xx) {

  if (!polynomial || mk_isnan(xx))
    return mk_dnan;
  int ii=0,jj=0,idx=0,downward=0;
  double res=.0,dxmin=fabs(polynomial->ctrlL[0][0]-xx),dxl=.0,dxh=.0,dxincr=.0;
  /* first columns of cc,dd are the ctrlL function values */ 
  for (ii=0;ii<polynomial->len;ii++)
    polynomial->cc[0][ii]=polynomial->dd[0][ii]=polynomial->ctrlL[ii][1];
  /* update cc,dd */
  for (ii=1;ii<polynomial->len;ii++) {
    /* calc nearest distance to (then) starting point in ctrlL underway */
    if (fabs(polynomial->ctrlL[ii][0]-xx)<dxmin) {
      dxmin=fabs(polynomial->ctrlL[ii][0]-xx);
      idx=ii;
    }
    /* 
    lagrange :
    p(x)=(x-x1)*(x-x2)*...*(x-xn)*y0/(x0-x1)*(x0-x2)*...*(x0-xn) +
         (x-x0)*(x-x2)*...*(x-xn)*y1/(x1-x0)*(x1-x2)*...*(x1-xn) + ...
         (x-x0)*(x-x1)*...*(x-xn-1)*yn/(xn-x0)*(xn-x1)*...*(xn-xn-1)
    ===> neville aitken : 
    p0(x)=y0
             p01(x)=((x-x1)*p0+(x0-x)*p1)/(x0-x1)
    p1(x)=y1                                      p012=((x-x2)*p01+(x0-x)*p12)/(x0-x2)
             p12(x)=((x-x2)*p1+(x1-x)*p2)/(x1-x2)
    p2(x)=y2                                      p123=((x-x3)*p12+(x0-x)*p23)/(x0-x3)
             p23(x)=((x-x3)*p2+(x2-x)*p3)/(x2-x3)
    ===> diffs :
    c10=p01-p0
    d10=p01-p1   
    c11=p12-p1
    d11=p12-p2
    c12=p23-p2 
               c20=p012-p01=((x0-x)*(c11-d10))/(x0-x2)
               d20=p012-p12=((x2-x)*(c11-d10))/(x0-x2)
               c21=p123-p12=((x1-x)*(c12-d11))/(x1-x3)
               d21=p123-p23=((x3-x)*(c12-d11))/(x1-x3)
    */
    for (jj=0;jj<(polynomial->len-ii);jj++) {
      dxl=polynomial->ctrlL[jj][0]-xx;
      dxh=polynomial->ctrlL[jj+ii][0]-xx;
      dxincr=polynomial->cc[ii-1][jj+1]-polynomial->dd[ii-1][jj];
      dxincr=(mk_deq(dxh-dxl,.0) ? .0 : dxincr/(dxl-dxh));
      polynomial->cc[ii][jj]=dxincr*dxl;
      polynomial->dd[ii][jj]=dxincr*dxh;
    }
  }
  /*
  accumulate result from diffs c,d (s.a) - walk along any of these ways 
  p(x)=p0+c10+c20 down-down     
  p(x)=p1+d10+c20 up-down | p1+c11+d20 down-up | p1+c11+c21 down-down
  p(x)=p2+d11+c20 up-up | p2+d11+c21 up-down | p2+c12+d21 down-up
  way up-or-down to nearest difference at resp index position 
  */
  res=polynomial->cc[0][idx];
  if (idx==0 || polynomial->len/idx>=2)
    downward=1;
  for (ii=1;ii<polynomial->len;ii++) {
    if (downward==1) {
      if (idx>=1)
        downward=0;
      res+=polynomial->cc[ii][idx];
    }
    else {
      if (idx<1 || idx<(polynomial->len-ii-1))
        downward=1;
      res+=polynomial->dd[ii][--idx];
    }
  }
  return res;

}

/* ########## */
int mk_polynomialcoeff(struct mk_polynomial *polynomial,double *coeff) {

  if (!polynomial || !coeff)
    return 1;
  int ii=0,idx=0,jj=0,kk=0,ll=0;
  double min=mk_dnan,zcoeff=.0;
  struct mk_polynomial pp;
  pp.ctrlL=0;
  pp.cc=0;
  pp.dd=0;
  pp.len=polynomial->len;
  mk_polynomialalloc(&pp);
  memcpy(&pp.ctrlL[0],&(polynomial->ctrlL[0]),pp.len*sizeof(mk_vertex));
  for (ii=0;ii<polynomial->len;ii++) {
    zcoeff=mk_polynomialinterp(&pp,.0);
    coeff[idx++]=zcoeff;
    min=mk_dnan;
    kk=0;
    for (jj=0;jj<polynomial->len-ii;jj++) {
      if (mk_isnan(min)!=0 || fabs(pp.ctrlL[jj][0])<min) {
        min=fabs(pp.ctrlL[jj][0]);
        kk=jj;
      }
      /* p(x)=a0+a1*x+a2*x*x+a3*x*x*x+a4*x*x*x*x .....
      ===> (p(x)-a0)/x=a1+a2*x+a3*x*x+a4*x*x*x ... */
      pp.ctrlL[jj][1]=(pp.ctrlL[jj][1]-zcoeff)/pp.ctrlL[jj][0];
    }
    /* recycle pp with shifted down elements and reinitialize cc,dd */
    for (jj=kk+1;jj<polynomial->len-ii;jj++) {
      pp.ctrlL[jj-1][0]=pp.ctrlL[jj][0];
      pp.ctrlL[jj-1][1]=pp.ctrlL[jj][1];
    }
    pp.len--;
    jj=pp.len;
    for (kk=0;kk<pp.len;kk++) {
      pp.cc[0][kk]=pp.dd[0][kk]=pp.ctrlL[kk][1];
      if (kk>0) {
        for (ll=0;ll<jj;ll++)
          pp.cc[kk][ll]=pp.dd[kk][ll]=.0;
      }
      jj--;
    }
  }
  mk_polynomialfree(&pp);
  return 0;

}

