
#include <graphic/charts/simpleplot/diagram.h>
#include <graphic/charts/simpleplot/scale.h>

namespace simpleplot {

Diagram::Diagram(const char *type,unsigned int idd) : aux::TypeId(type,idd),m_mouseMode(0) {
     	  
}

Diagram::~Diagram() {

  int i=0;
  Graph *graph=0;
  Axis *ax=0;
  aux::TypeId *t=0;
  for (i=0;i<m_corps.count();i++) {
    t=m_corps.remove(i);
    graph=dynamic_cast<Graph*>(t);
    ax=dynamic_cast<Axis*>(t);
    if (graph && graph->m_destr>0) delete graph;
    else if (ax && ax->m_destr>0) delete ax;
    else delete t;
  }

}

Graph *Diagram::removeGraph(const aux::TypeId &grid) {

  return dynamic_cast<Graph*>(m_corps.remove(m_corps.find(&grid)));
    
}

Axis *Diagram::removeAxis(const TypeId &axid) {

  return dynamic_cast<Axis*>(m_corps.remove(m_corps.find(&axid)));
	
}

aux::TypeId Diagram::setGraph(Graph *graph) {

  if (!graph || m_corps.find((const aux::TypeId*)graph)>=0) return aux::TypeId();
  m_corps.inSort(graph);
  return *((aux::TypeId*)graph);
  
}

aux::TypeId Diagram::setAxis(Axis *axis) {

  if (!axis || m_corps.find((const aux::TypeId*)axis)>=0) return aux::TypeId();
  m_corps.inSort(axis);
  return *((aux::TypeId*)axis);
  
}

Graph *Diagram::graph(const aux::TypeId &grid) {

  return dynamic_cast<Graph*>(m_corps[m_corps.find(&grid)]);
    
}

Axis *Diagram::axis(const aux::TypeId &axid) {

  return dynamic_cast<Axis*>(m_corps[m_corps.find(&axid)]);
      
}

int Diagram::typegraphs(const char *grtype,aux::TPList<Graph> *graphL) {

  aux::TypeId look(grtype);
  if (look.busted()) return 0;
  m_corps.cmp=aux::cmpType;
  aux::TVArr<int> idxL;
  int i=0,n=m_corps.findAll(&look,&idxL);
  m_corps.cmp=0;
  if (!graphL) return n;
  graphL->clear();
  if (graphL->size()<n) graphL->resize(n);
  Graph *graph;
  for (i=0;i<n;i++) {
    graph=dynamic_cast<Graph*>(m_corps[idxL[i]]);
    if (graph) graphL->inSort(graph);
  }
  return n;
    
}

int Diagram::typeaxes(const char *axtype,aux::TPList<Axis> *axisL) {

  aux::TypeId look(axtype);
  if (look.busted()) return 0;
  m_corps.cmp=aux::cmpType;
  aux::TVArr<int> idxL;
  int i=0,n=m_corps.findAll(&look,&idxL);
  m_corps.cmp=0;   
  if (!axisL) return n;  
  axisL->clear();
  if (axisL->size()<n) axisL->resize(n);
  Axis *ax=0;
  for (i=0;i<n;i++) {
    ax=dynamic_cast<Axis*>(m_corps[idxL[i]]);
    if (ax) axisL->inSort(ax);
  }
  return n;

}

int Diagram::setMouseMode(int mode,bool combine) {

  if (mode>=0 && mode<=4) {
    if (combine) m_mouseMode|=mode;
    else m_mouseMode=mode;
  }
  return m_mouseMode;

}

int Diagram::selected(aux::TVList<DiagramSelection> *selL) const {

  int i=0,cnt=m_selected.count();
  if (!selL) return cnt;
  selL->clear();
  if (selL->size()<cnt) selL->resize(cnt);
  for (i=0;i<cnt;i++) selL->inSort(m_selected[i]);
  return cnt;

}

// ***
 
DiagramXY::DiagramXY(int sz) : Diagram("diagramxy") {

  m_corps.resize(sz);
  	  
} 

int DiagramXY::setSelection(DiagramSelection sel,bool add) {

  int idx=-1;
  if (!add) m_selected.clear();
  else idx=m_selected.find(sel);
  if (idx<0 && !sel.m_sel.busted()) idx=m_selected.inSort(sel); 
  return idx;

}

aux::TypeId DiagramXY::selectGraph(num::Vector3 p,bool set,bool add) {

  DiagramSelection sel;
  if (p.busted(3)) {
    if (set) setSelection(sel,add);
    return aux::TypeId();
  }
  aux::TypeId gr("graphxy");
  aux::TVArr<int> gridxL;
  m_corps.cmp=aux::cmpType;
  int i=0,idx=-1,ngr=m_corps.findAll(&gr,&gridxL);
  m_corps.cmp=0;
  Graph *graph=0;
  for (i=0;i<ngr;i++) {
    graph=dynamic_cast<Graph*>(m_corps[gridxL[i]]);
    idx=(graph ? graph->match(p) : -1);
    if (idx>=0) {
      sel.set(*((aux::TypeId*)graph),idx,graph->value(idx));
      break;
    }
  }
//aux::Numstr str;
//sel.m_sel.toString(&str);
//printf ("selectGraph : %s idx=%d val=%f,%f\n",(const char*)str,sel.m_idx,sel.m_val.x(),sel.m_val.y());
  if (set) setSelection(sel,add);
  return sel.m_sel;

}

shapes::RectSize DiagramXY::setup(shapes::RectSize sz,shapes::RectSize *excess) {

  if (m_corps.count()==0) return sz;
  aux::TypeId xax("xaxis"),yax("yaxis"),gr("graphxy");
  aux::TVArr<int> xidxL,yidxL,gridxL;
  m_corps.cmp=aux::cmpType;
  int i=0,nx=m_corps.findAll(&xax,&xidxL),
          ny=m_corps.findAll(&yax,&yidxL),
          ngr=m_corps.findAll(&gr,&gridxL);
  m_corps.cmp=0;
  Axis *ax=0;
  Graph *graph=0;
  double hexcess=.0,wexcess=.0,tmp=.0;
  for (i=0;i<nx;i++) {  
    ax=dynamic_cast<Axis*>(m_corps[xidxL[i]]); 
    if (ax) tmp=ax->needExcess(sz);
    if (tmp>wexcess) wexcess=tmp;
  }
  wexcess=aux::round2(wexcess);
  wexcess+=(double)((int)wexcess%2);
  sz.setWidth(sz.width()-wexcess);
  for (i=0;i<ny;i++) {
    ax=dynamic_cast<Axis*>(m_corps[yidxL[i]]); 
    if (ax) tmp=ax->needExcess(sz);
    if (tmp>hexcess) hexcess=tmp;
  }
  hexcess=aux::round2(hexcess);
  hexcess+=(double)((int)hexcess%2);
  if (excess) *excess=shapes::RectSize(wexcess,hexcess);
  sz.setHeight(sz.height()-hexcess);
  for (i=0;i<nx;i++) {  
    ax=dynamic_cast<Axis*>(m_corps[xidxL[i]]); 
    if (ax) sz.setHeight(sz.height()-ax->needSize(sz));
  }
  for (i=0;i<ny;i++) {
    ax=dynamic_cast<Axis*>(m_corps[yidxL[i]]); 
    if (ax) sz.setWidth(sz.width()-ax->needSize(sz));
  }
  for (i=0;i<nx;i++) {  
    ax=dynamic_cast<Axis*>(m_corps[xidxL[i]]); 
    if (ax) ax->resize(sz);
  }
  for (i=0;i<ny;i++) {
    ax=dynamic_cast<Axis*>(m_corps[yidxL[i]]); 
    if (ax) ax->resize(sz);
  }
  for (i=0;i<ngr;i++) {
    graph=dynamic_cast<Graph*>(m_corps[gridxL[i]]);
    if (graph) graph->rescale();
  }
    
  return sz;
  
}

} // namespace simpleplot
