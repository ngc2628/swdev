
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

  if (mk_isbusted(size))
    size=.0;
  int idx=markshape2idx(type);
  shapes::Shape2 *ss=0;
  if (idx==1) {
    mk_vertex vv={size,size,.0,1.};
    ss=new shapes::Ellipse();
    
    ss->setVertex(0,vv);
  }
  else if (idx==2) {
    ss=new shapes::Rect(shapes::RectSize(1.414*size,1.414*size));
  }
  else if (idx==3) {
    mk_vertex vv1={.0,size,.0,1.};
    mk_vertex vv2={-.866*size,-.5*size,.0,1.};
    mk_vertex vv3={.866*size,-.5*size,.0,1.};
    ss=new shapes::Triangle();
    ss->setVertex(0,vv1);
    ss->setVertex(1,vv2);
    ss->setVertex(2,vv3);
  }
  else if (idx==4) {
    ss=new shapes::Rect(shapes::RectSize(1.414*size,1.414*size));
    ss->setRotate(45.);
  }
  else
    ss=new shapes::NoShape2();
  ss->setDescr(markshapes2d[idx]);
  return ss;

}

static int chkCoor(int coor) {

  return (coor<0 || coor>3 ? -1 : coor);

}

GraphData2::GraphData2(int sz) : 
  m_sortype(-1) {

  sz=(sz<0 ? 0 : (sz>maxdatacnt ? maxdatacnt : sz));
  mk_listalloc(&m_data,sizeof(mk_vertex),sz);
  mk_listalloc(&m_mark,sizeof(shapes::Shape2*),sz);
    
}

GraphData2::~GraphData2() {

  mk_listfree(&m_data);
  shapes::Shape2 *mark=0;
  while (m_mark.count>0) {
    mk_listremove(&m_mark,m_mark.count-1,(void *)&mark);
    delete mark;
  }
  mk_listfree(&m_mark);
  
}

int GraphData2::data(int idx,mk_vertex vertex,int *avail) {

  mk_vertexnan(vv);
  mk_listat(&m_data,idx,(void *)&vv);
  mk_vertexcopy(vertex,vv);
  return 0;

}

int GraphData2::data(mk_list *vL) {

  int cnt=m_data.count;
  if (!vL)
    return cnt;
  mk_listcopy(vL,&m_data);
  if (m_data.sorted>0)
    mk_listsort(vL);
  return cnt;

}

int GraphData2::setData(int idx,mk_vertex vv_) {

  mk_vertex vv;
  mk_vertexcopy(vv,vv_);
  if (idx>=0 && idx<m_data.count)
    mk_listsetat(&m_data,(void*)&vv,idx,0);
  else if (m_sortype>=0)
    idx=mk_listinsort(&m_data,(void*)&vv);
  else {
    if (idx<0) {
      idx=0;
      mk_listprepend(&m_data,(void*)&vv);
    }
    else {
      idx=m_data.count;
      mk_listappend(&m_data,(void*)&vv);
    }
  }
  return idx;

}

int GraphData2::findBounds(int type,double *bmin,double *bmax) {

  type&=3;
  int ii=0,cnt=m_data.count;
  if (type==0 || cnt==0 || (!bmin && !bmax))
    return cnt;
  if (bmin)
    *bmin=mk_dinf;
  if (bmax)
    *bmax=mk_dsinf;
  mk_vertexnan(vv);
  if (m_data.sorted>=0 && type==m_sortype) {
    if (bmin) {
      mk_listat(&m_data,0,(void*)&vv[0]);
      *bmin=(type==1 ? vv[0] : vv[1]);
    }
    if (bmax) {
      mk_listat(&m_data,cnt-1,(void*)&vv);
      *bmax=(type==1 ? vv[0] : vv[1]);
    }
  }
  else {
    for (ii=0;ii<cnt;ii++) {
      mk_listat(&m_data,ii,(void*)&vv[0]);
      if (bmin) {
        if (type==1 && mk_dlt(vv[0],*bmin))
          *bmin=vv[0];
        else if (type==2 && mk_dlt(vv[1],*bmin))
          *bmin=vv[1];
      }
      if (bmax) {
        if (type==1 && mk_dlt(*bmax,vv[0]))
          *bmax=vv[0];
        else if (type==2 && mk_dlt(*bmax,vv[1]))
          *bmax=vv[1];
      }
    }
  }
  return cnt;
  
}

int GraphData2::match(mk_vertex pp_,mk_vertex md) {

  int ii=0,idxl=-1,idxh=-1,cnt=m_data.count;
  int sortype=chkCoor(m_data.sorted ? m_sortype : -1);
  if (cnt==0)
    return -1;
  mk_vertexnan(pl);
  mk_vertexnan(ph);
  mk_vertex pp;
  mk_vertexcopy(pp,pp_);
  if (sortype>=0) {
    idxh=mk_listfindnextindex(&m_data,&pp[0]);
    if (idxh==0 || idxh==cnt)
      return -1;
    idxl=idxh-1;
    mk_listat(&m_data,idxl,(void*)&pl[0]);
    mk_listat(&m_data,idxh,(void*)&ph[0]);
    if (sortype==0) {
      if (fabs(pp[1]-pl[1])<md[1])
        return idxl;
      else if (fabs(pp[1]-ph[1])<md[1])
        return idxh;
    }
    else if (sortype==1) {
      if (fabs(pp[0]-pl[0])<md[0])
        return idxl;
      else if (fabs(pp[0]-ph[0])<md[0])
        return idxh;
    }
  }
  else {
    for (ii=0;ii<cnt;ii++) {
      mk_listat(&m_data,ii,(void*)&pl);
      if (fabs(pp[0]-pl[0])<fabs(md[0]) && fabs(pp[1]-pl[1])<fabs(md[1]))
        return ii;
    }
  }
  return -1;

}

shapes::Shape2 *GraphData2::mark(int idx) {

  shapes::Shape2 *res=0;
  mk_listat(&m_mark,idx,(void*)&res);
  return res;
  
}

int GraphData2::setMark(int idx,shapes::Shape2 *mark) {

  shapes::Shape2 *oldmark=0;
  mk_listat(&m_mark,idx,(void*)&oldmark);
  if (oldmark)
    delete oldmark;
  int res=mk_listsetat(&m_mark,(void*)&mark,idx,1);
  return res;

}

int GraphData2::setSortype(int coor) {

  int sortype=m_sortype;
  m_sortype=chkCoor(coor);
  m_data.cmp=(m_sortype>=0 ? mk_vertexcmp[m_sortype] : 0);
  mk_listsort(&m_data);
  return sortype;

}

// ***

Graph::Graph(const char *type,unsigned int idd) : mk::TypeId(type,idd),m_graphdata(0) {
      
  m_axis[0]=m_axis[1]=m_axis[2]=0;
  mk_listalloc(&m_scData,sizeof(mk_vertex),0);
  mk_listalloc(&m_scDataI,sizeof(mk_list*),0);
  mk_listalloc(&m_scMark,sizeof(shapes::Shape2*),0);
  
}

Graph::~Graph() {
		  
  if (m_graphdata)
	  delete m_graphdata;
  shapes::Shape2 *scmark=0;
  while (m_scMark.count>0) {
    mk_listremove(&m_scMark,m_scMark.count-1,(void *)&scmark);
    delete scmark;
  }
  mk_listfree(&m_scMark);
  struct mk_list *viL=0;
  while (m_scDataI.count>0) {
    mk_listremove(&m_scDataI,m_scDataI.count-1,(void *)&viL);
    mk_listfree(viL);
    free((void *)viL);
  }
  mk_listfree(&m_scDataI);
  mk_listfree(&m_scData);
		
}

int Graph::sc2sz(mk_vertex vv) const {

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

int Graph::sz2sc(mk_vertex vv) const {

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

int Graph::match(mk_vertex pp_) const {

  if (!m_graphdata)
    return -1;
  mk_vertex pp={pp_[0],pp_[1],mk_dnan,1.};
  mk_vertex md0={0,0,mk_dnan,1.};
  mk_vertex md1={matchdist,matchdist,mk_dnan,1.};
  mk_vertexzero(md);
  sz2sc(md0);
  sz2sc(md1);
  sz2sc(pp);
  mk_vertexsubs(md1,md0,md);
  return m_graphdata->match(pp,md);
 
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

int GraphXY::value(int idx,mk_vertex vertex,int *avail) const { 

  if (m_graphdata)
    return m_graphdata->data(idx,vertex,avail);
  if (avail)
    *avail=0;
  return 1;
  
}

int GraphXY::setValue(int idx,mk_vertex vv,int *modbounds) {

  if (!m_graphdata)
    return -1;
  int avail=0;
  mk_vertex vold;
  m_graphdata->data(idx,vold,&avail);
  int nidx=m_graphdata->setData(idx,vv),modb=0;
  Axis *ax0=dynamic_cast<Axis*>(m_axis[0]),*ax1=dynamic_cast<Axis*>(m_axis[1]);
  if (nidx==idx) {
    if (avail && ax0 && ax0->checkAutoBounds(vold[0],vold[0])>0)
      modb|=ax0->findAutoBounds(1,0,0);
    if (avail && ax1 && ax1->checkAutoBounds(vold[1],vold[1])>0)
      modb|=ax1->findAutoBounds(2,0,0);
  }
  if (ax0)
    modb|=ax0->checkAutoBounds(vv[0],vv[0]);
  if (ax1)
    modb|=ax1->checkAutoBounds(vv[1],vv[1]);
  if (modbounds)
    *modbounds=modb;
  return nidx;

}

int GraphXY::rescale() {

  mk_vertexnan(vvnan);
  mk_listclear(&m_scData,vvnan);
  struct mk_list *viL=0;
  while (m_scDataI.count>0) {
    mk_listremove(&m_scDataI,m_scDataI.count-1,(void *)&viL);
    mk_listfree(viL);
    free((void *)viL);
  }
  mk_string strdbg;
  mk_string markdescr;
  shapes::Shape2 *mark=0;
  shapes::Shape2 *scmark=0;
  while (m_scMark.count>0) {
    mk_listremove(&m_scMark,m_scMark.count-1,(void *)&scmark);
    delete scmark;
  }
  int ii=0,jj=0,idx=-1,ni=0,cnt=(m_graphdata ? m_graphdata->data(&m_scData) : 0);
  if (cnt==0)
    return 0;
  mk_listrealloc(&m_scDataI,cnt);
  mk_listrealloc(&m_scMark,cnt);
  mk_vertexnan(vv);
  if (!m_interpolation) { 
    for (ii=0;ii<cnt;ii++) {
      mk_listat(&m_scData,ii,(void*)&vv);
      sc2sz(vv);
      mk_listsetat(&m_scData,(void*)&vv,ii,0);
      mark=m_graphdata->mark(0);
      if (mark) {
        mark->getDescr(markdescr);
        scmark=buildMarkshape2(&markdescr[0],0);
        scmark->clone(mark);
        scmark->setTranslate(vv);
        mk_listsetat(&m_scMark,(void*)&scmark,ii,1);
      }
    }
    return cnt;
  }
  viL=(struct mk_list *)malloc(sizeof(struct mk_list));
  mk_listalloc(viL,sizeof(mk_vertex),cnt); 
  for (ii=0;ii<cnt;ii++) { 
    mk_listat(&m_scData,ii,(void*)&vv);
    if (mk_isbusted(vv[0])!=0 && mk_isbusted(vv[1])!=0) {
      mk_listappend(&m_scDataI,(void*)&viL);
      viL=(struct mk_list *)malloc(sizeof(struct mk_list));
      mk_listalloc(viL,sizeof(mk_vertex),cnt); 
    }
    else
      mk_listappend(viL,(void*)&vv);
    sc2sz(vv); 
    mk_listsetat(&m_scData,(void*)&vv,ii,0);
    mark=m_graphdata->mark(0);
    if (mark) {
      //mark->toString(strdbg);
      mark->getDescr(markdescr);
      scmark=buildMarkshape2(&markdescr[0],0);
      scmark->clone(mark);
      scmark->setTranslate(vv);
      mk_listsetat(&m_scMark,(void*)&scmark,ii,1);
    }
  }
  mk_listappend(&m_scDataI,(void*)&viL);
  int res=0;
  struct mk_list *scDataI=0;
  for (ii=0;ii<m_scDataI.count;ii++) {
    mk_listat(&m_scDataI,ii,(void*)&scDataI);
    m_interpolation->setCtrl(scDataI);
    m_interpolation->setup();
    ni=num::numInterpolIntermediates(m_interpolation);
    res=m_interpolation->interpol(ni,scDataI);
    for (jj=0;jj<ni;jj++) {
      mk_listat(scDataI,jj,(void*)&vv);
      sc2sz(vv);
      mk_listsetat(scDataI,(void*)&vv,jj,0);
    }
  }
  mk_ulreal itype=m_interpolation->options();
  if ((itype&num::interpol_const)>0 && (itype&num::interpol_steps)==0) {
    struct mk_list cpscDataI;
    mk_listalloc(&cpscDataI,sizeof(struct mk_list),m_scDataI.count);
    mk_listcopy(&cpscDataI,&m_scDataI);
    mk_listclear(&m_scDataI,0);
    for (ii=0;ii<cpscDataI.count;ii++) {
      mk_listat(&cpscDataI,ii,(void*)&scDataI);
      viL=(struct mk_list *)malloc(sizeof(struct mk_list));
      mk_listalloc(viL,sizeof(mk_vertex),scDataI->count); 
      for (jj=0;jj<scDataI->count;jj++) {
        if (jj>0 && (jj%2)>0) {
          mk_listappend(&m_scDataI,viL);
          viL=(struct mk_list *)malloc(sizeof(struct mk_list));
          mk_listalloc(viL,sizeof(mk_vertex),scDataI->count); 
        }
        mk_listat(scDataI,ii,(void*)&vv); 
        mk_listappend(viL,(void*)&vv);
      }
      mk_listappend(&m_scDataI,viL);       
    }
    mk_listfree(&cpscDataI);
  } 
  return cnt;
    
}

} // namespace
