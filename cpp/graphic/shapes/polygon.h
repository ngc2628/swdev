
#ifndef _polygon_h_
#define _polygon_h_

#include <auxx/auxx.h>
#include <auxx/vertex.h>
#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Polygon2 : public Shape2 {

  protected:
    aux::TVList<aux::Vector3> m_ppoints;

  public:
    Polygon2(int npoints=0);
    Polygon2(const Polygon2 &ass) : Shape2((const Shape2 &)ass) { }
    virtual ~Polygon2() { }
    Polygon2 &operator=(const Polygon2 &ass) { 
      if (this==&ass)
        return *this;
      ((Shape2*)this)->operator=((const Shape2 &)ass);
      m_ppoints=ass.m_ppoints; 
      return *this; 
    }
    bool operator==(const Polygon2 &cmp) const {
      return ((const Shape2*)this)->operator==((const Shape2&)cmp);
    }
    bool operator<(const Polygon2 &cmp) const {
      return ((const Shape2*)this)->operator<((const Shape2&)cmp);
    }
    aux::Vector3 operator[](int idx) const {
      if (idx<=0)
        return m_ppoints[0];
      if (idx>=m_ppoints.count()-1)
        return m_ppoints[m_ppoints.count()-1];
      return m_ppoints[idx];
    }
    aux::TypeId *clone() const {
      return new Polygon2((const Polygon2 &)(*this));
    }
    int eval(aux::TVList<aux::Vector3>*,int npoints=-1);
    int set(aux::TVList<aux::Vector3> *pointL) { 
      m_ppoints=*pointL; 
      m_points.clear();
      return m_ppoints.count(); 
    }
    int get(aux::TVList<aux::Vector3> *pointL) const {
      if (pointL)
        *pointL=m_ppoints;
      return m_ppoints.count();
    }
        
};

} // namespace

#endif