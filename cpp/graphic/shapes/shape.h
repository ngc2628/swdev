
#ifndef _shape_h_
#define _shape_h_

#include <numeric/matrix.h>
#include <osix/xxstyle.h>
#include <mkbase/mkconv.h>

namespace shapes {

static const int numstdshapes2d=3;
static const char * const stdshapes2d[numstdshapes2d]={"rect","circle","triangle"};

class oswinexp Shape2 {

  protected:
    double m_scale;
    double m_rotate;
    mk_vertex m_translate;
    mk_string m_descr;
    osix::xxStyle m_styleO;
    osix::xxStyle m_styleF;
    struct mk_list m_vertices;

  public:
    Shape2(const char *);
    Shape2(const Shape2 &);
    virtual ~Shape2();
    Shape2 &operator=(const Shape2 &);
    bool operator==(const Shape2 &) const;
    bool operator<(const Shape2 &) const;
    virtual int clone(Shape2 *);
    virtual double getScale() const;
    virtual double setScale(double);
    virtual double getRotate() const;
    virtual double setRotate(double);
    virtual int getTranslate(mk_vertex) const;
    virtual int setTranslate(mk_vertex);
    virtual int getDescr(mk_string);
    virtual int setDescr(const char *);
    virtual osix::xxStyle getStyleO() const;
    virtual osix::xxStyle setStyleO(osix::xxStyle);
    virtual osix::xxStyle getStyleF() const;
    virtual osix::xxStyle setStyleF(osix::xxStyle);
    virtual int getVertex(int,mk_vertex) const;
    virtual int setVertex(int,mk_vertex);
    virtual int numVertices() const;
    virtual int center(mk_vertex) const;
    virtual double circradius() const;
    virtual int eval(struct mk_list *,int npoints=-1);
    virtual int toString(mk_string) const;

};

class oswinexp NoShape2 : public Shape2 {

  public:
    NoShape2() : Shape2("noshape") { 
    }
    NoShape2(const NoShape2 &ass) : Shape2((const Shape2 &)ass) { 
    }
    virtual ~NoShape2() { 
    }
    NoShape2 &operator=(const NoShape2 &);
    bool operator==(const NoShape2 &) const;
    bool operator<(const NoShape2 &) const;
    int eval(struct mk_list *,int npoints=-1);
    
};

} // namespace

#endif
