
#include <graphic/shapes/shape.h>
#include <stdio.h>
#include <string.h>

namespace shapes {

Shape2 &Shape2::operator=(const Shape2 &ass) {
 
  if (this==&ass)
    return *this;
  ((aux::TypeId*)this)->operator=((const aux::TypeId &)ass); 
  m_scale=ass.m_scale;
  m_rotate=ass.m_rotate;
  m_translate=ass.m_translate;
  m_points=ass.m_points;
  memcpy(&m_descr[0],&ass.m_descr[0],mk_idlen);
  m_styleO=ass.m_styleO;
  m_styleF=ass.m_styleF;
  return *this; 
  
}

double Shape2::scale(double sc) {

  if (mk_isBusted(sc)==0 && sc>.0)
    m_scale=sc;
  m_points.clear();
  return m_scale;
  
}

double Shape2::rotate(double rr) { 
      
  if (mk_isBusted(rr)==0)
    m_rotate=mk_dsign(rr)*fmod(fabs(rr),360.);
  m_points.clear(); 
  return m_rotate; 
  
}
	
aux::Vector3 Shape2::translate(aux::Vector3 tt) { 

  if (tt.busted(aux::typeX|aux::typeY)==0)
    m_translate=tt;
  m_points.clear();
  return m_translate; 
  
}

void Shape2::setDescr(const char *descr) { 

  memset(&m_descr[0],0,mk_idlen);
  if (descr)
    strncpy(&m_descr[0],descr,mk_idlen-1);
  
}	

void Shape2::toString(aux::Asciistr *buf) const {
  
  if (!buf)
    return;
  TypeId::toString(buf);
  buf->reserve(1024*1024);
  buf->append("\ndescr=");
  buf->append(m_descr);
  buf->append("\nscale=");
  aux::Asciistr numstr;
  aux::d2a(m_scale,&numstr,-1);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\nrotate=");
  aux::d2a(m_rotate,&numstr,-1);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\ntranslate: ");
  m_translate.toString(&numstr);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\ncircradius=");
  aux::d2a(circradius(),&numstr,-1);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\ncenter=");
  center().toString(&numstr);
  buf->append((const char *)numstr);
  numstr=0; 
  buf->append("\nstyles:\n");
  buf->append("  ");
  m_styleO.toString(&numstr);
  buf->append((const char *)numstr);
  buf->append("\n  ");
  numstr=0;
  m_styleF.toString(&numstr);
  buf->append((const char *)numstr);
  buf->append("\n");
  toStringType(buf);
  buf->append("eval, ");
  numstr=0;
  aux::i2a(m_points.count(),&numstr);
  buf->append((const char*)numstr);
  buf->append(" points: \n");
  int ii=0;
  for (ii=0;ii<m_points.count();ii++) {
    buf->append("  p");
    aux::i2a(ii,&numstr);
    buf->append((const char*)numstr);
    numstr=0;
    buf->append(": ");
    m_points[ii].toString(&numstr);
    buf->append((const char*)numstr);
    buf->append("\n"); 
    numstr=0;
  }
  if (ii==0)
    buf->append("\n");
  
}

int NoShape2::eval(aux::TVList<aux::Vector3> *pointL,int) {
      
  m_points.resize(0);
  if (pointL)
    *pointL=m_points;
  return 0;  
  
}

} // namespace
