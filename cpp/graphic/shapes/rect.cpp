
#include <numeric/matrix.h>
#include <graphic/shapes/rect.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

static const int defCntPoints=4;

bool RectSize::operator==(const RectSize &cmp) const {

  return (m_width==cmp.m_width && m_height==cmp.m_height);

}
    
bool RectSize::operator<(const RectSize &cmp) const {
      
  return (m_width*m_height<cmp.m_width*cmp.m_height);
  
}

double RectSize::setWidth(double ww) {
      
  m_width=(mk_isbusted(ww)!=0 || ww<0. ? 0. : ww);
  return m_width;
  
}
    
double RectSize::setHeight(double hh) {
      
  m_height=(mk_isbusted(hh)!=0 || hh<0. ? 0. : hh);
  return m_height;
  
}

RectSize RectSize::set(double ww,double hh) {
      
  m_width=(mk_isbusted(ww)!=0 || ww<0. ? 0. : ww);
  m_height=(mk_isbusted(hh)!=0 || hh<0. ? 0. : hh);
  return *this;
  
}

bool RectSize::empty() const {
      
  return (m_width<=.0 || m_height<=.0);
  
}

int RectSize::toString(mk_string str) const {
  
  mk_stringappend(str,"w=");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_d2a(m_width,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," , h=");
  mk_d2a(m_height,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\n");
  return 0;  

}

Rect &Rect::operator=(const Rect &ass) {
  
  if (this!=&ass) {
    ((Shape2*)this)->operator=((const Shape2 &)ass); 
    m_sz=ass.m_sz; 
  }
  return *this; 

}

bool Rect::operator==(const Rect &cmp) const {
      
  return ((const Shape2*)this)->operator==((const Shape2&)cmp);
  
}

bool Rect::operator<(const Rect &cmp) const {

  return ((const Shape2*)this)->operator<((const Shape2&)cmp);
  
}

aux::TypeId *Rect::clone() const {

  return new Rect((const Rect &)(*this));
  
}
    
RectSize Rect::setSize(RectSize sz) {

  m_sz=sz;
  m_points.clear();
  return m_sz;
 
}

double Rect::rotate(double rr) { 

  m_rotate=mk_dsign(rr)*fmod(fabs(rr),90.); 
  m_points.clear();
  return m_rotate; 

}

num::Vector3 Rect::center() const {

  return num::Vector3(.0,.0,.0);

}

double Rect::circradius() const {

  return sqrt(m_sz.width()*m_sz.width()+m_sz.height()*m_sz.height());

}

int Rect::eval(num::VertexList *pointL,int npoints) {

  if (npoints!=defCntPoints) npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  m_points.clear();
  m_points.resize(npoints);
  num::Vector3 pc(.0,m_sz.width()/2.),
               p0(.0,m_sz.height()/2.),p1(p0),p2(.0,-m_sz.height()/2.),p3(p2);
  TransformMatrix mm;
  mm.scale(1.,m_scale);
  mm.rotateZ(90.+m_rotate);
  mm.transform(&pc);
  mm.reset();
  mm.scale(1.,m_scale);
  mm.rotateZ(m_rotate);
  mm.translate(pc[0],pc[1]);
  mm.transform(&p0);
  mm.transform(&p3);
  mm.translate(-2.*pc[0],-2.*pc[1]);
  mm.transform(&p1);
  mm.transform(&p2);
  mm.reset();
  mm.translate(m_translate[0],m_translate[1]);
  mm.transform(&p0);
  m_points.append(p0);
  mm.transform(&p1);
  m_points.append(p1);
  mm.transform(&p2);
  m_points.append(p2);
  mm.transform(&p3);
  m_points.append(p3);
  if (pointL)
    *pointL=m_points;
  return npoints;

}

int Rect::toStringType(mk_string str) const {
  
  mk_stringappend(str,"size: ");
  mk_string numstr;
  mk_stringset(numstr,0);
  m_sz.toString(numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\n");
  return 0;
    
}

} // namespace
