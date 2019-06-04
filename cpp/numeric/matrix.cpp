
#include <numeric/matrix.h>
#include <math.h>
#include <string.h>
#include <mkbase/mkutil.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>

namespace num {

Matrix::Matrix(int rows,int cols) {

  mk_matrixalloc(&m_mat,rows,cols);

}

Matrix::Matrix(const Matrix &ass) {

  mk_matrixalloc(&m_mat,0,0);
  mk_matrixcopy(&m_mat,&ass.m_mat);

}

Matrix::~Matrix() {

  mk_matrixfree(&m_mat);

}

Matrix &Matrix::operator=(const Matrix &ass) {

  if (&ass!=this) 
    mk_matrixcopy(&m_mat,&ass.m_mat);
  return *this;

}

int Matrix::rows() const {

  return m_mat.rows;

}

int Matrix::cols() const {

  return m_mat.cols;

}

double Matrix::getValue(int row,int col) const {

  return mk_matrixget(&m_mat,row,col);

}

int Matrix::setValue(int row,int col,double value) {

  int ii=0,jj=0,orr=m_mat.rows,occ=m_mat.cols;
  if (row>=orr || col>=occ) {
    struct mk_matrix cpmat;
    mk_matrixalloc(&cpmat,0,0);
    mk_matrixcopy(&cpmat,&m_mat);
    mk_matrixfree(&m_mat);
    mk_matrixalloc(&m_mat,(row>=orr ? row+1 : orr),(col>=occ ? col+1 : occ));
    for (ii=0;ii<orr;ii++) {
      for (jj=0;jj<occ;jj++)
        mk_matrixset(&m_mat,ii,jj,mk_matrixget(&cpmat,ii,jj));
    }
    mk_matrixfree(&cpmat);
  }
  int res=mk_matrixset(&m_mat,row,col,value);
  return res;

}

int Matrix::clear() {

  return mk_matrixfree(&m_mat);

}

int Matrix::reset(int identity) {

  return mk_matrixreset(&m_mat,identity);

}

int Matrix::transpose() {

  return mk_matrixtranspose(&m_mat);

}

int Matrix::mult(Matrix *mat) {

  struct mk_matrix mm;
  mk_matrixalloc(&mm,0,0);
  mk_matrixcopy(&mm,&m_mat);
  int res=mk_matrixmult((const struct mk_matrix*)&mm,(const struct mk_matrix*)&mat->m_mat,&m_mat);
  mk_matrixfree(&mm);
  return res;

}

int Matrix::toString(mk_string str) const {

  mk_string numstr;
  mk_stringset(numstr,0);
  int ii=0,jj=0;
  for (ii=0;ii<m_mat.rows;ii++) {
    for (jj=0;jj<m_mat.cols;jj++) {
      mk_d2str(getValue(ii,jj),numstr,8,'e',1,0,0);
      mk_stringappend(str,&numstr[0]);
      mk_stringappend(str,"  ");
    }
    mk_stringappend(str,"\n");
  }
  return 0;

}

SquareMatrix::SquareMatrix(int dim) : 
  Matrix(dim,dim) {

}

SquareMatrix::SquareMatrix(const SquareMatrix &ass) :
    Matrix(0,0) {

  mk_matrixcopy(&m_mat,&ass.m_mat);

}

SquareMatrix::~SquareMatrix() {

}

SquareMatrix &SquareMatrix::operator=(const SquareMatrix &ass) {

  if (this==&ass)
    return *this;
  mk_matrixcopy(&m_mat,&ass.m_mat);
  return *this;

}

int SquareMatrix::setValue(int row,int col,double value) {

  if ((row<m_mat.rows && col<m_mat.cols) || row==col)
    return Matrix::setValue(row,col,value);
  return 1;

}

bool SquareMatrix::isIdentity(double df) const {

  if (m_mat.rows!=m_mat.cols)
    return false;
  int ii=0,jj=0;
  double value=mk_dnan;
  for (ii=0;ii<m_mat.rows;ii++) {
    for (jj=0;jj<m_mat.cols;jj++) {
      value=mk_matrixget(&m_mat,ii,jj);
      if ((ii==jj && mk_diff(value,1.,df)!=.0) || (ii!=jj && mk_diff(value,.0,df)!=0))
        return false;
    }
  }
  return true;

}

double SquareMatrix::determinant() {
 
  double det=mk_matrixdet(&m_mat);
  return det;

}

int SquareMatrix::invert() {

  return mk_matrixinvert(&m_mat);

}

int SquareMatrix::solveFor(int num,double *rr,double *xx) {

  return mk_matrixsolve(&m_mat,rr,xx);

}

TransformMatrix::TransformMatrix() : SquareMatrix(4) {

  reset();

}

TransformMatrix::TransformMatrix(const TransformMatrix &ass) : 
  SquareMatrix(0) {

  mk_matrixcopy(&m_mat,&ass.m_mat);
  
}

TransformMatrix::~TransformMatrix() {

}

TransformMatrix &TransformMatrix::operator=(const TransformMatrix &ass) {

  if (&ass!=this)
    mk_matrixcopy(&m_mat,&ass.m_mat);
  return *this;

}

int TransformMatrix::translate(double xx,double yy,double zz) {

  TransformMatrix tm;
  tm.setValue(3,0,xx);
  tm.setValue(3,1,yy);
  tm.setValue(3,2,zz);
  int res=mult(&tm);
  return res;

}

int TransformMatrix::scale(double xx,double yy,double zz) {

  TransformMatrix tm;
  tm.setValue(0,0,xx);
  tm.setValue(1,1,yy);
  tm.setValue(2,2,zz);
  return mult(&tm);

}

int TransformMatrix::rotateZ(double degrees) {

  double radang=degrees*mk_rad;
  TransformMatrix tm;
  tm.setValue(0,0,cos(radang));
  tm.setValue(0,1,-sin(radang));
  tm.setValue(1,0,sin(radang));
  tm.setValue(1,1,cos(radang));
  return mult(&tm);

}

int TransformMatrix::rotateX(double degrees) {

  double radang=degrees*mk_rad;
  TransformMatrix tm;
  tm.setValue(1,1,cos(radang));
  tm.setValue(1,2,-sin(radang));
  tm.setValue(2,1,sin(radang));
  tm.setValue(2,2,cos(radang));
  return mult(&tm);

}

int TransformMatrix::rotateY(double degrees) {

  double radang=degrees*mk_rad;
  TransformMatrix tm;
  tm.setValue(0,0,cos(radang));
  tm.setValue(0,2,sin(radang));
  tm.setValue(2,0,-sin(radang));
  tm.setValue(2,2,cos(radang));
  return mult(&tm);

}

int TransformMatrix::shearXY(double xx,double yy) {

  TransformMatrix tm;
  tm.setValue(1,0,yy);
  tm.setValue(0,1,xx);
  return mult(&tm);

}

int TransformMatrix::shearXZ(double xx,double zz) {

  TransformMatrix tm;
  tm.setValue(0,2,xx);
  tm.setValue(2,0,zz);
  return mult(&tm);

}

int TransformMatrix::shearYZ(double yy,double zz) {

  TransformMatrix tm;
  tm.setValue(1,2,yy);
  tm.setValue(2,1,zz);
  return mult(&tm);

}

int TransformMatrix::transform(mk_vertex vertex) {

  mk_vertexnan(vv);
  mk_vertexcopy(vv,vertex);
  if (mk_isbusted(vv[0])!=0)
    vv[0]=.0;
  if (mk_isbusted(vv[1])!=0)
    vv[1]=.0;
  if (mk_isbusted(vv[2])!=0)
    vv[2]=.0;
  if (mk_isbusted(vv[3])!=0)
    vv[3]=1.;
  mk_vertexset(vertex,.0);
  int ii=0,jj=0;
  for (ii=0;ii<4;ii++) {
    for (jj=0;jj<4;jj++)
      vertex[ii]+=vv[jj]*getValue(jj,ii);
  }
  return 0;

}

} // namespace

