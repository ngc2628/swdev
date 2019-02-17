
#ifndef _line_h_
#define _line_h_

#include <numeric/vertex.h>
#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Line2 : public Shape2 {

  protected:
    num::Vector3 m_l[2]; 
            
  public:
    Line2(num::Vector3 p0=num::Vector3(),num::Vector3 p1=num::Vector3()) : Shape2("Line2") {
      if (p0.busted(num::typeX|num::typeY)!=0 || p1.busted(num::typeX|num::typeY)!=0)
        p0=p1=num::Vector3(0.,0.);
      m_l[0]=p0;
      m_l[1]=p1;
    }
    Line2(const Line2 &ass) : Shape2((const Shape2 &)ass) {
      m_l[0]=ass.m_l[0];
      m_l[1]=ass.m_l[1];
    }
    virtual ~Line2() { }
    Line2 &operator=(const Line2 &ass) {
      ((Shape2*)this)->operator=((const Shape2 &)ass); 
      m_l[0]=ass.m_l[0];
      m_l[1]=ass.m_l[1];
      return *this;
    }
    bool operator==(const Line2 &cmp) const {
      return ((const Shape2*)this)->operator==((const Shape2&)cmp);
    }
    bool operator<(const Line2 &cmp) const {
      return ((const Shape2*)this)->operator<((const Shape2&)cmp);
    }
    operator bool() const {
      return !(m_l[0]==m_l[1]);
    }
    num::Vector3 operator[](int i) const {
      return m_l[i];
    }
    aux::TypeId *clone() const {
      return new Line2((const Line2 &)(*this));
    }
    double len() const { 
      double x=m_l[1][0]-m_l[0][0],y=m_l[1][1]-m_l[0][1];
      return sqrt(x*x+y*y); 
    }
    void set(num::Vector3 p0,num::Vector3 p1) {
      if (!p0.busted(num::typeX|num::typeY)!=0)
        m_l[0]=p0;
      if (!p1.busted(num::typeX|num::typeY)!=0)
        m_l[1]=p1;
    }
    void get(num::Vector3 *p0,num::Vector3 *p1) const {
      *p0=m_l[0];
      *p1=m_l[1];
    }
    double ang() const { num::Vector3 n(.0,1.),d=m_l[1]-m_l[0]; return d.angdeg(n); } 
    num::Vector3 center() const { 
      return num::Vector3((m_l[1][0]+m_l[0][0])/2.,(m_l[1][1]+m_l[0][1])/2.,.0); 
    }
    double circradius() const {
      return len()/2.;
    }
    int eval(num::VertexList *,int npoints=-1);
    int toStringType(mk_string) const;
    
};

} // namespace

#endif
