
#include <numeric/matrix.h>
#include <graphic/shapes/ellipse.h>
#include <stdio.h>
#include <string.h>

using namespace aux;
using namespace num;

namespace shapes {

static const int defCntPoints=60;

class NEllipse {

  public:
    Vector3 *m_points;
    unsigned short m_cnt;
    NEllipse(unsigned short cnt,int cmp) { 
      m_cnt=cnt;
      if (m_cnt==0 || cmp) {
        m_points=0;
        return;
      }
      m_points=new Vector3[m_cnt];
      int ii=0,off=m_cnt/2;
      double sc=2.*mk_pi/(double)m_cnt;
      for (ii=0;ii<m_cnt;ii++) {
        if (ii<=off)
          m_points[ii]=Vector3(cos(sc*(double)ii),sin(sc*(double)ii));
        else
          m_points[ii]=Vector3(-cos(sc*(double)(ii-off)),-sin(sc*(double)(ii-off)));
      }
    }
    ~NEllipse() {
      if (m_points)
        delete [] m_points;
    }
    bool operator==(const NEllipse &cmp) const {
      return (m_cnt==cmp.m_cnt);
    }
    bool operator<(const NEllipse &cmp) const {
      return (m_cnt<cmp.m_cnt);
    }
    
  protected:
    NEllipse(const NEllipse &) { }
    NEllipse &operator=(const NEllipse &) {
      return *this;
    }

};

static TPList<NEllipse> elL;

int Ellipse::eval(num::VertexList *pointL,int npoints) {

  if (elL.size()<=0)
    elL.resize(127);
  if (npoints<=0)
    npoints=defCntPoints;
  if (npoints==m_points.count()) {
    if (pointL)
      *pointL=m_points;
    return npoints; // cache
  }
  NEllipse cel((unsigned short)npoints,1);
  NEllipse *el=0;
  int ii=elL.find(&cel);
  if (ii>=0)
    el=elL.at(ii);
  else {
    el=new NEllipse((unsigned short)npoints,0);
    elL.inSort(el);
  }
  m_points.clear();
  m_points.resize(npoints);
  TransformMatrix mm;
  mm.rotateZ(m_rotate);
  mm.translate(m_translate[0],m_translate[1]);
  Vector3 pp;
  for (ii=0;ii<npoints;ii++) {
    pp.setXY(m_ab[0]*el->m_points[ii][0],m_ab[1]*el->m_points[ii][1]);
    mm.transform(&pp);
    m_points.append(pp);
  }

  if (pointL)
    *pointL=m_points;
  return npoints;
  
}

} // namespace
