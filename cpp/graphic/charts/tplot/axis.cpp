
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/param.h>
#include <time.h>
#include <graphic/charts/simpleplot/scale.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/charts/simpleplot/graph.h>

using namespace aux;
using namespace shapes;

namespace simpleplot {

Scale *buildScale(const char *type) {

  if (!type) return 0;
  int i=0,idx=-1,len=strlen(type);
  if (len==0) return 0;
  for (i=0;i<numscaletypes;i++) {
    if (strncmp(type,scaletypes[i],len)==0) {
      idx=i;
      break;
    }
  }
  if (idx==1) return new Scale();
  return 0;

}

Axis::Axis(const char *type,unsigned int idd) : 
  TypeId(type,idd),m_scale(0),m_pos(0),m_destr(0),m_hasAutoBounds(1),
  m_visible(15),m_style(somecolors[black],1,1) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
  
  m_fnt[0]=m_fnt[1]=Fnt();
  	    
}

Axis::~Axis () {
    
  if (m_scale) delete m_scale;
  m_graphs.clear();
  
}

TypeId Axis::assignGraph(Graph *graph) {

  if (m_graphs.find(graph)>=0 || m_graphs.inSort(graph)<0) return TypeId();
  return TypeId((const TypeId&)(*graph));
}

int Axis::unassignGraph(const TypeId &grid) {

  int idx=m_graphs.find(&grid);
  if (idx>=0) {
    m_graphs.remove(idx);
    findAutoBounds();
  }
  return m_graphs.count();
  
}

int Axis::assignedGraphs(TPList<Graph> *graphL) {

  int i=0,cnt=m_graphs.count();
  if (graphL) {
    Graph *graph=0;
    for (i=0;i<cnt;i++) {
      graph=dynamic_cast<Graph*>(m_graphs[i]);
      if (graph) graphL->append(graph);
    }
  }
  return cnt;

}

Graph *Axis::findGraph(const TypeId &grid) {

  return dynamic_cast<Graph*>(m_graphs[m_graphs.find(&grid)]);
    
}

void Axis::setScale(Scale *scale) {

  if (m_scale) delete m_scale;
  m_scale=scale;
  
}

aux::MinMax Axis::findAutoBounds(int btype) {

  aux::MinMax b(dinf,dsinf),btmp=b;
  int i=0;
  Graph *graph=0;
  for (i=0;i<m_graphs.count();i++) {
    graph=dynamic_cast<Graph*>(m_graphs[i]);
    if (graph) btmp=graph->findBounds(btype);
    if (dlt(btmp[0],b[0])) b[0]=btmp[0];
    if (dlt(b[1],btmp[1])) b[1]=btmp[1];
  }
  m_autoBounds=b;
  return m_autoBounds;

}

int Axis::checkAutoBounds(aux::MinMax b) {

  int mod=0;
  if (dlt(b[0],m_autoBounds[0])) {
    m_autoBounds[0]=b[0];
    mod=1;
  }
  if (dlt(m_autoBounds[1],b[1])) {
    m_autoBounds[1]=b[1];
    mod|=2;
  }
  return mod;

}

int Axis::calcTics(double maxlen,double space,TPList<Tic> *ticL) {

  if (!m_scale || !ticL || space<=.0) {
    if (m_scale) m_scale->clear();
    return 0;
  }
  if (m_hasAutoBounds) m_scale->setBounds(m_autoBounds);
  int i=0,maxtics=(int)(maxlen/space/2.),ntics=m_scale->calcTics(maxtics,ticL);
  double sumlen=.0;
  while (ntics>3) {
    for (i=0;i<ntics;i++) {
      sumlen+=ticLen4Size(ticL->at(i))+space;
      if (sumlen>maxlen) break;
    }
    if (sumlen<=maxlen) break;
    maxtics=ntics-1;
    sumlen=0.;
    ntics=m_scale->calcTics(maxtics,ticL);
  }
  
  return ntics;

}

// ***

TypeId Xaxis::assignGraph(Graph *graph) {

  TypeId assid=Axis::assignGraph(graph);
  if (!assid.busted()) {
    graph->m_axis[0]=this;
    findAutoBounds();
  }
  return assid;
  
}

double Xaxis::ticLen4Size(const Tic *tic) {

  if (!tic) return 0.;
  Numstr str;
  tic->m_val.toString (&str);
  return (m_fnt[tic->m_size].m_metric.width()*str.len());

}

double Xaxis::needExcess(RectSize size) {

  TPList<Tic> ticL; 
  int ntics=calcTics(size.width(),m_fnt[0].m_metric.width(),&ticL);
  if (ntics==0) return .0; 
  double w0=ticLen4Size(ticL[0]),w1=ticLen4Size(ticL[ntics-1]);
  return MAX(w0,w1);

}

double Xaxis::needSize(RectSize) {

  m_size.setHeight(aux::round2(1.5*m_fnt[1].m_metric.height())); // improve 4 angle
  return m_size.height();
  
}

int Xaxis::resize(RectSize size) {

  m_size.setWidth(size.width());
  TPList<Tic> ticL; 
  return calcTics(m_size.width(),m_fnt[0].m_metric.width(),&ticL);
    
}

void Xaxis::sc2sz(num::Vector3 *v) const {

  if (m_scale && v) { 
    aux::MinMax b=m_scale->effBounds();
    v->setX(linEq(b[0],b[1],0.,m_size.width(),v->x()));
  }
  else if (v) v->setX(aux::dnan);

}

void Xaxis::sz2sc(num::Vector3 *v) const {

  if (m_scale && v) { 
    aux::MinMax b=m_scale->effBounds();
    v->setX(linEq(0.,m_size.width(),b[0],b[1],v->x()));
  }
  else if (v) v->setX(aux::dnan);

}

// ***

TypeId Yaxis::assignGraph(Graph *graph) {

  TypeId assid=Axis::assignGraph(graph);
  if (!assid.busted()) {
    graph->m_axis[1]=this;
    findAutoBounds();
  }
  return assid;
  
}

double Yaxis::ticLen4Size(const Tic *tic) {

  return m_fnt[tic->m_size].m_metric.height(); 

}

double Yaxis::needExcess(RectSize) {
 
  return m_fnt[1].m_metric.height(); // improve 4 angle

}

double Yaxis::needSize(RectSize size) {

  TPList<Tic> ticL;
  double w=1.5*m_fnt[1].m_metric.height(),wtmp=0.;
  int i=0,ntics=calcTics(size.height(),m_fnt[0].m_metric.height(),&ticL);
  if (ntics<=0) return w;
  Numstr str;
  for (i=0;i<ntics;i++) { 
    ticL[i]->m_val.toString (&str);
    wtmp=m_fnt[ticL[i]->m_size].m_metric.width()*str.len()+m_fnt[1].m_metric.height()/2.;
    if (wtmp>w) w=wtmp;
    str=0;
  }
  w=aux::round2(w);
  m_size.setWidth(w);  
  return w;

}

int Yaxis::resize(RectSize size) {

  m_size.setHeight(size.height());
  TPList<Tic> ticL;
  return calcTics(m_size.height(),m_fnt[0].m_metric.height(),&ticL);
    
}

void Yaxis::sc2sz(num::Vector3 *v) const {

  if (m_scale && v) { 
    aux::MinMax b=m_scale->effBounds();
    v->setY(linEq(b[0],b[1],0.,m_size.height(),v->y()));
  }
  else if (v) v->setY(aux::dnan);

}

void Yaxis::sz2sc(num::Vector3 *v) const {

  if (m_scale && v) { 
    aux::MinMax b=m_scale->effBounds();
    v->setY(linEq(0.,m_size.height(),b[0],b[1],v->y()));
  }
  else if (v) v->setY(aux::dnan);

}

} // namespace 
