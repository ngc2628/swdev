
#include <osix/xxtxt.h>
#include <stdio.h>
#include <string.h>

namespace osix {

xxtoClipboardF xxtoClipboardExt=0;
xxfromClipboardF xxfromClipboardExt=0;

xxFnt &xxFnt::operator=(const xxFnt &ass) {

  if (this==&ass)
    return *this;
  m_fam=ass.m_fam;
  m_size=ass.m_size;
  m_style=ass.m_style;
  m_metric=ass.m_metric;
  
  return *this;

}

void xxFnt::toString(aux::Asciistr *buf) const {
  
  if (!buf)
    return;
  buf->reserve(4096);
  aux::Asciistr numbuf;
  buf->append("Fnt : fam=");
  buf->append(m_fam);
  buf->append(" ; size=");
  aux::d2a((double)m_size,&numbuf);
  buf->append((const char *)numbuf);
  numbuf=0;
  buf->append(" ; style=");
  aux::i2a((mk_lreal)m_style,&numbuf);
  buf->append((const char *)numbuf);
  buf->append(" ; ");
  m_metric.toString(&numbuf);
  buf->append((const char *)numbuf);
  
}

int xxtoClipboard(aux::Ucsstr *str) {

  if (xxtoClipboardExt)
    return xxtoClipboardExt(str);
  return 0;

}

int xxfromClipboard(aux::Ucsstr *str) {

  if (xxfromClipboardExt)
    return xxfromClipboardExt(str);
  return 0;

}

} // namespace
