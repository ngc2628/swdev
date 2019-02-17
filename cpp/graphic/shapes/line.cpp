
#include <numeric/matrix.h>
#include <graphic/shapes/line.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

static const int defCntPoints=2;

int Line2::eval(num::VertexList *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  m_points.clear();
  m_points.resize(npoints);
  num::Vector3 p0(m_l[0]),p1(m_l[1]),pc=center();
  TransformMatrix m;
  m.translate(-pc[0],-pc[1]);
  m.scale(m_scale,m_scale);
  m.rotateZ(m_rotate);
  m.translate(pc[0]+m_translate[0],pc[1]+m_translate[1]);
  m.transform(&p0);
  m.transform(&p1);
  m_points.append(p0);
  m_points.append(p1);
  if (pointL)
    *pointL=m_points;
  return npoints;

}

int Line2::toStringType(mk_string str) const {
  
  mk_stringappend(str,"p0, ");
  mk_string numstr;
  m_l[0].toString(numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\np1, ");
  m_l[1].toString(numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\nlen=");
  mk_d2a(len(),numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\nang=");
  mk_d2a(ang(),numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\n");
  return 0;
  
}

} // namespace
