
#ifndef _qtpaint_h
#define _qtpaint_h

#include <osix/xxpaint.h>

namespace qtutil {

extern oswinexp int xxpredrawQt(void *,osix::xxDrawable *,osix::xxGC *);

extern oswinexp int xxpostdrawQt(void *,osix::xxDrawable *,osix::xxGC *);

extern oswinexp int xxdrawLineQt(void *,osix::xxDrawable *,osix::xxGC *);

extern oswinexp int xxdrawLinesQt(void *,osix::xxDrawable *,osix::xxGC *,aux::Vector3 *,int,int);

extern oswinexp int xxdrawRectQt(void *,osix::xxDrawable *,osix::xxGC *);

extern oswinexp int xxfillRectQt(void *,osix::xxDrawable *,osix::xxGC *);

extern oswinexp int xxfillPolygonQt(void *,osix::xxDrawable *,osix::xxGC *,aux::Vector3 *,int,int);

extern oswinexp int xxdrawTextQt(void *,osix::xxDrawable *,osix::xxGC *,aux::Ucsstr *,int);

extern oswinexp int xxdrawCtrlQt(void *,osix::xxDrawable *,osix::xxGC *,int,aux::Ucsstr *,int);

extern oswinexp int xxresizeDrawableQt(void *,osix::xxDrawable *,osix::xxGC *);

extern oswinexp int xxcopyAreaQt(void *,osix::xxDrawable *,osix::xxDrawable *,
  osix::xxGC *,osix::xxRect,osix::xxRect);

}

#endif
