
#include <spreadsheet/spreadsheetdata.h>
#include <stdio.h>

namespace spreadsheet {

int cmpCoords(const void *cc1,const void *cc2) {

  const Coords *c1=(const Coords*)cc1;
  const Coords *c2=(const Coords*)cc2;
  if (c1->m_row<c2->m_row || (c1->m_row==c2->m_row && c1->m_col<c2->m_col))
    return -1;
  if (c2->m_row<c1->m_row || (c1->m_row==c2->m_row && c2->m_col<c1->m_col))
    return 1;
  return 0;

}

int cmpSpreadsheetIndex(const void *ssi1,const void *ssi2) {

  const SpreadsheetIndex *si1=(const SpreadsheetIndex*)ssi1;
  const SpreadsheetIndex *si2=(const SpreadsheetIndex*)ssi2;
  
  if ((si1->m_type&sectiontypeNoHeader)<(si2->m_type&sectiontypeNoHeader))
    return -1;
  if ((si2->m_type&sectiontypeNoHeader)<(si1->m_type&sectiontypeNoHeader))
    return 1;
  return (si1->m_idx<si2->m_idx ? -1 : (si2->m_idx<si1->m_idx ? 1 : 0));
      
}

int SpreadsheetIndex::toString(mk_string str) {

  mk_stringset(str,"idx=");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_i2a(m_idx,numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringset(numstr,0);
  mk_stringappend(str," type=");
  mk_i2a(m_type,numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringset(numstr,0);
  mk_stringappend(str," size=");
  mk_d2a(m_sz,&numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringset(numstr,0);
  mk_stringappend(str," ");
  m_gridstyle.toString(numstr);
  mk_stringappend(str,&numstr[0]);
  return 0;

}

// **********
SpreadsheetDataItem::SpreadsheetDataItem(const SpreadsheetDataItemText &text,
                                         const osix::xxStyle &fgStyle,
                                         const osix::xxStyle &bgStyle) :
  m_text(text),m_selectable(0) {
  mk_stringset(m_editorType,"default");
  m_style[0]=fgStyle;
  m_style[1]=bgStyle;
  m_style[2]=m_style[3]=m_style[4]=m_style[5]=osix::xxStyle();
  
}

// **********
SpreadsheetDataItem::SpreadsheetDataItem(const SpreadsheetDataItem &ass) :
  m_text(ass.m_text),m_selectable(ass.m_selectable) {

  mk_stringset(m_editorType,&ass.m_editorType[0]);
  memcpy(&m_style[0],&ass.m_style[0],6*sizeof(osix::xxStyle));

}

// **********
SpreadsheetDataItem &SpreadsheetDataItem::operator=(const SpreadsheetDataItem &ass) {

  if (this==&ass)
    return *this;
  mk_stringset(m_editorType,&ass.m_editorType[0]);
  m_text=ass.m_text;
  m_selectable=ass.m_selectable;
  memcpy(&m_style[0],&ass.m_style[0],6*sizeof(osix::xxStyle));
  return *this;

}

// **********  
int SpreadsheetDataItem::draw(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc,int selected) {

  osix::xxGC gc=*xxgc;
  osix::xxStyle bg=m_style[1];
  if ((selected&1)>0 && (m_selectable&cellUnselectable)==0 && (m_selectable&cellOwncolor)==0)
    bg=gc.m_bg;
  gc.m_bg=bg;
  osix::xxfillRect(disp,xxdrawable,&gc);
  int ii=0,ww=0;
  int doframe[4]={0,0,0,0};
  double frext[4]={0.,0.,0.,0.};
  for (ii=2;ii<6;ii++) {
    if (!m_style[ii].cantPaint(bg.m_color,1)) {
      ww=(int)m_style[ii].m_width;
      if (ww%2==0)
        ww++;
      doframe[ii-2]=ww;
      frext[ii-2]=(double)ww/2.+1.;
    }
  }
  osix::xxRect rect=gc.m_r;
  mk_vertex lt={rect.left()+frext[0],rect.top()+frext[0],mk_dnan,1.};
  mk_vertex br={rect.left()+frext[0],rect.bottom()-frext[0]+1.,mk_dnan,1.};
  if (doframe[0]>0) {
    gc.m_r=osix::xxRect(lt,br);
    gc.m_fg=m_style[2];
    osix::xxdrawLine(disp,xxdrawable,&gc);
  }
  if (doframe[1]>0) {
    lt[0]=rect.left()+(double)doframe[0]+frext[1];
    lt[1]=rect.top()+frext[1];
    br[0]=rect.right()-(double)doframe[2]-frext[1]+1.;
    br[1]=rect.top()+frext[1];
    gc.m_r=osix::xxRect(lt,br);
    gc.m_fg=m_style[3];
    osix::xxdrawLine(disp,xxdrawable,&gc);
  }
  if (doframe[2]>0) {
    lt[0]=rect.right()-frext[2]+1.;
    lt[1]=rect.top()+frext[2];
    br[0]=rect.right()-frext[2]+1.;
    br[1]=rect.bottom()-frext[2]+1.;
    gc.m_r=osix::xxRect(lt,br);
    gc.m_fg=m_style[4];
    osix::xxdrawLine(disp,xxdrawable,&gc);
  }
  if (doframe[3]>0) {
    lt[0]=rect.left()+(double)doframe[0]+frext[3];
    lt[1]=rect.bottom()-frext[3]+1.;
    br[0]=rect.right()-(double)doframe[2]-frext[3]+1.;
    br[1]=rect.bottom()-frext[3]+1.;
    gc.m_r=osix::xxRect(lt,br);
    gc.m_fg=m_style[5];
    osix::xxdrawLine(disp,xxdrawable,&gc);
  }
  if (m_text.m_txt.length()>0) {
    gc.m_r=rect;
    gc.m_fg=m_style[0];
    gc.m_fnt=m_text.m_fnt;
    osix::xxdrawText(disp,xxdrawable,&gc,&m_text.m_txt,m_text.m_align);
  }

  return 0;

}

// **********
SpreadsheetDataItemGrid::SpreadsheetDataItemGrid(int rows,int cols) :
  m_rows(0),m_cols(0),m_grid(0) {

  rows=MIN(mk_maxarrsz-1,MAX(rows,0));
  cols=MIN(mk_maxarrsz-1,MAX(cols,0));
  if (rows>0 && cols>0) {
    
    m_grid=new SpreadsheetDataItem**[(size_t)m_rows];
    int ii=0;
    for (ii=0;ii<m_rows;ii++) {
      m_grid[ii]=new SpreadsheetDataItem*[(size_t)m_cols];
      memset(&m_grid[ii][0],0,m_cols*sizeof(SpreadsheetDataItem*));
    }
  }

}

// **********
SpreadsheetDataItemGrid::~SpreadsheetDataItemGrid() {

  

}

// **********
int SpreadsheetDataItemGrid::resize(int rr,int cc,int destr) {
    
  rr=MIN((int)mk_ipow2(mk_maxarrsz)-1,MAX(rr,0));
  cc=MIN((int)mk_ipow2(mk_maxarrsz)-1,MAX(cc,0));
  if ((rr==m_rows && cc==m_cols) || (rr*cc)>=mk_maxmem)
    return m_rows*m_cols;
  int ii=0,jj=0;
  if (destr>0 && cc<m_cols && m_grid) {
    for (ii=0;ii<m_rows;ii++) {
      for (jj=cc;jj<m_cols;jj++) {
        if (m_grid[ii][jj])
          delete m_grid[ii][jj];
      }
    }
  }
  if (destr>0 && rr<m_rows && m_grid) {
    for (ii=0;ii<m_cols;ii++) {
      for (jj=rr;jj<m_rows;jj++) {
        if (m_grid[jj][ii])
          delete m_grid[jj][ii];
      }
    }
  }
  if (rr==0 || cc==0) {
    if (m_grid) {
      for (ii=0;ii<m_rows;ii++)
        delete [] m_grid[ii];
      delete [] m_grid;
      m_grid=0;
    }
    m_rows=m_cols=0;
    return 0;
  }
  int saverows=MIN(rr,m_rows),savecols=MIN(cc,m_cols);
  SpreadsheetDataItem ***grid=0;
  if (m_grid) {
    if (saverows>0 && savecols>0)
      grid=new SpreadsheetDataItem**[(size_t)saverows];
    for (ii=0;ii<saverows;ii++) {
      grid[ii]=new SpreadsheetDataItem*[(size_t)savecols];
      for (jj=0;jj<savecols;jj++)
        grid[ii][jj]=m_grid[ii][jj];
    }
    for (ii=0;ii<m_rows;ii++)
      delete [] m_grid[ii];
    delete [] m_grid;
  }
  m_rows=rr;
  m_cols=cc;
  m_grid=new SpreadsheetDataItem**[(size_t)m_rows];
  for (ii=0;ii<m_rows;ii++) {
    m_grid[ii]=new SpreadsheetDataItem*[(size_t)m_cols];
    memset(&m_grid[ii][0],0,m_cols*sizeof(SpreadsheetDataItem*));
  }
  if (grid) {
    for (ii=0;ii<saverows;ii++) {
      for (jj=0;jj<savecols;jj++)
        m_grid[ii][jj]=grid[ii][jj];
      delete [] grid[ii];
    }
    delete [] grid;
  }
  return m_rows*m_cols;
}

// **********
SpreadsheetDataItem *SpreadsheetDataItemGrid::getItem(int row,int col) {

  if (row<0 || row>=m_rows || col<0 || col>=m_cols)
    return 0;
  return m_grid[row][col];

};

// **********
SpreadsheetDataItem *SpreadsheetDataItemGrid::setItem(int row,int col,SpreadsheetDataItem *itm) {

  if (row<0 || col<0)
    return 0;
  int rr=m_rows,cc=m_cols;
  if (row>=m_rows)
    rr=row+1;
  if (col>=m_cols) 
    cc=col+1;
  resize(rr,cc);
  SpreadsheetDataItem *curritm=m_grid[row][col];
  m_grid[row][col]=itm;
  return curritm;

};

// **********
SpreadsheetData::SpreadsheetData(int rows,int cols) :
    m_bgStyle(defBgStyle),m_fgStyle(defFgStyle),m_defHeight(defHeight),m_defWidth(defWidth) {

  mk_listalloc(&m_descr,sizeof(SpreadsheetIndex),0);
  m_descr.cmp=cmpSpreadsheetIndex;
  setDimension(rows,cols);

}

// **********
SpreadsheetData::~SpreadsheetData() {

  mk_listfree(&m_descr);
  m_data.resize(0,0,true);
  
}

// **********
SpreadsheetDataItem *SpreadsheetData::setData(int row,int col,SpreadsheetDataItem *nitm,bool destr) {

  if (row<0 || col<0 || row>=m_data.rows() || col>=m_data.cols())
    return 0;
  SpreadsheetDataItem *itm=m_data.setItem(row,col,nitm);
  if (destr && itm)
    delete itm;
  return (destr ? 0 : itm);

}

// **********
SpreadsheetDataItem *SpreadsheetData::data(int row,int col) {

  if (row<0 || col<0 || row>=m_data.rows() || col>=m_data.cols())
    return 0;
  return m_data.getItem(row,col);

}

// **********
int SpreadsheetData::setIndexDescr(SpreadsheetIndex section_) {

  SpreadsheetIndex section(section_),descr;
  int ii=0,idxl=-1,res=mk_listfind(&m_descr,(void*)&section,&idxl,0);
  if (res==0 || idxl<0)
    return 0;
  mk_listat(&m_descr,idxl,(void*)&descr);
  descr.m_type=section.m_type;
  if (!mk_isbusted(section.m_sz) && section.m_sz>.0)
    descr.m_sz=section.m_sz;
  descr.m_gridstyle=section.m_gridstyle;
  if ((section.isStatic() && descr.isStatic()) ||
      (!section.isStatic() && !descr.isStatic())) {
    mk_listsetat(&m_descr,(void*)&descr,idxl,0);
    return 1;
  }
  int cnt=((section.m_type&sectiontypeRow)>0 ? m_data.rows() : m_data.cols());
  int firststatic=-1,laststatic=cnt,idx=section.m_idx;
  SpreadsheetIndex si;
  for (ii=0;ii<cnt;ii++) {
    section.m_idx=ii;
    mk_listfind(&m_descr,(void*)&section,&idxl,0);
    mk_listat(&m_descr,idxl,(void*)&si);
    if (!si.isStatic())
      break;
    firststatic=ii;
  }
  for (ii=cnt-1;ii>=0;ii--) {
    section.m_idx=ii;
    mk_listfind(&m_descr,(void*)&section,&idxl,0);
    mk_listat(&m_descr,idxl,(void*)&si);
    if (!si.isStatic())
      break;
    laststatic=ii;
  }
  res=1;
  if (firststatic==idx || firststatic==idx-1) {
    if (section.isStatic())
      descr.m_type|=sectiontypeStatic;
    else
      descr.m_type&=sectiontypeNoStatic;
  }
  else if (laststatic==idx || laststatic==idx+1) {
    if (section.isStatic())
      descr.m_type|=(sectiontypeHeader|sectiontypeStatic);
    else
      descr.m_type&=sectiontypeNoStatic;
  }
  else
    res=2;
  mk_listsetat(&m_descr,(void*)&descr,idxl,0);
  return res;
  
}

// **********
int SpreadsheetData::indexDescr(SpreadsheetIndex *descr) const {

  if (!descr)
    return -1;
  SpreadsheetIndex fnd(*descr);
  int idxl=-1,res=mk_listfind(&m_descr,(void*)&fnd,&idxl,0);
  if (res==0) {
    descr->m_type&=sectiontypeNoHeader;
    descr->m_sz=mk_dnan;
    res=-1;
  }
  else {
    mk_listat(&m_descr,idxl,(void*)&fnd);
    descr->m_type=fnd.m_type;
    descr->m_sz=fnd.m_sz;
    res=descr->m_idx;
  }
  return res;

}

// **********
int SpreadsheetData::setDimension(int rr,int cc,bool destr) {

  int ii=0,oldrows=m_data.rows(),oldcols=m_data.cols();
  if (rr<0)
    rr=oldrows;
  else if (rr>=maxNumRows-1)
    rr=maxNumRows-1;
  if(cc<0)
    cc=oldcols;
  else if (cc>=maxNumCols-1)
    cc=maxNumCols-1;
  if (rr*cc>maxDimension)
    return (oldrows*oldcols);
  if (rr==oldrows && cc==oldcols)
    return (rr*cc);
  m_data.resize(rr,cc,destr);
  rr=m_data.rows();
  cc=m_data.cols();
  int idxl=-1,cntlaststaticr=0,cntlaststaticc=0;
  SpreadsheetIndex descr;
  for (ii=oldrows-1;ii>=0;ii--) {
    descr.set(sectiontypeRow,ii);
    if (mk_listfind(&m_descr,(void*)&descr,&idxl,0)==0)
      break;
    mk_listat(&m_descr,idxl,(void*)&descr);
    if (!descr.isStatic())
      break;
    cntlaststaticr++;
    descr.m_type&=sectiontypeNoStatic;
    mk_listsetat(&m_descr,(void*)&descr,idxl,0);
  }
  if (cntlaststaticr>rr)
    cntlaststaticr=rr;
  for (ii=0;ii<rr;ii++) {
    descr.set(sectiontypeRow,ii);
    if (mk_listfind(&m_descr,(void*)&descr,&idxl,0)==0)
      break;
    mk_listat(&m_descr,idxl,(void*)&descr);
    if (!descr.isStatic())
      break;
    if (ii+cntlaststaticr>=rr)
      cntlaststaticr--;
  }
  for (ii=oldcols-1;ii>=0;ii--) {
    descr.set(sectiontypeCol,ii);
    if (mk_listfind(&m_descr,(void*)&descr,&idxl,0)==0)
      break;
    mk_listat(&m_descr,idxl,(void*)&descr);
    if (!descr.isStatic())
      break;
    cntlaststaticc++;
    descr.m_type&=sectiontypeNoStatic;
    mk_listsetat(&m_descr,(void*)&descr,idxl,0);
  }
  if (cntlaststaticc>cc)
    cntlaststaticc=cc;
  for (ii=0;ii<cc;ii++) {
    descr.set(sectiontypeCol,ii);
    if (mk_listfind(&m_descr,(void*)&descr,&idxl,0)==0)
      break;
    mk_listat(&m_descr,idxl,(void*)&descr);
    if (!descr.isStatic())
      break;
    if (ii+cntlaststaticc>=cc)
      cntlaststaticc--;
  }
  mk_listrealloc(&m_descr,rr*cc);
  for (ii=oldrows;ii<rr;ii++) {
    descr.set(sectiontypeRow,ii,m_defHeight);
    mk_listinsort(&m_descr,(void*)&descr);
  }
  for (ii=oldcols;ii<cc;ii++) {
    descr.set(sectiontypeCol,ii,m_defWidth);
    mk_listinsort(&m_descr,(void*)&descr);
  }
  for (ii=0;ii<cntlaststaticr;ii++) {
    descr.set(sectiontypeRow,rr-ii-1);
    if (mk_listfind(&m_descr,(void*)&descr,&idxl,0)>0) {
      mk_listat(&m_descr,idxl,(void*)&descr);
      descr.m_type|=(sectiontypeHeader|sectiontypeStatic);
      mk_listsetat(&m_descr,(void*)&descr,idxl,0);
    }
  }
  for (ii=0;ii<cntlaststaticc;ii++) {
    descr.set(sectiontypeCol,cc-ii-1);
    if (mk_listfind(&m_descr,(void*)&descr,&idxl,0)>0) {
      mk_listat(&m_descr,idxl,(void*)&descr);
      descr.m_type|=(sectiontypeHeader|sectiontypeStatic);
      mk_listsetat(&m_descr,(void*)&descr,idxl,0);
    }
  }
  
  return (rr*cc);

}

// **********
int SpreadsheetData::drawCell(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc,
  int row,int col,int selected) {

  if (row<0 || col<0 || row>=m_data.rows() || col>=m_data.cols())
    return -1;

  SpreadsheetDataItem *itm=m_data.getItem(row,col);
  SpreadsheetIndex descr(sectiontypeRow,row);
  int idxl=-1;
  if (mk_listfind(&m_descr,(void*)&descr,&idxl,0)==0) {
    descr.set(sectiontypeCol,col);
    mk_listfind(&m_descr,(void*)&descr,&idxl,0);
  }
  mk_listat(&m_descr,idxl,(void*)&descr);
//printf ("r=%d c=%d itm=%ld type=%d\n",row,col,(long)itm,descr ? descr->m_type : 0);
  osix::xxGC gc=*xxgc;
  if (itm)
    gc.m_fnt=itm->m_text.m_fnt;
  if (descr.isHeader()) {
    if ((selected&2)>0)
      gc.m_fnt.m_style|=osix::xx_fntBold;
    osix::xxdrawCtrl(disp,xxdrawable,&gc,
                     (selected&1)>0 ? osix::xx_pushbutton_sunken : osix::xx_pushbutton_raised,
                     itm ? &itm->m_text.m_txt : 0,132);
  }
  else {
    if (itm)
      itm->draw(disp,xxdrawable,&gc,selected);
    else {
      if ((selected&1)==0)
        gc.m_bg=m_bgStyle;
      osix::xxfillRect(disp,xxdrawable,&gc);
    }
  }

  return 0;

}

// **********
int SpreadsheetData::drawGridLine(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc,
  int type,int section) {

  osix::xxStyle gridstyle=defGridStyle;
  SpreadsheetIndex descr(type,section);
  int idxl=-1;
  if (mk_listfind(&m_descr,(void*)&descr,&idxl,0)>0) {
    mk_listat(&m_descr,idxl,(void*)&descr);
    gridstyle=descr.m_gridstyle;
    if (gridstyle.m_width>1.) {
      if ((type&sectiontypeRow)>0)
        xxgc->m_r.setRight(xxgc->m_r.right()-gridstyle.m_width/2.-1.);
      else if ((type&sectiontypeCol)>0)
        xxgc->m_r.setBottom(xxgc->m_r.bottom()-gridstyle.m_width/2.-1.);
    }
  }
  xxgc->m_fg=gridstyle;
  osix::xxdrawLine(disp,xxdrawable,xxgc);

  return 0;

}

int SpreadsheetData::toString(mk_string str,bool dodescr,bool) {

  if (!dodescr)
    return 0; //m_descr.count;
  mk_string idxstr;
  int ii=0;
  SpreadsheetIndex descr;
  for (ii=0;ii<m_descr.count;ii++) {
    mk_listat(&m_descr,ii,(void*)&descr);
    mk_stringset(idxstr,0);
    descr.toString(idxstr);
    mk_stringappend(str,&idxstr[0]);
  }
  return 0;

}

} // namespace spreadsheet
