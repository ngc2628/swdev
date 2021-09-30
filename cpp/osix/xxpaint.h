
#ifndef _xxpaint_h_
#define _xxpaint_h_

#include <osix/xxstyle.h>
#include <osix/xxshape.h>
#include <osix/xxtxt.h>

namespace osix {

const int xx_pixmap=1;
const int xx_window=2;
const int xx_pushbutton_raised=1;
const int xx_pushbutton_sunken=2;
const int xx_nonconvex=1;
const int xx_convex=2;
const int xx_complex=4;
const int xx_modeor=8;
const int xx_modeprev=16;

class oswinexp xxDrawable {

  public:
    int m_t;
    void *m_w;
    void *m_extra;
    xxRect m_r;
    xxDrawable(int t=0,void *w=0,xxRect r=xxRect()) : 
      m_t(t),m_w(w),m_extra(0),m_r(r) {
    }
    xxDrawable(const xxDrawable &ass) : 
      m_t(ass.m_t),m_w(ass.m_w),m_extra(ass.m_extra),m_r(ass.m_r) {
    }
    virtual ~xxDrawable() {
    }
    xxDrawable &operator=(const xxDrawable &ass) {
      m_t=ass.m_t;
      m_w=ass.m_w;
      m_extra=ass.m_extra;
      m_r=ass.m_r;
      return *this;
    }

};

class oswinexp xxGC {

  public:
    xxRect m_r;
    xxStyle m_fg,m_bg;
    xxFnt m_fnt;
    void *m_w;
    xxGC() : m_w(0) {
    }
    xxGC(const xxGC &ass) :
      m_r(ass.m_r),m_fg(ass.m_fg),m_bg(ass.m_bg),m_fnt(ass.m_fnt),m_w(ass.m_w) {
    }
    ~xxGC() {
    }
    xxGC &operator=(const xxGC &ass) {
      m_r=ass.m_r;
      m_fg=ass.m_fg;
      m_bg=ass.m_bg;
      m_fnt=ass.m_fnt;
      m_w=ass.m_w;
      return *this;
    }

};

typedef int (*xxpredrawF)(void *,xxDrawable *,xxGC *);
extern xxpredrawF oswinexp xxpredrawExt;
extern int oswinexp xxpredraw(void *,xxDrawable *,xxGC *);

typedef int (*xxpostdrawF)(void *,xxDrawable *,xxGC *);
extern xxpostdrawF oswinexp xxpostdrawExt;
extern int oswinexp xxpostdraw(void *,xxDrawable *,xxGC *);

typedef int (*xxdrawLineF)(void *,xxDrawable *,xxGC *);
extern xxdrawLineF oswinexp xxdrawLineExt;
extern int oswinexp xxdrawLine(void *,xxDrawable *,xxGC *);

typedef int (*xxdrawLinesF)(void *,xxDrawable *,xxGC *,mk_vertex *,int,int);
extern xxdrawLinesF oswinexp xxdrawLinesExt;
extern int oswinexp xxdrawLines(void *,xxDrawable *,xxGC *,mk_vertex *,int,int);

typedef int (*xxdrawRectF)(void *,xxDrawable *,xxGC *);
extern xxdrawRectF oswinexp xxdrawRectExt;
extern int oswinexp xxdrawRect(void *,xxDrawable *,xxGC *);

typedef int (*xxfillRectF)(void *,xxDrawable *,xxGC *);
extern xxfillRectF oswinexp xxfillRectExt;
extern int oswinexp xxfillRect(void *,xxDrawable *,xxGC *);

typedef int (*xxfillPolygonF)(void *,xxDrawable *,xxGC *,mk_vertex *,int,int);
extern xxfillPolygonF oswinexp xxfillPolygonExt;
extern int oswinexp xxfillPolygon(void *,xxDrawable *,xxGC *,mk_vertex *,int,int);

typedef int (*xxdrawTextF)(void *,xxDrawable *,xxGC *,mk::Ucsstr *,int);
extern xxdrawTextF oswinexp xxdrawTextExt;
extern int oswinexp xxdrawText(void *,xxDrawable *,xxGC *,mk::Ucsstr *,int);

typedef int (*xxdrawCtrlF)(void *,xxDrawable *,xxGC *,int,mk::Ucsstr *,int);
extern xxdrawCtrlF oswinexp xxdrawCtrlExt;
extern int oswinexp xxdrawCtrl(void *,xxDrawable *,xxGC *,int,mk::Ucsstr *,int);

typedef int (*xxresizeDrawableF)(void *,xxDrawable *,xxGC *);
extern xxresizeDrawableF oswinexp xxresizeDrawableExt;
extern int oswinexp xxresizeDrawable(void *,xxDrawable *,xxGC *);

typedef int (*xxcopyAreaF)(void *,xxDrawable *,xxDrawable *,xxGC *,xxRect,xxRect);
extern xxcopyAreaF oswinexp xxcopyAreaExt;
extern int oswinexp xxcopyArea(void *,xxDrawable *,xxDrawable *,xxGC *,xxRect,xxRect);

}

#endif
