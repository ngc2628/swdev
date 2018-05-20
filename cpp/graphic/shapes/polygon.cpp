
#include <numeric/matrix.h>
#include <graphic/shapes/polygon.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

Polygon2::Polygon2(int npoints) : Shape2("polygon2") {

  if (npoints<0)
    npoints=0;
  else if(npoints>sLimit)
    npoints=sLimit;
  if (npoints>0)
    m_ppoints.resize(npoints);

}

int Polygon2::eval(TVList<Vector3> *pointL,int npoints) {

  int cnt=m_points.count();
  if (npoints<=cnt) {
    if (pointL) {
      *pointL=m_points;
      while (cnt>npoints) {
        pointL->remove(cnt-1);
        cnt=pointL->count();
      }
    }
    return npoints; // cache
  }

  int ii=0;
  cnt=m_ppoints.count();
  m_points.clear();
  if (m_points.size()<cnt)
    m_points.resize(cnt);
  Vector3 pp;
  TransformMatrix mm;
  mm.rotateZ(m_rotate);
  mm.translate(m_translate[0],m_translate[1]);
  for (ii=0;ii<cnt;ii++) {
    pp=m_ppoints[ii];
    mm.transform(&pp);
    m_points.append(pp);
  }
  if (pointL) {
    *pointL=m_points;
    while (cnt>npoints) {
      pointL->remove(cnt-1);
      cnt=pointL->count();
    }
  }
  return npoints;

}

} // namespace