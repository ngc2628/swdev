
#include <mkbase/defs.h>
#include <mkbase/mkana.h>
#include <mkbase/mkmath.h>
#include <ctype.h>

/* ########## */
int mk_polynomialalloc(struct mk_polynomial *polynomial,int len) {
 
  if (!polynomial)
    return 0;
  len=MAX(0,len);
  mk_listalloc(&polynomial->ctrlL,sizeof(mk_vertex),len);
  mk_vertexzero(vv);
  int ii=0;
  for (ii=0;ii<len;ii++)
    mk_listappend(&polynomial->ctrlL,(void*)&vv);
  mk_matrixalloc(&polynomial->cc,0,0);
  mk_matrixalloc(&polynomial->dd,0,0);
  return len;

}

/* ########## */
int mk_polynomialfree(struct mk_polynomial *polynomial) {

  if (!polynomial)
    return 1;
  mk_matrixfree(&polynomial->dd);
  mk_matrixfree(&polynomial->cc);
  mk_listfree(&polynomial->ctrlL);
  return 0;

}

/* ########## */
int mk_polynomialinterpol(struct mk_polynomial *polynomial,mk_vertex vertex) {

  int ii=0,jj=0,idx=0,downward=0,len=(polynomial ? polynomial->ctrlL.count : 0);
  if (len==0 || mk_isnan(vertex[0])) {
    vertex[1]=mk_dnan;
    return 1;
  }
  if (polynomial->cc.rows==0)
    mk_matrixalloc(&polynomial->cc,len,len);
  if (polynomial->dd.rows==0)
    mk_matrixalloc(&polynomial->dd,len,len);
  mk_vertexzero(vv0);
  mk_vertexzero(vv1);
  mk_listat(&polynomial->ctrlL,0,(void*)&vv0);
  double res=.0,tmp=.0,dxmin=fabs(vv0[0]-vertex[0]),dxl=.0,dxh=.0,dxincr=.0;
  /* first columns of cc,dd are the ctrlL function values */
  for (ii=0;ii<len;ii++) {
    mk_listat(&polynomial->ctrlL,ii,(void*)&vv0);
    mk_matrixset(&polynomial->cc,0,ii,vv0[1]);
    mk_matrixset(&polynomial->dd,0,ii,vv0[1]);
  }
  /* update cc,dd */
  for (ii=1;ii<len;ii++) {
    /* calc nearest distance to (then) starting point in ctrlL underway */
    mk_listat(&polynomial->ctrlL,ii,(void*)&vv0);
    if (fabs(vv0[0]-vertex[0])<dxmin) {
      dxmin=fabs(vv0[0]-vertex[0]);
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
    for (jj=0;jj<(len-ii);jj++) {
      mk_listat(&polynomial->ctrlL,jj,(void*)&vv0);
      mk_listat(&polynomial->ctrlL,jj+ii,(void*)&vv1);
      dxl=vv0[0]-vertex[0];
      dxh=vv1[0]-vertex[0];
      dxincr=mk_matrixget(&polynomial->cc,ii-1,jj+1)-mk_matrixget(&polynomial->dd,ii-1,jj);
      dxincr=(mk_deq(dxh-dxl,.0) ? .0 : dxincr/(dxl-dxh));
      mk_matrixset(&polynomial->cc,ii,jj,dxincr*dxl);
      mk_matrixset(&polynomial->dd,ii,jj,dxincr*dxh);
    }
  }
  /*
  accumulate result from diffs c,d (s.a) - walk along any of these ways
  p(x)=p0+c10+c20 down-down
  p(x)=p1+d10+c20 up-down | p1+c11+d20 down-up | p1+c11+c21 down-down
  p(x)=p2+d11+c20 up-up | p2+d11+c21 up-down | p2+c12+d21 down-up
  way up-or-down to nearest difference at resp index position
  */
  vertex[1]=mk_matrixget(&polynomial->cc,0,idx);
  if (idx==0 || len/idx>=2)
    downward=1;
  for (ii=1;ii<len;ii++) {
    if (downward==1) {
      if (idx>=1)
        downward=0;
      tmp=mk_matrixget(&polynomial->cc,ii,idx);
    }
    else {
      tmp=mk_matrixget(&polynomial->dd,ii,--idx);
      if (idx<1 || idx<(len-ii-1))
        downward=1;
    }
    vertex[1]+=tmp;
  }
  return 0;

}

/* ########## */
int mk_polynomialcoeff(struct mk_polynomial *polynomial,double *coeffL) {

  int ii=0,idx=0,jj=0,kk=0,ncoeff=0,len=(polynomial ? polynomial->ctrlL.count : 0);
  if (len==0 || !coeffL)
    return 1;
  double min=mk_dnan;
  mk_vertexzero(vv0);
  mk_vertexzero(zcoeff);
  struct mk_polynomial pp;
  mk_polynomialalloc(&pp,len);
  mk_listcopy(&pp.ctrlL,&polynomial->ctrlL);
  for (ii=0;ii<len;ii++) {
    mk_polynomialinterpol(&pp,zcoeff);
    coeffL[ncoeff++]=zcoeff[1];
    min=mk_dnan;
    kk=0;
    for (jj=0;jj<len-ii;jj++) {
      mk_listat(&pp.ctrlL,jj,(void*)&vv0);
      if (mk_isnan(min)!=0 || fabs(vv0[0])<min) {
        min=fabs(vv0[0]);
        kk=jj;
      }
      /* p(x)=a0+a1*x+a2*x*x+a3*x*x*x+a4*x*x*x*x .....
      ===> (p(x)-a0)/x=a1+a2*x+a3*x*x+a4*x*x*x ... */
      vv0[1]=(vv0[1]-zcoeff[1])/vv0[0];
      mk_listsetat(&pp.ctrlL,(void*)&vv0,jj,0);
    }
    /* recycle pp with shifted down elements and reinitialize cc,dd */
    for (jj=kk+1;jj<len-ii;jj++) {
      mk_listat(&pp.ctrlL,jj,(void*)&vv0);
      mk_listsetat(&pp.ctrlL,(void*)&vv0,jj-1,0);
    }
    mk_listpop(&pp.ctrlL,(void*)&vv0);
    jj=pp.ctrlL.count;
    mk_matrixreset(&pp.cc,0);
    mk_matrixreset(&pp.dd,0);
    for (kk=0;kk<pp.ctrlL.count;kk++) {
      mk_listat(&pp.ctrlL,kk,(void*)&vv0);
      mk_matrixset(&pp.cc,0,kk,vv0[1]);
      mk_matrixset(&pp.dd,0,kk,vv0[1]);
    }
  }
  mk_polynomialfree(&pp);
  return 0;

}

/* ########## */
int mk_polynomialeval(int len,double *coeffL,mk_vertex vertex) {

  if (len==0 || !coeffL || mk_isnan(vertex[0])) {
    vertex[1]=mk_dnan;
    return 1;
  }
  vertex[1]=.0;
  while (--len>-1)
    vertex[1]=vertex[0]*vertex[1]+coeffL[len];
  return 0;

}

/* ########## */
int mk_polynomialfitdegr(struct mk_polynomial *polynomial,int degree,double *coeffL,double *cod) {

  if (coeffL)
    memset(coeffL,0,(degree+1)*sizeof(double));
  if (cod)
    *cod=.0;
  int ii=0,jj=0,kk=0,len=(polynomial ? polynomial->ctrlL.count : 0);
  if (degree<1 || len<=degree || !coeffL)
    return 1;
  mk_string strdbg;
  double tmp=mk_dnan;
  mk_vertexnan(vv);
  struct mk_matrix xxpow;
  mk_matrixalloc(&xxpow,len,2*(degree+1));
  for (ii=0;ii<len;ii++) {
    mk_matrixset(&xxpow,ii,0,1.);
    mk_listat(&polynomial->ctrlL,ii,(void*)&vv);
    for (jj=1;jj<2*(degree+1);jj++)
      mk_matrixset(&xxpow,ii,jj,mk_matrixget(&xxpow,ii,jj-1)*vv[0]);
  }
  double *rhs=(double*)malloc((degree+1)*sizeof(double));
  memset(rhs,0,(degree+1)*sizeof(double));
  for (ii=0;ii<=degree;ii++) {
    for (jj=0;jj<len;jj++) {
      mk_listat(&polynomial->ctrlL,jj,(void*)&vv);
      rhs[ii]+=mk_matrixget(&xxpow,jj,ii)*vv[1];
    }
  }
  struct mk_matrix mat;
  mk_matrixalloc(&mat,degree+1,degree+1);
  for (ii=0;ii<=degree;ii++) {
    for (jj=0;jj<=degree;jj++) {
      if (ii==jj)
        mk_matrixset(&mat,ii,jj,.0);
      for (kk=0;kk<len;kk++) {
        tmp=mk_matrixget(&mat,ii,jj)+mk_matrixget(&xxpow,kk,ii+jj);
        mk_matrixset(&mat,ii,jj,tmp);
      }
    }
  }
  mk_matrixfree(&xxpow);
  mk_matrixsolve(&mat,rhs,coeffL);
  free(rhs);
  mk_matrixfree(&mat);
  if (!cod)
    return 0;
  /* coefficient of determination : 
    stot=sum((yi-ymean)*(yi-ymean))
    serr=sum((yi-interp(xi))*(yi-interp(xi)))
    cod=sqrt((stot-serr)/stot)
  */
  double ymean=.0,dym=.0,dyf=.0;
  for (ii=0;ii<len;ii++) {
    mk_listat(&polynomial->ctrlL,ii,(void*)&vv);
    ymean+=vv[1];
  }
  ymean/=(double)len;
  for (ii=0;ii<len;ii++) {
    mk_listat(&polynomial->ctrlL,ii,(void*)&vv);
    tmp=vv[1]-ymean;
    dym+=(tmp*tmp);
    tmp=vv[1];
    mk_polynomialeval(degree+1,coeffL,vv);
    tmp-=vv[1];
    dyf+=(tmp*tmp);
  }
  *cod=sqrt((dym-dyf)/dym);
  return 0;

}

/* ########## */
int mk_polynomialrootscrude(int ncoeff,double *coeffL,double left,double right,double *roots) {

  ncoeff=(coeffL ? MAX(ncoeff,0) : 0);
  if (ncoeff>0)
    memset((void*)roots,0,ncoeff*sizeof(double));
  int ii=0,degree=0;
  for (ii=ncoeff-1;ii>0;ii--)  {
    if (mk_isnan(coeffL[ii])==0 && coeffL[ii]!=.0) {
      degree=ii;
      break;
    }
  }
  if (degree==0) {
    if (mk_isnan(coeffL[ii])==0 && coeffL[0]==.0) /* all zero flat line */
      degree=1; 
  }
  else if (degree==1)
    roots[0]=-coeffL[0]/coeffL[1];
  else if (degree==2) {
    double pp2=-coeffL[1]/(2.*coeffL[2]),pqres=pp2*pp2-coeffL[0]/coeffL[2];
    if (pqres<.0)
      degree=0;            
    pqres=sqrt(pqres);
    roots[0]=pp2-pqres;
    roots[1]=pp2+pqres;
  }
  else if (degree==3) {
    // cardano goes here
  }
  if (degree<3)
    return degree;

  double res=.0,lastres=.0,minlres=mk_dnan,minhres=mk_dnan,minleval=mk_dnan,minheval=mk_dnan,
         intv=(right-left)/1000.,eval=left,lasteval=eval;
  int nroots=0;
  mk_vertexzero(vv);
  for (ii=0;ii<1000;ii++) {
    eval=left+(double)ii*intv;
    vv[0]=eval;
    mk_polynomialeval(degree+1,coeffL,vv);
    res=vv[1];
    if (res==.0 || (res>.0 && lastres<.0) || (lastres>.0 && res<.0)) {
      roots[nroots++]=(res==.0 ? eval : eval/2.+lasteval/2.);
      if (nroots==degree)
        break;
    }
    if (res<.0 && (mk_isbusted(minlres)!=0 || res>minlres)) {
      minlres=res;
      minleval=eval;
    }
    else if (res>.0 && (mk_isbusted(minhres)!=0 || res<minhres)) {
      minhres=res;
      minheval=eval;
    }
    lasteval=eval;
    lastres=res;
  }
  if (nroots==0) {
    if (mk_isbusted(minlres)==0) {
      if (mk_isbusted(minhres)==0)
        roots[nroots++]=(minhres<fabs(minlres) ? minheval : minleval);
      else
        roots[nroots++]=minleval;
    }
    else if (mk_isbusted(minhres)==0)
      roots[nroots++]=minheval;
  }
  mk_heapsort(nroots,sizeof(double),(void*)roots,mk_cmpdouble);
  return nroots;

}

/*
  inout spline* , in spline-options , return 0,1
*/
static int mk_cubicsplineder1st(struct mk_spline *spline) {

  if (!spline)
    return 1;
  if (spline->derL)
    free(spline->derL);
  spline->derL=0;
  int ii=0,nctrl=spline->ctrlL.count,num=nctrl,pennum=num-1;
  if (nctrl<=1)
    return 1;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  double *hx=(double*)malloc(pennum*sizeof(double));
  double *hy=(double*)malloc(pennum*sizeof(double));
  double *ss=(double*)malloc(pennum*sizeof(double));
  for (ii=0;ii<pennum;ii++) {
    mk_listat(&spline->ctrlL,ii,(void*)&vvl);
    mk_listat(&spline->ctrlL,ii+1,(void*)&vvh);
    hx[ii]=vvh[0]-vvl[0];
    hy[ii]=vvh[1]-vvl[1];
    if (hx[ii]==.0) {
      free(hx);
      free(hy);
      free(ss);
      return 1;
    }
    ss[ii]=hy[ii]/hx[ii];
  }
  struct mk_matrix mat;
  mk_matrixalloc(&mat,num,num);
  double *rr=(double*)malloc(num*sizeof(double));
  rr[0]=rr[pennum]=.0;
  for (ii=1;ii<pennum;ii++)
    rr[ii]=3.*hx[ii-1]*ss[ii]+3.*hx[ii]*ss[ii-1];
  if ((spline->options&mk_splineboundary_notaknot)>0 && num>3) {
    rr[0]=ss[0]*hx[1]*(2.*hx[1]+3.*hx[0])+ss[1]*hx[0]*hx[0];
    rr[pennum]=-ss[num-3]*hx[num-2]*hx[num-2]-ss[num-2]*hx[num-3]*(3.*hx[num-2]+2.*hx[num-3]);
    double tmp=hx[0]+hx[1];
    mk_matrixset(&mat,0,0,hx[1]*tmp);
    mk_matrixset(&mat,0,1,tmp*tmp);
    tmp=hx[num-2]+hx[num-3];
    mk_matrixset(&mat,pennum,pennum-1,-tmp*tmp);
    mk_matrixset(&mat,pennum,pennum,-hx[num-3]*tmp);
  }
  else {
    if ((spline->options&mk_splineboundary_der1st)>0) { // try der1st
      rr[0]=(nctrl>2 ? .0 : ss[0]);
      rr[pennum]=(nctrl>2 ? .0 : ss[0]);
      mk_matrixset(&mat,0,0,1.);
      mk_matrixset(&mat,pennum,pennum,1.);
    }
    else { // =natural spline (splineoptions&splineboundary_natural)>0
      rr[0]=.0;
      rr[pennum]=.0;
      mk_matrixset(&mat,0,0,1.);
      mk_matrixset(&mat,pennum,pennum,1.);
    }
  }
  for (ii=1;ii<pennum;ii++) {
    mk_matrixset(&mat,ii,ii-1,hx[ii]);
    mk_matrixset(&mat,ii,ii,2.*(hx[ii-1]+hx[ii]));
    mk_matrixset(&mat,ii,ii+1,hx[ii-1]);
  }
  spline->derL=(double*)malloc(num*sizeof(double));
  memset(&spline->derL[0],0,num*sizeof(double));
  int solved=mk_matrixsolve(&mat,rr,spline->derL);
  free(hy);
  free(rr);
  mk_matrixfree(&mat);
  if (solved!=0) {
    free(spline->derL);
    spline->derL=0;
    free(hx);
    free(ss);
    return 1;
  }
  if (nctrl>2 && (spline->options&mk_spline_monotonic)>0) {
    // apply hyman filter
    double corr=.0,pm=.0,pu=.0,pd=.0,MM=.0,deri=.0,fabsderi=.0;
    for (ii=0;ii<num;ii++) {
      corr=.0;
      deri=spline->derL[ii];
      fabsderi=fabs(deri);
      if (ii==0) {
        if (deri*ss[ii]>.0)
          corr=deri*MIN(fabsderi,fabs(3.*ss[ii]))/fabsderi;
      }
      else if (ii==pennum) {
        if (deri*ss[ii-1]>.0)
          corr=deri*MIN(fabsderi,fabs(3.*ss[ii-1]))/fabsderi;
      }
      else {
        pm=(ss[ii-1]*hx[ii]+ss[ii]*hx[ii-1])/(hx[ii-1]+hx[ii]);
        MM=3.*MIN(MIN(fabs(ss[ii-1]),fabs(ss[ii])),fabs(pm));
        if (ii>1) {
          if ((ss[ii-1]-ss[ii-2])*(ss[ii]-ss[ii-1])>.0) {
            pd=(ss[ii-1]*(2.*hx[ii-1]+hx[ii-2])-ss[ii-2]*hx[ii-1])/(hx[ii-2]+hx[ii-1]);
            if (pm*pd>.0 && pm*(ss[ii-1]-ss[ii-2])>.0)
              MM=MAX(MM,1.5*MIN(fabs(pm),fabs(pd)));
          }
        }
        if (ii<num-2) {
          if ((ss[ii]-ss[ii-1])*(ss[ii+1]-ss[ii])>.0) {
            pu=(ss[ii]*(2.*hx[ii]+hx[ii+1])-ss[ii+1]*hx[ii])/(hx[ii]+hx[ii+1]);
            if (pm*pu>.0 && -pm*(ss[ii]-ss[ii-1])>.0)
              MM=MAX(MM,1.5*MIN(fabs(pm),fabs(pu)));
          }
        }
        if (deri*pm>.0)
          corr=deri*MIN(fabsderi,MM)/fabsderi;
      }
      if (corr!=deri)
        spline->derL[ii]=corr;
   //printf ("i=%d deri=%f corr=%f\n",i,deri,corr);
    }
  }
  free(hx);
  free(ss);
  return solved;

}

/*
  inout spline* , in spline-options , return 0,1
*/
static int mk_cubicsplineder2nd(struct mk_spline *spline) {

  if (!spline)
    return 1;
  if (spline->derL)
    free(spline->derL);
  spline->derL=0;
  int ii=0,nctrl=spline->ctrlL.count,num=nctrl,pennum=num-1;
  if (nctrl<=1)
    return 1;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  double *hx=(double*)malloc(pennum*sizeof(double));
  memset(&hx[0],0,pennum*sizeof(double));
  double *hy=(double*)malloc(pennum*sizeof(double));
  memset(&hy[0],0,pennum*sizeof(double));
  for (ii=1;ii<num;ii++) {
    mk_listat(&spline->ctrlL,ii-1,(void*)&vvl);
    mk_listat(&spline->ctrlL,ii,(void*)&vvh);
    hx[ii]=vvh[0]-vvl[0];
    hy[ii]=vvh[1]-vvl[1];
    if (hx[ii]==.0) {
      free(hx);
      free(hy);
      return 1;
    }
  }
  struct mk_matrix mat;
  mk_matrixalloc(&mat,num,num);
  if ((spline->options&mk_splineboundary_notaknot)>0 && num>3) {
    mk_matrixset(&mat,0,0,hx[2]);
    mk_matrixset(&mat,0,1,-hx[1]-hx[2]);
    mk_matrixset(&mat,0,2,hx[1]);
    mk_matrixset(&mat,pennum,pennum-2,hx[pennum]);
    mk_matrixset(&mat,pennum,pennum-1,-hx[pennum]-hx[pennum-1]);
    mk_matrixset(&mat,pennum,pennum,hx[pennum-1]);
  }
  else if ((spline->options&mk_splineboundary_periodic)>0) { // not implemented -> natural spline
    mk_matrixset(&mat,0,0,1.);
    mk_matrixset(&mat,pennum,pennum,1.);
  }
  else if ((spline->options&mk_splineboundary_natural)>0) { // natural spline
    mk_matrixset(&mat,0,0,1.);
    mk_matrixset(&mat,pennum,pennum,1.);
  }
  else { // not implemented -> natural spline
    mk_matrixset(&mat,0,0,1.);
    mk_matrixset(&mat,pennum,pennum,1.);
  }
  for (ii=1;ii<pennum;ii++) {
    mk_matrixset(&mat,ii,ii-1,hx[ii]/6.0);   
    mk_matrixset(&mat,ii,ii,(hx[ii+1]+hx[ii])/3.0);
    mk_matrixset(&mat,ii,ii+1,hx[ii+1]/6.0);
  }
  double *rr=(double*)malloc(num*sizeof(double));
  rr[0]=rr[pennum]=.0;
  for (ii=1;ii<pennum;ii++)
    rr[ii]=hy[ii+1]/hx[ii+1]-hy[ii]/hx[ii];
  spline->derL=(double*)malloc(num*sizeof(double));
  memset(&spline->derL[0],0,num*sizeof(double));
  int solved=mk_matrixsolve(&mat,rr,spline->derL);
  free(hx);
  free(hy);
  free(rr);
  mk_matrixfree(&mat);
  if (solved!=0) {
    free(spline->derL);
    spline->derL=0; 
  }
  return solved;

}

/* ########## */
int mk_cubicsplinealloc(struct mk_spline *spline,int len) {

  if (!spline)
    return 0;
  len=MAX(0,len);
  mk_listalloc(&spline->ctrlL,sizeof(mk_vertex),len);
  spline->options=0;
  mk_vertexzero(vv);
  int ii=0;
  for (ii=0;ii<len;ii++)
    mk_listappend(&spline->ctrlL,(void*)&vv);
  spline->derL=0;
  return len;

}

/* ########## */
int mk_cubicsplinefree(struct mk_spline *spline) {

  if (!spline)
    return 1;
  if (spline->derL)
    free(spline->derL);
  spline->derL=0;
  spline->options=0;
  mk_listfree(&spline->ctrlL);
  return 0;

}

/* ########## */
int mk_cubicsplineder(struct mk_spline *spline) {

  int res=1;
  if ((spline->options&mk_spline_solve1st)>0)
    res=mk_cubicsplineder1st(spline);
  else if ((spline->options&mk_spline_solve2nd)>0)
    res=mk_cubicsplineder2nd(spline);
  return res;

}

/* ########## */
int mk_cubicsplinecoeff(double xx,struct mk_spline *spline,double *coeffL) {

  if (coeffL)
    memset(&coeffL[0],0,4*sizeof(double));
  int ii=0,nctrl=(spline ? spline->ctrlL.count : 0),idxh=-1,idxl=nctrl,res=1;
  if (mk_isbusted(xx)!=0 || nctrl==0 || !spline->derL || !coeffL)
    return res;
  mk_vertex vvl={xx,mk_dnan,mk_dnan,mk_dnan};
  res=mk_binsearchinterval((void*)&vvl,sizeof(mk_vertex),nctrl,
    spline->ctrlL.arr,mk_vertexcmpx,&idxl,&idxh,0);
  if (res>0 || idxh<idxl)
    return res;
  mk_vertexnan(vvh);
  mk_listat(&spline->ctrlL,idxl,(void*)&vvl);
  mk_listat(&spline->ctrlL,idxh,(void*)&vvh);
  double hx=vvh[0]-vvl[0],hy=vvh[1]-vvl[1],tmp=.0;
  if (hx==.0)
    return 1;
  if ((spline->options&mk_spline_solve1st)>0) {
    coeffL[0]=vvl[1];
    coeffL[1]=spline->derL[idxl];
    tmp=(spline->derL[idxh]+spline->derL[idxl]-2.*hy/hx)/(hx*hx);
    coeffL[2]=(hy/hx-spline->derL[idxl])/hx-hx*tmp;
    coeffL[3]=tmp;
  }
  else if ((spline->options&mk_spline_solve2nd)>0) {
    double dx=-hx,dx6=6.*dx;
    coeffL[0]=(6.*vvl[0]*vvh[1]+vvh[0]*
      (-6.*vvl[1]+vvl[0]*(-vvh[0]*(2.*spline->derL[idxl]+spline->derL[idxh])+
      vvl[0]*(spline->derL[idxl]+2.*spline->derL[idxh]))))/dx6;
    coeffL[1]=(6.*(vvl[1]-vvh[1])+
               2.*vvl[0]*vvh[0]*(spline->derL[idxl]-spline->derL[idxh])+
               vvh[0]*vvh[0]*(2.*spline->derL[idxl]+spline->derL[idxh])-
               vvl[0]*vvl[0]*(spline->derL[idxl]+2*spline->derL[idxh]))/dx6;
    coeffL[2]=(vvl[0]*spline->derL[idxh]-vvh[0]*spline->derL[idxl])/(2.*dx);
    coeffL[3]=(spline->derL[idxl]-spline->derL[idxh])/dx6;
  }
  return res;

}

/* ########## */
int mk_cubicsplineinterpol(struct mk_spline *spline,mk_vertex vv) {

  vv[1]=mk_dnan;
  if (mk_isnan(vv[0]) || !spline || !spline->derL)
    return 1;
  int nctrl=spline->ctrlL.count,idxh=-1,idxl=nctrl;
  mk_vertex vvv;
  mk_vertexcopy(vvv,vv);
  int res=mk_binsearchinterval((void*)&vvv,sizeof(mk_vertex),nctrl,spline->ctrlL.arr,
    mk_vertexcmpx,&idxl,&idxh,0);
  if (res!=0 || idxh<idxl)
    return 1;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  mk_listat(&spline->ctrlL,idxl,(void*)&vvl);
  mk_listat(&spline->ctrlL,idxh,(void*)&vvh);
  double vlx=vvl[0],vly=vvl[1],vhx=vvh[0],vhy=vvh[1];
  double hx=vhx-vlx,hy=vhy-vly;
  if(hx==.0) {
    return 1;
  }
  double vvres=.0,aa=.0,bb=.0;
  double derl=spline->derL[idxl],derh=spline->derL[idxh];
  if ((spline->options&mk_spline_solve1st)>0) {
    double eval=(vv[0]-vlx);
    aa=vly;
    bb=derl;
    double dd=(derh+derl-2.*hy/hx)/(hx*hx);
    double cc=(hy/hx-derl)/hx-hx*dd;
    vvres=aa+eval*(bb+(eval*(cc+eval*dd)));
  }
  else if ((spline->options&mk_spline_solve2nd)>0) {
    aa=(vhx-vv[0])/hx;
    bb=(vv[0]-vlx)/hx;
    vvres=aa*vly+bb*vhy+(aa*aa*aa-aa)*hx*hx*derl/6.0+(bb*bb*bb-bb)*hx*hx*derh/6.0;
  }
  vv[1]=vvres;
  return 0;
  
}

/* ########## */
int mk_cubicsplineextrapol(struct mk_spline *spline,mk_vertex vv) {

  vv[1]=mk_dnan;
  if (mk_isnan(vv[0]) || !spline || !spline->derL)
    return 1;
  int nctrl=spline->ctrlL.count;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  mk_listat(&spline->ctrlL,0,(void*)&vvl);
  mk_listat(&spline->ctrlL,nctrl-1,(void*)&vvh);
  if (vv[0]>=vvl[0] && vv[0]<=vvh[0])
    return mk_cubicsplineinterpol(spline,vv);
  int idxl=(vv[0]<vvl[0] ? 0 : nctrl-2),
      idxh=(vv[0]<vvl[0] ? 1 : nctrl-1);
  mk_listat(&spline->ctrlL,idxl,(void*)&vvl);
  mk_listat(&spline->ctrlL,idxh,(void*)&vvh);
  double vlx=vvl[0],vly=vvl[1],vhx=vvh[0],vhy=vvh[1];
  double hx=vhx-vlx,hy=vhy-vly;
  if (hx==.0) {
    return 1;
  }
  double c0=.0,c1=.0,c2=.0,c3=.0;
  double derl=spline->derL[idxl],derh=spline->derL[idxh];
  if ((spline->options&mk_spline_solve1st)>0) {
    c0=vly;
    c1=derl;
    c3=(derh+derl-2.*hy/hx)/(hx*hx);
    c2=(hy/hx-derl)/hx-hx*c3;
  }
  else if ((spline->options&mk_spline_solve2nd)>0) {
    double dx=-hx,dx6=6.*dx;
    c0=(6.*vlx*vhy+vhx*(-6.*vly+vlx*(-vhx*(2.*derl+derh)+vlx*(derl+2.*derh))))/dx6;
    c1=(6.*(vly-vhy)+2.*vlx*vhx*(derl-derh)+vhx*vhx*(2.*derl+derh)-vlx*vlx*(derl+2*derh))/dx6;
    c2=(vlx*derh-vhx*derl)/(2.*dx);
    c3=(derl-derh)/dx6;
  }
  double res=(c0+vv[0]*(c1+vv[0]*(c2+vv[0]*c3)));
  vv[1]=res;
  return 0;

}

/* ########## */
int mk_bezierinterpol(struct mk_list *ctrlL,int nint,struct mk_list *vint) {

  nint=(vint ? MAX(0,nint) : 0);
  int ii=0,jj=0,nctrl=(ctrlL ? ctrlL->count : 0),penctrl=nctrl-1,penint=nint-1;
  if (nctrl==0 || nint==0)
    return 1;
  mk_listclear(vint,0);
  mk_listrealloc(vint,nint);
  double t1=.0,t2=1.,bino=1.;
  double *fac1=(double*)malloc(nctrl*sizeof(double));
  double *fac2=(double*)malloc(nctrl*sizeof(double));
  for (ii=0;ii<nctrl;ii++)
    fac1[ii]=fac2[ii]=1.;
  mk_vertexzero(vv);
  mk_vertexzero(vctrl);
  for (ii=0;ii<nint;ii++) {
    vv[0]=vv[1]=.0;
    t1=(double)ii/(double)penint;
    t2=1.-t1;
    /* poly-degree=nctrl , accumulate bernstein polynomials */
    for (jj=1;jj<nctrl;jj++) {
      fac1[jj]=fac1[jj-1]*t1;
      fac2[penctrl-jj]=fac2[penctrl-jj+1]*t2;
    }
    for (jj=0;jj<nctrl;jj++) {
      bino=mk_binomialcoeff(penctrl,jj);
      mk_listat(ctrlL,jj,(void*)&vctrl);
      vv[0]=(vv[0]+vctrl[0]*bino*fac1[jj]*fac2[jj]);
      vv[1]=(vv[1]+vctrl[1]*bino*fac1[jj]*fac2[jj]);
    }
    for (jj=0;jj<nctrl;jj++)
      fac1[jj]=fac2[jj]=1.;
    mk_listappend(vint,(void*)&vv);
  }
  free(fac1);
  free(fac2);
  return 0;

}

/*
single patch, four points z(x0,y0),z(x1,y1),z(x2,y2),z(x3,y3) lower left counterclockwise
in : values, derivatives dz/dx, derivatives dz/dy, cross derivatives (dz/dx)*(dz/dy)

using unit square without loss of information and dissection 0<t,u<1 
t=(x-x(i))/(x(i+1)-x(i)) 
u=(y-y(j))/(y(j+1)-y(j)) 

reformulates result
z=sum(0,3)sum(0,3) cij*(t**i)*(u**j)
dz/dt=sum(0,3)sum(0,3) i*cij*(t**(i-1))*u
dz/du=sum(0,3)sum(0,3) j*cij*t*(u**(j-1))
d2z/dtdu=sum(0,3)sum(0,3) i*j*cij*(t**(i-1))*(u**(j-1))

evaluate the 4 functions at the 4 grid points leads to the linear equation 
system (let 0**0==1)  

z(0,0)=c00
z(1,0)=c00+c10+c20+c30
z(1,1)=c00+c10+c20+c30+c01+c11+c12+c13+c02+c12+c22+c23+c03+c13+c23+c33
z(0,1)=c00+c01+c02+c03
dz/dt(0,0)=c10
dz/dt(1,0)=c10+2*c20*3*c30
dz/dt(1,1)=c10+c11+c12+c13+2*c20+2*c21+2*c22+2*c23+3*c30+3*c31+3*c32+3*c33
dz/dt(0,1)=c10+c11+c12+c13
dz/du(0,0)=c01
dz/du(1,0)=c01+c11+c21+c31
dz/du(1,1)=c01+2*c02+3*c03+c11+2*c12+3*c13+c21+2*c22+3*c23+c31+2*c32+3*c33
dz/du(0,1)=c01+2*c02+3*c03
d2z/dtdu(0,0)=c11
d2z/dtdu(1,0)=c11+2*c21+3*c31
d2z/dtdu(1,1)=c11+2*c12+3*c13+2*c21+4*c22+6*c23+3*c31+6*c32+9*c33
d2z/dtdu(0,1)=c11+2*c12+3*c13

reorganize to a matrix equation 
static helper matrix multiplied by left hand side of equations yield cij 
values and derivatives are provided in x,y (not t,u) 
dz/dt=(dz/dx)*(x(i+1)-x(i))  
dz/du=(dz/du)*(y(i+1)-y(i))  
d2z/dtdu=(dz/dx)*(dz/dy)*(x(i+1)-x(i))*(y(i+1)-y(i))  

lefthand (from input) =
z(0,0)
z(1,0)
z(1,1)
z(0,1)
dz/dx(0,0)*(x1-x0)
dz/dx(1,0)*(x1-x0)
dz/dx(1,1)*(x1-x0)
dz/dx(0,1)*(x1-x0)
dz/dy(0,0)*(y1-y0)
dz/dy(1,0)*(y1-y0)
dz/dy(1,1)*(y1-y0)
dz/dy(0,1)*(y1-y0)
d2z/dxdy(0,0)*(x1-x0)*(y1-y0)
d2z/dxdy(1,0)*(x1-x0)*(y1-y0)
d2z/dxdy(1,1)*(x1-x0)*(y1-y0)
d2z/dxdy(0,1)*(x1-x0)*(y1-y0)

cij=(bicubic_helper**-1)*lefthand (helper matrix below is already inverted)

with cij known a surface point and derivatives are calculated straight forward 
from result equations and x0<x<x1, y0<y<y1 
derivatives are then used to plug into the calculation of the next adjescent patch
*/

static double mk_bicubic_helper[16][16]={
  {1. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
  {0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,1. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
  {-3.,0. ,0. ,3. ,0. ,0. ,0. ,0. ,-2.,0. ,0. ,-1.,0. ,0. ,0. ,0. },
  {2. ,0. ,0. ,-2.,0. ,0. ,0. ,0. ,1. ,0. ,0. ,1. ,0. ,0. ,0. ,0. },
  {0. ,0. ,0. ,0. ,1. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
  {0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,1. ,0. ,0. ,0. },
  {0. ,0. ,0. ,0. ,-3.,0. ,0. ,3. ,0. ,0. ,0. ,0. ,-2.,0. ,0. ,-1.},
  {0. ,0. ,0. ,0. ,2. ,0. ,0. ,-2.,0. ,0. ,0. ,0. ,1. ,0. ,0. ,1. },
  {-3.,3. ,0. ,0. ,-2.,-1.,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
  {0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,-3.,3. ,0. ,0. ,-2.,-1.,0. ,0. },
  {9. ,-9.,9. ,-9.,6. ,3. ,-3.,-6.,6. ,-6.,-3.,3. ,4. ,2. ,1. ,2. },
  {-6.,6. ,-6.,6. ,-4.,-2.,2. ,4. ,-3.,3. ,3. ,-3.,-2.,-1.,-1.,-2.},
  {2. ,-2.,0. ,0. ,1. ,1. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. },
  {0. ,0. ,0. ,0. ,0. ,0. ,0. ,0. ,2. ,-2.,0. ,0. ,1. ,1. ,0. ,0. },
  {-6.,6. ,-6.,6. ,-3.,-3.,3. ,3. ,-4.,4. ,2. ,-2.,-2.,-2.,-1.,-1.},
  {4. ,-4.,4. ,-4.,2. ,2. ,-2.,-2.,2. ,-2.,-2.,2. ,1. ,1. ,1. ,1. }
};


