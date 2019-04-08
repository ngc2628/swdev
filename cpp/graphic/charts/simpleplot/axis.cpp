
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
  mk_listalloc(&m_graphs,sizeof(mk::TypeId*),63);
  m_graphs.cmp=mk::cmpTypeIdRef;
  
}

Axis::~Axis () {
    
  if (m_scale)
    delete m_scale;
  mk_listfree(&m_graphs);
  
}

int Axis::assignGraph(Graph *graph) {

  int idxl=-1,idxh=-1;
  mk_listfind(&m_graphs,(void*)&graph,&idxl,&idxh);
  if (idxl<0)
    idxl=mk_listinsort(&m_graphs,(void*)&graph);

  return idxl;

}

int Axis::unassignGraph(mk::TypeId grid) {

  mk::TypeId look(grid);
  int idx=mk_listfind(&m_graphs,(void*)&look,0,0);
  if (idx>=0) {
    mk_listremove(&m_graphs,idx,(void*)&look);
    findAutoBounds(3,0,0);
  }
  return m_graphs.count;
  
}

int Axis::assignedGraphs(mk_list *graphL) {

  int ii=0,cnt=m_graphs.count;
  if (graphL) 
    mk_listcopy(graphL,&m_graphs);
  return cnt;

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
  for (ii=0;ii<m_graphs.count;ii++) {
    mk_listat(&m_graphs,ii,(void*)&graph);
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

int Axis::calcTics(double maxlen,double space,mk_list *ticL) {

  if (!m_scale || !ticL || space<=.0) {
    if (m_scale)
      m_scale->clear();
    return 0;
  }
  m_scale->setRange((m_scale->rangeOption()&typeBoundAutoMin)>0 ? m_autobounds[0] : mk_dnan,
                     (m_scale->rangeOption()&typeBoundAutoMax)>0 ? m_autobounds[1] : mk_dnan);
  int ii=0,maxtics=(int)(maxlen/space/2.),ntics=m_scale->calcTics(maxtics,ticL);
  double sumlen=.0;
  Tic tic;
  m_scale->tics(ticL);
  while (ntics>3) {
    for (ii=0;ii<ntics;ii++) {
      mk_listat(ticL,ii,(void *)&tic);
      sumlen+=ticLen4Size(&tic)+space;
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

int Xaxis::assignGraph(Graph *graph) {

  int res=Axis::assignGraph(graph);
  if (res>=0) {
    graph->m_axis[0]=this;
    findAutoBounds(1,0,0);
  }
  return res;
  
}

double Xaxis::ticLen4Size(const Tic *tic) {

  mk::Ucsstr ticstr;
  tic->ucsText(&ticstr);
  return (m_fnt[tic->m_size].m_metric.width()*ticstr.length());

}

double Xaxis::needExcess(osix::xxRectSize size) {

  mk_list ticL;
  mk_listalloc(&ticL,sizeof(Tic),0);
  int ntics=calcTics(size.width(),m_fnt[0].m_metric.width(),&ticL);
  if (ntics==0)
    return .0;
  Tic tic;
  mk_listat(&ticL,0,(void*)&tic);
  double w0=ticLen4Size(&tic);
  mk_listat(&ticL,ntics-1,(void*)&tic);
  double w1=ticLen4Size(&tic);
  mk_listfree(&ticL);
  return MAX(w0,w1);

}

double Xaxis::needSize(osix::xxRectSize) {

  m_size.setHeight(mk_round2(1.5*m_fnt[1].m_metric.height())); // improve 4 angle
  return m_size.height();
  
}

int Xaxis::resize(osix::xxRectSize size) {

  m_size.setWidth(size.width());
  mk_list ticL;
  mk_listalloc(&ticL,sizeof(Tic),0);
  int res=calcTics(m_size.width(),m_fnt[0].m_metric.width(),&ticL);
  mk_listfree(&ticL);
  return res;
    
}

int Xaxis::sc2sz(mk_vertex vv) const {

  mk_vertexnan(p1);
  mk_vertexnan(p2);
  p1[1]=.0;
  p2[1]=(double)m_size.width();
  if (m_scale)
    m_scale->effRange(&p1[0],&p2[0]);
  vv[0]=mk_lineq(p1,p2,vv[0]);
  return (mk_isnan(vv[0])==0 ? 0 : -1);

}

int Xaxis::sz2sc(mk_vertex vv) const {

  mk_vertexnan(p1);
  mk_vertexnan(p2);
  p1[0]=.0;
  p2[0]=(double)m_size.width();
  if (m_scale)
    m_scale->effRange(&p1[1],&p2[1]);
  vv[0]=mk_lineq(p1,p2,vv[0]);
  return (mk_isnan(vv[0])==0 ? 0 : -1);

}

int Xaxis::setPos(int pos) {

  return Axis::setPos(pos);

}

// ***

int Yaxis::assignGraph(Graph *graph) {

  int res=Axis::assignGraph(graph);
  if (res>=0) {
    graph->m_axis[1]=this;
    findAutoBounds(2,0,0);
  }
  return res;
  
}

double Yaxis::ticLen4Size(const Tic *tic) {

  return m_fnt[tic->m_size].m_metric.height(); 

}

double Yaxis::needExcess(osix::xxRectSize) {
 
  return m_fnt[1].m_metric.height(); // improve 4 angle

}

double Yaxis::needSize(osix::xxRectSize size) {

  double ww=1.5*m_fnt[1].m_metric.height(),wtmp=0.;
  mk_list ticL;
  mk_listalloc(&ticL,sizeof(Tic),0);
  int ii=0,ntics=calcTics(size.height(),m_fnt[0].m_metric.height(),&ticL);
  if (ntics<=0) {
    mk_listfree(&ticL);
    return ww;
  }
  mk_string str;
  Tic tic;
  for (ii=0;ii<ntics;ii++) { 
    mk_listat(&ticL,ii,(void*)&tic);
    mk_d2a(tic.m_val,str,m_scale->interval().m_a);
    mk_listsetat(&ticL,(void*)&tic,ii,0);
    wtmp=m_fnt[tic.m_size].m_metric.width()*mk_stringlength(str)+m_fnt[1].m_metric.height()/2.;
    if (wtmp>ww)
      ww=wtmp;
    mk_stringset(str,0);
  }
  ww=mk_round2(ww);
  m_size.setWidth(ww); 
  mk_listfree(&ticL); 
  return ww;

}

int Yaxis::resize(osix::xxRectSize size) {

  m_size.setHeight(size.height());
  mk_list ticL;
  mk_listalloc(&ticL,sizeof(Tic),0);
  int res=calcTics(m_size.height(),m_fnt[0].m_metric.height(),&ticL);
  mk_listfree(&ticL);
  return res;
    
}

int Yaxis::sc2sz(mk_vertex vv) const {

  mk_vertexnan(p1);
  mk_vertexnan(p2);
  p1[1]=.0;
  p2[1]=(double)m_size.height();
  if (m_scale)
    m_scale->effRange(&p1[0],&p2[0]);
  vv[1]=mk_lineq(p1,p2,vv[1]);
  return (mk_isnan(vv[1])==0 ? 0 : -1);

}

int Yaxis::sz2sc(mk_vertex vv) const {

  mk_vertexnan(p1);
  mk_vertexnan(p2);
  p1[0]=.0;
  p2[0]=m_size.height();
  if (m_scale)
    m_scale->effRange(&p1[1],&p2[1]);
  vv[1]=mk_lineq(p1,p2,vv[1]);
  return (mk_isnan(vv[1])==0 ? 0 : -1);

}

int Yaxis::setPos(int pos) {

  return Axis::setPos(pos);

}

} // namespace 
