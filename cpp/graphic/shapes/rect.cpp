
#include <numeric/matrix.h>
#include <graphic/shapes/rect.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

static const int defCntPoints=4;

int Rect::eval(TVList<Vector3> *pointL,int npoints) {

  if (npoints!=defCntPoints) npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  m_points.clear();
  if (m_points.size()<npoints)
    m_points.resize(npoints);
  aux::Vector3 pc(.0,m_sz.width()/2.),
               p0(.0,m_sz.height()/2.),p1(p0),p2(.0,-m_sz.height()/2.),p3(p2);
  TransformMatrix mm;
  mm.scale(1.,m_scale);
  mm.rotateZ(90.+m_rotate);
  mm.transform(&pc);
  mm.reset();
  mm.scale(1.,m_scale);
  mm.rotateZ(m_rotate);
  mm.translate(pc[0],pc[1]);
  mm.transform(&p0);
  mm.transform(&p3);
  mm.translate(-2.*pc[0],-2.*pc[1]);
  mm.transform(&p1);
  mm.transform(&p2);
  mm.reset();
  mm.translate(m_translate[0],m_translate[1]);
  mm.transform(&p0);
  m_points.append(p0);
  mm.transform(&p1);
  m_points.append(p1);
  mm.transform(&p2);
  m_points.append(p2);
  mm.transform(&p3);
  m_points.append(p3);
  if (pointL)
    *pointL=m_points;
  return npoints;

}

void Rect::toStringType(Asciistr *buf) const {
  
  if (!buf)
    return;
  aux::Asciistr numstr;
  buf->append("size: ");
  m_sz.toString(&numstr);
  buf->append((const char *)numstr);
  buf->append("\n");
    
}

} // namespace
