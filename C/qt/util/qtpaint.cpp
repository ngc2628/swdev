
#include <osix/xxstyle.h>
#include <osix/xxshape.h>
#include <qt/util/qtpaint.h>
#include <qt/util/qtutil.h>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOptionButton>

namespace qtutil {

int xxpredrawQt(void *,osix::xxDrawable *xxdrawable,osix::xxGC *) {

  QPainter *pp=(xxdrawable->m_extra ? (QPainter*)(xxdrawable->m_extra) : 0);
  if (!pp) {
    pp=new QPainter();
    xxdrawable->m_extra=pp;
  }
  if (!pp->isActive()) {
    if ((xxdrawable->m_t&osix::xx_pixmap)>0)
      pp->begin((QPixmap*)xxdrawable->m_w);
    else if ((xxdrawable->m_t&osix::xx_window)>0)
      pp->begin((QWidget*)xxdrawable->m_w); 
  }
  return 0;

}

int xxpostdrawQt(void *,osix::xxDrawable *xxdrawable,osix::xxGC *) {

  QPainter *pp=(xxdrawable->m_extra ? (QPainter*)(xxdrawable->m_extra) : 0);
  if (pp) {
    if (pp->isActive())
      pp->end();
    delete pp;
  }
  xxdrawable->m_extra=0;
  return 0;

}

int xxdrawLineQt(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc) {

  if (!xxdrawable->m_extra)
    return 1;
  QPainter *pp=(QPainter*)(xxdrawable->m_extra);
  QPen qp;
  toQPen(&xxgc->m_fg,&qp);
  osix::xxLine l(xxgc->m_r.leftTop(),xxgc->m_r.rightBottom());
  QLineF ql;
  toQLineF(&l,&ql);
  pp->setPen(qp);
  pp->setBrush(Qt::NoBrush);
  pp->drawLine(ql);
  return 0;

}

int xxdrawLinesQt(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc,aux::Vector3 *pL,int pcnt,int) {

  if (!xxdrawable->m_extra)
    return 1;
  QPainter *pp=(QPainter*)(xxdrawable->m_extra);
  QPen qp;
  toQPen(&xxgc->m_fg,&qp);
  QPointF *qpL=new QPointF[pcnt];
  int ii=0;
  for (ii=0;ii<pcnt;ii++) 
    qpL[ii]=QPointF(pL[ii].x(),pL[ii].y()); 
  pp->setPen(qp);
  pp->setBrush(Qt::NoBrush);
  pp->drawPolyline (qpL,pcnt);
  delete[] qpL;
  return 0;

}

int xxdrawRectQt(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc) {
  
  if (!xxdrawable->m_extra)
    return 1;
  QPainter *pp=(QPainter*)(xxdrawable->m_extra);
  QPen qp;
  toQPen(&xxgc->m_fg,&qp);
  QRectF qr;
  toQRectF(&xxgc->m_r,&qr);
  pp->setPen(qp);
  pp->setBrush(Qt::NoBrush);
  pp->drawRect(qr);
  return 0;

}

int xxfillRectQt(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc) {

  if (!xxdrawable->m_extra)
    return 1;
  QPainter *pp=(QPainter*)(xxdrawable->m_extra);
  QPen qp;
  toQPen(&xxgc->m_fg,&qp);
  QBrush qb;
  toQBrush(&xxgc->m_bg,&qb);
  QRectF qr;
  toQRectF(&xxgc->m_r,&qr);
  pp->setPen(qp);
  pp->setBrush(qb);
  pp->fillRect(qr,qb);
  return 0;

}

int xxfillPolygonQt(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc,aux::Vector3 *pL,int pcnt,int opt) {

  if (!xxdrawable->m_extra)
    return 1;
  QPainter *pp=(QPainter*)(xxdrawable->m_extra);
  QPen qp;
  toQPen(&xxgc->m_fg,&qp);
  QBrush qb;
  toQBrush(&xxgc->m_bg,&qb);
  QPointF *qpL=new QPointF[pcnt];
  int ii=0;
  for (ii=0;ii<pcnt;ii++) 
    qpL[ii]=QPointF(pL[ii].x(),pL[ii].y());
  pp->setPen(qp);
  pp->setBrush(qb);
  pp->drawPolygon(qpL,pcnt,(opt&osix::xx_convex)>0 ? Qt::WindingFill : Qt::OddEvenFill);
  delete [] qpL;
  return 0;

}

int xxdrawTextQt(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc,aux::Ucsstr *t,int a) {

  if (!xxdrawable->m_extra)
    return 1;
  QPainter *pp=(QPainter*)(xxdrawable->m_extra);
  QPen qp;
  toQPen(&xxgc->m_fg,&qp);
  QRectF qr;
  toQRectF(&xxgc->m_r,&qr);
  QFont qfnt;
  toQFont(&xxgc->m_fnt,&qfnt);
  QString qstr;
  toQString(t,&qstr);
  //aux::Asciistr strDbg1=t->asciistr(),strDbg2;
  //xxgc->m_r.toString(&strDbg2);
  pp->setPen(qp);
  pp->setBrush(Qt::NoBrush);
  pp->setFont(qfnt);
  pp->drawText(qr,toQAlign(a),qstr);
  return 0;

}

int xxdrawCtrlQt(void *disp,osix::xxDrawable *xxdrawable,osix::xxGC *xxgc,int ctrltype,aux::Ucsstr *t,int) {

  if (!xxdrawable->m_extra)
    return 1;
  QPainter *pp=(QPainter*)(xxdrawable->m_extra);
  QPen qp;
  toQPen(&xxgc->m_fg,&qp);
  QBrush qb;
  toQBrush(&xxgc->m_bg,&qb);
  QRectF qr;
  toQRectF(&xxgc->m_r,&qr);
  QFont qfnt;
  toQFont(&xxgc->m_fnt,&qfnt);
  QString qstr;
  toQString(t,&qstr);
  pp->setPen(qp);
  pp->setBrush(qb);
  pp->setFont(qfnt);

  if (xxgc->m_w &&
      (ctrltype==osix::xx_pushbutton_raised || ctrltype==osix::xx_pushbutton_sunken)) {
    QWidget *ww=(QWidget*)(xxgc->m_w);
    QStyleOptionButton option;
    option.initFrom(ww);
    option.rect=qr.toRect();
    option.text=qstr;
    option.state=(ctrltype==osix::xx_pushbutton_raised ? QStyle::State_Raised : QStyle::State_Sunken);
    ww->style()->drawControl(QStyle::CE_PushButton,&option,pp,ww);
  }
  return 0;

}

int xxresizeDrawableQt(void *,osix::xxDrawable *dest,osix::xxGC *gc) {

  if (dest->m_w==0 || gc->m_r.busted()>0)
    return 1;
  int ww=aux::round2(gc->m_r.size().width()),hh=aux::round2(gc->m_r.size().height());
  if ((dest->m_t&osix::xx_window)>0) {
    QWidget *wq=(QWidget*)dest->m_w;
    if (wq->size().width()!=ww || wq->size().height()!=hh)
      wq->resize(ww,hh);
  }
  else if ((dest->m_t&osix::xx_pixmap)>0) {
    QPixmap *pm=(QPixmap*)dest->m_w;
    if (pm->size().width()!=ww || pm->size().height()!=hh) {
      delete pm;
      pm=new QPixmap(ww,hh);
      dest->m_w=(void*)pm;
      QBrush brush;
      toQBrush(&gc->m_bg,&brush);
      QPainter pp(pm);
      pp.fillRect(0,0,ww,hh,brush);
    }
  }
  dest->m_r=gc->m_r; 
  return 0;

}

int xxcopyAreaQt(void *,osix::xxDrawable *dest,osix::xxDrawable *src,
  osix::xxGC *,osix::xxRect rdest,osix::xxRect rsrc) {

  if (((dest->m_t&osix::xx_pixmap)==0 && (dest->m_t&osix::xx_window)==0) || 
      (src->m_t&osix::xx_pixmap)==0)
    return 1;
  int destx=0,desty=0,srcx=0,srcy=0,
      srcw=(int)aux::round2(src->m_r.size().width()),
      srch=(int)aux::round2(src->m_r.size().height()),
      destw=(int)aux::round2(dest->m_r.size().width()),
      desth=(int)aux::round2(dest->m_r.size().height());
  if (rsrc.empty()==0) {
    srcx=(int)aux::round2(rsrc.left());
    srcy=(int)aux::round2(rsrc.top());
    srcw=(int)aux::round2(rsrc.size().width());
    srch=(int)aux::round2(rsrc.size().height());
  }
  if (rdest.empty()==0) {
    destx=(int)aux::round2(rdest.left());
    desty=(int)aux::round2(rdest.top());
    destw=(int)aux::round2(rdest.size().width());
    desth=(int)aux::round2(rdest.size().height());
  }
  QPainter pp;
  if ((dest->m_t&osix::xx_pixmap)==0)
    pp.begin((QWidget*)dest->m_w);
  else
    pp.begin((QPixmap*)dest->m_w);
//printf ("%d dx=%d dy=%d dw=%d dh=%d sx=%d sy=%d sw=%d sh=%d\n",
//__LINE__,destx,desty,destw,desth,srcx,srcy,srcw,srch);
  pp.drawPixmap(destx,desty,destw,desth,*((QPixmap*)src->m_w),srcx,srcy,srcw,srch);
  pp.end();
  return 0;

}

}
