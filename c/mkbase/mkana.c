
#include <mkbase/defs.h>
#include <mkbase/mkana.h>
#include <mkbase/mkmath.h>
#include <ctype.h>

/* ########## */
int mk_polynomialalloc(struct mk_polynomial *polynomial,int len) {

  len=MAX(0,len);
  mk_listalloc(&polynomial->ctrlL,sizeof(mk_vertex),len);
  mk_vertexzero(vv);
  int ii=0;
  for (ii=0;ii<len;ii++)
    mk_listappend(&polynomial->ctrlL,(void*)&vv);
  mk_matrixalloc(&polynomial->cc,len,len);
  mk_matrixalloc(&polynomial->dd,len,len);
  return 0;

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
double mk_polynomialinterp(struct mk_polynomial *polynomial,double xx,double *coeffL) {

  if (!polynomial || mk_isnan(xx))
    return mk_dnan;
  int ii=0,jj=0,idx=0,downward=0,len=polynomial->ctrlL.count;
  mk_vertexzero(vv0);
  mk_vertexzero(vv1);
  mk_listat(&polynomial->ctrlL,0,(void*)&vv0);
  double res=.0,tmp=.0,dxmin=fabs(vv0[0]-xx),dxl=.0,dxh=.0,dxincr=.0;
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
    if (fabs(vv0[0]-xx)<dxmin) {
      dxmin=fabs(vv0[0]-xx);
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
      dxl=vv0[0]-xx;
      dxh=vv1[0]-xx;
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
  res=mk_matrixget(&polynomial->cc,0,idx);
  if (idx==0 || len/idx>=2)
    downward=1;
  for (ii=1;ii<len;ii++) {
    if (downward==1) {
      if (idx>=1)
        downward=0;
      tmp=mk_matrixget(&polynomial->cc,ii,idx);
    }
    else {
      if (idx<1 || idx<(len-ii-1))
        downward=1;
      tmp=mk_matrixget(&polynomial->dd,ii,--idx);
    }
    res+=tmp;
  }
  return res;

}

/* ########## */
int mk_polynomialcoeff(struct mk_polynomial *polynomial,double *coeff) {

  if (!polynomial || !coeff)
    return 1;
  int ii=0,idx=0,jj=0,kk=0,ll=0,len=polynomial->ctrlL.count;
  double min=mk_dnan,zcoeff=.0;
  mk_vertexzero(vv0);
  struct mk_polynomial pp;
  mk_polynomialalloc(&pp,len);
  mk_listcopy(&pp.ctrlL,&polynomial->ctrlL);
  for (ii=0;ii<len;ii++) {
    zcoeff=mk_polynomialinterp(&pp,.0,0);
    coeff[idx++]=zcoeff;
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
      vv0[1]=(vv0[1]-zcoeff)/vv0[0];
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
int mk_polynomialfitdegr(struct mk_polynomial *polynomial,int degree,double *coeffL) {

  int ii=0,jj=0,kk=0,len=(polynomial ? polynomial->ctrlL.count : 0);
  if (degree<1 || len<=degree || !coeffL)
    return 1;
  memset((void*)coeffL,0,(degree+2)*sizeof(double));
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

  return 0;

}
