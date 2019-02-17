
#ifndef _rect_h_
#define _rect_h_

#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp RectSize {

  protected:
    double m_width;
    double m_height;

  public:
    RectSize(double w=.0,double h=.0) :
      m_width(mk_isbusted(w)!=0 || w<.0 ? .0 : w),
      m_height(mk_isbusted(h)!=0 || h<.0 ? .0 : h) { 
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
    bool operator==(const RectSize &) const;
    bool operator<(const RectSize &) const;
    double setWidth(double);
    double setHeight(double);
    RectSize set(double w=.0,double h=.0);
    double width() const {
      return m_width;
    }
    double height() const {
      return m_height;
    }
    bool empty() const;
    int toString(mk_string) const;

};

class oswinexp Rect : public Shape2 {

  protected:
    RectSize m_sz;
          
  public:
    Rect(RectSize sz=RectSize(.0,.0)) : Shape2("rect"),m_sz(sz) { }
    Rect(num::Vector3,RectSize sz=RectSize(.0,.0));
    Rect(const Rect &ass) : Shape2((const Shape2 &)ass),m_sz(ass.m_sz) { }
    virtual ~Rect() { }
    Rect &operator=(const Rect &);
    bool operator==(const Rect &) const;
    bool operator<(const Rect &) const;
    aux::TypeId *clone() const;
    RectSize size() const {
      return m_sz;
    }
    RectSize setSize(RectSize);
    double rotate(double);
    num::Vector3 center() const;
    double circradius() const;
    int eval(num::VertexList *,int npoints=-1);
    int toStringType(mk_string) const;

};

} // namespace

#endif
