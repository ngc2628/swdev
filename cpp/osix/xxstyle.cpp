
#include <auxx/auxx.h>
#include <osix/xxstyle.h>
#include <mkbase/mkconv.h>
#include <stdio.h>
#include <string.h>

namespace osix {

int xxStyle::toString(mk_string str) const {
  
  mk_stringappend(str,"Style : color(r=");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_i2a((m_color>>16)&255,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," g=");
  mk_i2a((m_color>>8)&255,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," b=");
  mk_i2a(m_color&255,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,") ; style=");
  mk_i2a(m_style,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"; width=");
  mk_i2a(m_width,numstr);
  mk_stringappend(str,numstr);
  return 0;
    
}

unsigned int xxcolor(unsigned short r,unsigned short g,unsigned short b,unsigned short a) {

  return (unsigned int)(b+(g<<8)+(r<<16)+(a<<24));

}

void xxrgba(unsigned int color,unsigned short *r,unsigned short *g,unsigned short *b,unsigned short *a) {

  if (r)
    *r=(unsigned short)((color>>16)&255);
  if (g)
    *g=(unsigned short)((color>>8)&255);
  if (b)
    *b=(unsigned short)(color&255);
  if (a)
    *a=(unsigned short)((color>>24)&255);

}

unsigned short xxred(unsigned int color) {

  return (unsigned short)((color>>16)&255);

}

unsigned short xxgreen(unsigned int color) {

  return (unsigned short)((color>>8)&255);

}

unsigned short xxblue(unsigned int color) {

  return (unsigned short)(color&255);

}

unsigned short xxalpha(unsigned int color) {

  return (unsigned short)((color>>24)&255);

}

} // namespace
