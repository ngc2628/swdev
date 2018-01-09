
#ifndef _spreadsheet_
#define _spreadsheet_

#include <auxx/auxx.h>
#include <osix/xxevent.h>
#include <osix/xxpaint.h>
#include <spreadsheet/spreadsheetdata.h>

namespace spreadsheet {

const int maxNumViewRows=256;
const int maxNumViewCols=256;
const int autoscroll=1;
const int scrollInterval=100;
const int hlCell=1;
const int hlFocus=2;

const unsigned int defSelectedColor=4278190335U; // blue
const osix::xxStyle defSelectedStyle(defSelectedColor,1,0);

class Spreadsheet;

class PosRect {

  public:
    Coords m_pos;
    osix::xxRect m_rect;
    PosRect(Coords pos=Coords(),osix::xxRect rect=osix::xxRect()) : m_pos(pos),m_rect(rect) {
    }
    PosRect(const PosRect &ass) : m_pos(ass.m_pos),m_rect(ass.m_rect) {
    }
    ~PosRect() {
    }
    PosRect &operator=(const PosRect &ass) {
      m_pos=ass.m_pos;
      m_rect=ass.m_rect;
      return *this;
    }
    bool operator==(const PosRect &cmp) const {
      return (m_pos==cmp.m_pos);
    }
    bool operator<(const PosRect &cmp) const {
      return (m_pos<cmp.m_pos);
    }

};

inline int cmpCoordsRow(const void *c1,const void *c2) {

  if (((const Coords *)c1)->m_row<((const Coords *)c2)->m_row)
    return -1;
  if (((const Coords *)c2)->m_row<((const Coords *)c1)->m_row)
    return 1;
  return 0;

}

inline int cmpCoordsCol(const void *c1,const void *c2) {

  if (((const Coords *)c1)->m_col<((const Coords *)c2)->m_col)
    return -1;
  if (((const Coords *)c2)->m_col<((const Coords *)c1)->m_col)
    return 1;
  return 0;

}

class oswinexp SpreadsheetSelection {

  public:
    aux::TVList<Coords> m_list;
    int m_isRect;
    Coords m_anchor;
    SpreadsheetSelection() : m_isRect(0) {
    }
    SpreadsheetSelection(const SpreadsheetSelection &ass) :
      m_list(ass.m_list),m_isRect(ass.m_isRect),m_anchor(ass.m_anchor) {
    }
    virtual ~SpreadsheetSelection() {
    }
    SpreadsheetSelection &operator=(const SpreadsheetSelection &ass) {
      m_list=ass.m_list;
      m_isRect=ass.m_isRect;
      m_anchor=ass.m_anchor;
      return *this;
    }
    bool operator==(const SpreadsheetSelection &) const;
    bool operator<(const SpreadsheetSelection &) const;
    int count() const;
    int bounds(Coords *,Coords *) const;
    int setRect(Coords,Coords);
    int clear(int mod=7);
    int resolve();

};

class oswinexp Spreadsheet {

  public:
    osix::xxDrawable m_winscr,m_pixscr;
    osix::xxStyle m_bgStyle;
    osix::xxStyle m_focusStyle;
    Spreadsheet(const char *name=0);
    virtual ~Spreadsheet();
    virtual SpreadsheetData *setSpreadsheetData(SpreadsheetData *,bool remove=true);
    virtual Coords cellAt(aux::Vector3,int *dir=0) const;
    virtual osix::xxRect cellRect(Coords,Coords *vpos=0) const;
    virtual Coords setPos(Coords pos) {
      return setPos(pos,1,0,false);
    }
    Coords pos() const {
      return m_actPos;
    }
    Coords focusPos() const {
      return m_focusPos;
    }
    virtual Coords setFocusPos(Coords pos) {
      return setFocusPos(pos,1,0);
    }
    virtual int cellVisible(Coords) const;
    virtual int updateGeometry() {
      return 0;
    }
    virtual int setSelection(aux::TVList<Coords>,int mod=0); // 0:set 1:add 2:remove
    virtual int selection(aux::TVList<Coords>*,int *iRect=0) const;
    virtual int isSelected(Coords s) const;
    virtual int isScrolling() const {
      return 0;
    }
    virtual int stopScrolling() {
      return 0;
    }
    virtual int startScrolling() {
      return 0;
    }
    virtual int isEditing(Coords *pos=0) const;

    virtual int redraw(osix::xxRect rr=osix::xxRect(),int flag=0);
    virtual void *findDisplay() {
      return 0;
    }
    virtual int osEvent(void *,void *);
    virtual int osUpdate(osix::xxRect rr=osix::xxRect()) {
      return 0;
    }

  protected:
    Coords m_actPos;
    Coords m_lastPos;
    SpreadsheetData *m_data;
    Coords m_focusPos;
    Coords m_lastFocusPos;
    aux::TVArr2d<PosRect> m_rects; // view rects
    SpreadsheetSelection m_selection;
    osix::xxEvent m_xxlastinputev;

    virtual int calcGeometry(osix::xxRectSize);
    virtual int calcPositions();
    virtual int validView(int *rows=0,int *cols=0) const;
    virtual Coords nextVisible(Coords coords) const;
    virtual osix::xxRect calcPaintRect(Coords,Coords,osix::xxRect *);
    virtual int selRect(Coords *,Coords *) const;
    virtual int paintCells(void *,osix::xxDrawable *,osix::xxGC *);
    virtual Coords setPos(Coords,int,osix::xxRect *,int fromslider=0);
    virtual Coords setFocusPos(Coords,int,osix::xxRect *);
    virtual int select(Coords,int,int,osix::xxRect *,int mode=0); // char= 0:new 1:add 2:unite
    virtual int createEditor(aux::Ucsstr) {
      return 0;
    }
    virtual int startEditCell(Coords,int sym=0);
    virtual int stopEditCell(int) {
      return -1;
    }
    virtual int copyCells(aux::Ucsstr *,int);
    virtual int pasteCells(aux::Ucsstr *);
    virtual int scrollAction(int,aux::Vector3);
    virtual int adjustSliders(int,int,int,int,int,int) {
      return 0;
    }
    virtual int adjustEditor() {
      return 0;
    }
    virtual int inputEvent(osix::xxEvent *);
    virtual int mouseMoved(osix::xxEvent *);
    virtual int keyPressed(osix::xxEvent *);
    virtual int keyReleased(osix::xxEvent *);
    virtual int navKeyPressed(osix::xxEvent *);
    virtual int navKeyReleased(osix::xxEvent *);

    Spreadsheet(const Spreadsheet &) {
    }
    Spreadsheet &operator=(const Spreadsheet &) {
      return *this;
    }

};

} // namespace spreadsheet


#endif
