
#include <osix/xxpaint.h>

namespace osix {

xxpredrawF xxpredrawExt=0;
xxpostdrawF xxpostdrawExt=0;
xxdrawLineF xxdrawLineExt=0;
xxdrawLinesF xxdrawLinesExt=0;
xxdrawRectF xxdrawRectExt=0;
xxfillRectF xxfillRectExt=0;
xxfillPolygonF xxfillPolygonExt=0;
xxdrawTextF xxdrawTextExt=0;
xxdrawCtrlF xxdrawCtrlExt=0;
xxresizeDrawableF xxresizeDrawableExt=0;
xxcopyAreaF xxcopyAreaExt=0;

int xxpredraw(void *disp,xxDrawable *dev,xxGC *gc) {

  if (xxpredrawExt)
    return xxpredrawExt(disp,dev,gc);
  
  return 0;

}

int xxpostdraw(void *disp,xxDrawable *dev,xxGC *gc) {

  if (xxpostdrawExt)
    return xxpostdrawExt(disp,dev,gc);
  
  return 0;

}

int xxdrawLine(void *disp,xxDrawable *dev,xxGC *gc) {

  if (xxdrawLineExt)
    return xxdrawLineExt(disp,dev,gc);
  
  return 0;

}

int xxdrawLines(void *disp,xxDrawable *dev,xxGC *gc,aux::Vector3 *pL,int pcnt,int opt) {

  if (xxdrawLinesExt)
    return xxdrawLinesExt(disp,dev,gc,pL,pcnt,opt);
  
  return 0;

}

int xxdrawRect(void *disp,xxDrawable *dev,xxGC *gc) {

  if (xxdrawRectExt)
    return xxdrawRectExt(disp,dev,gc);
  
  return 0;

}

int xxfillRect(void *disp,xxDrawable *dev,xxGC *gc) {

  if (xxfillRectExt)
    return xxfillRectExt(disp,dev,gc);
  
  return 0;

}

int xxfillPolygon(void *disp,xxDrawable *dev,xxGC *gc,aux::Vector3 *pL,int pcnt,int opt) {

  if (xxfillPolygonExt)
    return xxfillPolygonExt(disp,dev,gc,pL,pcnt,opt);
  
  return 0;

}

int xxdrawText(void *disp,xxDrawable *dev,xxGC *gc,aux::Ucsstr *str,int opt) {

  if (xxdrawTextExt)
    return xxdrawTextExt(disp,dev,gc,str,opt);
  
  return 0;

}

int xxdrawCtrl(void *disp,xxDrawable *dev,xxGC *gc,int type,aux::Ucsstr *str,int opt) {

  if (xxdrawCtrlExt)
    return xxdrawCtrlExt(disp,dev,gc,type,str,opt);
  
  return 0;

}

int xxcopyArea(void *disp,xxDrawable *dest,xxDrawable *src,
  xxGC *gc,xxRect rdest,xxRect rsrc) {

  if (xxcopyAreaExt)
    return xxcopyAreaExt(disp,dest,src,gc,rdest,rsrc);
  
  return 0;

}

int xxresizeDrawable(void *disp,xxDrawable *dest,xxGC *gc) {

  if (xxresizeDrawableExt)
    return xxresizeDrawableExt(disp,dest,gc);
  
  return 0;

}

}


