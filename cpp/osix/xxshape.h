
#ifndef _xxshape_h_
#define _xxshape_h_

#include <mkbase/mkmath.h>
#include <mkbase/mkla.h>

namespace osix {

class oswinexp xxRectSize {

  protected:
    double m_width;
    double m_height;

  public:
    xxRectSize(double ww=.0,double hh=.0) :
      m_width(mk_isbusted(ww) || ww<.0 ? .0 : ww),
      m_height(mk_isbusted(hh) || hh<.0 ? .0 : hh) {
    }
    xxRectSize(const xxRectSize &ass) : 
      m_width(ass.m_width),m_height(ass.m_height) {
    }
    ~xxRectSize() {
    }
    xxRectSize &operator=(const xxRectSize &);
    bool operator==(const xxRectSize &) const;
    bool operator<(const xxRectSize &) const;
    double setWidth(double);
    double setHeight(double);
    xxRectSize set(double ww=.0,double hh=.0);
    double width() const;
    double height() const;
    bool empty() const;
    int toString(mk_string) const;
    
};

class oswinexp xxLine {

  protected:
    mk_vertex m_p0;
    mk_vertex m_p1;
    
  public:
    xxLine();
    xxLine(const xxLine &);
    ~xxLine() {
    }
    xxLine &operator=(const xxLine &);
    bool operator==(const xxLine &) const;
    bool operator<(const xxLine &) const;
    void set(mk_vertex,mk_vertex);
    void setP0(mk_vertex);
    void setP1(mk_vertex);
    int p0(mk_vertex) const;
    int p1(mk_vertex) const;
    double len() const;
    double angdeg() const;
    double angrad() const;
    int busted() const;
    int empty() const;
    int toString(mk_string) const;

};

class oswinexp xxRect {

  protected:
    double m_r[4]; // left,top,right,bottom

  public:
    xxRect(double left=mk_dnan,double top=mk_dnan,double right=mk_dnan,double bottom=mk_dnan);
    xxRect(mk_vertex,mk_vertex);
    xxRect(double,double,xxRectSize);
    xxRect(mk_vertex,xxRectSize);
    xxRect(const xxRect &ass) {
      memcpy(&m_r[0],&ass.m_r[0],4*sizeof(double));
    }
    ~xxRect() {
    }
    xxRect &operator=(const xxRect &ass) {
      memcpy(&m_r[0],&ass.m_r[0],4*sizeof(double)); return *this;
    }
    xxRect operator+(double);
    xxRect operator-(double);
    bool operator==(const xxRect &) const;
    bool operator<(const xxRect &) const;
    double &operator[](int idx) {
      return m_r[idx];
    }
    double *raw() {
      return &m_r[0];
    }
    void set(double left=mk_dnan,double top=mk_dnan,double right=mk_dnan,double bottom=mk_dnan);
    void set(mk_vertex,mk_vertex);
    void set(double,double,xxRectSize);
    int busted() const;
    int empty() const;
    double left() const {
      return m_r[0];
    }
    double setLeft(double);
    double top() const {
      return m_r[1];
    }
    double setTop(double);
    double right() const {
      return m_r[2];
    }
    double setRight(double);
    double bottom() const {
      return m_r[3];
    }
    double setBottom(double);
    int leftTop(mk_vertex vv) const {
      vv[0]=m_r[0];
      vv[1]=m_r[1];
      return 0;
    }
    xxRectSize setLeftTop(mk_vertex);
    int leftBottom(mk_vertex vv) const {
      vv[0]=m_r[0];
      vv[1]=m_r[3];
      return 0;
    }
    xxRectSize setLeftBottom(mk_vertex);
    int rightTop(mk_vertex vv) const {
      vv[0]=m_r[2];
      vv[1]=m_r[1];
      return 0;
    }
    xxRectSize setRightTop(mk_vertex);
    int rightBottom(mk_vertex vv) const {
      vv[0]=m_r[2];
      vv[1]=m_r[3];
      return 0;
    }
    xxRectSize setRightBottom(mk_vertex);
    xxRectSize size() const {
      return (busted() ? xxRectSize() : xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]));
    }
    xxRectSize resize(double w=-1.,double h=-1.,unsigned char type=3); // type, 1:left 2:top 4:right 8:bottom any:center
    int center(mk_vertex vv) const {
      vv[0]=(m_r[0]+m_r[2])/2.;
      vv[1]=(m_r[3]+m_r[1])/2.;
      return 0;
    }
    int translate(double,double,unsigned char type=0); // type, 1:left 2:top 4:right 8:bottom any:center
    xxRect unite(const xxRect &) const;
    xxRect intersect(const xxRect &) const;
    double circ() const {
      return 2.*(m_r[2]-m_r[0]+m_r[3]-m_r[1]);
    }
    double area() const {
      return ((m_r[2]-m_r[0])*(m_r[3]-m_r[1]));
    }
    double scale(double,unsigned char type=0); // type, 1:left 2:top 4:right 8:bottom any:center
    void rotate(double,mk_vertex,mk_vertex,mk_vertex,mk_vertex) const;
    int toString(mk_string) const;
    
};

} // namespace

#endif
