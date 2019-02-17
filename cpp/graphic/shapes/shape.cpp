
#include <graphic/shapes/shape.h>
#include <stdio.h>
#include <string.h>

namespace shapes {

Shape2::Shape2(const char *type) : 
  aux::TypeId(type),m_scale(1.),m_rotate(.0),m_translate(num::Vector3(0.,0.)) {
   
}

Shape2 &Shape2::operator=(const Shape2 &ass) {
 
  if (this==&ass)
    return *this;
  ((aux::TypeId*)this)->operator=((const aux::TypeId &)ass); 
  m_scale=ass.m_scale;
  m_rotate=ass.m_rotate;
  m_translate=ass.m_translate;
  m_points=ass.m_points;
  mk_stringset(m_descr,&ass.m_descr[0]);
  m_styleO=ass.m_styleO;
  m_styleF=ass.m_styleF;
  return *this; 
  
}

double Shape2::scale(double sc) {

  if (mk_isbusted(sc)==0 && sc>.0)
    m_scale=sc;
  m_points.clear();
  return m_scale;
  
}

double Shape2::rotate(double rr) { 
      
  if (mk_isbusted(rr)==0)
    m_rotate=mk_dsign(rr)*fmod(fabs(rr),360.);
  m_points.clear(); 
  return m_rotate; 
  
}
	
num::Vector3 Shape2::translate(num::Vector3 tt) { 

  if (tt.busted(num::typeX|num::typeY)==0)
    m_translate=tt;
  m_points.clear();
  return m_translate; 
  
}

void Shape2::setDescr(const char *descr) { 

  mk_stringset(m_descr,descr);
  
}	

int Shape2::toString(mk_string str) const {
  
  mk_string numstr;
  mk_stringset(numstr,0);
  TypeId::toString(numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\ndescr=");
  mk_stringappend(str,&m_descr[0]);
  mk_stringappend(str,"\nscale=");
  mk_d2a(m_scale,numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\nrotate=");
  mk_stringset(numstr,0);
  mk_d2a(m_rotate,numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\ntranslate: ");
  mk_stringset(numstr,0);
  m_translate.toString(numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\ncircradius=");
  mk_d2a(circradius(),numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\ncenter=");
  mk_stringset(numstr,0);
  center().toString(numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\nstyles:\n   ");
  mk_stringset(numstr,0);
  m_styleO.toString(numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\n  ");
  mk_stringset(numstr,0);
  m_styleF.toString(numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\n");
  mk_stringset(numstr,0);
  toStringType(numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"eval, ");
  mk_stringset(numstr,0);
  mk_i2a(m_points.count(),numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str," points: \n");
  int ii=0;
  for (ii=0;ii<m_points.count();ii++) {
    mk_stringappend(str,"  p");
    mk_stringset(numstr,0);
    mk_i2a(ii,numstr);
    mk_stringappend(str,&numstr[0]);
    mk_stringappend(str,": ");
    mk_stringset(numstr,0);
    m_points[ii].toString(numstr);
    mk_stringappend(str,&numstr[0]);
    mk_stringappend(str,"\n"); 
  }
  if (ii==0)
    mk_stringappend(str,"\n");
  return 0;
  
}

int NoShape2::eval(num::VertexList *pointL,int) {
      
  m_points.resize(0);
  if (pointL)
    *pointL=m_points;
  return 0;  
  
}

} // namespace
