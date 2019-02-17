
#include <numeric/matrix.h>
#include <graphic/shapes/ellipse.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

static const int defCntPoints=60;

class NEllipse {

  public:
    Vector3 *m_points;
    unsigned short m_cnt;
    NEllipse(unsigned short cnt,int cmp) { 
      m_cnt=cnt;
      if (m_cnt==0 || cmp) {
        m_points=0;
        return;
      }
      m_points=new Vector3[m_cnt];
      int ii=0,off=m_cnt/2;
      double sc=2.*mk_pi/(double)m_cnt;
      for (ii=0;ii<m_cnt;ii++) {
        if (ii<=off)
          m_points[ii]=Vector3(cos(sc*(double)ii),sin(sc*(double)ii));
        else
          m_points[ii]=Vector3(-cos(sc*(double)(ii-off)),-sin(sc*(double)(ii-off)));
      }
    }
    ~NEllipse() {
      if (m_points)
        delete [] m_points;
    }
    bool operator==(const NEllipse &cmp) const {
      return (m_cnt==cmp.m_cnt);
    }
    bool operator<(const NEllipse &cmp) const {
      return (m_cnt<cmp.m_cnt);
    }
    
  protected:
    NEllipse(const NEllipse &) { }
    NEllipse &operator=(const NEllipse &) {
      return *this;
    }

};

static TPList<NEllipse> elL;

Ellipse::Ellipse(num::Vector3 ab) : Shape2("ellipse"),m_ab(fabs(ab[0]),fabs(ab[1])) {
	  
  double *vab=m_ab.data();
  if (vab[0]>vab[1]) 
	  aux::swap(&vab[0],&vab[1]);
  
}

Ellipse &Ellipse::operator=(const Ellipse &ass) {
      
  if (this!=&ass) {
    ((Shape2*)this)->operator=((const Shape2 &)ass); 
    m_ab=ass.m_ab;
  }
  return *this;
  
}

bool Ellipse::operator==(const Ellipse &cmp) const {
      
  return ((const Shape2*)this)->operator==((const Shape2&)cmp);
  
}

bool Ellipse::operator<(const Ellipse &cmp) const {
      
  return ((const Shape2*)this)->operator<((const Shape2&)cmp);

}

aux::TypeId *Ellipse::clone() const {
      
  return new Ellipse((const Ellipse &)(*this));
  
}

num::Vector3 Ellipse::set(num::Vector3 ab) {
      
  double *vab=m_ab.data();	
  if (vab[0]>vab[1])
    mk_swapf(&vab[0],&vab[1]);
  m_ab=ab; 
  m_points.clear(); 
  return m_ab;
  
}

double Ellipse::ecc() const {
      
  return (sqrt(m_ab.x()*m_ab.x()-m_ab.y()*m_ab.y())/m_ab.x());
  
}

double Ellipse::area() const {

  return (mk_pi*m_ab.x()*m_ab.y());
  
}

double Ellipse::circ() const { // circumference

  double ll=(m_ab.x()-m_ab.y())/(m_ab.x()+m_ab.y());
  double fac=ll*ll;
  double add=1.+fac/4.;
  fac*=fac;
  add+=(fac/64.+fac*ll*ll/256.);
  fac*=fac;
  add+=25.*fac/16384.;
  return (mk_pi*(m_ab.x()+m_ab.y())*add);
    
}

double Ellipse::circradius() const {

  return (m_ab.x()>m_ab.y() ? m_ab.x() : m_ab.y());
  
}

int Ellipse::eval(num::VertexList *pointL,int npoints) {

  if (elL.size()<=0)
    elL.resize(127);
  if (npoints<=0)
    npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  NEllipse cel((unsigned short)npoints,1);
  NEllipse *el=0;
  int ii=elL.find(&cel);
  if (ii>=0)
    el=elL.at(ii);
  else {
    el=new NEllipse((unsigned short)npoints,0);
    elL.inSort(el);
  }
  m_points.clear();
  m_points.resize(npoints);
  TransformMatrix mm;
  mm.rotateZ(m_rotate);
  mm.translate(m_translate[0],m_translate[1]);
  Vector3 pp;
  for (ii=0;ii<npoints;ii++) {
    pp.setXY(m_ab[0]*el->m_points[ii][0],m_ab[1]*el->m_points[ii][1]);
    mm.transform(&pp);
    m_points.append(pp);
  }

  if (pointL)
    *pointL=m_points;
  return npoints;
  
}

} // namespace
