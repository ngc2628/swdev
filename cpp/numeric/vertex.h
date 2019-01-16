
#ifndef _vertex_H_
#define _vertex_H_

#include <mkbase/mkla.h>
#include <auxx/auxx.h>

namespace num {

const int typeX=1;
const int typeY=2;
const int typeXY=3;
const int typeZ=4;
const int typeXZ=5;
const int typeYZ=6;
const int typeXYZ=7;
const int typeW=8;
const int typeXW=9;
const int typeYW=10;
const int typeXYW=11;
const int typeZW=12;
const int typeXZW=13;
const int typeYZW=14;
const int typeXYZW=15;

class oswinexp Vertex {
  
  protected:
    mk_vertex m_tuple;
  
  public:
    Vertex();
    Vertex(double);
    Vertex(double,double);
    Vertex(double,double,double);
    Vertex(double *);
    Vertex(const Vertex &);
    ~Vertex();
    Vertex &operator=(const Vertex &);
    bool operator==(const Vertex &) const;
    bool operator<(const Vertex &) const;
    double operator[](int) const;
    Vertex operator+(const Vertex &) const;
    Vertex operator-(const Vertex &) const;
    Vertex operator*(double) const;
    Vertex operator/(double) const;
    int busted(int) const;
    double *data();
    double x() const;
    void setX(double);
    double y() const;
    void setY(double);
    double z() const;
    void setZ(double);
    double w() const;
    void setW(double);
    void setXY(double,double);
    void setXYZ(double,double,double);
    void set(double *);
    double len() const;
    double dot(const Vertex &) const;
    Vertex norm() const;
    Vertex cross(const Vertex &) const;
    double angrad(const Vertex &) const;
    double angdeg(const Vertex &) const;
    void toString(aux::Asciistr *,int prec=3) const;

};

extern double oswinexp lineRot(Vertex,Vertex,Vertex,Vertex,Vertex *);

extern double oswinexp mindistpointline(Vertex,Vertex,Vertex,Vertex *vi=0);

extern double oswinexp mindistpointplane(Vertex,Vertex,Vertex,Vertex,Vertex *vi=0);

typedef Vertex Vector3;

class oswinexp VertexList {

  protected:
    mk_list m_list;
    
  public:
    VertexList(int sz=0);
    VertexList(const VertexList &);
    virtual ~VertexList() { }
    VertexList &operator=(const VertexList &);
    bool operator==(const VertexList &) const;
    bool operator<(const VertexList &) const;
    Vertex get(int,int *inlist=0) const;
    Vertex operator[](int idx) const {
      return get(idx);
    }
    int set(int,Vertex,int);
    int append(Vertex);
    int clear();
    int resize(int);
    int count(int *reserved=0) const;
    int sort(int);
    int sorted() const;
    int find(Vertex) const;
    int findNextIndex(Vertex) const;
    int findInterval(Vertex,int *,int*) const;
    int inSort(Vertex);
    int remove(int); 

};

} // namespace

#endif //_vertex_H_
