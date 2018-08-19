
#ifndef _vertex_H_
#define _vertex_H_

#include <mkbase/mkbase.h>
#include <auxx/auxx.h>

namespace aux {

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
    double m_xyzw[4];
  
  public:
    Vertex();
    Vertex(double);
    Vertex(double,double);
    Vertex(double,double,double);
    Vertex(double,double,double,double);
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
    void set(double,double,double,double);
    double len() const;
    double dot(const Vertex &) const;
    Vertex norm() const;
    Vertex cross(const Vertex &) const;
    double angrad(const Vertex &) const;
    double angdeg(const Vertex &) const;
    void toString(Asciistr *,int prec=mk_ddPrec) const;

};

extern double oswinexp lineRot(Vertex,Vertex,Vertex,Vertex,Vertex *);

extern double oswinexp mindistpointline(Vertex,Vertex,Vertex,Vertex *vi=0);

extern double oswinexp mindistpointplane(Vertex,Vertex,Vertex,Vertex,Vertex *vi=0);

extern int oswinexp cmpVectorI(const void *,const void *,int);

extern int oswinexp cmpVector0(const void *,const void *);

extern int oswinexp cmpVector1(const void *,const void *);

extern int oswinexp cmpVector2(const void *,const void *);

extern int oswinexp cmpVector3(const void *,const void *);

typedef int (*cmpVectorf)(const void *,const void *);

const cmpVectorf cmpVector[4]={cmpVector0,cmpVector1,cmpVector2,cmpVector3};

typedef Vertex Vector3;

} // namespace

#endif //_vertex_H_
