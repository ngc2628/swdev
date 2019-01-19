
#ifndef interpolation2d_h
#define interpolation2d_h

#include <auxx/auxx.h>
#include <numeric/vertex.h>
#include <numeric/matrix.h>

namespace num {

const int numinterpolationtypes=7;
const int numinerpolationoptions=18;
extern int numsmoothIntermediates;

const mk_ulreal interpolation_none=0;
const mk_ulreal interpolation_const=1;
const mk_ulreal interpolation_linear=2;
const mk_ulreal interpolation_cubicspline=4;
const mk_ulreal interpolation_polynomial=8;
const mk_ulreal interpolation_bezier=16;
const mk_ulreal interpolation_bicubic=32;
const mk_ulreal interpolation_type=63;

const mk_ulreal interpolation_notappl=64;
const mk_ulreal interpolation_steps=128; /* #8 */
const mk_ulreal interpolation_fwd=256;
const mk_ulreal interpolation_bwd=512;
const mk_ulreal interpolation_lin1=1024;
const mk_ulreal interpolation_lin2=2048;
const mk_ulreal interpolation_solve1st=4096;
const mk_ulreal interpolation_solve2nd=8192;
const mk_ulreal interpolation_parametric=16384;
const mk_ulreal interpolation_notaknot=32768; /* #16 */
const mk_ulreal interpolation_nat=65536;
const mk_ulreal interpolation_periodic=131072;
const mk_ulreal interpolation_der1st=262144;
const mk_ulreal interpolation_der2nd=524288;
const mk_ulreal interpolation_monotonic=1048576;
const mk_ulreal interpolation_ctrl=2097152;
const mk_ulreal interpolation_eq=4194304;
const mk_ulreal interpolation_regr=8388608; /* #24 */
const mk_ulreal interpolation_options=16777152;

const char *const interpolationtypes[numinterpolationtypes]=
  {"none","const","linear","cubicspline","polynomial","bezier","bicubic"};
const char *const interpolationoptions[numinerpolationoptions]=
  {"notappl","steps","fwd","bwd","lin1","lin2","solve1st","solve2nd","parametric",
   "notaknot","nat","periodic","der1st","der2nd","monotonic","ctrl","eq","regr"};

class oswinexp Interpolation {

  protected:
    mk_ulreal m_options;
    int m_ready;
    VertexList m_ctrlL;
    VertexList m_coeffL;
    
  public:
    virtual ~Interpolation();
    mk_ulreal options() const;
    virtual int invalidate();
    virtual int clearCtrl();
    virtual int setCtrl(VertexList *);
    int nctrl() const;
    virtual int setup();
    virtual int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    virtual int interp(Vertex *) const;
    virtual int extrap(Vertex *) const;
    virtual int coeff(double,VertexList *);
    mk_ulreal setOptions(mk_ulreal options=0);

  protected:
    Interpolation(mk_ulreal);
    Interpolation(const Interpolation &) {
    }
    Interpolation &operator=(const Interpolation &) {
      return *this;
    }
    void clearArr();
    int setArr(VertexList *);

};

extern Interpolation* oswinexp buildInterpolation(mk_ulreal);
extern int oswinexp numInterpolIntermediates(Interpolation *);
extern aux::Asciistr oswinexp interpolation2string(mk_ulreal);

class oswinexp InterpolationConst : public Interpolation {

  public:
    InterpolationConst(VertexList *ctrlL=0);
    virtual ~InterpolationConst();
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;

  protected:
    InterpolationConst(const InterpolationConst &) : Interpolation(0) {
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
    InterpolationLinear(const InterpolationLinear &) : Interpolation(0) {
    }
    InterpolationLinear &operator=(const InterpolationLinear &) {
      return *this;
    }

};

class oswinexp CubicSpline : public Interpolation {

  protected:
    double *m_der;

  public:
    CubicSpline(mk_ulreal options=0);
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
    CubicSpline(const CubicSpline &) : Interpolation(0) {
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
    CubicSplineP(const CubicSplineP &) : Interpolation(0) {
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
    Polynomial(mk_ulreal options=0);
    virtual ~Polynomial();
    int invalidate();
		int setCtrl(VertexList *);
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;
    int coeff(double,VertexList *);
    int rootsBrute(double *,double,double,int *effdeg=0);

  protected:
    Polynomial(const Polynomial &) : Interpolation(0) {
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
    Bezier(const Bezier &) : Interpolation(0) {
    }
    Bezier &operator=(const Bezier &) {
      return *this;
    }

};

class BicubicPatch {

  public:
    mk_list m_der;
    SquareMatrix m_c;
    BicubicPatch();
    ~BicubicPatch();

  protected:
    BicubicPatch(const BicubicPatch &) {
    }
    BicubicPatch &operator=(const BicubicPatch &) {
      return *this;
    }

}; 

class oswinexp Bicubic : public Interpolation {

  public:
    Bicubic();
    virtual ~Bicubic();
    int setup();
    int interpol(int,VertexList *,double start=mk_dnan,double end=mk_dnan);
    int interp(Vertex *) const;

  protected:
    Bicubic(const Bicubic &) : Interpolation(0) {
    }
    Bicubic &operator=(const Bicubic &) {
      return *this;
    }

};

} // namespace

#endif
