
#include <osix/xxtxt.h>
#include <mkbase/mkconv.h>
#include <stdio.h>
#include <string.h>

namespace osix {

xxtoClipboardF xxtoClipboardExt=0;
xxfromClipboardF xxfromClipboardExt=0;

xxFnt::xxFnt(const char *fam,float size,int style,xxRectSize metric) :
  m_size(size),m_style(style),m_metric(metric) {

  mk_stringset(m_fam,fam);
  if (mk_stringlength(m_fam)==0)
    mk_stringset(m_fam,"Arial");

}

xxFnt::xxFnt(const xxFnt &ass) : 
  m_size(ass.m_size),m_style(ass.m_style),m_metric(ass.m_metric) {

  mk_stringset(m_fam,0);
  if (this!=&ass)
    mk_stringset(m_fam,ass.m_fam);
  if (mk_stringlength(m_fam)==0)
    mk_stringset(m_fam,"Arial");

}
    
xxFnt &xxFnt::operator=(const xxFnt &ass) {

  m_size=ass.m_size;
  m_style=ass.m_style;
  m_metric=ass.m_metric;
  mk_stringset(m_fam,0);
  if (this!=&ass)
    mk_stringset(m_fam,ass.m_fam);
  if (mk_stringlength(m_fam)==0)
    mk_stringset(m_fam,"Arial");
  return *this;

}  

bool xxFnt::operator==(const xxFnt &cmp) const {

  return (mk_stringcmp(m_fam,cmp.m_fam)==0 && m_size==cmp.m_size && m_style==cmp.m_style);
  
}

bool xxFnt::operator<(const xxFnt &cmp) const {

  int cmpfam=mk_stringcmp(m_fam,cmp.m_fam);
  return (cmpfam<0 || (cmpfam==0 && (m_size<cmp.m_size || 
           (m_size==cmp.m_size && m_style<cmp.m_style))));

}

int xxFnt::toString(mk_string str) const {
  
  mk_stringappend(str,"Fnt : fam=");
  mk_stringappend(str,&m_fam[0]);
  mk_stringappend(str," ; size=");
  mk_string numstr;
  mk_d2a((double)m_size,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," ; style=");
  mk_i2a(m_style,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," ; ");
  m_metric.toString(numstr);
  mk_stringappend(str,numstr);
  return 0;
  
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
