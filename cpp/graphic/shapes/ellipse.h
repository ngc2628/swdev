
#ifndef _ellipse_h_
#define _ellipse_h_

#include <auxx/auxx.h>
#include <numeric/vertex.h>
#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Ellipse : public Shape2 {

  protected:
    num::Vector3 m_ab;
      
  public:
    Ellipse(num::Vector3);
    Ellipse(const Ellipse &ass) : Shape2((const Shape2 &)ass),m_ab(ass.m_ab) { 
	  }
    virtual ~Ellipse() { 
	  }
    Ellipse &operator=(const Ellipse &);
    bool operator==(const Ellipse &) const;
    bool operator<(const Ellipse &) const;
    aux::TypeId *clone() const;
    num::Vector3 get() const {
      return m_ab;
    }
    num::Vector3 set(num::Vector3);
    double ecc() const;
    double area() const;
    double circ() const;
    double circradius() const;
    int eval(num::VertexList *,int npoints=-1);

};

} // namespace

#endif
