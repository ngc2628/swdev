
#ifndef _ellipse_h_
#define _ellipse_h_

#include <auxx/auxx.h>
#include <auxx/vertex.h>
#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Ellipse : public Shape2 {

  protected:
    aux::Vector3 m_ab;
      
  public:
    Ellipse(aux::Vector3 ab) : Shape2("ellipse"),m_ab(fabs(ab[0]),fabs(ab[1])) {
	  double *vab=m_ab.data();
      if (vab[0]>vab[1]) 
	    aux::swap(&vab[0],&vab[1]);
    }
    Ellipse(const Ellipse &ass) : Shape2((const Shape2 &)ass),m_ab(ass.m_ab) { 
	}
    virtual ~Ellipse() { 
	}
    Ellipse &operator=(const Ellipse &ass) {
      if (this==&ass)
        return *this;
      ((Shape2*)this)->operator=((const Shape2 &)ass); 
      m_ab=ass.m_ab;
      return *this;
    }
    bool operator==(const Ellipse &cmp) const {
      return ((const Shape2*)this)->operator==((const Shape2&)cmp);
    }
    bool operator<(const Ellipse &cmp) const {
      return ((const Shape2*)this)->operator<((const Shape2&)cmp);
    }
    aux::TypeId *clone() const {
      return new Ellipse((const Ellipse &)(*this));
    }
    aux::Vector3 get() const {
      return m_ab;
    }
    aux::Vector3 set(aux::Vector3 ab) {
      double *vab=m_ab.data();	
      if (vab[0]>vab[1])
        aux::swap(&vab[0],&vab[1]);
      m_ab=ab; 
      m_points.clear(); 
      return m_ab;
    }
    double ecc() const {
      return (sqrt(m_ab.x()*m_ab.x()-m_ab.y()*m_ab.y())/m_ab.x());
    }
    double area() const {
      return (aux::pi*m_ab.x()*m_ab.y());
    }
    double circ() const { // circumference
      double l=(m_ab.x()-m_ab.y())/(m_ab.x()+m_ab.y());
      return (aux::pi*(m_ab.x()+m_ab.y())*(1.+l*l/4.+l*l*l*l/64.+l*l*l*l*l*l/256.+25.*l*l*l*l*l*l*l*l/16384.));
    }
    double circradius() const {
      return (m_ab.x()>m_ab.y() ? m_ab.x() : m_ab.y());
    }
    int eval(aux::TVList<aux::Vector3> *,int npoints=-1);

};

} // namespace

#endif
