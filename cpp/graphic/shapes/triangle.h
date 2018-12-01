
#ifndef _triangle_h_
#define _triangle_h_

#include <auxx/auxx.h>
#include <auxx/vertex.h>
#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Triangle : public Shape2 {

  protected:
    aux::Vector3 m_t[3];    
          
  public:
    Triangle(aux::Vector3 t0=aux::Vector3(),aux::Vector3 t1=aux::Vector3(),aux::Vector3 t2=aux::Vector3()) : 
      Shape2("triangle") {
      set(t0,t1,t2);
    }
    Triangle(const Triangle &ass) : Shape2((const Shape2 &)ass) {
      int i=0;
      for (i=0;i<3;i++)
        m_t[i]=ass.m_t[i];
    }
    virtual ~Triangle() { }
    Triangle &operator=(const Triangle &ass) {
      if (this==&ass)
        return *this;
      ((Shape2*)this)->operator=((const Shape2 &)ass);
      int i=0;
      for (i=0;i<3;i++)
        m_t[i]=ass.m_t[i];
      return *this;
    }
    bool operator==(const Triangle &cmp) const {
      return ((const Shape2*)this)->operator==((const Shape2&)cmp);
    }
    bool operator<(const Triangle &cmp) const {
      return ((const Shape2*)this)->operator<((const Shape2&)cmp);
    }
    aux::Vector3 operator[](int idx) const {
      if (idx<=0)
        return m_t[0];
      if (idx>=2)
        return m_t[2];
      return m_t[1];
    }
    aux::TypeId *clone() const {
      return new Triangle((const Triangle &)(*this));
    }
    void set(aux::Vector3,aux::Vector3,aux::Vector3);
    void get(aux::Vector3 *t0,aux::Vector3 *t1,aux::Vector3 *t2) const {
      if (t0)
        *t0=m_t[0];
      if (t1)
        *t1=m_t[1];
      if (t2)
        *t2=m_t[2];
    }
    double circradius() const;
    aux::Vector3 center() const;
    int eval(aux::TVList<aux::Vector3> *,int npoints=-1);
    void toStringType(aux::Asciistr*) const;
    
};

class TriangleEq : public Shape2 {

  protected:
    double m_a;

  public:
    TriangleEq(double a=.0) : Shape2("triangleeq"),m_a(mk_isfinite(a) ? a : 0.) { }
    TriangleEq(const TriangleEq &ass) : Shape2((const Shape2 &)ass),m_a(ass.m_a) { }
    virtual ~TriangleEq() { }
    TriangleEq &operator=(const TriangleEq &ass) {
      if (this==&ass)
        return *this;
      ((Shape2*)this)->operator=((const Shape2 &)ass);
      m_a=ass.m_a;
      return *this;
    }
    bool operator==(const TriangleEq &cmp) const {
      return ((const Shape2*)this)->operator==((const Shape2&)cmp);
    }
    bool operator<(const TriangleEq &cmp) const {
      return ((const Shape2*)this)->operator<((const Shape2&)cmp);
    }
    double set(double a) {
      m_a=(mk_isfinite(a) ? a : 0.);
      m_points.clear();
      return m_a;
    }
    double get() const {
      return m_a;
    }
    int eval(aux::TVList<aux::Vector3> *,int npoints=-1);
    
};

} // namespace

#endif
