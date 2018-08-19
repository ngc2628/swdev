
#include <auxx/auxx.h>
#include <numeric/matrix.h>
#include <numeric/interpolation.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
//#include <sys/param.h>

namespace num {

static double *ellipsex=0;
static double *ellipsey=0;
static int ellipsen=0;
int numsmoothIntermediates=500;

void ellipse(int nn,double *x,double *y) {

  if (!x || !y || nn<=0)
    return;
  if (!ellipsex || !ellipsey || ellipsen<=0 || nn!=ellipsen) {
    if (ellipsex)
      delete [] ellipsex;
    if (ellipsey)
      delete [] ellipsey;
    ellipsex=new double[(size_t)nn];
    ellipsey=new double[(size_t)nn];
    int ii=0,off=nn/2;
    double sc=360./(double)nn;
    for (ii=0;ii<nn;ii++) {
      if (ii<=off) {
        ellipsex[ii]=cos(sc*mk_rad*(double)ii);
        ellipsey[ii]=sin(sc*mk_rad*(double)ii);
      }
      else {
        ellipsex[ii]=-cos(sc*mk_rad*(double)(ii-off));
        ellipsey[ii]=-sin(sc*mk_rad*(double)(ii-off));
      }
    }
    ellipsen=nn;
  }
  memcpy(x,ellipsex,ellipsen*sizeof(double));
  memcpy(y,ellipsey,ellipsen*sizeof(double));

}

Interpolation *buildInterpolation(const char *type,aux::TVList<aux::Asciistr> *optL) {

  if (!type)
    return 0;
  int ii=0,idx=-1,len=(int)strlen(type);
  if (len==0)
    return 0;
  for (ii=0;ii<numinterpolationtypes;ii++) {
    if (strncmp(type,interpolationtypes[ii],(size_t)len)==0) {
      idx=ii;
      break;
    }
  }
  if (idx<0)
    return 0;
  if (optL)
    optL->sort();
  Interpolation *interpolation=0;
  if (idx==1)
    interpolation=new InterpolationConst(0,0,0);
  else if (idx==2)
    interpolation=new InterpolationLinear(0,0,0);
  else if (idx==3) {
    aux::Asciistr optp("parametric");
    if (optL && optL->find(optp)>=0)
      interpolation=new SplineP(0,0,0);
    else
      interpolation=new Spline(optL);
  }
  else if (idx==4)
    interpolation=new Polynomial(optL);
  else if (idx==5)
    interpolation=new Bezier(0,0,0);

  interpolation->setOptions(optL);

  return interpolation;

}

int numInterpolIntermediates(Interpolation *interpolation) {

  if (!interpolation)
    return 0;
  aux::Asciistr itype;
  interpolation->type(&itype);
  int ii=0,idx=-1,len=itype.len();
  for (ii=0;ii<numinterpolationtypes;ii++) {
    if (strncmp((const char *)itype,interpolationtypes[ii],(size_t)len)==0) {
      idx=ii;
      break;
    }
  }
  switch (idx) {
    default:
      break;
    case 1:
      return 2*interpolation->nctrl()-1;
    case 2:
      return interpolation->nctrl();
    case 3: case 4: case 5: case 6: case 7: case 8 :
      return numsmoothIntermediates;
  }
  return 0;

}

Interpolation::Interpolation(const char *type,aux::TVList<aux::Asciistr> *optL) :
    m_type(type),m_x(0),m_y(0),m_z(0),m_nctrl(0) {

  setOptions(optL,1);

}

Interpolation::~Interpolation() {

  clearArr();

}

void Interpolation::type(aux::Asciistr *type) const {

  *type=m_type;

}

int Interpolation::invalidate() {

  return 0;

}

int Interpolation::clearCtrl() {

  invalidate();
  clearArr();
  return 0;

}

int Interpolation::setCtrl(int nctrl,double *x,double *y,double *z) {

  clearCtrl();
  return setArr(nctrl,x,y,z);

}

int Interpolation::setCtrl(aux::TVList<aux::Vector3> *v) {

  clearCtrl();
  return setArr(v);

}

int Interpolation::nctrl() const {

  return m_nctrl;

}

int Interpolation::setup() {

  return 0;

}

int Interpolation::interpol(int,double *,double *,double *,double,double) {

  return 0;

}

int Interpolation::interpol(int,aux::TVList<aux::Vector3> *,double,double) {

  return 0;

}

double Interpolation::interp(double val) const {

  return val;

}

double Interpolation::extrap(double val) const {

  return val;

}

int Interpolation::options(aux::TVList<aux::Asciistr> *optL) const {

  if (optL)
    *optL=m_options;
  return m_options.count();

}

int Interpolation::setOptions(aux::TVList<aux::Asciistr> *optL,int clr) {

  if (!optL || clr>0)
    m_options.clear();
  if (!optL)
    return 0;
  int ii=0;
  aux::Asciistr *opt=0;
  for (ii=0;ii<optL->count();ii++) {
    opt=optL->at(ii);
    if (m_options.find(*opt)<0)
      m_options.inSort(*opt);
  }
  return m_options.count();

}

void Interpolation::clearArr() {

  if (m_x) {
    delete [] m_x;
    m_x=0;
  }
  if (m_y) {
    delete [] m_y;
    m_y=0;
  }
  if (m_z) {
    delete [] m_z;
    m_z=0;
  }
  m_nctrl=0;

}

int Interpolation::setArr(int nctrl,double *x,double *y,double *z) {

  clearArr();
  if (nctrl<=1 || (!x && !y && !z))
    return -1;
  if (x)
    m_x=new double[(size_t)nctrl];
  if (y)
    m_y=new double[(size_t)nctrl];
  if (z)
    m_z=new double[(size_t)nctrl];
  int ii=0;
  for (ii=0;ii<nctrl;ii++) {
    if (m_x && x)
      m_x[ii]=x[ii];
    if (m_y && y)
      m_y[ii]=y[ii];
    if (m_z && z)
      m_z[ii]=z[ii];
  }
  m_nctrl=nctrl;
  return 0;

}

int Interpolation::setArr(aux::TVList<aux::Vector3> *v) {

  clearArr();
  if (!v || v->count()==0)
    return -1;
  m_nctrl=v->count();
  m_x=new double[(size_t)m_nctrl];
  m_y=new double[(size_t)m_nctrl];
  m_z=new double[(size_t)m_nctrl];
  int ii=0;
  for (ii=0;ii<m_nctrl;ii++) {
    m_x[ii]=v->at(ii)->x();
    m_y[ii]=v->at(ii)->y();
    m_z[ii]=v->at(ii)->z();
  }
  return 0;

}

InterpolationConst::InterpolationConst(int nctrl,double *x,double *y) : Interpolation("const") {

  setArr(nctrl,x,y,0);

}

InterpolationConst::~InterpolationConst() {

}

int InterpolationConst::interpol(int nint,double *xint,double *yint,double *zint,double,double) {

  if (nint<=0 || m_nctrl<=0 || nint<m_nctrl || !xint || !yint || !m_x || !m_y)
    return -1;
  int ii=0,jj=0,kk=0,fidx=1,nintmin=2*m_nctrl-1,fillcnt=(nint-nintmin)/(m_nctrl-1);
  if (nint<nintmin) {
    for (ii=0;ii<nint;ii++) {
      xint[ii]=(ii>=m_nctrl ? mk_dnan : m_x[ii]);
      yint[ii]=(ii>=m_nctrl ? mk_dnan : m_y[ii]);
      if (zint)
        zint[ii]=mk_dnan;
    }
    return 0;
  }
  double xl=.0,xh=.0;
  xint[0]=m_x[0];
  yint[0]=m_y[0];
  if (zint)
    zint[0]=(m_z ? m_z[0] : mk_dnan);
  aux::Asciistr opt("bwd");
  if (m_options.find(opt)>=0)
    fidx=0;
  for (ii=1;ii<m_nctrl;ii++) {
    xl=m_x[ii-1];
    xh=m_x[ii];
    // next -> last
    xint[++kk]=xl;
    yint[kk]=m_y[ii-fidx];
    if (zint)
      zint[kk]=(m_z ? m_z[ii-fidx] : mk_dnan);
    for (jj=0;jj<fillcnt;jj++) {
      xint[++kk]=xl+(double)(jj+1)*(xh-xl)/(double)fillcnt;
      yint[kk]=m_y[ii-fidx];
      if (zint)
        zint[kk]=(m_z ? m_z[ii-fidx] : mk_dnan);
    }
    xint[++kk]=xh;
    yint[kk]=m_y[ii-fidx];
    if (zint)
      zint[kk]=(m_z ? m_z[ii-fidx] : mk_dnan);
  }

  return 0;

}

int InterpolationConst::interpol(int nint,aux::TVList<aux::Vector3> *vint,double,double) {

  if (nint<=0 || m_nctrl<=0 || nint<m_nctrl || !m_x || !m_y || !vint)
    return -1;
  int ii=0,jj=0,fidx=1,nintmin=2*m_nctrl-1,fillcnt=(nint-nintmin)/(m_nctrl-1);
  if (vint->size()<MAX(nint,nintmin))
    vint->resize(MAX(nint,nintmin));
  vint->clear();
  if (nint<nintmin) {
    for (ii=0;ii<nint;ii++) {
      vint->append(aux::Vector3(ii>=m_nctrl ? mk_dnan : m_x[ii],ii>=m_nctrl ? mk_dnan : m_y[ii]));
    }
    return 0;
  }
  double xl=.0,xh=.0;
  vint->append(aux::Vector3(m_x[0],m_y[0]));
  aux::Asciistr opt("bwd");
  if (m_options.find(opt)>=0)
    fidx=0;
  for (ii=1;ii<m_nctrl;ii++) {
    xl=m_x[ii-1];
    xh=m_x[ii];
    // next -> last
    vint->append(aux::Vector3(xl,m_y[ii-fidx]));
    for (jj=0;jj<fillcnt;jj++) {
      vint->append(aux::Vector3(xl+(double)(jj+1)*(xh-xl)/(double)fillcnt,m_y[ii-fidx]));
    }
    vint->append(aux::Vector3(xh,m_y[ii-fidx]));
  }

  return 0;

}

double InterpolationConst::interp(double x) const {

  if (m_nctrl<=0 || !m_x || !m_y)
    return mk_dnan;
  aux::Asciistr opt("bwd");
  int ii=0;
  for (ii=1;ii<m_nctrl;ii++) {
    if (m_x[ii]>x) {
      if (m_options.find(opt)>=0)
        return m_y[ii];
      else
        return m_y[ii-1];
    }
  }
  return m_y[m_nctrl-1];

}

InterpolationLinear::InterpolationLinear(int nctrl,double *x,double *y) : 
  Interpolation("linear") {

  setArr(nctrl,x,y,0);

}

InterpolationLinear::~InterpolationLinear() {

}

int InterpolationLinear::interpol(int nint,double *xint,double *yint,double *zint,double,double) {

  if (nint<=0 || m_nctrl<=0 || nint<m_nctrl || !xint || !yint || !m_x || !m_y)
    return -1;
  int ii=0;
  if (m_nctrl==1) {
    for (ii=0;ii<nint;ii++) {
      xint[ii]=m_x[0];
      yint[ii]=m_y[0];
      if (zint)
        zint[ii]=mk_dnan;
    }
    return 0;
  }
  int jj=0,kk=0,chidx=(nint-m_nctrl)/(m_nctrl-1);
  double xl=.0,xh=.0,yl=.0,yh=.0,tmp=.0;
  xint[kk]=m_x[kk];
  yint[kk]=m_y[kk];
  if (zint)
    zint[kk]=(m_z ? m_z[kk] : mk_dnan);
  for (ii=1;ii<m_nctrl;ii++) {
    if (kk>=(nint-1))
      break;
    xl=m_x[ii-1];
    xh=m_x[ii];
    yl=m_y[ii-1];
    yh=m_y[ii];
    for (jj=0;jj<chidx;jj++) {
      tmp=xl+(double)(jj+1)*(xh-xl)/(double)(chidx+1);
      xint[++kk]=tmp;
      yint[kk]=mk_linEq(xl,xh,yl,yh,tmp);
      if (zint)
        zint[ii]=(m_z ? m_z[ii*m_nctrl/nint] : mk_dnan);
    }
    xint[++kk]=xh;
    yint[kk]=yh;
    if (zint)
      zint[kk]=(m_z ? m_z[ii] : mk_dnan);
  }
  for (ii=(kk+1);ii<nint;ii++) {
    xint[ii]=m_x[m_nctrl-1];
    yint[ii]=m_y[m_nctrl-1];
    if (zint)
      zint[ii]=(m_z ? m_z[m_nctrl-1] : mk_dnan);
  }
  return 0;

}

int InterpolationLinear::interpol(int nint,aux::TVList<aux::Vector3> *vint,double,double) {

  if (nint<=0 || m_nctrl<=0 || nint<m_nctrl || !vint || !m_x || !m_y)
    return -1;
  if (vint->size()<nint)
    vint->resize(nint);
  vint->clear();
  int ii=0;
  if (m_nctrl==1) {
    for (ii=0;ii<nint;ii++)
      vint->append(aux::Vector3(m_x[0],m_y[0]));
    return 0;
  }
  int jj=0,kk=0,chidx=(nint-m_nctrl)/(m_nctrl-1);
  double xl=.0,xh=.0,yl=.0,yh=.0,tmp=.0;
  vint->append(aux::Vector3(m_x[kk],m_y[kk]));
  for (ii=1;ii<m_nctrl;ii++) {
    if (kk>=(nint-1))
      break;
    xl=m_x[ii-1];
    xh=m_x[ii];
    yl=m_y[ii-1];
    yh=m_y[ii];
    for (jj=0;jj<chidx;jj++) {
      tmp=xl+(double)(jj+1)*(xh-xl)/(double)(chidx+1);
      vint->append(aux::Vector3(tmp,mk_linEq(xl,xh,yl,yh,tmp)));
      kk++;
    }
    vint->append(aux::Vector3(xh,yh));
    kk++;
  }
  for (ii=(kk+1);ii<nint;ii++)
    vint->append(aux::Vector3(m_x[m_nctrl-1],m_y[m_nctrl-1]));
  return 0;

}

double InterpolationLinear::interp(double x) const {

  if (m_nctrl<=0 || !m_x || !m_y)
    return mk_dnan;
  int ii=0;
  for (ii=1;ii<m_nctrl;ii++) {
    if (m_x[ii]>x)
      return ((x-m_x[ii-1])*(m_y[ii]-m_y[ii-1])/(m_x[ii]-m_x[ii-1]));
  }
  return m_y[m_nctrl-1];

}

Spline::Spline(aux::TVList<aux::Asciistr> *optL) :
  Interpolation("spline",optL),m_der(0) {

}

Spline::~Spline() {

  if (m_der) {
    delete [] m_der;
    m_der=0;
  }

}

int Spline::setup() {

  return makeSpline(0);

}

int Spline::invalidate() {

  if (m_der) {
    delete [] m_der;
    m_der=0;
  }
  return 0;

}

int Spline::makeSpline(double *der) {

  int res=-1;
  aux::Asciistr opt1("solve1st"),opt2("solve2nd");
  if (m_options.find(opt1)>=0)
    res=makeSpline1st();
  else if (m_options.find(opt2)>=0)
    res=makeSpline2nd();
  if (der) {
    if (res==0 && m_der)
      memcpy(&der[0],&m_der[0],m_nctrl*sizeof(double));
    else
      memset(&der[0],0,m_nctrl*sizeof(double));
  }

  return res;

}

int Spline::setSpline(double *der) {

  if (!der || m_nctrl==0 || !m_x || !m_y)
    return -1;
  if (m_der)
    delete [] m_der;
  m_der=new double[m_nctrl];
  memcpy(&m_der[0],&der[0],m_nctrl*sizeof(double));
  return 0;

}

int Spline::interpol(int nint,double *xint,double *yint,double *zint,double from,double to) {

  int ii=0,jj=0;
  if (nint<=0 || !xint || !yint || !m_der) {
    for (ii=0;ii<nint;ii++) {
      if (xint)
        xint[ii]=mk_dnan;
      if (yint)
        yint[ii]=mk_dnan;
      if (zint)
        zint[ii]=mk_dnan;
    }
    return -1;
  }

  if (nint==m_nctrl) {
    for (ii=0;ii<nint;ii++) {
      xint[ii]=m_x[ii];
      yint[ii]=m_y[ii];
      if (zint)
        zint[ii]=(m_z ? m_z[ii] : .0);
    }
    return 0;
  }

  if (mk_isNan(from))
    from=m_x[0];
  if (mk_isNan(to))
    to=m_x[m_nctrl-1];
  int startidx=0,endidx=m_nctrl-1;
  for (ii=0;ii<m_nctrl;ii++) {
    if (from>=m_x[ii])
      startidx=ii;
    if (to<=m_x[ii])
      endidx=ii;
  }

  int nnint=nint-(endidx-startidx+1)+2;
  double interval=(m_x[endidx]-m_x[startidx])/(double)(nnint-1);
  xint[jj++]=m_x[startidx];
  for (ii=1;ii<nnint-1;ii++) {
    xint[jj]=xint[jj-1]+interval;
    jj++;
  }
  for (ii=startidx+1;ii<=endidx;ii++)
    xint[jj++]=m_x[ii];

  aux::heapsortv(nint,xint);

  for (ii=0;ii<nint;ii++) {
    yint[ii]=interp(xint[ii]);
    if (zint)
      zint[ii]=(m_z ? m_z[ii*(endidx-startidx+1)/nint] : 0.0);
  }

  return 0;

}

int Spline::interpol(int nint,aux::TVList<aux::Vector3> *vint,double from,double to) {

  if (nint<=0 || !vint)
    return -1;
  if (vint->size()<nint)
    vint->resize(nint);
  vint->clear();
  int ii=0,jj=0;
  if (!m_der) {
    for (ii=0;ii<nint;ii++)
      vint->append(aux::Vector3(mk_dnan,mk_dnan));
    return -1;
  }

  if (mk_isNan(from))
    from=m_x[0];
  if (mk_isNan(to))
    to=m_x[m_nctrl-1];
  int startidx=0,endidx=m_nctrl-1;
  for (ii=0;ii<m_nctrl;ii++) {
    if (from>=m_x[ii])
      startidx=ii;
    if (to<=m_x[ii])
      endidx=ii;
  }

  int nnint=nint-(endidx-startidx+1)+2;
  double interval=(m_x[endidx]-m_x[startidx])/(double)(nnint-1);
  double *xint=new double[nint];
  xint[jj++]=m_x[startidx];
  for (ii=1;ii<nnint-1;ii++) {
    xint[jj]=xint[jj-1]+interval;
    jj++;
  }
  for (ii=startidx+1;ii<=endidx;ii++)
    xint[jj++]=m_x[ii];
  aux::heapsortv(nint,xint);

  for (ii=0;ii<nint;ii++)
    vint->append(aux::Vector3(xint[ii],interp(xint[ii])));

  delete [] xint;

  return 0;

}

double Spline::interp(double x) const {

  if (!m_der)
    return .0;
  int idxh=-1,idxl=m_nctrl;
  if (!aux::binsearchInterval(&x,m_nctrl,m_x,&idxl,&idxh) || idxh<=idxl) {
    return .0;
  }
  double hx=m_x[idxh]-m_x[idxl],hy=m_y[idxh]-m_y[idxl],a=.0,b=.0;
  if(hx==.0) {
    return .0;
  }
  double res=.0;
  aux::Asciistr opt("solve1st");
  if (m_options.find(opt)>=0) {
    double eval=(x-m_x[idxl]);
    a=m_y[idxl];
    b=m_der[idxl];
    double d=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    double c=(hy/hx-m_der[idxl])/hx-hx*d;
    res=a+b*eval+c*eval*eval+d*eval*eval*eval;
  }
  else {
    opt="solve2nd";
    if (m_options.find(opt)>=0) {
      a=(m_x[idxh]-x)/hx;
      b=(x-m_x[idxl])/hx;
      res=a*m_y[idxl]+b*m_y[idxh]+(a*a*a-a)*hx*hx*m_der[idxl]/6.0+(b*b*b-b)*hx*hx*m_der[idxh]/6.0;
    }
  }

  return res;

}

double Spline::extrap(double x) const {

  if (!m_der)
    return .0;

  if (x>=m_x[0] && x<=m_x[m_nctrl-1])
    return interp(x);

  int idxl=(x<m_x[0] ? 0 : m_nctrl-2),
      idxh=(x<m_x[0] ? 1 : m_nctrl-1);

  double hx=m_x[idxh]-m_x[idxl],hy=m_y[idxh]-m_y[idxl];
  if (hx==.0) {
    return .0;
  }

  double c0=.0,c1=.0,c2=.0,c3=.0;

  aux::Asciistr deropt("solve1st");
  if (m_options.find(deropt)>=0) {
    c0=m_y[idxl];
    c1=m_der[idxl];
    c3=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    c2=(hy/hx-m_der[idxl])/hx-hx*c3;
  }
  else {
    deropt="solve2nd";
    if (m_options.find(deropt)>=0) {
      double dx=-hx,dx6=6.*dx;
      c0=(6.*m_x[idxl]*m_y[idxh]+
                 m_x[idxh]*(-6.*m_y[idxl]+m_x[idxl]*(-m_x[idxh]*(2.*m_der[idxl]+m_der[idxh])+
                 m_x[idxl]*(m_der[idxl]+2.*m_der[idxh]))))/dx6;
      c1=(6.*(m_y[idxl]-m_y[idxh])+
                 2.*m_x[idxl]*m_x[idxh]*(m_der[idxl]-m_der[idxh])+
                 m_x[idxh]*m_x[idxh]*(2.*m_der[idxl]+m_der[idxh])-
                 m_x[idxl]*m_x[idxl]*(m_der[idxl]+2*m_der[idxh]))/dx6;
      c2=(m_x[idxl]*m_der[idxh]-m_x[idxh]*m_der[idxl])/(2.*dx);
      c3=(m_der[idxl]-m_der[idxh])/dx6;
    }
  }

  return (c0+x*(c1+x*(c2+x*c3)));

}

int Spline::coeff(double x,aux::TVList<double> *c) {

  if (!m_der)
    return -1;
  int idxh=-1,idxl=m_nctrl;
  if (!aux::binsearchInterval(&x,m_nctrl,m_x,&idxl,&idxh) ||
      idxl<0 || idxh<0 || m_nctrl<=idxl || m_nctrl<=idxh || idxh<=idxl)
    return -1;
  c->resize(4);
  c->clear();
  double hx=m_x[idxh]-m_x[idxl],hy=m_y[idxh]-m_y[idxl],tmp=.0;
  if (hx==.0)
    return -1;
  aux::Asciistr deropt("solve1st");
  if (m_options.find(deropt)>=0) {
    c->append(m_y[idxl]);
    c->append(m_der[idxl]);
    tmp=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    c->append((hy/hx-m_der[idxl])/hx-hx*tmp);
    c->append(tmp);
    return 4;
  }
  deropt="solve2nd";
  if (m_options.find(deropt)>=0) {
    double dx=-hx,dx6=6.*dx;
    c->append((6.*m_x[idxl]*m_y[idxh]+
               m_x[idxh]*(-6.*m_y[idxl]+m_x[idxl]*(-m_x[idxh]*(2.*m_der[idxl]+m_der[idxh])+
               m_x[idxl]*(m_der[idxl]+2.*m_der[idxh]))))/dx6);
    c->append((6.*(m_y[idxl]-m_y[idxh])+
               2.*m_x[idxl]*m_x[idxh]*(m_der[idxl]-m_der[idxh])+
               m_x[idxh]*m_x[idxh]*(2.*m_der[idxl]+m_der[idxh])-
               m_x[idxl]*m_x[idxl]*(m_der[idxl]+2*m_der[idxh]))/dx6);
    c->append((m_x[idxl]*m_der[idxh]-m_x[idxh]*m_der[idxl])/(2.*dx));
    c->append((m_der[idxl]-m_der[idxh])/dx6);
    return 4;
  }

  return -1;

}

int Spline::makeSpline1st() {

  invalidate();
  if (m_nctrl<=1 || !m_x || !m_y)
    return -1;

  int ii=0,num=m_nctrl,pennum=num-1;
  double *hx=new double[(size_t)pennum];
  double *hy=new double[(size_t)pennum];
  double *s=new double[(size_t)pennum];
  for (ii=0;ii<pennum;ii++) {
    hx[ii]=m_x[ii+1]-m_x[ii];
    hy[ii]=m_y[ii+1]-m_y[ii];
    if (hx[ii]==.0) {
      delete [] hx;
      delete [] hy;
      delete [] s;
      return -1;
    }
    s[ii]=hy[ii]/hx[ii];
  }

  SquareMatrix mat(num);
  double **m=mat.data();
  double *r=new double[(size_t)num];
  r[0]=r[pennum]=.0;
  for (ii=1;ii<pennum;ii++)
    r[ii]=3.*hx[ii-1]*s[ii]+3.*hx[ii]*s[ii-1];

  aux::Asciistr optnotaknot("notaknot"),optder1st("der1st"),optnat("nat"),optmonotonic("monotonic");
  if (m_options.find(optnotaknot)>=0 && num>3) {
    r[0]=s[0]*hx[1]*(2.*hx[1]+3.*hx[0])+s[1]*hx[0]*hx[0];
    r[pennum]=-s[num-3]*hx[num-2]*hx[num-2]-s[num-2]*hx[num-3]*(3.*hx[num-2]+2.*hx[num-3]);
    double tmp=hx[0]+hx[1];
    m[0][0]=hx[1]*tmp;
    m[0][1]=tmp*tmp;
    tmp=hx[num-2]+hx[num-3];
    m[pennum][pennum-1]=-tmp*tmp;
    m[pennum][pennum]=-hx[num-3]*tmp;
  }
  else {
    if (m_options.find(optder1st)>=0) { // try der1st
      r[0]=(m_nctrl>2 ? .0 : s[0]);
      r[pennum]=(m_nctrl>2 ? .0 : s[0]);
      m[0][0]=1.;
      m[pennum][pennum]=1.;
    }
    else { // natural spline
      r[0]=.0;
      r[pennum]=.0;
      m[0][0]=1.;
      m[pennum][pennum]=1.;
    }
  }

  for (ii=1;ii<pennum;ii++) {
    m[ii][ii-1]=hx[ii];
    m[ii][ii]=2.*(hx[ii-1]+hx[ii]);
    m[ii][ii+1]=hx[ii-1];
  }

  m_der=new double[(size_t)num];
  memset(&m_der[0],0,num*sizeof(double));
  mat.invalidate();
  int solved=mat.solveFor(num,r,m_der);
  delete [] hy;
  delete [] r;
  if (solved!=0) {
    invalidate();
    delete [] hx;
    delete [] s;
    return -1;
  }
  if (m_nctrl>2 && m_options.find(optmonotonic)>=0) {
    // apply hyman filter
    double corr=.0,pm=.0,pu=.0,pd=.0,M=.0,deri=.0,fabsderi=.0;
    for (ii=0;ii<num;ii++) {
      corr=.0;
      deri=m_der[ii];
      fabsderi=fabs(deri);
      if (ii==0) {
        if (deri*s[ii]>.0)
          corr=deri*MIN(fabsderi,fabs(3.*s[ii]))/fabsderi;
      }
      else if (ii==pennum) {
        if (deri*s[ii-1]>.0)
          corr=deri* MIN(fabsderi,fabs(3.*s[ii-1]))/fabsderi;
      }
      else {
        pm=(s[ii-1]*hx[ii]+s[ii]*hx[ii-1])/(hx[ii-1]+hx[ii]);
        M=3.*MIN(MIN(fabs(s[ii-1]),fabs(s[ii])),fabs(pm));
        if (ii>1) {
          if ((s[ii-1]-s[ii-2])*(s[ii]-s[ii-1])>.0) {
            pd=(s[ii-1]*(2.*hx[ii-1]+hx[ii-2])-s[ii-2]*hx[ii-1])/(hx[ii-2]+hx[ii-1]);
            if (pm*pd>.0 && pm*(s[ii-1]-s[ii-2])>.0)
              M=MAX(M,1.5*MIN(fabs(pm),fabs(pd)));
          }
        }
        if (ii<num-2) {
          if ((s[ii]-s[ii-1])*(s[ii+1]-s[ii])>.0) {
            pu=(s[ii]*(2.*hx[ii]+hx[ii+1])-s[ii+1]*hx[ii])/(hx[ii]+hx[ii+1]);
            if (pm*pu>.0 && -pm*(s[ii]-s[ii-1])>.0)
              M=MAX(M,1.5*MIN(fabs(pm),fabs(pu)));
          }
        }
        if (deri*pm>.0)
          corr=deri*MIN(fabsderi,M)/fabsderi;
      }
      if (corr!=deri)
        m_der[ii]=corr;
   //printf ("i=%d deri=%f corr=%f\n",i,deri,corr);
    }
  }

  delete [] hx;
  delete [] s;
  return solved;

}

int Spline::makeSpline2nd() {

  invalidate();
  if (m_nctrl<=1 || !m_x || !m_y)
    return -1;

  int ii=0,num=m_nctrl,pennum=num-1;
  double *hx=new double[(size_t)num];
  double *hy=new double[(size_t)num];
  hx[0]=0.0;
  hy[0]=0.0;
  for (ii=1;ii<num;ii++) {
    hx[ii]=m_x[ii]-m_x[ii-1];
    hy[ii]=m_y[ii]-m_y[ii-1];
    if (mk_deq(hx[ii],0.0)) {
      delete [] hx;
      delete [] hy;
      return -1;
    }
  }
  SquareMatrix mat(num);
  double **m=mat.data();
  double *r=new double[(size_t)num];
  r[0]=r[pennum]=0.0;
  for (ii=1;ii<pennum;ii++)
    r[ii]=hy[ii+1]/hx[ii+1]-hy[ii]/hx[ii];

  aux::Asciistr optnotaknot("notaknot"),optperiodic("periodic"),optnat("nat");
  if (m_options.find(optnotaknot)>=0  && num>3) {
    m[0][0]=hx[2];
    m[0][1]=-hx[1]-hx[2];
    m[0][2]=hx[1];
    m[pennum][pennum-2]=hx[pennum]; m[pennum][pennum-1]=-hx[pennum]-hx[pennum-1]; m[pennum][pennum]=hx[pennum-1];
  }
  else if (m_options.find(optperiodic)>=0) { // not implemented -> natural spline
    m[0][0]=1.;
    m[pennum][pennum]=1.;
  }
  else if (m_options.find(optnat)>=0) { // natural spline
    m[0][0]=1.;
    m[pennum][pennum]=1.;
  }
  else { // not implemented -> natural spline
    m[0][0]=1.;
    m[pennum][pennum]=1.;
  }

  for (ii=1;ii<pennum;ii++) {
    m[ii][ii-1]=hx[ii]/6.0;
    m[ii][ii]=(hx[ii+1]+hx[ii])/3.0;
    m[ii][ii+1]=hx[ii+1]/6.0;
  }

  m_der=new double[(size_t)num];
  for (ii=0;ii<num;ii++)
    m_der[ii]=.0;
  mat.invalidate();
  int solved=mat.solveFor(num,r,m_der);
  if (solved!=0)
    invalidate();
  delete [] hx;
  delete [] hy;
  delete [] r;

  return solved;

}

SplineP::SplineP(int nctrl,double *x,double *y) :
  Interpolation("spline"),m_pXspl(0),m_pYspl(0) {

  setArr(nctrl,x,y,0);

}

SplineP::~SplineP() {

  if (m_pXspl) {
    delete m_pXspl;
    m_pXspl=0;
  }
  if (m_pYspl) {
    delete m_pYspl;
    m_pYspl=0;
  }

}

int SplineP::invalidate() {

  if (m_pXspl) {
    delete m_pXspl;
    m_pXspl=0;
  }
  if (m_pYspl) {
    delete m_pYspl;
    m_pYspl=0;
  }
  return 0;

}

int SplineP::setup() {

  return makeSpline(0,0);

}

int SplineP::makeSpline(double *der1,double *der2) {

  invalidate();
  if (m_nctrl<=1 || !m_x || !m_y)
    return -1;
  int ii=0,num=m_nctrl;
  double *seg=new double[(size_t)num];
  seg[0]=.0;
  for (ii=1;ii<num;ii++)
    seg[ii]=seg[ii-1]+sqrt(m_x[ii]*m_x[ii]+m_y[ii]*m_y[ii]);
  aux::TVList<aux::Asciistr> optL(2);
  aux::Asciistr opt("solve2nd");
  optL.inSort(opt);
  opt="nat";
  optL.inSort(opt);
  m_pXspl=new Spline(&optL);
  m_pYspl=new Spline(&optL);
  int spl1=m_pXspl->setCtrl(num,seg,m_x),
      spl2=m_pYspl->setCtrl(num,seg,m_y);
  delete [] seg;
  spl1=m_pXspl->makeSpline(der1);
  spl2=m_pYspl->makeSpline(der2);
  if (spl1!=0 || spl2!=0) {
    if (der1)
      memset(&der1[0],0,num*sizeof(double));
    if (der1)
      memset(&der2[0],0,num*sizeof(double));
    invalidate();
  }
  return (spl1==0 && spl2==0 ? 0 : -1);

}

int SplineP::setSpline(double *der1,double *der2) {

  if (!der1 || !der2 || m_nctrl==0 || !m_pXspl || !m_pYspl)
    return -1;
  return (m_pXspl->setSpline(der1)==0 && m_pYspl->setSpline(der2)==0 ? 0 : -1);

}

int SplineP::interpol(int nint,double *xint,double *yint,double *zint,double,double) {

  int ii=0;
  if (nint<=m_nctrl || !xint || !yint || !m_pXspl || !m_pYspl) {
    for (ii=0;ii<nint;ii++) {
      if (xint)
        xint[ii]=mk_dnan;
      if (yint)
        yint[ii]=mk_dnan;
      if (zint)
        zint[ii]=mk_dnan;
    }
    return -1;
  }
  double *tmp=new double[(size_t)nint];
  m_pXspl->interpol(nint,tmp,xint);
  m_pYspl->interpol(nint,tmp,yint);
  delete [] tmp;
  if (zint) {
    for (ii=0;ii<nint;ii++)
      zint[ii]=(m_z ? m_z[ii*m_nctrl/nint] : mk_dnan);
  }
  return 0;

}

int SplineP::interpol(int nint,aux::TVList<aux::Vector3> *vint,double,double) {

  if (!vint || nint<=m_nctrl)
    return -1;
  if (vint->size()<nint)
    vint->resize(nint);
  vint->clear();
  int ii=0;

  if (!m_pXspl || !m_pYspl) {
    for (ii=0;ii<nint;ii++)
      vint->append(aux::Vector3(mk_dnan,mk_dnan));
    return -1;
  }
  double *tmp=new double[(size_t)nint];
  double *xint=new double[(size_t)nint];
  double *yint=new double[(size_t)nint];
  m_pXspl->interpol(nint,tmp,xint);
  m_pYspl->interpol(nint,tmp,yint);
  for (ii=0;ii<nint;ii++)
    vint->append(aux::Vector3(xint[ii],yint[ii]));
  delete [] tmp;
  delete [] xint;
  delete [] yint;
  return 0;

}

double SplineP::interp(double) const {

  return .0;

}

Polynomial::Polynomial(aux::TVList<aux::Asciistr> *optL) :
  Interpolation("polynomial",optL),m_c(0),m_d(0) {

}

Polynomial::~Polynomial() {

  int ii=0;
  if (m_c) {
    for (ii=0;ii<m_nctrl;ii++)
      delete [] m_c[ii];
    delete [] m_c;
    m_c=0;
  }
  if (m_d) {
    for (ii=0;ii<m_nctrl;ii++)
      delete [] m_d[ii];
    delete [] m_d;
    m_d=0;
  }

}

int Polynomial::invalidate() {

  int ii=0;
  if (m_c) {
    for (ii=0;ii<m_nctrl;ii++)
      delete [] m_c[ii];
    delete [] m_c;
    m_c=0;
  }
  if (m_d) {
    for (ii=0;ii<m_nctrl;ii++)
      delete [] m_d[ii];
    delete [] m_d;
    m_d=0;
  }

  return 0;

}

int Polynomial::setCtrl(int nctrl,double *x,double *y,double *z) {

  if (Interpolation::setCtrl(nctrl,x,y,z)!=0)
    return -1;

  aux::Asciistr opteq("eq"),optregr("regr");
  int ii=0;
  if (m_options.find(opteq)>=0) {
    m_coeff.clear();
    if (m_coeff.size()<m_nctrl)
      m_coeff.resize(m_nctrl);
    for (ii=0;ii<m_nctrl;ii++)
      m_coeff.append(m_x[ii]);
  }
  else if (m_options.find(optregr)>=0) {
    int regrdeg=0;
    for (ii=0;ii<m_options.count();ii++) {
      regrdeg=(int)mk_a2i((const char *)m_options[ii],0,0);
      if (regrdeg>0)
        return fitRegr(regrdeg);
    }
  }

  return 0;

}

int Polynomial::setCtrl(aux::TVList<aux::Vector3> *v) {

  if (Interpolation::setCtrl(v)!=0)
    return -1;

  aux::Asciistr opteq("eq"),optregr("regr");
  int ii=0;
  if (m_options.find(opteq)>=0) {
    m_coeff.clear();
    if (m_coeff.size()<m_nctrl)
      m_coeff.resize(m_nctrl);
    for (ii=0;ii<m_nctrl;ii++)
      m_coeff.append(m_x[ii]);
  }
  else if (m_options.find(optregr)>=0) {
    int regrdeg=0;
    for (ii=0;ii<m_options.count();ii++) {
      regrdeg=(int)mk_a2i((const char *)m_options[ii],0,0);
      if (regrdeg>0)
        return fitRegr(regrdeg);
    }
  }

  return 0;

}

int Polynomial::fitRegr(int regrdeg) {

  m_coeff.clear();
  if (m_nctrl==0 || !m_y || regrdeg<=0 || regrdeg>=m_nctrl)
    return 0;

  int ii=0,jj=0,kk=0;
  double **xxpow=new double*[m_nctrl];
  for (ii=0;ii<m_nctrl;ii++) {
    xxpow[ii]=new double[2*(regrdeg+1)];
	  xxpow[ii][0]=1.;
	  for (jj=1;jj<2*(regrdeg+1);jj++)
	    xxpow[ii][jj]=xxpow[ii][jj-1]*m_x[ii];
	}
  double *rs=new double[regrdeg+1];
	for (ii=0;ii<=regrdeg;ii++) {
	  for (jj=0;jj<m_nctrl;jj++)
		  rs[ii]+=xxpow[jj][ii]*m_y[jj];
	}
  num::SquareMatrix mm(regrdeg+1);
  double **rr=mm.data();
	for (ii=0;ii<=regrdeg;ii++) {
    for (jj=0;jj<=regrdeg;jj++) {
      if (ii==jj)
        rr[ii][jj]=.0;
	    for (kk=0;kk<m_nctrl;kk++)
	      rr[ii][jj]+=xxpow[kk][ii+jj];
	  }
	}
  //aux::Asciistr str;
  //mm.toString(&str);
	//printf ("%d mm=\n%s\n",__LINE__,(const char *)str);
  for (ii=0;ii<m_nctrl;ii++)
    delete [] xxpow[ii];
  delete [] xxpow;
  double *coeff=new double[regrdeg+1];
  memset(&coeff[0],0,(regrdeg+1)*sizeof(double));
  mm.invalidate();
  int res=mm.solveFor(regrdeg+1,rs,coeff);
  delete [] rs;
  if (res<0) {
    delete [] coeff;
    return 0;
  }
  if (m_coeff.size()<regrdeg+2)
    m_coeff.resize(regrdeg+2);
  for (ii=0;ii<=regrdeg;ii++)
    m_coeff.append(coeff[ii]);
  m_coeff.append(.0);
  delete [] coeff;
  double ymean=.0,dym=.0,dyf=.0,tmp=.0;
  for (ii=0;ii<m_nctrl;ii++)
    ymean+=m_y[ii];
  ymean/=(double)m_nctrl;
  for (ii=0;ii<m_nctrl;ii++) {
    tmp=m_y[ii]-ymean;
    dym+=(tmp*tmp);
    tmp=m_y[ii]-interp(m_x[ii]);
    dyf+=(tmp*tmp);
  }
  m_coeff.replace(m_coeff.count()-1,sqrt((dym-dyf)/dym));

  return (m_coeff.count()-1);

}

int Polynomial::prepTable() {

  invalidate();
  if (m_nctrl==0)
    return -1;

  aux::Asciistr opteq("eq"),optregr("regr");

  if (m_options.find(opteq)>=0 ||
      (m_options.find(optregr)>=0 && m_coeff.count()>0))
    return -1;

  int ii=0,jj=m_nctrl,kk=0;
  m_c=new double*[(size_t)m_nctrl];
  m_d=new double*[(size_t)m_nctrl];
  for (ii=0;ii<m_nctrl;ii++) {
    m_c[ii]=new double[(size_t)jj];
    m_d[ii]=new double[(size_t)jj];
    m_c[0][ii]=m_d[0][ii]=m_y[ii];
    if (ii>0) {
      for (kk=0;kk<jj;kk++)
        m_c[ii][kk]=m_d[ii][kk]=0.0;
    }
    jj--;
  }

  return 0;

}

int Polynomial::interpol(int nint,double *xint,double *yint,double *zint,double start,double end) {

  if (!xint || !yint || nint<=0 || m_nctrl==0)
    return -1;

  aux::Asciistr opteq("eq"),optregr("regr");
  int ii=0,idxeq=m_options.find(opteq),idxregr=m_options.find(optregr);
  if (mk_isNan(start))
    start=(idxeq>=0 ? .0 : m_x[0]);
  if (mk_isNan(end))
    end=(idxeq>=0 ? 1. : m_x[m_nctrl-1]);
  double interval=(end-start)/(double)(nint-1);

  if (m_coeff.count()>0 && (idxeq>=0 || idxregr>=0)) {
    for (ii=0;ii<nint;ii++) {
      xint[ii]=start;
      yint[ii]=interp(start);
      if (zint)
        zint[ii]=(m_z ? m_z[ii*m_nctrl/nint] : mk_dnan);
      start+=interval;
    }
    return 0;
  }

  if (nint<m_nctrl) {
    for (ii=0;ii<nint;ii++) {
      if (xint)
        xint[ii]=mk_dnan;
      if (yint)
        yint[ii]=mk_dnan;
      if (zint)
        zint[ii]=mk_dnan;
    }
    return -1;
  }
  prepTable();

  int idx=0;
  for (ii=0;ii<m_nctrl;ii++) {
    if (start>=m_x[ii])
      idx=ii;
  }

  yint[0]=m_y[idx++];
  for (ii=1;ii<nint;ii++) {
    start+=interval;
    if (start>=m_x[idx]) {
      xint[ii]=m_x[idx];
      yint[ii]=m_y[idx];
      idx++;
      if (idx>=m_nctrl)
        idx=m_nctrl-1;
    }
    else {
      xint[ii]=start;
      yint[ii]=interp(start);
    }
    if (zint)
      zint[ii]=(m_z ? m_z[ii*m_nctrl/nint] : mk_dnan);
  }

  return 0;

}

int Polynomial::interpol(int nint,aux::TVList<aux::Vector3> *vint,double start,double end) {

  if (nint<=0 || !vint)
    return -1;
  vint->clear();
  if (vint->size()<nint)
    vint->resize(nint);

  aux::Asciistr opteq("eq"),optregr("regr");
  int ii=0,idxeq=m_options.find(opteq),idxregr=m_options.find(optregr);
  if (mk_isNan(start))
    start=(idxeq>=0 ? .0 : m_x[0]);
  if (mk_isNan(end))
    end=(idxeq>=0 ? 1. : m_x[m_nctrl-1]);
  double interval=(end-start)/(double)(nint-1);

  if (m_coeff.count()>0 && (idxeq>=0 || idxregr>=0)) {
    for (ii=0;ii<nint;ii++) {
      vint->append(aux::Vector3(start,interp(start)));
      start+=interval;
    }
    return 0;
  }

  if (nint<m_nctrl) {
    for (ii=0;ii<nint;ii++)
      vint->append(aux::Vector3(mk_dnan,mk_dnan));
    return -1;
  }
  prepTable();

  int idx=0;
  for (ii=0;ii<m_nctrl;ii++) {
    if (start>=m_x[ii])
      idx=ii;
  }
  vint->append(aux::Vector3(start,m_y[idx++]));
  for (ii=1;ii<nint;ii++) {
    start+=interval;
    if (start>=m_x[idx]) {
      vint->append(aux::Vector3(m_x[idx],m_y[idx]));
      idx++;
      if (idx>=m_nctrl)
        idx=m_nctrl-1;
    }
    else
      vint->append(aux::Vector3(start,interp(start)));
  }

  return 0;

}

double Polynomial::interp(double x) const {

  aux::Asciistr opteq("eq"),optregr("regr");
  int ii=0,jj=0,idx=0,idxeq=m_options.find(opteq),idxregr=m_options.find(optregr);
  double res=.0,tmp1=1.;

  if (m_coeff.count()>0 && (idxeq>=0 || idxregr>=0)) {
    int ncoeff=m_coeff.count();
    if (idxregr>=0)
      ncoeff--;
    for (ii=0;ii<ncoeff;ii++) {
      res+=m_coeff[ii]*tmp1;
      tmp1*=x;
    }
    return res;
  }

  if (m_nctrl==0 || !m_y || !m_c || !m_d)
    return mk_dnan;

  double tmp2=.0,tmp3=.0,tmp4=.0,tmp5=fabs(m_x[0]-x),tmp6=.0;
  for (ii=1;ii<m_nctrl;ii++) {
    tmp6=fabs(m_x[ii]-x);
    if (tmp6<tmp5) {
      tmp5=tmp6;
      idx=ii;
    }
    for (jj=0;jj<(m_nctrl-ii);jj++) {
      tmp1=m_x[jj]-x;
      tmp2=m_x[jj+ii]-x;
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
  bool down=(idx==0 ? true : (m_nctrl/idx>=2 ? true : false));
  //bool down=(idx>=idm ? false : true);
  for (ii=1;ii<m_nctrl;ii++) {
    if (down) {
      if (idx<1)
        down=true;
      else
        down=false;
      res+=m_c[ii][idx];
    }
    else {
      res+=m_d[ii][--idx];
      if (idx<1 || idx<(m_nctrl-ii-1))
        down=true;
      else
        down=false;
    }
    //idm=(m_nctrl-i)/2;
    //down=(idx>idm ? false : true);
  }
  return res;

}

int Polynomial::coeff(double,aux::TVList<double> *c) {

  if (!c)
    return -1;
  c->clear();
  aux::Asciistr optctrl("ctrl"),opteq("eq"),optregr("regr");
  int ii=0,jj=0,kk=0,ll=0,idx=0,idxeq=m_options.find(opteq),idxregr=m_options.find(optregr);

  if (m_coeff.count()>0 && (idxeq>=0 || idxregr>=0)) {
    if (c->size()<m_coeff.count())
      c->resize(m_coeff.count());
    for (ii=0;ii<m_coeff.count();ii++)
      c->append(m_coeff[ii]);
    return m_coeff.count();
  }

  if (!m_y)
    return 0;
  if (m_coeff.size()<m_nctrl)
    m_coeff.resize(m_nctrl);
  aux::TVList<aux::Asciistr> optL(1);
  optL.inSort(optctrl);
  Polynomial p(&optL);
  p.setCtrl(m_nctrl,m_x,m_y);
  p.prepTable();
  double min=mk_dLimit,ctmp=.0;
  for (ii=0;ii<m_nctrl;ii++) {
    ctmp=p.interp(.0);
    m_coeff.append(ctmp);
    min=mk_dLimit;
    kk=0;
    for (jj=0;jj<m_nctrl-ii;jj++) {
      if (fabs(p.m_x[jj])<min) {
        min=fabs(p.m_x[jj]);
        kk=jj;
      }
      p.m_y[jj]=(p.m_y[jj]-ctmp)/p.m_x[jj];
    }
    for (jj=kk+1;jj<m_nctrl-ii;jj++) {
      p.m_x[jj-1]=p.m_x[jj];
      p.m_y[jj-1]=p.m_y[jj];
    }
    p.m_nctrl--;
    //for (j=0;j<p.m_nctrl;j++) printf ("j=%d x=%f y=%f c=%f\n",j,p.m_x[j],p.m_y[j],c[i]);
    //p.clearTable();
    jj=p.m_nctrl;
    for (kk=0;kk<p.m_nctrl;kk++) {
      p.m_c[0][kk]=p.m_d[0][kk]=p.m_y[kk];
      if (kk>0) {
        for (ll=0;ll<jj;ll++)
          p.m_c[kk][ll]=p.m_d[kk][ll]=.0;
      }
      jj--;
    }
  }
  if (c->size()<m_coeff.count())
    c->resize(m_coeff.count());
  for (ii=0;ii<m_coeff.count();ii++)
    c->append(m_coeff[ii]);

  p.m_nctrl=m_nctrl;

  return m_coeff.count();

}

int Polynomial::rootsBrute(double *roots,double min,double max,int *effdeg) {

  if (!m_x)
    return 0;

  aux::Asciistr opteq("eq");
  if (m_options.find(opteq)<0)
    return 0;

  int ii=0,jj=0,kk=0,degree=m_nctrl-1;
  for ( ii = m_nctrl - 1; ii>-1; ii-- )  {
    if ( m_x[ii]==.0 ) {
      degree=ii;
      break;
    }
  }
  if (effdeg)
    *effdeg=degree;

  if (degree==0) {
    if (m_x[0]==.0) {
      roots[0]=.0;
      return 1;
    }
    return 0;
  }
  if (degree==1) {
    roots[0]=-m_x[0]/m_x[1];
    return degree;
  }
  if (degree==2) {
    double pp2=-m_x[1]/(2.*m_x[2]),pqres=pp2*pp2-m_x[0]/m_x[2];
    if (pqres<0)
      return 0;
    double rpqres=sqrt(pqres);
    roots[0]=pp2+rpqres;
    roots[1]=pp2-rpqres;
    if (roots[1]<roots[0])
      aux::swap(&roots[0],&roots[1]);
    return degree;
  }

  double res=.0,lastres=.0,minlres=mk_dnan,minhres=mk_dnan,minleval=mk_dnan,minheval=mk_dnan,
         tmp=1.,intv=(max-min)/1000.,eval=min,lasteval=eval;

  for (ii=0;ii<1000;ii++) {
    res=.0;
    tmp=1.;
    eval=min+(double)ii*intv;
    for ( jj = 0; jj <=degree;jj++ ) {
      res += m_x[jj] * tmp;
      tmp *= eval;
    }
    if (res==.0 || (res>.0 && lastres<.0) || (lastres>.0 && res<.0)) {
      roots[kk]=(res==.0 ? eval : eval/2.+lasteval/2.);
      if (kk++==degree)
        break;
    }
    if (res<.0 && (mk_isBusted(minlres)!=0 || res>minlres)) {
      minlres=res;
      minleval=eval;
    }
    else if (res>.0 && (mk_isBusted(minhres)!=0 || res<minhres)) {
      minhres=res;
      minheval=eval;
    }
    lasteval=eval;
    lastres=res;
  }
  if (kk==0) {
    if (mk_isBusted(minlres)==0) {
      if (mk_isBusted(minhres)==0)
        roots[kk++]=(minhres<fabs(minlres) ? minheval : minleval);
      else
        roots[kk++]=minleval;
    }
    else if (mk_isBusted(minhres)==0)
      roots[kk++]=minheval;
  }
  if (kk>1)
    aux::heapsortv(kk, roots);
  return kk;

}

Bezier::Bezier(int nctrl,double *x,double *y) : Interpolation("bezier") {

  setArr(nctrl,x,y,0);

}

Bezier::~Bezier() {

}

int Bezier::interpol(int nint,double *xint,double *yint, double *zint,double,double) {

  if (m_nctrl<=0 || nint<=1 || !m_x || !m_y)
    return -1;
  int penctrl=m_nctrl-1,penint=nint-1,ii=0,jj=0;
  double t1=.0,t2=1.,bino=1.;
  double *fac1=new double[(size_t)m_nctrl];
  double *fac2=new double[(size_t)m_nctrl];
  for (jj=0;jj<m_nctrl;jj++)
    fac1[jj]=fac2[jj]=1.0;
  for (ii=0;ii<nint;ii++) {
    xint[ii]=yint[ii]=.0;
    t1=(double)ii/(double)penint;
    t2=1.-t1;
    for (jj=1;jj<m_nctrl;jj++) {
      fac1[jj]=fac1[jj-1]*t1;
      fac2[penctrl-jj]=fac2[penctrl-jj+1]*t2;
    }
    for (jj=0;jj<m_nctrl;jj++) {
      bino=mk_binomialCoeff(penctrl,jj);
      xint[ii]+=m_x[jj]*bino*fac1[jj]*fac2[jj];
      yint[ii]+=m_y[jj]*bino*fac1[jj]*fac2[jj];
    }
    for (jj=0;jj<m_nctrl;jj++)
      fac1[jj]=fac2[jj]=1.;
    if (zint)
      zint[ii]=(m_z ? m_z[ii*m_nctrl/nint] : mk_dnan);
  }
  delete [] fac1;
  delete [] fac2;
  return 0;

}

int Bezier::interpol(int nint,aux::TVList<aux::Vector3> *vint,double,double) {

  if (m_nctrl<=0 || nint<=1 || !m_x || !m_y || !vint)
    return -1;
  if (vint->size()<nint)
    vint->resize(nint);
  vint->clear();
  int penctrl=m_nctrl-1,penint=nint-1,ii=0,jj=0;
  double t1=.0,t2=1.,bino=1.,tmpx=.0,tmpy=.0;
  double *fac1=new double[(size_t)m_nctrl];
  double *fac2=new double[(size_t)m_nctrl];
  for (jj=0;jj<m_nctrl;jj++)
    fac1[jj]=fac2[jj]=1.;
  for (ii=0;ii<nint;ii++) {
    tmpx=tmpy=.0;
    t1=(double)ii/(double)penint;
    t2=1.-t1;
    for (jj=1;jj<m_nctrl;jj++) {
      fac1[jj]=fac1[jj-1]*t1;
      fac2[penctrl-jj]=fac2[penctrl-jj+1]*t2;
    }
    for (jj=0;jj<m_nctrl;jj++) {
      bino=mk_binomialCoeff(penctrl,jj);
      tmpx+=m_x[jj]*bino*fac1[jj]*fac2[jj];
      tmpy+=m_y[jj]*bino*fac1[jj]*fac2[jj];
    }
    for (jj=0;jj<m_nctrl;jj++)
      fac1[jj]=fac2[jj]=1.0;
    vint->append(aux::Vector3(tmpx,tmpy));
  }
  delete [] fac1;
  delete [] fac2;
  return 0;

}

} // namespace
