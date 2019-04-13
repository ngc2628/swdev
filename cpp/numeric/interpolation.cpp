
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkmath.h>
#include <tools/misc.h>
#include <numeric/matrix.h>
#include <numeric/interpolation.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
//#include <sys/param.h>

namespace num {

int numsmoothIntermediates=500;

Interpolation *buildInterpolation(mk_ulreal options) {

  if ((options&interpolation_type)==interpolation_none)
    return 0;
  Interpolation *interpolation=0;
  if ((options&interpolation_const)>0)
    interpolation=new InterpolationConst(0);
  else if ((options&interpolation_linear)>0)
    interpolation=new InterpolationLinear(0);
  else if ((options&interpolation_cubicspline)>0) {
    if ((options&interpolation_parametric)==0)
      interpolation=new CubicSpline(options);
    else
      interpolation=new CubicSplineP();
  }
  else if ((options&interpolation_polynomial)>0)  
    interpolation=new Polynomial(options);
  else if ((options&interpolation_bezier)>0)
    interpolation=new Bezier(0);
  else if ((options&interpolation_bicubic)>0)    
    ;
  interpolation->setOptions(options);
  return interpolation;

}

int numInterpolIntermediates(Interpolation *interpolation) {

  mk_ulreal options=(interpolation ? interpolation->options() : interpolation_none);
  if ((options&interpolation_type)==interpolation_none)
    return 0;
  if ((options&interpolation_const)>0)
    return 2*interpolation->nctrl()-1;
  if ((options&interpolation_linear)>0)
    return interpolation->nctrl();
  return numsmoothIntermediates;
  
}

int interpolation2string(mk_ulreal option,mk_string res) {

  mk_stringset(res,"none");
  if (option==interpolation_none)
    return 0;
  mk_ulreal ii=0,jj=0;
  for (ii=1,jj=1;jj<(numinterpolationtypes+numinerpolationoptions);ii*=2,jj++) {
    if ((option&ii)>0) {
      mk_stringset(res,(jj<numinterpolationtypes ? interpolationtypes[jj] : interpolationoptions[jj]));
      break;
    }
  }
  return 0;

}

Interpolation::Interpolation(mk_ulreal options) : 
  m_options(options),m_ready(0) {

  mk_listalloc(&m_ctrlL,sizeof(mk_vertex),0);
  mk_listalloc(&m_coeffL,sizeof(mk_vertex),0);

}

Interpolation::~Interpolation() {

  clearArr();
  mk_listfree(&m_coeffL);
  mk_listfree(&m_ctrlL);

}

mk_ulreal Interpolation::options() const {

  return m_options;

}

int Interpolation::invalidate() {

  return 0;

}

int Interpolation::clearCtrl() {

  invalidate();
  clearArr();
  return 0;

}

int Interpolation::setCtrl(struct mk_list *ctrlL) {

  clearCtrl();
  int res=setArr(ctrlL);
  return res;

}

int Interpolation::nctrl() const {

  return m_ctrlL.count;

}

int Interpolation::setup() {

  return 0;

}

int Interpolation::interpol(int,struct mk_list *,double,double) {

  return 0;

}

int Interpolation::interp(mk_vertex) {

  return 0;

}

int Interpolation::extrap(mk_vertex) {

  return 0;

}

int Interpolation::coeff(double,struct mk_list *) {
  
  return 0;

}

mk_ulreal Interpolation::setOptions(mk_ulreal options) {

  m_options=(options&(interpolation_type|interpolation_options));
  return m_options;

}

void Interpolation::clearArr() {

  mk_listclear(&m_ctrlL,0);
  m_ready=0;

}

int Interpolation::setArr(struct mk_list *ctrlL) {

  clearArr();
  int ii=0,cnt=(ctrlL ? ctrlL->count : 0);  
  mk_listrealloc(&m_ctrlL,cnt);
  mk_vertex vertex;
  for (ii=0;ii<cnt;ii++) {
    mk_listat(ctrlL,ii,(void*)&vertex);
    mk_listsetat(&m_ctrlL,(void*)&vertex,ii,1);
  }
  return 0;

}

InterpolationConst::InterpolationConst(struct mk_list *ctrlL) : 
  Interpolation(interpolation_const) {

  setArr(ctrlL);

}

InterpolationConst::~InterpolationConst() {

}

int InterpolationConst::interpol(int nint,struct mk_list *vint,double,double) {

  int ii=0,jj=0,kk=0,fidx=1,nctrl=m_ctrlL.count,
    nintmin=2*nctrl-1,fillcnt=(nint-nintmin)/(nctrl-1);
  if (nint<=0 || nint<nctrl || nctrl==0 || !vint)
    return 1;
  mk_vertexnan(vv1);
  mk_vertexnan(vv2);
  mk_vertexnan(vidx);
  mk_listclear(vint,0);
  mk_listrealloc(vint,nint);
  if (nint<nintmin) {
    for (ii=0;ii<nctrl;ii++) {
      mk_listat(&m_ctrlL,ii,(void*)&vv1);
      mk_listappend(vint,(void*)&vv1);
    }
    return 0;
  }
  
  double xl=.0,xh=.0;
  mk_listat(&m_ctrlL,0,(void*)&vv1);
  mk_listappend(vint,(void*)&vv1);
  if ((m_options&interpolation_bwd)>0)
    fidx=0;
  for (ii=1;ii<nctrl;ii++) {
    mk_listat(&m_ctrlL,ii-1,(void*)&vv1);
    mk_listat(&m_ctrlL,ii,(void*)&vv2);
    xl=vv1[0];
    xh=vv2[0];
    // next -> last
    mk_listat(&m_ctrlL,ii-fidx,(void*)&vidx);
    vv1[0]=xl;
    vv1[1]=vidx[1];
    vv1[2]=vidx[2];
    mk_listappend(vint,(void*)&vv1);
    for (jj=0;jj<fillcnt;jj++) {
      vv1[0]=xl+(double)(jj+1)*(xh-xl)/(double)fillcnt;
      vv1[1]=vidx[1];
      vv1[2]=vidx[2];
      mk_listappend(vint,(void*)&vv1);
    }
    vv1[0]=xh;
    vv1[1]=vidx[1];
    vv1[2]=vidx[2];
    mk_listappend(vint,(void*)&vv1);
  }
  return 0;

}

int InterpolationConst::interp(mk_vertex vv) {

  int ii=0,nctrl=m_ctrlL.count;
  if (nctrl==0) {
    vv[1]=mk_dnan;
    return 1;
  }
  mk_vertexnan(vvidx);
  for (ii=1;ii<nctrl;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&vvidx);
    if (vvidx[0]>vv[0]) {
      if ((m_options&interpolation_bwd)>0)
        vv[1]=vvidx[1];
      else {
        mk_listat(&m_ctrlL,ii-1,(void*)&vvidx);
        vv[1]=vvidx[1];
      }
      return 0;
    }
  }
  mk_listat(&m_ctrlL,nctrl-1,(void*)&vvidx);
  vv[1]=vvidx[1];
  return 1;

}

InterpolationLinear::InterpolationLinear(struct mk_list *ctrlL) : 
  Interpolation(interpolation_linear) {

  setArr(ctrlL);

}

InterpolationLinear::~InterpolationLinear() {

}

int InterpolationLinear::interpol(int nint,struct mk_list *vint,double,double) {

  int ii=0,jj=0,nctrl=m_ctrlL.count;
  if (nint<=0 || nctrl==0 || nint<nctrl || !vint)
    return 1;
  mk_listclear(vint,0);
  mk_listrealloc(vint,nint);
  mk_vertexnan(pl);
  mk_vertexnan(ph);
  mk_vertexnan(vv);
  mk_listat(&m_ctrlL,0,(void*)&pl);
  mk_vertexcopy(vv,pl);
  if (nctrl==1) {
    for (ii=0;ii<nint;ii++) 
      mk_listappend(vint,(void*)&vv);
    return 0;
  }
  int chidx=(nint-nctrl)/(nctrl-1);
  double tmp=.0;
  mk_listappend(vint,(void*)&vv);
  for (ii=1;ii<nctrl;ii++) {
    if (vint->count>=nint)
      break;
    mk_listat(&m_ctrlL,ii-1,(void*)&pl);
    mk_listat(&m_ctrlL,ii,(void*)&ph);
    for (jj=0;jj<chidx;jj++) {
      tmp=pl[0]+(double)(jj+1)*(ph[0]-pl[0])/(double)(chidx+1);
      mk_listat(&m_ctrlL,ii*nctrl/nint,(void*)&vv);
      vv[0]=tmp;
      vv[1]=mk_lineq(pl,ph,tmp);
      mk_listappend(vint,(void*)&vv);
    }
    mk_listat(&m_ctrlL,ii,(void*)&vv);
    vv[0]=ph[0];
    vv[1]=ph[1];
    mk_listappend(vint,(void*)&vv);
  }
  mk_listat(&m_ctrlL,nctrl-1,(void*)&vv);
  for (ii=vint->count;ii<nint;ii++)
    mk_listappend(vint,(void*)&vv);
  return 0;

}

int InterpolationLinear::interp(mk_vertex vv) {

  int ii=0,nctrl=m_ctrlL.count;
  if (nctrl<=0) {
    vv[0]=mk_dnan;
    return 1;
  }
  mk_vertexnan(pl);
  mk_vertexnan(ph);
  for (ii=1;ii<nctrl;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&ph);
    if (ph[0]>vv[0]) {
    mk_listat(&m_ctrlL,ii-1,(void*)&pl);  
      vv[1]=((vv[0]-pl[0])*(ph[1]-pl[1])/(ph[0]-pl[0]));      
      return 0;
    }
  }
  return 1;

}

CubicSpline::CubicSpline(mk_ulreal options) :
  Interpolation(interpolation_cubicspline|options),m_der(0) {

}

CubicSpline::~CubicSpline() {

  if (m_der) {
    delete [] m_der;
    m_der=0;
  }

}

int CubicSpline::setup() {

  m_ctrlL.cmp=mk_vertexcmpx;
  mk_listsort(&m_ctrlL);
  return makeSpline(0);

}

int CubicSpline::invalidate() {

  if (m_der) {
    delete [] m_der;
    m_der=0;
  }
  return 0;

}

int CubicSpline::makeSpline(double *der) {

  int res=1,nctrl=m_ctrlL.count;
  if ((m_options&interpolation_solve1st)>0)
    res=makeSpline1st();
  else if ((m_options&interpolation_solve2nd)>0)
    res=makeSpline2nd();
  if (der) {
    if (res==0 && m_der)
      memcpy(&der[0],&m_der[0],nctrl*sizeof(double));
    else
      memset(&der[0],0,nctrl*sizeof(double));
  }
  return res;

}

int CubicSpline::setSpline(double *der) {

  int nctrl=m_ctrlL.count;
  if (!der || nctrl==0)
    return 1;
  if (m_der)
    delete [] m_der;
  m_der=new double[nctrl];
  memcpy(&m_der[0],&der[0],nctrl*sizeof(double));
  return 0;

}

int CubicSpline::interpol(int nint,struct mk_list *vint,double from,double to) {

  if (!vint || nint<=0)
    return 1;
  int ii=0,jj=0,nctrl=m_ctrlL.count;
  mk_listclear(vint,0);
  mk_listrealloc(vint,nint);
  if (!m_der)
    return 1;
  mk_vertexnan(vv1);
  mk_vertexnan(vv2);
  if (nint==nctrl) {
    for (ii=0;ii<nint;ii++) {
      mk_listat(&m_ctrlL,ii,(void*)&vv1);
      mk_listappend(vint,(void*)&vv1);
    }
    return 0;
  }
  if (mk_isnan(from)) {
    mk_listat(&m_ctrlL,0,(void*)&vv1);
    from=vv1[0];
  }
  if (mk_isnan(to)) {
    mk_listat(&m_ctrlL,nctrl-1,(void*)&vv1);
    to=vv1[0];
  }
  int startidx=0,endidx=nctrl-1;
  for (ii=0;ii<nctrl;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&vv1);
    if (mk_isbusted(from)==0 && from>=vv1[0])
      startidx=ii;
    if (mk_isbusted(to)==0 && to<=vv1[0])
      endidx=ii;
  }
  int nnint=nint-(endidx-startidx+1)+2;
  mk_listat(&m_ctrlL,startidx,(void*)&vv1);
  mk_listat(&m_ctrlL,endidx,(void*)&vv2);
  double interval=(vv2[0]-vv1[0])/(double)(nnint-1);
  double xx=vv1[0];
  mk_listappend(vint,(void*)&vv1);
  for (ii=1;ii<nnint-1;ii++) {
    xx+=interval;
    vv1[0]=xx;
    mk_listappend(vint,(void*)&vv1);
  }
  for (ii=startidx+1;ii<=endidx;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&vv2);
    vv1[0]=vv2[0];
    mk_listappend(vint,(void*)&vv1);
  }
  vint->cmp=mk_vertexcmpx;
  mk_listsort(vint);

  for (ii=0;ii<nint;ii++) {
    mk_listat(vint,ii,(void*)&vv1);
    mk_listat(&m_ctrlL,ii*(endidx-startidx+1)/nint,(void*)&vv2);
    vv1[2]=vv2[2];
    if (interp(vv1)!=0)
      vv1[1]=.0;
    mk_listsetat(vint,(void*)&vv1,ii,0);
  }
  return 0;

}

int CubicSpline::interp(mk_vertex vv) {

  if (!m_der)
    return 1;
  int nctrl=m_ctrlL.count,idxh=-1,idxl=nctrl;
  mk_vertex vvv;
  mk_vertexcopy(vvv,vv);
  int ss=mk_binsearchinterval((void*)&vvv,sizeof(mk_vertex),nctrl,m_ctrlL.arr,
        mk_vertexcmpx,&idxl,&idxh,0);
  if (ss==1 || idxh<=idxl)
    return 1;
  double res=.0,aa=.0,bb=.0,hx=mk_dnan,hy=mk_dnan;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  mk_listat(&m_ctrlL,idxl,(void*)&vvl);
  mk_listat(&m_ctrlL,idxh,(void*)&vvh);
  hx=vvh[0]-vvl[0];
  hy=vvh[1]-vvl[1];
  if(hx==.0) {
    return 1;
  }
  if ((m_options&interpolation_solve1st)>0) {
    double eval=(vv[0]-vvl[0]);
    aa=vvl[1];
    bb=m_der[idxl];
    double dd=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    double cc=(hy/hx-m_der[idxl])/hx-hx*dd;
    res=aa+bb*eval+cc*eval*eval+dd*eval*eval*eval;
  }
  else if ((m_options&interpolation_solve2nd)>0) {
    aa=(vvh[0]-vv[0])/hx;
    bb=(vv[0]-vvl[0])/hx;
    res=aa*vvl[1]+
        bb*vvh[1]+
        (aa*aa*aa-aa)*hx*hx*m_der[idxl]/6.0+
        (bb*bb*bb-bb)*hx*hx*m_der[idxh]/6.0;
  }
  vv[1]=res;
  return 0;

}

int CubicSpline::extrap(mk_vertex vv) {

  if (!vv || !m_der)
    return 1;

  int nctrl=m_ctrlL.count;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  mk_listat(&m_ctrlL,0,(void*)&vvl);
  mk_listat(&m_ctrlL,nctrl-1,(void*)&vvh);
  if (vv[0]>=vvl[0] && vv[0]<=vvh[0])
    return interp(vv);
  int idxl=(vv[0]<vvl[0] ? 0 : nctrl-2),
      idxh=(vv[0]<vvl[0] ? 1 : nctrl-1);
  mk_listat(&m_ctrlL,idxl,(void*)&vvl);
  mk_listat(&m_ctrlL,idxh,(void*)&vvh);
  double hx=vvh[0]-vvl[0],hy=vvh[1]-vvl[1];
  if (hx==.0) {
    return 1;
  }
  double c0=.0,c1=.0,c2=.0,c3=.0;
  if ((m_options&interpolation_solve1st)>0) {
    c0=vvl[1];
    c1=m_der[idxl];
    c3=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    c2=(hy/hx-m_der[idxl])/hx-hx*c3;
  }
  else if ((m_options&interpolation_solve2nd)>0) {
    double dx=-hx,dx6=6.*dx;
    c0=(6.*vvl[0]*vvh[1]+
         vvh[0]*(-6.*vvl[1]+vvl[0]*
         (-vvh[0]*(2.*m_der[idxl]+m_der[idxh])+vvl[0]*
         (m_der[idxl]+2.*m_der[idxh]))))/dx6;
    c1=(6.*(vvl[1]-vvh[1])+
          2.*vvl[0]*vvh[0]*(m_der[idxl]-m_der[idxh])+
          vvh[0]*vvh[0]*(2.*m_der[idxl]+m_der[idxh])-
          vvl[0]*vvl[0]*(m_der[idxl]+2*m_der[idxh]))/dx6;
    c2=(vvl[0]*m_der[idxh]-vvh[0]*m_der[idxl])/(2.*dx);
    c3=(m_der[idxl]-m_der[idxh])/dx6;
  }

  double res=(c0+vv[0]*(c1+vv[0]*(c2+vv[0]*c3)));
  vv[1]=res;
  return 0;

}

int CubicSpline::coeff(double xx,struct mk_list *coeffL) {

  if (mk_isbusted(xx)!=0 || !coeffL || !m_der)
    return 1;
  int nctrl=m_ctrlL.count,idxh=-1,idxl=nctrl;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  mk_vertexnan(vvidx);
  vvl[0]=xx;
  if (mk_binsearchinterval((void*)&vvl,sizeof(mk_vertex),nctrl,&(m_ctrlL.arr),
      mk_vertexcmpx,&idxl,&idxh,0)==1 || idxh<=idxl)
    return 1;
  mk_listat(&m_ctrlL,idxl,(void*)&vvl);
  mk_listat(&m_ctrlL,idxh,(void*)&vvh);
  double hx=vvh[0]-vvl[0],hy=vvh[1]-vvl[1],tmp=.0;
  if (hx==.0)
    return 1;
  mk_listclear(coeffL,0);
  if ((m_options&interpolation_solve1st)>0) {
    vvidx[1]=vvl[1];
    mk_listappend(coeffL,(void*)&vvidx);
    vvidx[1]=m_der[idxl];
    mk_listappend(coeffL,(void*)&vvidx);
    tmp=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    vvidx[1]=(hy/hx-m_der[idxl])/hx-hx*tmp;
    mk_listappend(coeffL,(void*)&vvidx);
    vvidx[1]=tmp;
    mk_listappend(coeffL,(void*)&vvidx);
    return 0;
  }
  else if ((m_options&interpolation_solve2nd)>0) {
    double dx=-hx,dx6=6.*dx;
    vvidx[1]=(6.*vvl[0]*vvh[1]+vvh[0]*
      (-6.*vvl[1]+vvl[0]*(-vvh[0]*(2.*m_der[idxl]+m_der[idxh])+
      vvl[0]*(m_der[idxl]+2.*m_der[idxh]))))/dx6;
    mk_listappend(coeffL,(void *)&vvidx);
    vvidx[1]=(6.*(vvl[1]-vvh[1])+
               2.*vvl[0]*vvh[0]*(m_der[idxl]-m_der[idxh])+
               vvh[0]*vvh[0]*(2.*m_der[idxl]+m_der[idxh])-
               vvl[0]*vvl[0]*(m_der[idxl]+2*m_der[idxh]))/dx6;
    mk_listappend(coeffL,(void*)&vvidx);
    vvidx[1]=(vvl[0]*m_der[idxh]-vvh[0]*m_der[idxl])/(2.*dx);
    mk_listappend(coeffL,(void*)&vvidx);
    vvidx[1]=(m_der[idxl]-m_der[idxh])/dx6;
    mk_listappend(coeffL,(void*)&vvidx);
    return 0;
  }
  return 1;

}

int CubicSpline::makeSpline1st() {

  invalidate();
  int ii=0,nctrl=m_ctrlL.count,num=nctrl,pennum=num-1;
  if (nctrl<=1)
    return 1;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  double *hx=new double[(size_t)pennum];
  double *hy=new double[(size_t)pennum];
  double *ss=new double[(size_t)pennum];
  for (ii=0;ii<pennum;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&vvl);
    mk_listat(&m_ctrlL,ii+1,(void*)&vvh);
    hx[ii]=vvh[0]-vvl[0];
    hy[ii]=vvh[1]-vvl[1];
    if (hx[ii]==.0) {
      delete [] hx;
      delete [] hy;
      delete [] ss;
      return -1;
    }
    ss[ii]=hy[ii]/hx[ii];
  }

  SquareMatrix mat(num);
  double *rr=new double[(size_t)num];
  rr[0]=rr[pennum]=.0;
  for (ii=1;ii<pennum;ii++)
    rr[ii]=3.*hx[ii-1]*ss[ii]+3.*hx[ii]*ss[ii-1];

  if ((m_options&interpolation_notaknot)>0 && num>3) {
    rr[0]=ss[0]*hx[1]*(2.*hx[1]+3.*hx[0])+ss[1]*hx[0]*hx[0];
    rr[pennum]=-ss[num-3]*hx[num-2]*hx[num-2]-ss[num-2]*hx[num-3]*(3.*hx[num-2]+2.*hx[num-3]);
    double tmp=hx[0]+hx[1];
    mat.setValue(0,0,hx[1]*tmp);
    mat.setValue(0,1,tmp*tmp);
    tmp=hx[num-2]+hx[num-3];
    mat.setValue(pennum,pennum-1,-tmp*tmp);
    mat.setValue(pennum,pennum,-hx[num-3]*tmp);
  }
  else {
    if ((m_options&interpolation_der1st)>0) { // try der1st
      rr[0]=(nctrl>2 ? .0 : ss[0]);
      rr[pennum]=(nctrl>2 ? .0 : ss[0]);
      mat.setValue(0,0,1.);
      mat.setValue(pennum,pennum,1.);
    }
    else { // natural spline
      rr[0]=.0;
      rr[pennum]=.0;
      mat.setValue(0,0,1.);
      mat.setValue(pennum,pennum,1.);
    }
  }

  for (ii=1;ii<pennum;ii++) {
    mat.setValue(ii,ii-1,hx[ii]);
    mat.setValue(ii,ii,2.*(hx[ii-1]+hx[ii]));
    mat.setValue(ii,ii+1,hx[ii-1]);
  }

  m_der=new double[(size_t)num];
  memset(&m_der[0],0,num*sizeof(double));
  mat.invalidate();
  int solved=mat.solveFor(num,rr,m_der);
  delete [] hy;
  delete [] rr;
  if (solved!=0) {
    invalidate();
    delete [] hx;
    delete [] ss;
    return 1;
  }
  if (nctrl>2 && (m_options&interpolation_monotonic)>0) {
    // apply hyman filter
    double corr=.0,pm=.0,pu=.0,pd=.0,MM=.0,deri=.0,fabsderi=.0;
    for (ii=0;ii<num;ii++) {
      corr=.0;
      deri=m_der[ii];
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
        m_der[ii]=corr;
   //printf ("i=%d deri=%f corr=%f\n",i,deri,corr);
    }
  }

  delete [] hx;
  delete [] ss;
  return solved;

}

int CubicSpline::makeSpline2nd() {

  invalidate();
  int ii=0,nctrl=m_ctrlL.count,num=nctrl,pennum=num-1;
  if (nctrl<=1)
    return 1;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  double *hx=new double[(size_t)num];
  double *hy=new double[(size_t)num];
  hx[0]=.0;
  hy[0]=.0;
  for (ii=1;ii<num;ii++) {
    mk_listat(&m_ctrlL,ii-1,(void*)&vvl);
    mk_listat(&m_ctrlL,ii,(void*)&vvh);
    hx[ii]=vvh[0]-vvl[0];
    hy[ii]=vvh[1]-vvl[1];
    if (mk_deq(hx[ii],0.0)) {
      delete [] hx;
      delete [] hy;
      return 1;
    }
  }
  SquareMatrix mat(num);
  double *rr=new double[(size_t)num];
  rr[0]=rr[pennum]=0.0;
  for (ii=1;ii<pennum;ii++)
    rr[ii]=hy[ii+1]/hx[ii+1]-hy[ii]/hx[ii];

  if ((m_options&interpolation_notaknot)>0 && num>3) {
    mat.setValue(0,0,hx[2]);
    mat.setValue(0,1,-hx[1]-hx[2]);
    mat.setValue(0,2,hx[1]);
    mat.setValue(pennum,pennum-2,hx[pennum]);
    mat.setValue(pennum,pennum-1,-hx[pennum]-hx[pennum-1]);
    mat.setValue(pennum,pennum,hx[pennum-1]);
  }
  else if ((m_options&interpolation_periodic)>0) { // not implemented -> natural spline
    mat.setValue(0,0,1.);
    mat.setValue(pennum,pennum,1.);
  }
  else if ((m_options&interpolation_nat)>0) { // natural spline
    mat.setValue(0,0,1.);
    mat.setValue(pennum,pennum,1.);
  }
  else { // not implemented -> natural spline
    mat.setValue(0,0,1.);
    mat.setValue(pennum,pennum,1.);
  }

  for (ii=1;ii<pennum;ii++) {
    mat.setValue(ii,ii-1,hx[ii]/6.0);   
    mat.setValue(ii,ii,(hx[ii+1]+hx[ii])/3.0);
    mat.setValue(ii,ii+1,hx[ii+1]/6.0);
  }
  m_der=new double[(size_t)num];
  for (ii=0;ii<num;ii++)
    m_der[ii]=.0;
  mat.invalidate();
  int solved=mat.solveFor(num,rr,m_der);
  if (solved!=0)
    invalidate();
  delete [] hx;
  delete [] hy;
  delete [] rr;

  return solved;

}

CubicSplineP::CubicSplineP(struct mk_list *ctrlL) :
  Interpolation(interpolation_cubicspline),m_xspline(0),m_yspline(0) {

  setArr(ctrlL);

}

CubicSplineP::~CubicSplineP() {

  if (m_xspline) {
    delete m_xspline;
    m_xspline=0;
  }
  if (m_yspline) {
    delete m_yspline;
    m_yspline=0;
  }

}

int CubicSplineP::invalidate() {

  if (m_xspline) {
    delete m_xspline;
    m_xspline=0;
  }
  if (m_yspline) {
    delete m_yspline;
    m_yspline=0;
  }
  return 0;

}

int CubicSplineP::setup() {

  return makeSpline(0,0);

}

int CubicSplineP::makeSpline(double *der1,double *der2) {

  invalidate();
  int ii=0,nctrl=m_ctrlL.count,num=nctrl;
  if (nctrl<=1)
    return 1;
  mk_vertexnan(vv);
  mk_vertexnan(vvc);
  mk_vertexnan(vvs);
  vv[0]=.0;
  struct mk_list ctrlL;
  mk_listalloc(&ctrlL,sizeof(mk_vertex),nctrl);
  mk_listappend(&ctrlL,(void*)&vv);
  for (ii=1;ii<num;ii++) {
    mk_listat(&ctrlL,ii-1,(void*)&vvs);
    mk_listat(&m_ctrlL,ii,(void*)&vvc);
    vv[0]=vvs[0]+sqrt(vvc[0]*vvc[0]+vvc[1]*vvc[1]);
    mk_listappend(&ctrlL,(void*)&vv);
  }
  mk_ulreal optL=(interpolation_solve2nd|interpolation_nat);
  m_xspline=new CubicSpline(optL);
  m_yspline=new CubicSpline(optL);
  for (ii=0;ii<nctrl;ii++) {
    mk_listat(&ctrlL,ii,(void*)&vvs);
    mk_listat(&m_ctrlL,ii,(void*)&vvc);
    vvs[1]=vvc[0];
    mk_listsetat(&ctrlL,(void*)&vvs,ii,0);
  }
  int spl1=m_xspline->setCtrl(&ctrlL);
  for (ii=0;ii<nctrl;ii++) {
    mk_listat(&ctrlL,ii,(void*)&vvs);
    mk_listat(&m_ctrlL,ii,(void*)&vvc);
    vvs[1]=vvc[1];
    mk_listsetat(&ctrlL,(void*)&vvs,ii,0);
  }
  int spl2=m_yspline->setCtrl(&ctrlL);
  spl1=m_xspline->makeSpline(der1);
  spl2=m_yspline->makeSpline(der2);
  if (spl1!=0 || spl2!=0) {
    if (der1)
      memset(&der1[0],0,num*sizeof(double));
    if (der1)
      memset(&der2[0],0,num*sizeof(double));
    invalidate();
  }
  return (spl1==0 && spl2==0 ? 0 : 1);

}

int CubicSplineP::setSpline(double *der1,double *der2) {

  if (!der1 || !der2 || m_ctrlL.count==0 || !m_xspline || !m_yspline)
    return 1;
  return (m_xspline->setSpline(der1)==0 && m_yspline->setSpline(der2)==0 ? 0 : 1);

}

int CubicSplineP::interpol(int nint,struct mk_list *vint,double,double) {

  if (!vint)
    return 1;
  mk_listclear(vint,0);
  mk_listrealloc(vint,nint);
  mk_vertexnan(vv1);
  mk_vertexnan(vv2);
  int ii=0,nctrl=m_ctrlL.count;
  if (nint<=nctrl || !m_xspline || !m_yspline) {
    for (ii=0;ii<nint;ii++) {
      mk_listat(&m_ctrlL,ii,(void*)&vv1);
      mk_listappend(vint,(void*)&vv1);
    }
    return 1;
  }
  struct mk_list tmp;
  mk_listalloc(&tmp,sizeof(mk_vertex),nint);
  m_xspline->interpol(nint,&tmp);
  for (ii=0;ii<nint;ii++) {
    mk_listat(&tmp,ii,(void*)&vv1);
    vv1[0]=vv1[1];
    mk_listappend(vint,(void*)&vv1);
  }
  mk_listclear(&tmp,0);
  m_yspline->interpol(nint,&tmp);
  for (ii=0;ii<nint;ii++) {
    mk_listat(&tmp,ii,(void*)&vv1);
    mk_listat(vint,ii,(void*)&vv2);
    vv2[1]=vv1[1];
    mk_listat(&m_ctrlL,ii*nctrl/nint,(void*)&vv1);
    vv2[2]=vv1[2];
    mk_listsetat(vint,(void*)&vv2,ii,0);
  }
  mk_listfree(&tmp);
  return 0;

}

int CubicSplineP::interp(mk_vertex) {

  return 0;

}

Polynomial::Polynomial(mk_ulreal options) :
  Interpolation(interpolation_polynomial|options),m_c(0),m_d(0) {

}

Polynomial::~Polynomial() {

  int ii=0,nctrl=m_ctrlL.count;
  if (m_c) {
    for (ii=0;ii<nctrl;ii++)
      delete [] m_c[ii];
    delete [] m_c;
    m_c=0;
  }
  if (m_d) {
    for (ii=0;ii<nctrl;ii++)
      delete [] m_d[ii];
    delete [] m_d;
    m_d=0;
  }

}

int Polynomial::invalidate() {

  int ii=0,nctrl=m_ctrlL.count;
  if (m_c) {
    for (ii=0;ii<nctrl;ii++)
      delete [] m_c[ii];
    delete [] m_c;
    m_c=0;
  }
  if (m_d) {
    for (ii=0;ii<nctrl;ii++)
      delete [] m_d[ii];
    delete [] m_d;
    m_d=0;
  }
  return 0;

}

int Polynomial::setCtrl(struct mk_list *ctrlL) {

  invalidate();
  int res=Interpolation::setCtrl(ctrlL);
  if (res==0 && (m_options&interpolation_regr)>0)
    fitRegr(m_options&(~interpolation_regr));
  return 0;

}

int Polynomial::fitRegr(int regrdeg) {

  mk_listclear(&m_coeffL,0);
  int ii=0,jj=0,kk=0,nctrl=m_ctrlL.count; 
  if (nctrl==0 || regrdeg<=0 || regrdeg>=nctrl)
    return 1;
  double tmp=mk_dnan;
  double **xxpow=new double*[nctrl];
  mk_vertexnan(vv);
  for (ii=0;ii<nctrl;ii++) {
    xxpow[ii]=new double[2*(regrdeg+1)];
	  xxpow[ii][0]=1.;
    mk_listat(&m_ctrlL,ii,(void*)&vv);
	  for (jj=1;jj<2*(regrdeg+1);jj++)
	    xxpow[ii][jj]=xxpow[ii][jj-1]*vv[0];
	}
  double *rs=new double[regrdeg+1];
	for (ii=0;ii<=regrdeg;ii++) {
	  for (jj=0;jj<nctrl;jj++) {
      mk_listat(&m_ctrlL,jj,(void*)&vv);
		  rs[ii]+=xxpow[jj][ii]*vv[1];
    }
	}
  SquareMatrix mm(regrdeg+1);
	for (ii=0;ii<=regrdeg;ii++) {
    for (jj=0;jj<=regrdeg;jj++) {
      if (ii==jj)
        mm.setValue(ii,jj,.0);
	    for (kk=0;kk<nctrl;kk++) {
        tmp=mm.getValue(ii,jj)+xxpow[kk][ii+jj];
        mm.setValue(ii,jj,tmp);
      }
	  }
	}
  //aux::Asciistr str;
  //mm.toString(&str);
	//printf ("%d mm=\n%s\n",__LINE__,(const char *)str);
  for (ii=0;ii<nctrl;ii++)
    delete [] xxpow[ii];
  delete [] xxpow;
  double *coeffL=new double[regrdeg+1];
  memset(&coeffL[0],0,(regrdeg+1)*sizeof(double));
  mm.invalidate();
  int res=mm.solveFor(regrdeg+1,rs,coeffL);
  delete [] rs;
  if (res<0) {
    delete [] coeffL;
    return 0;
  }
  mk_listrealloc(&m_coeffL,regrdeg+2);
  mk_vertexnan(coeff);
  for (ii=0;ii<=regrdeg;ii++) {
    coeff[1]=coeffL[ii];
    mk_listappend(&m_coeffL,(void*)&coeff);
  }
  delete [] coeffL;
  coeff[1]=.0;
  mk_listappend(&m_coeffL,(void*)&coeff);
  double ymean=.0,dym=.0,dyf=.0;
  for (ii=0;ii<nctrl;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&vv);
    ymean+=vv[1];
  }
  ymean/=(double)nctrl;
  for (ii=0;ii<nctrl;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&vv);
    mk_vertexcopy(coeff,vv);
    tmp=vv[1]-ymean;
    dym+=(tmp*tmp);
    if (interp(coeff)!=0)
      coeff[1]=.0;
    tmp=vv[1]-coeff[1];
    dyf+=(tmp*tmp);
  }
  coeff[1]=sqrt((dym-dyf)/dym);
  mk_listsetat(&m_coeffL,(void*)&coeff,m_coeffL.count-1,0);
  return (m_coeffL.count-1);

}

int Polynomial::prepTable() {

  invalidate();
  int ii=0,nctrl=m_ctrlL.count,jj=nctrl,kk=0;
  if (nctrl==0)
    return 1;
  if ((m_options&interpolation_eq)>0 || 
       ((m_options&interpolation_regr)>0 && m_coeffL.count>0))
    return 1;
  m_c=new double*[(size_t)nctrl];
  m_d=new double*[(size_t)nctrl];
  mk_vertexnan(vv);
  for (ii=0;ii<nctrl;ii++) {
    m_c[ii]=new double[(size_t)jj];
    m_d[ii]=new double[(size_t)jj];
    mk_listat(&m_ctrlL,ii,(void*)&vv);
    m_c[0][ii]=m_d[0][ii]=vv[1];
    if (ii>0) {
      for (kk=0;kk<jj;kk++)
        m_c[ii][kk]=m_d[ii][kk]=0.0;
    }
    jj--;
  }
  return 0;

}

int Polynomial::interpol(int nint,struct mk_list *vint,double start,double end) {

  int ii=0,idx=0,nctrl=m_ctrlL.count;
  if (!vint || nint<=0 || nctrl==0)
    return 1;
  mk_listclear(vint,0);
  if (nint<nctrl)
    return 1;
  mk_listrealloc(vint,nint);
  mk_vertexnan(vv);
  mk_vertexnan(vidx);
  if (mk_isnan(start)) {
    mk_listat(&m_ctrlL,0,(void*)&vv);
    start=((m_options&interpolation_eq)>0 ? .0 : vv[0]);
  }
  if (mk_isnan(end)) {
    mk_listat(&m_ctrlL,nctrl-1,(void*)&vv);
    end=((m_options&interpolation_eq)>0 ? 1. : vv[0]);
  }
  double interval=(end-start)/(double)(nint-1);
  if (m_coeffL.count>0 && 
       ((m_options&interpolation_eq)>0 || (m_options&interpolation_regr)>0)) {
    for (ii=0;ii<nint;ii++) {
      vidx[0]=start;
      if (interp(vidx)!=0)
        vidx[1]=.0;
      vv[0]=start;
      vv[1]=vidx[1];
      mk_listat(&m_ctrlL,ii*nctrl/nint,(void*)&vidx);
      vv[2]=vidx[2];  
      mk_listappend(vint,(void*)&vv);    
      start+=interval;
    }
    return 0;
  }
  prepTable();
  for (ii=0;ii<nctrl;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&vv);
    if (start>=vv[0])
      idx=ii;
  }
  mk_listat(&m_ctrlL,idx,(void*)&vv);
  mk_listsetat(vint,(void*)&vv,idx,0);
  idx++;
  for (ii=1;ii<nint;ii++) {
    start+=interval;
    mk_listat(&m_ctrlL,idx,(void*)&vidx);
    if (start>=vidx[0]) {
      mk_listat(&m_ctrlL,idx,(void*)&vv);
      vv[0]=vidx[0];
      vv[1]=vidx[1];
      idx++;
      if (idx>=nctrl)
        idx=nctrl-1;
    }
    else {
      mk_listat(&m_ctrlL,ii,(void*)&vv);
      vv[0]=start;
      vv[1]=interp(vv);
    }
    mk_listat(&m_ctrlL,ii*nctrl/nint,(void*)&vidx);
    vv[2]=vidx[2];
    mk_listappend(vint,(void*)&vv);
  }
  return 0;

}

int Polynomial::interp(mk_vertex vv) {

  int ii=0,jj=0,idx=0,nctrl=m_ctrlL.count;
  double res=.0,tmp1=1.,tmp2=mk_dnan,tmp3=mk_dnan,tmp4=mk_dnan,tmp5=mk_dnan,tmp6=mk_dnan;
  mk_vertexnan(vvl);
  mk_vertexnan(vvh);
  if (m_coeffL.count>0 && 
       ((m_options&interpolation_eq)>0 || (m_options&interpolation_regr)>0)) {
    int ncoeff=m_coeffL.count;
    if ((m_options&interpolation_regr)>0)
      ncoeff--;
    for (ii=0;ii<ncoeff;ii++) {
      mk_listat(&m_coeffL,ii,(void*)&vvl);
      res+=vvl[1]*tmp1;
      tmp1*=vv[0];
    }
    vv[1]=res;
    return 0;
  }
  if (nctrl==0 || !m_c || !m_d)
    return mk_dnan;
  mk_listat(&m_ctrlL,0,(void*)&vvl);
  tmp5=fabs(vvl[0]-vv[0]);
  for (ii=1;ii<nctrl;ii++) {
    mk_listat(&m_ctrlL,ii,(void*)&vvl);
    tmp6=fabs(vvl[0]-vv[0]);
    if (tmp6<tmp5) {
      tmp5=tmp6;
      idx=ii;
    }
    for (jj=0;jj<(nctrl-ii);jj++) {
      mk_listat(&m_ctrlL,jj,(void*)&vvl);
      mk_listat(&m_ctrlL,jj+ii,(void*)&vvh);
      tmp1=vvl[0]-vv[0];
      tmp2=vvh[0]-vv[0];
      tmp4=tmp1-tmp2;
      if (mk_deq(tmp4,.0))
        tmp3=.0;
      else
        tmp3=(m_c[ii-1][jj+1]-m_d[ii-1][jj])/tmp4;
      m_d[ii][jj]=tmp2*tmp3;
      m_c[ii][jj]=tmp1*tmp3;
    }
  }
  res=m_c[0][idx];
  bool down=(idx==0 ? true : (nctrl/idx>=2 ? true : false));
  //bool down=(idx>=idm ? false : true);
  for (ii=1;ii<nctrl;ii++) {
    if (down) {
      if (idx<1)
        down=true;
      else
        down=false;
      res+=m_c[ii][idx];
    }
    else {
      res+=m_d[ii][--idx];
      if (idx<1 || idx<(nctrl-ii-1))
        down=true;
      else
        down=false;
    }
    //idm=(m_nctrl-i)/2;
    //down=(idx>idm ? false : true);
  }
  vv[1]=res;
  return 0;

}

int Polynomial::coeff(double,struct mk_list *cc) {

  if (!cc)
    return -1;
  mk_listclear(cc,0);
  mk_vertexnan(vv);
  int ii=0,jj=0,kk=0,ll=0,idx=0,nctrl=m_ctrlL.count,ncoeff=m_coeffL.count;
  if (ncoeff>0 && ((m_options&interpolation_eq)>0 || (m_options&interpolation_regr)>0)) {
    mk_listrealloc(cc,ncoeff);
    for (ii=0;ii<ncoeff;ii++) {
      mk_listat(&m_coeffL,ii,(void*)&vv);
      mk_listappend(cc,(void*)&vv);
    }
    return ncoeff;
  }
  mk_listclear(&m_coeffL,0);
  mk_listrealloc(&m_coeffL,nctrl);
  Polynomial pp(interpolation_ctrl);
  pp.setCtrl(&m_ctrlL);
  pp.prepTable();
  double min=mk_dnan;
  mk_vertexnan(coeff);
  for (ii=0;ii<nctrl;ii++) {
    coeff[0]=.0;
    if (pp.interp(coeff)!=0)
      coeff[1]=.0;
    mk_listappend(&m_coeffL,(void*)&coeff);
    min=mk_dnan;
    kk=0;
    for (jj=0;jj<nctrl-ii;jj++) {
      mk_listat(&(pp.m_ctrlL),jj,(void*)&vv);
      if (mk_isnan(min) || fabs(vv[0])<min) {
        min=fabs(vv[0]);
        kk=jj;
      }
      // y=f(x)=a0+a1+x+a2*x*x+a3*x*x*x+a4*x*x*x*x .....
      // -> (y-a0)/x=a1+a2*x+a3*x*x+a4*x*x*x ...
      vv[1]=((vv[1]-coeff[1])/vv[0]);
      mk_listsetat(&(pp.m_ctrlL),(void*)&vv,jj,0);
    }
    for (jj=kk+1;jj<nctrl-ii;jj++) {
      mk_listat(&(pp.m_ctrlL),jj,(void*)&vv);
      mk_listsetat(&(pp.m_ctrlL),(void*)&vv,jj-1,0);
    }
    mk_listremove(&(pp.m_ctrlL),pp.m_ctrlL.count-1,0);
    jj=pp.m_ctrlL.count;
    for (kk=0;kk<pp.m_ctrlL.count;kk++) {
      mk_listat(&(pp.m_ctrlL),kk,(void*)&vv);
      pp.m_c[0][kk]=pp.m_d[0][kk]=vv[1];
      if (kk>0) {
        for (ll=0;ll<jj;ll++)
          pp.m_c[kk][ll]=pp.m_d[kk][ll]=.0;
      }
      jj--;
    }
  }
  mk_listrealloc(cc,m_coeffL.count);
  for (ii=0;ii<m_coeffL.count;ii++) {
    mk_listat(&m_coeffL,ii,(void*)&vv);
    mk_listappend(cc,(void*)&vv);
  }
  return m_coeffL.count;

}

int Polynomial::rootsBrute(double *roots,double min,double max,int *effdeg) {

  if ((m_options&interpolation_eq)>0)
    return 0;
  int ii=0,jj=0,kk=0,nctrl=m_ctrlL.count,degree=nctrl-1;
  mk_vertexnan(vv0);
  mk_vertexnan(vv1);
  mk_vertexnan(vv2);
  for (ii=nctrl-1;ii>-1;ii--)  {
    mk_listat(&m_ctrlL,ii,(void*)&vv0);
    if (vv0[0]==.0) {
      degree=ii;
      break;
    }
  }
  if (effdeg)
    *effdeg=degree;

  if (degree==0) {
    mk_listat(&m_ctrlL,0,(void*)&vv0);
    if (vv0[0]==.0) {
      roots[0]=.0;
      return 1;
    }
    return 0;
  }
  if (degree==1) {
    mk_listat(&m_ctrlL,0,(void*)&vv0);
    mk_listat(&m_ctrlL,1,(void*)&vv1);
    roots[0]=-vv0[0]/vv1[0];
    return degree;
  }
  if (degree==2) {
    mk_listat(&m_ctrlL,0,(void*)&vv0);
    mk_listat(&m_ctrlL,1,(void*)&vv1);
    mk_listat(&m_ctrlL,2,(void*)&vv2);
    double pp2=-vv1[0]/(2.*vv2[0]),pqres=pp2*pp2-vv0[0]/vv2[0];
    if (pqres<0)
      return 0;
    double rpqres=sqrt(pqres);
    roots[0]=pp2+rpqres;
    roots[1]=pp2-rpqres;
    if (roots[1]<roots[0])
      mk::swap(&roots[0],&roots[1]);
    return degree;
  }

  double res=.0,lastres=.0,minlres=mk_dnan,minhres=mk_dnan,minleval=mk_dnan,minheval=mk_dnan,
         tmp=1.,intv=(max-min)/1000.,eval=min,lasteval=eval;

  for (ii=0;ii<1000;ii++) {
    res=.0;
    tmp=1.;
    eval=min+(double)ii*intv;
    for (jj=0;jj<=degree;jj++) {
      mk_listat(&m_ctrlL,jj,(void*)&vv0);
      res+=vv0[0]*tmp;
      tmp*=eval;
    }
    if (res==.0 || (res>.0 && lastres<.0) || (lastres>.0 && res<.0)) {
      roots[kk]=(res==.0 ? eval : eval/2.+lasteval/2.);
      if (kk++==degree)
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
  if (kk==0) {
    if (mk_isbusted(minlres)==0) {
      if (mk_isbusted(minhres)==0)
        roots[kk++]=(minhres<fabs(minlres) ? minheval : minleval);
      else
        roots[kk++]=minleval;
    }
    else if (mk_isbusted(minhres)==0)
      roots[kk++]=minheval;
  }
  if (kk>1)
    mk::heapsortv(kk,roots);
  return kk;

}

Bezier::Bezier(struct mk_list *ctrlL) : 
  Interpolation(interpolation_bezier) {

  setArr(ctrlL);

}

Bezier::~Bezier() {

}

int Bezier::interpol(int nint,struct mk_list *vint,double,double) {

  int ii=0,jj=0,nctrl=m_ctrlL.count,penctrl=nctrl-1,penint=nint-1;
  if (nctrl<=0 || nint<=1 || !vint)
    return 1;
  double t1=.0,t2=1.,bino=1.;
  double *fac1=new double[(size_t)nctrl];
  double *fac2=new double[(size_t)nctrl];
  mk_vertexnan(vv);
  mk_vertexnan(vctrl);
  for (jj=0;jj<nctrl;jj++)
    fac1[jj]=fac2[jj]=1.0;
  for (ii=0;ii<nint;ii++) {
    vv[0]=vv[1]=.0;
    t1=(double)ii/(double)penint;
    t2=1.-t1;
    // poly-degree=nctrl , accumulate bernstein polys
    for (jj=1;jj<nctrl;jj++) {
      fac1[jj]=fac1[jj-1]*t1;
      fac2[penctrl-jj]=fac2[penctrl-jj+1]*t2;
    }
    for (jj=0;jj<nctrl;jj++) {
      bino=mk_binomialcoeff(penctrl,jj);
      mk_listat(&m_ctrlL,jj,(void*)&vctrl);
      vv[0]=(vv[0]+vctrl[0]*bino*fac1[jj]*fac2[jj]);
      vv[1]=(vv[1]+vctrl[1]*bino*fac1[jj]*fac2[jj]);
    }
    for (jj=0;jj<nctrl;jj++)
      fac1[jj]=fac2[jj]=1.;
    mk_listat(&m_ctrlL,ii*nctrl/nint,(void*)&vctrl);
    vv[2]=vctrl[2];
    mk_listappend(vint,(void*)&vv);
  }
  delete [] fac1;
  delete [] fac2;
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

evalulate the 4 functions at the 4 grid points leads to the linear equation 
system (0**0=!=1)  

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

static const double bicubic_helper[16][16]={
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

BicubicPatch::BicubicPatch() {

  

}

BicubicPatch::~BicubicPatch() {

  

}

Bicubic::Bicubic() : 
  Interpolation(interpolation_bicubic) {

  /*rows=MAX(1,rows);
  cols=MAX(1,cols);
  setArr(rows*cols,xx,yy,zz);*/

}

Bicubic::~Bicubic() {


}

int Bicubic::setup() {

  return 0;

}

int Bicubic::interp(mk_vertex) {


  return 0;

}

int Bicubic::interpol(int nint,struct mk_list *vint,double,double) {

  if (!vint)
    return 1;
  
  return 0;

}

} // namespace
