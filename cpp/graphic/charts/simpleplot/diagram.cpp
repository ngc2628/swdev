
#include <graphic/charts/simpleplot/diagram.h>
#include <graphic/charts/simpleplot/scale.h>
#include <osix/xxpaint.h>

namespace simpleplot {

Diagram::Diagram(const char *type,unsigned int idd) : 
  aux::TypeId(type,idd),m_mouseMode(0) {
    	  
}

Diagram::~Diagram() {

}

Graph *Diagram::graph(const aux::TypeId &grid) {

  return dynamic_cast<Graph*>(m_corps[m_corps.find(&grid)]);
    
}

Axis *Diagram::axis(const aux::TypeId &axid) {

  return dynamic_cast<Axis*>(m_corps[m_corps.find(&axid)]);

}

aux::TypeId Diagram::setGraph(Graph *graph) {

  if (!graph || m_corps.find((const aux::TypeId*)graph)>=0)
    return aux::TypeId();
  m_corps.inSort(graph);
  return *((aux::TypeId*)graph);
  
}

aux::TypeId Diagram::setAxis(Axis *axis) {

  if (!axis || m_corps.find((const aux::TypeId*)axis)>=0)
    return aux::TypeId();
  m_corps.inSort(axis);
  return *((aux::TypeId*)axis);
  
}

Graph *Diagram::removeGraph(const aux::TypeId &grid) {

  return dynamic_cast<Graph*>(m_corps.remove(m_corps.find(&grid)));
    
}

Axis *Diagram::removeAxis(const aux::TypeId &axid) {

  return dynamic_cast<Axis*>(m_corps.remove(m_corps.find(&axid)));

}

int Diagram::clear(bool destr) {

  int ii=0,cnt=m_corps.count();
  aux::TypeId *tt=0;
  for (ii=cnt-1;ii>-1;ii--) {
    tt=m_corps.remove(ii);
    if (destr)
      delete tt;
  }

  return cnt;

}

int Diagram::typegraphs(const char *grtype,aux::TPList<Graph> *graphL) {

  aux::TypeId look(grtype);
  if (look.busted())
    return 0;
  m_corps.cmp=aux::cmpType;
  aux::TVArr<int> idxL;
  int ii=0,nn=m_corps.findAll(&look,&idxL);
  m_corps.cmp=0;
  if (!graphL)
    return nn;
  graphL->clear();
  if (graphL->size()<nn)
    graphL->resize(nn);
  Graph *graph;
  for (ii=0;ii<nn;ii++) {
    graph=dynamic_cast<Graph*>(m_corps[idxL[ii]]);
    if (graph)
      graphL->inSort(graph);
  }
  return nn;
    
}

int Diagram::typeaxes(const char *axtype,aux::TPList<Axis> *axisL) {

  aux::TypeId look(axtype);
  if (look.busted())
    return 0;
  m_corps.cmp=aux::cmpType;
  aux::TVArr<int> idxL;
  int ii=0,n=m_corps.findAll(&look,&idxL);
  m_corps.cmp=0;   
  if (!axisL)
    return n;
  axisL->clear();
  if (axisL->size()<n)
    axisL->resize(n);
  Axis *ax=0;
  for (ii=0;ii<n;ii++) {
    ax=dynamic_cast<Axis*>(m_corps[idxL[ii]]);
    if (ax)
      axisL->inSort(ax);
  }
  return n;

}

int Diagram::selected(aux::TVList<DiagramSelection> *selL) const {

  int ii=0,cnt=m_selected.count();
  if (!selL)
    return cnt;
  selL->clear();
  if (selL->size()<cnt)
    selL->resize(cnt);
  for (ii=0;ii<cnt;ii++)
    selL->inSort(m_selected[ii]);
  return cnt;

}

int Diagram::setSelection(DiagramSelection sel,int) {

  return -1;

}

aux::TypeId Diagram::selectGraph(num::Vector3,int,int) {

  return aux::TypeId();
    
}

int Diagram::mouseMode(int *mod) {

  if (mod) {
    if (((*mod)&mouseCoorTracking)>0)
      m_mouseMode|=mouseCoorTracking;
    else
      m_mouseMode&=(mouseCrosshair|mouseZoom|mouseModify);
    if (((*mod)&mouseCrosshair)>0)
      m_mouseMode|=mouseCrosshair;
    else
      m_mouseMode&=(mouseCoorTracking|mouseZoom|mouseModify);
    if (((*mod)&mouseZoom)>0)
      m_mouseMode|=mouseZoom;
    else
      m_mouseMode&=(mouseCoorTracking|mouseCrosshair|mouseModify);
    if (((*mod)&mouseModify)>0)
      m_mouseMode|=mouseModify;
    else
      m_mouseMode&=(mouseCoorTracking|mouseCrosshair|mouseZoom);
  }
  return m_mouseMode;

}

int Diagram::redraw(int flag) {

  if (flag==0)
    return 0;
//printf ("%d redraw : %d\n",__LINE__,flag);
  //void *disp=findDisplay();
  if ((flag&redoScaleRect)>0)
    flag|=resetRect();
  if ((flag&redoScaleGraph)>0)
    flag|=rescaleGraph();
  if ((flag&redoDrawAxes)>0)
    flag|=drawAxes();
  if ((flag&redoDrawGraph)>0)
    flag|=drawGraphs();
  if ((flag&redoScr)>0)
    osUpdate();

  return 0;

}

// **********
int Diagram::osEvent(void *disp,void *osev) {

  osix::xxEvent xxev;
  osix::xxtranslateEvent(osev,&xxev);
  int redrawflag=0;
  osix::xxGC gc;
  gc.m_bg=m_bg;
  gc.m_r=xxev.m_r;
  if (xxev.m_type==osix::xx_resize) {
    m_winscr.m_r=gc.m_r;
    osix::xxresizeDrawable(disp,&m_pixscr,&gc);
    redrawflag|=(redoScaleRect|redoDrawAxes|redoDrawGraph|redoScr);  
  }
  else if (xxev.m_type==osix::xx_paint)
    osix::xxcopyArea(disp,&m_winscr,&m_pixscr,&gc,m_winscr.m_r,m_pixscr.m_r);
  else if (xxev.m_type==osix::xx_enter)
    redrawflag|=mouseEnter(&xxev);
  else if (xxev.m_type==osix::xx_leave) {
    redrawflag|=mouseLeave(&xxev);
  }
  else if (osix::xxTypeInput(&xxev)>0)
    redrawflag|=inputEvent(&xxev);

//xxev.type2txt(&strerr1);
//printf ("%d event=%s flag=%d\n",__LINE__,(const char *)strerr1,redrawflag);
  redraw(redrawflag);
  return xxev.m_consumer;  

}

int Diagram::inputEvent(osix::xxEvent *xxev) {

  int res=0;
  if (osix::xxTypeMouse(xxev)>0) {
    xxev->m_lastpos=m_xxlastinputev.m_pos;
    if (xxev->m_type==osix::xx_mousePressed)
      res|=mousePressed(xxev);
    else if (xxev->m_type==osix::xx_mouseMove) {
      xxev->m_downpos=m_xxlastinputev.m_downpos;
      res|=mouseMoved(xxev);   
    }
    else if (xxev->m_type==osix::xx_mouseReleased) {
      xxev->m_downpos=m_xxlastinputev.m_downpos;
      if ((m_xxlastinputev.m_consumer&osix::xx_consumed)>0)
        xxev->m_consumer|=osix::xx_consumed;  
      if ((m_xxlastinputev.m_consumer&osix::xx_processed)>0)
        res|=mouseReleased(xxev);
    }
  }

  m_xxlastinputev=*xxev;

  return res;

}

// ***
 
DiagramXY::DiagramXY(int sz) : Diagram("diagramxy") {

  m_corps.resize(sz);
  	  
} 

int DiagramXY::setSelection(DiagramSelection sel,int add) {

  int idx=-1;
  if (add==0)
    m_selected.clear();
  else
    idx=m_selected.find(sel);
  if (idx<0 && !sel.m_sel.busted())
    idx=m_selected.inSort(sel);
  return idx;

}

aux::TypeId DiagramXY::selectGraph(num::Vector3 pp,int set,int add) {

  DiagramSelection sel;
  if (pp.busted(num::typeXY)) {
    if (set>0)
      setSelection(sel,add);
    return aux::TypeId();
  }
  aux::TypeId gr("graphxy");
  aux::TVArr<int> gridxL;
  m_corps.cmp=aux::cmpType;
  int ii=0,idx=-1,ngr=m_corps.findAll(&gr,&gridxL);
  m_corps.cmp=0;
  Graph *graph=0;
  for (ii=0;ii<ngr;ii++) {
    graph=dynamic_cast<Graph*>(m_corps[gridxL[ii]]);
    idx=(graph ? graph->match(pp) : -1);
    if (idx>=0) {
      sel.clear(*((aux::TypeId*)graph),idx,graph->value(idx));
      break;
    }
  }

//sel.m_sel.toString(&strerr1);
//printf ("%d selectGraph : %s idx=%d val=%f,%f\n",__LINE__,(const char*)strerr1,sel.m_idx,sel.m_val.x(),sel.m_val.y());
  if (set>0)
    setSelection(sel,add);
  return sel.m_sel;

}

int DiagramXY::resetRect() {

  //printf ("%d resetRect\n",__LINE__);
  osix::xxRect baserect=m_pixscr.m_r;
  osix::xxRectSize excess,msz=setup(baserect.size(),&excess);
  baserect.setLeft(baserect.left()+excess.width()/2.);
  baserect.setRight(baserect.right()-excess.width()/2.);
  baserect.setTop(baserect.top()+excess.height()/2.);
  baserect.setBottom(baserect.bottom()-excess.height()/2.);
  aux::TPList<Axis> xaxL,yaxL;
  int ii=0,nxax=typeaxes("xaxis",&xaxL),nyax=typeaxes("yaxis",&yaxL);
  double xl=baserect.left(),yt=baserect.top(),xr=baserect.right(),yb=baserect.bottom();
  for (ii=0;ii<nxax;ii++) {
    if (xaxL[ii]->pos()==typeTop)
      yt+=xaxL[ii]->size().height();
    else if (xaxL[ii]->pos()==typeBottom)
      yb-=xaxL[ii]->size().height();
  }
  for (ii=0;ii<nyax;ii++) {
    if (yaxL[ii]->pos()==typeLeft)
      xl+=yaxL[ii]->size().width();
    else if (yaxL[ii]->pos()==typeRight)
      xr-=yaxL[ii]->size().width();
  }
  osix::xxGC gc;
  gc.m_bg=m_bg;
  gc.m_r=osix::xxRect(xl,yt,xr,yb);
  osix::xxresizeDrawable(findDisplay(),&m_pixplot,&gc);
  
  return rescaleGraph();

}

int DiagramXY::rescaleGraph() {

  aux::TPList<Graph> grL;
  int ii=0,ngr=typegraphs("graphxy",&grL);
  for (ii=0;ii<ngr;ii++) {
    grL.at(ii)->rescale();
  }
  return (ngr>0 ? redoDrawGraph : 0);

}

int DiagramXY::drawAxes() {

  //printf ("%d drawAxes\n",__LINE__);
  osix::xxRect lr(m_pixscr.m_r.left(),m_pixscr.m_r.top(),m_pixplot.m_r.left(),m_pixscr.m_r.bottom()),
             tr(m_pixplot.m_r.left(),m_pixscr.m_r.top(),m_pixscr.m_r.right(),m_pixplot.m_r.top()),
             rr(m_pixplot.m_r.right(),m_pixplot.m_r.top(),m_pixscr.m_r.right(),m_pixscr.m_r.bottom()),
             br(m_pixplot.m_r.left(),m_pixplot.m_r.bottom(),m_pixplot.m_r.right(),m_pixscr.m_r.bottom());

  void *disp=findDisplay();
  osix::xxGC gc;
  gc.m_bg=m_bg;
  osix::xxpredraw(disp,&m_pixscr,&gc);
  
  if (!lr.empty()) {
    gc.m_r=lr;
    osix::xxfillRect(disp,&m_pixscr,&gc);
  }

  if (!tr.empty()) {
    gc.m_r=tr;
    osix::xxfillRect(disp,&m_pixscr,&gc);
  }

  if (!rr.empty()) {
    gc.m_r=rr;
    osix::xxfillRect(disp,&m_pixscr,&gc);
  }

  if (!br.empty()) {
    gc.m_r=br;
    osix::xxfillRect(disp,&m_pixscr,&gc);
  }

  aux::TPList<Axis> xaxL,yaxL;
  int ii=0,jj=0,nxax=typeaxes("xaxis",&xaxL),nyax=typeaxes("yaxis",&yaxL);
  if (nxax==0 && nyax==0) {
    osix::xxpostdraw(disp,&m_pixscr,&gc);
    return 0;
  }
//printf ("%d nxax=%d nyax=%d\n",__LINE__,nxax,nyax);
  double basex0=m_pixplot.m_r.left(),basex1=m_pixplot.m_r.right(),
         basey0=m_pixplot.m_r.bottom(),basey1=m_pixplot.m_r.top(),baseyy0=basey0,baseyy1=basey1,
         wmetric=.0,hmetric=.0;
  Axis *ax=0;
  Scale *sc=0;
  Tic *tic=0;
  aux::Ucsstr ticstr;
  aux::TVList<Tic> ticL;
  int ntics=0,ticsz=0;
  num::Vector3 v0,v1;
  double basepos=.0,marklen=.0;
  for (ii=0;ii<nxax;ii++) {
    ax=xaxL[ii];
    gc.m_fg=ax->m_style;
    if (ax->pos()==typeTop) {
      basepos=baseyy1-1;
      baseyy1-=ax->size().height();
    }
    else {
      basepos=baseyy0;
      baseyy0+=ax->size().height();
    }
    gc.m_r.set(basex0,basepos,basex1,basepos);
    osix::xxdrawLine(disp,&m_pixscr,&gc);
    sc=ax->scale();
    if (!sc)
      continue;
    ntics=sc->tics(&ticL);
    for (jj=0;jj<ntics;jj++) {
      tic=ticL.at(jj);
      ticsz=tic->m_size;
      wmetric=ax->m_fnt[ticsz].m_metric.width();
      hmetric=ax->m_fnt[ticsz].m_metric.height();
      marklen=hmetric/3.;
      if (marklen<3.)
        marklen=3.;
      v0.setXY(tic->m_val,basepos);
      ax->sc2sz(&v0);
      v0.setX(v0[0]+basex0);
      v1=v0;
      v1.setY(v1[1]+(ax->pos()==typeTop ? -marklen : marklen));
      gc.m_r.set(v0,v1);
      osix::xxdrawLine(disp,&m_pixscr,&gc);
      if (tic->m_drawable==0)
        continue;
      tic->ucsText(&ticstr);
      v0.setX(v0[0]-(double)ticstr.length()*wmetric/2.);
      v0.setY(v0[1]+(ax->pos()==typeTop ? -marklen-hmetric : marklen+hmetric));
      v1.setX(v1[0]+(double)ticstr.length()*wmetric/2.);
      gc.m_r.set(v0,v1);
      gc.m_fnt=ax->m_fnt[ticsz];
      osix::xxdrawText(disp,&m_pixscr,&gc,&ticstr,0);
    }
  }

  for (ii=0;ii<nyax;ii++) {
    ax=yaxL[ii];
    if (ax->pos()==typeRight) {
      basepos=basex1;
      basex1+=ax->size().width();
    }
    else {
      basepos=basex0;
      basex0-=ax->size().width();
    }
    gc.m_r.set(basepos,basey0,basepos,basey1);
    osix::xxdrawLine(disp,&m_pixscr,&gc);
    sc=ax->scale();
    if (!sc)
      continue;
    ntics=sc->tics(&ticL);
    for (jj=0;jj<ntics;jj++) {
      tic=ticL.at(jj);
      ticsz=tic->m_size;
      wmetric=ax->m_fnt[ticsz].m_metric.width();
      hmetric=ax->m_fnt[ticsz].m_metric.height();
      marklen=hmetric/3.;
      if (marklen<3.)
        marklen=3.;
      v0.setXY(basepos,tic->m_val);
      ax->sc2sz(&v0);
      v0.setY(basey0-v0[1]);
      v1=v0;
      v1.setX(v1[0]+(ax->pos()==typeRight ? marklen : -marklen));
      gc.m_r.set(v0,v1);
      osix::xxdrawLine(disp,&m_pixscr,&gc);
      if (tic->m_drawable==0)
        continue;
      tic->ucsText(&ticstr);
      if (ax->pos()==typeRight) {
        v0.setX(v1[0]+marklen/2.);
        v1.setY(v0[1]-hmetric/2.);
        v0.setY(v0[1]+hmetric/2.);
        v1.setX(v0[0]+(double)ticstr.length()*wmetric);  
      }
      else {
        v0.setXY(v1[0]-marklen/2.-(double)ticstr.length()*wmetric,v0[1]+hmetric/2.);
        v1.setXY(v1[0],v1[1]-hmetric/2.);
      }
      gc.m_r.set(v0,v1);
      gc.m_fnt=ax->m_fnt[ticsz];
      osix::xxdrawText(disp,&m_pixscr,&gc,&ticstr,0);
    }
  }

  osix::xxpostdraw(disp,&m_pixscr,&gc);
  return redoScr;

}

int DiagramXY::drawGraphs() {

  //printf ("%d drawGraphs\n",__LINE__);

  osix::xxRect rdest(m_pixplot.m_r),rsrc(.0,.0,rdest.size());
  rdest.setLeft(rdest.left()+1.);
  void *disp=findDisplay();
  osix::xxGC gc;
  gc.m_r=rsrc;
  gc.m_bg=m_bg;
  osix::xxpredraw(disp,&m_pixplot,&gc);
  osix::xxfillRect(disp,&m_pixplot,&gc);
  
  aux::TPList<Graph> grL;
  int ii=0,jj=0,kk=0,cntp=0,cantpaint=0,ngr=typegraphs("graphxy",&grL);
  if (ngr==0) {
    osix::xxpostdraw(disp,&m_pixplot,&gc);
    osix::xxcopyArea(disp,&m_pixscr,&m_pixplot,&gc,rdest,rsrc); 
    return 0;
  }

  mk_string str;
  Graph *graph=0;
  aux::TVList<num::VertexList> *iL=0;
  num::VertexList *vL=0;
  num::VertexList mL;
  shapes::Shape2 *mshape=0;
  num::Vector3 pL[maxdatacnt];
  memset(&pL[0],0,maxdatacnt*sizeof(num::Vector3));
  for (ii=0;ii<ngr;ii++) {
    graph=grL[ii];
    gc.m_fg=graph->m_linestyle;
    iL=&(graph->m_scDataI);
    for (jj=0;jj<iL->count();jj++) {
      vL=iL->at(jj);
      cntp=vL->count();
      cantpaint=(cntp==0 ? 1 : 0);
      for (kk=0;kk<cntp;kk++) {
        if (vL->get(kk).busted(3)>0) {
          cantpaint=1;
          break;
        }
        pL[kk].setXY(vL->get(kk).x(),rsrc.size().height()-vL->get(kk).y()-1.);
      }
      if (cantpaint==0) {
        osix::xxdrawLines(disp,&m_pixplot,&gc,pL,kk,0);
      }  
    }
    for (jj=0;jj<=graph->m_scMark.last();jj++) {
      mshape=graph->m_scMark.at(jj); 
      if (!mshape || (mshape->m_styleF.m_style==0 && 
          (mshape->m_styleO.m_width==.0 || mshape->m_styleO.m_style==0))) 
        continue;
      mshape->eval(&mL); 
      cntp=mL.count();
      if (cntp>0) {
        for (kk=0;kk<cntp;kk++)
          pL[kk].setXY(mL.get(kk).x(),rsrc.size().height()-mL.get(kk).y()-1.);
        gc.m_fg=mshape->m_styleO;
        gc.m_bg=mshape->m_styleF;
        osix::xxfillPolygon(disp,&m_pixplot,&gc,pL,cntp,0);
      }
    }
  }
  
  osix::xxpostdraw(disp,&m_pixplot,&gc);
  osix::xxcopyArea(disp,&m_pixscr,&m_pixplot,&gc,rdest,rsrc); 
  return redoScr;

}

int DiagramXY::redrawGraphs(void *disp) {

  osix::xxRect rdest(m_pixplot.m_r),rsrc(.0,.0,rdest.size());
  rdest.setLeft(rdest.left()+1.);
  osix::xxGC gc;
  gc.m_r=rsrc;
  osix::xxcopyArea(disp,&m_pixscr,&m_pixplot,&gc,rdest,rsrc);
  return 0;

}

osix::xxRectSize DiagramXY::setup(osix::xxRectSize sz,osix::xxRectSize *excess) {

  if (m_corps.count()==0)
    return sz;
  aux::TypeId xax(typeXaxis),yax(typeYaxis),gr(typeGraphXY);
  aux::TVArr<int> xidxL,yidxL,gridxL;
  m_corps.cmp=aux::cmpType;
  int ii=0,nx=m_corps.findAll(&xax,&xidxL),
          ny=m_corps.findAll(&yax,&yidxL),
          ngr=m_corps.findAll(&gr,&gridxL);
  m_corps.cmp=0;
  Axis *ax=0;
  Graph *graph=0;
  double hexcess=.0,wexcess=.0,tmp=.0;
  for (ii=0;ii<nx;ii++) {  
    ax=dynamic_cast<Axis*>(m_corps[xidxL[ii]]); 
    if (ax)
      tmp=ax->needExcess(sz);
    if (tmp>wexcess)
      wexcess=tmp;
  }
  wexcess=mk_round2(wexcess);
  wexcess+=(double)((int)wexcess%2);
  sz.setWidth(sz.width()-wexcess);
  for (ii=0;ii<ny;ii++) {
    ax=dynamic_cast<Axis*>(m_corps[yidxL[ii]]); 
    if (ax)
      tmp=ax->needExcess(sz);
    if (tmp>hexcess)
      hexcess=tmp;
  }
  hexcess=mk_round2(hexcess);
  hexcess+=(double)((int)hexcess%2);
  if (excess)
    *excess=osix::xxRectSize(wexcess,hexcess);
  sz.setHeight(sz.height()-hexcess);
  for (ii=0;ii<nx;ii++) {  
    ax=dynamic_cast<Axis*>(m_corps[xidxL[ii]]); 
    if (ax)
      sz.setHeight(sz.height()-ax->needSize(sz));
  }
  for (ii=0;ii<ny;ii++) {
    ax=dynamic_cast<Axis*>(m_corps[yidxL[ii]]); 
    if (ax)
      sz.setWidth(sz.width()-ax->needSize(sz));
  }
  for (ii=0;ii<nx;ii++) {  
    ax=dynamic_cast<Axis*>(m_corps[xidxL[ii]]); 
    if (ax)
      ax->resize(sz);
  }
  for (ii=0;ii<ny;ii++) {
    ax=dynamic_cast<Axis*>(m_corps[yidxL[ii]]); 
    if (ax)
      ax->resize(sz);
  }
  for (ii=0;ii<ngr;ii++) {
    graph=dynamic_cast<Graph*>(m_corps[gridxL[ii]]);
    if (graph)
      graph->rescale();
  }
    
  return sz;
  
}

int DiagramXY::mousePressed (osix::xxEvent *xxev) {

  if (m_mouseMode==0)
    return 0;
  bool inplotarea=(m_pixplot.m_r.left()+1.<xxev->m_pos.x() && 
                   xxev->m_pos.x()<m_pixplot.m_r.right()-1. &&
                   m_pixplot.m_r.top()+1.<xxev->m_pos.y() && 
                   xxev->m_pos.y()<m_pixplot.m_r.bottom()-1.);
  if (inplotarea) {
    if ((m_mouseMode&mouseModify)>0 && (xxev->m_buttons&osix::xxm_leftButton)>0) {
      if (xxev->m_mods==0) {
        aux::TypeId sel=selectGraph(num::Vector3(xxev->m_pos.x()-m_pixplot.m_r.left(),
                                    m_pixplot.m_r.bottom()-xxev->m_pos.y()));
      }
    }
    xxev->m_consumer|=osix::xx_processed;
  }
  else {

  }

  return 0;

}

int DiagramXY::mouseMoved (osix::xxEvent *xxev) {

  if (m_mouseMode==0)
    return 0;
  bool inplotarea=(m_pixplot.m_r.left()+1.<xxev->m_pos.x() && 
                   xxev->m_pos.x()<m_pixplot.m_r.right()-1. &&
                   m_pixplot.m_r.top()+1.<xxev->m_pos.y() && 
                   xxev->m_pos.y()<m_pixplot.m_r.bottom()-1.);
  //void *disp=findDisplay();
  int redrawflag=0;
  osix::xxGC xxgc;
  osix::xxRect rdest(m_pixplot.m_r),rsrc(.0,.0,rdest.size());
  if (inplotarea) {
    xxev->m_consumer|=osix::xx_processed;
    if (xxev->m_mods==0) {
      redrawflag|=drawCrosshair(xxev);
      redrawflag|=drawZoomRect(xxev);
      if ((m_mouseMode&mouseModify)>0 && (xxev->m_buttons&osix::xxm_leftButton)>0) {
        int selcnt=m_selected.count();
        if (selcnt==0)
          return 0;
        Graph *gg=graph(m_selected.at(0)->m_sel);
        if (gg) {
          num::Vector3 vv(xxev->m_pos.x()-m_pixplot.m_r.left(),
                          m_pixplot.m_r.bottom()-xxev->m_pos.y());
          gg->sz2sc(&vv);
//printf ("v=%f,%f\n",vv.x(),vv.y());
          gg->setValue(m_selected.at(0)->m_idx,&vv);
          gg->rescale();
          redrawflag|=redoDrawGraph;
        }
      }
    }
  }
  else {
    redrawflag|=mouseLeave(xxev);

  }

  return redrawflag;

}

int DiagramXY::mouseReleased (osix::xxEvent *xxev) {

  //printf ("%d mouserelease %d\n",__LINE__,m_mouseMode);

  if (m_mouseMode==0)
    return 0;
  bool inplotarea=(m_pixplot.m_r.left()+1.<xxev->m_pos.x() && 
                   xxev->m_pos.x()<m_pixplot.m_r.right()-1. &&
                   m_pixplot.m_r.top()+1.<xxev->m_pos.y() && 
                   xxev->m_pos.y()<m_pixplot.m_r.bottom()-1.);
  int redrawflag=0,ii=0,jj=0,nxax=0,nyax=0,idx=-1,selcnt=m_selected.count(),
      boundsopt=(typeBoundStaticMin|typeBoundStaticMax);
  if (inplotarea) {
    xxev->m_consumer|=osix::xx_processed;
    aux::TPList<Axis> xaxL,yaxL;
    Axis *ax=0;
    num::Vector3 vv(xxev->m_pos.x()-m_pixplot.m_r.left(),
                    m_pixplot.m_r.bottom()-xxev->m_pos.y()),
                 vdown(xxev->m_downpos.x()-m_pixplot.m_r.left(),
                       m_pixplot.m_r.bottom()-xxev->m_downpos.y()),
                 vvv=vv,vvdown=vdown;
//aux::Aciistr numstr;
    Graph *gg=(selcnt>0 ? graph(m_selected.at(0)->m_sel) : 0);
    if ((m_mouseMode&mouseModify)>0 && (xxev->m_buttons&osix::xxm_leftButton)>0) {
      if ((xxev->m_mods&osix::xx_modAlt)>0) {
        if (!gg)
          gg=graph(selectGraph(vvv,false));
//printf ("mode=%d state=%d mods=%d cnt=%d g=%d\n",m_mouseMode,state,(int)mods,m_selected.count(),(long)g);
        if (!gg) {
          if ((xxev->m_mods&osix::xx_modShift)==0) {

          }
        }
        else {
          if ((xxev->m_mods&osix::xx_modShift)>0) {
            idx=gg->match(vvv);
            if (idx>=0) {
              idx=gg->setValue(idx,0);
              if (idx>=0) {
                gg->rescale();
                redrawflag|=redoDrawGraph;
              }
            }
          }
          else {
            gg->sz2sc(&vvv);
//printf ("v=%f,%f\n",vv.x(),vv.y());
            idx=gg->setValue(-1,&vvv);
            if (idx>=0) {
              gg->rescale();
              redrawflag|=redoDrawGraph;
            }
          }
        }
      }
      else {

      }
    }
    else if((m_mouseMode&mouseZoom)>0) {
      if ((xxev->m_buttons&osix::xxm_leftButton)>0 &&
          (xxev->m_buttons&(~osix::xxm_leftButton))==0) {
        nxax=typeaxes("xaxis",&xaxL);
        nyax=typeaxes("yaxis",&yaxL);
        for (ii=0;ii<nxax;ii++) {
          ax=xaxL[ii];
          if (!ax->scale())
            continue;
          if ((xxev->m_mods&osix::xx_modShift)>0) {
            ax->scale()->unstackRange(1);
            jj++; 
          } 
          else if (xxev->m_mods==0) { 
            if (vdown.busted(3)==0) {
              vvv=vv;
              vvdown=vdown;
              ax->sz2sc(&vvv);
              ax->sz2sc(&vvdown);
              ax->scale()->stackRange(vvdown[0],vvv[0],boundsopt);
              jj++; 
            }
          }
        }  
        for (ii=0;ii<nyax;ii++) {
          ax=yaxL[ii];
          if (!ax->scale())
            continue;
          if ((xxev->m_mods&osix::xx_modShift)>0) { 
            ax->scale()->unstackRange(1);
            jj++; 
          }
          else if (xxev->m_mods==0) { 
            vvv=vv;
            vvdown=vdown;
            ax->sz2sc(&vvv);
            ax->sz2sc(&vvdown);
            ax->scale()->stackRange(vvdown[1],vvv[1],boundsopt);
            jj++; 
          }
        }
        if (jj>0)
          redrawflag|=(redoScaleRect|redoDrawAxes|redoDrawGraph|redoScr);
      }
      else if ((xxev->m_buttons&osix::xxm_rightButton)>0 &&
               (xxev->m_buttons&(~osix::xxm_rightButton))==0) {

      }
    }
  }
  else {

  }

  return redrawflag;

}

int DiagramXY::mouseEnter(osix::xxEvent *) {

  return 0;

}

int DiagramXY::mouseLeave(osix::xxEvent *) {

  if ((m_mouseMode&mouseCrosshair)>0) {
    osix::xxRect rdest(m_pixplot.m_r),rsrc(.0,.0,rdest.size());
    rdest.setLeft(rdest.left()+1.);
    void *disp=findDisplay();
    osix::xxGC gc;
    gc.m_r=rsrc;
    osix::xxcopyArea(disp,&m_pixscr,&m_pixplot,&gc,rdest,rsrc);
    return redoScr;
  }
  return 0;

}

int DiagramXY::drawCrosshair(osix::xxEvent *xxev) {

  if ((m_mouseMode&mouseCrosshair)==0) 
    return 0;
  
  num::Vector3 pv0(xxev->m_pos.x(),m_pixplot.m_r.top()+1.),
               pv1(xxev->m_pos.x(),m_pixplot.m_r.bottom()-1.),
               ph0(m_pixplot.m_r.left()+1.,xxev->m_pos.y()),
               ph1(m_pixplot.m_r.right()-1.,xxev->m_pos.y());
  osix::xxGC gc;
  void *disp=findDisplay();
  redrawGraphs(disp);
  osix::xxpredraw(disp,&m_pixscr,&gc);
  gc.m_fg=osix::xxStyle(osix::xx_somecolors[osix::grey128],2,1);
  gc.m_r.set(pv0,pv1);
  osix::xxdrawLine(disp,&m_pixscr,&gc);
  gc.m_r.set(ph0,ph1);
  osix::xxdrawLine(disp,&m_pixscr,&gc);
  osix::xxpostdraw(disp,&m_pixscr,&gc);
  return redoScr;
  
}
    
int DiagramXY::drawZoomRect(osix::xxEvent *xxev) {

  if ((m_mouseMode&mouseZoom)==0 || (xxev->m_buttons&osix::xxm_leftButton)==0)
    return 0;
  void *disp=findDisplay();
  redrawGraphs(disp);
  osix::xxGC gc;
  gc.m_r.set(xxev->m_downpos,xxev->m_pos);
  osix::xxpredraw(disp,&m_pixscr,&gc);
  gc.m_fg=osix::xxStyle(osix::xx_somecolors[osix::grey128],2,1);
  osix::xxdrawRect(disp,&m_pixscr,&gc);
  osix::xxpostdraw(disp,&m_pixscr,&gc);
  return redoScr;

}

} // namespace simpleplot
