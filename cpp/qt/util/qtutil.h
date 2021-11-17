
#ifndef _QTUTIL_
#define _QTUTIL_

#include <mkbase/exportdefs.h>
#include <mkbase/mkutil.h>

namespace mk {
class Ucsstr;
}

namespace osix {
class xxEvent;
class xxStyle;
class xxLine;
class xxRect;
class xxFnt;
}

class QKeyEvent;
class QMouseEvent;
class QString;
class QLineF;
class QRectF;
class QPen;
class QBrush;
class QFont;

extern int dbgmode;

namespace qtutil {

extern oswinexp int xxtranslateQevent(void *,osix::xxEvent *);

extern oswinexp int translateQkey(QKeyEvent *,osix::xxEvent *);

extern oswinexp int translateQmouse(QMouseEvent *,osix::xxEvent *);

extern oswinexp const char *qasciistr(const QString *);

extern oswinexp int fromQString(const QString *,mk_string);

extern oswinexp int fromQString(const QString *,mk::Ucsstr *);

extern oswinexp int fromQLineF(const QLineF *,osix::xxLine *);

extern oswinexp int fromQRectF(const QRectF *,osix::xxRect *);

extern oswinexp int fromQPen(const QPen *,osix::xxStyle *);

extern oswinexp int fromQBrush(const QBrush *,osix::xxStyle *);

extern oswinexp int fromQFont(const QFont *,osix::xxFnt *);

extern oswinexp int fromQClipboard(mk::Ucsstr *);

extern oswinexp int fromQAlign(int);

extern oswinexp int toQString(mk_string,QString *);

extern oswinexp int toQString(const mk::Ucsstr *,QString *);

extern oswinexp int toQLineF(const osix::xxLine *,QLineF *);

extern oswinexp int toQRectF(const osix::xxRect *,QRectF *);

extern oswinexp int toQPen(const osix::xxStyle *,QPen *);

extern oswinexp int toQBrush(const osix::xxStyle *,QBrush *);

extern oswinexp int toQFont(const osix::xxFnt *,QFont *);

extern oswinexp int toQClipboard(mk::Ucsstr *);

extern oswinexp int toQAlign(int);

}

#endif
