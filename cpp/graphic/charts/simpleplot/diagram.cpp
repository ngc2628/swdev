
#include <graphic/charts/simpleplot/diagram.h>
#include <graphic/charts/simpleplot/scale.h>
#include <osix/xxpaint.h>

namespace simpleplot {

DiagramSelection::DiagramSelection(mk::TypeId sel,int idx) :
  m_sel(sel),m_idx(idx) {

  mk_vertexset(m_val,mk_dnan);
    
}

    
DiagramSelection::DiagramSelection(const DiagramSelection &ass) : 
  m_sel(ass.m_sel),m_idx(ass.m_idx) {

  mk_vertexcopy(m_val,ass.m_val);
    
}
    
DiagramSelection &DiagramSelection::operator=(const DiagramSelection &ass) { 
      
  m_sel=ass.m_sel;
  m_idx=ass.m_idx;
  mk_vertexcopy(m_val,ass.m_val);
  return *this;
  
}
    
void DiagramSelection::clear(mk::TypeId sel,int idx) {

  m_sel=sel;
  m_idx=idx;
  mk_vertexset(m_val,mk_dnan);

}

int cmpSel(const void *s1,const void *s2) {

  if ((((const DiagramSelection *)s2)<((const DiagramSelection *)s1))>0)
    return 1;
  else if ((((const DiagramSelection *)s1)<((const DiagramSelection *)s2))>0)
    return -1;
  return 0;

}

Diagram::Diagram(const char *type,unsigned int idd) : 
  mk::TypeId(type,idd),m_mouseMode(0) {

  mk_listalloc(&m_selected,sizeof(DiagramSelection),65);
  m_selected.cmp=cmpSel;
  mk_listalloc(&m_entityL,sizeof(mk::TypeId *),128);
  m_entityL.cmp=mk::cmpTypeIdRef;
    	  
}

Diagram::~Diagram() {

  mk_listfree(&m_selected);
  mk::TypeId *tid=0;
  while (m_entityL.count>0) {
    mk_listremove(&m_entityL,m_entityL.count-1,(void *)&tid);
    delete tid;
  }
  mk_listfree(&m_entityL);

}

Graph *Diagram::graph(mk::TypeId grid_) {

  mk::TypeId grid(grid_);
  mk::TypeId *res=0;
  int idxl=-1;
  m_entityL.cmp=mk::cmpTypeIdRefi;
  mk_listfind(&m_entityL,(void*)&grid,&idxl,0);
  m_entityL.cmp=mk::cmpTypeIdRef;
  if (idxl>=0)
    mk_listat(&m_entityL,idxl,(void*)&res);
  return (Graph*)res;
    
}

Axis *Diagram::axis(mk::TypeId axid_) {

  mk::TypeId axid(axid_);
  mk::TypeId *res=0;
  int idxl=-1;
  m_entityL.cmp=mk::cmpTypeIdRefi;
  mk_listfind(&m_entityL,(void*)&axid,&idxl,0);
  m_entityL.cmp=mk::cmpTypeIdRef;
  if (idxl>=0)
    mk_listat(&m_entityL,idxl,(void*)&res);
  return (Axis*)res;

}

mk::TypeId Diagram::setGraph(Graph *graph) {

  mk::TypeId res;
  int idxl=-1;
  mk_listfind(&m_entityL,(void*)&graph,&idxl,0);
  if (!graph || idxl>=0)
    return res;
  idxl=mk_listinsort(&m_entityL,(void*)&graph);  
  if (idxl>=0)
    res=*((mk::TypeId*)graph);
  return res;
  
}

mk::TypeId Diagram::setAxis(Axis *axis) {

  mk::TypeId res;
  int idxl=-1;
  mk_listfind(&m_entityL,(void*)&axis,&idxl,0);
  if (!axis || idxl>=0)
    return res;
  idxl=mk_listinsort(&m_entityL,(void*)&axis);  
  if (idxl>=0)
    res=*((mk::TypeId*)axis);
  return res;

}

Graph *Diagram::removeGraph(mk::TypeId grid_) {

  mk::TypeId grid(grid_);
  mk::TypeId *res=0;
  int idxl=-1;
  m_entityL.cmp=mk::cmpTypeIdRefi;
  mk_listfind(&m_entityL,(void*)&grid,&idxl,0);
  m_entityL.cmp=mk::cmpTypeIdRef;
  if (idxl>=0)
    mk_listremove(&m_entityL,idxl,(void*)&res);
  return (Graph*)res;
    
}

Axis *Diagram::removeAxis(mk::TypeId axid_) {

  mk::TypeId axid(axid_);
  mk::TypeId *res=0;
  int idxl=-1;
  m_entityL.cmp=mk::cmpTypeIdRefi;
  mk_listfind(&m_entityL,(void*)&axid,&idxl,0);
  m_entityL.cmp=mk::cmpTypeIdRef;
  if (idxl>=0)
    mk_listremove(&m_entityL,idxl,(void*)&res);
  return (Axis*)res;

}

int Diagram::entities(mk_list *entityL,const char *type_) {

  int ii=0,idxl=-1,idxh=-1,cnt=m_entityL.count;
  if (!entityL)
    return cnt;
  mk::TypeId type(type_);
  mk_listrealloc(entityL,cnt);
  if (type_ && strlen(type_)>0) {
    m_entityL.cmp=mk::cmpTypeIdRefiType;
    mk_listfind(&m_entityL,(void*)&type,&idxl,&idxh);
    m_entityL.cmp=mk::cmpTypeIdRef;
  }
  else {
    idxl=0;
    idxh=cnt-1;
  }  
  mk::TypeId *tid=0;
  mk_string str;
  for (ii=MAX(0,idxl);ii<=idxh;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    tid->toString(str);
    type.fromString(str);
    mk_listappend(entityL,(void*)&type);
  }
  entityL->cmp=mk::cmpTypeId;
  mk_listsort(entityL);
  return entityL->count;

}

int Diagram::clear(bool destr) {

  int cnt=m_entityL.count;
  mk::TypeId *tid=0;
  while (m_entityL.count>0) {
    mk_listremove(&m_entityL,m_entityL.count-1,(void *)&tid);
    if (destr)
      delete tid;
  }
  return cnt;

}

int Diagram::selected(mk_list *selL) const {

  int ii=0,cnt=m_selected.count;
  if (!selL)
    return cnt;
  DiagramSelection sel;
  mk_listclear(selL,0);
  for (ii=0;ii<cnt;ii++) {
    mk_listat(&m_selected,ii,(void *)&sel);
    mk_listinsort(selL,(void *)&sel);
  }
  return cnt;

}

int Diagram::setSelection(DiagramSelection sel,int) {

  return -1;

}

mk::TypeId Diagram::selectGraph(mk_vertex,int,int) {

  return mk::TypeId();
    
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
    mk_vertexcopy(xxev->m_lastpos,m_xxlastinputev.m_pos);
    if (xxev->m_type==osix::xx_mousePressed)
      res|=mousePressed(xxev);
    else if (xxev->m_type==osix::xx_mouseMove) {
      mk_vertexcopy(xxev->m_downpos,m_xxlastinputev.m_downpos);
      res|=mouseMoved(xxev);   
    }
    else if (xxev->m_type==osix::xx_mouseReleased) {
      mk_vertexcopy(xxev->m_downpos,m_xxlastinputev.m_downpos);
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

  if (m_entityL.reserved<sz)
    mk_listrealloc(&m_entityL,sz);  
  	  
} 

int DiagramXY::setSelection(DiagramSelection sel,int add) {

  int idx=-1;
  if ((add&1)==0)
    mk_listclear(&m_selected,0);
  else
    mk_listfind(&m_selected,(void *)&sel,&idx,0);
  if (idx<0 && !sel.m_sel.busted())
    idx=mk_listinsort(&m_selected,(void *)&sel);
  return idx;

}

mk::TypeId DiagramXY::selectGraph(mk_vertex pp,int set,int add) {

  DiagramSelection sel;
  mk::TypeId grxy("graphxy");
  m_entityL.cmp=mk::cmpTypeIdRefiType;
  int ii=0,idx=-1,idxl=-1,idxh=-1;
  mk_listfind(&m_entityL,(void*)&grxy,&idxl,&idxh);
  m_entityL.cmp=mk::cmpTypeIdRef;
  mk::TypeId *tid=0;
  Graph *graph=0;
  for (ii=MAX(0,idxl);ii<=idxh;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    graph=(Graph*)tid;
    idx=(graph ? graph->match(pp) : -1);
    if (idx>=0) {
      sel.clear(*((mk::TypeId*)graph),idx);
      graph->value(idx,sel.m_val);
      break;
    }
  }
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
  
  int ii=0,idxl=-1,idxh=-1;
  Xaxis *xax=0;
  Yaxis *yax=0;
  mk::TypeId *tid=0;
  mk::TypeId xaxt(typeXaxis),yaxt(typeYaxis);
  m_entityL.cmp=mk::cmpTypeIdRefiType;
  mk_listfind(&m_entityL,(void*)&xaxt,&idxl,&idxh);
  m_entityL.cmp=mk::cmpTypeIdRef;
  double xl=baserect.left(),yt=baserect.top(),xr=baserect.right(),yb=baserect.bottom();
  for (ii=MAX(0,idxl);ii<=idxh;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    xax=(Xaxis*)tid;
    if (xax->pos()==typeTop)
      yt+=xax->size().height();
    else if (xax->pos()==typeBottom)
      yb-=xax->size().height();
  }
  m_entityL.cmp=mk::cmpTypeIdRefiType;
  mk_listfind(&m_entityL,(void*)&yaxt,&idxl,&idxh);
  m_entityL.cmp=mk::cmpTypeIdRef;
  for (ii=MAX(0,idxl);ii<=idxh;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    yax=(Yaxis*)tid;
    if (yax->pos()==typeLeft)
      xl+=yax->size().width();
    else if (yax->pos()==typeRight)
      xr-=yax->size().width();
  }
  osix::xxGC gc;
  gc.m_bg=m_bg;
  gc.m_r=osix::xxRect(xl,yt,xr,yb);
  osix::xxresizeDrawable(findDisplay(),&m_pixplot,&gc);
  
  return rescaleGraph();

}

int DiagramXY::rescaleGraph() {

  mk::TypeId grt(typeGraphXY);
  mk::TypeId *tid=0;
  GraphXY *grxy=0;
  int ii=0,idxl=-1,idxh=-1;
  m_entityL.cmp=mk::cmpTypeIdRefiType;
  mk_listfind(&m_entityL,(void*)&grt,&idxl,&idxh);
  m_entityL.cmp=mk::cmpTypeIdRef;
  for (ii=MAX(0,idxl);ii<=idxh;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    grxy=(GraphXY*)tid;
    grxy->rescale();
  }
  return (idxh>idxl ? redoDrawGraph : 0);

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

  int ii=0,jj=0,idxlx=-1,idxhx=-1,idxly=-1,idxhy=-1;
  Xaxis *xax=0;
  Yaxis *yax=0;
  mk::TypeId *tid=0;
  mk::TypeId xaxt(typeXaxis),yaxt(typeYaxis);
  m_entityL.cmp=mk::cmpTypeIdRefiType;
  mk_listfind(&m_entityL,(void*)&xaxt,&idxlx,&idxhx);
  mk_listfind(&m_entityL,(void*)&yaxt,&idxly,&idxhy);
  m_entityL.cmp=mk::cmpTypeIdRef;
  if (idxlx<0 && idxly<0) {
    osix::xxpostdraw(disp,&m_pixscr,&gc);
    return 0;
  }
//printf ("%d nxax=%d nyax=%d\n",__LINE__,nxax,nyax);
  double basex0=m_pixplot.m_r.left(),basex1=m_pixplot.m_r.right(),
         basey0=m_pixplot.m_r.bottom(),basey1=m_pixplot.m_r.top(),baseyy0=basey0,baseyy1=basey1,
         wmetric=.0,hmetric=.0;
  Scale *sc=0;
  Tic tic;
  mk::Ucsstr ticstr;
  mk_list ticL;
  mk_listalloc(&ticL,sizeof(Tic),0);
  int ntics=0,ticsz=0;
  mk_vertexnan(v0);
  mk_vertexnan(v1);
  double basepos=.0,marklen=.0;
  for (ii=MAX(0,idxlx);ii<=idxhx;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    xax=(Xaxis*)tid;
    gc.m_fg=xax->m_style;
    if (xax->pos()==typeTop) {
      basepos=baseyy1-1;
      baseyy1-=xax->size().height();
    }
    else {
      basepos=baseyy0;
      baseyy0+=xax->size().height();
    }
    gc.m_r.set(basex0,basepos,basex1,basepos);
    osix::xxdrawLine(disp,&m_pixscr,&gc);
    sc=xax->scale();
    if (!sc)
      continue;
    ntics=sc->tics(&ticL);
    for (jj=0;jj<ntics;jj++) {
      mk_listat(&ticL,jj,(void *)&tic);
      ticsz=tic.m_size;
      wmetric=xax->m_fnt[ticsz].m_metric.width();
      hmetric=xax->m_fnt[ticsz].m_metric.height();
      marklen=hmetric/3.;
      if (marklen<3.)
        marklen=3.;
      v0[0]=tic.m_val;
      v0[1]=basepos;
      xax->sc2sz(v0);
      v0[0]=(v0[0]+basex0);
      mk_vertexcopy(v1,v0);
      v1[1]=(v1[1]+(xax->pos()==typeTop ? -marklen : marklen));
      gc.m_r.set(v0,v1);
      osix::xxdrawLine(disp,&m_pixscr,&gc);
      if (tic.m_drawable==0)
        continue;
      tic.ucsText(&ticstr);
      v0[0]=(v0[0]-(double)ticstr.length()*wmetric/2.);
      v0[1]=(v0[1]+(xax->pos()==typeTop ? -marklen-hmetric : marklen+hmetric));
      v1[0]=(v1[0]+(double)ticstr.length()*wmetric/2.);
      gc.m_r.set(v0,v1);
      gc.m_fnt=xax->m_fnt[ticsz];
      osix::xxdrawText(disp,&m_pixscr,&gc,&ticstr,0);
    }
  }

  for (ii=MAX(0,idxly);ii<=idxhy;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    yax=(Yaxis*)tid;
    if (yax->pos()==typeRight) {
      basepos=basex1;
      basex1+=yax->size().width();
    }
    else {
      basepos=basex0;
      basex0-=yax->size().width();
    }
    gc.m_r.set(basepos,basey0,basepos,basey1);
    osix::xxdrawLine(disp,&m_pixscr,&gc);
    sc=yax->scale();
    if (!sc)
      continue;
    ntics=sc->tics(&ticL);
    for (jj=0;jj<ntics;jj++) {
      mk_listat(&ticL,jj,(void *)&tic);
      ticsz=tic.m_size;
      wmetric=yax->m_fnt[ticsz].m_metric.width();
      hmetric=yax->m_fnt[ticsz].m_metric.height();
      marklen=hmetric/3.;
      if (marklen<3.)
        marklen=3.;
      v0[0]=basepos;
      v0[1]=tic.m_val;
      yax->sc2sz(v0);
      v0[1]=(basey0-v0[1]);
      mk_vertexcopy(v1,v0);
      v1[0]=(v1[0]+(yax->pos()==typeRight ? marklen : -marklen));
      gc.m_r.set(v0,v1);
      osix::xxdrawLine(disp,&m_pixscr,&gc);
      if (tic.m_drawable==0)
        continue;
      tic.ucsText(&ticstr);
      if (yax->pos()==typeRight) {
        v0[0]=(v1[0]+marklen/2.);
        v1[1]=(v0[1]-hmetric/2.);
        v0[1]=(v0[1]+hmetric/2.);
        v1[0]=(v0[0]+(double)ticstr.length()*wmetric);  
      }
      else {
        v0[0]=(v1[0]-marklen/2.-(double)ticstr.length()*wmetric);
        v0[1]=(v0[1]+hmetric/2.);
        v1[1]=(v1[1]-hmetric/2.);
      }
      gc.m_r.set(v0,v1);
      gc.m_fnt=yax->m_fnt[ticsz];
      osix::xxdrawText(disp,&m_pixscr,&gc,&ticstr,0);
    }
  }
  osix::xxpostdraw(disp,&m_pixscr,&gc);
  mk_listfree(&ticL);
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
  
  int ii=0,jj=0,kk=0,cntp=0,cantpaint=0,idxl=-1,idxh=-1;
  mk::TypeId grt(typeGraphXY);
  m_entityL.cmp=mk::cmpTypeIdRefiType;
  mk_listfind(&m_entityL,(void*)&grt,&idxl,&idxh);
  m_entityL.cmp=mk::cmpTypeIdRef;
  if (idxl<0) {
    osix::xxpostdraw(disp,&m_pixplot,&gc);
    osix::xxcopyArea(disp,&m_pixscr,&m_pixplot,&gc,rdest,rsrc); 
    return 0;
  }
  mk_string str;
  mk_stringset(str,0);
  mk::TypeId *tid=0;
  GraphXY *graph=0;
  mk_list *iL=0;
  mk_list *vL=0;
  mk_list mL;
  mk_listalloc(&mL,sizeof(mk_vertex),0);
  shapes::Shape2 *mshape=0;
  mk_vertex pL[maxdatacnt];
  mk_vertexnan(vv);
  memset(&pL[0],0,maxdatacnt*sizeof(mk_vertex));
  for (ii=MAX(0,idxl);ii<=idxh;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    graph=(GraphXY*)tid;
    gc.m_fg=graph->m_linestyle;
    iL=&(graph->m_scDataI);
    for (jj=0;jj<iL->count;jj++) {
      mk_listat(iL,jj,(void*)&vL);
      cntp=vL->count;
      cantpaint=(cntp==0 ? 1 : 0);
      for (kk=0;kk<cntp;kk++) {
        mk_listat(vL,kk,(void*)&vv);
        if (mk_isbusted(vv[0])>0 || mk_isbusted(vv[1])>0) {
          cantpaint=1;
          break;
        }
        pL[kk][0]=vv[0];
        pL[kk][1]=(rsrc.size().height()-vv[1]-1.);
      }
      if (cantpaint==0) {
        osix::xxdrawLines(disp,&m_pixplot,&gc,pL,kk,0);
      }  
    }
    for (jj=0;jj<graph->m_scMark.count;jj++) {
      mk_listat(&graph->m_scMark,jj,(void*)&mshape);
      if (mshape->getStyleF().m_style==0 && 
          (mshape->getStyleO().m_width==.0 || mshape->getStyleO().m_style==0)) 
        continue;
      mshape->eval(&mL); 
      cntp=mL.count;
      if (cntp>0) {
        for (kk=0;kk<cntp;kk++) {
          mk_listat(&mL,kk,(void*)&vv);
          pL[kk][0]=vv[0];
          pL[kk][1]=rsrc.size().height()-vv[1]-1.;
        }
        gc.m_fg=mshape->getStyleO();
        gc.m_bg=mshape->getStyleF();
        osix::xxfillPolygon(disp,&m_pixplot,&gc,pL,cntp,0);
      }
    }
  }
  
  osix::xxpostdraw(disp,&m_pixplot,&gc);
  osix::xxcopyArea(disp,&m_pixscr,&m_pixplot,&gc,rdest,rsrc); 
  mk_listfree(&mL);
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

  if (m_entityL.count==0)
    return sz;
  mk::TypeId xaxt(typeXaxis),yaxt(typeYaxis),grt(typeGraphXY);
  int ii=0,idxlx=-1,idxhx=-1,idxly=-1,idxhy=-1,idxlg=-1,idxhg=-1;
  m_entityL.cmp=mk::cmpTypeIdRefiType;
  mk_listfind(&m_entityL,(void*)&xaxt,&idxlx,&idxhx);
  mk_listfind(&m_entityL,(void*)&yaxt,&idxly,&idxhy);
  mk_listfind(&m_entityL,(void*)&grt,&idxlg,&idxhg);
  m_entityL.cmp=mk::cmpTypeIdRef;
  Xaxis *xax=0;
  Yaxis *yax=0;
  GraphXY *graph=0;
  mk::TypeId *tid=0;
  double hexcess=.0,wexcess=.0,tmp=.0;
  for (ii=idxlx;ii<=idxhx;ii++) {  
    mk_listat(&m_entityL,ii,(void*)&tid);
    xax=(Xaxis*)tid; 
    tmp=xax->needExcess(sz);
    if (tmp>wexcess)
      wexcess=tmp;
  }
  wexcess=mk_round2(wexcess);
  wexcess+=(double)((int)wexcess%2);
  sz.setWidth(sz.width()-wexcess);
  for (ii=idxly;ii<=idxhy;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    yax=(Yaxis*)tid; 
    tmp=yax->needExcess(sz);
    if (tmp>hexcess)
      hexcess=tmp;
  }
  hexcess=mk_round2(hexcess);
  hexcess+=(double)((int)hexcess%2);
  if (excess)
    *excess=osix::xxRectSize(wexcess,hexcess);
  sz.setHeight(sz.height()-hexcess);

  for (ii=idxlx;ii<=idxhx;ii++) {  
    mk_listat(&m_entityL,ii,(void*)&tid);
    xax=(Xaxis*)tid;
    sz.setHeight(sz.height()-xax->needSize(sz));
  }
  for (ii=idxly;ii<=idxhy;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    yax=(Yaxis*)tid; 
    sz.setWidth(sz.width()-yax->needSize(sz));
  }
  for (ii=idxlx;ii<=idxhx;ii++) {  
    mk_listat(&m_entityL,ii,(void*)&tid);
    xax=(Xaxis*)tid;
    xax->resize(sz);
  }
  for (ii=idxly;ii<=idxhy;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    yax=(Yaxis*)tid; 
    yax->resize(sz);
  }
  for (ii=idxlg;ii<=idxhg;ii++) {
    mk_listat(&m_entityL,ii,(void*)&tid);
    graph=(GraphXY*)tid; 
    graph->rescale();
  }
  return sz;
  
}

int DiagramXY::mousePressed (osix::xxEvent *xxev) {

  if (m_mouseMode==0)
    return 0;
  bool inplotarea=(m_pixplot.m_r.left()+1.<xxev->m_pos[0] && 
                   xxev->m_pos[0]<m_pixplot.m_r.right()-1. &&
                   m_pixplot.m_r.top()+1.<xxev->m_pos[1] && 
                   xxev->m_pos[1]<m_pixplot.m_r.bottom()-1.);
  if (inplotarea) {
    if ((m_mouseMode&mouseModify)>0 && (xxev->m_buttons&osix::xxm_leftButton)>0) {
      if (xxev->m_mods==0) {
        mk_vertex vv={xxev->m_pos[0]-m_pixplot.m_r.left(),
                      m_pixplot.m_r.bottom()-xxev->m_pos[1],mk_dnan,1.};
        mk::TypeId sel=selectGraph(vv);
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
  bool inplotarea=(m_pixplot.m_r.left()+1.<xxev->m_pos[0] && 
                   xxev->m_pos[0]<m_pixplot.m_r.right()-1. &&
                   m_pixplot.m_r.top()+1.<xxev->m_pos[1] && 
                   xxev->m_pos[1]<m_pixplot.m_r.bottom()-1.);
  //void *disp=findDisplay();
  int redrawflag=0;
  osix::xxGC xxgc;
  osix::xxRect rdest(m_pixplot.m_r),rsrc(.0,.0,rdest.size());
  DiagramSelection sel;
  if (inplotarea) {
    xxev->m_consumer|=osix::xx_processed;
    if (xxev->m_mods==0) {
      redrawflag|=drawCrosshair(xxev);
      redrawflag|=drawZoomRect(xxev);
      if ((m_mouseMode&mouseModify)>0 && (xxev->m_buttons&osix::xxm_leftButton)>0) {
        int selcnt=m_selected.count;
        if (selcnt==0)
          return 0;
        mk_listat(&m_selected,0,(void *)&sel);
        Graph *gg=graph(sel.m_sel);
        if (gg) {
          mk_vertex vv={xxev->m_pos[0]-m_pixplot.m_r.left(),
                        m_pixplot.m_r.bottom()-xxev->m_pos[1],mk_dnan,1.};
          gg->sz2sc(vv);
          gg->setValue(sel.m_idx,vv);
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
  bool inplotarea=(m_pixplot.m_r.left()+1.<xxev->m_pos[0] && 
                   xxev->m_pos[0]<m_pixplot.m_r.right()-1. &&
                   m_pixplot.m_r.top()+1.<xxev->m_pos[1] && 
                   xxev->m_pos[1]<m_pixplot.m_r.bottom()-1.);
  int redrawflag=0,ii=0,jj=0,idx=-1,idxlx=-1,idxhx=-1,idxly=-1,idxhy=-1,
    selcnt=m_selected.count,boundsopt=(typeBoundStaticMin|typeBoundStaticMax);
  mk::TypeId xaxt(typeXaxis),yaxt(typeYaxis);
  Xaxis *xax=0;
  Yaxis *yax=0;
  mk::TypeId *tid=0;
  if (inplotarea) {
    xxev->m_consumer|=osix::xx_processed;
    mk_vertex vv={xxev->m_pos[0]-m_pixplot.m_r.left(),
                  m_pixplot.m_r.bottom()-xxev->m_pos[1],mk_dnan,1.};
    mk_vertex vdown={xxev->m_downpos[0]-m_pixplot.m_r.left(),
                     m_pixplot.m_r.bottom()-xxev->m_downpos[1],mk_dnan,1.};
    mk_vertexnan(vvv);
    mk_vertexcopy(vvv,vv);
    mk_vertexnan(vvdown);
    mk_vertexcopy(vvdown,vdown);
    DiagramSelection sel;
    mk_listat(&m_selected,0,(void *)&sel);
    Graph *gg=(selcnt>0 ? graph(sel.m_sel) : 0);
    if ((m_mouseMode&mouseModify)>0 && (xxev->m_buttons&osix::xxm_leftButton)>0) {
      if ((xxev->m_mods&osix::xx_modAlt)>0) {
        if (!gg)
          gg=graph(selectGraph(vvv,false));
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
            gg->sz2sc(vvv);
            idx=gg->setValue(-1,vvv);
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
        m_entityL.cmp=mk::cmpTypeIdRefiType;
        mk_listfind(&m_entityL,(void*)&xaxt,&idxlx,&idxhx);
        mk_listfind(&m_entityL,(void*)&yaxt,&idxly,&idxhy);
        m_entityL.cmp=mk::cmpTypeIdRef;
        for (ii=MAX(idxlx,0);ii<=idxhx;ii++) {
          mk_listat(&m_entityL,ii,(void*)&tid);
          xax=(Xaxis*)tid;
          if (!xax->scale())
            continue;
          if ((xxev->m_mods&osix::xx_modShift)>0) {
            xax->scale()->unstackRange(1);
            jj++; 
          } 
          else if (xxev->m_mods==0) { 
            if (mk_isbusted(vdown[0])==0 && mk_isbusted(vdown[1])==0) {
              mk_vertexcopy(vvv,vv);
              mk_vertexcopy(vvdown,vdown);
              xax->sz2sc(vvv);
              xax->sz2sc(vvdown);
              xax->scale()->stackRange(vvdown[0],vvv[0],boundsopt);
              jj++; 
            }
          }
        }  
        for (ii=MAX(idxly,0);ii<=idxhy;ii++) {
          mk_listat(&m_entityL,ii,(void*)&tid);
          yax=(Yaxis*)tid;
          if (!yax->scale())
            continue;
          if ((xxev->m_mods&osix::xx_modShift)>0) { 
            yax->scale()->unstackRange(1);
            jj++; 
          }
          else if (xxev->m_mods==0) { 
            mk_vertexcopy(vvv,vv);
            mk_vertexcopy(vvdown,vdown);
            yax->sz2sc(vvv);
            yax->sz2sc(vvdown);
            yax->scale()->stackRange(vvdown[1],vvv[1],boundsopt);
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
  
  mk_vertex pv0={xxev->m_pos[0],m_pixplot.m_r.top()+1.,mk_dnan,1.};  
  mk_vertex pv1={xxev->m_pos[0],m_pixplot.m_r.bottom()-1.,mk_dnan,1.}; 
  mk_vertex ph0={m_pixplot.m_r.left()+1.,xxev->m_pos[1],mk_dnan,1.}; 
  mk_vertex ph1={m_pixplot.m_r.right()-1.,xxev->m_pos[1],mk_dnan,1.}; 
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
