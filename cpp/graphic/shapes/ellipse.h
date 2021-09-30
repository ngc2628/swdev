
#ifndef _ellipse_h_
#define _ellipse_h_

#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Ellipse : public Shape2 {
      
  public:
    Ellipse();
    Ellipse(const Ellipse &);
    virtual ~Ellipse() { 
	  }
    Ellipse &operator=(const Ellipse &);
    bool operator==(const Ellipse &) const;
    bool operator<(const Ellipse &) const;
    int setVertex(int,mk_vertex);
    double ecc() const;
    double area() const;
    double circ() const;
    double circradius() const;
    int eval(struct mk_list *,int npoints=-1);

};

} // namespace

#endif
