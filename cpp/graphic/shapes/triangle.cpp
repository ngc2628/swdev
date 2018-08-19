
#include <numeric/matrix.h>
#include <graphic/shapes/triangle.h>
#include <graphic/shapes/line.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

static const int defCntPoints=3;

void Triangle::set(aux::Vector3 t0,aux::Vector3 t1,aux::Vector3 t2) {

  if (t0.busted(aux::typeX|aux::typeY)!=0)
    t0=aux::Vector3(0.,0.);
  if (t1.busted(aux::typeX|aux::typeY)!=0)
    t1=aux::Vector3(0.,0.);
  if (t2.busted(aux::typeX|aux::typeY)!=0)
    t2=aux::Vector3(0.,0.);
  if (t1[0]<t0[0])
    aux::swap(&t0,&t1);
  if (t2[0]<t1[0])
    aux::swap(&t1,&t2);
  m_t[0]=t0;
  m_t[1]=t1;
  m_t[2]=t2;
  m_points.clear();
  
}

double Triangle::circradius() const { 
      
  Line2 aa(m_t[0],m_t[1]),bb(m_t[1],m_t[2]),cc(m_t[2],m_t[0]);
  double la=aa.len(),lb=bb.len(),lc=cc.len(),ss=(la+lb+lc)/2.,A4=4*sqrt(ss*(ss-la)*(ss-lb)*(ss-lc));
  return (la*lb*lc/A4);
  
}

aux::Vector3 Triangle::center() const {

  TransformMatrix mm;
  aux::Vector3 mt0(m_t[0]),mt1(m_t[1]),mt2(m_t[2]),
               m10(mt1-mt0),m21(mt2-mt1),nn(.0,.0,1.),
               n10=m10.cross(nn),n21=m21.cross(nn);
  double xm10=(mt1[0]+mt0[0])/2.,ym10=(mt1[1]+mt0[1])/2.,
         xm21=(mt2[0]+mt1[0])/2.,ym21=(mt2[1]+mt1[1])/2.;
  mm.translate(xm10,ym10);
  mm.transform(&n10);
  mm.reset();
  mm.translate(xm21,ym21);
  mm.transform(&n21);
  mm.reset();
  double xs=mk_dnan,ys=mk_dnan;
  mk_linesintersection(xm10,ym10,n10[0],n10[1],xm21,ym21,n21[0],n21[1],&xs,&ys,mk_ddPrec,0,0);
  return aux::Vector3(xs,ys);

}

int Triangle::eval(TVList<Vector3> *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  m_points.clear();
  if (m_points.size()<npoints)
    m_points.resize(npoints);
  TransformMatrix mm;
  aux::Vector3 mt0(m_t[0]),mt1(m_t[1]),mt2(m_t[2]),
               m10(mt1-mt0),m21(mt2-mt1),nn(.0,.0,1.),
               n10=m10.cross(nn),n21=m21.cross(nn);
  double xm10=(mt1[0]+mt0[0])/2.,ym10=(mt1[1]+mt0[1])/2.,
         xm21=(mt2[0]+mt1[0])/2.,ym21=(mt2[1]+mt1[1])/2.;
  mm.translate(xm10,ym10);
  mm.transform(&n10);
  mm.reset();
  mm.translate(xm21,ym21);
  mm.transform(&n21);
  mm.reset();
  double xs=mk_dnan,ys=mk_dnan;
  mk_linesintersection(xm10,ym10,n10[0],n10[1],xm21,ym21,n21[0],n21[1],&xs,&ys,mk_ddPrec,0,0);
  mm.translate(-xs,-ys);
  mm.scale(m_scale,m_scale,1.);
  mm.rotateZ(m_rotate);
  mm.translate(xs+m_translate[0],ys+m_translate[1]);
  mm.transform(&mt0);
  m_points.append(mt0);
  mm.transform(&mt1);
  m_points.append(mt1);
  mm.transform(&mt2);
  m_points.append(mt2);
  
  if (pointL)
    *pointL=m_points;
  return npoints;

}

void Triangle::toStringType(Asciistr *buf) const {

  if (!buf)
    return;
  aux::Asciistr numstr;
  buf->append("p0, ");
  m_t[0].toString(&numstr);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\np1, ");
  m_t[1].toString(&numstr);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\np2, ");
  m_t[2].toString(&numstr);
  buf->append((const char *)numstr);
  buf->append("\n");

}

int TriangleEq::eval(TVList<Vector3> *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  m_points.clear();
  if (m_points.size()<npoints)
    m_points.resize(npoints);
  double xx=m_a/2.,yy=m_a*sqrt(3.)/6.;
  Vector3 tt[3]={Vector3(-xx,-yy),Vector3(xx,-yy),Vector3(0.,2.*yy)};
  TransformMatrix mm;
  mm.rotateZ(m_rotate);
  mm.translate(m_translate[0],m_translate[1]);
  int ii=0;
  for (ii=0;ii<npoints;ii++) {
    mm.transform(&tt[ii]);
    m_points.append(tt[ii]);
  }

  if (pointL)
    *pointL=m_points;
  return npoints;

}

} // namespace
