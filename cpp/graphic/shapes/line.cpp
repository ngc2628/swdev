
#include <numeric/matrix.h>
#include <graphic/shapes/line.h>
#include <stdio.h>
#include <string.h>

using namespace num;

namespace shapes {

static const int defCntPoints=2;

Line2::Line2() : Shape2("Line2") {
    
  mk_listrealloc(&m_vertices,2);
  mk_vertexzero(pp);
  mk_listappend(&m_vertices,pp);
  mk_listappend(&m_vertices,pp);
    
}

Line2::Line2(const Line2 &ass) : Shape2((const Shape2 &)ass) {
  
}

Line2 &Line2::operator=(const Line2 &ass) {
      
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

bool Line2::operator==(const Line2 &cmp) const {
      
  return (len()==cmp.len());
  
}

bool Line2::operator<(const Line2 &cmp) const {
      
  return (len()<cmp.len());
  
}

double Line2::len() const { 
      
  mk_vertexzero(p1);
  mk_vertexzero(p2);
  mk_listat(&m_vertices,0,(void*)&p1);
  mk_listat(&m_vertices,1,(void*)&p2);
  double xx=p2[0]-p1[0],yy=p2[1]-p1[1];
  return sqrt(xx*xx+yy*yy); 
  
}
    
double Line2::ang() const { 

  mk_vertexzero(p1);
  mk_vertexzero(p2);
  mk_listat(&m_vertices,0,(void*)&p1);
  mk_listat(&m_vertices,1,(void*)&p2);
  mk_vertexnan(nn);
  nn[0]=.0;
  nn[1]=1.;
  mk_vertexnan(res);
  mk_vertexsubs(p2,p1,res);
  return mk_vertexangdeg(res,nn); 

}
 
int Line2::center(mk_vertex vv) const { 

  mk_vertexzero(p1);
  mk_vertexzero(p2);
  mk_listat(&m_vertices,0,(void*)&p1);
  mk_listat(&m_vertices,1,(void*)&p2);
  vv[0]=(p2[0]+p1[0])/2.;
  vv[1]=(p2[1]+p1[1])/2.;
  vv[2]=0;
  return 0;

}

double Line2::circradius() const {
      
  return len()/2.;
  
}

int Line2::eval(struct mk_list *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  Shape2::eval(pointL,npoints);
  mk_vertexzero(p1);
  mk_vertexzero(p2);
  mk_listat(&m_vertices,0,(void*)&p1);
  mk_listat(&m_vertices,1,(void*)&p2);
  mk_vertexnan(pc);
  center(pc);
  TransformMatrix m;
  m.translate(-pc[0],-pc[1]);
  m.scale(m_scale,m_scale);
  m.rotateZ(m_rotate);
  m.translate(pc[0]+m_translate[0],pc[1]+m_translate[1]);
  m.transform(p1);
  m.transform(p2);
  mk_listappend(pointL,(void*)&p1);
  mk_listappend(pointL,(void*)&p2);
  return npoints;

}

int Line2::toString(mk_string str) const {
  
  Shape2::toString(str);
  mk_vertexzero(p1);
  mk_vertexzero(p2);
  mk_listat(&m_vertices,0,(void*)&p1);
  mk_listat(&m_vertices,1,(void*)&p2);
  mk_stringappend(str,"p0, ");
  mk_string numstr;
  mk_vertexdbg(p1,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\np1, ");
  mk_vertexdbg(p2,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\nlen=");
  mk_d2a(len(),numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\nang=");
  mk_d2a(ang(),numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\n");
  return 0;
  
}

} // namespace
