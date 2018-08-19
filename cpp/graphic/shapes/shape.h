
#ifndef _shape_h_
#define _shape_h_

#include <auxx/auxx.h>
#include <auxx/vertex.h>
#include <numeric/matrix.h>
#include <osix/xxstyle.h>

namespace shapes {

static const int numstdshapes2d=3;
static const char * const stdshapes2d[numstdshapes2d]={"rect","circle","triangle"};

class oswinexp Shape2 : public aux::TypeId {

  protected:
    double m_scale;
    double m_rotate;
    aux::Vector3 m_translate;
    aux::TVList<aux::Vector3> m_points;
    char m_descr[mk_idlen];
       
  public:
    osix::xxStyle m_styleO;
    osix::xxStyle m_styleF;
    Shape2(const char *type) : 
      aux::TypeId(type),m_scale(1.),m_rotate(.0),m_translate(aux::Vector3(0.,0.)) { 
      memset(&m_descr[0],0,mk_idlen);
    }
    Shape2(const Shape2 &ass) : 
      aux::TypeId((const aux::TypeId &)ass),m_scale(ass.m_scale),m_rotate(ass.m_rotate),
      m_translate(ass.m_translate),m_points(ass.m_points),m_styleO(ass.m_styleO),
      m_styleF(ass.m_styleF) { 
      memcpy(&m_descr[0],&ass.m_descr[0],mk_idlen);
    }
    virtual ~Shape2() { }
    Shape2 &operator=(const Shape2 &);
    bool operator==(const Shape2 &cmp) const {
      return ((const aux::TypeId*)this)->operator==((const aux::TypeId&)cmp);
    }
    bool operator<(const Shape2 &cmp) const {
      return ((const aux::TypeId*)this)->operator<((const aux::TypeId&)cmp);
    }
    virtual aux::TypeId *clone() const=0;
    double scale() const {
      return m_scale;
    }
    virtual double scale(double);
    double rotate() const {
      return m_rotate;
    }
    virtual double rotate(double); 
    aux::Vector3 translate() const {
      return m_translate;
    }
    virtual aux::Vector3 translate(aux::Vector3); 
    virtual aux::Vector3 center() const {
      return aux::Vector3(.0,.0,.0);
    }
    virtual double circradius() const {
      return .0;
    }
    virtual int eval(aux::TVList<aux::Vector3> *pointL,int npoints=-1)=0;
    void setDescr(const char *);
    const char * descr() {
      return &m_descr[0];
    }
    virtual void toString(aux::Asciistr*) const;
    virtual void toStringType(aux::Asciistr*) const { }

};

class oswinexp NoShape2 : public Shape2 {

  public:
    NoShape2() : Shape2("NoShape2") { }
    NoShape2(const NoShape2 &ass) : Shape2((const Shape2 &)ass) { }
    virtual ~NoShape2() { }
    NoShape2 &operator=(const NoShape2 &ass) {
      ((Shape2*)this)->operator=((const Shape2 &)ass); 
      return *this;
    }
    bool operator==(const NoShape2 &cmp) const {
      return ((const Shape2*)this)->operator==((const Shape2&)cmp);
    }
    bool operator<(const NoShape2 &cmp) const {
      return ((const Shape2*)this)->operator<((const Shape2&)cmp);
    }
    aux::TypeId *clone() const {
      return new NoShape2((const NoShape2 &)(*this));
    }
    int eval(aux::TVList<aux::Vector3> *pointL,int npoints=-1);
    
};

} // namespace

#endif
