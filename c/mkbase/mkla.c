
#include <mkbase/defs.h>
#include <mkbase/mkutil.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>

#include <ctype.h>

/* ########## */
int mk_vertexdbg(const mk_vertex vertex,mk_string str) {

  mk_string resx;
  mk_stringset(resx,0);
  char *buf=0;
  resx[0]='[';
  int ii=0,jj=0;
  for (ii=3;ii>-1;ii--) {
    if (mk_isnan(vertex[ii])==0) {
      for (jj=0;jj<ii+1;jj++) {
        mk_d2a(vertex[jj],resx,3);
        mk_stringappend(str,&resx[0]);
        if (jj<ii)
          mk_stringappend(str,";");
      }
      break;
    }
  }
  return 0;

}

/* ########## */
int mk_vertexcopy(mk_vertex to,const mk_vertex from) {

  memcpy(&to[0],&from[0],4*sizeof(double));
  return 0;

}

/* ########## */
int mk_vertexset(mk_vertex vertex,double val) {

  vertex[0]=vertex[1]=vertex[2]=vertex[3]=val;
  return 0;

}

/* ########## */
int mk_vertexswap(mk_vertex vv1,mk_vertex vv2) {

  mk_vertexnan(swp);
  mk_vertexcopy(swp,vv1);
  mk_vertexcopy(vv1,vv2);
  mk_vertexcopy(vv2,swp);
  return 0;

}

/* ########## */
double mk_vertexlen(const mk_vertex vertex) {

  mk_vertexzero(pp);
  int ii=0;
  for (ii=0;ii<3;ii++)
    if (mk_isbusted(vertex[ii])==0)
      pp[ii]=vertex[ii];
  return sqrt(pp[0]*pp[0]+pp[1]*pp[1]+pp[2]*pp[2]);

}

/* ########## */
int mk_vertexcmpidx(const void *cmp1,const void *cmp2,int idx) {

  idx=MIN(3,MAX(0,idx));
  const mk_vertex *vv1=(const mk_vertex*)cmp1;
  const mk_vertex *vv2=(const mk_vertex*)cmp2;
  if ((*vv1)[idx]<(*vv2)[idx])
    return -1;
  if ((*vv2)[idx]<(*vv1)[idx])
    return 1;
  return 0;

}

/* ########## */
int mk_vertexcmpx(const void *cmp1,const void *cmp2) {

  return mk_vertexcmpidx(cmp1,cmp2,0);

}

/* ########## */
int mk_vertexcmpy(const void *cmp1,const void *cmp2) {

  return mk_vertexcmpidx(cmp1,cmp2,1);

}

/* ########## */
int mk_vertexcmpz(const void *cmp1,const void *cmp2) {

  return mk_vertexcmpidx(cmp1,cmp2,2);

}

/* ########## */
int mk_vertexcmpw(const void *cmp1,const void *cmp2) {

  return mk_vertexcmpidx(cmp1,cmp2,3);

}

/* ########## */
mk_vertexcmpf mk_vertexcmp[4]={mk_vertexcmpx,mk_vertexcmpy,mk_vertexcmpz,mk_vertexcmpw};

/* ########## */
int mk_vertexadd(const mk_vertex vertex1,const mk_vertex vertex2,mk_vertex res) {

  int ii=0;
  for (ii=0;ii<4;ii++) {
    if (mk_isbusted(vertex1[ii])==0 && mk_isbusted(vertex2[ii])==0)
      res[ii]=vertex1[ii]+vertex2[ii];
    else
      res[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
int mk_vertexsubs(const mk_vertex vertex1,const mk_vertex vertex2,mk_vertex res) {

  int ii=0;
  for (ii=0;ii<4;ii++) {
    if (mk_isbusted(vertex1[ii])==0 && mk_isbusted(vertex2[ii])==0)
      res[ii]=vertex1[ii]-vertex2[ii];
    else
      res[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
int mk_vertexmult(const mk_vertex vertex,double sc,mk_vertex res) {

  int bsc=mk_isbusted(sc),ii=0;
  for (ii=0;ii<4;ii++) {
    if (mk_isbusted(vertex[ii])==0 && bsc==0)
      res[ii]=vertex[ii]*sc;
    else
      res[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
int mk_vertexdiv(const mk_vertex vertex,double sc,mk_vertex res) {

  int bsc=(mk_isbusted(sc) && sc!=.0 ? 0 : 1),ii=0;
  for (ii=0;ii<4;ii++) {
    if (mk_isbusted(vertex[ii])==0 && bsc==0)
      res[ii]=vertex[ii]/sc;
    else
      res[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
double mk_vertexdot(const mk_vertex vertex1,const mk_vertex vertex2) {

  mk_vertexzero(vertex);
  int ii=0;
  for (ii=0;ii<3;ii++) {
    if (mk_isbusted(vertex1[ii])==0 && mk_isbusted(vertex2[ii])==0)
      vertex[ii]=vertex1[ii]*vertex2[ii];
  }
  double res=vertex[0]+vertex[1]+vertex[2];
  return res;

}

/* ########## */
int mk_vertexnorm(const mk_vertex vertex,mk_vertex vn) {

  mk_vertexset(vn,mk_dnan);
  double ll=mk_vertexlen(vertex);
  int ii=0;
  for (ii=0;ii<3;ii++) {
    if (mk_isbusted(vertex[ii])==0 && ll>.0)
      vn[ii]=vertex[ii]/ll;
    else
      vn[ii]=mk_dnan;
  }
  return 0;

}

/* ########## */
int mk_vertexcross(const mk_vertex vertex1,const mk_vertex vertex2,mk_vertex vc) {

  mk_vertexset(vc,mk_dnan);
  if (mk_isbusted(vertex1[1])==0 && mk_isbusted(vertex1[2])==0 &&
      mk_isbusted(vertex2[1])==0 && mk_isbusted(vertex2[2])==0)
    vc[0]=vertex1[1]*vertex2[2]-vertex1[2]*vertex2[1];
  if (mk_isbusted(vertex1[0])==0 && mk_isbusted(vertex1[2])==0 &&
      mk_isbusted(vertex2[0])==0 && mk_isbusted(vertex2[2])==0)
    vc[1]=vertex1[2]*vertex2[0]-vertex1[0]*vertex2[2];
  if (mk_isbusted(vertex1[0])==0 && mk_isbusted(vertex1[1])==0 &&
      mk_isbusted(vertex2[0])==0 && mk_isbusted(vertex2[1])==0)
    vc[2]=vertex1[0]*vertex2[1]-vertex1[1]*vertex2[0];
  return 0;

}

/* ########## */
double mk_vertexangrad(const mk_vertex vertex1,const mk_vertex vertex2) {

  double den=mk_vertexlen(vertex1)*mk_vertexlen(vertex2);
  if (den==.0)
    return .0;
  return acos(mk_vertexdot(vertex1,vertex2)/den);
  //return atan2(sqrt(1.-arg*arg),arg)/rad;

}

/* ########## */
double mk_vertexangdeg(const mk_vertex vertex1,const mk_vertex vertex2) {

  double den=mk_vertexlen(vertex1)*mk_vertexlen(vertex2);
  if (den==.0)
    return .0;
  double arg=mk_vertexdot(vertex1,vertex2)/den;
  return acos(arg)/mk_rad;

}

/* ########## */
double mk_lineq(const mk_vertex lp1,const mk_vertex lp2,double xarg) {

  int dfx=0,dbx=mk_dbusted(lp2[0],lp1[0],&dfx),dfy=0,dby=mk_dbusted(lp2[1],lp1[1],&dfy),
      dfa=0,dba=mk_dbusted(xarg,lp1[0],&dfa),dbsgxl=mk_dsgn(lp1[0]),dbsg=(dfx==0 ? dbsgxl : dfx);
  dbsg*=((dfy==0 ? mk_dsgn(lp1[1]) : dfy)*(dfa==0 ? dbsgxl : dfa));
  if (dfx==0 || dba!=0 || dby!=0)
    return (dbsg<0 ? mk_dsnan : mk_dnan);
  if (dbx!=0)
    return lp1[1];
  double res=lp1[1]+(xarg-lp1[0])*(lp2[1]-lp1[1])/(lp2[0]-lp1[0]);
  dba=mk_isbusted(res);
  if (dba==0)
    return res;
  res=(mk_isinf(res) ? (dba<0 ? mk_dsinf : mk_dinf) : (dba<0 ? mk_dsnan : mk_dnan));
  return res;

}

/* ########## */
double mk_lgeq(const mk_vertex hhp1,const mk_vertex hhp2,double xarg) {

  if (mk_isbusted(hhp1[0])!=0 || mk_isbusted(hhp2[0])!=0 || mk_isbusted(hhp1[1])!=0 ||
      mk_isbusted(hhp2[1])!=0 || mk_isbusted(xarg)!=0)
    return mk_dnan;
  double dy=hhp2[1]-hhp1[1];
  if (xarg<=.0 || hhp1[0]<.0 || hhp2[0]<.0 || dy==.0)
    return mk_dnan;
  mk_vertex hp1;
  mk_vertex hp2;
  mk_vertexcopy(hp1,hhp1);
  mk_vertexcopy(hp2,hhp2);
  int vv=0;
  if (hp2[0]<hp1[0]) {
    mk_swapf(&hp1[0],&hp2[0]);
    vv=1;
  }
  if (hp2[1]<hp1[1]) {
    mk_swapf(&hp1[1],&hp2[1]);
    vv=~vv;
  }
  if (hp1[0]<=.0)
    /* i do not like this - but what would the left edge be otherwise ? */
    hp1[0]=mk_ipow10((-1)*mk_dprec); 
  double mm=1.*pow(hp2[0]/hp1[0],1./(hp2[1]-hp1[1]));
  double nn=hp1[1]-mk_logm(hp1[0],mm);
  double res=mk_logm(xarg,mm)+nn;
  return ((vv&1)>0 ? hp2[1]-res+hp1[1] : res);

}

/* ########## */
int mk_linesintersection(const mk_vertex ll1p1,const mk_vertex ll1p2,const mk_vertex ll2p1,
  const mk_vertex ll2p2,mk_vertex pinter,int prec,int box) {
  /* line equation :
   y=m*x+b
   m=(y2-y1)/(x2-x1)
   b=y1-m*x1
   condition : m1x+b1==m2x+b2 ->
   xintersect=(b2-b1)/(m1-m2) ; yintersect=(b2*m1-b1*m2)/(m1-m2) */
  mk_vertex l1p1;
  mk_vertex l1p2;
  mk_vertex l2p1;
  mk_vertex l2p2;
  mk_vertexcopy(l1p1,ll1p1);
  mk_vertexcopy(l1p2,ll1p2);
  mk_vertexcopy(l2p1,ll2p1);
  mk_vertexcopy(l2p2,ll2p2);
  double eps=mk_ipow10(-prec);
  if (mk_diff(l1p1[0],l1p2[0],eps)>.0)
    mk_vertexswap(l1p1,l1p2);
  if (mk_diff(l2p1[0],l2p2[0],eps)>.0)
    mk_vertexswap(l2p1,l2p2);
  double m1=.0,m2=.0;
  int vertical=0;
  if (mk_diff(l1p1[0],l1p2[0],eps)==.0)
    vertical|=1;
  else {
    m1=(l1p2[1]-l1p1[1])/(l1p2[0]-l1p1[0]);
    if (mk_isfinite(m1)==0) {
      vertical|=1;
      m1=.0;
    }
  }
  if ((vertical&1)>0 && mk_diff(l1p1[1],l1p2[1],eps)>.0)
    mk_swapf(&l1p1[1],&l1p2[1]);
  if (mk_diff(l2p1[0],l2p2[0],eps)==.0)
    vertical|=2;
  else {
    m2=(l2p2[1]-l2p1[1])/(l2p2[0]-l2p1[0]);
    if (mk_isfinite(m2)==0) {
      vertical|=2;
      m2=.0;
    }
  }
  if ((vertical&2)>0 && mk_diff(l2p1[1],l2p2[1],eps)>.0)
    mk_swapf(&l2p1[1],&l2p2[1]);
  double b1=l1p1[1]-m1*l1p1[0],b2=l2p1[1]-m2*l2p1[0];
  if (mk_diff(m1,m2)==.0) {
    if (vertical==1) {
      pinter[0]=l1p1[0];
      pinter[1]=l2p1[1];
    }  
    else if (vertical==2) {
      pinter[0]=l2p1[0];
      pinter[1]=l1p1[1];
    }
    else if ((vertical==3 && mk_diff(l1p1[0],l2p1[0],eps)==.0) ||
             (vertical==0 && mk_diff(l1p1[1],l2p1[1],eps)==.0) ||
             (l2p1[1]==(b1+m1*l2p1[0]))) {  /* degenerated case (do not know ... to be precised) */
      pinter[0]=l1p1[0];
      pinter[1]=l1p1[1];
      if (box>0)
        return 0;
      return 1;
    }
    else {
      pinter[0]=.0;
      pinter[1]=.0;
      return 0;
    }
  }
  else {
    if ((vertical&1)>0) {
      pinter[0]=l1p1[0];
      pinter[1]=b2+m2*l1p1[0];
      if (mk_diff(l1p1[1],l1p2[1],eps)==.0) {
        if (mk_diff(l1p1[1],m2*l1p1[0]+b2,eps)==.0)
          return 1;
        return 0;
      }
    }
    else if ((vertical&2)>0) {
      pinter[0]=l2p1[0];
      pinter[1]=b1+m1*l2p1[0];
      if (mk_diff(l2p1[1],l2p2[1],eps)==.0) {
        if (mk_diff(l2p1[1],m1*l2p1[0]+b1,eps)==.0)
          return 1;
        return 0;
      }
    }
    else {
      pinter[0]=(b2-b1)/mk_diff(m1,m2);
      pinter[1]=(b2*m1-b1*m2)/mk_diff(m1,m2);
/*printf("%d [%.1f,%.1f]\n",__LINE__,pinter[0],pinter[1]);*/
    }
  }
  if ((box&1)>0 && (mk_diff(pinter[0],l1p1[0],eps)<.0 || mk_diff(pinter[0],l2p1[0],eps)<.0 || 
      mk_diff(pinter[0],l1p2[0],eps)>.0 || mk_diff(pinter[0],l2p2[0],eps)>.0))
    return 0;
  if ((box&2)>0) {
    if (mk_diff(m1,m2)==.0) {
      if (vertical==1 && (mk_diff(pinter[1],l1p1[1],eps)<.0 || mk_diff(pinter[1],l1p2[1],eps)>.0))
        return 0;
      if (vertical==2 && (mk_diff(pinter[1],l2p1[1],eps)<.0 || mk_diff(pinter[1],l2p2[1],eps)>.0))
        return 0;
      return 1;
    }
    if ((m1>.0 && (mk_diff(pinter[1],l1p1[1],eps)<.0 || mk_diff(pinter[1],l1p2[1],eps)>.0)) || 
        (m1<.0 && (mk_diff(pinter[1],l1p1[1],eps)>.0 || mk_diff(pinter[1],l1p2[1],eps)<0.0)))
      return 0;
    if ((m2>.0 && (mk_diff(pinter[1],l2p1[1],eps)<.0 || mk_diff(pinter[1],l2p2[1],eps)>.0)) || 
        (m2<.0 && (mk_diff(pinter[1],l2p1[1],eps)>.0 || mk_diff(pinter[1],l2p2[1],eps)<0.0)))
      return 0;
  }
  return 1;

}

/* ########## */
int mk_intersectionpointslinerect(
  const mk_vertex rrtl,const mk_vertex rrbr,const mk_vertex llp1,const mk_vertex llp2,
  mk_vertex pl,mk_vertex pt,mk_vertex pr,mk_vertex pb,mk_vertex cut_ltrb,int clip) {

  /* cuts at every rect-bound */
  mk_vertexset(cut_ltrb,1.);
  mk_vertex rtl;
  mk_vertex rbr;
  mk_vertex lp1;
  mk_vertex lp2;
  mk_vertexcopy(rtl,rrtl);
  mk_vertexcopy(rbr,rrbr);
  mk_vertexcopy(lp1,llp1);
  mk_vertexcopy(lp2,llp2);
  /* normalize rect */
  if (rtl[0]>rbr[0])
    mk_swapf(&rtl[0],&rbr[0]);
  if (rbr[1]>rtl[1])
    mk_swapf(&rbr[1],&rtl[1]);
  /* normalize line */
  if (lp1[0]>lp2[0]) 
    mk_vertexswap(lp1,lp2);
  /* make line equation and calc intersection points for regular line */
  if (mk_deq(lp1[0],lp2[0])==0 && mk_deq(lp1[1],lp2[1])==0) {
    /* special values for infinite (non hor/ver) line */
    if (mk_isinf(lp1[0])!=0)
      lp1[0]=(mk_isinf(lp1[0])<0 ? rtl[0] : rbr[0]);
    if (mk_isinf(lp2[0])!=0)
      lp2[0]=(mk_isinf(lp2[0])<0 ? rtl[0] : rbr[0]);
    if (mk_isinf(lp1[1])!=0)
      lp1[1]=(mk_isinf(lp1[1])<0 ? rbr[1] : rtl[1]);
    if (mk_isinf(lp2[1])!=0)
      lp2[1]=(mk_isinf(lp2[1])<0 ? rbr[1] : rtl[1]);
    /* gradient */
    double mm=(lp2[1]-lp1[1])/(lp2[0]-lp1[0]);
    /* interpolated points cuts on either side of rect */
    pl[0]=rtl[0];
    pl[1]=lp1[1]+mm*(rtl[0]-lp1[0]);
    if (pl[1]<rbr[1] || pl[1]>rtl[1])
      cut_ltrb[0]=.0;
    pt[0]=lp1[0]+(rtl[1]-lp1[1])/mm;
    pt[1]=rtl[1];
    if (pt[0]<rtl[0] || pt[0]>rbr[0])
      cut_ltrb[1]=.0;
    pr[0]=rbr[0];
    pr[1]=lp1[1]+mm*(rbr[0]-lp1[0]);
    if (pr[1]<rbr[1] || pr[1]>rtl[1])
      cut_ltrb[2]=.0;
    pb[0]=lp1[0]+(rbr[1]-lp1[1])/mm;
    pb[1]=rbr[1];
    if (pb[0]<rtl[0] || pb[0]>rbr[0])
      cut_ltrb[3]=.0;
  }
  else if(mk_deq(lp1[1],lp2[1])!=0) {
    pl[0]=rtl[0];
    pl[1]=lp1[1];
    pr[0]=rbr[0];
    pr[1]=lp1[1];
    cut_ltrb[1]=cut_ltrb[3]=.0;
    if (pl[1]<rbr[1] || pl[1]>rtl[1])
      cut_ltrb[0]=.0;
    if (pr[1]<rbr[1] || pr[1]>rtl[1])
      cut_ltrb[2]=.0;
  }
  else if(mk_deq(lp1[0],lp2[0])!=0) {
    pt[0]=lp1[0];
    pt[1]=rbr[1];
    pb[0]=lp1[0];
    pb[1]=rtl[1];
    cut_ltrb[0]=cut_ltrb[2]=.0;
    if (pt[0]<rtl[0] || pt[0]>rbr[0])
      cut_ltrb[1]=.0;
    if (pb[0]<rtl[0] || pb[0]>rbr[0])
      cut_ltrb[3]=.0;
  }
  else {
    cut_ltrb[0]=cut_ltrb[1]=cut_ltrb[2]=cut_ltrb[3]=.0;
  }
  if (clip==1) {
    if ( (lp1[0]<rtl[0] && lp2[0]<rtl[0]) || 
         (lp1[0]>rbr[0] && lp2[0]>rbr[0]) || 
         (lp1[1]<rbr[1] && lp2[1]<rbr[1]) || 
         (lp1[1]>rtl[1] && lp2[1]>rtl[1]) )
      cut_ltrb[0]=cut_ltrb[1]=cut_ltrb[2]=cut_ltrb[3]=.0;
    if (cut_ltrb[0]==1.) {
      if ( (pl[1]<lp1[1] && pl[1]<lp2[1]) || 
           (pl[1]>lp1[1] && pl[1]>lp2[1]) || 
           (pl[0]<lp1[0] && pl[0]<lp2[0]) )
        cut_ltrb[0]=.0;
    }
    if (cut_ltrb[2]==1.) {
      if ( (pr[1]<lp1[1] && pr[1]<lp2[1]) || 
           (pr[1]>lp1[1] && pr[1]>lp2[1]) || 
           (pr[0]>lp1[0] && pr[0]>lp2[0]) )
        cut_ltrb[2]=.0;
    }
    if (cut_ltrb[1]==1.) {
      if ( (pt[0]<lp1[0] && pt[0]<lp2[0]) || 
           (pt[0]>lp1[0] && pt[0]>lp2[0]) || 
           (pt[1]>lp1[1] && pt[1]>lp2[1]) )
        cut_ltrb[1]=.0;
    }
    if (cut_ltrb[3]==1.) {
      if ( (pb[0]<lp1[0] && pb[0]<lp2[0]) || 
           (pb[0]>lp1[0] && pb[0]>lp2[0]) || 
           (pb[1]<lp1[1] && pb[1]<lp2[1]) )
        cut_ltrb[3]=.0;
    }
  }
  if (cut_ltrb[3]==1.) {
    if ( (mk_deq(rbr[1],lp1[1])) || (mk_deq(rbr[1],lp2[1])) )
      cut_ltrb[3]=.0;
  }
  if (cut_ltrb[0]==1.) {
    if ( (mk_deq(rtl[0],lp1[0])) || (mk_deq(rtl[0],lp2[0])) )
      cut_ltrb[0]=.0;
  }
  if (cut_ltrb[1]==1.) {
    if ( (mk_deq(rtl[1],lp1[1])) || (mk_deq(rtl[1],lp2[1])) )
      cut_ltrb[1]=.0;
  }
  if (cut_ltrb[2]==1.) {
    if ( (mk_deq(rbr[0],lp1[0])) || (mk_deq(rbr[0],lp2[0])) )
      cut_ltrb[2]=.0;
  }
  return 0;

}

/* ########## */
int mk_ellipse(struct mk_list *vertices,int nn) {

  if (!vertices)
    return 1;
  mk_listclear(vertices,0);
  nn=MAX(1,MIN(vertices->reserved,nn));
  int ii=0,off=nn/2;
  double sc=360./(double)nn;
  mk_vertexnan(vv);
  for (ii=0;ii<nn;ii++) {  
    if (ii<=off) {
      vv[0]=cos(sc*mk_rad*(double)ii);
      vv[1]=sin(sc*mk_rad*(double)ii);
    }
    else {
      vv[0]=-cos(sc*mk_rad*(double)(ii-off));
      vv[1]=-sin(sc*mk_rad*(double)(ii-off));
    }
    mk_listappend(vertices,(void*)&vv);
  }
  return 0;

}

/* ########## */
int mk_polygonintersection(struct mk_list *poly1,struct mk_list *poly2,struct mk_list *pinter) {

  if (!poly1 || !poly2 || !pinter)
    return 0;
  int ii=0,jj=0,kk=0;
  double m1=.0,m2=.0,mdiff=.0,b1=.0,b2=.0,xinter=.0;
  mk_vertexnan(interp);
  mk_listclear(pinter,(void*)&interp);
  mk_vertexnan(vv1);
  mk_vertexnan(vv2);
  /* line equation :
   y=m*x+b
   m=(y2-y1)/(x2-x1)
   b=y1-m*x1
   condition : m1x+b1==m2x+b2 ->
   xintersect=(b2-b1)/(m1-m2) ; yintersect=(b2*m1-b1*m2)/(m1-m2) */
  for (ii=1;ii<poly1->count;ii++) {
    mk_listat(poly1,ii,(void*)&vv1);
    mk_listat(poly1,ii-1,(void*)&vv2);
    m1=(vv1[1]-vv2[1])/(vv1[0]-vv2[0]);
    b1=vv2[1]-m1*vv2[0];
    for (jj=1;jj<poly2->count;jj++) {
      mk_listat(poly2,ii,(void*)&vv1);
      mk_listat(poly2,ii-1,(void*)&vv2);
      m2=(vv1[1]-vv2[1])/(vv1[0]-vv2[0]);
      b2=vv2[1]-m1*vv2[0];
      mdiff=mk_diff(m1,m2);
      if (mdiff==.0)
        continue;
      xinter=(b2-b1)/mdiff;
      if (mk_isfinite(xinter)!=0 && xinter<=vv1[0] && vv2[0]<=xinter) {
        mk_listat(poly1,ii,(void*)&vv1);
        mk_listat(poly1,ii-1,(void*)&vv2); 
        if (xinter<=vv1[0] && vv2[0]<=xinter) { 
          interp[0]=xinter;
          interp[1]=(b2*m1-b1*m2)/mdiff;
          mk_listappend(pinter,(void*)&interp);
        }
      } 
    }
  }
  return pinter->count;

}

/* ########## */
int mk_derivatives(struct mk_list *vertices,struct mk_list *der1,struct mk_list *der2) {

  if (!vertices)
    return 1;
  mk_vertexnan(vv1);
  mk_vertexnan(vv2);
  if (vertices->count==1) {
    if (der1)
      mk_listappend(der1,(void*)&vv1);
    if (der2)
      mk_listappend(der2,(void*)&vv2);
    return 0;
  }
  mk_vertexnan(vvl);
  mk_vertexnan(vvc);
  mk_vertexnan(vvr);
  double df1=mk_dnan,df2=mk_dnan;
  int ii=0,idx=0;
  for (ii=0;ii<vertices->count;ii++) {
    idx=(ii==0 ? ii : ii-1);
    mk_listat(vertices,idx,(void*)&vvl);
    idx=(ii==vertices->count-1 ? ii-1 : ii);
    mk_listat(vertices,idx,(void*)&vvc);
    idx=(ii==vertices->count-1 ? ii : ii+1);
    mk_listat(vertices,idx,(void*)&vvr);
    if (mk_isnan(vvl[2])!=0 || mk_isnan(vvc[2])!=0 || mk_isnan(vvr[2])!=0)
      df1=df2=.0;
    else {
      df1=(vvr[2]-vvl[2]);
      df2=(vvr[2]-2.*vvc[2]+vvl[2]);
    }
    if (mk_isnan(vvl[0])==0 || mk_isnan(vvr[0])==0 || mk_diff(vvl[0],vvr[0])!=.0) {
      vv1[0]=df1/(vvr[0]-vvl[0]);
      vv2[0]=df2/((vvr[0]-vvl[0])*(vvr[0]-vvl[0]));
    }
    else
      vv1[0]=vv2[0]=mk_dnan;
    if (mk_isnan(vvl[1])==0 || mk_isnan(vvr[1])==0 || mk_diff(vvl[1],vvr[1])!=.0) {
      vv1[1]=df1/(vvr[1]-vvl[1]);
      vv2[1]=df2/((vvr[1]-vvl[1])*(vvr[1]-vvl[1]));
    }
    else
      vv1[1]=vv2[1]=mk_dnan;
    if (der1)
      mk_listsetat(der1,&vv1,ii,der1->count);
    if (der2)
      mk_listsetat(der2,&vv2,ii,der2->count);
  }
  return 0;

}

/* ########## */
int mk_derivativesmixed(struct mk_list *vertices,struct mk_list *der,int cols) {

  if (!vertices || !der || cols<2)
    return 1;
  mk_vertexnan(vv);
  mk_vertexnan(vv00);
  mk_vertexnan(vv01);
  mk_vertexnan(vv10);
  mk_vertexnan(vv11);
  int ii=0,jj=0,kk=0,row=0,col=0,idx=0,rows=vertices->count/cols;
  for (ii=0;ii<rows;ii++) {
    for (jj=0;jj<cols;jj++) {
      row=(ii==0 ? 0 : ii-1);
      col=(jj==0 ? 0 : jj-1);
      idx=row*cols+col;
      mk_listat(vertices,idx,vv00);
      col=(jj==cols-1 ? cols-1 : jj+1);
      idx=row*cols+col;
      mk_listat(vertices,idx,vv10);
      row=(ii==rows-1 ? rows-1 : ii+1);
      idx=row*cols+col;
      mk_listat(vertices,idx,vv11);
      col=(jj==0 ? 0 : jj-1);
      idx=row*cols+col;
      mk_listat(vertices,idx,vv01);
      mk_listat(der,kk,vv);
      if (mk_isnan(vv11[2])==0 && mk_isnan(vv01[2])==0 && mk_isnan(vv10[2])==0 && 
          mk_isnan(vv00[2])==0 && mk_isnan(vv10[0])==0 && mk_isnan(vv00[0])==0 && 
          mk_isnan(vv01[1])==0 && mk_isnan(vv11[1])==0) {
        vv[2]=(vv11[2]-vv01[2]-vv10[2]+vv00[2])/(4.*(vv10[0]-vv00[0])*(vv11[1]-vv01[1])); 
        mk_listsetat(der,&vv,kk,der->count);
      }
      kk++;     
    } 
  }
  return 0;

}

/* ########## */
int mk_matrixalloc(struct mk_matrix *mat,int rows_,int cols_) {

  mat->rows=MAX(rows_,0);
  mat->cols=MAX(cols_,0);
  mat->matrix=0;
  int sz=mat->rows*mat->cols;
  if (sz==0)
    mat->rows=mat->cols=0;
  else {
    mat->matrix=(double *)malloc(sz*sizeof(double));
    memset(&mat->matrix[0],0,sz*sizeof(double));
  }
  return sz; 

}

/* ########## */
int mk_matrixfree(struct mk_matrix *mat) {

  if (mat && mat->matrix)
    free(mat->matrix);
  mat->matrix=0;
  mat->rows=mat->cols=0;
  return 0;

}

/* ########## */
double mk_matrixget(const struct mk_matrix *mat,int row,int col) {

  if (mat && mat->matrix && row>=0 && row<mat->rows && col>=0 && col<mat->cols)
    return *(mat->matrix+(row*mat->cols+col));
  return mk_dnan;

}

/* ########## */
int mk_matrixset(struct mk_matrix *mat,int row,int col,double val) {

  int res=1;
  if (mat && mat->matrix && row>=0 && row<mat->rows && col>=0 && col<mat->cols) {
    *(mat->matrix+(row*mat->cols+col))=val;
    res=0;
  } 
  return res;

}

/* ########## */
int mk_matrixcopy(struct mk_matrix *dest,const struct mk_matrix *src) {

  if (!dest)
    return 1;
  int sz=(src ? src->rows*src->cols : 0);
  if (sz==0) {
    mk_matrixfree(dest);
    return 0;
  }
  if (sz!=dest->rows*dest->cols) {
    mk_matrixfree(dest);
    mk_matrixalloc(dest,src->rows,src->cols);
  }
  memcpy((void*)dest->matrix,(void*)src->matrix,sz*sizeof(double));
  return sz;

}

/* ########## */
int mk_matrixreset(struct mk_matrix *mat,int identity) {

  if (!mat || mat->rows*mat->cols==0)
    return 1;
  memset((void*)mat->matrix,0,mat->rows*mat->cols*sizeof(double));
  if (identity==0)
    return 0;
  int ii=0,jj=0;
  for (ii=0;ii<mat->rows;ii++) {
    for (jj=0;jj<mat->cols;jj++) {
      if (ii==jj)
        mk_matrixset(mat,ii,jj,1.);
    }
  }
  return 0;  

}

/* ########## */
int mk_matrixisidentity(struct mk_matrix *mat,double df) {

  if (!mat || mat->rows!=mat->cols)
    return 0;
  int ii=0,jj=0;
  double value=mk_dnan;
  for (ii=0;ii<mat->rows;ii++) {
    for (jj=0;jj<mat->cols;jj++) {
      value=mk_matrixget(mat,ii,jj);
      if ((ii==jj && mk_diff(value,1.,df)!=.0) || (ii!=jj && mk_diff(value,.0,df)!=0))
        return 0;
    }
  }
  return 1;

}

/* ########## */
int mk_matrixtranspose(struct mk_matrix *trmat) {

  if (!trmat || !trmat->matrix || trmat->rows*trmat->cols==0)
    return 1;
  int row=0,col=0,sz=trmat->rows*trmat->cols;
  struct mk_matrix mat;
  mk_matrixalloc(&mat,trmat->rows,trmat->cols);
  mk_matrixcopy(&mat,trmat);
  mk_swapi(&trmat->rows,&trmat->cols);
  for (row=0;row<trmat->rows;row++) {
    for (col=0;col<trmat->cols;col++) {
      *(trmat->matrix+(row*trmat->cols+col))=*(mat.matrix+(col*mat.cols+row));
    }
  }
  mk_matrixfree(&mat);
  return 0;

}

/* ########## */
int mk_matrixmult(
  const struct mk_matrix *mat1,const struct mk_matrix *mat2,struct mk_matrix *matres) {

  int ii=0,jj=0,kk=0,r1=(mat1 ? mat1->rows: 0),c1=(mat1 ? mat1->cols : 0),
      r2=(mat2 ? mat2->rows: 0),c2=(mat2 ? mat2->cols : 0);
  if (!matres || r1*c1==0 || r2*c2==0 || c1!=r2)
    return 1;
  if (matres->rows!=r1 || matres->cols!=c2) {
    mk_matrixfree(matres);
    mk_matrixalloc(matres,r1,c2);
  }  
  mk_matrixreset(matres,0);
  double tmp=.0;
  for (ii=0;ii<r1;ii++) {
    for (jj=0;jj<c2;jj++) {
      for (kk=0;kk<c1;kk++) {
        tmp=mk_matrixget(matres,ii,jj);
        mk_matrixset(matres,ii,jj,tmp+mk_matrixget(mat1,ii,kk)*mk_matrixget(mat2,kk,jj));
      }
    }
  }
  return 0;

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
int mk_matrixludecomposition(
  struct mk_matrix *mat,struct mk_matrix *lumat,int *rowperm,double *parity) {

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
      mk_matrixset(lumat,ii,jj,mk_matrixget(mat,ii,jj));
      tmp=fabs(mk_matrixget(lumat,ii,jj));
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
      for (kk=0;kk<ii;kk++) {
        tmp=mk_matrixget(lumat,ii,jj)-mk_matrixget(lumat,ii,kk)*mk_matrixget(lumat,kk,jj);
        mk_matrixset(lumat,ii,jj,tmp);
      }    
    }
    maxcoeff=.0;
    /* loop rows for 'l'ower triangular matrix
     and diagonal (denominators for lower matrix elements) inclusive */
    for (ii=jj;ii<num;ii++) {
      /* do the matrix multiplication
       beta[j,j]*alpha[i,j]=m[i,j]-sum(alpha[i,k]*beta[k,j]) */
      for (kk=0;kk<jj;kk++) {
        tmp=mk_matrixget(lumat,ii,jj)-mk_matrixget(lumat,ii,kk)*mk_matrixget(lumat,kk,jj);
        mk_matrixset(lumat,ii,jj,tmp);
      }
      /* beta[j,j] is to calculate as pivot(largest element)
       from this row=i and (precalculated) =rowscale[i] */
      tmp=fabs(mk_matrixget(lumat,ii,jj))/rowscale[ii];
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
      for (ii=0;ii<num;ii++) {
        tmp=mk_matrixget(lumat,imax,ii);
        mk_matrixset(lumat,imax,ii,mk_matrixget(lumat,jj,ii));
        mk_matrixset(lumat,jj,ii,tmp);
      }
      rowscale[imax]=rowscale[jj]; /* rowscale[j] is not needed anymore */
      mk_swapi(&rowperm[jj],&rowperm[imax]);
      *parity=-(*parity);
    }
    if (mk_deq(mk_matrixget(lumat,jj,jj),.0))
      return -1;
    /* finallly (for this column) divide all lower row elements by the pivot */
    if (jj<(num-1)) {
      for (ii=(jj+1);ii<num;ii++) {
        tmp=mk_matrixget(lumat,ii,jj)/mk_matrixget(lumat,jj,jj);
        mk_matrixset(lumat,ii,jj,tmp);
      }
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
      xx[ii]-=mk_matrixget(lumat,ii,jj)*xx[jj];
  }
  /* now do the backsubstitution by solving for the upper triangular matrix (beta)
   e.g. for (rows=cols=3) lum(upperpart)={lum[1,1],lum[1,2],lum[1,3],
   lum[2,1]=0,lum[2,2],lum[2,3],lum[3,1]=0,lum[3,2]=0,lum[3,3]} as determined
   in --> ludecomposition
   (since pivot is not ==1 here we have to do the dividing) */
  double tmp=.0;
  for (ii=(num-1);ii>-1;ii--) {
    tmp=xx[ii];
    for (jj=(ii+1);jj<num;jj++)
      tmp-=mk_matrixget(lumat,ii,jj)*xx[jj];
    xx[ii]=tmp/mk_matrixget(lumat,ii,ii);
  }
  return 0;

}

/* ########## */
double mk_matrixdet(struct mk_matrix *dmat) {
 
  int ii=0,rr=(dmat ? dmat->rows : 0),cc=(dmat ? dmat->cols : 0);
  if (rr*cc==0 || rr!=cc)
    return 1;
  struct mk_matrix lumat;
  mk_matrixalloc(&lumat,rr,cc);
  int *rowperm=(int*)malloc(rr*sizeof(int));
  double parity=1.;
  if (mk_matrixludecomposition(dmat,&lumat,rowperm,&parity)!=0)
    return .0;
  double det=parity;
  for (ii=0;ii<dmat->rows;ii++)
    det*=mk_matrixget(&lumat,ii,ii);
  free(rowperm);
  mk_matrixfree(&lumat);
  return det;

}

/* ########## */
int mk_matrixinvert(struct mk_matrix *imat) {

  int ii=0,jj=0,len=imat->rows;
  struct mk_matrix lumat;
  mk_matrixalloc(&lumat,len,len);
  int *rowperm=(int*)malloc(len*sizeof(int));
  for (ii=0;ii<len;ii++)
    rowperm[ii]=ii;
  double *identity=(double*)malloc(len*sizeof(double));
  double *inverted=(double*)malloc(len*sizeof(double));
  double parity=1.;
  int res=mk_matrixludecomposition(imat,&lumat,rowperm,&parity);
  for (ii=0;ii<len && res==0;ii++) {
    for (jj=0;jj<len;jj++)
      identity[jj]=inverted[jj]=(ii==jj ? 1. : .0);
    // solve column by column the equation M*M(-1)=1
    if (mk_matrixlubacksubstitution(&lumat,rowperm,identity,inverted)==0) {
      // set the new elements ...
      for (jj=0;jj<len;jj++)
        mk_matrixset(imat,jj,ii,inverted[jj]);
    }
  }
  free(inverted);
  free(identity);
  free(rowperm);
  mk_matrixfree(&lumat);
  return res;

}

/* ########## */
int mk_matrixsolve(struct mk_matrix *mat,double *rr,double *xx) {

  if (!mat || !rr || !xx || mat->rows<=0 || mat->rows!=mat->cols)
    return 1;
  int num=mat->rows;
  struct mk_matrix lumat;
  mk_matrixalloc(&lumat,num,num);
  int *rowperm=(int*)malloc(num*sizeof(int));
  memset(&rowperm[0],0,num*sizeof(int));
  double parity=1.;
  int res=mk_matrixludecomposition(mat,&lumat,&rowperm[0],&parity);
  if (res==0)
    res=mk_matrixlubacksubstitution(&lumat,&rowperm[0],rr,xx);
  free(rowperm);
  mk_matrixfree(&lumat);
  return res;

}

/* ########## */
int mk_matrixdbg(struct mk_matrix *mat,mk_string str) {

  char buf[16];
  memset(&buf[0],0,16);
  int ii=0,jj=0;
  for (ii=0;ii<mat->rows;ii++) {
    for (jj=0;jj<mat->cols;jj++) {
      sprintf(&buf[0],"%+1.8e",mk_matrixget(mat,ii,jj));
      mk_stringappend(str,&buf[0]);
      if (jj>0 && (jj%4)==0)
        mk_stringappend(str,"\n");
      else
        mk_stringappend(str,"  ");
      memset(&buf[0],0,16);
    }
    mk_stringappend(str,"\n");
  }
  return 0;

}


