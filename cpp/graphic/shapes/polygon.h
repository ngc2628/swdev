
#ifndef _polygon_h_
#define _polygon_h_

#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Polygon2 : public Shape2 {

  public:
    Polygon2(int npoints=0);
    Polygon2(const Polygon2 &);
    virtual ~Polygon2() {
    }
    Polygon2 &operator=(const Polygon2 &);
    bool operator==(const Polygon2 &) const;
    bool operator<(const Polygon2 &) const;
    int setVertex(int,mk_vertex);
    int eval(struct mk_list *,int npoints=-1);
        
};

} // namespace

#endif
