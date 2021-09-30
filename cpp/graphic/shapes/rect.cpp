
#include <numeric/matrix.h>
#include <graphic/shapes/rect.h>
#include <mkbase/mkmath.h>
#include <stdio.h>
#include <string.h>

using namespace num;

namespace shapes {

static const int defCntPoints=4;

RectSize::RectSize(double ww,double hh) : m_width(ww<.0 ? .0 : ww),m_height(hh<.0 ? .0 : hh) { 
    
}

RectSize::RectSize(const RectSize &ass) : m_width(ass.m_width),m_height(ass.m_height) {
    
}
    
RectSize &RectSize::operator=(const RectSize &ass) {

  m_width=ass.m_width;
  m_height=ass.m_height;
  return *this;

}

bool RectSize::operator==(const RectSize &cmp) const {

  return (m_width==cmp.m_width && m_height==cmp.m_height);

}
    
bool RectSize::operator<(const RectSize &cmp) const {
      
  return (m_width*m_height<cmp.m_width*cmp.m_height);
  
}

double RectSize::getWidth() const {
      
  return m_width;
  
}

double RectSize::setWidth(double ww) {
      
  m_width=(ww<0. ? 0. : ww);
  return m_width;
  
}

double RectSize::getHeight() const {
      
  return m_height;
  
}
    
double RectSize::setHeight(double hh) {
      
  m_height=(hh<0. ? 0. : hh);
  return m_height;
  
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

Rect::Rect(RectSize sz) : Shape2("rect") { 

  mk_listrealloc(&m_vertices,1);
  mk_vertex vv={sz.getWidth(),sz.getHeight(),.0,1.};
  mk_listappend(&m_vertices,vv);

}

Rect::Rect(const Rect &ass) : Shape2((const Shape2 &)ass) { 

}

Rect &Rect::operator=(const Rect &ass) {
  
  if (&ass!=this) {
    m_scale=ass.m_scale;
    m_rotate=ass.m_rotate;
    mk_vertexcopy(m_translate,ass.m_translate);
    mk_stringset(m_descr,&ass.m_descr[0]);
    m_styleO=ass.m_styleO;
    m_styleF=ass.m_styleF;
    mk_listcopy(&m_vertices,&ass.m_vertices);
  }
  return *this;

}

bool Rect::operator==(const Rect &cmp) const {
      
  return (getSize()==cmp.getSize());
  
}

bool Rect::operator<(const Rect &cmp) const {

  return (getSize()<cmp.getSize());
  
}

RectSize Rect::getSize() const {
     
  mk_vertexzero(vv);
  mk_listat(&m_vertices,0,(void*)&vv);
  RectSize sz(vv[0],vv[1]);
  return sz;
  
}
    
RectSize Rect::setSize(RectSize sz) {

  mk_vertex vv={sz.getWidth(),sz.getHeight(),.0,1.};
  mk_listsetat(&m_vertices,(void*)&vv,0,0);
  return sz;
 
}

double Rect::setRotate(double rotate) { 

  m_rotate=mk_dsign(rotate)*fmod(fabs(rotate),90.); 
  return m_rotate; 

}

int Rect::center(mk_vertex vv) const {

  mk_vertexset(vv,.0);
  return 0;

}

double Rect::circradius() const {

  RectSize sz=getSize();
  return sqrt(sz.getWidth()*sz.getWidth()+sz.getHeight()*sz.getHeight());

}

int Rect::eval(struct mk_list *pointL,int npoints) {

  if (npoints!=defCntPoints) 
    npoints=defCntPoints;
  Shape2::eval(pointL,npoints);
  mk_vertexzero(vv);
  mk_listat(&m_vertices,0,(void*)&vv);
  RectSize sz(vv[0],vv[1]);
  mk_vertexzero(pc);
  pc[0]=.0;
  pc[1]=sz.getWidth()/2.;
  mk_vertexzero(p0);
  p0[0]=.0;
  p0[1]=sz.getHeight()/2.;
  mk_vertexzero(p1);
  mk_vertexcopy(p1,p0);
  mk_vertexnan(p2);
  p2[0]=.0;
  p2[1]=-sz.getHeight()/2.;
  mk_vertexzero(p3);
  mk_vertexcopy(p3,p2);
  TransformMatrix mm;
  mm.scale(1.,m_scale);
  mm.rotateZ(90.+m_rotate);
  mm.transform(pc);
  mm.reset();
  mm.scale(1.,m_scale);
  mm.rotateZ(m_rotate);
  mm.translate(pc[0],pc[1]);
  mm.transform(p0);
  mm.transform(p3);
  mm.translate(-2.*pc[0],-2.*pc[1]);
  mm.transform(p1);
  mm.transform(p2);
  mm.reset();
  mm.translate(m_translate[0],m_translate[1]);
  mm.transform(p0);
  mk_listappend(pointL,(void*)&p0);
  mm.transform(p1);
  mk_listappend(pointL,(void*)&p1);
  mm.transform(p2);
  mk_listappend(pointL,(void*)&p2);
  mm.transform(p3);
  mk_listappend(pointL,(void*)&p3);
  return npoints;

}

int Rect::toString(mk_string str) const {
  
  Shape2::toString(str);
  mk_stringappend(str,"size: ");
  mk_string numstr;
  mk_stringset(numstr,0);
  RectSize sz=getSize();
  sz.toString(numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\n");
  return 0;
    
}

} // namespace
