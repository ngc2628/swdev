
#include <spreadsheet/spreadsheetdata.h>
#include <stdio.h>

namespace spreadsheet {

void SpreadsheetIndex::toString(aux::Asciistr *str) {

  *str=0;
  str->reserve(100);
  aux::Asciistr numstr;
  str->append("idx=");
  aux::i2a(m_idx,&numstr);
  str->append(numstr);
  numstr=0;
  str->append(" type=");
  aux::i2a(m_type,&numstr);
  str->append(numstr);
  numstr=0;
  str->append(" size=");
  aux::d2a(m_sz,&numstr);
  str->append(numstr);
  numstr=0;
  str->append(" ");
  m_gridstyle.toString(&numstr);
  str->append(numstr);

}

// **********
SpreadsheetDataItem::SpreadsheetDataItem(const SpreadsheetDataItemText &text,
                                         const osix::xxStyle &fgStyle,
                                         const osix::xxStyle &bgStyle) :
  m_editorType("default"),m_text(text),m_selectable(0) {
  m_style[0]=fgStyle;
  m_style[1]=bgStyle;
  m_style[2]=m_style[3]=m_style[4]=m_style[5]=osix::xxStyle();
  
}

// **********
SpreadsheetDataItem::SpreadsheetDataItem(const SpreadsheetDataItem &ass) :
  m_editorType(ass.m_editorType),m_text(ass.m_text),m_selectable(ass.m_selectable) {

  memcpy(&m_style[0],&ass.m_style[0],6*sizeof(osix::xxStyle));

}

// **********
SpreadsheetDataItem &SpreadsheetDataItem::operator=(const SpreadsheetDataItem &ass) {

  if (this==&ass)
    return *this;
  m_editorType=ass.m_editorType;
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
  aux::Asciistr dbgstr;
  if (doframe[0]>0) {
    gc.m_r=osix::xxRect(aux::Vector3(rect.left()+frext[0],rect.top()+frext[0]),
                        aux::Vector3(rect.left()+frext[0],rect.bottom()-frext[0]+1.));
    gc.m_fg=m_style[2];
    osix::xxdrawLine(disp,xxdrawable,&gc);
  }
  if (doframe[1]>0) {
    gc.m_r=osix::xxRect(aux::Vector3(rect.left()+(double)doframe[0]+frext[1],rect.top()+frext[1]),
                        aux::Vector3(rect.right()-(double)doframe[2]-frext[1]+1.,rect.top()+frext[1]));
    gc.m_fg=m_style[3];
    osix::xxdrawLine(disp,xxdrawable,&gc);
  }
  if (doframe[2]>0) {
    gc.m_r=osix::xxRect(aux::Vector3(rect.right()-frext[2]+1.,rect.top()+frext[2]),
                        aux::Vector3(rect.right()-frext[2]+1.,rect.bottom()-frext[2]+1.));
    gc.m_fg=m_style[4];
    osix::xxdrawLine(disp,xxdrawable,&gc);
  }
  if (doframe[3]>0) {
    gc.m_r=osix::xxRect(aux::Vector3(rect.left()+(double)doframe[0]+frext[3],rect.bottom()-frext[3]+1.),
                        aux::Vector3(rect.right()-(double)doframe[2]-frext[3]+1.,rect.bottom()-frext[3]+1.));
    gc.m_fg=m_style[5];
    osix::xxdrawLine(disp,xxdrawable,&gc);
  }
  if (m_text.m_txt.len()>0) {
    gc.m_r=rect;
    gc.m_fg=m_style[0];
    gc.m_fnt=m_text.m_fnt;
    osix::xxdrawText(disp,xxdrawable,&gc,&m_text.m_txt,m_text.m_align);
  }

  return 0;

}

// **********
SpreadsheetData::SpreadsheetData(int rows,int cols) :
    m_bgStyle(defBgStyle),m_fgStyle(defFgStyle),m_defHeight(defHeight),m_defWidth(defWidth) {

  setDimension(rows,cols);

}

// **********
SpreadsheetData::~SpreadsheetData() {

  m_data.resize(0,0,true);
  
}

// **********
SpreadsheetDataItem *SpreadsheetData::setData(int row,int col,SpreadsheetDataItem *nitm,bool destr) {

  if (row<0 || col<0 || row>=m_data.rows() || col>=m_data.cols())
    return 0;
  SpreadsheetDataItem *itm=m_data.set(row,col,nitm);
  if (destr && itm)
    delete itm;
  return (destr ? 0 : itm);

}

// **********
SpreadsheetDataItem *SpreadsheetData::data(int row,int col) {

  if (row<0 || col<0 || row>=m_data.rows() || col>=m_data.cols())
    return 0;
  return m_data.at(row,col);

}

// **********
int SpreadsheetData::setIndexDescr(SpreadsheetIndex section) {

  SpreadsheetIndex *descr=m_descr.at(m_descr.find(section));
  if (!descr)
    return 0;
  descr->m_type=section.m_type;
  if (!mk_isbusted(section.m_sz) && section.m_sz>.0)
    descr->m_sz=section.m_sz;
  descr->m_gridstyle=section.m_gridstyle;
  if ((section.isStatic() && descr->isStatic()) ||
      (!section.isStatic() && !descr->isStatic()))
    return 1;
  int ii=0,cnt=((section.m_type&1)>0 ? m_data.rows() : m_data.cols());
  int firststatic=-1,laststatic=cnt,idx=section.m_idx;
  SpreadsheetIndex *si=0;
  for (ii=0;ii<cnt;ii++) {
    section.m_idx=ii;
    si=m_descr.at(m_descr.find(section));
    if (!si || !si->isStatic())
      break;
    firststatic=ii;
  }
  for (ii=cnt-1;ii>=0;ii--) {
    section.m_idx=ii;
    si=m_descr.at(m_descr.find(section));
    if (!si || !si->isStatic())
      break;
    laststatic=ii;
  }
  if (firststatic==idx || firststatic==idx-1) {
    if (section.isStatic())
      descr->m_type|=sectiontypeStatic;
    else
      descr->m_type&=sectiontypeNoStatic;
  }
  else if (laststatic==idx || laststatic==idx+1) {
    if (section.isStatic())
      descr->m_type|=(sectiontypeHeader|sectiontypeStatic);
    else
      descr->m_type&=sectiontypeNoStatic;
  }
  else
    return 2;
  return 1;
  
}

// **********
int SpreadsheetData::indexDescr(SpreadsheetIndex *descr) const {

  if (!descr)
    return -1;
  const SpreadsheetIndex *fnd=m_descr.at(m_descr.find(*descr));
  if (!fnd) {
    descr->m_type&=sectiontypeNoHeader;
    descr->m_sz=mk_dnan;
    return -1;
  }
  descr->m_type=fnd->m_type;
  descr->m_sz=fnd->m_sz;
  return descr->m_idx;

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
  int cntlaststaticr=0,cntlaststaticc=0;
  SpreadsheetIndex fnd;
  SpreadsheetIndex *descr=0;
  for (ii=oldrows-1;ii>=0;ii--) {
    fnd.set(sectiontypeRow,ii);
    descr=m_descr.at(m_descr.find(fnd));
    if (!descr || !descr->isStatic())
      break;
    cntlaststaticr++;
    descr->m_type&=sectiontypeNoStatic;
  }
  if (cntlaststaticr>rr)
    cntlaststaticr=rr;
  for (ii=0;ii<rr;ii++) {
    fnd.set(sectiontypeRow,ii);
    descr=m_descr.at(m_descr.find(fnd));
    if (!descr || !descr->isStatic())
      break;
    if (ii+cntlaststaticr>=rr)
      cntlaststaticr--;
  }
  for (ii=oldcols-1;ii>=0;ii--) {
    fnd.set(sectiontypeCol,ii);
    descr=m_descr.at(m_descr.find(fnd));
    if (!descr || !descr->isStatic())
      break;
    cntlaststaticc++;
    descr->m_type&=sectiontypeNoStatic;
  }
  if (cntlaststaticc>cc)
    cntlaststaticc=cc;
  for (ii=0;ii<cc;ii++) {
    fnd.set(sectiontypeCol,ii);
    descr=m_descr.at(m_descr.find(fnd));
    if (!descr || !descr->isStatic())
      break;
    if (ii+cntlaststaticc>=cc)
      cntlaststaticc--;
  }
  m_descr.resize(rr*cc);
  for (ii=oldrows;ii<rr;ii++) {
    fnd.set(sectiontypeRow,ii,m_defHeight);
    m_descr.inSort(fnd);
  }
  for (ii=oldcols;ii<cc;ii++) {
    fnd.set(sectiontypeCol,ii,m_defWidth);
    m_descr.inSort(fnd);
  }
  for (ii=0;ii<cntlaststaticr;ii++) {
    fnd.set(sectiontypeRow,rr-ii-1);
    descr=m_descr.at(m_descr.find(fnd));
    if (descr)
      descr->m_type|=(sectiontypeHeader|sectiontypeStatic);
  }
  for (ii=0;ii<cntlaststaticc;ii++) {
    fnd.set(sectiontypeCol,cc-ii-1);
    descr=m_descr.at(m_descr.find(fnd));
    if (descr)
      descr->m_type|=(sectiontypeHeader|sectiontypeStatic);
  }
  
  return (rr*cc);

}

// **********
int SpreadsheetData::drawCell(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc,
  int row,int col,int selected) {

  if (row<0 || col<0 || row>=m_data.rows() || col>=m_data.cols())
    return -1;

  SpreadsheetDataItem *itm=m_data.at(row,col);
  SpreadsheetIndex fnd(sectiontypeRow,row);
  SpreadsheetIndex *descr=m_descr.at(m_descr.find(fnd));
  if (!descr || !descr->isHeader()) {
    fnd.set(sectiontypeCol,col);
    descr=m_descr.at(m_descr.find(fnd));
  }
//printf ("r=%d c=%d itm=%ld type=%d\n",row,col,(long)itm,descr ? descr->m_type : 0);
  osix::xxGC gc=*xxgc;
  if (itm)
    gc.m_fnt=itm->m_text.m_fnt;
  if (descr && descr->isHeader()) {
    if ((selected&2)>0)
      gc.m_fnt.m_style|=osix::xx_fntBold;
    osix::xxdrawCtrl(disp,xxdrawable,&gc,
                     (selected&1)>0 ? osix::xx_pushbutton_sunken : osix::xx_pushbutton_raised,
                     itm ? &(itm->m_text.m_txt) : 0,132);
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
  SpreadsheetIndex fnd(type,section);
  SpreadsheetIndex *descr=m_descr.at(m_descr.find(fnd));
  if (descr) {
    gridstyle=descr->m_gridstyle;
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

void SpreadsheetData::toString(aux::Asciistr *str,bool descr,bool) {

  *str=0;
  str->reserve(10000000);
  aux::Asciistr idxstr;
  int ii=0;
  if (descr) {
    for (ii=0;ii<m_descr.count();ii++) {
      m_descr.at(ii)->toString(&idxstr);
      str->append(idxstr);
    }
  }

}

} // namespace spreadsheet
