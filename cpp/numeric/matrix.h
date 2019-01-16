
#ifndef _matrix_H_
#define _matrix_H_

#include <auxx/auxx.h>
#include <numeric/vertex.h>

namespace num {

class oswinexp Matrix {

  protected:
    int m_rows;
    int m_cols;
    double **m_m;
    void clearArr();
    int setArr(int,int,double **);
    int mult(int,int,double **);

  public:
    Matrix(int rows=0,int cols=0,double **m=0);
    Matrix(const Matrix &);
    virtual ~Matrix();
    Matrix &operator=(const Matrix &);
    bool operator==(const Matrix &) const;
    bool operator<(const Matrix &) const;
    int rows() const;
    int cols() const;
    double **data(int *rows=0,int *cols=0);
    double m(int,int);
    bool isIdentity(double);
    virtual void clearCtrl();
    virtual int setCtrl(int,int,double **);
    virtual void invalidate();
    void reset(bool zero=false);
    int alter(int, int, double);
    void transpose();
    int mult(Matrix *);
    void toString(aux::Asciistr *) const;

};

class oswinexp SquareMatrix : public Matrix {

  protected:
    double **m_lum;
    int *m_rowperm;
    double m_parity;
    int decomposition();
    int backsubstitution(double *,double *);

  public:
    SquareMatrix(int rows=0,double **m=0);
    virtual ~SquareMatrix();
    SquareMatrix(const SquareMatrix &);
    SquareMatrix &operator=(const SquareMatrix &);
    bool operator==(const SquareMatrix &cmp) const;
    bool operator<(const SquareMatrix &cmp) const;
    virtual void clearCtrl();
    virtual int setCtrl(int,int,double **);
    int setCtrl(int,double **);
    virtual void invalidate ();
    double determinant();
    int invert();
    int solveFor(int,double *,double *);

};

class oswinexp TransformMatrix : public SquareMatrix {

  public:
    TransformMatrix();
    TransformMatrix(const TransformMatrix &);
    virtual ~TransformMatrix();
    TransformMatrix &operator=(const TransformMatrix &);
    bool operator==(const TransformMatrix &) const;
    bool operator<(const TransformMatrix &) const;
    int scale(double x=1.,double y=1.,double z=1.);
    int translate(double x=.0,double y=.0,double z=.0);
    int rotateX(double degrees=.0);
    int rotateY(double degrees=.0);
    int rotateZ(double degrees=.0);
    int shearXY(double x=.0,double y=.0);
    int shearXZ(double x=.0,double z=.0);
    int shearYZ(double y=.0,double z=.0);
    int transform(Vertex *);

};

} // namespace

#endif
