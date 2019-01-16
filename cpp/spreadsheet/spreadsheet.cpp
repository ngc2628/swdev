
#include <osix/xxkey.h>
#include <osix/xxmouse.h>
#include <osix/xxpaint.h>
#include <osix/xxstyle.h>
#include <osix/xxshape.h>
#include <osix/xxpaint.h>
#include <spreadsheet/spreadsheet.h>
#include <stdio.h>
//#include <sys/param.h>

namespace spreadsheet {

static const osix::xxStyle defFocusStyle(defFgColor,3,1);

bool SpreadsheetSelection::operator==(const SpreadsheetSelection &) const {

  return false;

}

bool SpreadsheetSelection::operator<(const SpreadsheetSelection &) const {

  return false;

}

int SpreadsheetSelection::count() const {

  if (m_isRect==0)
    return m_list.count();
  return ((m_list[m_list.count()-1].m_row-m_list[0].m_row+1)*(m_list[m_list.count()-1].m_col-m_list[0].m_col+1));

}

int SpreadsheetSelection::bounds(Coords *lt,Coords *rb) const {

  int cnt=m_list.count();
  if (m_list.sorted()==0 || cnt==0)
    return -1;
  *lt=m_list[0];
  *rb=m_list[cnt-1];
  return 0;

}

int SpreadsheetSelection::setRect(Coords lt,Coords rb) {

  m_list.clear();
  m_list.inSort(lt);
  if (!(lt==rb))
    m_list.inSort(rb);
  m_isRect=1;
  return 0;

}

int SpreadsheetSelection::clear(int mod) {

  mod=(abs(mod)&7);
  if ((mod&1)>0)
    m_list.clear();
  if ((mod&2)>0)
    m_isRect=0;
  if ((mod&4)>0)
    m_anchor.set();
  return 0;

}

int SpreadsheetSelection::resolve() {

  int ii=0,jj=0,cnt=m_list.count();
  if (cnt<2 || !m_isRect)
    return -1;
  Coords si,lt=m_list[0],rb=m_list[cnt-1];
  m_list.clear();
  m_isRect=0;
  for (ii=lt.m_row;ii<=rb.m_row;ii++) {
    for (jj=lt.m_col;jj<=rb.m_col;jj++) {
      si.set(ii,jj);
      m_list.append(si);
    }
  }
  m_list.setSorted(1);
  return 0;

}

// **********
Spreadsheet::Spreadsheet(const char *) :
  m_bgStyle(osix::xxStyle(osix::xx_somecolors[4],1,1)),
  m_focusStyle(defFocusStyle),m_actPos(Coords(0,0)),m_lastPos(Coords(0,0)), m_data(0) {

}

// **********
Spreadsheet::~Spreadsheet() {

}

// **********
SpreadsheetData *Spreadsheet::setSpreadsheetData(SpreadsheetData *data,bool remove) {

  SpreadsheetData *olddata=m_data;
  if (remove&&m_data)
    delete m_data;
  m_data=data;
  m_selection.m_list.resize(m_data ? m_data->nrows()*m_data->ncols() : 0);
  m_selection.m_list.clear();
  m_selection.m_anchor.set(-1,-1);
  m_selection.m_isRect=0;
  return olddata;

}

// **********
int Spreadsheet::calcGeometry(osix::xxRectSize sz) {

  //printf ("%d calcGeometry w=%d h=%d\n",__LINE__,isz.width(),isz.height());

  if (!m_data) {
    m_rects.resize(0,0);
    return 0;
  }

  int ii=0,jj=0,kk=0,cntcols=m_data->ncols(),cntrows=m_data->nrows(),vrows=0,vcols=0,backstatic=cntrows;
  if (cntcols==0||cntrows==0)
    return 0;
  double hh=.0,tt=.0,ll=.0,wh=sz.height(),ww=sz.width(),mysz=.0,tmpsz=.0,corrsz=.0;
  m_rects.resize(cntrows,1);
  SpreadsheetIndex descr(sectiontypeRow);
  for (ii=0;ii<cntrows;ii++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defHeight;
//printf ("r=%d h=%f static=%d\n",i,descr.m_sz,descr.m_static);
    m_rects[ii][0].m_rect.set(0.,tt,osix::xxRectSize(defWidth,descr.m_sz));
    m_rects[ii][0].m_pos.set(-1,-1);
    tt+=descr.m_sz;
  }
  for (ii=cntrows-1;ii>=0;ii--) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defHeight;
//printf ("r=%d h=%f static=%d\n",i,descr.m_sz,descr.m_static);
    if (!descr.isStatic()) {
      backstatic=ii+1;
      break;
    }
    mysz+=descr.m_sz;
  }
  for (ii=0;ii<cntrows;ii++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defHeight;
//printf ("r=%d h=%f static=%d\n",i,descr.m_sz,descr.m_static);
    if (!descr.isStatic())
      break;
    tmpsz=descr.m_sz;
    if (ii>0)
      m_rects[vrows][0].m_rect.setTop(m_rects[vrows-1][0].m_rect.bottom());
    m_rects[vrows][0].m_rect.resize(-1.,tmpsz);
    mysz+=tmpsz+1.;
    vrows++;
  }
  for (jj=ii;jj<backstatic;jj++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defHeight;
//printf ("r=%d h=%f static=%d\n",i,descr.m_sz,descr.m_static);
    tmpsz=descr.m_sz;
    if (mysz+tmpsz>wh) {
      /*m_rects[0][0].m_rect.resize(aux::dnan,m_rects[0][0].m_rect.size().height()+wh-mysz); // row0>>
       for (k=1;k<j;k++) {
       corrsz=m_rects[k][0].m_rect.size().height();
       m_rects[k][0].m_rect.setTop(m_rects[k-1][0].m_rect.bottom()+1.);
       m_rects[k][0].m_rect.resize(aux::dnan,corrsz);
       }*/
      break;
    }
    if (vrows>0)
      m_rects[vrows][0].m_rect.setTop(m_rects[vrows-1][0].m_rect.bottom());
    m_rects[vrows][0].m_rect.resize(-1.,tmpsz);
    mysz+=tmpsz;
    vrows++;
  }
  for (ii=backstatic;ii<cntrows;ii++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defHeight;
//printf ("r=%d h=%f static=%d\n",i,descr.m_sz,descr.m_static);
    if (vrows>0)
      m_rects[vrows][0].m_rect.setTop(m_rects[vrows-1][0].m_rect.bottom());
    m_rects[vrows][0].m_rect.resize(-1.,descr.m_sz);
    vrows++;
  }

  descr.m_type=sectiontypeCol;
  m_rects.resize(vrows,cntcols);
  for (ii=0;ii<vrows;ii++) {
    ll=.0;
    tt=m_rects[ii][0].m_rect.top();
    hh=m_rects[ii][0].m_rect.size().height();
    for (jj=0;jj<cntcols;jj++) {
      descr.m_idx=ii;
      m_data->indexDescr(&descr);
      if (mk_isbusted(descr.m_sz))
        descr.m_sz=defWidth;
//printf ("c=%d w=%f static=%d\n",i,descr.m_sz,descr.m_static);
      m_rects[ii][jj].m_rect.set(ll,tt,osix::xxRectSize(descr.m_sz,hh));
      m_rects[ii][jj].m_pos.set(-1,-1);
      ll+=descr.m_sz;
    }
  }

  mysz=0.;
  backstatic=cntcols;
  for (ii=cntcols-1;ii>0;ii--) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defWidth;
//printf ("c=%d w=%f static=%d\n",i,descr.m_sz,descr.m_static);
    if (!descr.isStatic()) {
      backstatic=ii+1;
      break;
    }
    mysz+=descr.m_sz+1.;
  }
  for (ii=0;ii<cntcols;ii++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defWidth;
//printf ("c=%d w=%f static=%d\n",i,descr.m_sz,descr.m_static);
    if (!descr.isStatic())
      break;
    tmpsz=descr.m_sz;
    if (ii>0)
      m_rects[0][vcols].m_rect.setLeft(m_rects[0][vcols-1].m_rect.right());
    m_rects[0][vcols].m_rect.resize(tmpsz);
    mysz+=tmpsz;
    vcols++;
  }
  for (jj=ii;jj<backstatic;jj++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defWidth;
//printf ("c=%d w=%f static=%d\n",i,descr.m_sz,descr.m_static);
    tmpsz=descr.m_sz;
    if (mysz+tmpsz>ww) {
      m_rects[0][0].m_rect.resize(m_rects[0][0].m_rect.size().width()+ww-mysz);
      for (kk=1;kk<=jj;kk++) {
        corrsz=m_rects[0][kk].m_rect.size().width();
        m_rects[0][kk].m_rect.setLeft(m_rects[0][kk-1].m_rect.right());
        m_rects[0][kk].m_rect.resize(corrsz);
      }
      break;
    }
    if (vcols>0)
      m_rects[0][vcols].m_rect.setLeft(m_rects[0][vcols-1].m_rect.right());
    m_rects[0][vcols].m_rect.resize(tmpsz);
    mysz+=tmpsz;
    vcols++;
  }
  for (ii=backstatic;ii<cntcols;ii++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (mk_isbusted(descr.m_sz))
      descr.m_sz=defWidth;
//printf ("c=%d w=%f static=%d\n",i,descr.m_sz,descr.m_static);
    if (vcols>0)
      m_rects[0][vcols].m_rect.setLeft(m_rects[0][vcols-1].m_rect.right());
    m_rects[0][vcols].m_rect.resize(descr.m_sz);
    vcols++;
  }
  m_rects.resize(vrows,vcols);
//printf ("vrows=%d vcols=%d\n",vrows,vcols);
  for (ii=0;ii<vrows;ii++) {
    tt=m_rects[ii][0].m_rect.top();
    hh=m_rects[ii][0].m_rect.size().height();
    for (jj=0;jj<vcols;jj++) {
      m_rects[ii][jj].m_rect.set(m_rects[0][jj].m_rect.left(),tt,osix::xxRectSize(m_rects[0][jj].m_rect.size().width(),hh));
      m_rects[ii][jj].m_pos.set(-1,-1);
    }
  }

  /*for (i=0;i<vrows;i++) {
   for (j=0;j<vcols;j++)
   printf("r=%d c=%d rect=%f,%f,%f,%f\n",i,j,
   m_rects[i][j].m_rect.left(),m_rects[i][j].m_rect.top(),m_rects[i][j].m_rect.right(),m_rects[i][j].m_rect.bottom());
   }*/

  int maxscrrows=m_data->nrows()-vrows,maxscrcols=m_data->ncols()-vcols,reposrow=-1,reposcol=-1;
//printf ("vrows=%d vcols=%d\n",vrows,vcols);
  if (m_actPos.m_col>maxscrcols)
    m_actPos.m_col=reposcol=0;
  if (m_actPos.m_row>maxscrrows)
    m_actPos.m_row=reposrow=0;
  adjustSliders(reposrow,reposcol,maxscrrows,maxscrcols,1,1);
  calcPositions();

//printf ("calc sz=%f,%f last=%f,%f,%f,%f\n",sz.width(),sz.height(),
//m_rects[vrows-1][vcols-1].m_rect.left(),m_rects[vrows-1][vcols-1].m_rect.top(),m_rects[vrows-1][vcols-1].m_rect.right(),m_rects[vrows-1][vcols-1].m_rect.bottom());

  return vrows*vcols;

}

// **********
int Spreadsheet::validView(int *rows,int *cols) const {

  if (!m_data)
    return 0;
  if (rows)
    *rows=m_rects.rows();
  if (m_rects.rows()==0)
    return 0;
  if (cols)
    *cols=m_rects.cols();
  return (m_rects.cols()>0 ? 1 : 0);

}

// **********
int Spreadsheet::calcPositions() {

  int vrows=0,vcols=0;
  if (validView(&vrows,&vcols)<0)
    return -1;
  int drows=m_data->nrows(),dcols=m_data->ncols();
  if (drows==0||dcols==0)
    return -1;
  int ii=0,jj=0,scrrow0=0,scrrow1=vrows,scrcol0=0,scrcol1=vcols;
  SpreadsheetIndex descr(sectiontypeRow);
  for (ii=drows-1;ii>-1;ii--) {
    scrrow1--;
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
//printf ("r=%d h=%f static=%d\n",i,descr.m_sz,descr.m_static);
    if (!descr.isStatic() || scrrow1<0)
      break;
    for (jj=0;jj<vcols;jj++)
      m_rects[scrrow1][jj].m_pos.m_row=ii;
  }
  for (ii=0;ii<drows;ii++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (!descr.isStatic() || ii>scrrow1)
      break;
    for (jj=0;jj<vcols;jj++)
      m_rects[ii][jj].m_pos.m_row=ii;
  }
  scrrow0=ii;
  descr.m_type=sectiontypeCol;
  for (ii=dcols-1;ii>-1;ii--) {
    scrcol1--;
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (!descr.isStatic() || scrcol1<0)
      break;
    for (jj=0;jj<vrows;jj++)
      m_rects[jj][scrcol1].m_pos.m_col=ii;
  }
  for (ii=0;ii<dcols;ii++) {
    descr.m_idx=ii;
    m_data->indexDescr(&descr);
    if (!descr.isStatic() || ii>scrcol1)
      break;
    for (jj=0;jj<vrows;jj++)
      m_rects[jj][ii].m_pos.m_col=ii;
  }
  scrcol0=ii;

  for (ii=0;ii<vrows;ii++) {
    for (jj=scrcol0;jj<=scrcol1;jj++)
      m_rects[ii][jj].m_pos.m_col=m_actPos.m_col+jj;
  }
  for (ii=0;ii<vcols;ii++) {
    for (jj=scrrow0;jj<=scrrow1;jj++)
      m_rects[jj][ii].m_pos.m_row=m_actPos.m_row+jj;
  }

  adjustEditor();
  return 0;

}

// **********
Coords Spreadsheet::cellAt(num::Vector3 pos,int *dir) const {

  Coords coords(-1,-1);
  if (dir)
    *dir=0;
  int rows=0,cols=0;
  if (validView(&rows,&cols)<0)
    return coords;
  int lb=-1,mb=0,ub=rows;
  const PosRect *pr=0;
  while ((ub-lb)>1) {
    mb=(ub+lb)/2;
    pr=&m_rects[mb][0];
    if (pr->m_rect.top()<=pos.y() && pr->m_rect.bottom()>=pos.y()) {
      coords.m_row=pr->m_pos.m_row;
      break;
    }
    if (pr->m_rect.bottom()<pos.y())
      lb=mb;
    else
      ub=mb;
  }
  if (dir) {
    if (lb==mb)
      (*dir)|=2;
    else if (ub==mb)
      (*dir)|=8;
  }
  lb=-1;
  mb=0;
  ub=cols;
  while ((ub-lb)>1) {
    mb=(ub+lb)/2;
    pr=&m_rects[0][mb];
    if (pr->m_rect.left()<=pos.x() && pr->m_rect.right()>=pos.x()) {
      coords.m_col=pr->m_pos.m_col;
      break;
    }
    if (pr->m_rect.right()<pos.x())
      lb=mb;
    else
      ub=mb;
  }
  if (dir) {
    if (lb==mb)
      (*dir)|=1;
    else if (ub==mb)
      (*dir)|=4;
  }

  return coords;

}

// **********
osix::xxRect Spreadsheet::cellRect(Coords coords,Coords *vpos) const {

  osix::xxRect rect;
  int rows=0,cols=0;
  if (vpos)
    vpos->set(-1,-1);
  if (validView(&rows,&cols)<0)
    return rect;
  int lb=-1,mb=0,ub=rows;
  const PosRect *pr=0;
  while ((ub-lb)>1) {
    mb=(ub+lb)/2;
    pr=m_rects.at(mb,0);
    if (pr->m_pos.m_row==coords.m_row) {
      rect.setTop(pr->m_rect.top());
      rect.setBottom(pr->m_rect.bottom());
      if (vpos)
        vpos->m_row=mb;
      break;
    }
    if (pr->m_pos.m_row<coords.m_row)
      lb=mb;
    else
      ub=mb;
  }
  lb=-1;
  mb=0;
  ub=cols;
  while ((ub-lb)>1) {
    mb=(ub+lb)/2;
    pr=m_rects.at(0,mb);
    if (pr->m_pos.m_col==coords.m_col) {
      rect.setLeft(pr->m_rect.left());
      rect.setRight(pr->m_rect.right());
      if (vpos)
        vpos->m_col=mb;
      break;
    }
    if (pr->m_pos.m_col<coords.m_col)
      lb=mb;
    else
      ub=mb;
  }

  return rect;

}

// **********
int Spreadsheet::cellVisible(Coords coords) const {

  int rows=0,cols=0;
  if (!validView(&rows,&cols) || !coords.valid())
    return -1;
  int lb=-1,mb=0,ub=rows,ret=0;
  bool found=false;
  const PosRect *pr=0;
  if (coords.m_row>=0 && coords.m_row<m_data->nrows()) {
    while ((ub-lb)>1) {
      mb=(ub+lb)/2;
      pr=&m_rects[mb][0];
      if (pr->m_pos.m_row==coords.m_row) {
        found=true;
        break;
      }
      if (pr->m_pos.m_row<coords.m_row)
        lb=mb;
      else
        ub=mb;
    }
    if (!found) {
      if (lb<=1)
        ret|=2;
      else if (ub>=rows-1)
        ret|=8;
    }
  }
  else {
    if (coords.m_row<0)
      ret|=2;
    else if (coords.m_row>=m_data->nrows())
      ret|=8;
  }

  found=false;
  lb=-1;
  mb=0;
  ub=cols;
  if (coords.m_col>=0 && coords.m_col<m_data->ncols()) {
    while ((ub-lb)>1) {
      mb=(ub+lb)/2;
      pr=&m_rects[0][mb];
      if (pr->m_pos.m_col==coords.m_col) {
        found=true;
        break;
      }
      if (pr->m_pos.m_col<coords.m_col)
        lb=mb;
      else
        ub=mb;
    }
    if (!found) {
      if (lb<=1)
        ret|=1;
      else if (ub>=cols-1)
        ret|=4;
    }
  }
  else {
    if (coords.m_col<0)
      ret|=1;
    else if (coords.m_col>=m_data->ncols())
      ret|=4;
  }

  return ret;

}

// **********
Coords Spreadsheet::nextVisible(Coords coords) const {

  int res=cellVisible(coords);
  if (res<0)
    return Coords(-1,-1);
  if (res==0)
    return coords;
  int ii=0,vrows=0,vcols=0;
  validView(&vrows,&vcols);
  const PosRect *pr=0;
  if ((res&1)>0) {
    for (ii=0;ii<vcols;ii++) {
      pr=&m_rects[0][ii];
      if (pr->m_pos.m_col>coords.m_col) {
        coords.m_col=pr->m_pos.m_col;
        break;
      }
    }
  }
  else if ((res&4)>0) {
    for (ii=vcols-1;ii>-1;ii--) {
      pr=&m_rects[0][ii];
      if (pr->m_pos.m_col<coords.m_col) {
        coords.m_col=pr->m_pos.m_col;
        break;
      }
    }
  }
  if ((res&2)>0) {
    for (ii=0;ii<vrows;ii++) {
      pr=&m_rects[ii][0];
      if (pr->m_pos.m_row>coords.m_row) {
        coords.m_row=pr->m_pos.m_row;
        break;
      }
    }
  }
  else if ((res&8)>0) {
    for (ii=vrows-1;ii>-1;ii--) {
      pr=&m_rects[ii][0];
      if (pr->m_pos.m_row<coords.m_row) {
        coords.m_row=pr->m_pos.m_row;
        break;
      }
    }
  }

  return coords;

}

// **********
osix::xxRect Spreadsheet::calcPaintRect(Coords pos1,Coords pos2,osix::xxRect *rr) {

  int rows=0,cols=0;
  if (!validView(&rows,&cols)||!pos1.valid()||!pos2.valid())
    return osix::xxRect();
  int r1=MIN(pos1.m_row,pos2.m_row),r2=MAX(pos1.m_row,pos2.m_row),
      c1=MIN(pos1.m_col,pos2.m_col),c2=MAX(pos1.m_col,pos2.m_col);
  pos1.set(r1,c1);
  pos2.set(r2,c2);
  int vis1=cellVisible(pos1),vis2=cellVisible(pos2);
//printf ("%d pos1=%d,%d pos2=%d,%d vis1=%d vis2=%d\n",__LINE__,pos1.m_row,pos1.m_col,pos2.m_row,pos2.m_col,vis1,vis2);
  if (vis1>0 || vis2>0) {
    if (((vis1&1)>1 && (vis2&1)>0) || ((vis1&4)>0 && (vis2&4)>0) || ((vis1&2)>1 && (vis2&2)>0) || ((vis1&8)>0 && (vis2&8)>0))
      return osix::xxRect();
    if (vis1>0)
      pos1=nextVisible(pos1);
    if (vis2>0)
      pos2=nextVisible(pos2);
//printf ("%d pos1=%d,%d pos2=%d,%d\n",__LINE__,pos1.m_row,pos1.m_col,pos2.m_row,pos2.m_col);
  }
  Coords vpos1,vpos2;
  osix::xxRect rect1=cellRect(pos1,&vpos1),rect2=cellRect(pos2,&vpos2);
//printf ("%d vpos1=%d,%d vpos2=%d,%d\n",__LINE__,vpos1.m_row,vpos1.m_col,vpos2.m_row,vpos2.m_col);
  if (rect1.empty())
    rect1=cellRect(pos2,&vpos1);
  if (rect2.empty())
    rect2=cellRect(pos1,&vpos2);
  if (rect1.empty() || rect2.empty())
    return rect1;
  if (vpos1.m_row>0)
    rect1.setTop(m_rects[vpos1.m_row-1][0].m_rect.top());
  if (vpos1.m_col>0)
    rect1.setLeft(m_rects[0][vpos1.m_col-1].m_rect.left());
  if (vpos2.m_row>=0 && vpos2.m_row<rows-1)
    rect2.setBottom(m_rects[vpos2.m_row+1][0].m_rect.bottom());
  if (vpos2.m_col>=0 && vpos2.m_col<cols-1)
    rect2.setRight(m_rects[0][vpos2.m_col+1].m_rect.right());
  rect1.setRightBottom(rect2.rightBottom());
  if (rr)
    *rr=rr->unite(rect1);

  return rect1;

}

// **********
Coords Spreadsheet::setFocusPos(Coords focusPos,int repaint,osix::xxRect *paintrect) {

  m_lastFocusPos=m_focusPos;
  osix::xxRect rr;
  if (cellVisible(m_lastFocusPos)<=0)
    rr=calcPaintRect(m_lastFocusPos,m_lastFocusPos,paintrect);
  int ii=0,rows=0,cols=0;
  if (!validView(&rows,&cols)) {
    m_focusPos.set(-1,-1);
    if (repaint>0)
      redraw(paintrect ? *paintrect : rr);
    return m_focusPos;
  }
  int drows=m_data->nrows(),dcols=m_data->ncols();
  if (focusPos.m_row>=0&&focusPos.m_row<drows)
    m_focusPos.m_row=focusPos.m_row;
  if (focusPos.m_col>=0&&focusPos.m_col<dcols)
    m_focusPos.m_col=focusPos.m_col;
  if (focusPos.m_row<0||focusPos.m_col<0)
    m_focusPos.set(-1,-1);
  if (m_focusPos.valid()) {
    SpreadsheetIndex descr(sectiontypeRow,m_focusPos.m_row);
    m_data->indexDescr(&descr);
    if (descr.isHeader()) {
      for (ii=0;ii<drows;ii++) {
        descr.m_idx=ii;
        m_data->indexDescr(&descr);
        if (!descr.isHeader()) {
          m_focusPos.m_row=ii;
          break;
        }
      }
    }
    descr.m_type=sectiontypeCol;
    descr.m_idx=m_focusPos.m_col;
    m_data->indexDescr(&descr);
    if (descr.isHeader()) {
      for (ii=0;ii<dcols;ii++) {
        descr.m_idx=ii;
        m_data->indexDescr(&descr);
        if (!descr.isHeader()) {
          m_focusPos.m_col=ii;
          break;
        }
      }
    }
  }
  if (m_lastFocusPos==m_focusPos) {
    if (repaint>0)
      redraw(paintrect ? *paintrect : rr);
    return m_focusPos;
  }
  int vis=cellVisible(m_focusPos);
  if (vis<=0) {
    if (cellVisible(m_lastFocusPos)<=0)
      rr=calcPaintRect(m_lastFocusPos,m_lastFocusPos,0);
    calcPaintRect(m_focusPos,m_focusPos,&rr);
    if (paintrect)
      *paintrect=paintrect->unite(rr);
    if (repaint>0)
      redraw(paintrect ? *paintrect : rr);
    return m_focusPos;
  }
  Coords shpos=m_actPos;
  while (vis>0) {
    if ((vis&1)>0)
      shpos.m_col--;
    else if ((vis&4)>0)
      shpos.m_col++;
    if ((vis&2)>0)
      shpos.m_row--;
    else if ((vis&8)>0)
      shpos.m_row++;
    shpos=setPos(shpos,false,&rr);
    vis=cellVisible(m_focusPos);
  }
  if (paintrect)
    *paintrect=paintrect->unite(rr);
  if (repaint>0)
    redraw(paintrect ? *paintrect : rr);

  return m_focusPos;

}

// **********
Coords Spreadsheet::setPos(Coords pos,int repaint,osix::xxRect *paintrect,int fromslider) {

  int rows=0,cols=0;
  m_lastPos=m_actPos;
  if (!validView(&rows,&cols))
    return m_lastPos;
  if (pos.m_row<0)
    pos.m_row=m_lastPos.m_row;
  if (pos.m_col<0)
    pos.m_col=m_lastPos.m_col;
  if (pos.m_row>=m_data->nrows()-rows)
    pos.m_row=m_data->nrows()-rows;
  if (pos.m_col>=m_data->ncols()-cols)
    pos.m_col=m_data->ncols()-cols;
  m_actPos=pos;
  int move=0;
  if (m_actPos!=m_lastPos) {
    calcPositions();
    move=1;
  }
  if (move>0) {
    osix::xxRect r=calcPaintRect(Coords(0,0),Coords(m_data->nrows(),m_data->ncols()),paintrect);
    aux::Asciistr str;
    r.toString(&str);
//printf ("%d move %s\n",__LINE__,(const char *)str);
    if (repaint>0)
      redraw(r);
  }

  if (fromslider==0)
    adjustSliders(m_actPos.m_row,m_actPos.m_col,-1,-1,-1,-1);

  return m_actPos;

}

// **********
int Spreadsheet::selRect(Coords *cmin,Coords *cmax) const {

  int cnt=m_selection.m_list.count(),maxcol=(m_data ? m_data->ncols()-1 : maxNumCols-1);
  //maxrow=(m_data ? m_data->nrows()-1 : maxNumRows-1);
  cmin->set();
  cmax->set();
  if (cnt==0)
    return 0;
  *cmin=m_selection.m_list[0];
  *cmax=m_selection.m_list[cnt-1];
  if (cnt<3)
    return 1;
  else if (m_selection.m_isRect)
    return 2;
  Coords nrow(cmin->m_row,maxcol),act,last;
  int idx=m_selection.m_list.findNextIndex(nrow);
  while (idx>0&&idx<cnt) {
    act=m_selection.m_list[idx];
    last=m_selection.m_list[idx-1];
    if (act.m_row==last.m_row)
      break;
    if (act.m_col<cmin->m_col)
      cmin->m_col=act.m_col;
    if (cmax->m_col<act.m_col)
      cmax->m_col=act.m_col;
    if (last.m_col<cmin->m_col)
      cmin->m_col=last.m_col;
    if (cmax->m_col<last.m_col)
      cmax->m_col=last.m_col;
    nrow.m_row=act.m_row;
    idx=m_selection.m_list.findNextIndex(nrow);
  }

  return 2;

}

// **********
int isRectSel(const aux::TVList<Coords> &ss,Coords *s0,Coords *s1) {

  int cnt=ss.count();
  if (cnt==0)
    return 0;
  if (cnt==1)
    return 1;
  Coords lt=ss[0],rb=ss[cnt-1],nrow(lt.m_row,maxNumCols);
  Coords last;
  int ii=0,actrow=lt.m_row,lastcol=rb.m_col,idx=ss.findNextIndex(nrow),nidx=idx;
  while (idx>0&&idx<cnt) {
    actrow=ss[idx].m_row;
    last=ss[idx-1];
    if (actrow==last.m_row)
      break;
    if (actrow-last.m_row>1||last.m_col!=lastcol)
      return 0;
    nrow.m_row=actrow;
    nidx=ss.findNextIndex(nrow);
    for (ii=idx+1;ii<nidx;ii++) {
      if (ss[ii].m_col-ss[ii-1].m_col>1)
        return 0;
    }
    idx=nidx;
  }
  *s0=lt;
  *s1=rb;
//for (i=0;i<s.count();i++)
  //printf ("iRect r=%d c=%d\n",s[i].m_row,s[i].m_col);

  return 1;

}

// **********
int Spreadsheet::setSelection(aux::TVList<Coords> ss,int mod) {

  ss.setCmp(0);
  ss.sort();
  int ii=0,idx=-1,cnt=m_selection.m_list.count(),scnt=ss.count();
  osix::xxRect rr;
  Coords si,lt,rb;
  if (selRect(&lt,&rb)>0)
    calcPaintRect(lt,rb,&rr);
  if (mod<0||mod>2)
    mod=0;
  if (mod==2 && (cnt==0 || scnt==0)) {
    redraw(rr);
    return 0;
  }
  if (mod==1 && scnt==0) {
    redraw(rr);
    return m_selection.count();
  }
  if (mod==0 || cnt==0) {
    if (scnt==0 && cnt>0)
      m_selection.clear();
    else if (scnt>0) {
      m_selection.m_list=ss;
      m_selection.m_anchor=m_selection.m_list[0];
      m_selection.m_list.sort();
      if (isRectSel(m_selection.m_list,&lt,&rb)>0)
        m_selection.setRect(lt,rb);
      if (selRect(&lt,&rb)>0)
        calcPaintRect(lt,rb,&rr);
    }
    redraw(rr);
    return m_selection.count();
  }
  m_selection.resolve();
  for (ii=0;ii<scnt;ii++) {
    si=ss[ii];
    idx=m_selection.m_list.find(si);
    if (mod==1) {
      if (idx<0)
        m_selection.m_list.inSort(si);
    }
    else if (mod==2) {
      if (idx>=0)
        m_selection.m_list.remove(idx);
    }
  }
  if (isRectSel(m_selection.m_list,&lt,&rb)>0)
    m_selection.setRect(lt,rb);
  cnt=m_selection.count();
  if (cnt==0)
    m_selection.m_anchor.set();
  else {
    m_selection.m_anchor=m_selection.m_list[0];
    if (selRect(&lt,&rb)>0)
      calcPaintRect(lt,rb,&rr);
  }
  redraw(rr);

  return cnt;

}

// **********
int Spreadsheet::selection(aux::TVList<Coords> *ss,int *isRect) const {

  if (ss)
    *ss=m_selection.m_list;
  if (isRect)
    *isRect=m_selection.m_isRect;
  return m_selection.count();

}

// **********
int Spreadsheet::isSelected(Coords ss) const {

  int res=0,cnt=m_selection.m_list.count();
  if (cnt==0)
    return res;
  SpreadsheetDataItem *itm=0;
  if (m_data) {
    itm=m_data->data(ss.m_row,ss.m_col);
    if (itm)
      res=itm->m_selectable;
  }
  if ((res&cellUnselectable)==0) {
    if (m_selection.m_isRect && cnt>1) {
      int t=m_selection.m_list[0].m_row,l=m_selection.m_list[0].m_col,
          b=m_selection.m_list[cnt-1].m_row,r=m_selection.m_list[cnt-1].m_col;
      if (t<=ss.m_row && ss.m_row<=b && l<=ss.m_col && ss.m_col<=r)
        res|=cellSelected;
    }
    else if (m_selection.m_list.find(ss)>=0)
      res|=cellSelected;
  }

  return res;

}

int Spreadsheet::isEditing(Coords *) const {

  return 0;

}

// **********
int Spreadsheet::select(Coords s,int mods,int repaint,osix::xxRect *paintrect,int) {

  int vrows=0,vcols=0;
  if (!s.valid()||!validView(&vrows,&vcols))
    return -1;
//printf ("mod=%d\n",mod);
  osix::xxRect rr;
  Coords si,lt,rb;
  int ii=0,selflag=isSelected(s),maxrow=m_data->nrows()-1,maxcol=m_data->ncols()-1;
  //iRect=m_selection.m_iRect;
//printf ("oldsel beg=%d,%d end=%d,%d\n",m_selection[0].m_row,m_selection[0].m_col,m_selection[cnt-1].m_row,m_selection[cnt-1].m_col);
  if (selRect(&lt,&rb)>0)
    calcPaintRect(lt,rb,&rr);
  bool rowsel=false,colsel=false,expand=false;
  SpreadsheetIndex descr(sectiontypeRow,s.m_row);
  m_data->indexDescr(&descr);
  if (descr.isHeader())
    colsel=true;
  descr.m_type=sectiontypeCol;
  descr.m_idx=s.m_col;
  m_data->indexDescr(&descr);
  if (descr.isHeader())
    rowsel=true;
//printf ("%d rowsel=%d colsel=%d mod=%d s=%d,%d\n",__LINE__,rowsel,colsel,mods,s.m_row,s.m_col);
  if (rowsel && colsel)
    mods=0;
  if (mods==0) {
    m_selection.clear();
    if (rowsel && colsel) {
      lt.set(0,0);
      rb.set(maxrow,maxcol);
      m_selection.setRect(lt,rb);
    }
    else if (rowsel) {
      lt.set(s.m_row,0);
      rb.set(s.m_row,maxcol);
      m_selection.setRect(lt,rb);
    }
    else if (colsel) {
      lt.set(0,s.m_col);
      rb.set(maxrow,s.m_col);
      m_selection.setRect(lt,rb);
    }
    else if ((selflag&cellUnselectable)==0)
      m_selection.setRect(s,s);
    if ((selflag&cellUnselectable)==0)
      m_selection.m_anchor=s;
  }
  else if ((mods&osix::xx_modCtrl)>0) {
    if ((selflag&cellUnselectable)==0) {
      if (!m_selection.m_anchor.valid())
        m_selection.m_anchor=s;
      if ((selflag&cellSelected)==0) {
        expand=false;
        if (rowsel) {
          if (m_selection.m_isRect && m_selection.bounds(&lt,&rb)>0) {
            if (lt.m_col==0 && rb.m_col==maxcol) {
              if (s.m_row<lt.m_row && lt.m_row-s.m_row<2)
                m_selection.setRect(Coords(s.m_row,0),rb);
              else if (s.m_row>rb.m_row&&s.m_row-rb.m_row<2)
                m_selection.setRect(lt,Coords(s.m_row,maxcol));
              else
                expand=true;
            }
            else
              expand=true;
          }
          else
            expand=true;
          if (expand) {
            m_selection.resolve();
            for (ii=0;ii<=maxcol;ii++) {
              si.set(s.m_row,ii);
              if (m_selection.m_list.find(si)<0)
                m_selection.m_list.inSort(si);
            }
            if (isRectSel(m_selection.m_list,&lt,&rb)>0)
              m_selection.setRect(lt,rb);
          }
        }
        else if (colsel) {
          if (m_selection.m_isRect && m_selection.bounds(&lt,&rb)>0) {
            if (lt.m_row==0 && rb.m_row==maxrow) {
              if (s.m_col<lt.m_col && lt.m_col-s.m_col<2)
                m_selection.setRect(Coords(0,s.m_col),rb);
              else if (s.m_col>rb.m_col && s.m_col-rb.m_col<2)
                m_selection.setRect(lt,Coords(maxrow,s.m_col));
              else
                expand=true;
            }
            else
              expand=true;
          }
          else
            expand=true;
          if (expand) {
            m_selection.resolve();
            for (ii=0;ii<=maxrow;ii++) {
              si.set(ii,s.m_col);
              if (m_selection.m_list.find(si)<0)
                m_selection.m_list.inSort(si);
            }
            if (isRectSel(m_selection.m_list,&lt,&rb)>0)
              m_selection.setRect(lt,rb);
          }
        }
        else {
          m_selection.resolve();
          m_selection.m_list.inSort(s);
          if (isRectSel(m_selection.m_list,&lt,&rb)>0)
            m_selection.setRect(lt,rb);
          else
            m_selection.clear(2);
        }
      }
      else {
        expand=false;
        if (rowsel) {
          if (m_selection.m_isRect && m_selection.bounds(&lt,&rb)>0) {
            if (lt.m_col==0 && rb.m_col==maxcol) {
              if (lt.m_row==s.m_row)
                m_selection.setRect(Coords(s.m_row,0),rb);
              else if (rb.m_row==s.m_row)
                m_selection.setRect(lt,Coords(s.m_row,maxcol));
              else
                expand=true;
            }
            else
              expand=true;
          }
          else
            expand=true;
          if (expand) {
            m_selection.resolve();
            for (ii=0;ii<=maxcol;ii++) {
              si.set(s.m_row,ii);
              m_selection.m_list.remove(m_selection.m_list.find(si));
            }
            if (isRectSel(m_selection.m_list,&lt,&rb)>0)
              m_selection.setRect(lt,rb);
          }
        }
        else if (colsel) {
          if (m_selection.m_isRect && m_selection.bounds(&lt,&rb)>0) {
            if (lt.m_row==0 && rb.m_row==maxrow) {
              if (lt.m_col==s.m_col)
                m_selection.setRect(Coords(0,s.m_col),rb);
              else if (rb.m_col==s.m_col)
                m_selection.setRect(lt,Coords(maxrow,s.m_col));
              else
                expand=true;
            }
          else
            expand=true;
          }
          else
            expand=true;
          if (expand) {
            m_selection.resolve();
            for (ii=0;ii<=maxrow;ii++) {
              si.set(ii,s.m_col);
              m_selection.m_list.remove(m_selection.m_list.find(si));
            }
            if (isRectSel(m_selection.m_list,&lt,&rb)>0)
              m_selection.setRect(lt,rb);
          }
        }
        else {
          m_selection.resolve();
          m_selection.m_list.remove(m_selection.m_list.find(s));
          if (isRectSel(m_selection.m_list,&lt,&rb)>0)
            m_selection.setRect(lt,rb);
          else
            m_selection.clear(2);
        }
        if (m_selection.m_anchor==s) {
          if (m_lastFocusPos.valid())
            m_selection.m_anchor=m_lastFocusPos;
          else
            m_selection.m_anchor=m_focusPos;
        }
      }
    }
  }
  else if ((mods&osix::xx_modShift)>0) {
    int minr=MIN(m_selection.m_anchor.m_row,s.m_row),minc=MIN(m_selection.m_anchor.m_col,s.m_col),
        maxr=MAX(m_selection.m_anchor.m_row,s.m_row),maxc=MAX(m_selection.m_anchor.m_col,s.m_col);
    if (rowsel)
      maxc=maxcol;
    if (colsel)
      maxr=maxrow;
    m_selection.clear(3);
    m_selection.setRect(Coords(minr,minc),Coords(maxr,maxc));
//printf ("%d rt=%d rb=%d cl=%d cr=%d selcnt=%d listcnt=%d\n",__LINE__,minr,maxr,minc,maxc,m_selection.count(),m_selection.m_list.count());
    if ((isSelected(m_selection.m_anchor)&cellSelected)==0)
      m_selection.m_anchor=m_selection.m_list[0];
  }

  if (selRect(&lt,&rb)>0)
    calcPaintRect(lt,rb,&rr);
  if (paintrect)
    *paintrect=paintrect->unite(rr);
  if (repaint)
    redraw(paintrect ? *paintrect : rr);

  return 0;

}

int Spreadsheet::startEditCell(Coords,int) {

  return -1;

}

// **********
// **********

class Gridline {
  public:
    int m_type,m_section;
    osix::xxRect m_rect;
    Gridline() {
    }
    Gridline(const Gridline &ass) : m_type(ass.m_type),m_section(ass.m_section),m_rect(ass.m_rect) {
    }
    ~Gridline() {
    }
    Gridline &operator=(const Gridline &ass) {
      m_type=ass.m_type;
      m_section=ass.m_section;
      m_rect=ass.m_rect;
      return *this;
    }
    bool operator==(const Gridline &cmp) const {
      return (m_type==cmp.m_type && m_section==cmp.m_section);
    }
    bool operator<(const Gridline &cmp) const {
      return (m_type<cmp.m_type || (m_type==cmp.m_type && m_section<cmp.m_section));
    }
    void set(int type,int section,osix::xxRect rect) {
      m_type=type;
      m_section=section;
      m_rect=rect;
    }
};

int Spreadsheet::paintCells(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc) {

  int rows=0,cols=0;
  osix::xxGC gc=*xxgc;
  osix::xxRect rr=xxgc->m_r;
  if (rr.empty() || !validView(&rows,&cols))
    return -1;
  if (!disp)
    disp=findDisplay();
//printf ("%d l=%f t=%f r=%f b=%f\n",__LINE__,rr.left(),rr.top(),rr.right(),rr.bottom());
  int ii=0,jj=0,rowf=0,rowl=rows-1,colf=0,coll=cols-1;
  PosRect *pr=0;

  for (ii=0;ii<rows;ii++) {
    pr=m_rects.at(ii,0);
    if (rr.top()<=pr->m_rect.top()) {
      rowf=ii;
      break;
    }
  }
  for (ii=rows-1;ii>-1;ii--) {
    pr=m_rects.at(ii,0);
    if (rr.bottom()>=pr->m_rect.bottom()) {
      rowl=ii;
      break;
    }
  }
  for (ii=0;ii<cols;ii++) {
    pr=m_rects.at(0,ii);
    if (rr.left()<=pr->m_rect.left()) {
      colf=ii;
      break;
    }
  }
  for (ii=cols-1;ii>-1;ii--) {
    pr=m_rects.at(0,ii);
//printf ("%d col=%d rright=%d prright=%f\n",__LINE__,i,r.right(),pr->m_rect.right());
    if (rr.right()>=pr->m_rect.right()) {
      coll=ii;
      break;
    }
  }
  if (rowf<0)
    rowf=0;
  if (rowl>=rows || rowl<rowf)
    rowl=rows-1;
  if (colf<0)
    colf=0;
  if (coll>=cols || coll<colf)
    coll=cols-1;

  osix::xxRect focusRect;
  osix::xxStyle sStyle=defSelectedStyle;
  aux::TVList<Gridline> gridlines(rowl-rowf+2+coll-colf+2);
  Gridline gridline;
  PosRect *prnext=0;
  int selflag=0;
//aux::Asciistr strDbg;
//printf ("%d rowf=%d rowl=%d colf=%d coll=%d\n",__LINE__,rowf,rowl,colf,coll);
  for (jj=colf;jj<=coll;jj++) {
    pr=m_rects.at(rowf,jj);
    prnext=m_rects.at(rowl,jj);
    gridline.set(sectiontypeCol,pr->m_pos.m_col,osix::xxRect(pr->m_rect.leftTop(),prnext->m_rect.leftBottom()));
    gridline.m_rect.setBottom(gridline.m_rect.bottom());
    gridlines.inSort(gridline);
    if (jj==coll) {
      gridline.set(sectiontypeCol,pr->m_pos.m_col+1,osix::xxRect(pr->m_rect.rightTop(),prnext->m_rect.rightBottom()));
      gridlines.inSort(gridline);
    }
  }
  for (ii=rowf;ii<=rowl;ii++) {
    pr=m_rects.at(ii,colf);
    prnext=m_rects.at(ii,coll);
    gridline.set(sectiontypeRow,pr->m_pos.m_row,osix::xxRect(pr->m_rect.leftTop(),prnext->m_rect.rightTop()));
    gridlines.inSort(gridline);
    if (ii==rowl) {
      gridline.set(sectiontypeRow,pr->m_pos.m_row+1,osix::xxRect(pr->m_rect.leftBottom(),prnext->m_rect.rightBottom()));
      gridlines.inSort(gridline);
    }
    for (jj=colf;jj<=coll;jj++) {
      pr=m_rects.at(ii,jj);
//printf ("%d i=%d j=%d r=%d c=%d r=%f,%f,%f,%f\n",__LINE__,i,j,pr->m_pos.m_row,pr->m_pos.m_col,
//pr->m_rect.left(),pr->m_rect.top(),pr->m_rect.right(),pr->m_rect.bottom());
//if (pr->m_pos.m_row==rows-1 || pr->m_pos.m_col==cols-1)
//printf ("%d r=%d c=%d r=%f,%f,%f,%f\n",__LINE__,pr->m_pos.m_row,pr->m_pos.m_col,
//pr->m_rect.left(),pr->m_rect.top(),pr->m_rect.right(),pr->m_rect.bottom());
//printf ("%d cell=%d,%d sel=%d\n",__LINE__,pr->m_pos.m_row,pr->m_pos.m_col,isSelected(pr->m_pos));
      gc.m_r=pr->m_rect;
      gc.m_bg=sStyle;
      gc.m_w=m_winscr.m_w;
      selflag=(pr->m_pos!=m_focusPos && ((isSelected(pr->m_pos)&cellSelected)>0) ? hlCell : 0);
      m_data->drawCell(disp,xxdrawable,&gc,pr->m_pos.m_row,pr->m_pos.m_col,selflag);
      if (m_focusPos.m_row==pr->m_pos.m_row && m_focusPos.m_col==pr->m_pos.m_col && !isEditing())
        focusRect=pr->m_rect;
    }
  }
  Gridline *pgridline=0;
  for (ii=0;ii<gridlines.count();ii++) {
    pgridline=gridlines.at(ii);
    gc.m_r=pgridline->m_rect;
    m_data->drawGridLine(disp,xxdrawable,&gc,pgridline->m_type,pgridline->m_section);
  }

/*pr=m_rects.at(rowf,colf);
printf ("i=%d j=%d r=%d c=%d r=%f,%f,%f,%f\n",i,j,pr->m_pos.m_row,pr->m_pos.m_col,
pr->m_rect.left(),pr->m_rect.top(),pr->m_rect.right(),pr->m_rect.bottom());
pr=m_rects.at(rowl,coll);
printf ("i=%d j=%d r=%d c=%d r=%f,%f,%f,%f\n",i,j,pr->m_pos.m_row,pr->m_pos.m_col,
pr->m_rect.left(),pr->m_rect.top(),pr->m_rect.right(),pr->m_rect.bottom());*/

  if (!focusRect.empty()) {
    gc.m_r=focusRect;
    gc.m_fg=m_focusStyle;
    osix::xxdrawRect(disp,xxdrawable,&gc);
  }

  return 0;

}

// **********
int Spreadsheet::scrollAction(int,num::Vector3 vpos) {

  int out=0,rows=0,cols=0;
  if (!validView(&rows,&cols))
    return stopScrolling();
  Coords pos=cellAt(vpos,&out);
  if (!pos.valid() && out>0) {
    if ((out&1)>0)
      pos.m_col=m_data->ncols()-1;
    else if ((out&4)>0)
      pos.m_col=0;
    if ((out&2)>0)
      pos.m_row=m_data->nrows()-1;
    else if ((out&8)>0)
      pos.m_row=0;
  }
  if (!pos.valid())
    return stopScrolling();

//printf ("%d scrollto %d,%d\n",__LINE__,pos.m_row,pos.m_col);

  osix::xxRect rr;
  SpreadsheetIndex rdescr(sectiontypeRow,pos.m_row),cdescr(sectiontypeCol,pos.m_col),
                   frdescr(sectiontypeRow,m_focusPos.m_row),fcdescr(sectiontypeCol,m_focusPos.m_col);
  m_data->indexDescr(&rdescr);
  m_data->indexDescr(&cdescr);
  m_data->indexDescr(&frdescr);
  m_data->indexDescr(&fcdescr);
  if (rdescr.isStatic() && !frdescr.isStatic()) {
    if (pos.m_row<m_focusPos.m_row) {
      if (0<m_actPos.m_row)
        pos.m_row=m_focusPos.m_row-1;
    }
    else if (m_focusPos.m_row<pos.m_row) {
      if (m_actPos.m_row+rows<m_data->nrows())
        pos.m_row=m_focusPos.m_row+1;
    }
  }
  else if (frdescr.isStatic() && !rdescr.isStatic()) {
    if (pos.m_row<m_focusPos.m_row) {
      if (m_actPos.m_row+rows<m_data->nrows())
        pos.m_row=m_focusPos.m_row+1;
    }
    else if (m_focusPos.m_row<pos.m_row) {
      if (0<m_actPos.m_row)
        pos.m_row=m_focusPos.m_row-1;
    }
  }
  if (cdescr.isStatic() && !fcdescr.isStatic()) {
    if (pos.m_col<m_focusPos.m_col) {
      if (0<m_actPos.m_col)
        pos.m_col=m_focusPos.m_col-1;
    }
    else if (m_focusPos.m_col<pos.m_col) {
      if (m_actPos.m_col+cols<m_data->ncols())
        pos.m_col=m_focusPos.m_col+1;
    }
  }
  else if (fcdescr.isStatic() && !cdescr.isStatic()) {
    if (pos.m_col<m_focusPos.m_col) {
      if (m_actPos.m_col+cols<m_data->ncols())
        pos.m_col=m_focusPos.m_col+1;
    }
    else if (m_focusPos.m_col<pos.m_col) {
      if (0<m_actPos.m_col)
        pos.m_col=m_focusPos.m_col-1;
    }
  }
  if (m_focusPos==pos)
    return stopScrolling();
  setFocusPos(pos,0,&rr);
//printf ("%d pos=%d,%d actp=%d,%d vr=%d vc=%d fp=%d,%d mfp=%d,%d\n",__LINE__,pos.m_row,pos.m_col,m_actPos.m_row,m_actPos.m_col,rows,cols,focusPos.m_row,focusPos.m_col,m_focusPos.m_row,m_focusPos.m_col);
  if (rdescr.isHeader() || cdescr.isHeader()) {
    select(pos,osix::xx_modShift,0,&rr,1);
    redraw(rr);
  }
  else if (m_focusPos!=m_lastFocusPos) {
    select(m_focusPos,osix::xx_modShift,0,&rr,1);
    redraw(rr);
  }
  else
    return stopScrolling();

  return 0;

}

// **********
int Spreadsheet::redraw(osix::xxRect rr,int) {

  void *disp=findDisplay();
  osix::xxGC gc;
  gc.m_r=rr;
  osix::xxpredraw(disp,&m_pixscr,&gc);
  paintCells(disp,&m_pixscr,&gc);
  osix::xxpostdraw(disp,&m_pixscr,&gc);
  osUpdate();
  return 0;

}

// **********
int Spreadsheet::osEvent(void *disp,void *osev) {

  osix::xxEvent xxev;
  osix::xxtranslateEvent(osev,&xxev);
  osix::xxGC gc;
  gc.m_bg=m_bgStyle;
  gc.m_r=xxev.m_r;
  osix::xxRect rdest,rsrc;
  if (xxev.m_type==osix::xx_resize) {
    m_winscr.m_r=gc.m_r;
    osix::xxresizeDrawable(disp,&m_pixscr,&gc);
    calcGeometry(gc.m_r.size());
    redraw(gc.m_r);
  }
  else if (xxev.m_type==osix::xx_paint)
    osix::xxcopyArea(disp,&m_winscr,&m_pixscr,&gc,rdest,rsrc);
  else if (osix::xxTypeInput(&xxev)>0)
    inputEvent(&xxev);

  return xxev.m_consumer;

}

// **********
int Spreadsheet::inputEvent(osix::xxEvent *xxev) {

  int res=0,rows=0,cols=0,validview=validView(&rows,&cols),
      autoscrollconsumer=(isScrolling()==autoscroll ? osix::xx_processed :0);

  Coords pos;
  osix::xxRect rr;
  if (osix::xxTypeMouse(xxev)>0)
    pos=cellAt(xxev->m_pos);
  if (xxev->m_type==osix::xx_mousePressed) {
    stopEditCell(1);
    if (validview==0)
      return xxev->m_consumer;
    setFocusPos(pos,0,&rr);
    select(pos,(xxev->m_mods&osix::xx_modCtrl)|(xxev->m_mods&osix::xx_modShift),0,&rr);
    redraw(rr);
    xxev->m_consumer|=osix::xx_processed;
    res=xxev->m_consumer;
    xxev->m_lastpos=m_xxlastinputev.m_pos;
  }
  else if (xxev->m_type==osix::xx_mouseMove) {
    if (validview==0) {
      xxev->m_consumer|=autoscrollconsumer;
      stopScrolling();
      res=xxev->m_consumer;
    }
    else if ((xxev->m_buttons&osix::xxm_leftButton)==0 || (xxev->m_mods&osix::xx_modCtrl)>0) {
      xxev->m_consumer|=autoscrollconsumer;
      stopScrolling();
      res=xxev->m_consumer;
    }
    else if (m_focusPos==pos) {
      xxev->m_consumer|=autoscrollconsumer;
      stopScrolling();
      res=xxev->m_consumer;
    }
    else
      res=mouseMoved(xxev);
    xxev->m_lastpos=m_xxlastinputev.m_pos;
    xxev->m_downpos=m_xxlastinputev.m_downpos;
  }
  else if (xxev->m_type==osix::xx_mouseReleased) {
    if ((m_xxlastinputev.m_consumer&osix::xx_consumed)>0)
      xxev->m_consumer|=osix::xx_consumed;
    xxev->m_consumer|=autoscrollconsumer;
    stopScrolling();
    res=xxev->m_consumer;
    xxev->m_lastpos=m_xxlastinputev.m_pos;
    xxev->m_downpos=m_xxlastinputev.m_downpos;
  }
  else if (xxev->m_type==osix::xx_mouseDblClicked) {
    if (validview==0) {
      xxev->m_consumer|=autoscrollconsumer;
      stopScrolling();
    }
    else if (startEditCell(pos)==0)
      xxev->m_consumer|=osix::xx_processed;
    res=xxev->m_consumer;
  }
  else if (xxev->m_type==osix::xx_keyPressed) {
    if (autoscrollconsumer==osix::xx_processed)
      res=xxev->m_consumer;
    else
      res=keyPressed(xxev);
  }
  else if (xxev->m_type==osix::xx_keyReleased) {
    if ((m_xxlastinputev.m_consumer&osix::xx_consumed)>0)
      xxev->m_consumer|=osix::xx_consumed;
    if (autoscrollconsumer==osix::xx_processed)
      res=xxev->m_consumer;
    else if ((m_xxlastinputev.m_consumer&osix::xx_processed)>0)
      res=keyReleased(xxev);
  }
  else
    res=xxev->m_consumer;

  m_xxlastinputev=*xxev;

  return res;

}

// **********
int Spreadsheet::mouseMoved(osix::xxEvent *xxev) {

  Coords pos=cellAt(xxev->m_pos),focusPos=pos;
  SpreadsheetIndex rdescr(sectiontypeRow,pos.m_row),cdescr(sectiontypeCol,pos.m_col),
                   frdescr(sectiontypeRow,m_focusPos.m_row),fcdescr(sectiontypeCol,m_focusPos.m_col);
  m_data->indexDescr(&rdescr);
  m_data->indexDescr(&cdescr);
  m_data->indexDescr(&frdescr);
  m_data->indexDescr(&fcdescr);
  int rows=0,cols=0;
  validView(&rows,&cols);
  if (rdescr.isStatic() && !frdescr.isStatic()) {
    if (pos.m_row<m_focusPos.m_row) {
      if (0<m_actPos.m_row)
        focusPos.m_row=m_focusPos.m_row-1;
    }
    else if (m_focusPos.m_row<pos.m_row) {
      if (m_actPos.m_row+rows<m_data->nrows())
        focusPos.m_row=m_focusPos.m_row+1;
    }
  }
  else if (frdescr.isStatic() && !rdescr.isStatic()) {
    if (pos.m_row<m_focusPos.m_row) {
      if (m_actPos.m_row+rows<m_data->nrows())
        focusPos.m_row=m_focusPos.m_row+1;
    }
    else if (m_focusPos.m_row<pos.m_row) {
      if (0<m_actPos.m_row)
        focusPos.m_row=m_focusPos.m_row-1;
    }
  }
  if (cdescr.isStatic() && !fcdescr.isStatic()) {
    if (pos.m_col<m_focusPos.m_col) {
      if (0<m_actPos.m_col)
        focusPos.m_col=m_focusPos.m_col-1;
    }
    else if (m_focusPos.m_col<pos.m_col) {
      if (m_actPos.m_col+cols<m_data->ncols())
        focusPos.m_col=m_focusPos.m_col+1;
    }
  }
  else if (fcdescr.isStatic() && !cdescr.isStatic()) {
    if (pos.m_col<m_focusPos.m_col) {
      if (m_actPos.m_col+cols<m_data->ncols())
        focusPos.m_col=m_focusPos.m_col+1;
    }
    else if (m_focusPos.m_col<pos.m_col) {
      if (0<m_actPos.m_col)
        focusPos.m_col=m_focusPos.m_col-1;
    }
  }
  osix::xxRect rr;
  if (pos==focusPos) {
    xxev->m_consumer|=osix::xx_processed;
    stopScrolling();
    setFocusPos(pos,0,&rr);
    if (cdescr.isHeader() || rdescr.isHeader())
      select(pos,osix::xx_modShift,0,&rr);
    else if (m_focusPos!=m_lastFocusPos)
      select(m_focusPos,osix::xx_modShift,0,&rr);
    redraw(rr);
  }
  else if (isScrolling()==0) {
    if (startScrolling()==autoscroll)
      xxev->m_consumer|=osix::xx_processed;
  }

  return xxev->m_consumer;

}

  // **********
int Spreadsheet::keyPressed(osix::xxEvent *xxev) {

  if (osix::navkey(xxev)) {
    stopEditCell(1);
    navKeyPressed(xxev);
  }
  else if (osix::typewriterkey(xxev)) {
    if ((xxev->m_xxk==osix::xxk_C || xxev->m_xxk==osix::xxk_V || xxev->m_xxk==osix::xxk_X) &&
        (xxev->m_mods&osix::xx_modCtrl)>0 && (xxev->m_mods&osix::xx_modAlt)==0 &&
        (xxev->m_mods&osix::xx_modShift)==0) {
      aux::Ucsstr str;
      if (xxev->m_xxk==osix::xxk_V)
        pasteCells(&str);
      else
        copyCells(&str,xxev->m_xxk==osix::xxk_X);
    }
    else
      startEditCell(m_focusPos,xxev->m_xxsym);
  }
  else if (xxev->m_xxk==osix::xxk_F2) {
    startEditCell(m_focusPos);
  }
  else {
    if (xxev->m_xxk==osix::xxk_Escape)
      stopEditCell(0);
    else if (xxev->m_xxk==osix::xxk_Enter || xxev->m_xxk==osix::xxk_Return)
      stopEditCell(1);
  }
  xxev->m_consumer|=osix::xx_processed;

  return 0;

}

// **********
int Spreadsheet::keyReleased(osix::xxEvent *xxev) {

  if (osix::navkey(xxev))
    navKeyReleased(xxev);
  else if (osix::typewriterkey(xxev) || xxev->m_xxk==osix::xxk_F2) {

  }
  /*else if (xxev->m_xxk==osix::xxk_F3) {
    SpreadsheetDataItem *chkitm=0;
    aux::TPArr<SpreadsheetDataItem> chkarr;
    int ii=0,chkcnt=0;
    chkcnt=m_data->col(8,&chkarr);
    for (ii=0;ii<chkcnt;ii++) {
      chkitm=chkarr[ii];
      printf ("%d r=%d,c=8 chk=%s\n",__LINE__,
      ii,chkitm ? (const char *)(chkitm->m_text.m_txt.asciistr()) : "null");
    }
    chkcnt=m_data->row(8,&chkarr);
    for (ii=0;ii<chkcnt;ii++) {
      chkitm=chkarr[ii];
      for (ii=0;ii<chkcnt;ii++) {
        chkitm=chkarr[ii];
        printf ("%d r=8,c=%d chk=%s\n",__LINE__,
        ii,chkitm ? (const char *)(chkitm->m_text.m_txt.asciistr()) : "null");
      }
    }
  }*/

  return 0;

}

// **********
int Spreadsheet::navKeyPressed(osix::xxEvent *xxev) {

  Coords focuspos=m_focusPos;
  if (xxev->m_xxk==osix::xxk_Left)
    focuspos.m_col--;
  else if (xxev->m_xxk==osix::xxk_Right)
    focuspos.m_col++;
  else if (xxev->m_xxk==osix::xxk_Up)
    focuspos.m_row--;
  else if (xxev->m_xxk==osix::xxk_Down)
    focuspos.m_row++;
  osix::xxRect rr;
  setFocusPos(focuspos,0,&rr);
  if (m_focusPos!=m_lastFocusPos)
    select(m_focusPos,xxev->m_mods,0,&rr);
  redraw(rr);

  return 0;

}

// **********
int Spreadsheet::navKeyReleased(osix::xxEvent *) {

  return 0;

}

// **********
int Spreadsheet::copyCells(aux::Ucsstr *buf,int cut) {

  *buf=0;
  int ii=0,jj=0;
  Coords lt,rb;
  if (!m_data || m_selection.bounds(&lt,&rb)==0 || !lt.valid() || !rb.valid())
    return -1;
  aux::Ucsstr extbuf;
  buf->reserve((rb.m_row-lt.m_row+1)*(rb.m_col-lt.m_col+1)*128);
  SpreadsheetDataItem *itm=0;
  for (ii=lt.m_row;ii<=rb.m_row;ii++) {
    for (jj=lt.m_col;jj<=rb.m_col;jj++) {
      itm=m_data->data(ii,jj);
      if (itm && itm->m_text.m_txt.len()>0) {
        extbuf=itm->m_text.m_txt;
        if (buf->append(extbuf)==*buf) {
          buf->reserve(2*buf->len()+1);
          buf->append(extbuf);
        }
        if (cut)
          itm->m_text.m_txt=0;
      }
      if (jj<rb.m_col) {
        extbuf="\t";
        if (buf->append(extbuf)==*buf) {
          buf->reserve(2*buf->len()+1);
          buf->append(extbuf);
        }
      }
    }
    if (ii<rb.m_row) {
      extbuf="\n";
      if (buf->append(extbuf)==*buf) {
        buf->reserve(2*buf->len()+1);
        buf->append(extbuf);
      }
    }
  }

  osix::xxtoClipboard(buf);
  if (cut>0) {
    osix::xxRect rr=calcPaintRect(lt,rb,0);
    redraw(rr);
  }

  return 0;

}

// **********
int Spreadsheet::pasteCells(aux::Ucsstr *str) {

  if (!m_data)
    return -1;
  osix::xxfromClipboard(str);
  int ii=0,ll=str->len(),idx=0;
  if (ll==0)
    return -1;
  const unsigned short *buf=str->data();
  SpreadsheetDataItem *itm=0;
  Coords cc=m_focusPos;
  if (!cc.valid())
    cc.set(0,0);
  for (ii=0;ii<ll;ii++) {
    if (buf[ii]==9 || buf[ii]==10 || buf[ii]==13) {
      itm=m_data->data(cc.m_row,cc.m_col);
      if (itm)
        itm->m_text=aux::Ucsstr(ii-idx,&buf[idx]);
      else
        m_data->setData(cc.m_row,cc.m_col,new SpreadsheetDataItem(aux::Ucsstr(ii-idx,&buf[idx])));
      if (str[ii]==9)
        cc.m_col++;
      else {
        cc.m_row++;
        cc.m_col=m_focusPos.m_col;
      }
      idx=ii+1;
    }
  }

  osix::xxRect rr=calcPaintRect(Coords(0,0),Coords(m_data->nrows(),m_data->ncols()),0);
  redraw(rr);

  return 0;

}

} // namespace spreadsheet
