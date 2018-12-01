
#include <numeric/matrix.h>
#include <math.h>
#include <string.h>

using namespace aux;

namespace num {

Matrix::Matrix(int rows,int cols,double **m) : m_rows(0),m_cols(0),m_m(0) {

  setArr(rows,cols,m);

}

Matrix::Matrix(const Matrix &ass)  : m_rows(0),m_cols(0),m_m(0) {

  setArr(ass.m_rows,ass.m_cols,ass.m_m);

}

Matrix::~Matrix() {

  clearArr();

}

Matrix &Matrix::operator=(const Matrix &ass) {

  if (this==&ass)
    return *this;
  setArr(ass.m_rows,ass.m_cols,ass.m_m);
  return *this;

}

bool Matrix::operator==(const Matrix &cmp) const {

  if (m_rows==cmp.m_rows && m_cols==cmp.m_cols) {
    int ii=0,jj=0;
    for (ii=0;ii<m_rows;ii++) {
      for (jj=0;jj<m_cols;jj++) {
        if (m_m[ii][jj]!=cmp.m_m[ii][jj])
          return false;
      }
    }
    return true;
  }
  return false;

}

bool Matrix::operator<(const Matrix &cmp) const {

  if (m_rows==cmp.m_rows && m_cols==cmp.m_cols) {
    int ii=0,jj=0;
    for (ii=0;ii<m_rows;ii++) {
      for (jj=0;jj<m_cols;jj++) {
        if (m_m[ii][jj]<cmp.m_m[ii][jj])
          return true;
      }
    }
    return true;
  }
  return (m_rows<cmp.m_rows || (m_rows==cmp.m_rows && m_cols<cmp.m_cols));

}

void Matrix::clearArr() {

  int ii=0;
  if (m_m) {
    for (ii=0;ii<m_rows;ii++)
      delete [] m_m[ii];
    delete [] m_m;
  }
  m_m=0;
  m_rows=m_cols=0;

}

int  Matrix::setArr(int rows,int cols,double **m) {

  clearArr();
  if (rows<1 || cols<1)
    return -1; // that does not look like a matrix --> out
  int ii=0,jj=0;
  m_rows=rows;
  m_cols=cols;
  m_m=new double*[(size_t)rows];
  for (ii=0;ii<rows;ii++) {
    m_m[ii]=new double[(size_t)cols];
    // make identity matrix (if quadratic, else set subdiagonal to identity)
    for (jj=0;jj<cols;jj++)
      m_m[ii][jj]=(m ? m[ii][jj] : (ii==jj ? 1. : .0));
  }
  return 0;

}

int Matrix::rows() const {

  return m_rows;

}

int Matrix::cols() const {

  return m_cols;

}

double **Matrix::data(int *rows,int *cols) {

  if (rows)
    *rows=m_rows;
  if (cols)
    *cols=m_cols;
  return m_m;

}

double Matrix::m(int row,int col) {

  if (!m_m || row<0 || row>=m_rows || col<0 || col>=m_cols)
    return .0;
  return m_m[row][col];

}

bool Matrix::isIdentity(double df) {

  if (!m_m || m_rows!=m_cols)
    return false;
  int ii=0,jj=0;
  for (ii=0;ii<m_rows;ii++) {
    for (jj=0;jj<m_cols;jj++) {
      if ((ii==jj && mk_diff(m_m[ii][jj],1.,df)!=.0) || (ii!=jj && mk_diff(m_m[ii][jj],.0,df)!=0))
        return false;
    }
  }
  return true;

}

void Matrix::clearCtrl() {

  clearArr();

}

int Matrix::setCtrl(int rows,int cols,double **m) {

  return setArr(rows,cols,m);

}

void Matrix::invalidate() {

}

void Matrix::reset(bool zero) {

  int ii=0,jj=0;
  for (ii=0;ii<m_rows;ii++) {
    if (zero)
      memset(m_m[ii],0,m_cols*sizeof(double));
    else {
      for (jj=0;jj<m_cols;jj++)
        m_m[ii][jj]=(ii==jj ? 1. : .0);
    }
  }

}

int Matrix::alter(int row, int col, double value) {

  if (row<0 || col<0)
    return -1;
  int ii=0;
  // do we have to expand the data structure ? ...
  bool addrows=(row>=m_rows || !m_m),addcols=(col>=m_cols || !m_m);
  // ... if yes do so (copy all contents into a temorary matrix and copy back afterwards)
  if (addrows || addcols) {
    int rows=m_rows,cols=m_cols;
    Matrix *cpm=0;
    if (m_m)
      cpm=new Matrix(rows,cols,m_m);
    if (!setArr(addrows ? row+1 : rows,addcols ? col+1 : cols,0)) {
      if (cpm)
        delete cpm;
      return -1;
    }
    if (cpm) {
      for (ii=0;ii<rows;ii++)
        memcpy(m_m[ii],cpm->m_m[ii],cols*sizeof(double));
      delete cpm;
    }
  }
  // set the value now ...
  m_m[row][col]=value;
  // ... and invalidate all post calculated data
  invalidate();
  return 0;

}

void Matrix::transpose() {

  if (!m_m)
    return;
  Matrix cpm(m_rows,m_cols,m_m);
  if (m_rows!=m_cols)
    setArr(m_cols,m_rows,0);
  int ii=0,jj=0;
  for (ii=0;ii<m_rows;ii++) {
    for (jj=0;jj<m_cols;jj++)
      m_m[ii][jj]=cpm.m_m[jj][ii];
  }

}

int Matrix::mult(int rows,int cols,double **m) {

  if (!m_m || m_cols!=rows)
    return -1;
  int ii=0,jj=0,kk=0,myrows=m_rows;
  double **res=new double*[(size_t)m_rows];
  for (ii=0;ii<m_rows;ii++) {
    res[ii]=new double[(size_t)cols];
    memset(&res[ii][0],0,cols*sizeof(double));
  }
  for (ii=0;ii<m_rows;ii++) {
    for (jj=0;jj<cols;jj++) {
      for (kk=0;kk<m_cols;kk++)
        res[ii][jj]+=m_m[ii][kk]*m[kk][jj];
    }
  }
  invalidate();
  clearArr();
  m_rows=myrows;
  m_cols=cols;
  m_m=res;
  return 0;

}

int Matrix::mult(Matrix *matrix) {

  if (!matrix || !m_m || m_cols!=matrix->m_rows)
    return -1;
  int ii=0,jj=0,kk=0,myrows=m_rows;
  double **res=new double*[(size_t)m_rows];
  for (ii=0;ii<m_rows;ii++) {
    res[ii]=new double[(size_t)matrix->m_cols];
    memset(&res[ii][0],0,matrix->m_cols*sizeof(double));
  }
  for (ii=0;ii<m_rows;ii++) {
    for (jj=0;jj<matrix->m_cols;jj++) {
      for (kk=0;kk<m_cols;kk++)
        res[ii][jj]+=m_m[ii][kk]*matrix->m_m[kk][jj];
    }
  }
  invalidate();
  clearArr();
  m_rows=myrows;
  m_cols=matrix->m_cols;
  m_m=res;
  return 0;

}

void Matrix::toString(Asciistr *str) const {

  if (!m_m || !str)
    return;
  Asciistr numbuf;
  str->reserve(4096);
  int ii=0,jj=0;
  for (ii=0;ii<m_rows;ii++) {
    for (jj=0;jj<m_cols;jj++) {
      d2a(m_m[ii][jj],&numbuf,6,1,12);
      str->append((const char *)numbuf);
      str->append("  ");
    }
    str->append("\n");
  }

}

SquareMatrix::SquareMatrix(int dim,double **m) : Matrix(dim,dim,m),m_lum(0),m_rowperm(0),m_parity(1.) {

  decomposition();

  /*int ii=0,jj=0;
  for (ii=0;ii<m_rows;ii++) {
    for (jj=0;jj<m_cols;jj++)
      printf ("%e ",m_lum[ii][jj]);
    printf ("\n");
  }*/

}

SquareMatrix::SquareMatrix(const SquareMatrix &ass) :
    Matrix(ass.m_rows,ass.m_cols,ass.m_m),m_lum(0),m_rowperm(0),m_parity(ass.m_parity) {

  int ii=0,jj=0;
  if (ass.m_lum) {
    m_lum=new double*[(size_t)ass.m_rows];
    for (ii=0;ii<ass.m_rows;ii++) {
      m_lum[ii]=new double[(size_t)ass.m_cols];
      for (jj=0;jj<ass.m_cols;jj++)
        m_lum[ii][jj]=ass.m_lum[ii][jj];
    }
  }
  if (ass.m_rowperm) {
    m_rowperm=new int[(size_t)ass.m_rows];
    for (ii=0;ii<ass.m_rows;ii++)
      m_rowperm[ii]=ass.m_rowperm[ii];
  }

}

SquareMatrix::~SquareMatrix() {

  invalidate();

}

SquareMatrix &SquareMatrix::operator=(const SquareMatrix &ass) {

  if (this==&ass)
    return *this;
  ((Matrix*)this)->operator=((const Matrix &)ass);
  invalidate();
  int ii=0,jj=0;
  if (ass.m_lum) {
    m_lum=new double*[(size_t)ass.m_rows];
    for (ii=0;ii<ass.m_rows;ii++) {
      m_lum[ii]=new double[(size_t)ass.m_cols];
      for (jj=0;jj<ass.m_cols;jj++)
        m_lum[ii][jj]=ass.m_lum[ii][jj];
    }
  }
  if (ass.m_rowperm) {
    m_rowperm=new int[(size_t)ass.m_rows];
    for (ii=0;ii<ass.m_rows;ii++)
      m_rowperm[ii]=ass.m_rowperm[ii];
  }
  m_parity=ass.m_parity;
  return *this;

}

bool SquareMatrix::operator==(const SquareMatrix &cmp) const {

  return ((Matrix*)this)->operator==((const Matrix &)cmp);

}

bool SquareMatrix::operator<(const SquareMatrix &cmp) const {

  return ((Matrix*)this)->operator<((const Matrix &)cmp);

}

void SquareMatrix::invalidate() {

  if (m_lum) {
    int ii=0;
    for (ii=0;ii<m_cols;ii++)
      delete [] m_lum[ii];
    delete [] m_lum;
    m_lum=0;
  }
  if (m_rowperm) {
    delete [] m_rowperm;
    m_rowperm=0;
  }
  m_parity=1.;

}

void SquareMatrix::clearCtrl() {

  invalidate();
  clearArr();

}

int SquareMatrix::setCtrl(int num,double **m) {

  invalidate();
  return Matrix::setCtrl(num,num,m);

}

int SquareMatrix::setCtrl(int rows,int cols,double **m) {

  if (rows==cols)
    return setCtrl(rows,m);
  return -1;

}

double SquareMatrix::determinant() {

  if (!m_m || m_rows<=1)
    return .0;
  if (!m_lum) {
    if (decomposition()!=0)
      return .0;
  }
  int ii=0;
  double det=m_parity;
  for (ii=0;ii<m_rows;ii++)
    det*=m_lum[ii][ii];
  return det;

}

int SquareMatrix::invert() {

  if (!m_m || m_rows<=1)
    return -1;
  int ii=0,jj=0;
  double *identity=new double[(size_t)m_cols];
  double *inv=new double[(size_t)m_cols];
  for (ii=0;ii<m_cols;ii++) {
    for (jj=0;jj<m_rows;jj++)
      identity[jj]=inv[jj]=(ii==jj ? 1. : .0);
    // solve column by column the equation M*M(-1)=1
    if (backsubstitution(identity,inv)!=0) {
      delete [] identity;
      delete [] inv;
      return -1;
    }
    // set the new elements ...
    for (jj=0;jj<m_rows;jj++)
      m_m[jj][ii]=inv[jj];
  }
  // ... and invalidate decomposition matrix
  invalidate();
  delete [] identity;
  delete [] inv;
  return 0;

}

int SquareMatrix::decomposition() {

  if (!m_m || m_rows<=1)
    return -1;
  int ii=0,jj=0,kk=0,imax=0,num=m_rows;
  double maxcoeff=.0,tmp=.0;
  invalidate();
  // first find the largest element in every row (implicit pivoting)
  // also copy the original matrix since we do not want to destroy it
  double *rowscale=new double[(size_t)num];
  m_lum=new double*[(size_t)num];
  for (ii=0;ii<num;ii++) {
    maxcoeff=.0;
    m_lum[ii]=new double[(size_t)num];
    for (jj=0;jj<num;jj++) {
      m_lum[ii][jj]=m_m[ii][jj];
      tmp=fabs(m_lum[ii][jj]);
      if (tmp>maxcoeff)
        maxcoeff=tmp;
    }
    if (mk_deq(maxcoeff,.0)) {
      invalidate();
      delete [] rowscale;
      return -1;
    }
    rowscale[ii]=maxcoeff;
  }
  m_rowperm=new int[(size_t)num];
  for (ii=0;ii<num;ii++)
    m_rowperm[ii]=ii; // no row interchanging yet
  // loop every column
  // used alphas and betas are already calculated by the time they are needed
  for (jj=0;jj<num;jj++) {
    // loop rows for 'u'pper triangular matrix
    for (ii=0;ii<jj;ii++) {
      // do the matrix multiplication
      // beta[i,j]=m[i,j]-sum(alpha[i,k]*beta[k,j])
      for (kk=0;kk<ii;kk++)
        m_lum[ii][jj]-=m_lum[ii][kk]*m_lum[kk][jj];
    }
    maxcoeff=.0;
    // loop rows for 'l'ower triangular matrix
    // and diagonal (denominators for lower matrix elements) inclusive
    for (ii=jj;ii<num;ii++) {
      // do the matrix multiplication
      // beta[j,j]*alpha[i,j]=m[i,j]-sum(alpha[i,k]*beta[k,j])
      for (kk=0;kk<jj;kk++)
        m_lum[ii][jj]-=m_lum[ii][kk]*m_lum[kk][jj];
      // beta[j,j] is to calculate as pivot(largest element)
      // from this row=i and (precalculated) =rowscale[i]
      tmp=fabs(m_lum[ii][jj])/rowscale[ii];
      if (tmp>=maxcoeff) {
        maxcoeff=tmp;
        imax=ii;
      }
    }
    // if index of precalculated rowscale!=scale[actrow]
    // rows must be interchanged and index table updated
    // for later dividing by pivot element (beta[j,j])
    // this is possible since columns<j are already determined, and
    // columns>j are not used yet, therefore row interchanging
    // does not destroy the solution only just scrambles the order
    // which means that the out-matrix may look queer but dissolves into
    // a rowwise permutation of m
    if (jj!=imax) {
      for (ii=0;ii<num;ii++)
        swap(&m_lum[imax][ii],&m_lum[jj][ii]);
      rowscale[imax]=rowscale[jj]; // rowscale[j] is not needed anymore
      swap(&m_rowperm[jj],&m_rowperm[imax]);
      m_parity=-m_parity;
    }
    if (mk_deq(m_lum[jj][jj],.0)) {
      invalidate();
      delete [] rowscale;
      return -1;
    }
    // finallly (for this column) divide all lower row elements
    // by the pivot
    if (jj<(num-1)) {
      for (ii=(jj+1);ii<num;ii++)
        m_lum[ii][jj]/=m_lum[jj][jj];
    }
  }
  delete [] rowscale;
  return 0;

}

int SquareMatrix::backsubstitution(double *r,double *x) {

  if (!r || !x)
    return -1;
  if (!m_lum || !m_rowperm) {
    if (decomposition()!=0)
      return -1;
  }
  int ii=0,jj=0,num=m_rows;
  // first adapt the row permutation for the right hand side vector r
  // also copy right hand side input (do not destroy)
  for (ii=0;ii<num;ii++)
    x[ii]=r[m_rowperm[ii]];
  // do the forward substitution by solving for the lower triangular matrix (alpha)
  // e.g. for (rows=cols=3) lum(lowerpart)={lum[1,1]=1,lum[1,2]=0,lum[1,3]=0,
  // lum[2,1],lum[2,2]=1,lum[2,3]=0,lum[3,1],lum[3,2],lum[3,3]=1} as determined
  // in --> ludecomposition
  for (ii=0;ii<num;ii++) {
    for (jj=0;jj<ii;jj++)
      x[ii]-=m_lum[ii][jj]*x[jj];
  }
  // now do the backsubstitution by solving for the upper triangular matrix (beta)
  // e.g. for (rows=cols=3) lum(upperpart)={lum[1,1],lum[1,2],lum[1,3],
  // lum[2,1]=0,lum[2,2],lum[2,3],lum[3,1]=0,lum[3,2]=0,lum[3,3]} as determined
  // in --> ludecomposition
  // (since pivot is not ==1 here we have to do the dividing)
  double tmp=.0;
  for (ii=(num-1);ii>-1;ii--) {
    tmp=x[ii];
    for (jj=(ii+1);jj<num;jj++)
      tmp-=m_lum[ii][jj]*x[jj];
    x[ii]=tmp/m_lum[ii][ii];
  }
  return 0;

}

int SquareMatrix::solveFor(int num,double *r,double *x) {

  if (!x)
    return -1;
  int ii=0;
  if (!r || num!=m_rows || backsubstitution(r,x)!=0) {
    for (ii=0;ii<num;ii++)
      x[ii]=.0;
    return -1;
  }
  return 0;

}

TransformMatrix::TransformMatrix() : SquareMatrix(4,0) {

}

TransformMatrix::TransformMatrix(const TransformMatrix &ass) : SquareMatrix(ass.m_rows,ass.m_m) {

}

TransformMatrix::~TransformMatrix() {

}

TransformMatrix &TransformMatrix::operator=(const TransformMatrix &ass) {

  if (this==&ass)
    return *this;
  ((SquareMatrix*)this)->operator=((const SquareMatrix &)ass);
  return *this;

}

bool TransformMatrix::operator==(const TransformMatrix &cmp) const {

  return ((SquareMatrix*)this)->operator==((const SquareMatrix &)cmp);

}

bool TransformMatrix::operator<(const TransformMatrix &cmp) const {

  return ((SquareMatrix*)this)->operator<((const SquareMatrix &)cmp);

}

int TransformMatrix::translate(double x,double y,double z) {

  if (!m_m)
    return -1;
  TransformMatrix tm;
  tm.m_m[3][0]=x;
  tm.m_m[3][1]=y;
  tm.m_m[3][2]=z;
  return mult(m_rows,m_rows,tm.m_m);

}

int TransformMatrix::scale(double x,double y,double z) {

  if (!m_m)
    return -1;
  TransformMatrix tm;
  tm.m_m[0][0]=x;
  tm.m_m[1][1]=y;
  tm.m_m[2][2]=z;
  return mult(m_rows,m_rows,tm.m_m);

}

int TransformMatrix::rotateZ(double degrees) {

  if (!m_m)
    return -1;
  double radang=degrees*mk_rad;
  TransformMatrix tm;
  tm.m_m[0][0]=cos(radang);
  tm.m_m[0][1]=-sin(radang);
  tm.m_m[1][0]=sin(radang);
  tm.m_m[1][1]=cos(radang);
  return mult(m_rows,m_rows,tm.m_m);

}

int TransformMatrix::rotateX(double degrees) {

  if (!m_m)
    return -1;
  double radang=degrees*mk_rad;
  TransformMatrix tm;
  tm.m_m[1][1]=cos(radang);
  tm.m_m[1][2]=-sin(radang);
  tm.m_m[2][1]=sin(radang);
  tm.m_m[2][2]=cos(radang);
  return mult(m_rows,m_rows,tm.m_m);

}

int TransformMatrix::rotateY(double degrees) {

  if (!m_m)
    return -1;
  double radang=degrees*mk_rad;
  TransformMatrix tm;
  tm.m_m[0][0]=cos(radang);
  tm.m_m[0][2]=sin(radang);
  tm.m_m[2][0]=-sin(radang);
  tm.m_m[2][2]=cos(radang);
  return mult(m_rows,m_rows,tm.m_m);

}

int TransformMatrix::shearXY(double x,double y) {

  if (!m_m)
    return -1;
  TransformMatrix tm;
  tm.m_m[1][0]=y;
  tm.m_m[0][1]=x;
  return mult(m_rows,m_rows,tm.m_m);

}

int TransformMatrix::shearXZ(double x,double z) {

  if (!m_m)
    return -1;
  TransformMatrix tm;
  tm.m_m[0][2]=x;
  tm.m_m[2][0]=z;
  return mult(m_rows,m_rows,tm.m_m);

}

int TransformMatrix::shearYZ(double y,double z) {

  if (!m_m)
    return -1;
  TransformMatrix tm;
  tm.m_m[1][2]=y;
  tm.m_m[2][1]=z;
  return mult(m_rows,m_rows,tm.m_m);

}

int TransformMatrix::transform(Vertex *vertex) {

  if (!m_m || !vertex)
    return -1;
  Vertex v(mk_isbusted(vertex->x())!=0 ? .0 : vertex->x(),
           mk_isbusted(vertex->y())!=0 ? .0 : vertex->y(),
           mk_isbusted(vertex->z())!=0 ? .0 : vertex->z(),
           mk_isbusted(vertex->w())!=0 ? 1. : vertex->w());
  vertex->set(.0,.0,.0,.0);
  int i=0,j=0;
  double *vdata=vertex->data();
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++)
      vdata[i]+=v[j]*m_m[j][i];
  }
  return 0;

}

} // namespace

