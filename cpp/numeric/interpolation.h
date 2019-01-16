
#ifndef interpolation2d_h
#define interpolation2d_h

#include <auxx/auxx.h>
#include <numeric/vertex.h>
#include <numeric/matrix.h>

// types :
// none=0
// const=1
// histogram=2
// linear=4
// spline=8
// splinep=16
// bezier=32
// polynomial=64
// polynomialeq=128
// unknown=65536

// subtypes :
// backward=131072
// forward=262144

namespace num {

const int numinterpolationtypes=7;
const char *const interpolationtypes[numinterpolationtypes]=
{"none","const","linear","cubicspline","polynomial","bezier","bicubic"};
const int numinerpolationoptions=18;
const char *const interpolationoptions[numinerpolationoptions]=
{"notappl","steps","fwd","bwd","lin1","lin2","solve1st","solve2nd","parametric",
 "notaknot","nat","periodic","der1st","der2nd","monotonic","ctrl","eq","regr"};

extern int numsmoothIntermediates;

class oswinexp Interpolation {

  protected:
    int m_ready;
    VertexList m_ctrlL;
    VertexList m_coeffL;
    aux::Asciistr m_type;
	  aux::TVList<aux::Asciistr> m_options;

  public:
    virtual ~Interpolation();
    void type(aux::Asciistr *) const;
    virtual int invalidate();
    virtual int clearCtrl();
    virtual int setCtrl(VertexList *);
    int nctrl() const;
    virtual int setup();
    virtual int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    virtual int interp(Vertex *) const;
    virtual int extrap(Vertex *) const;
    virtual int coeff(double,VertexList *);
    int options(aux::TVList<aux::Asciistr> *optL=0) const;
    int setOptions(aux::TVList<aux::Asciistr> *optL=0,int clr=0);

  protected:
    Interpolation(const char *,aux::TVList<aux::Asciistr> *optL=0);
    Interpolation(const Interpolation &) {
    }
    Interpolation &operator=(const Interpolation &) {
      return *this;
    }
    void clearArr();
    int setArr(VertexList *);

};

extern Interpolation* oswinexp buildInterpolation(const char *,aux::TVList<aux::Asciistr> *);
extern int oswinexp numInterpolIntermediates(Interpolation *);

class oswinexp InterpolationConst : public Interpolation {

  public:
    InterpolationConst(VertexList *ctrlL=0);
    virtual ~InterpolationConst();
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;

  protected:
    InterpolationConst(const InterpolationConst &) : Interpolation("none") {
    }
    InterpolationConst &operator=(const InterpolationConst &) {
      return *this;
    }

};

class oswinexp InterpolationLinear : public Interpolation {

  public:
    InterpolationLinear(VertexList *ctrlL=0);
    virtual ~InterpolationLinear();
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;

  protected:
    InterpolationLinear(const InterpolationLinear &) : Interpolation("none") {
    }
    InterpolationLinear &operator=(const InterpolationLinear &) {
      return *this;
    }

};

class oswinexp CubicSpline : public Interpolation {

  protected:
    double *m_der;

  public:
    CubicSpline(aux::TVList<aux::Asciistr> *optL=0);
    virtual ~CubicSpline();
    int setup();
    int invalidate();
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;
    int extrap(Vertex *) const;
    int makeSpline(double *der=0);
    int setSpline(double *der=0);
    int coeff(double,VertexList *);

  protected:
    CubicSpline(const CubicSpline &) : Interpolation("none") {
    }
    CubicSpline &operator=(const CubicSpline &) {
      return *this;
    }
    int makeSpline1st();
    int makeSpline2nd();

};

class oswinexp CubicSplineP : public Interpolation {

  protected:
    CubicSpline *m_xspline;
    CubicSpline *m_yspline;

  public:
    CubicSplineP(VertexList *ctrlL=0);
    virtual ~CubicSplineP();
    int setup();
    int invalidate();
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;
    int makeSpline(double *der1=0,double *der2=0);
    int setSpline(double *der1=0,double *der2=0);

  protected:
    CubicSplineP(const CubicSplineP &) : Interpolation("none") {
    }
    CubicSplineP &operator=(const CubicSplineP &) {
      return *this;
    }

};

class oswinexp Polynomial : public Interpolation {

  protected:
    double **m_c;
    double **m_d;

  public:
    Polynomial(aux::TVList<aux::Asciistr> *optL=0);
    virtual ~Polynomial();
    int invalidate();
		int setCtrl(VertexList *);
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;
    int coeff(double,VertexList *);
    int rootsBrute(double *,double,double,int *effdeg=0);

  protected:
    Polynomial(const Polynomial &) : Interpolation("none") {
    }
    Polynomial &operator=(const Polynomial &) {
      return *this;
    }
    int prepTable();
		int fitRegr(int);

};

class oswinexp Bezier : public Interpolation {

  public:
    Bezier(VertexList *ctrlL=0);
    virtual ~Bezier ();
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const {
      return 0;
    }

  protected:
    Bezier(const Bezier &) : Interpolation("none") {
    }
    Bezier &operator=(const Bezier &) {
      return *this;
    }

};

class BicubicPatch {

  public:
    mk_list m_der;
    SquareMatrix m_c;
    BicubicPatch ();
    ~BicubicPatch ();

  protected:
    BicubicPatch(const BicubicPatch &) {
    }
    BicubicPatch &operator=(const BicubicPatch &) {
      return *this;
    }

}; 

class oswinexp Bicubic : public Interpolation {

  public:
    Bicubic ();
    virtual ~Bicubic ();
    int setup();
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;

  protected:
    Bicubic(const Bicubic &) : Interpolation("none") {
    }
    Bicubic &operator=(const Bicubic &) {
      return *this;
    }

};

} // namespace

#endif
