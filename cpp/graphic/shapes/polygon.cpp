
#include <numeric/matrix.h>
#include <graphic/shapes/polygon.h>
#include <mkbase/mkmath.h>
#include <stdio.h>
#include <string.h>

using namespace num;

namespace shapes {

Polygon2::Polygon2(int npoints) : Shape2("polygon2") {

  npoints=MIN(mk_slimit,MAX(0,npoints));
  mk_listalloc(&m_vertices,sizeof(mk_vertex),npoints);

}

Polygon2::Polygon2(const Polygon2 &ass) : Shape2((const Shape2 &)ass) { 

}

Polygon2 &Polygon2::operator=(const Polygon2 &ass) { 
      
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

bool Polygon2::operator==(const Polygon2 &cmp) const {
      
  int ii=0,cnt=m_vertices.count;
  if (cnt!=cmp.m_vertices.count)
    return false;
  mk_vertexzero(vv1);
  mk_vertexzero(vv2);
  for (ii=0;ii<cnt;ii++) {
    mk_listat(&m_vertices,ii,(void*)&vv1);
    mk_listat(&cmp.m_vertices,ii,(void*)&vv2);
    if (vv1[0]!=vv2[0] || vv1[1]!=vv2[1])
      return false;
  }
  return true;  
  
}

bool Polygon2::operator<(const Polygon2 &cmp) const {

  int ii=0,cnt=m_vertices.count;
  if (cnt<cmp.m_vertices.count)
    return true;
  if (cmp.m_vertices.count<cnt)
    return false;
  mk_vertexzero(vv1);
  mk_vertexzero(vv2);
  for (ii=0;ii<cnt;ii++) {
    mk_listat(&m_vertices,ii,(void*)&vv1);
    mk_listat(&cmp.m_vertices,ii,(void*)&vv2);
    if (vv2[0]<vv1[0])
      return false;
  }
  return true;
  
}

int Polygon2::setVertex(int idx,mk_vertex vertex) {

  int res=1;
  if (idx>=0 && idx<=m_vertices.count && mk_isbusted(vertex[0])==0 && mk_isbusted(vertex[1])==0)
    res=0;
  if (res==0) {
    mk_vertexzero(vv);
    mk_vertexcopy(vv,vertex);
    mk_listsetat(&m_vertices,(void*)&vv,idx,idx==m_vertices.count ? 1 : 0);
  }
  return res;

}

int Polygon2::eval(struct mk_list *pointL,int npoints) {

  npoints=m_vertices.count;
  Shape2::eval(pointL,npoints);
  mk_vertexzero(vv);
  TransformMatrix mm;
  mm.rotateZ(m_rotate);
  mm.translate(m_translate[0],m_translate[1]);
  int ii=0;
  for (ii=0;ii<npoints;ii++) {
    mk_listat(&m_vertices,ii,(void*)&vv);
    mm.transform(vv);
    mk_listappend(pointL,(void*)&vv);
  }
  return npoints;

}

} // namespace
