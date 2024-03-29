
#include <QtCore/QString>
#include <QtCore/QRectF>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QFont>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>
#include <osix/xxevent.h>
#include <osix/xxstyle.h>
#include <osix/xxshape.h>
#include <osix/xxtxt.h>
#include <qt/util/qtutil.h>
#include <stdio.h>

namespace qtutil {

int xxtranslateQevent(void *qev,osix::xxEvent *guiev) {

  //printf ("%d trqinp\n",__LINE__);
  guiev->clear();
  int qevtype=((QEvent*)qev)->type();
  if (qevtype==QEvent::KeyPress)
    guiev->m_type=osix::xx_keyPressed;
  else if (qevtype==QEvent::KeyRelease)
    guiev->m_type=osix::xx_keyReleased;
  else if (qevtype==QEvent::MouseButtonPress)
    guiev->m_type=osix::xx_mousePressed;
  else if (qevtype==QEvent::MouseButtonRelease)
    guiev->m_type=osix::xx_mouseReleased;
  else if (qevtype==QEvent::MouseMove)
    guiev->m_type=osix::xx_mouseMove;
  else if (qevtype==QEvent::Enter)
    guiev->m_type=osix::xx_enter;
  else if (qevtype==QEvent::Leave)
    guiev->m_type=osix::xx_leave;
  else if (qevtype==QEvent::FocusIn)
    guiev->m_type=osix::xx_focusIn;
  else if (qevtype==QEvent::FocusOut)
    guiev->m_type=osix::xx_focusOut;
  else if (qevtype==QEvent::Paint) {
    guiev->m_type=osix::xx_paint;
    QRectF qrf(((QPaintEvent*)qev)->rect());
    fromQRectF(&qrf,&guiev->m_r);
  }
  else if (qevtype==QEvent::Show)
    guiev->m_type=osix::xx_show;
  else if (qevtype==QEvent::Resize) {
    guiev->m_type=osix::xx_resize;
    QRect qr(QPoint(0,0),((QResizeEvent*)qev)->size());
    QRectF qrf(qr);
    fromQRectF(&qrf,&guiev->m_r);    
  }
  else if (qevtype==QEvent::MouseButtonDblClick)
    guiev->m_type=osix::xx_mouseDblClicked;
  else
    guiev->m_type=(osix::xx_unknownEvent|qevtype);
  
  if (osix::xxTypeKey(guiev)>0)
    return translateQkey((QKeyEvent*)qev,guiev);
  else if (osix::xxTypeMouse(guiev)>0)
    return translateQmouse((QMouseEvent*)qev,guiev);

  return 0;

}

const char *qasciistr(const QString *qstr) {

  return (qstr ? qstr->toLatin1().constData() : 0);

}

int fromQString(const QString *qstr,mk_string str) {

  if (qstr)
    mk_stringset(str,qstr->toLatin1().constData());
  else
    mk_stringset(str,0);
  return 0;

}

int fromQString(const QString *qstr,mk::Ucsstr *str) {

  mk::Ucsstr ucsstr(qstr ? qstr->length() : 0,qstr ? qstr->utf16() : 0);
  *str=ucsstr;
  return 0;

}

int fromQLineF(const QLineF *ql,osix::xxLine *ll) {

  mk_vertex vl1={(ql ? ql->p1().x() : .0),(ql ? ql->p1().y() : .0),.0,1.};
  mk_vertex vl2={(ql ? ql->p2().x() : .0),(ql ? ql->p2().y() : .0),.0,1.};
  ll->set(vl1,vl2);
  return 0;

}

int fromQRectF(const QRectF *qr,osix::xxRect *rr) {

  rr->set(qr ? qr->left() : 0,
          qr ? qr->top() : 0,
          qr ? qr->right() : 0,
          qr ? qr->bottom() : 0);
  return 0;

}

int fromQPen(const QPen *qpen,osix::xxStyle *style) {

  style->set(qpen ? (unsigned int)qpen->color().rgba() : 0,
             qpen ? (short)qpen->style() : 1,qpen ? (float)qpen->width() : 1.);
  return 0;

}

int fromQBrush(const QBrush *qbrush,osix::xxStyle *style) {

  style->set(qbrush ? (unsigned int)qbrush->color().rgba() : mk_uilimit,
             qbrush ? (short)qbrush->style() : 1,.0);
  return 0;

}

int fromQFont(const QFont *qfnt,osix::xxFnt *fnt) {

  int style=(qfnt && qfnt->bold() ? osix::xx_fntBold : osix::xx_fntReg);
  if (qfnt && qfnt->italic())
    style|=osix::xx_fntItalic;
  if (qfnt) {
    mk_stringset(fnt->m_fam,qfnt->family().toLatin1().constData());
    fnt->m_size=qfnt->pointSizeF();
    fnt->m_style=style;
  }
  return 0;

}

int fromQClipboard(mk::Ucsstr *str) {

  QString qstr=QApplication::clipboard()->text();
  fromQString(&qstr,str);
  return 0;

}

int fromQAlign(int aa) {

  int res=0;
  if ((aa&(int)Qt::AlignLeft)>0)
    res|=osix::xx_alignleft;
  if ((aa&(int)Qt::AlignRight)>0)
    res|=osix::xx_alignright;
  if ((aa&(int)Qt::AlignHCenter)>0)
    res|=osix::xx_alignhcenter;
  if ((aa&(int)Qt::AlignTop)>0)
    res|=osix::xx_aligntop;
  if ((aa&(int)Qt::AlignBottom)>0)
    res|=osix::xx_alignbottom;
  if ((aa&(int)Qt::AlignVCenter)>0)
    res|=osix::xx_alignvcenter;
  return res;

}

int toQString(mk_string str,QString *qstr) {

  if (mk_stringlength(str)==0)
    *qstr=QString();
  else
    *qstr=QLatin1String(&str[0]);
  return 0;

}

int toQString(const mk::Ucsstr *str,QString *qstr) {

  int ll=(str ? str->length() : 0);
  if (ll>0)
    qstr->setUtf16(str->data(),ll);
  else
    *qstr=QString();
  return 0;

}

int toQLineF(const osix::xxLine *ll,QLineF *ql) {

  mk_vertexzero(vl1);
  mk_vertexzero(vl2);
  if (ll) {
    ll->p0(vl1);
    ll->p1(vl2);
  }
  ql->setLine(vl1[0],vl1[1],vl2[0],vl2[1]);
  return 0;

}

int toQRectF(const osix::xxRect *rr,QRectF *qr) {

  qr->setCoords(rr ? rr->left() : 0,rr ? rr->top() : 0,rr ? rr->right() : 0,rr ? rr->bottom() : 0);
  return 0;

}

int toQPen(const osix::xxStyle *style,QPen *pen) {

  int iwidth=(int)mk_round2(style ? style->m_width : 1.),
    ok=(iwidth>0 && style && style->m_style>0 ? 0 : -1);
  if (ok==0) {
    QBrush brush(style ? style->m_color : 0,Qt::SolidPattern);
    *pen=QPen(brush,iwidth%2 ? iwidth : iwidth+1,(Qt::PenStyle)(style ? style->m_style : 1));
  }
  else
    *pen=QPen(Qt::NoPen);
  return ok;

}

int toQBrush(const osix::xxStyle *style,QBrush *brush) {

  if (style && style->m_style>0)
    *brush=QBrush(QColor(style->m_color),(Qt::BrushStyle)style->m_style);
  else
    *brush=QBrush(Qt::NoBrush);
  return (style ? (style->m_style>0 ? 0 : -1) : -1);

}

int toQFont(const osix::xxFnt *fnt,QFont *qfnt) {

  qfnt->setFamily(fnt ? (const char *)fnt->m_fam : "Arial");
  qfnt->setPointSizeF(fnt ? fnt->m_size : 10.);
  if (fnt && (fnt->m_style&osix::xx_fntBold)>0)
    qfnt->setBold(true);
  if (fnt && (fnt->m_style&osix::xx_fntItalic)>0)
    qfnt->setItalic(true);
  return 0;

}

int toQClipboard(mk::Ucsstr *str) {

  QString qstr;
  toQString(str,&qstr);
  QApplication::clipboard()->setText(qstr);
  return 0;

}

int toQAlign(int aa) {

  int res=0;
  if ((aa&osix::xx_alignleft)>0)
    res|=(int)Qt::AlignLeft;
  if ((aa&osix::xx_alignright)>0)
    res|=(int)Qt::AlignRight;
  if ((aa&osix::xx_alignhcenter)>0)
    res|=(int)Qt::AlignHCenter;
  if ((aa&osix::xx_aligntop)>0)
    res|=(int)Qt::AlignTop;
  if ((aa&osix::xx_alignbottom)>0)
    res|=(int)Qt::AlignBottom;
  if ((aa&osix::xx_alignvcenter)>0)
    res|=(int)Qt::AlignVCenter;
  return res;

}


} // namespace
