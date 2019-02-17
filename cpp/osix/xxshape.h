
#ifndef _xxshape_h_
#define _xxshape_h_

#include <mkbase/mkmath.h>
#include <numeric/vertex.h>

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
    num::Vector3 m_l[2];
    
  public:
    xxLine(num::Vector3 p0=num::Vector3(),num::Vector3 p1=num::Vector3()) {
      set(p0,p1);
    }
    xxLine(const xxLine &ass) {
      m_l[0]=ass.m_l[0]; m_l[1]=ass.m_l[1];
    }
    ~xxLine() {
    }
    xxLine &operator=(const xxLine &ass) {
      m_l[0]=ass.m_l[0];
      m_l[1]=ass.m_l[1];
      return *this;
    }
    bool operator==(const xxLine &cmp) const {
      return (m_l[0]==cmp.m_l[0] && m_l[1]==cmp.m_l[1]);
    }
    bool operator<(const xxLine &cmp) const {
      return (mk_dlt(len(),cmp.len())==0 ? false : true);
    }
    num::Vector3 &operator[](int idx) {
      return m_l[idx];
    }
    void set(num::Vector3 p0=num::Vector3(),num::Vector3 p1=num::Vector3());
    void setP0(num::Vector3);
    void setP1(num::Vector3);
    num::Vector3 p0() const {
      return m_l[0];
    }
    num::Vector3 p1() const {
      return m_l[1];
    }
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
    xxRect(num::Vector3,num::Vector3);
    xxRect(double,double,xxRectSize);
    xxRect(num::Vector3,xxRectSize);
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
    void set(num::Vector3,num::Vector3);
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
    num::Vector3 leftTop() const {
      return num::Vector3(m_r[0],m_r[1]);
    }
    xxRectSize setLeftTop(num::Vector3);
    num::Vector3 leftBottom() const {
      return num::Vector3(m_r[0],m_r[3]);
    }
    xxRectSize setLeftBottom(num::Vector3);
    num::Vector3 rightTop() const {
      return num::Vector3(m_r[2],m_r[1]);
    }
    xxRectSize setRightTop(num::Vector3);
    num::Vector3 rightBottom() const {
      return num::Vector3(m_r[2],m_r[3]);
    }
    xxRectSize setRightBottom(num::Vector3);
    xxRectSize size() const {
      return (busted() ? xxRectSize() : xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]));
    }
    xxRectSize resize(double w=-1.,double h=-1.,unsigned char type=3); // type, 1:left 2:top 4:right 8:bottom any:center
    num::Vector3 center() const {
      return num::Vector3((m_r[0]+m_r[2])/2.,(m_r[3]+m_r[1])/2.);
    }
    num::Vector3 translate(double,double,unsigned char type=0); // type, 1:left 2:top 4:right 8:bottom any:center
    xxRect unite(const xxRect &) const;
    xxRect intersect(const xxRect &) const;
    double circ() const {
      return 2.*(m_r[2]-m_r[0]+m_r[3]-m_r[1]);
    }
    double area() const {
      return ((m_r[2]-m_r[0])*(m_r[3]-m_r[1]));
    }
    double scale(double,unsigned char type=0); // type, 1:left 2:top 4:right 8:bottom any:center
    void rotate(double,num::Vector3 *,num::Vector3 *,num::Vector3 *,num::Vector3 *) const;
    int toString(mk_string) const;
    
};

} // namespace

#endif
