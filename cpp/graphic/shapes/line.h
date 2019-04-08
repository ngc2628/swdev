
#ifndef _line_h_
#define _line_h_

#include <graphic/shapes/shape.h>

namespace shapes {

class oswinexp Line2 : public Shape2 {

  public:
    Line2();
    Line2(const Line2 &);
    virtual ~Line2() {
    }
    Line2 &operator=(const Line2 &);
    bool operator==(const Line2 &) const;
    bool operator<(const Line2 &) const;
    double len() const;
    double ang() const; 
    int center(mk_vertex) const;
    double circradius() const;
    int eval(struct mk_list *,int npoints=-1);
    int toString(mk_string) const;
    
};

} // namespace

#endif
