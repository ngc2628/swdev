
#ifndef _rect_h_
#define _rect_h_

#include <auxx/auxx.h>
#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp RectSize {

  protected:
    double m_width;
    double m_height;

  public:
    RectSize(double w=.0,double h=.0) :
      m_width(aux::dbusted(w)!=0 || w<.0 ? .0 : w),
      m_height(aux::dbusted(h)!=0 || h<.0 ? .0 : h) { 
    }
    RectSize(const RectSize &ass) : m_width(ass.m_width),m_height(ass.m_height) {
    }
    ~RectSize() {
    }
    RectSize &operator=(const RectSize &ass) {
      m_width=ass.m_width;
      m_height=ass.m_height;
      return *this;
    }
    bool operator==(const RectSize &cmp) const {
      return (m_width==cmp.m_width && m_height==cmp.m_height);
    }
    bool operator<(const RectSize &cmp) const {
      return (m_width*m_height<cmp.m_width*cmp.m_height);
    }
    double setWidth(double w) {
      m_width=(aux::dbusted(w)!=0 || w<0. ? 0. : w);
      return m_width;
    }
    double setHeight(double h) {
      m_height=(aux::dbusted(h)!=0 || h<0. ? 0. : h);
      return m_height;
    }
    RectSize set(double w=.0,double h=.0) {
      m_width=(aux::dbusted(w)!=0 || w<0. ? 0. : w);
      m_height=(aux::dbusted(h)!=0 || h<0. ? 0. : h);
      return *this;
    }
    double width() const {
      return m_width;
    }
    double height() const {
      return m_height;
    }
    bool empty() const {
      return (m_width<=.0 || m_height<=.0);
    }
    void toString(aux::Asciistr *str) const {
      str->reserve(1024);
      aux::Asciistr numstr;
      str->append("w=");
      aux::d2a(m_width,&numstr,-1);
      str->append((const char*)numstr);
      str->append(" , h=");
      aux::d2a(m_height,&numstr,-1);
      str->append((const char*)numstr);
      str->append("\n");
    }

};

class oswinexp Rect : public Shape2 {

  protected:
    RectSize m_sz;
          
  public:
    Rect(RectSize sz=RectSize(.0,.0)) : Shape2("rect"),m_sz(sz) { }
    Rect(aux::Vector3,RectSize sz=RectSize(.0,.0));
    Rect(const Rect &ass) : Shape2((const Shape2 &)ass),m_sz(ass.m_sz) { }
    virtual ~Rect() { }
    Rect &operator=(const Rect &ass) {
      if (this==&ass)
        return *this;
      ((Shape2*)this)->operator=((const Shape2 &)ass); 
      m_sz=ass.m_sz; 
      return *this; 
    }
    bool operator==(const Rect &cmp) const {
      return ((const Shape2*)this)->operator==((const Shape2&)cmp);
    }
    bool operator<(const Rect &cmp) const {
      return ((const Shape2*)this)->operator<((const Shape2&)cmp);
    }
    aux::TypeId *clone() const {
      return new Rect((const Rect &)(*this));
    }
    RectSize size() const {
      return m_sz;
    }
    RectSize setSize(RectSize sz) {
      m_sz=sz;
      m_points.clear();
      return m_sz;
    }
    double rotate(double r) { 
      m_rotate=aux::dsign(r)*fmod(fabs(r),90.); 
      m_points.clear();
      return m_rotate; 
    }
    aux::Vector3 center() const {
      return aux::Vector3(.0,.0,.0);
    }
    double circradius() const {
      return sqrt(m_sz.width()*m_sz.width()+m_sz.height()*m_sz.height());
    }
    int eval(aux::TVList<aux::Vector3> *,int npoints=-1);
    void toStringType(aux::Asciistr*) const;

};

} // namespace

#endif
