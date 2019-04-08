
#ifndef _triangle_h_
#define _triangle_h_

#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Triangle : public Shape2 {

  public:
    Triangle();
    Triangle(const Triangle &);
    virtual ~Triangle() { }
    Triangle &operator=(const Triangle &);
    bool operator==(const Triangle &) const;
    bool operator<(const Triangle &) const;
    int setVertex(int,mk_vertex);
    double circradius() const;
    int center(mk_vertex) const;
    int eval(struct mk_list *,int npoints=-1);
    int toString(mk_string) const;
    
};

class TriangleEq : public Shape2 {

  public:
    TriangleEq(double a=.0);
    TriangleEq(const TriangleEq &);
    virtual ~TriangleEq() { 
    }
    TriangleEq &operator=(const TriangleEq &);
    bool operator==(const TriangleEq &) const;
    bool operator<(const TriangleEq &) const;
    int eval(struct mk_list *,int npoints=-1);
    
};

} // namespace

#endif
