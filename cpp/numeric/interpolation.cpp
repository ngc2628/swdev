
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

  if ((options&interpol_type)==interpol_none)
    return 0;
  Interpolation *interpolation=0;
  if ((options&interpol_const)>0)
    interpolation=new InterpolationConst(0);
  else if ((options&interpol_linear)>0)
    interpolation=new InterpolationLinear(0);
  else if ((options&interpol_cubicspline)>0) {
    if ((options&interpol_parametric)==0)
      interpolation=new CubicSpline(options);
    else
      interpolation=new CubicSplineP();
  }
  else if ((options&interpol_polynomial)>0)  
    interpolation=new Polynomial(options);
  else if ((options&interpol_bezier)>0)
    interpolation=new Bezier(0);
  else if ((options&interpol_bicubic)>0)    
    interpolation=new Bicubic();
  interpolation->setOptions(options);
  return interpolation;

}

int numInterpolIntermediates(Interpolation *interpolation) {

  mk_ulreal options=(interpolation ? interpolation->options() : interpol_none);
  if ((options&interpol_type)==interpol_none)
    return 0;
  if ((options&interpol_const)>0)
    return 2*interpolation->nctrl()-1;
  if ((options&interpol_linear)>0)
    return interpolation->nctrl();
  return numsmoothIntermediates;
  
}

int interpolation2string(mk_ulreal option,mk_string res) {

  mk_stringset(res,"none");
  if (option==interpol_none)
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
  m_options(options) {

  mk_listalloc(&m_ctrlL,sizeof(mk_vertex),0);

}

Interpolation::~Interpolation() {

  mk_listfree(&m_ctrlL);

}

mk_ulreal Interpolation::options() const {

  return m_options;

}

mk_ulreal Interpolation::setOptions(mk_ulreal options,int acc) {

  options=(options&(interpol_type|interpol_options));
  m_options=(acc>0 ? m_options|options : options);
  return m_options;

}

int Interpolation::nctrl() const {
      
  return m_ctrlL.count;
  
}

int Interpolation::clearCtrl() {
      
  return mk_listfree(&m_ctrlL);
  
}

int Interpolation::setCtrl(struct mk_list *ctrlL) {
   
  return mk_listcopy(&m_ctrlL,ctrlL);
  
}

InterpolationConst::InterpolationConst(struct mk_list *ctrlL) : 
  Interpolation(interpol_const) {

  mk_listcopy(&m_ctrlL,ctrlL);

}

InterpolationConst::~InterpolationConst() {

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
      if ((m_options&interpol_bwd)>0)
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
  if ((m_options&interpol_bwd)>0)
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

InterpolationLinear::InterpolationLinear(struct mk_list *ctrlL) : 
  Interpolation(interpol_linear) {

  mk_listcopy(&m_ctrlL,ctrlL);

}

InterpolationLinear::~InterpolationLinear() {

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

CubicSpline::CubicSpline(mk_ulreal options) :
  Interpolation(interpol_cubicspline|options) {

  mk_cubicsplinealloc(&m_spline,0);

}

CubicSpline::~CubicSpline() {
 
  mk_cubicsplinefree(&m_spline);

}

mk_ulreal CubicSpline::setOptions(mk_ulreal options,int acc) {

  m_spline.options=Interpolation::setOptions(options,acc);
  return m_options;

}

int CubicSpline::clearCtrl() {

  mk_cubicsplinefree(&m_spline);
  return Interpolation::clearCtrl();

}

int CubicSpline::setCtrl(struct mk_list *ctrlL) {

  mk_cubicsplinefree(&m_spline);
  int res=Interpolation::setCtrl(ctrlL);
  mk_listcopy(&m_spline.ctrlL,&m_ctrlL);
  m_spline.options=m_options;
  return res;

}

int CubicSpline::setup() {

  int solved=mk_cubicsplineder(&m_spline);
  return solved;

}

int CubicSpline::coeff(double xx,double *coeffL) {

  int res=mk_cubicsplinecoeff(xx,&m_spline,coeffL);
  return res;

}

int CubicSpline::interp(mk_vertex vv) {

  int res=mk_cubicsplineinterpol(&m_spline,vv);
  return res;

}

int CubicSpline::extrap(mk_vertex vv) {

  int res=mk_cubicsplineextrapol(&m_spline,vv);
  return res;

}

int CubicSpline::interpol(int nint,struct mk_list *vint,double start,double end) {

  if (!vint || nint<=0)
    return 1;
  int ii=0,jj=0,res=1,nctrl=m_spline.ctrlL.count,doextrapol=0;
  mk_vertexzero(vv);
  mk_listat(&m_spline.ctrlL,0,(void*)&vv);
  if (mk_isnan(start)) 
    start=vv[0];
  if (start<vv[0])
    doextrapol=1;
  mk_listat(&m_spline.ctrlL,nctrl-1,(void*)&vv);
  if (mk_isnan(end))
    end=vv[0];
  if (vv[0]<end)
    doextrapol=2;
  if (end<start)
    mk_swapf(&start,&end);
  mk_listclear(vint,0);
  mk_listrealloc(vint,nint);
  nint-=nctrl;
  for (ii=0;ii<nctrl;ii++) {
    mk_listat(&m_spline.ctrlL,ii,(void*)&vv);
    mk_listappend(vint,(void*)&vv);
  }
  double interval=(end-start)/(double)(nint-1);
  for (ii=0;ii<nint;ii++) {
    vv[0]=start;
    res=(doextrapol>0 ? extrap(vv) : interp(vv));
    if (res!=0)
      vv[1]=.0;
    mk_listappend(vint,(void*)&vv);
    start+=interval;
    if ((doextrapol&2)==0 && end<start)
      start=end; 
  }
  vint->cmp=mk_vertexcmpx;
  mk_listsort(vint);
  return 0;

}

CubicSplineP::CubicSplineP() :
  Interpolation(
    interpol_cubicspline|interpol_parametric|
    interpol_solve2nd|interpol_nat) {

  mk_cubicsplinealloc(&m_xspline,0);
  mk_cubicsplinealloc(&m_yspline,0);

}

CubicSplineP::~CubicSplineP() {

  mk_cubicsplinefree(&m_xspline);
  mk_cubicsplinefree(&m_yspline);

}

mk_ulreal CubicSplineP::setOptions(mk_ulreal,int) {

  return m_options;

}

int CubicSplineP::clearCtrl() {

  mk_cubicsplinefree(&m_xspline);
  mk_cubicsplinefree(&m_yspline);
  int res=Interpolation::clearCtrl();
  return res;

}

int CubicSplineP::setCtrl(struct mk_list *ctrlL) {

  mk_cubicsplinefree(&m_xspline);
  mk_cubicsplinefree(&m_yspline);
  int res=Interpolation::setCtrl(ctrlL);
  return res;

}

int CubicSplineP::setup() {

  int ii=0,nctrl=m_ctrlL.count,num=nctrl;
  if (nctrl<=1)
    return 1;
  mk_vertexzero(vv);
  mk_vertexzero(vvc);
  struct mk_list ctrlL;
  mk_listalloc(&ctrlL,sizeof(mk_vertex),nctrl);
  mk_listat(&m_ctrlL,0,(void*)&vv);
  vv[2]=vv[1];
  vv[1]=vv[0];
  vv[0]=.0;
  mk_listappend(&ctrlL,(void*)&vv);
  for (ii=1;ii<nctrl;ii++) {
    mk_listat(&ctrlL,ii-1,(void*)&vv);
    mk_listat(&m_ctrlL,ii,(void*)&vvc);
    vv[0]=vv[0]+sqrt(vvc[0]*vvc[0]+vvc[1]*vvc[1]);
    vv[1]=vvc[0];
    vv[2]=vvc[1];
    mk_listappend(&ctrlL,(void*)&vv);
  }
  mk_cubicsplinefree(&m_xspline);
  mk_listcopy(&m_xspline.ctrlL,&ctrlL);
  m_xspline.options=m_options;
  for (ii=0;ii<nctrl;ii++) {
    mk_listat(&ctrlL,ii,(void*)&vv);
    vv[1]=vv[2];
    mk_listsetat(&ctrlL,(void*)&vv,ii,0);
  }
  mk_cubicsplinefree(&m_yspline);
  mk_listcopy(&m_yspline.ctrlL,&ctrlL);
  m_yspline.options=m_options;
  mk_listfree(&ctrlL);
  int solved=mk_cubicsplineder(&m_xspline);
  solved+=mk_cubicsplineder(&m_yspline);
  if (solved!=0) {
    mk_cubicsplinefree(&m_xspline);
    mk_cubicsplinefree(&m_yspline);
  }
  return (solved>0 ? 1 : solved);

}

int CubicSplineP::interpol(int nint,struct mk_list *vint,double,double) {

  if (!vint || nint<=0)
    return 1;
  int ii=0,res=1;
  mk_listclear(vint,0);
  mk_listrealloc(vint,nint);
  mk_vertexzero(vv);
  mk_vertexzero(vvi);
  mk_listat(&m_xspline.ctrlL,0,(void*)&vv);
  double start=vv[0];
  mk_listat(&m_xspline.ctrlL,m_xspline.ctrlL.count-1,(void*)&vv);
  double end=vv[0];
  double interval=(end-start)/(double)(nint-1);
  for (ii=0;ii<nint;ii++) {
    vvi[0]=start;
    res=mk_cubicsplineinterpol(&m_xspline,vvi);
    vv[0]=vvi[1];
    res=mk_cubicsplineinterpol(&m_yspline,vvi);
    vv[1]=vvi[1];
    mk_listappend(vint,(void*)&vv);
    start+=interval;
  }
  return 0;

}

Polynomial::Polynomial(mk_ulreal options) :
  Interpolation(interpol_polynomial|options) {

  mk_polynomialalloc(&m_polynomial,0);

}

Polynomial::~Polynomial() {

  mk_polynomialfree(&m_polynomial);
  
}

mk_ulreal Polynomial::setOptions(mk_ulreal options,int acc) {

  m_options=Interpolation::setOptions(options,acc)|(options&interpol_regrdeg);
  return m_options;

}

int Polynomial::canRegr() {

  if ((m_options&interpol_regr)>0) {
    int degree=(m_options&interpol_regrdeg);
    if (degree>0 && degree<m_polynomial.ctrlL.count)
      return degree;
  }
  return 0;

}

int Polynomial::clearCtrl() {

  mk_polynomialfree(&m_polynomial);
  return Interpolation::clearCtrl();

}

int Polynomial::setCtrl(struct mk_list *ctrlL) {

  mk_polynomialfree(&m_polynomial);
  int res=Interpolation::setCtrl(ctrlL);
  mk_listcopy(&m_polynomial.ctrlL,&m_ctrlL);
  return res;

}

int Polynomial::coeff(double,double *coeffL) {

  if (!coeffL)
    return 1;
  int res=1,degree=canRegr();
  if (degree>0)
    res=mk_polynomialfitdegr(&m_polynomial,degree,coeffL,0);
  else
    res=mk_polynomialcoeff(&m_polynomial,coeffL);
  return res;

}

int Polynomial::interpol(int nint,struct mk_list *vint,double start,double end) {

  nint=(vint ? MAX(0,nint) : 0);
  if (nint==0)
    return 1;
  int ii=0,res=1,len=m_polynomial.ctrlL.count;
  mk_vertexzero(vv);
  mk_listat(&m_polynomial.ctrlL,0,(void*)&vv);
  if (mk_isnan(start) || start<vv[0]) 
    start=vv[0];
  mk_listat(&m_polynomial.ctrlL,len-1,(void*)&vv);
  if (mk_isnan(end) || vv[0]<end) 
    end=vv[0];
  if (end<start)
    mk_swapf(&start,&end);
  mk_listclear(vint,0);
  mk_listrealloc(vint,nint);
  double *coeffL=0;
  int regrdeg=canRegr();
  if (regrdeg>0) {
    len=regrdeg+1;
    coeffL=new double[len];
    res=mk_polynomialfitdegr(&m_polynomial,regrdeg,coeffL,0);
  }
  else {
    nint-=len;
    for (ii=0;ii<len;ii++) {
      mk_listat(&m_polynomial.ctrlL,ii,(void*)&vv);
      mk_listappend(vint,(void*)&vv);
    }
  }
  double interval=(end-start)/(double)(nint-1);
  for (ii=0;ii<nint;ii++) {
    vv[0]=start;
    if (coeffL)
      res=mk_polynomialeval(len,coeffL,vv);
    else
      res=mk_polynomialinterpol(&m_polynomial,vv);
    if (res!=0)
      vv[1]=.0;
    mk_listappend(vint,(void*)&vv);
    start+=interval;
    if (start>=end)
      start=end;
  }
  vint->cmp=mk_vertexcmpx;
  mk_listsort(vint);
  if (coeffL)
    delete [] coeffL;
  return 0;

}

Bezier::Bezier(struct mk_list *ctrlL) : 
  Interpolation(interpol_bezier) {

  mk_listcopy(&m_ctrlL,ctrlL);

}

Bezier::~Bezier() {

}

int Bezier::interpol(int nint,struct mk_list *vint,double,double) {

  int res=mk_bezierinterpol(&m_ctrlL,nint,vint);
  return res;

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
  Interpolation(interpol_bicubic) {

  /*rows=MAX(1,rows);
  cols=MAX(1,cols);
  setArr(rows*cols,xx,yy,zz);*/

}

Bicubic::~Bicubic() {


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
