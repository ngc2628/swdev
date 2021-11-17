
#ifndef _rect_h_
#define _rect_h_

#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp RectSize {

  protected:
    double m_width;
    double m_height;

  public:
    RectSize(double ww=.0,double hh=.0);
    RectSize(const RectSize &);
    ~RectSize() {
    }
    RectSize &operator=(const RectSize &);
    bool operator==(const RectSize &) const;
    bool operator<(const RectSize &) const;
    double setWidth(double);
    double setHeight(double);
    double getWidth() const;
    double getHeight() const;
    bool empty() const;
    int toString(mk_string) const;

};

class oswinexp Rect : public Shape2 {

  protected:
    RectSize m_sz;
          
  public:
    Rect(RectSize sz=RectSize());
    Rect(const Rect &);
    virtual ~Rect() { 
    }
    Rect &operator=(const Rect &);
    bool operator==(const Rect &) const;
    bool operator<(const Rect &) const;
    RectSize getSize() const;
    RectSize setSize(RectSize);
    double setRotate(double);
    int center(mk_vertex) const;
    double circradius() const;
    int eval(struct mk_list *,int npoints=-1);
    int toString(mk_string) const;

};

} // namespace

#endif
