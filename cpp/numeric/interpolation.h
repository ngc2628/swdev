
#ifndef interpolation2d_h
#define interpolation2d_h

#include <numeric/matrix.h>
#include <mkbase/mkana.h>

namespace num {

const int numinterpolationtypes=7;
const int numinerpolationoptions=18;
extern int numsmoothIntermediates;

const mk_ulreal interpol_none=0;
const mk_ulreal interpol_regrdeg=(MK_SH(1,13))-1;          // mask 1fff
const mk_ulreal interpol_const=MK_SH(1,13);
const mk_ulreal interpol_linear=MK_SH(1,14);
const mk_ulreal interpol_cubicspline=MK_SH(1,15);
const mk_ulreal interpol_polynomial=MK_SH(1,16);
const mk_ulreal interpol_bezier=MK_SH(1,17);
const mk_ulreal interpol_bicubic=MK_SH(1,18);
const mk_ulreal interpol_type=MK_SH(63,13);   // mask 7e000
const mk_ulreal interpol_notappl=MK_SH(1,24);
const mk_ulreal interpol_steps=MK_SH(1,25);
const mk_ulreal interpol_fwd=MK_SH(1,26);
const mk_ulreal interpol_bwd=MK_SH(1,27);
const mk_ulreal interpol_lin1=MK_SH(1,28);
const mk_ulreal interpol_lin2=MK_SH(1,29);
const mk_ulreal interpol_solve1st=mk_spline_solve1st;
const mk_ulreal interpol_solve2nd=mk_spline_solve2nd;
const mk_ulreal interpol_parametric=MK_SH(1,32);
const mk_ulreal interpol_notaknot=mk_splineboundary_notaknot;
const mk_ulreal interpol_nat=mk_splineboundary_natural;
const mk_ulreal interpol_periodic=mk_splineboundary_periodic;
const mk_ulreal interpol_der1st=mk_splineboundary_der1st;
const mk_ulreal interpol_monotonic=mk_spline_monotonic;
const mk_ulreal interpol_der2nd=MK_SH(1,38);
const mk_ulreal interpol_ctrl=MK_SH(1,39);
const mk_ulreal interpol_regr=MK_SH(1,40);
const mk_ulreal interpol_options=MK_SH(262143,24);  // mask 3ffff000000

const char *const interpolationtypes[numinterpolationtypes]=
  {"none","const","linear","cubicspline","polynomial","bezier","bicubic"};
const char *const interpolationoptions[numinerpolationoptions]=
  {"notappl","steps","fwd","bwd","lin1","lin2","solve1st","solve2nd","parametric",
   "notaknot","nat","periodic","der1st","der2nd","monotonic","ctrl","eq","regr"};

class oswinexp Interpolation {

  protected:
    mk_ulreal m_options;
    struct mk_list m_ctrlL;
    Interpolation(mk_ulreal);
    Interpolation(const Interpolation &) {
    }
    Interpolation &operator=(const Interpolation &) {
      return *this;
    }
    
  public:
    virtual ~Interpolation();
    mk_ulreal options() const;
    virtual mk_ulreal setOptions(mk_ulreal,int acc=1);
    virtual int nctrl() const;
    virtual int clearCtrl();
    virtual int setCtrl(struct mk_list *);
    virtual int setup() {
      return 0;
    }
    virtual int coeff(double,double *) {
      return 1;
    }
    virtual int interp(mk_vertex) {
      return 1;
    }
    virtual int extrap(mk_vertex) {
      return 1;
    }
    virtual int interpol(int,struct mk_list *,double start=mk_dnan,double end=mk_dnan) {
      return 1;
    }

};

extern Interpolation* oswinexp buildInterpolation(mk_ulreal);
extern int oswinexp numInterpolIntermediates(Interpolation *);
extern int oswinexp interpolation2string(mk_ulreal,mk_string);

class oswinexp InterpolationConst : public Interpolation {

  protected:
    InterpolationConst(const InterpolationConst &) : 
      Interpolation(interpol_const) {
    }
    InterpolationConst &operator=(const InterpolationConst &) {
      return *this;
    }

  public:
    InterpolationConst(struct mk_list *ctrlL=0);
    virtual ~InterpolationConst();
    int interp(mk_vertex);
    int interpol(int,struct mk_list *,double start=mk_dnan,double end=mk_dnan);
   
};

class oswinexp InterpolationLinear : public Interpolation {

  protected:
    InterpolationLinear(const InterpolationLinear &) : 
      Interpolation(interpol_linear) {
    }
    InterpolationLinear &operator=(const InterpolationLinear &) {
      return *this;
    }
  
  public:
    InterpolationLinear(struct mk_list *ctrlL=0);
    virtual ~InterpolationLinear();
    int interp(mk_vertex);
    int interpol(int,struct mk_list *,double start=mk_dnan,double end=mk_dnan);
    
};

class oswinexp CubicSpline : public Interpolation {

  protected:
    struct mk_spline m_spline;
    CubicSpline(const CubicSpline &) : 
      Interpolation(interpol_cubicspline|interpol_solve2nd|interpol_nat) {
    }
    CubicSpline &operator=(const CubicSpline &) {
      return *this;
    }

  public:
    CubicSpline(
      mk_ulreal options=interpol_cubicspline|interpol_solve2nd|interpol_nat);
    virtual ~CubicSpline();
    mk_ulreal setOptions(mk_ulreal,int acc=1);
    int clearCtrl();
		int setCtrl(struct mk_list *);
    int setup();
    int coeff(double,double *);
    int interp(mk_vertex);
    int extrap(mk_vertex);
    int interpol(int,struct mk_list *,double start=mk_dnan,double end=mk_dnan);
    
};

class oswinexp CubicSplineP : public Interpolation {

  protected:
    struct mk_spline m_xspline;
    struct mk_spline m_yspline;
    CubicSplineP(const CubicSplineP &) : 
      Interpolation(
        interpol_cubicspline|interpol_parametric|
        interpol_solve2nd|interpol_nat) {
    }
    CubicSplineP &operator=(const CubicSplineP &) {
      return *this;
    }

  public:
    CubicSplineP();
    virtual ~CubicSplineP();
    mk_ulreal setOptions(mk_ulreal,int acc=1);
    int clearCtrl();
		int setCtrl(struct mk_list *);
    int setup();
    int interpol(int,struct mk_list *,double start=mk_dnan,double end=mk_dnan);

};

class oswinexp Polynomial : public Interpolation {

  protected:
    struct mk_polynomial m_polynomial;
    Polynomial(const Polynomial &) : Interpolation(interpol_polynomial) {
    }
    Polynomial &operator=(const Polynomial &) {
      return *this;
    }
    int canRegr();

  public:
    Polynomial(mk_ulreal options=interpol_polynomial);
    virtual ~Polynomial();
    mk_ulreal setOptions(mk_ulreal,int acc=1);
    int clearCtrl();
		int setCtrl(struct mk_list *);
    int coeff(double,double *);
    int interpol(int,struct mk_list *,double start=mk_dnan,double end=mk_dnan);
    int rootsBrute(double *,double,double,int *effdeg=0);

};

class oswinexp Bezier : public Interpolation {

  protected:
    Bezier(const Bezier &) : Interpolation(interpol_bezier) {
    }
    Bezier &operator=(const Bezier &) {
      return *this;
    }

  public:
    Bezier(struct mk_list *ctrlL=0);
    virtual ~Bezier ();
    int interp(mk_vertex) {
      return 0;
    }
    int interpol(int,struct mk_list *,double start=mk_dnan,double end=mk_dnan);

};

class BicubicPatch {

  protected:
    BicubicPatch(const BicubicPatch &) {
    }
    BicubicPatch &operator=(const BicubicPatch &) {
      return *this;
    }
  
  public:
    struct mk_list m_der;
    SquareMatrix m_c;
    BicubicPatch();
    ~BicubicPatch();

}; 

class oswinexp Bicubic : public Interpolation {

  protected:
    Bicubic(const Bicubic &) : Interpolation(interpol_bicubic) {
    }
    Bicubic &operator=(const Bicubic &) {
      return *this;
    }

  public:
    Bicubic();
    virtual ~Bicubic();
    int interp(mk_vertex);
    int interpol(int,struct mk_list *,double start=mk_dnan,double end=mk_dnan);
   
};

} // namespace

#endif
