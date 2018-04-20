
#include <numeric/matrix.h>
#include <graphic/shapes/line.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

static const int defCntPoints=2;

int Line2::eval(TVList<Vector3> *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  m_points.clear();
  if (m_points.size()<npoints)
    m_points.resize(npoints);
  aux::Vector3 p0(m_l[0]),p1(m_l[1]),pc=center();
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

void Line2::toStringType(aux::Asciistr *buf) const {
  
  if (!buf)
    return;
  aux::Asciistr numstr;
  buf->append("p0, ");
  m_l[0].toString(&numstr);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\np1, ");
  m_l[1].toString(&numstr);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\nlen=");
  aux::d2a(len(),&numstr,-1);
  buf->append((const char *)numstr);
  numstr=0;
  buf->append("\nang=");
  aux::d2a(ang(),&numstr,-1);
  buf->append((const char *)numstr);
  buf->append("\n");
  
}

} // namespace
