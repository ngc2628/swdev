
#ifndef _char_h_
#define _char_h_

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

const unsigned short nullUcsstr[7]={91,78,85,76,76,93,0};

class oswinexp Ucsstr {

  protected:
    unsigned short *m_str;
    int m_sz;

  public:
    Ucsstr(int l=0,const unsigned short *str=0);
    Ucsstr(const Ucsstr &);
    ~Ucsstr();
    Ucsstr &operator=(const Ucsstr &);
    Ucsstr &operator=(const char *);
    operator const unsigned short *() const;
    unsigned short operator[](int) const;
    bool operator==(const Ucsstr &) const;
    bool operator<(const Ucsstr &) const;
    unsigned short *data() const;
    int length() const;
    int reserve(int);
    int setLength(int);
    int set(int,unsigned short);
    int find(unsigned short,int idx=0,unsigned char dir='f') const;
    Ucsstr substr(int,int) const;
    Ucsstr &append(const char *);
    Ucsstr &append(unsigned short);
    Ucsstr &append(const Ucsstr &);
    Ucsstr &prepend(const char *);
    Ucsstr &prepend(unsigned short);
    Ucsstr &prepend(const Ucsstr &);
    int toAscii(mk_string) const;

};

}

#endif
