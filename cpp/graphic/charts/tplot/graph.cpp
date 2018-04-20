
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <time.h>
#include <graphic/charts/simpleplot/graph.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/shapes/shape.h>
#include <graphic/shapes/ellipse.h>
#include <graphic/shapes/sshape.h>
#include <graphic/shapes/rect.h>
#include <graphic/shapes/triangle.h>

using namespace aux;
using namespace num;
using namespace shapes;

namespace simpleplot {

const double matchdist=5.;

int markshape2idx(const char *type) {
  
  if (!type) return 0;
  int i=0,len=strlen(type);
  if (len==0) return 0;
  int idx=nummarkshapes2d-1;
  if (len>=aux::idlen) len=aux::idlen-1;
  for (i=0;i<nummarkshapes2d;i++) {
    if (strncmp(type,markshapes2d[i],len)==0) {
      idx=i;
      break;
    }
  }
  return idx;

}

const char *idx2markshape2(int idx) {

  if (idx<0 || idx>=nummarkshapes2d) return markshapes2d[nummarkshapes2d-1];
  return markshapes2d[idx];

}

shapes::Shape2 *buildMarkshape2(const char *type,double size) {

  int idx=markshape2idx(type);
  shapes::Shape2 *s=0;
  switch (idx) {
    case 0: break;
    case 1: s=new shapes::Ellipse(aux::Vector3(size,size)); break;
    case 2: s=new shapes::Rect(shapes::RectSize(1.414*size,1.414*size)); break;
    case 3: s=new shapes::Triangle(aux::Vector3(.0,size),aux::Vector3(-.866*size,-.5*size),aux::Vector3(.866*size,-.5*size)); break;
    case 4: s=new shapes::Rect(shapes::RectSize(1.414*size,1.414*size)); s->rotate(45.); break;
    default: s=new shapes::NoShape2();
  }
  if (s) s->setDescr(markshapes2d[idx]);
  return s;

}

GraphData2::GraphData2(const GraphData2 &ass) : GraphData(),m_data(ass.m_data),m_sortype(ass.m_sortype) {
      
  int i=0,sz=ass.m_mark.size();
  m_mark.resize(sz);
  for (i=0;i<sz;i++) {
    if (ass.m_mark[i]) m_mark.set(i,(Shape2*)((ass.m_mark[i])->clone()));
  }
    
}

GraphData2 &GraphData2::operator=(const GraphData2 &ass) {  
      
  if (this==&ass) return *this;
  m_data=ass.m_data;
  int i=0,sz=ass.m_mark.size();
  m_mark.resize(sz);
  for (i=0;i<sz;i++) {
    if (ass.m_mark[i]) m_mark.set(i,(Shape2*)((ass.m_mark[i])->clone()));
  }
  m_sortype=ass.m_sortype;
  return *this;
  
}

aux::Vector3 GraphData2::data(int idx,int *avail) {

  if (avail) *avail=(int)m_data.at(idx);
  return m_data[idx];

}

int GraphData2::data(TVList<Vector3> *vL) {

  int i=0,cnt=m_data.count();
  if (!vL) return cnt;
  if (vL->size()<cnt) vL->resize(cnt);
  vL->clear();
  if(cnt==0) return cnt;
  if (m_sortype>=0 && m_data.sorted()) {
    vL->setCmp(aux::cmpVector[m_sortype]);
    vL->inSort(m_data[0]);
  }
  else vL->append(m_data[0]);
  for (i=1;i<cnt;i++) vL->append(m_data[i]);
  return cnt;

}

int GraphData2::setData(int idx,Vector3 *v) {

  if (!v) idx=m_data.remove(idx);
  else if (idx>=0 && idx<m_data.count()) m_data.replace(idx,*v);
  else if (m_sortype<0) idx=(idx<0 ? m_data.prepend(*v) : m_data.append(*v));
  else idx=m_data.inSort(*v);
  return idx;

}

int GraphData2::findBounds(aux::MinMax *bx,aux::MinMax *by) {

  int i=0,cnt=m_data.count();
  if (!bx && !by) return cnt;
  if (bx) bx->setXY(dinf,dsinf);
  if (by) by->setXY(dinf,dsinf);
  if (cnt==0) return cnt;
  int sortype=(m_sortype>=0 && m_data.sorted() ? m_sortype : -1);
  if (sortype==0 && bx) {
    bx->setX(m_data.at(0)->x());
    bx->setY(m_data.at(cnt-1)->x());
    if (!by) return cnt;
  }
  else if (sortype==1 && by) {
    by->setX(m_data.at(0)->y());
    by->setY(m_data.at(cnt-1)->y());
    if (!bx) return cnt;
  }
  Vector3 *v=0;
  for (i=0;i<cnt;i++) {
    v=m_data.at(i);
    if (bx && sortype!=0) {
      if (dlt(v->x(),bx->x())) bx->setX(v->x());
      if (dlt(bx->y(),v->x())) bx->setY(v->x());
    }
    if (by && sortype!=1) {
      if (dlt(v->y(),by->x())) by->setX(v->y());
      if (dlt(by->y(),v->y())) by->setY(v->y());
    }
  }
  return cnt;
  
}

int GraphData2::match(aux::Vector3 p,aux::Vector3 md) {

  int i=0,idxl=-1,idxh=-1,cnt=m_data.count(),sortype=(m_sortype>=0 && m_data.sorted() ? m_sortype : -1);
  if (cnt==0) return -1;
  aux::Vector3 *pl=0;
  aux::Vector3 *ph=0;
  if (sortype>=0) {
    idxh=m_data.findNextIndex(p);
    if (idxh==0 || idxh==cnt) return -1; 
    idxl=idxh-1;
    pl=m_data.at(idxl);
    ph=m_data.at(idxh);
    if (sortype==0) {
//printf ("p=%f,%f pl=%f,%f ph=%f,%f md=%f,%f distl=%d disth=%d plbust=%d\n",
//p.x(),p.y(),pl->x(),pl->y(),ph->x(),ph->y(),md.x(),md.y(),fabs(p.y()-pl->y())<md.y(),fabs(p.y()-ph->y())<md.y(),pl->busted(2));
      if (!pl->busted(1) && fabs(p.y()-pl->y())<md.y()) return idxl;
      else if (!ph->busted(1) && fabs(p.y()-ph->y())<md.y()) return idxh;
    }
    else if (sortype==1) {
      if (!pl->busted(0) && fabs(p.x()-pl->x())<md.x()) return idxl;
      else if (!ph->busted(0) && fabs(p.x()-ph->x())<md.x()) return idxh;
    }
  }
  else {
    for (i=0;i<cnt;i++) {
      pl=m_data.at(i);
      if (pl->busted(3)) continue;
      if (fabs(p.x()-pl->x())<md.x() && fabs(p.y()-pl->y())<md.y()) return i;
    }
  }
  return -1;

}

shapes::Shape2 *GraphData2::mark(int idx) {

  return m_mark.at(0);
  
}

int GraphData2::setMark(int idx,shapes::Shape2 *mark) {

  shapes::Shape2 *oldmark=m_mark.set(0,mark ? (Shape2*)(mark->clone()) : 0);
  if (oldmark) delete oldmark;
  return (m_mark[0] ? idx : (mark ? -1 : idx));
  //m_mark.set(idx,mark ? (Shape2*)(mark->clone()) : 0);
  //return (m_mark[idx] ? idx : (mark ? -1 : idx));

}

int GraphData2::setSortype(int coor) {

  int sortype=m_sortype;
  if (coor<0 || coor>2) m_data.setCmp(0);
  else {
    m_data.setCmp(aux::cmpVector[coor]);
    m_data.sort(true);
  }
  m_sortype=coor;
  return sortype;

}

// ***

void Graph::sc2sz(aux::Vector3 *v) const {

  if (!m_graphdata) return;
  int i=0;
  Axis *ax=0;
  for (i=0;i<3;i++) {
    ax=dynamic_cast<Axis*>(m_axis[i]);
    if (ax) ax->sc2sz(v);
  } 


}

void Graph::sz2sc(aux::Vector3 *v) const {

  if (!m_graphdata) return;
  int i=0;
  Axis *ax=0;
  for (i=0;i<3;i++) {
    ax=dynamic_cast<Axis*>(m_axis[i]);
    if (ax) ax->sz2sc(v);
  }  

}

int Graph::match(aux::Vector3 p) const {

  if (!m_graphdata) return -1;
  aux::Vector3 md1(matchdist,matchdist),md0(.0,.0);
  sz2sc(&md0);
  sz2sc(&md1);
  sz2sc(&p);
  return m_graphdata->match(p,aux::Vector3(md1.x()-md0.x(),md1.y()-md0.y()));
 
}

GraphXY::GraphXY(int destr) : Graph("graphxy"),m_interpolation(0) {

  m_destr=destr;
		  
}
 
GraphXY::~GraphXY() {

  if (m_interpolation) delete m_interpolation;
   
}

aux::MinMax GraphXY::findBounds(int type) {

  aux::MinMax b;
  if (m_graphdata) m_graphdata->findBounds(type==0 ? &b : 0,type==1 ? &b : 0);
  return b;  

}

aux::Vector3 GraphXY::value(int idx,int *avail) const { 

  if (m_graphdata) return m_graphdata->data(idx,avail);
  if (avail) *avail=0;
  return aux::Vector3();
  
}

int GraphXY::setValue(int idx,aux::Vector3 *v,int *modbounds) {

  if (!m_graphdata) return -1;
  int avail=0;
  Vector3 vold=m_graphdata->data(idx,&avail);
  int nidx=m_graphdata->setData(idx,v),modb=0;
  Axis *ax0=dynamic_cast<Axis*>(m_axis[0]),*ax1=dynamic_cast<Axis*>(m_axis[1]);
  if (nidx==idx) {
    if (avail && !vold.busted(0) && ax0 && ax0->checkAutoBounds(aux::MinMax(vold[0],vold[0]))) {
      ax0->findAutoBounds(0);
      modb=1;
    }
    if (avail && !vold.busted(1) && ax1 && ax1->checkAutoBounds(aux::MinMax(vold[1],vold[1]))) {
      ax1->findAutoBounds(1);
      modb|=2;
    }
  }
  if (v) {
    if (ax0 && ax0->checkAutoBounds(aux::MinMax(v->x(),v->x()))) modb|=1;
    if (ax1 && ax1->checkAutoBounds(aux::MinMax(v->y(),v->y()))) modb|=2;
  }
  if (modbounds) *modbounds=modb;
  return nidx;

}

int GraphXY::rescale() {

  m_scData.clear();
  m_scDataI.clear(true);
  m_scMark.clear(true);
  if (!m_graphdata) return 0;
  int i=0,j=0,ni=0,cnt=m_graphdata->data(&m_scData);
  if (cnt==0) return 0;
  if (m_scDataI.size()<cnt) m_scDataI.resize(cnt);
  if (m_scMark.size()<cnt) m_scMark.resize(cnt);
  Shape2 *scm=0;
  Vector3 *v=0;
  if (!m_interpolation) {
    for (i=0;i<cnt;i++) {
      v=m_scData.at(i);
      sc2sz(v);
      if (m_graphdata->mark(i)) {
        scm=(Shape2*)(m_graphdata->mark(i)->clone());
        scm->translate(*v);
        m_scMark.set(i,scm);
      }
    }
    return cnt;
  }
  
  ni=num::numInterpolIntermediates(m_interpolation);
  TVList<Vector3> *viL=new TVList<Vector3>(cnt);
  for (i=0;i<cnt;i++) {
    v=m_scData.at(i);
    if (v->busted(3)) {
      m_scDataI.append(viL);
      viL=new TVList<Vector3>(cnt-i+1);
    }
    else viL->append(*v);
//printf ("i=%d scdata=%f,%f\n",i,v->x(),v->y());
    sc2sz(v); 
//printf ("i=%d scdata=%f,%f\n",i,v->x(),v->y());
    if (m_graphdata->mark(i) && v->busted(3)==0) {
      scm=(Shape2*)(m_graphdata->mark(i)->clone());
      scm->translate(*v);
      m_scMark.set(i,scm);
    }
  }
  m_scDataI.append(viL);
  for (i=0;i<m_scDataI.count();i++) {
    m_interpolation->setCtrl(m_scDataI[i]);
    m_interpolation->interpol(ni,m_scDataI[i]);
    for (j=0;j<ni;j++) {
      v=m_scDataI[i]->at(j);
      sc2sz(v); 
    }
  }
      
  return cnt;
    
}

} // namespace
