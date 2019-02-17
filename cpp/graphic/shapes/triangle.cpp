
#include <mkbase/mkla.h>
#include <numeric/matrix.h>
#include <graphic/shapes/triangle.h>
#include <graphic/shapes/line.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

static const int defCntPoints=3;

Triangle::Triangle(num::Vector3 t0,num::Vector3 t1,num::Vector3 t2) : Shape2("triangle") {

  if (t0.busted(num::typeX|num::typeY)!=0)
    t0=num::Vector3(0.,0.);
  if (t1.busted(num::typeX|num::typeY)!=0)
    t1=num::Vector3(0.,0.);
  if (t2.busted(num::typeX|num::typeY)!=0)
    t2=num::Vector3(0.,0.);
  if (t1[0]<t0[0])
    aux::swap(&t0,&t1);
  if (t2[0]<t1[0])
    aux::swap(&t1,&t2);
  m_t[0]=t0;
  m_t[1]=t1;
  m_t[2]=t2;

}
    
Triangle::Triangle(const Triangle &ass) : Shape2((const Shape2 &)ass) {

  int ii=0;
  for (ii=0;ii<3;ii++)
    m_t[ii]=ass.m_t[ii];

}
 
Triangle &Triangle::operator=(const Triangle &ass) {
      
  if (this!=&ass) {
    ((Shape2*)this)->operator=((const Shape2 &)ass);
    int ii=0;
    for (ii=0;ii<3;ii++)
      m_t[ii]=ass.m_t[ii];
  }
  return *this;
  
}
   
bool Triangle::operator==(const Triangle &cmp) const {
      
  return ((const Shape2*)this)->operator==((const Shape2&)cmp);
  
}
    
bool Triangle::operator<(const Triangle &cmp) const {

  return ((const Shape2*)this)->operator<((const Shape2&)cmp);
  
}

num::Vector3 Triangle::operator[](int idx) const {
      
  if (idx<=0)
    return m_t[0];
  if (idx>=2)
    return m_t[2];
  return m_t[1];
  
}

aux::TypeId *Triangle::clone() const {

  return new Triangle((const Triangle &)(*this));
  
}
    
void Triangle::get(num::Vector3 *t0,num::Vector3 *t1,num::Vector3 *t2) const {

  if (t0)
    *t0=m_t[0];
  if (t1)
    *t1=m_t[1];
  if (t2)
    *t2=m_t[2];
  
}

void Triangle::set(num::Vector3 t0,num::Vector3 t1,num::Vector3 t2) {

  if (t0.busted(num::typeX|num::typeY)!=0)
    t0=num::Vector3(0.,0.);
  if (t1.busted(num::typeX|num::typeY)!=0)
    t1=num::Vector3(0.,0.);
  if (t2.busted(num::typeX|num::typeY)!=0)
    t2=num::Vector3(0.,0.);
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

num::Vector3 Triangle::center() const {

  TransformMatrix mm;
  num::Vector3 mt0(m_t[0]),mt1(m_t[1]),mt2(m_t[2]),
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
  mk_vertex 
    vm1={xm10,ym10,mk_dnan,mk_dnan},n1={n10[0],n10[1],mk_dnan,mk_dnan},
    vm2={xm21,ym21,mk_dnan,mk_dnan},n2={n21[0],n21[1],mk_dnan,mk_dnan},
    ss={mk_dnan,mk_dnan,mk_dnan,mk_dnan};
  mk_linesintersection(vm1,n1,vm2,n2,ss,3,0);
  return num::Vector3(ss[0],ss[1]);

}

int Triangle::eval(num::VertexList *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  m_points.clear();
  m_points.resize(npoints);
  TransformMatrix mm;
  num::Vector3 mt0(m_t[0]),mt1(m_t[1]),mt2(m_t[2]),
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
  mk_vertex 
    vm1={xm10,ym10,mk_dnan,mk_dnan},n1={n10[0],n10[1],mk_dnan,mk_dnan},
    vm2={xm21,ym21,mk_dnan,mk_dnan},n2={n21[0],n21[1],mk_dnan,mk_dnan},
    ss={mk_dnan,mk_dnan,mk_dnan,mk_dnan};  
  mk_linesintersection(vm1,n1,vm2,n2,ss,3,0);
  mm.translate(-ss[0],-ss[1]);
  mm.scale(m_scale,m_scale,1.);
  mm.rotateZ(m_rotate);
  mm.translate(ss[0]+m_translate[0],ss[1]+m_translate[1]);
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

int Triangle::toStringType(mk_string str) const {

  mk_stringappend(str,"p0, ");
  mk_string numstr;
  mk_stringset(numstr,0);
  m_t[0].toString(numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\np1, ");
  mk_stringset(numstr,0);
  m_t[1].toString(numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\np2, ");
  mk_stringset(numstr,0);
  m_t[2].toString(numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\n");
  return 0;

}

TriangleEq &TriangleEq::operator=(const TriangleEq &ass) {
      
  if (this!=&ass) {
    ((Shape2*)this)->operator=((const Shape2 &)ass);
    m_a=ass.m_a;
  }
  return *this;
  
}

bool TriangleEq::operator==(const TriangleEq &cmp) const {
      
  return ((const Shape2*)this)->operator==((const Shape2&)cmp);
  
}
    
bool TriangleEq::operator<(const TriangleEq &cmp) const {
      
  return ((const Shape2*)this)->operator<((const Shape2&)cmp);
  
}
    
double TriangleEq::set(double a) {
      
  m_a=(mk_isfinite(a) ? a : 0.);
  m_points.clear();
  return m_a;
  
}

int TriangleEq::eval(num::VertexList *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  m_points.clear();
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
