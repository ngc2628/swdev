
#ifndef interpolation2d_h
#define interpolation2d_h

#include <auxx/auxx.h>
#include <auxx/vertex.h>

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

const int numinterpolationtypes=6;
const char *const interpolationtypes[numinterpolationtypes]=
{"none","const","linear","spline","polynomial","bezier"};
const int numinerpolationoptions=18;
const char *const interpolationoptions[numinerpolationoptions]=
{"notappl","steps","fwd","bwd","lin1","lin2","solve1st","solve2nd","parametric",
 "notaknot","nat","periodic","der1st","der2nd","monotonic","ctrl","eq","regr"};

extern int numsmoothIntermediates;
extern void oswinexp ellipse(int n,double *x,double *y);

class oswinexp Interpolation {

  protected:
    aux::Asciistr m_type;
    double *m_x;
    double *m_y;
    double *m_z;
    int m_nctrl;
    int m_ready;
	  aux::TVList<aux::Asciistr> m_options;

  public:
    virtual ~Interpolation();
    void type(aux::Asciistr *) const;
    virtual int invalidate();
    virtual int clearCtrl();
    virtual int setCtrl(int nctrl=0,double *x=0,double *y=0,double *z=0);
    virtual int setCtrl(aux::TVList<aux::Vector3> *);
    int nctrl() const;
    virtual int setup();
    virtual int interpol(int,double *xint=0,double *yint=0,double *zint=0,
												 double start=mk_dnan,double end=mk_dnan);
    virtual int interpol(int,aux::TVList<aux::Vector3> *,
												 double start=mk_dnan,double end=mk_dnan);
    virtual double interp(double) const;
    virtual double extrap(double) const;
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
    int setArr(int,double *,double *,double *);
    int setArr(aux::TVList<aux::Vector3> *);

};

extern Interpolation* oswinexp buildInterpolation(const char *,aux::TVList<aux::Asciistr> *);
extern int oswinexp numInterpolIntermediates(Interpolation *);

class oswinexp InterpolationConst : public Interpolation {

  public:
    InterpolationConst(int nctrl=0,double *x=0,double *y=0);
    virtual ~InterpolationConst();
    int interpol(int,double *xint=0,double *yint=0,double *zint=0,
								 double start=mk_dnan,double end=mk_dnan);
    int interpol(int,aux::TVList<aux::Vector3> *,
								 double start=mk_dnan,double end=mk_dnan);
    double interp(double) const;

  protected:
    InterpolationConst(const InterpolationConst &) : Interpolation("none") {
    }
    InterpolationConst &operator=(const InterpolationConst &) {
      return *this;
    }

};

class oswinexp InterpolationLinear : public Interpolation {

  public:
    InterpolationLinear(int nctrl=0,double *x=0,double *y=0);
    virtual ~InterpolationLinear();
    int interpol(int,double *xint=0,double *yint=0,double *zint=0,
								 double start=mk_dnan,double end=mk_dnan);
    int interpol(int,aux::TVList<aux::Vector3> *,
								 double start=mk_dnan,double end=mk_dnan);
    double interp(double) const;

  protected:
    InterpolationLinear(const InterpolationLinear &) : Interpolation("none") {
    }
    InterpolationLinear &operator=(const InterpolationLinear &) {
      return *this;
    }

};

class oswinexp Spline : public Interpolation {

  protected:
    double *m_der;

  public:
    Spline(aux::TVList<aux::Asciistr> *optL=0);
    virtual ~Spline();
    int setup();
    int invalidate();
    int interpol(int,double *xint=0,double *yint=0,double *zint=0,
								 double start=mk_dnan,double end=mk_dnan);
    int interpol(int,aux::TVList<aux::Vector3> *,
								 double start=mk_dnan,double end=mk_dnan);
    double interp(double) const;
    double extrap(double) const;

    int makeSpline(double *der=0);
    int setSpline(double *der=0);
    int coeff(double,aux::TVList<double> *);

  protected:
    Spline(const Spline &) : Interpolation("none") {
    }
    Spline &operator=(const Spline &) {
      return *this;
    }
    int makeSpline1st();
    int makeSpline2nd();

};

class oswinexp SplineP : public Interpolation {

  protected:
    Spline *m_pXspl;
    Spline *m_pYspl;

  public:
    SplineP(int nctrl=0,double *x=0,double *y=0);
    virtual ~SplineP();
    int setup();
    int invalidate();
    int interpol(int,double *xint=0,double *yint=0,double *zint=0,
								 double start=mk_dnan,double end=mk_dnan);
    int interpol(int,aux::TVList<aux::Vector3> *,
								 double start=mk_dnan,double end=mk_dnan);
    double interp(double) const;

    int makeSpline(double *der1=0,double *der2=0);
    int setSpline(double *der1=0,double *der2=0);

  protected:
    SplineP(const SplineP &) : Interpolation("none") {
    }
    SplineP &operator=(const SplineP &) {
      return *this;
    }

};

class oswinexp Polynomial : public Interpolation {

  protected:
    double **m_c;
    double **m_d;
		aux::TVList<double> m_coeff;

  public:
    Polynomial(aux::TVList<aux::Asciistr> *optL=0);
    virtual ~Polynomial();
    int invalidate();
		int setCtrl(int nctrl=0,double *x=0,double *y=0,double *z=0);
    int setCtrl(aux::TVList<aux::Vector3> *);
    int interpol(int,double *xint=0,double *yint=0,double *zint=0,
								 double start=mk_dnan,double end=mk_dnan);
    int interpol(int,aux::TVList<aux::Vector3> *,
								 double start=mk_dnan,double end=mk_dnan);
    double interp(double) const;
    int coeff(double,aux::TVList<double> *);
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
    Bezier (int nctrl=0,double *x=0,double *y=0);
    virtual ~Bezier ();
    int interpol (int,double *xint=0, double *yint=0,double *zint=0,
									double start=mk_dnan,double end=mk_dnan);
    int interpol(int,aux::TVList<aux::Vector3> *,
								 double start=mk_dnan,double end=mk_dnan);
    double interp(double d) const {
      return d;
    }

  protected:
    Bezier(const Bezier &) : Interpolation("none") {
    }
    Bezier &operator=(const Bezier &) {
      return *this;
    }

};

} // namespace

#endif
