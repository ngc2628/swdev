
#include <mkbase/mkconv.h>
#include <tools/typeid.h>

namespace mk {

int cmpTypeId(const void *t1,const void *t2) {

  const TypeId *tt1=(const TypeId*)t1;
  const TypeId *tt2=(const TypeId*)t2;
  return mk_cmptypeid((const void *)&tt1->m_typeid,(const void *)&tt2->m_typeid);

}

int cmpTypeIdType(const void *t1,const void *t2) {

  const TypeId *tt1=(const TypeId*)t1;
  const TypeId *tt2=(const TypeId*)t2;
  return mk_cmptypeidtype((const void *)&tt1->m_typeid,(const void *)&tt2->m_typeid);

}

int cmpTypeIdRef(const void *t1,const void *t2) {

  const TypeId **tt1=(const TypeId**)t1;
  const TypeId **tt2=(const TypeId**)t2;
  return mk_cmptypeid((const void *)&(*tt1)->m_typeid,(const void *)&(*tt2)->m_typeid);

}

int cmpTypeIdRefi(const void *t1,const void *t2) {

  const TypeId **tt1=(const TypeId**)t1;
  const TypeId *tt2=(const TypeId*)t2;
  return mk_cmptypeid((const void *)&(*tt1)->m_typeid,(const void *)&tt2->m_typeid);

}

int cmpTypeIdRefiType(const void *t1,const void *t2) {

  const TypeId **tt1=(const TypeId**)t1;
  const TypeId *tt2=(const TypeId*)t2;
  return mk_cmptypeidtype((const void *)&(*tt1)->m_typeid,(const void *)&tt2->m_typeid);

}

TypeId::TypeId(const char *name,mk_ulreal idd)  {

  memset(&m_typeid,0,sizeof(struct mk_typeid));
  mk_stringset(m_typeid.type,name);
  m_typeid.idd=(idd==0 ? mk_nextcnt() : idd);

}

TypeId::TypeId(const TypeId &ass) {

  if (&ass!=this) {
    memcpy(&m_typeid,&ass.m_typeid,sizeof(struct mk_typeid));
  }

}

TypeId::~TypeId() {

}

TypeId &TypeId::operator=(const TypeId &ass) {

  if (this!=&ass) {
    memcpy(&m_typeid,&ass.m_typeid,sizeof(struct mk_typeid));
  }
  return *this;

}

bool TypeId::operator==(const TypeId &cmp) const {

  return (mk_cmptypeid((const void*)&m_typeid,(const void*)&cmp.m_typeid)==0);

}

bool TypeId::operator<(const TypeId &cmp) const {

  return (mk_cmptypeid((const void*)&m_typeid,(const void*)&cmp.m_typeid)<0);

}

int TypeId::busted() const {

  return ((m_typeid.idd==0 || mk_stringlength(m_typeid.type)==0) ? 1 : 0);

}

const char *TypeId::type() const {

  return (const char*)(&m_typeid.type[0]);

}

mk_ulreal TypeId::idd() const {

  return m_typeid.idd;

}

int TypeId::toString(mk_string str) const {

  mk_typeid2string(&m_typeid,str);
  return 0;

}

int TypeId::fromString(const char *ss) {

  mk_string str;
  mk_stringset(str,ss);
  return mk_typeid4string(&m_typeid,str);

}

} // namespace
