
#include <mkbase/defs.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <osix/xxshape.h>
#include <graphic/charts/simpleplot/graph.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/shapes/shape.h>
#include <graphic/shapes/ellipse.h>
#include <graphic/shapes/triangle.h>
#include <graphic/shapes/rect.h>
#include <stdio.h>
#include <stdlib.h>

namespace simpleplot {

const double matchdist=5.;

int markshape2idx(const char *type) {
  
  if (!type)
    return 0;
  int ii=0,len=(int)strlen(type);
  if (len==0)
    return 0;
  int idx=nummarkshapes2d-1;
  for (ii=0;ii<nummarkshapes2d;ii++) {
    if (strncmp(type,markshapes2d[ii],(size_t)len)==0) {
      idx=ii;
      break;
    }
  }
  return idx;

}

const char *idx2markshape2(int idx) {

  if (idx<0 || idx>=nummarkshapes2d)
    return markshapes2d[nummarkshapes2d-1];
  return markshapes2d[idx];

}

shapes::Shape2 *buildMarkshape2(const char *type,double size) {

  int idx=markshape2idx(type);
  shapes::Shape2 *ss=0;
  switch (idx) {
    case 0:
      break;
    case 1:
      ss=new shapes::Ellipse(aux::Vector3(size,size));
      break;
    case 2:
      ss=new shapes::Rect(shapes::RectSize(1.414*size,1.414*size));
      break;
    case 3:
      ss=new shapes::Triangle(aux::Vector3(.0,size),aux::Vector3(-.866*size,-.5*size),aux::Vector3(.866*size,-.5*size));
      break;
    case 4:
      ss=new shapes::Rect(shapes::RectSize(1.414*size,1.414*size));
      ss->rotate(45.);
      break;
    default:
      ss=new shapes::NoShape2();
  }
  if (ss)
    ss->setDescr(markshapes2d[idx]);
  return ss;

}

GraphData2::GraphData2(int sz) : 
  m_data(sz<0 ? 0 : (sz>maxdatacnt ? maxdatacnt : sz)),
  m_mark(sz<0 ? 0 : (sz>maxdatacnt ? maxdatacnt : sz)),m_sortype(0) {
    
}

GraphData2::GraphData2(const GraphData2 &ass) : 
  GraphData(),m_data(ass.m_data),m_sortype(ass.m_sortype) {
      
  int ii=0,sz=ass.m_mark.size();
  m_mark.resize(sz);
  for (ii=0;ii<sz;ii++) {
    if (ass.m_mark[ii])
      m_mark.set(ii,(shapes::Shape2*)((ass.m_mark[ii])->clone()));
  }
    
}

GraphData2 &GraphData2::operator=(const GraphData2 &ass) {  
      
  if (this==&ass)
    return *this;
  m_data=ass.m_data;
  int ii=0,sz=ass.m_mark.size();
  m_mark.resize(sz);
  for (ii=0;ii<sz;ii++) {
    if (ass.m_mark[ii])
      m_mark.set(ii,(shapes::Shape2*)((ass.m_mark[ii])->clone()));
  }
  m_sortype=ass.m_sortype;
  return *this;
  
}

aux::Vector3 GraphData2::data(int idx,int *avail) {

  if (avail)
    *avail=(m_data.at(idx) ? 1 : 0);
  return m_data[idx];

}

int GraphData2::data(aux::TVList<aux::Vector3> *vL) {

  int ii=0,cnt=m_data.count();
  if (!vL)
    return cnt;
  if (vL->size()<cnt)
    vL->resize(cnt);
  vL->clear();
  if(cnt==0)
    return cnt;
  if (m_data.sorted()) {
    vL->setCmp(aux::cmpVector[m_sortype]);
    vL->inSort(m_data[0]);
  }
  else
    vL->append(m_data[0]);
  for (ii=1;ii<cnt;ii++)
    vL->append(m_data[ii]);
  return cnt;

}

int GraphData2::setData(int idx,aux::Vector3 *vv) {

  if (!vv)
    idx=m_data.remove(idx);
  else if (idx>=0 && idx<m_data.count())
    m_data.replace(idx,*vv);
  else
    idx=m_data.inSort(*vv);
  return idx;

}

int GraphData2::findBounds(int type,double *bmin,double *bmax) {

  type&=(aux::typeX|aux::typeY);
  int ii=0,cnt=m_data.count();
  if (type==0 || cnt==0 || (!bmin && !bmax))
    return cnt;
  if (bmin)
    *bmin=mk_dinf;
  if (bmax)
    *bmax=mk_dsinf;
  if (m_data.sorted()>0 && type==m_sortype) {
    if (bmin)
      *bmin=(type==aux::typeX ? m_data.at(0)->x() : m_data.at(0)->y());
    if (bmax)
      *bmax=(type==aux::typeX ? m_data.at(cnt-1)->x() : m_data.at(cnt-1)->y());
  }
  else {
    aux::Vector3 *vv=0;
    for (ii=0;ii<cnt;ii++) {
      vv=m_data.at(ii);
      if (bmin) {
        if (type==aux::typeX && mk_dlt(vv->x(),*bmin))
          *bmin=vv->x();
        else if (type==aux::typeY && mk_dlt(vv->y(),*bmin))
          *bmin=vv->y();
      }
      if (bmax) {
        if (type==aux::typeX && mk_dlt(*bmax,vv->x()))
          *bmax=vv->x();
        else if (type==aux::typeY && mk_dlt(*bmin,vv->y()))
          *bmax=vv->y();
      }
    }
  }

  return cnt;
  
}

int GraphData2::match(aux::Vector3 pp,aux::Vector3 md) {

  int ii=0,idxl=-1,idxh=-1,cnt=m_data.count(),sortype=(m_data.sorted() ? m_sortype : -1);
  if (cnt==0)
    return -1;
  aux::Vector3 *pl=0;
  aux::Vector3 *ph=0;
  if (sortype>=0) {
    idxh=m_data.findNextIndex(pp);
    if (idxh==0 || idxh==cnt)
      return -1;
    idxl=idxh-1;
    pl=m_data.at(idxl);
    ph=m_data.at(idxh);
    if (sortype==0) {
//printf ("p=%f,%f pl=%f,%f ph=%f,%f md=%f,%f distl=%d disth=%d plbust=%d\n",
//p.x(),p.y(),pl->x(),pl->y(),ph->x(),ph->y(),md.x(),md.y(),fabs(p.y()-pl->y())<md.y(),fabs(p.y()-ph->y())<md.y(),pl->busted(2));
      if (!pl->busted(1) && fabs(pp.y()-pl->y())<md.y())
        return idxl;
      else if (!ph->busted(1) && fabs(pp.y()-ph->y())<md.y())
        return idxh;
    }
    else if (sortype==1) {
      if (!pl->busted(0) && fabs(pp.x()-pl->x())<md.x())
        return idxl;
      else if (!ph->busted(0) && fabs(pp.x()-ph->x())<md.x())
        return idxh;
    }
  }
  else {
    for (ii=0;ii<cnt;ii++) {
      pl=m_data.at(ii);
      if (pl->busted(3))
        continue;
      if (fabs(pp.x()-pl->x())<md.x() && fabs(pp.y()-pl->y())<md.y())
        return ii;
    }
  }
  return -1;

}

shapes::Shape2 *GraphData2::mark(int) {

  return m_mark.at(0);
  
}

int GraphData2::setMark(int idx,shapes::Shape2 *mark) {

  shapes::Shape2 *oldmark=m_mark.set(0,mark ? (shapes::Shape2*)(mark->clone()) : 0);
  if (oldmark)
    delete oldmark;
  return (m_mark[0] ? idx : (mark ? -1 : idx));
  //m_mark.set(idx,mark ? (Shape2*)(mark->clone()) : 0);
  //return (m_mark[idx] ? idx : (mark ? -1 : idx));

}

int GraphData2::setSortype(int coor) {

  int sortype=m_sortype;
  if (coor<0 || coor>2)
    m_data.setCmp(0);
  else {
    m_data.setCmp(aux::cmpVector[coor]);
    m_data.sort(true);
  }
  m_sortype=coor;
  return sortype;

}

// ***

int Graph::sc2sz(aux::Vector3 *vv) const {

  if (!m_graphdata)
    return -1;
  int ii=0;
  Axis *ax=0;
  for (ii=0;ii<3;ii++) {
    ax=dynamic_cast<Axis*>(m_axis[ii]);
    if (ax)
      ax->sc2sz(vv);
  } 

    return 0;

}

int Graph::sz2sc(aux::Vector3 *vv) const {

  if (!m_graphdata)
    return -1;
  int ii=0;
  Axis *ax=0;
  for (ii=0;ii<3;ii++) {
    ax=dynamic_cast<Axis*>(m_axis[ii]);
    if (ax)
      ax->sz2sc(vv);
  }  

  return 0;

}

int Graph::match(aux::Vector3 pp) const {

  if (!m_graphdata)
    return -1;
  aux::Vector3 md1(matchdist,matchdist),md0(.0,.0);
  sz2sc(&md0);
  sz2sc(&md1);
  sz2sc(&pp);
  return m_graphdata->match(pp,aux::Vector3(md1.x()-md0.x(),md1.y()-md0.y()));
 
}

GraphXY::GraphXY() : Graph(typeGraphXY),m_interpolation(0) {
		  
}
 
GraphXY::~GraphXY() {

  if (m_interpolation)
    delete m_interpolation;
   
}

int GraphXY::findBounds(int type,double *min,double *max) {

  if (m_graphdata)
    m_graphdata->findBounds(type,min,max);
  return 0;

}

aux::Vector3 GraphXY::value(int idx,int *avail) const { 

  if (m_graphdata)
    return m_graphdata->data(idx,avail);
  if (avail)
    *avail=0;
  return aux::Vector3();
  
}

int GraphXY::setValue(int idx,aux::Vector3 *vv,int *modbounds) {

  if (!m_graphdata)
    return -1;
  int avail=0,nidx=m_graphdata->setData(idx,vv),modb=0;
  aux::Vector3 vold=m_graphdata->data(idx,&avail);
  Axis *ax0=dynamic_cast<Axis*>(m_axis[0]),*ax1=dynamic_cast<Axis*>(m_axis[1]);
  if (nidx==idx) {
    if (avail && vold.busted(0)==0 && ax0 && ax0->checkAutoBounds(vold[0],vold[0])>0)
      modb|=ax0->findAutoBounds(aux::typeX,0,0);
    if (avail && !vold.busted(1) && ax1 && ax1->checkAutoBounds(vold[1],vold[1])>0)
      modb|=ax1->findAutoBounds(aux::typeY,0,0);
  }
  if (vv) {
    if (ax0)
      modb|=ax0->checkAutoBounds(vv->x(),vv->x());
    if (ax1)
      modb|=ax1->checkAutoBounds(vv->y(),vv->y());
  }
  if (modbounds)
    *modbounds=modb;
  return nidx;

}

int GraphXY::rescale() {

  m_scData.clear();
  m_scDataI.clear();
  m_scMark.clear(true);
  if (!m_graphdata)
    return 0;
  int ii=0,jj=0,idx=-1,ni=0,cnt=m_graphdata->data(&m_scData);
  if (cnt==0)
    return 0;
  if (m_scDataI.size()<cnt)
    m_scDataI.resize(cnt);
  if (m_scMark.size()<cnt)
    m_scMark.resize(cnt);
  shapes::Shape2 *scm=0;
  aux::Vector3 *vv=0;
  if (!m_interpolation) {
    for (ii=0;ii<cnt;ii++) {
      vv=m_scData.at(ii);
      sc2sz(vv);
      if (m_graphdata->mark(ii)) {
        scm=(shapes::Shape2*)(m_graphdata->mark(ii)->clone());
        scm->translate(*vv);
        m_scMark.set(ii,scm);
      }
    }
    return cnt;
  }
  aux::TVList<aux::Vector3> viL(cnt);
  for (ii=0;ii<cnt;ii++) {
    vv=m_scData.at(ii);
    if (vv->busted(3)>0) {
      m_scDataI.append(viL);
      viL.clear();
    }
    else
      viL.append(*vv);
    sc2sz(vv); 
    if (m_graphdata->mark(ii) && vv->busted(3)==0) {
      scm=(shapes::Shape2*)(m_graphdata->mark(ii)->clone());
      scm->translate(*vv);
      m_scMark.set(ii,scm);
    }
  }
  if (viL.count()>0)
    m_scDataI.append(viL);
  int res=0;
  for (ii=0;ii<m_scDataI.count();ii++) {
    m_interpolation->setCtrl(m_scDataI.at(ii));
    m_interpolation->setup();
    ni=num::numInterpolIntermediates(m_interpolation);
    res=m_interpolation->interpol(ni,m_scDataI.at(ii));
    for (jj=0;jj<ni;jj++) {
      vv=m_scDataI.at(ii)->at(jj);
      sc2sz(vv);
    }
  }

  aux::Asciistr strInterpoltype,strInterpoltypeConst("const"),strOptSteps("steps");
  m_interpolation->type(&strInterpoltype);
  aux::TVList<aux::Asciistr> optL;
  m_interpolation->options(&optL);
  if (strInterpoltype==strInterpoltypeConst && optL.find(strOptSteps)<0) {
    aux::TVList<aux::TVList<aux::Vector3> > scDataI=m_scDataI;
    m_scDataI.clear();
    viL.clear();
    for (ii=0;ii<scDataI.count();ii++) {
      for (jj=0;jj<scDataI.at(ii)->count();jj++) {

        if (jj>0 && (jj%2)>0) {
          m_scDataI.append(viL);
          viL.clear();
        }        
        viL.append(scDataI.at(ii)->operator[](jj));
      }
      if (viL.count()>0) {
        m_scDataI.append(viL); 
        viL.clear();
      }       
    }
  } 

  return cnt;
    
}

} // namespace
