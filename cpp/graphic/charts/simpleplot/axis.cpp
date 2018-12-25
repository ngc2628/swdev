
#include <mkbase/mkla.h>
#include <graphic/charts/simpleplot/scale.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/charts/simpleplot/graph.h>
#include <sys/timeb.h>
//#include <sys/time.h>
//#include <sys/param.h>
#include <time.h>

namespace simpleplot {

Scale *buildScale(const char *type) {

  if (!type) 
    return 0;
  int ii=0,idx=-1,len=(int)strlen(type);
  if (len==0) 
    return 0;
  for (ii=0;ii<numscaletypes;ii++) {
    if (strncmp(type,scaletypes[ii],(size_t)len)==0) {
      idx=ii;
      break;
    }
  }
  if (idx==1) 
    return new Scale();
  return 0;

}

Axis::Axis(const char *type,unsigned int idd) : 
  TypeId(type,idd),m_scale(0),m_pos(0),
  m_visible(typeShowLine|typeShowNotch|typeShowText),
  m_style(osix::xx_somecolors[osix::black],1,1) {

  m_autobounds[0]=m_autobounds[1]=mk_dnan;
  m_fnt[0]=m_fnt[1]=osix::xxFnt();
  	    
}

Axis::~Axis () {
    
  if (m_scale)
    delete m_scale;
  m_graphs.clear();
  
}

aux::TypeId Axis::assignGraph(Graph *graph) {

  if (m_graphs.find(graph)>=0 || m_graphs.inSort(graph)<0)
    return TypeId();
  return TypeId((const TypeId&)(*graph));
}

int Axis::unassignGraph(const aux::TypeId &grid) {

  int idx=m_graphs.find(&grid);
  if (idx>=0) {
    m_graphs.remove(idx);
    findAutoBounds(aux::typeX|aux::typeY,0,0);
  }
  return m_graphs.count();
  
}

int Axis::assignedGraphs(aux::TPList<Graph> *graphL) {

  int ii=0,cnt=m_graphs.count();
  if (graphL) {
    Graph *graph=0;
    for (ii=0;ii<cnt;ii++) {
      graph=dynamic_cast<Graph*>(m_graphs[ii]);
      if (graph)
        graphL->append(graph);
    }
  }
  return cnt;

}

Graph *Axis::findGraph(const TypeId &grid) {

  return dynamic_cast<Graph*>(m_graphs[m_graphs.find(&grid)]);
    
}

void Axis::setScale(Scale *scale) {

  if (m_scale)
    delete m_scale;
  m_scale=scale;
  
}

int Axis::findAutoBounds(int btype,double *min,double *max) {

  double xmin=mk_dinf,xmax=mk_dsinf,xmintmp=xmin,xmaxtmp=xmax;
  int ii=0;
  Graph *graph=0;
  for (ii=0;ii<m_graphs.count();ii++) {
    graph=dynamic_cast<Graph*>(m_graphs[ii]);
    if (graph) {
      graph->findBounds(btype,&xmintmp,&xmaxtmp);
      if (mk_dlt(xmintmp,xmin))
        xmin=xmintmp;
      if (mk_dlt(xmax,xmaxtmp))
        xmax=xmaxtmp;
    }
  }
  m_autobounds[0]=xmin;
  m_autobounds[1]=xmax;
  if (min)
    *min=xmin;
  if (max)
    *max=xmax;
  int res=0;
  if (!mk_isinf(xmin))
    res|=typeBoundAutoMin;
  if (!mk_isinf(xmax))
    res|=typeBoundAutoMax;

  return res;

}

int Axis::checkAutoBounds(double min,double max) {

  short res=0;
  if (mk_dlt(min,m_autobounds[0])) {
    m_autobounds[0]=min;
    res|=typeBoundAutoMin;
  }
  if (mk_dlt(m_autobounds[1],max)) {
    m_autobounds[1]=max;
    res|=typeBoundAutoMax;
  }

  return res;

}

int Axis::calcTics(double maxlen,double space,aux::TVList<Tic> *ticL) {

  if (!m_scale || !ticL || space<=.0) {
    if (m_scale)
      m_scale->clear();
    return 0;
  }
  m_scale->setRange((m_scale->rangeOption()&typeBoundAutoMin)>0 ? m_autobounds[0] : mk_dnan,
                     (m_scale->rangeOption()&typeBoundAutoMax)>0 ? m_autobounds[1] : mk_dnan);
  int ii=0,maxtics=(int)(maxlen/space/2.),ntics=m_scale->calcTics(maxtics,ticL);
  double sumlen=.0;
  while (ntics>3) {
    for (ii=0;ii<ntics;ii++) {
      sumlen+=ticLen4Size(ticL->at(ii))+space;
      if (sumlen>maxlen)
        break;
    }
    if (sumlen<=maxlen)
      break;
    maxtics=ntics-1;
    sumlen=0.;
    ntics=m_scale->calcTics(maxtics,ticL);
  }
  
  return ntics;

}

// ***

aux::TypeId Xaxis::assignGraph(Graph *graph) {

  TypeId assid=Axis::assignGraph(graph);
  if (!assid.busted()) {
    graph->m_axis[0]=this;
    findAutoBounds(aux::typeX,0,0);
  }
  return assid;
  
}

double Xaxis::ticLen4Size(const Tic *tic) {

  return (m_fnt[tic->m_size].m_metric.width()*tic->m_str.len());

}

double Xaxis::needExcess(osix::xxRectSize size) {

  aux::TVList<Tic> ticL;
  int ntics=calcTics(size.width(),m_fnt[0].m_metric.width(),&ticL);
  if (ntics==0)
    return .0;
  double w0=ticLen4Size(ticL.at(0)),w1=ticLen4Size(ticL.at(ntics-1));
  return MAX(w0,w1);

}

double Xaxis::needSize(osix::xxRectSize) {

  m_size.setHeight(mk_round2(1.5*m_fnt[1].m_metric.height())); // improve 4 angle
  return m_size.height();
  
}

int Xaxis::resize(osix::xxRectSize size) {

  m_size.setWidth(size.width());
  aux::TVList<Tic> ticL;
  return calcTics(m_size.width(),m_fnt[0].m_metric.width(),&ticL);
    
}

int Xaxis::sc2sz(aux::Vector3 *vv) const {

  if (!vv)
    return -1;
  mk_vertexnan(p1);
  mk_vertexnan(p2);
  p1[1]=.0;
  p2[1]=(double)m_size.width();
  if (m_scale)
    m_scale->effRange(&p1[0],&p2[0]);
  vv->setX(mk_lineq(p1,p2,vv->x()));
  return (mk_isnan(vv->x())==0 ? 0 : -1);

}

int Xaxis::sz2sc(aux::Vector3 *vv) const {

  if (!vv)
    return -1;
  mk_vertexnan(p1);
  mk_vertexnan(p2);
  p1[0]=.0;
  p2[0]=(double)m_size.width();
  if (m_scale)
    m_scale->effRange(&p1[1],&p2[1]);
  vv->setX(mk_lineq(p1,p2,vv->x()));
  return (mk_isnan(vv->x())==0 ? 0 : -1);

}

int Xaxis::setPos(int pos) {

  return Axis::setPos(pos);

}

// ***

aux::TypeId Yaxis::assignGraph(Graph *graph) {

  aux::TypeId assid=Axis::assignGraph(graph);
  if (!assid.busted()) {
    graph->m_axis[1]=this;
    findAutoBounds(aux::typeY,0,0);
  }
  return assid;
  
}

double Yaxis::ticLen4Size(const Tic *tic) {

  return m_fnt[tic->m_size].m_metric.height(); 

}

double Yaxis::needExcess(osix::xxRectSize) {
 
  return m_fnt[1].m_metric.height(); // improve 4 angle

}

double Yaxis::needSize(osix::xxRectSize size) {

  aux::TVList<Tic> ticL;
  double ww=1.5*m_fnt[1].m_metric.height(),wtmp=0.;
  int ii=0,ntics=calcTics(size.height(),m_fnt[0].m_metric.height(),&ticL);
  if (ntics<=0)
    return ww;
  aux::Asciistr str;
  for (ii=0;ii<ntics;ii++) { 
    aux::d2a(ticL.at(ii)->m_val,&str,m_scale->interval().m_a);
    wtmp=m_fnt[ticL.at(ii)->m_size].m_metric.width()*str.len()+m_fnt[1].m_metric.height()/2.;
    if (wtmp>ww)
      ww=wtmp;
    str=0;
  }
  ww=mk_round2(ww);
  m_size.setWidth(ww);  
  return ww;

}

int Yaxis::resize(osix::xxRectSize size) {

  m_size.setHeight(size.height());
  aux::TVList<Tic> ticL;
  return calcTics(m_size.height(),m_fnt[0].m_metric.height(),&ticL);
    
}

int Yaxis::sc2sz(aux::Vector3 *vv) const {

  if (!vv)
    return -1;
  mk_vertexnan(p1);
  mk_vertexnan(p2);
  p1[1]=.0;
  p2[1]=(double)m_size.height();
  if (m_scale)
    m_scale->effRange(&p1[0],&p2[0]);
  vv->setY(mk_lineq(p1,p2,vv->y()));
  return (mk_isnan(vv->y())==0 ? 0 : -1);

}

int Yaxis::sz2sc(aux::Vector3 *vv) const {

  if (!vv)
    return -1;
  mk_vertexnan(p1);
  mk_vertexnan(p2);
  p1[0]=.0;
  p2[0]=(double)m_size.height();
  if (m_scale)
    m_scale->effRange(&p1[1],&p2[1]);
  vv->setY(mk_lineq(p1,p2,vv->y()));
  return (mk_isnan(vv->y())==0 ? 0 : -1);

}

int Yaxis::setPos(int pos) {

  return Axis::setPos(pos);

}

} // namespace 
