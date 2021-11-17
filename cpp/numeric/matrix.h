
#ifndef _matrix_H_
#define _matrix_H_

#include <mkbase/mkla.h>

namespace num {

class oswinexp Matrix {

  protected:
    struct mk_matrix m_mat;
    bool operator==(const Matrix &) const {
      return false;
    }
    bool operator<(const Matrix &) const {
      return false;
    }

  public:
    Matrix(int rows=0,int cols=0);
    Matrix(const Matrix &);
    virtual ~Matrix();
    Matrix &operator=(const Matrix &);
    int rows() const;
    int cols() const;
    double getValue(int,int) const;
    virtual int setValue(int,int,double);
    virtual int clear();
    int reset(int identity=1);
    int transpose();
    int mult(Matrix *);
    int toString(mk_string) const;

};

class oswinexp SquareMatrix : public Matrix {

  protected:
    bool operator==(const SquareMatrix &cmp) const {
      return false;
    }
    bool operator<(const SquareMatrix &cmp) const {
      return false;
    }

  public:
    SquareMatrix(int rows=0);
    virtual ~SquareMatrix();
    SquareMatrix(const SquareMatrix &);
    SquareMatrix &operator=(const SquareMatrix &);
    int setValue(int,int,double);
    bool isIdentity(double) const;
    double determinant();
    int invert();
    int solveFor(int,double *,double *);

};

class oswinexp TransformMatrix : public SquareMatrix {

  protected:
    bool operator==(const TransformMatrix &) const {
      return false;
    }
    bool operator<(const TransformMatrix &) const {
      return false;
    }

  public:
    TransformMatrix();
    TransformMatrix(const TransformMatrix &);
    virtual ~TransformMatrix();
    TransformMatrix &operator=(const TransformMatrix &);
    int scale(double x=1.,double y=1.,double z=1.);
    int translate(double x=.0,double y=.0,double z=.0);
    int rotateX(double degrees=.0);
    int rotateY(double degrees=.0);
    int rotateZ(double degrees=.0);
    int shearXY(double x=.0,double y=.0);
    int shearXZ(double x=.0,double z=.0);
    int shearYZ(double y=.0,double z=.0);
    int transform(mk_vertex);

};

} // namespace

#endif
