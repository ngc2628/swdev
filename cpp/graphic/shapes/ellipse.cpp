
#include <numeric/matrix.h>
#include <graphic/shapes/ellipse.h>
#include <mkbase/mkmath.h>
#include <stdio.h>
#include <string.h>

using namespace num;

namespace shapes {

static const int defCntPoints=60;

Ellipse::Ellipse() : Shape2("ellipse") {

  mk_listrealloc(&m_vertices,1);
  mk_vertexzero(ab);
  mk_listappend(&m_vertices,ab);
  
}

Ellipse &Ellipse::operator=(const Ellipse &ass) {
      
  if (&ass!=this) {
    m_scale=ass.m_scale;
    m_rotate=ass.m_rotate;
    mk_vertexcopy(m_translate,ass.m_translate);
    mk_stringset(m_descr,&ass.m_descr[0]);
    m_styleO=ass.m_styleO;
    m_styleF=ass.m_styleF;
    mk_listcopy(&m_vertices,&ass.m_vertices);
  }
  return *this;
  
}

bool Ellipse::operator==(const Ellipse &cmp) const {
      
  return (circ()==cmp.circ());
  
}

bool Ellipse::operator<(const Ellipse &cmp) const {
      
  return (circ()<cmp.circ());

}

int Ellipse::setVertex(int idx,mk_vertex ab) {
    
  int res=Shape2::setVertex(idx,ab);
  if (res!=0)
    return res;
  mk_vertexzero(mab);
  mk_listat(&m_vertices,0,(void*)&mab);
  if (mab[1]<mab[0]) {
    mk_swapf(&mab[0],&mab[1]);
    mk_listsetat(&m_vertices,(void*)&mab,0,0);
  }
  return 0;
  
}

double Ellipse::ecc() const {
      
  mk_vertexzero(ab);
  mk_listat(&m_vertices,0,(void*)&ab);
  return (sqrt(ab[0]*ab[0]-ab[1]*ab[1])/ab[0]);
  
}

double Ellipse::area() const {

  mk_vertexzero(ab);
  mk_listat(&m_vertices,0,(void*)&ab);
  return (mk_pi*ab[0]*ab[1]);
  
}

double Ellipse::circ() const { // circumference

  mk_vertexzero(ab);
  mk_listat(&m_vertices,0,(void*)&ab);
  double ll=(ab[0]-ab[1])/(ab[0]+ab[1]);
  double fac=ll*ll;
  double add=1.+fac/4.;
  fac*=fac;
  add+=(fac/64.+fac*ll*ll/256.);
  fac*=fac;
  add+=25.*fac/16384.;
  return (mk_pi*(ab[0]+ab[1])*add);
    
}

double Ellipse::circradius() const {

  mk_vertexzero(ab);
  mk_listat(&m_vertices,0,(void*)&ab);
  return (ab[0]>ab[1] ? ab[0] : ab[1]);
  
}

int Ellipse::eval(mk_list *pointL,int npoints) {

  if (npoints<=0) 
    npoints=defCntPoints;
  Shape2::eval(pointL,npoints);
  mk_vertexzero(ab);
  mk_listat(&m_vertices,0,(void*)&ab);
  TransformMatrix mm;
  mm.rotateZ(m_rotate);
  mm.translate(m_translate[0],m_translate[1]);
  int ii=0,off=npoints/2;
  double sc=2.*mk_pi/(double)npoints;
  mk_vertexzero(vv);
  for (ii=0;ii<npoints;ii++) {
    if (ii<=off) {
      vv[0]=cos(sc*(double)ii);
      vv[1]=sin(sc*(double)ii);
    }
    else {
      vv[0]=-cos(sc*(double)(ii-off));
      vv[1]=-sin(sc*(double)(ii-off));
    }
    vv[0]=ab[0]*vv[0];
    vv[1]=ab[1]*vv[1];
    mm.transform(vv);
    mk_listappend(pointL,(void*)&vv);
  }
  return npoints;
  
}

} // namespace
