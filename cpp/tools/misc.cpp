
#include <mkbase/mkconv.h>
#include <tools/misc.h>

namespace mk {

Rounded &Rounded::operator=(const Rounded &ass) {

  m_d=ass.m_d;
  m_a=ass.m_a;
  return *this;

}

bool Rounded::operator==(const Rounded &cmp) const {

  return (mk_deq(m_d,cmp.m_d)>0);

}

bool Rounded::operator<(const Rounded &cmp) const {

  return (mk_dlt(m_d,cmp.m_d)>0);

}

int Rounded::cmprd(const Rounded &cmp) const {

  int cc=0,db=mk_dbusted(m_d,cmp.m_d,&cc);
  if (db!=0 || cc==0)
    return cc;
  int aa=(m_a>cmp.m_a ? m_a : cmp.m_a);
  db=mk_dbusted(mk_round2(m_d,aa),mk_round2(cmp.m_d,aa),&cc);
  return cc;

}

double Rounded::rounded() const {

  return mk_round2(m_d,m_a);

}

int Rounded::toString(mk_string str,char fmt,int pad,const char *dec,const char *group) const {

  return mk_d2a(m_d,str,m_a,fmt,pad,dec,group);
  
}

} // namespace
