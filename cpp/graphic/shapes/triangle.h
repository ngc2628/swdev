
#ifndef _triangle_h_
#define _triangle_h_

#include <auxx/auxx.h>
#include <numeric/vertex.h>
#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Triangle : public Shape2 {

  protected:
    num::Vector3 m_t[3];    
          
  public:
    Triangle(num::Vector3 t0=num::Vector3(),num::Vector3 t1=num::Vector3(),
             num::Vector3 t2=num::Vector3());
    Triangle(const Triangle &);
    virtual ~Triangle() { }
    Triangle &operator=(const Triangle &);
    bool operator==(const Triangle &) const;
    bool operator<(const Triangle &) const;
    num::Vector3 operator[](int) const;
    aux::TypeId *clone() const;
    void set(num::Vector3,num::Vector3,num::Vector3);
    void get(num::Vector3 *,num::Vector3 *,num::Vector3 *) const;
    double circradius() const;
    num::Vector3 center() const;
    int eval(num::VertexList *,int npoints=-1);
    int toStringType(mk_string) const;
    
};

class TriangleEq : public Shape2 {

  protected:
    double m_a;

  public:
    TriangleEq(double a=.0) : Shape2("triangleeq"),m_a(mk_isfinite(a) ? a : 0.) { }
    TriangleEq(const TriangleEq &ass) : Shape2((const Shape2 &)ass),m_a(ass.m_a) { }
    virtual ~TriangleEq() { }
    TriangleEq &operator=(const TriangleEq &);
    bool operator==(const TriangleEq &) const;
    bool operator<(const TriangleEq &) const;
    double set(double);
    double get() const {
      return m_a;
    }
    int eval(num::VertexList *,int npoints=-1);
    
};

} // namespace

#endif
