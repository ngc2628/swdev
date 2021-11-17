
#include <graphic/shapes/shape.h>
#include <mkbase/mkmath.h>
#include <stdio.h>
#include <string.h>

namespace shapes {

Shape2::Shape2(const char *type) : m_scale(1.),m_rotate(.0) {

  mk_vertexset(m_translate,.0);
  mk_stringset(m_descr,type);
  mk_listalloc(&m_vertices,sizeof(mk_vertex),0);
     
}

Shape2::Shape2(const Shape2 &ass) : m_scale(ass.m_scale),m_rotate(ass.m_rotate) {

  if (this!=&ass) {
    mk_vertexcopy(m_translate,ass.m_translate);
    mk_stringset(m_descr,&ass.m_descr[0]);
    m_styleO=ass.m_styleO;
    m_styleF=ass.m_styleF;
    mk_listalloc(&m_vertices,sizeof(mk_vertex),0);
    mk_listcopy(&m_vertices,&ass.m_vertices);
  }

}

Shape2::~Shape2() {
    
  mk_listfree(&m_vertices);

}

Shape2 &Shape2::operator=(const Shape2 &ass) {
 
  if (this!=&ass) {
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

bool Shape2::operator==(const Shape2 &cmp) const {

  return (strcmp(&m_descr[0],&cmp.m_descr[0])==0);

}
 
bool Shape2::operator<(const Shape2 &cmp) const {

  return (strcmp(&m_descr[0],&cmp.m_descr[0])<0);

}

int Shape2::clone(Shape2 *shape2) {

  if (!shape2 || this==shape2) 
    return 1;
  m_scale=shape2->m_scale;
  m_rotate=shape2->m_rotate;
  mk_vertexcopy(m_translate,shape2->m_translate);
  mk_stringset(m_descr,&shape2->m_descr[0]);
  m_styleO=shape2->m_styleO;
  m_styleF=shape2->m_styleF;
  mk_listcopy(&m_vertices,&shape2->m_vertices);
  return 0;

}

double Shape2::getScale() const {

  return m_scale;
  
}

double Shape2::setScale(double scale) {

  if (mk_isbusted(scale)==0 && scale>.0)
    m_scale=scale;
  return m_scale;
    
} 
    
double Shape2::getRotate() const {
      
  return m_rotate;
  
}

double Shape2::setRotate(double rotate) {
      
  if (mk_isbusted(rotate)==0)
    m_rotate=mk_dsign(rotate)*fmod(fabs(rotate),360.);
  return m_rotate;
    
}

int Shape2::getTranslate(mk_vertex translate) const {
      
  mk_vertexcopy(translate,m_translate);
  return 0;
  
}

int Shape2::setTranslate(mk_vertex translate) {
      
  mk_vertexcopy(m_translate,translate);
  return 0;
  
}

int Shape2::getDescr(mk_string descr) {

  mk_stringset(descr,&m_descr[0]);
  return 0;

}

int Shape2::setDescr(const char *descr) { 

  mk_stringset(m_descr,descr);
  return 0;
  
}

osix::xxStyle Shape2::getStyleO() const {

  return m_styleO;

}

osix::xxStyle Shape2::setStyleO(osix::xxStyle styleO) {

  m_styleO=styleO;
  return m_styleO;

}

osix::xxStyle Shape2::getStyleF() const {

  return m_styleF;

}

osix::xxStyle Shape2::setStyleF(osix::xxStyle styleF) {

  m_styleF=styleF;
  return m_styleF;

}
 
int Shape2::getVertex(int idx,mk_vertex vertex) const {

  mk_vertexset(vertex,mk_dnan);
  int res=(idx<0 || idx>=m_vertices.count ? 1 : 0);
  if (res==0) {
    mk_vertexnan(vv);
    mk_listat(&m_vertices,idx,(void*)&vv);
    mk_vertexcopy(vertex,vv);
  }
  return res;

}
    
int Shape2::setVertex(int idx,mk_vertex vertex) {

  int res=1;
  if (idx>=0 && idx<m_vertices.count && mk_isbusted(vertex[0])==0 && mk_isbusted(vertex[1])==0)
    res=0;
  if (res==0) {
    mk_vertexzero(vv);
    mk_vertexcopy(vv,vertex);
    mk_listsetat(&m_vertices,(void*)&vv,idx,0);
  }
  return res;

}

int Shape2::numVertices() const {

  return m_vertices.count;

}
    
int Shape2::center(mk_vertex) const {
      
  return 0;
  
}
    
double Shape2::circradius() const {
      
  return .0;
  
}	

int Shape2::toString(mk_string str) const {
  
  mk_stringset(str,0);
  mk_string numstr;
  mk_stringset(numstr,0);
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
  mk_vertexdbg(m_translate,numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\ncircradius=");
  mk_d2a(circradius(),numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"\ncenter=");
  mk_stringset(numstr,0);
  mk_vertexnan(vv);
  center(vv);
  mk_vertexdbg(vv,numstr);
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
  return 0;
  
}

int Shape2::eval(struct mk_list *pointL,int npoints) {

  mk_listfree(pointL);
  mk_listalloc(pointL,sizeof(mk_vertex),npoints);
  return npoints;

}

NoShape2 &NoShape2::operator=(const NoShape2 &ass) {
 
  mk_stringset(m_descr,"noshape");
  return *this; 
  
}

bool NoShape2::operator==(const NoShape2 &cmp) const {

  return (strcmp(&m_descr[0],&cmp.m_descr[0])==0);

}
 
bool NoShape2::operator<(const NoShape2 &cmp) const {

  return (strcmp(&m_descr[0],&cmp.m_descr[0])<0);

}
    
int NoShape2::eval(struct mk_list *pointL,int) {
   
  mk_listfree(pointL);
  return 0;
  
}

} // namespace
