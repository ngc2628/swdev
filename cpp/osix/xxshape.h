
#ifndef _xxshape_h_
#define _xxshape_h_

#include <auxx/auxx.h>
#include <auxx/vertex.h>

namespace osix {

class oswinexp xxRectSize {

  protected:
    double m_width;
    double m_height;

  public:
    xxRectSize(double w=.0,double h=.0) :
      m_width(aux::dbusted(w) || w<.0 ? .0 : w),
      m_height(aux::dbusted(h) || h<.0 ? .0 : h) {
    }
    xxRectSize(const xxRectSize &ass) : m_width(ass.m_width),m_height(ass.m_height) {
    }
    ~xxRectSize() {
    }
    xxRectSize &operator=(const xxRectSize &ass) {
      m_width=ass.m_width;
      m_height=ass.m_height;
      return *this;
    }
    bool operator==(const xxRectSize &cmp) const {
      return (m_width==cmp.m_width && m_height==cmp.m_height);
    }
    bool operator<(const xxRectSize &cmp) const {
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
    xxRectSize set(double w=.0,double h=.0) {
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

class oswinexp xxLine {

  protected:
    aux::Vector3 m_l[2];
    
  public:
    xxLine(aux::Vector3 p0=aux::Vector3(),aux::Vector3 p1=aux::Vector3()) {
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
      return (aux::dlt(len(),cmp.len())==0 ? false : true);
    }
    aux::Vector3 &operator[](int idx) {
      return m_l[idx];
    }
    void set(aux::Vector3 p0=aux::Vector3(),aux::Vector3 p1=aux::Vector3()) {
      if (aux::dlt(p1.x(),p0.x()))
        aux::swap(&p0,&p1);
      m_l[0]=p0; m_l[1]=p1;
    }
    void setP0(aux::Vector3);
    void setP1(aux::Vector3);
    aux::Vector3 p0() const {
      return m_l[0];
    }
    aux::Vector3 p1() const {
      return m_l[1];
    }
    double len() const;
    double angdeg() const;
    double angrad() const;
    int busted() const;
    int empty() const;
    void toString(aux::Asciistr *) const;

};

class oswinexp xxRect {

  protected:
    double m_r[4]; // left,top,right,bottom

  public:
    xxRect(double left=aux::dnan,double top=aux::dnan,double right=aux::dnan,double bottom=aux::dnan);
    xxRect(aux::Vector3,aux::Vector3);
    xxRect(double,double,xxRectSize);
    xxRect(aux::Vector3,xxRectSize);
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
    void set(double left=aux::dnan,double top=aux::dnan,double right=aux::dnan,double bottom=aux::dnan);
    void set(aux::Vector3,aux::Vector3);
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
    aux::Vector3 leftTop() const {
      return aux::Vector3(m_r[0],m_r[1]);
    }
    xxRectSize setLeftTop(aux::Vector3);
    aux::Vector3 leftBottom() const {
      return aux::Vector3(m_r[0],m_r[3]);
    }
    xxRectSize setLeftBottom(aux::Vector3);
    aux::Vector3 rightTop() const {
      return aux::Vector3(m_r[2],m_r[1]);
    }
    xxRectSize setRightTop(aux::Vector3);
    aux::Vector3 rightBottom() const {
      return aux::Vector3(m_r[2],m_r[3]);
    }
    xxRectSize setRightBottom(aux::Vector3);
    xxRectSize size() const {
      return (busted() ? xxRectSize() : xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]));
    }
    xxRectSize resize(double w=-1.,double h=-1.,unsigned char type=3); // type, 1:left 2:top 4:right 8:bottom any:center
    aux::Vector3 center() const {
      return aux::Vector3((m_r[0]+m_r[2])/2.,(m_r[3]+m_r[1])/2.);
    }
    aux::Vector3 translate(double,double,unsigned char type=0); // type, 1:left 2:top 4:right 8:bottom any:center
    xxRect unite(const xxRect &) const;
    xxRect intersect(const xxRect &) const;
    double circ() const {
      return 2.*(m_r[2]-m_r[0]+m_r[3]-m_r[1]);
    }
    double area() const {
      return ((m_r[2]-m_r[0])*(m_r[3]-m_r[1]));
    }
    double scale(double,unsigned char type=0); // type, 1:left 2:top 4:right 8:bottom any:center
    void rotate(double,aux::Vector3 *,aux::Vector3 *,aux::Vector3 *,aux::Vector3 *) const;
    void toString(aux::Asciistr *) const;
    
};

} // namespace

#endif