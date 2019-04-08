
#ifndef _typeid_h_
#define _typeid_h_

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <mkbase/defs.h>
#include <mkbase/exportdefs.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkutil.h>

namespace mk {

extern int oswinexp cmpTypeIdRef(const void *,const void *);
extern int oswinexp cmpTypeIdRefi(const void *,const void *);
extern int oswinexp cmpTypeIdRefiType(const void *,const void *);
extern int oswinexp cmpTypeId(const void *,const void *);
extern int oswinexp cmpTypeIdType(const void *,const void *);

class oswinexp TypeId {

  protected:
    struct mk_typeid m_typeid;

  public:
    TypeId(const char *name=0,mk_ulreal idd=0);
    TypeId(const TypeId &);
    virtual ~TypeId();
    TypeId &operator=(const TypeId &);
    bool operator==(const TypeId &cmp) const;
    bool operator<(const TypeId &cmp) const;
    int busted() const;
    const char *type() const;
    mk_ulreal idd() const;
    virtual int toString(mk_string) const;
    virtual int fromString(const char *);
    friend int cmpTypeIdRef(const void *,const void *);
    friend int cmpTypeIdRefi(const void *,const void *);
    friend int cmpTypeIdRefiType(const void *,const void *);
    friend int cmpTypeId(const void *,const void *);
    friend int cmpTypeIdType(const void *,const void *);

};

} // namespace

#endif
