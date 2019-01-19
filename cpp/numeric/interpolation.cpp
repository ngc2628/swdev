
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <auxx/auxx.h>
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

aux::Asciistr interpolation2string(mk_ulreal option) {

  aux::Asciistr res("none");
  if (option==interpolation_none)
    return res;
  mk_ulreal ii=0,jj=0;
  for (ii=1,jj=1;jj<(numinterpolationtypes+numinerpolationoptions);ii*=2,jj++) {
    if ((option&ii)>0) {
      res=(jj<numinterpolationtypes ? interpolationtypes[jj] : interpolationoptions[jj]);
      break;
    }
  }
  return res;

}

Interpolation::Interpolation(mk_ulreal options) : 
  m_options(options),m_ready(0) {

}

Interpolation::~Interpolation() {

  clearArr();

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

int Interpolation::setCtrl(VertexList *ctrlL) {

  clearCtrl();
  return setArr(ctrlL);

}

int Interpolation::nctrl() const {

  return m_ctrlL.count();

}

int Interpolation::setup() {

  return 0;

}

int Interpolation::interpol(int,VertexList *,double,double) {

  return 0;

}

int Interpolation::interp(Vertex *) const {

  return 0;

}

int Interpolation::extrap(Vertex *) const {

  return 0;

}

int Interpolation::coeff(double,VertexList *) {
  
  return 0;

}

mk_ulreal Interpolation::setOptions(mk_ulreal options) {

  m_options=(options&(interpolation_type|interpolation_options));
  return m_options;

}

void Interpolation::clearArr() {

  m_ctrlL.clear();
  m_ready=0;

}

int Interpolation::setArr(VertexList *ctrlL) {

  clearArr();
  int ii=0,cnt=(ctrlL ? ctrlL->count() : 0);
  m_ctrlL.resize(cnt);
  for (ii=0;ii<cnt;ii++)
    m_ctrlL.append(ctrlL->get(ii));
  return 0;

}

InterpolationConst::InterpolationConst(VertexList *ctrlL) : 
  Interpolation(interpolation_const) {

  setArr(ctrlL);

}

InterpolationConst::~InterpolationConst() {

}

int InterpolationConst::interpol(int nint,VertexList *vint,double,double) {

  int ii=0,jj=0,kk=0,fidx=1,nctrl=m_ctrlL.count(),
    nintmin=2*nctrl-1,fillcnt=(nint-nintmin)/(nctrl-1);
  if (nint<=0 || nint<nctrl || nctrl==0 || !vint)
    return 1;
  vint->clear();
  vint->resize(nint);
  if (nint<nintmin) {
    for (ii=0;ii<nctrl;ii++)
      vint->append(m_ctrlL[ii]);
    return 0;
  }
  Vertex vv,vidx;
  double xl=.0,xh=.0;
  vint->append(m_ctrlL[0]);
  if ((m_options&interpolation_bwd)>0)
    fidx=0;
  for (ii=1;ii<nctrl;ii++) {
    xl=m_ctrlL[ii-1].x();
    xh=m_ctrlL[ii].x();
    // next -> last
    vidx=m_ctrlL[ii-fidx];
    vv.setXYZ(xl,vidx.y(),vidx.z());
    vint->append(vv);
    for (jj=0;jj<fillcnt;jj++) {
      vv.setXYZ(xl+(double)(jj+1)*(xh-xl)/(double)fillcnt,vidx.y(),vidx.z());
      vint->append(vv);
    }
    vv.setXYZ(xh,vidx.y(),vidx.z());
    vint->append(vv);
  }
  return 0;

}

int InterpolationConst::interp(Vertex *vv) const {

  int ii=0,nctrl=m_ctrlL.count();
  if (!vv || nctrl==0) {
    if (vv)
      vv->setY(mk_dnan);
    return 1;
  }
  for (ii=1;ii<nctrl;ii++) {
    if (m_ctrlL[ii].x()>vv->x()) {
      if ((m_options&interpolation_bwd)>0)
        vv->setY(m_ctrlL[ii].y());
      else
        vv->setY(m_ctrlL[ii-1].y());
      return 0;
    }
  }
  vv->setY(m_ctrlL[nctrl-1].y());
  return 1;

}

InterpolationLinear::InterpolationLinear(VertexList *ctrlL) : 
  Interpolation(interpolation_linear) {

  setArr(ctrlL);

}

InterpolationLinear::~InterpolationLinear() {

}

int InterpolationLinear::interpol(int nint,VertexList *vint,double,double) {

  int ii=0,jj=0,nctrl=m_ctrlL.count();
  if (nint<=0 || nctrl==0 || nint<nctrl || !vint)
    return 1;
  vint->clear();
  vint->resize(nint);
  Vertex pl,ph,vv(m_ctrlL[0]);
  Vertex *vidx=0;
  if (nctrl==1) {
    for (ii=0;ii<nint;ii++) 
      vint->append(vv);
    return 0;
  }
  int chidx=(nint-nctrl)/(nctrl-1);
  double tmp=.0;
  vint->append(vv);
  for (ii=1;ii<nctrl;ii++) {
    if (vint->count()>=nint)
      break;
    pl=m_ctrlL[ii-1];
    ph=m_ctrlL[ii];
    for (jj=0;jj<chidx;jj++) {
      tmp=pl.x()+(double)(jj+1)*(ph.x()-pl.x())/(double)(chidx+1);
      vv.setXYZ(tmp,mk_lineq(pl.data(),ph.data(),tmp),m_ctrlL[ii*nctrl/nint].z());
      vint->append(vv);
    }
    vv.setXYZ(ph.x(),ph.y(),m_ctrlL[ii].z());
    vint->append(vv);
  }
  vv=m_ctrlL[nctrl-1];
  for (ii=vint->count();ii<nint;ii++)
    vint->append(vv);
  return 0;

}

int InterpolationLinear::interp(Vertex *vv) const {

  int ii=0,nctrl=m_ctrlL.count();
  if (!vv || nctrl<=0) {
    if (vv)
      vv->setY(mk_dnan);
    return 1;
  }
  for (ii=1;ii<nctrl;ii++) {
    if (m_ctrlL[ii].x()>vv->x()) {
      vv->setY((vv->x()-m_ctrlL[ii-1].x())*(m_ctrlL[ii].y()-m_ctrlL[ii-1].y())/
               (m_ctrlL[ii].x()-m_ctrlL[ii-1].x()));
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

  m_ctrlL.sort(0);
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

  int res=1,nctrl=m_ctrlL.count();
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

  int nctrl=m_ctrlL.count();
  if (!der || nctrl==0)
    return 1;
  if (m_der)
    delete [] m_der;
  m_der=new double[nctrl];
  memcpy(&m_der[0],&der[0],nctrl*sizeof(double));
  return 0;

}

int CubicSpline::interpol(int nint,VertexList *vint,double from,double to) {

  if (!vint || nint<=0)
    return 1;
  int ii=0,jj=0,nctrl=m_ctrlL.count();
  vint->clear();
  vint->resize(nint);
  if (!m_der)
    return 1;
  Vertex vv;
  if (nint==nctrl) {
    for (ii=0;ii<nint;ii++) {
      vv=m_ctrlL[ii];
      vint->append(vv);
    }
    return 0;
  }
  if (mk_isnan(from))
    from=m_ctrlL[0].x();
  if (mk_isnan(to))
    to=m_ctrlL[nctrl-1].x();
  int startidx=0,endidx=nctrl-1;
  for (ii=0;ii<nctrl;ii++) {
    if (from>=m_ctrlL[ii].x())
      startidx=ii;
    if (to<=m_ctrlL[ii].x())
      endidx=ii;
  }
  int nnint=nint-(endidx-startidx+1)+2;
  double interval=(m_ctrlL[endidx].x()-m_ctrlL[startidx].x())/(double)(nnint-1);
  double xx=m_ctrlL[startidx].x();
  vv.setX(xx);
  vint->append(vv);
  for (ii=1;ii<nnint-1;ii++) {
    xx+=interval;
    vv.setX(xx);
    vint->append(vv);
  }
  for (ii=startidx+1;ii<=endidx;ii++) {
    vv.setX(m_ctrlL[ii].x());
    vint->append(vv);
  }

  vint->sort(0);

  Vertex vidx;
  for (ii=0;ii<nint;ii++) {
    vidx=vint->get(ii);
    vidx.setZ(m_ctrlL[ii*(endidx-startidx+1)/nint].z());
    if (interp(&vidx)!=0)
      vidx.setY(.0);
    vint->set(ii,vidx,0);
  }
  return 0;

}

int CubicSpline::interp(Vertex *vv) const {

  if (!vv || !m_der)
    return 1;
  int nctrl=m_ctrlL.count(),idxh=-1,idxl=nctrl;
  if (m_ctrlL.findInterval(*vv,&idxl,&idxh)==1 || idxh<=idxl)
    return 1;
  double res=.0,aa=.0,bb=.0,
    hx=m_ctrlL[idxh].x()-m_ctrlL[idxl].x(),hy=m_ctrlL[idxh].y()-m_ctrlL[idxl].y();
  if(hx==.0) {
    return 1;
  }
  if ((m_options&interpolation_solve1st)>0) {
    double eval=(vv->x()-m_ctrlL[idxl].x());
    aa=m_ctrlL[idxl].y();
    bb=m_der[idxl];
    double dd=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    double cc=(hy/hx-m_der[idxl])/hx-hx*dd;
    res=aa+bb*eval+cc*eval*eval+dd*eval*eval*eval;
  }
  else if ((m_options&interpolation_solve2nd)>0) {
    aa=(m_ctrlL[idxh].x()-vv->x())/hx;
    bb=(vv->x()-m_ctrlL[idxl].x())/hx;
    res=aa*m_ctrlL[idxl].y()+
        bb*m_ctrlL[idxh].y()+
        (aa*aa*aa-aa)*hx*hx*m_der[idxl]/6.0+
        (bb*bb*bb-bb)*hx*hx*m_der[idxh]/6.0;
  }
  vv->setY(res);
  return 0;

}

int CubicSpline::extrap(Vertex *vv) const {

  if (!vv || !m_der)
    return 1;

  int nctrl=m_ctrlL.count();

  if (vv->x()>=m_ctrlL[0].x() && vv->x()<=m_ctrlL[nctrl-1].x())
    return interp(vv);

  int idxl=(vv->x()<m_ctrlL[0].x() ? 0 : nctrl-2),
      idxh=(vv->x()<m_ctrlL[0].x() ? 1 : nctrl-1);

  double hx=m_ctrlL[idxh].x()-m_ctrlL[idxl].x(),hy=m_ctrlL[idxh].y()-m_ctrlL[idxl].y();
  if (hx==.0) {
    return 1;
  }

  double c0=.0,c1=.0,c2=.0,c3=.0;

  if ((m_options&interpolation_solve1st)>0) {
    c0=m_ctrlL[idxl].y();
    c1=m_der[idxl];
    c3=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    c2=(hy/hx-m_der[idxl])/hx-hx*c3;
  }
  else if ((m_options&interpolation_solve2nd)>0) {
    double dx=-hx,dx6=6.*dx;
    c0=(6.*m_ctrlL[idxl].x()*m_ctrlL[idxh].y()+
         m_ctrlL[idxh].x()*(-6.*m_ctrlL[idxl].y()+m_ctrlL[idxl].x()*
         (-m_ctrlL[idxh].x()*(2.*m_der[idxl]+m_der[idxh])+m_ctrlL[idxl].x()*
         (m_der[idxl]+2.*m_der[idxh]))))/dx6;
    c1=(6.*(m_ctrlL[idxl].y()-m_ctrlL[idxh].y())+
          2.*m_ctrlL[idxl].x()*m_ctrlL[idxh].x()*(m_der[idxl]-m_der[idxh])+
          m_ctrlL[idxh].x()*m_ctrlL[idxh].x()*(2.*m_der[idxl]+m_der[idxh])-
          m_ctrlL[idxl].x()*m_ctrlL[idxl].x()*(m_der[idxl]+2*m_der[idxh]))/dx6;
    c2=(m_ctrlL[idxl].x()*m_der[idxh]-m_ctrlL[idxh].x()*m_der[idxl])/(2.*dx);
    c3=(m_der[idxl]-m_der[idxh])/dx6;
  }

  double res=(c0+vv->x()*(c1+vv->x()*(c2+vv->x()*c3)));
  vv->setY(res);
  return 0;

}

int CubicSpline::coeff(double xx,VertexList *coeffL) {

  if (mk_isbusted(xx)!=0 || !coeffL || !m_der)
    return 1;
  int nctrl=m_ctrlL.count(),idxh=-1,idxl=nctrl;
  Vertex vv(xx),vvidx;
  if (m_ctrlL.findInterval(vv,&idxl,&idxh)==1 || idxh<=idxl)
    return 1;
  double hx=m_ctrlL[idxh].x()-m_ctrlL[idxl].x(),hy=m_ctrlL[idxh].y()-m_ctrlL[idxl].y(),tmp=.0;
  if (hx==.0)
    return 1;
  coeffL->clear();
  if ((m_options&interpolation_solve1st)>0) {
    vvidx.setX(m_ctrlL[idxl].y());
    coeffL->append(vvidx);
    vvidx.setX(m_der[idxl]);
    coeffL->append(vvidx);
    tmp=(m_der[idxh]+m_der[idxl]-2.*hy/hx)/(hx*hx);
    vvidx.setX((hy/hx-m_der[idxl])/hx-hx*tmp);
    coeffL->append(vvidx);
    vvidx.setX(tmp);
    coeffL->append(vvidx);
    return 0;
  }
  else if ((m_options&interpolation_solve2nd)>0) {
    double dx=-hx,dx6=6.*dx;
    vvidx.setX((6.*m_ctrlL[idxl].x()*m_ctrlL[idxh].y()+m_ctrlL[idxh].x()*
      (-6.*m_ctrlL[idxl].y()+m_ctrlL[idxl].x()*(-m_ctrlL[idxh].x()*(2.*m_der[idxl]+m_der[idxh])+
      m_ctrlL[idxl].x()*(m_der[idxl]+2.*m_der[idxh]))))/dx6);
    coeffL->append(vvidx);
    vvidx.setX((6.*(m_ctrlL[idxl].y()-m_ctrlL[idxh].y())+
               2.*m_ctrlL[idxl].x()*m_ctrlL[idxh].x()*(m_der[idxl]-m_der[idxh])+
               m_ctrlL[idxh].x()*m_ctrlL[idxh].x()*(2.*m_der[idxl]+m_der[idxh])-
               m_ctrlL[idxl].x()*m_ctrlL[idxl].x()*(m_der[idxl]+2*m_der[idxh]))/dx6);
    coeffL->append(vvidx);
    vvidx.setX((m_ctrlL[idxl].x()*m_der[idxh]-m_ctrlL[idxh].x()*m_der[idxl])/(2.*dx));
    coeffL->append(vvidx);
    vvidx.setX((m_der[idxl]-m_der[idxh])/dx6);
    coeffL->append(vvidx);
    return 0;
  }
  return 1;

}

int CubicSpline::makeSpline1st() {

  invalidate();
  int ii=0,nctrl=m_ctrlL.count(),num=nctrl,pennum=num-1;
  if (nctrl<=1)
    return 1;

  double *hx=new double[(size_t)pennum];
  double *hy=new double[(size_t)pennum];
  double *ss=new double[(size_t)pennum];
  for (ii=0;ii<pennum;ii++) {
    hx[ii]=m_ctrlL[ii+1].x()-m_ctrlL[ii].x();
    hy[ii]=m_ctrlL[ii+1].y()-m_ctrlL[ii].y();
    if (hx[ii]==.0) {
      delete [] hx;
      delete [] hy;
      delete [] ss;
      return -1;
    }
    ss[ii]=hy[ii]/hx[ii];
  }

  SquareMatrix mat(num);
  double **mm=mat.data();
  double *rr=new double[(size_t)num];
  rr[0]=rr[pennum]=.0;
  for (ii=1;ii<pennum;ii++)
    rr[ii]=3.*hx[ii-1]*ss[ii]+3.*hx[ii]*ss[ii-1];

  if ((m_options&interpolation_notaknot)>0 && num>3) {
    rr[0]=ss[0]*hx[1]*(2.*hx[1]+3.*hx[0])+ss[1]*hx[0]*hx[0];
    rr[pennum]=-ss[num-3]*hx[num-2]*hx[num-2]-ss[num-2]*hx[num-3]*(3.*hx[num-2]+2.*hx[num-3]);
    double tmp=hx[0]+hx[1];
    mm[0][0]=hx[1]*tmp;
    mm[0][1]=tmp*tmp;
    tmp=hx[num-2]+hx[num-3];
    mm[pennum][pennum-1]=-tmp*tmp;
    mm[pennum][pennum]=-hx[num-3]*tmp;
  }
  else {
    if ((m_options&interpolation_der1st)>0) { // try der1st
      rr[0]=(nctrl>2 ? .0 : ss[0]);
      rr[pennum]=(nctrl>2 ? .0 : ss[0]);
      mm[0][0]=1.;
      mm[pennum][pennum]=1.;
    }
    else { // natural spline
      rr[0]=.0;
      rr[pennum]=.0;
      mm[0][0]=1.;
      mm[pennum][pennum]=1.;
    }
  }

  for (ii=1;ii<pennum;ii++) {
    mm[ii][ii-1]=hx[ii];
    mm[ii][ii]=2.*(hx[ii-1]+hx[ii]);
    mm[ii][ii+1]=hx[ii-1];
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
          corr=deri* MIN(fabsderi,fabs(3.*ss[ii-1]))/fabsderi;
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
  int ii=0,nctrl=m_ctrlL.count(),num=nctrl,pennum=num-1;
  if (nctrl<=1)
    return 1;

  double *hx=new double[(size_t)num];
  double *hy=new double[(size_t)num];
  hx[0]=.0;
  hy[0]=.0;
  for (ii=1;ii<num;ii++) {
    hx[ii]=m_ctrlL[ii].x()-m_ctrlL[ii-1].x();
    hy[ii]=m_ctrlL[ii].y()-m_ctrlL[ii-1].y();
    if (mk_deq(hx[ii],0.0)) {
      delete [] hx;
      delete [] hy;
      return 1;
    }
  }
  SquareMatrix mat(num);
  double **mm=mat.data();
  double *rr=new double[(size_t)num];
  rr[0]=rr[pennum]=0.0;
  for (ii=1;ii<pennum;ii++)
    rr[ii]=hy[ii+1]/hx[ii+1]-hy[ii]/hx[ii];

  if ((m_options&interpolation_notaknot)>0 && num>3) {
    mm[0][0]=hx[2];
    mm[0][1]=-hx[1]-hx[2];
    mm[0][2]=hx[1];
    mm[pennum][pennum-2]=hx[pennum]; 
    mm[pennum][pennum-1]=-hx[pennum]-hx[pennum-1]; 
    mm[pennum][pennum]=hx[pennum-1];
  }
  else if ((m_options&interpolation_periodic)>0) { // not implemented -> natural spline
    mm[0][0]=1.;
    mm[pennum][pennum]=1.;
  }
  else if ((m_options&interpolation_nat)>0) { // natural spline
    mm[0][0]=1.;
    mm[pennum][pennum]=1.;
  }
  else { // not implemented -> natural spline
    mm[0][0]=1.;
    mm[pennum][pennum]=1.;
  }

  for (ii=1;ii<pennum;ii++) {
    mm[ii][ii-1]=hx[ii]/6.0;
    mm[ii][ii]=(hx[ii+1]+hx[ii])/3.0;
    mm[ii][ii+1]=hx[ii+1]/6.0;
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

CubicSplineP::CubicSplineP(VertexList *ctrlL) :
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
  int ii=0,nctrl=m_ctrlL.count(),num=nctrl;
  if (nctrl<=1)
    return 1;
  Vertex vv(.0);
  Vertex vvc,vvs;
  double seg=mk_dnan;
  VertexList spline(nctrl);
  spline.append(vv);
  for (ii=1;ii<num;ii++) {
    vvs=spline[ii-1];
    vvc=m_ctrlL[ii];
    seg=vvs.x()+sqrt(vvc.x()*vvc.x()+vvc.y()*vvc.y());
    vv.setX(seg);
    spline.append(vv);
  }
  mk_ulreal optL=(interpolation_solve2nd|interpolation_nat);
  m_xspline=new CubicSpline(optL);
  m_yspline=new CubicSpline(optL);
  for (ii=0;ii<nctrl;ii++) {
    vvs=spline[ii];
    vvs.setY(m_ctrlL[ii].x());
  }
  int spl1=m_xspline->setCtrl(&spline);
  for (ii=0;ii<nctrl;ii++) {
    vvs=spline[ii];
    vvs.setY(m_ctrlL[ii].x());
    spline.set(ii,vv,0);
  }
  int spl2=m_yspline->setCtrl(&spline);
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

  if (!der1 || !der2 || m_ctrlL.count()==0 || !m_xspline || !m_yspline)
    return 1;
  return (m_xspline->setSpline(der1)==0 && m_yspline->setSpline(der2)==0 ? 0 : 1);

}

int CubicSplineP::interpol(int nint,VertexList *vint,double,double) {

  if (!vint)
    return 1;
  vint->clear();
  vint->resize(nint);
  int ii=0,nctrl=m_ctrlL.count();
  if (nint<=nctrl || !m_xspline || !m_yspline) {
    for (ii=0;ii<nint;ii++) 
      vint->append(m_ctrlL[ii]);
    return 1;
  }
  Vertex vv,vvint;
  VertexList tmp(nint);
  m_xspline->interpol(nint,&tmp);
  for (ii=0;ii<nint;ii++) {
    vv.setX(tmp[ii].y());
    vint->append(vv);
  }
  tmp.clear();
  m_yspline->interpol(nint,&tmp);
  for (ii=0;ii<nint;ii++) {
    vvint=vint->get(ii);
    vvint.setY(tmp[ii].y());
    vvint.setZ(m_ctrlL[ii*nctrl/nint].z()); 
    vint->set(ii,vvint,0);
  }
  return 0;

}

int CubicSplineP::interp(Vertex *) const {

  return 0;

}

Polynomial::Polynomial(mk_ulreal options) :
  Interpolation(interpolation_polynomial|options),m_c(0),m_d(0) {

}

Polynomial::~Polynomial() {

  int ii=0,nctrl=m_ctrlL.count();
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

  int ii=0,nctrl=m_ctrlL.count();
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

int Polynomial::setCtrl(VertexList *ctrlL) {

  if (Interpolation::setCtrl(ctrlL)!=0)
    return 1;
  int regrdeg=0;
  return fitRegr(regrdeg);
  return 0;

}

int Polynomial::fitRegr(int regrdeg) {

  m_coeffL.clear();
  int ii=0,jj=0,kk=0,nctrl=m_ctrlL.count();
  if (nctrl==0 || regrdeg<=0 || regrdeg>=nctrl)
    return 1;
  double **xxpow=new double*[nctrl];
  for (ii=0;ii<nctrl;ii++) {
    xxpow[ii]=new double[2*(regrdeg+1)];
	  xxpow[ii][0]=1.;
	  for (jj=1;jj<2*(regrdeg+1);jj++)
	    xxpow[ii][jj]=xxpow[ii][jj-1]*m_ctrlL[ii].x();
	}
  double *rs=new double[regrdeg+1];
	for (ii=0;ii<=regrdeg;ii++) {
	  for (jj=0;jj<nctrl;jj++)
		  rs[ii]+=xxpow[jj][ii]*m_ctrlL[jj].y();
	}
  num::SquareMatrix mm(regrdeg+1);
  double **rr=mm.data();
	for (ii=0;ii<=regrdeg;ii++) {
    for (jj=0;jj<=regrdeg;jj++) {
      if (ii==jj)
        rr[ii][jj]=.0;
	    for (kk=0;kk<nctrl;kk++)
	      rr[ii][jj]+=xxpow[kk][ii+jj];
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
  m_coeffL.resize(regrdeg+2);
  Vertex coeff;
  for (ii=0;ii<=regrdeg;ii++) {
    coeff.setY(coeffL[ii]);
    m_coeffL.append(coeff);
  }
  delete [] coeffL;
  coeff.setY(.0);
  m_coeffL.append(coeff);
  double ymean=.0,dym=.0,dyf=.0,tmp=.0;
  for (ii=0;ii<nctrl;ii++)
    ymean+=m_ctrlL[ii].y();
  ymean/=(double)nctrl;
  for (ii=0;ii<nctrl;ii++) {
    tmp=m_ctrlL[ii].y()-ymean;
    dym+=(tmp*tmp);
    coeff=m_ctrlL[ii];
    if (interp(&coeff)!=0)
      coeff.setY(.0);
    tmp=m_ctrlL[ii].y()-coeff.y();
    dyf+=(tmp*tmp);
  }
  coeff.setY(sqrt((dym-dyf)/dym));
  m_coeffL.set(m_coeffL.count()-1,coeff,0);  
  return (m_coeffL.count()-1);

}

int Polynomial::prepTable() {

  invalidate();
  int ii=0,nctrl=m_ctrlL.count(),jj=nctrl,kk=0;
  if (nctrl==0)
    return 1;
  if ((m_options&interpolation_eq)>0 || 
       ((m_options&interpolation_regr)>0 && m_coeffL.count()>0))
    return 1;
  m_c=new double*[(size_t)nctrl];
  m_d=new double*[(size_t)nctrl];
  for (ii=0;ii<nctrl;ii++) {
    m_c[ii]=new double[(size_t)jj];
    m_d[ii]=new double[(size_t)jj];
    m_c[0][ii]=m_d[0][ii]=m_ctrlL[ii].y();
    if (ii>0) {
      for (kk=0;kk<jj;kk++)
        m_c[ii][kk]=m_d[ii][kk]=0.0;
    }
    jj--;
  }
  return 0;

}

int Polynomial::interpol(int nint,VertexList *vint,double start,double end) {

  int ii=0,idx=0,nctrl=m_ctrlL.count();
  if (!vint || nint<=0 || nctrl==0)
    return 1;
  vint->clear();
  if (nint<nctrl)
    return 1;
  vint->resize(nint);
  Vertex vv,vidx;
  if (mk_isnan(start))
    start=((m_options&interpolation_eq)>0 ? .0 : m_ctrlL[0].x());
  if (mk_isnan(end))
    end=((m_options&interpolation_eq)>0 ? 1. : m_ctrlL[nctrl-1].x());
  double interval=(end-start)/(double)(nint-1);
  if (m_coeffL.count()>0 && 
       ((m_options&interpolation_eq)>0 || (m_options&interpolation_regr)>0)) {
    for (ii=0;ii<nint;ii++) {
      vidx.setX(start);
      if (interp(&vidx)!=0)
        vidx.setY(.0);
      vv.setXYZ(start,vidx.y(),m_ctrlL[ii*nctrl/nint].z());
      vint->append(vv);
      start+=interval;
    }
    return 0;
  }
  prepTable();
  for (ii=0;ii<nctrl;ii++) {
    if (start>=m_ctrlL[ii].x())
      idx=ii;
  }
  vint->append(m_ctrlL[0]);
  for (ii=1;ii<nint;ii++) {
    vidx=m_ctrlL[idx];
    start+=interval;
    if (start>=vidx.x()) {
      vv.setXYZ(vidx.x(),vidx.y(),m_ctrlL[ii*nctrl/nint].z());
      idx++;
      if (idx>=nctrl)
        idx=nctrl-1;
    }
    else {
      vidx.setX(start);
      if (interp(&vidx)!=0)
        vidx.setY(.0);
      vv.setXYZ(start,vidx.y(),m_ctrlL[ii*nctrl/nint].z());
    }
    vint->append(vv);
  }
  return 0;

}

int Polynomial::interp(Vertex *vv) const {

  if (!vv)
    return 1;
  int ii=0,jj=0,idx=0,nctrl=m_ctrlL.count();
  double res=.0,tmp1=1.,tmp2=mk_dnan,tmp3=mk_dnan,tmp4=mk_dnan,tmp5=mk_dnan,tmp6=mk_dnan;
  if (m_coeffL.count()>0 && 
       ((m_options&interpolation_eq)>0 || (m_options&interpolation_regr)>0)) {
    int ncoeff=m_coeffL.count();
    if ((m_options&interpolation_regr)>0)
      ncoeff--;
    for (ii=0;ii<ncoeff;ii++) {
      res+=m_coeffL[ii].x()*tmp1;
      tmp1*=vv->x();
    }
    vv->setY(res);
    return 0;
  }
  if (nctrl==0 || !m_c || !m_d)
    return mk_dnan;
  tmp5=fabs(m_ctrlL[0].x()-vv->x());
  for (ii=1;ii<nctrl;ii++) {
    tmp6=fabs(m_ctrlL[ii].x()-vv->x());
    if (tmp6<tmp5) {
      tmp5=tmp6;
      idx=ii;
    }
    for (jj=0;jj<(nctrl-ii);jj++) {
      tmp1=m_ctrlL[jj].x()-vv->x();
      tmp2=m_ctrlL[jj+ii].x()-vv->x();
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
  vv->setY(res);
  return 0;

}

int Polynomial::coeff(double,VertexList *cc) {

  if (!cc)
    return -1;
  cc->clear();
  int ii=0,jj=0,kk=0,ll=0,idx=0,nctrl=m_ctrlL.count(),ncoeff=m_coeffL.count();
  if (ncoeff>0 && ((m_options&interpolation_eq)>0 || (m_options&interpolation_regr)>0)) {
    cc->resize(ncoeff);
    for (ii=0;ii<ncoeff;ii++)
      cc->append(m_coeffL[ii]);
    return ncoeff;
  }
  cc->resize(nctrl);
  Polynomial pp(interpolation_ctrl);
  pp.setCtrl(&m_ctrlL);
  pp.prepTable();
  double min=mk_dnan,ctmp=.0;
  Vertex coeff,ctrl;
  for (ii=0;ii<nctrl;ii++) {
    coeff.setX(.0);
    if (pp.interp(&coeff)!=0)
      coeff.setY(.0);
    m_coeffL.append(coeff);
    min=mk_dnan;
    kk=0;
    for (jj=0;jj<nctrl-ii;jj++) {
      if (mk_isnan(min) || fabs(pp.m_ctrlL[jj].x())<min) {
        min=fabs(pp.m_ctrlL[jj].x());
        kk=jj;
      }
      // y=f(x)=a0+a1+x+a2*x*x+a3*x*x*x+a4*x*x*x*x .....
      // -> (y-a0)/x=a1+a2*x+a3*x*x+a4*x*x*x ...
      ctrl=m_ctrlL[jj];
      ctrl.setY((ctrl.y()-ctmp)/ctrl.x());
      pp.m_ctrlL.set(jj,ctrl,0);
    }
    for (jj=kk+1;jj<nctrl-ii;jj++) {
      ctrl=m_ctrlL[jj-1];      
      ctrl.setXY(pp.m_ctrlL[jj].x(),pp.m_ctrlL[jj].y());
      pp.m_ctrlL.set(jj-1,ctrl,0);
    }
    pp.m_ctrlL.remove(pp.m_ctrlL.count()-1);
    //for (j=0;j<pp.nctrl;j++) printf ("j=%d x=%f y=%f c=%f\n",j,pp.m_x[j],pp.m_y[j],c[i]);
    //pp.clearTable();
    jj=pp.m_ctrlL.count();
    for (kk=0;kk<pp.m_ctrlL.count();kk++) {
      pp.m_c[0][kk]=pp.m_d[0][kk]=pp.m_ctrlL[kk].y();
      if (kk>0) {
        for (ll=0;ll<jj;ll++)
          pp.m_c[kk][ll]=pp.m_d[kk][ll]=.0;
      }
      jj--;
    }
  }
  cc->resize(m_coeffL.count());
  for (ii=0;ii<m_coeffL.count();ii++)
    cc->append(m_coeffL[ii]);
  return m_coeffL.count();

}

int Polynomial::rootsBrute(double *roots,double min,double max,int *effdeg) {

  if ((m_options&interpolation_eq)>0)
    return 0;
  int ii=0,jj=0,kk=0,nctrl=m_ctrlL.count(),degree=nctrl-1;
  for (ii=nctrl-1;ii>-1;ii--)  {
    if (m_ctrlL[ii].x()==.0) {
      degree=ii;
      break;
    }
  }
  if (effdeg)
    *effdeg=degree;

  if (degree==0) {
    if (m_ctrlL[0].x()==.0) {
      roots[0]=.0;
      return 1;
    }
    return 0;
  }
  if (degree==1) {
    roots[0]=-m_ctrlL[0].x()/m_ctrlL[1].x();
    return degree;
  }
  if (degree==2) {
    double pp2=-m_ctrlL[1].x()/(2.*m_ctrlL[2].x()),pqres=pp2*pp2-m_ctrlL[0].x()/m_ctrlL[2].x();
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
    for (jj=0;jj<=degree;jj++) {
      res+=m_ctrlL[jj].x()*tmp;
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
    aux::heapsortv(kk,roots);
  return kk;

}

Bezier::Bezier(VertexList *ctrlL) : 
  Interpolation(interpolation_bezier) {

  setArr(ctrlL);

}

Bezier::~Bezier() {

}

int Bezier::interpol(int nint,VertexList *vint,double,double) {

  int ii=0,jj=0,nctrl=m_ctrlL.count(),penctrl=nctrl-1,penint=nint-1;
  if (nctrl<=0 || nint<=1 || !vint)
    return 1;
  double t1=.0,t2=1.,bino=1.;
  double *fac1=new double[(size_t)nctrl];
  double *fac2=new double[(size_t)nctrl];
  Vertex vv;
  for (jj=0;jj<nctrl;jj++)
    fac1[jj]=fac2[jj]=1.0;
  for (ii=0;ii<nint;ii++) {
    vv.setXY(.0,.0);
    t1=(double)ii/(double)penint;
    t2=1.-t1;
    // poly-degree=nctrl , accumulate bernstein polys
    for (jj=1;jj<nctrl;jj++) {
      fac1[jj]=fac1[jj-1]*t1;
      fac2[penctrl-jj]=fac2[penctrl-jj+1]*t2;
    }
    for (jj=0;jj<nctrl;jj++) {
      bino=mk_binomialcoeff(penctrl,jj);
      vv.setX(vv.x()+m_ctrlL[jj].x()*bino*fac1[jj]*fac2[jj]);
      vv.setY(vv.y()+m_ctrlL[jj].y()*bino*fac1[jj]*fac2[jj]);
    }
    for (jj=0;jj<nctrl;jj++)
      fac1[jj]=fac2[jj]=1.;
    vv.setZ(m_ctrlL[ii*nctrl/nint].z());
    vint->append(vv);
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

int Bicubic::interp(Vertex *) const {


  return 0;

}

int Bicubic::interpol(int nint,VertexList *vint,double,double) {

  if (!vint)
    return 1;
  
  return 0;

}

} // namespace
