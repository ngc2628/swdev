
#ifndef _spreadsheetdata_
#define _spreadsheetdata_

#include <mkbase/mkconv.h>
#include <auxx/auxx.h>
#include <osix/xxstyle.h>
#include <osix/xxshape.h>
#include <osix/xxtxt.h>
#include <osix/xxpaint.h>

namespace spreadsheet {

const int maxNumRows=32768;
const int maxNumCols=32768;
const int maxDimension=1048576;
const int nullrow=0;
const int nullcol=0;
const double defWidth=100.;
const double defHeight=24.;
const unsigned int defNoColor=0U; // black
const unsigned int defFgColor=4278190080U; // black
const unsigned int defBgColor=4294638330U; // snow
const unsigned int defGridColor=4289703855U; // grey
const unsigned int tstColor=4284782180U; // greenish
const osix::xxStyle defBgStyle(defBgColor,1,0);
const osix::xxStyle defFgStyle(defFgColor,1,1);
const osix::xxStyle defGridStyle(defGridColor,3,1);
const osix::xxStyle tstStyle(tstColor,1,0);
const osix::xxFnt defFnt("Arial",10.);
const int sectiontypeRow=1;
const int sectiontypeCol=2;
const int sectiontypeNoHeader=3;
const int sectiontypeHeader=4;
const int sectiontypeNoStatic=7;
const int sectiontypeStatic=8;
const int sectiontype=15;
const int cellSelected=1;
const int cellUnselectable=2;
const int cellOwncolor=4;

class oswinexp Coords {

  public:
    int m_row;
    int m_col;
    Coords(int row=-1,int col=-1) : m_row(row),m_col(col) {
    }
    Coords(const Coords &ass) : m_row(ass.m_row),m_col(ass.m_col) {
    }
    ~Coords() {
    }
    Coords &operator=(const Coords &ass) {
      m_row=ass.m_row;
      m_col=ass.m_col;
      return *this;
    }
    bool operator==(const Coords &cmp) const {
      return (m_row==cmp.m_row && m_col==cmp.m_col);
    }
    bool operator!=(const Coords &cmp) const {
      return (m_row!=cmp.m_row || m_col!=cmp.m_col);
    }
    bool operator<(const Coords &cmp) const {
      return (m_row<cmp.m_row || (m_row==cmp.m_row && m_col<cmp.m_col));
    }
    bool valid() const {
      return (m_row>=0 && m_col>=0);
    }
    int set(int row=-1,int col=-1) {
      m_row=row;
      m_col=col;
      return 0;
    }

};

class oswinexp SpreadsheetDataItemText {

  public:
    aux::Ucsstr m_txt;
    osix::xxFnt m_fnt;
    int m_align;
    SpreadsheetDataItemText(const aux::Ucsstr &txt=aux::Ucsstr(),
                            osix::xxFnt fnt=osix::xxFnt(),
                            int align=(osix::xx_alignhcenter|osix::xx_alignvcenter)) :
      m_txt(txt),m_fnt(fnt),m_align(align) {
    }
    SpreadsheetDataItemText(const SpreadsheetDataItemText &ass) :
      m_txt(ass.m_txt),m_fnt(ass.m_fnt),m_align(ass.m_align) {
    }
    ~SpreadsheetDataItemText() {
    }
    SpreadsheetDataItemText &operator=(const SpreadsheetDataItemText &ass) {
      if (this==&ass)
        return *this;
      m_txt=ass.m_txt;
      m_fnt=ass.m_fnt;
      m_align=ass.m_align;
      return *this;
    }
    bool operator==(SpreadsheetDataItemText &cmp) const {
      return (m_txt==cmp.m_txt && m_fnt==cmp.m_fnt && m_align==cmp.m_align);
    }
    bool operator<(SpreadsheetDataItemText &cmp) const {
      return (m_txt<cmp.m_txt || (m_txt==cmp.m_txt && (m_fnt<cmp.m_fnt || (m_fnt==cmp.m_fnt && m_align<cmp.m_align))));
    }

};

class oswinexp SpreadsheetDataItem {

  public:
    mk_string m_editorType;
    SpreadsheetDataItemText m_text;
    int m_selectable;
    osix::xxStyle m_style[6]; // fg,bg,frleft,frtop,frright,frbottom
    SpreadsheetDataItem(const SpreadsheetDataItemText &text=SpreadsheetDataItemText(),
                        const osix::xxStyle &fg=defFgStyle,
                        const osix::xxStyle &bg=defBgStyle);
    SpreadsheetDataItem(const SpreadsheetDataItem &);
    virtual ~SpreadsheetDataItem() {
    }
    SpreadsheetDataItem &operator=(const SpreadsheetDataItem &);
    bool operator==(const SpreadsheetDataItem &cmp) const {
       return (this==&cmp);
    }
    bool operator<(const SpreadsheetDataItem &cmp) const {
      return ((mk_ulreal)this<(mk_ulreal)&cmp);
    }
    virtual int draw(void *,osix::xxDrawable *,osix::xxGC *,int);

};

class oswinexp SpreadsheetIndex {

  public:
    int m_type;
    int m_idx;
    double m_sz;
    osix::xxStyle m_gridstyle;
    SpreadsheetIndex(int type=0,int idx=0,double sz=mk_dnan,osix::xxStyle gridstyle=defGridStyle) :
      m_type(type),m_idx(idx),m_sz(sz),m_gridstyle(gridstyle) {
    }
    SpreadsheetIndex(const SpreadsheetIndex &ass) :
      m_type(ass.m_type),m_idx(ass.m_idx),m_sz(ass.m_sz),m_gridstyle(ass.m_gridstyle) {
    }
    ~SpreadsheetIndex() {
    }
    SpreadsheetIndex &operator=(const SpreadsheetIndex &ass) {
      m_type=ass.m_type;
      m_idx=ass.m_idx;
      m_sz=ass.m_sz;
      m_gridstyle=ass.m_gridstyle;
      return *this;
    }
    bool operator==(const SpreadsheetIndex &cmp) const {
      return ((m_type&sectiontypeNoHeader)==(cmp.m_type&sectiontypeNoHeader) && m_idx==cmp.m_idx);
    }
    bool operator<(const SpreadsheetIndex &cmp) const {
      return ((m_type&sectiontypeNoHeader)<(cmp.m_type&sectiontypeNoHeader) ||
              ((m_type&sectiontypeNoHeader)==(cmp.m_type&sectiontypeNoHeader) && m_idx<cmp.m_idx));
    }
    bool isHeader() const {
      return ((m_type&sectiontypeHeader)>0);
    }
    bool isStatic() const {
      return ((m_type&sectiontypeStatic)>0);
    }
    int set(int type=0,int idx=0,double sz=mk_dnan,osix::xxStyle gridstyle=defGridStyle) {
      m_type=type;
      m_idx=idx;
      m_sz=sz;
      m_gridstyle=gridstyle;
      return 0;
    }
    int toString(mk_string);

};

class oswinexp SpreadsheetData {

  public:
    osix::xxStyle m_bgStyle;
    osix::xxStyle m_fgStyle;
    SpreadsheetData(int nrows=0,int ncols=0);
    virtual ~SpreadsheetData();
    virtual int nrows() {
      return m_data.rows();
    }
    virtual int ncols() {
      return m_data.cols();
    }
    virtual int setDimension(int,int,bool destr=true);
    virtual SpreadsheetDataItem *setData(int,int,SpreadsheetDataItem *,bool destr=true);
    virtual SpreadsheetDataItem *data(int,int);
    virtual int row(int rr,aux::TPArr<SpreadsheetDataItem> *rL) {
      return m_data.row(rr,rL);
    }
    virtual int col(int cc,aux::TPArr<SpreadsheetDataItem> *cL) {
      return m_data.col(cc,cL);
    }
    virtual int setIndexDescr(SpreadsheetIndex);
    virtual int indexDescr(SpreadsheetIndex *) const;
    virtual int drawCell(void *,osix::xxDrawable *,osix::xxGC *,int,int,int);
    virtual int drawGridLine(void *,osix::xxDrawable *,osix::xxGC *,int,int);
    virtual int toString(mk_string,bool descr=true,bool data=true);

  protected:
    SpreadsheetData(const SpreadsheetData &) {
    }
    SpreadsheetData &operator=(const SpreadsheetData &) {
      return *this;
    }
    aux::TVList<SpreadsheetIndex> m_descr;
    aux::TPArr2d<SpreadsheetDataItem> m_data;
    double m_defHeight;
    double m_defWidth;

};

} // namespace spreadsheet

#endif
