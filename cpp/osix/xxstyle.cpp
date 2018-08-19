
#include <auxx/auxx.h>
#include <osix/xxstyle.h>
#include <stdio.h>
#include <string.h>

namespace osix {

void xxStyle::toString(aux::Asciistr *buf) const {
  
  if (!buf)
    return;
  buf->reserve(4096);
  aux::Asciistr strbuf;
  buf->append("Style : color(r=");
  aux::i2a((mk_lreal)((m_color>>16)&255),&strbuf);
  buf->append((const char *)strbuf);
  strbuf=0;
  buf->append(" g=");
  aux::i2a((mk_lreal)((m_color>>8)&255),&strbuf);
  buf->append((const char *)strbuf);
  strbuf=0;
  buf->append(" b=");
  aux::i2a((mk_lreal)(m_color&255),&strbuf);
  buf->append((const char *)strbuf);
  strbuf=0;
  buf->append(") ; style=");
  aux::i2a((mk_lreal)m_style,&strbuf);
  buf->append((const char *)strbuf);
  strbuf=0;
  buf->append("; width=");
  aux::d2a(m_width,&strbuf,2);
  buf->append((const char *)strbuf);
    
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
