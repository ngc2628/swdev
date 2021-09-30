
#include <mkbase/mkconv.h>
#include <tools/char.h>

namespace mk {

Ucsstr::Ucsstr(int ll,const unsigned short *str) : m_str(0),m_sz(0) {

  if (ll>0 && str) {
    m_sz=ll+1;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));
    memcpy(&m_str[0],&str[0],ll*sizeof(unsigned short));
    m_str[ll]=0;
  }

}

Ucsstr::Ucsstr(const Ucsstr &ass) {

  if (&ass!=this) {
    int ll=ass.length();
    if (ll>0) {
      m_sz=ll+1;
      m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));
      memcpy(&m_str[0],&ass.m_str[0],ll*sizeof(unsigned short));
      m_str[ll]=0;
    }
    else {
      m_str=0;
      m_sz=0;
    }
  }

}

Ucsstr::~Ucsstr() {

  if (m_str)
    free(m_str);

}

Ucsstr &Ucsstr::operator=(const Ucsstr &ass) {

  int ll=ass.length();
  if (&ass==this || (!m_str && ll==0))
    return *this;
  if (ll>=m_sz) {
    if (m_str)
      free (m_str);
    m_sz=ll+1;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));
  }
  memset(&m_str[0],0,m_sz*sizeof(unsigned short));
  if (ll>0)
    memcpy(&m_str[0],&ass.m_str[0],ll*sizeof(unsigned short));
  return *this;

}

Ucsstr &Ucsstr::operator=(const char *str) {

  int ll=(str ? (int)strlen(str) : 0);
  if (!m_str && ll==0)
    return *this;
  if (ll>=m_sz) {
    if (m_str)
      free (m_str);
    m_sz=ll+1;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));
  }
  memset(&m_str[0],0,m_sz*sizeof(unsigned short));
  int ii=0;
  for (ii=0;ii<ll;ii++)
    m_str[ii]=(unsigned short)str[ii];
  return *this;

}

Ucsstr::operator const unsigned short *() const {

  return (m_str ? m_str : &nullUcsstr[0]);

}

unsigned short Ucsstr::operator[](int ii) const {

  if (!m_str || ii<0 || ii>=m_sz)
    return 0;
  return m_str[ii];

}

bool Ucsstr::operator==(const Ucsstr &cmp) const {

  int ii=0,ll=length();
  if (ll!=cmp.length())
    return false;
  if (ll==0)
    return true;
  for (ii=0;ii<ll;ii++) {
    if (m_str[ii]!=cmp.m_str[ii])
      return false;
  }
  return true;

}

bool Ucsstr::operator<(const Ucsstr &cmp) const {

  int ii=0,ll=length(),cmpl=cmp.length();
  if (ll!=cmpl)
    return (ll<cmpl);
  for (ii=0;ii<ll;ii++) {
    if (m_str[ii]==cmp.m_str[ii])
      continue;
    return (m_str[ii]<cmp.m_str[ii] ? true : false);
  }
  return false;

}

unsigned short *Ucsstr::data() const {

  return m_str;

}

int Ucsstr::length() const {

  if (m_str) {
    int ii=0;
    for (ii=0;ii<m_sz;ii++) {
      if (m_str[ii]==0)
        return ii;
    }
  }
  return 0;

}

int Ucsstr::setLength(int nlength) {

  int ll=length();
  nlength=MIN(ll,MAX(0,nlength));
  if (ll==nlength)
    return ll;
  memset(&m_str[ll],0,(m_sz-ll)*sizeof(unsigned short));
  return length();

}

int Ucsstr::reserve(int cnt) {

  if (cnt<=m_sz)
    return m_sz;
  m_sz=cnt;
  unsigned short *cpstr=0;
  int ll=length();
  if (ll>0) {
    cpstr=(unsigned short*)malloc(ll*sizeof(unsigned short));
    memcpy(&cpstr[0],&m_str[0],ll*sizeof(unsigned short));
    free (m_str);
  }
  m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));
  memset(&m_str[0],0,m_sz*sizeof(unsigned short));
  if (cpstr) {
    memcpy(&m_str[0],&cpstr[0],ll*sizeof(unsigned short));
    free (cpstr);
  }
  return m_sz;

}

int Ucsstr::set(int ii,unsigned short cc) {

  int oc=(m_str && ii>=0 && ii<length() ? m_str[ii] : -1);
  if (oc>=0)
    m_str[ii]=cc;
  return oc;

}

int Ucsstr::find(unsigned short cc,int idx,unsigned char dir) const {

  int ll=length(),ss=(idx<0 ? ll+idx : idx),ee=(dir==mk_asciib ? -1 : ll);
  if (ll==0 || ss<0 || ss>=ll)
    return -1;
  do {
    if (m_str[ss]==cc)
      return ss;
    ss+=(dir==mk_asciib ? -1 : 1);
  } while (ss!=ee);

  return -1;

}

Ucsstr Ucsstr::substr(int idx,int cnt) const {

  if (idx<0)
    idx=0;
  int ll=length();
  if (ll==0 || cnt==0 || ll<=idx)
    return Ucsstr();
  if (cnt<0 || ll<idx+cnt)
    cnt=ll-idx;
  unsigned short *buf=(unsigned short*)malloc((cnt+1)*sizeof(unsigned short));
  memcpy(&buf[0],&m_str[idx],cnt*sizeof(unsigned short));
  buf[cnt]=0;
  Ucsstr str(cnt,buf);
  free(buf);
  return str;

}

Ucsstr &Ucsstr::append(const char *str) {

  int ii=0,ll=length(),sl=(str ? (int)strlen(str) : 0);
  if (sl==0)
    return *this;
  if (ll+sl>=m_sz) {
    unsigned short *cpstr=0;
    if (m_str) {
      cpstr=(unsigned short*)malloc(ll*sizeof(unsigned short));
      memcpy(&cpstr[0],&m_str[0],ll*sizeof(unsigned short));
      free (m_str);
    }
    m_sz=ll+sl+1;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));;
    memset(&m_str[0],0,m_sz*sizeof(unsigned short));
    if (cpstr) {
      memcpy(&m_str[0],&cpstr[0],ll*sizeof(unsigned short));
      free (cpstr);
    }
  }
  for (ii=0;ii<sl;ii++)
    m_str[ll+ii]=(unsigned short)str[ii];
  return *this;

}

Ucsstr &Ucsstr::append(unsigned short uc) {

  if (uc==0)
    return *this;
  int ll=length();
  if (ll+1>=m_sz) {
    unsigned short *cpstr=0;
    if (m_str) {
      cpstr=(unsigned short*)malloc(ll*sizeof(unsigned short));
      memcpy(&cpstr[0],&m_str[0],ll*sizeof(unsigned short));
      free (m_str);
    }
    m_sz=ll+2;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));;
    memset(&m_str[0],0,m_sz*sizeof(unsigned short));
    if (cpstr) {
      memcpy(&m_str[0],&cpstr[0],ll*sizeof(unsigned short));
      free (cpstr);
    }
  }
  m_str[ll]=uc;
  return *this;

}

Ucsstr &Ucsstr::append(const Ucsstr &str) {

  int ll=length(),sl=str.length();
  if (sl==0)
    return *this;
  if (ll+sl>=m_sz) {
    unsigned short *cpstr=0;
    if (m_str) {
      cpstr=(unsigned short*)malloc(ll*sizeof(unsigned short));
      memcpy(&cpstr[0],&m_str[0],ll*sizeof(unsigned short));
      free (m_str);
    }
    m_sz=ll+sl+1;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));;
    memset(&m_str[0],0,m_sz*sizeof(unsigned short));
    if (cpstr) {
      memcpy(&m_str[0],&cpstr[0],ll*sizeof(unsigned short));
      free (cpstr);
    }
  }
  memcpy(&m_str[ll],&str.m_str[0],sl*sizeof(unsigned short));
  return *this;

}

Ucsstr &Ucsstr::prepend(const char *str) {

  int ii=0,ll=length(),sl=(str ? (int)strlen(str) : 0);
  if (sl==0)
    return *this;
  if (ll+sl>=m_sz) {
    unsigned short *cpstr=0;
    if (m_str) {
      cpstr=(unsigned short*)malloc(ll*sizeof(unsigned short));
      memcpy(&cpstr[0],&m_str[0],ll*sizeof(unsigned short));
      free (m_str);
    }
    m_sz=ll+sl+1;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));
    memset(&m_str[0],0,m_sz*sizeof(unsigned short));
    for (ii=0;ii<sl;ii++)
      m_str[ii]=(unsigned short)str[ii];
    if (cpstr) {
      memcpy(&m_str[sl],&cpstr[0],ll*sizeof(unsigned short));
      free (cpstr);
    }
  }
  else {
    memmove(&m_str[sl],&m_str[0],ll*sizeof(unsigned short));
    for (ii=0;ii<sl;ii++)
      m_str[ii]=(unsigned short)str[ii];
  }
  return *this;

}

Ucsstr &Ucsstr::prepend(unsigned short uc) {

  if (uc==0)
    return *this;
  int ll=length();
  if (ll+1>=m_sz) {
    unsigned short *cpstr=0;
    if (m_str) {
      cpstr=(unsigned short*)malloc(ll*sizeof(unsigned short));
      memcpy(&cpstr[0],&m_str[0],ll*sizeof(unsigned short));
      free (m_str);
    }
    m_sz=ll+2;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));
    memset(&m_str[0],0,m_sz*sizeof(unsigned short));
    m_str[0]=uc;
    if (cpstr) {
      memcpy(&m_str[1],&cpstr[0],ll*sizeof(unsigned short));
      free (cpstr);
    }
  }
  else {
    memmove(&m_str[1],&m_str[0],ll*sizeof(unsigned short));
    m_str[0]=uc;
  }
  return *this;

}

Ucsstr &Ucsstr::prepend(const Ucsstr &str) {

  int ll=length(),sl=str.length();
  if (sl==0)
    return *this;
  if (ll+sl>=m_sz) {
    unsigned short *cpstr=0;
    if (m_str) {
      cpstr=(unsigned short*)malloc(ll*sizeof(unsigned short));
      memcpy(&cpstr[0],&m_str[0],ll*sizeof(unsigned short));
      free (m_str);
    }
    m_sz=ll+sl+1;
    m_str=(unsigned short*)malloc(m_sz*sizeof(unsigned short));
    memset(&m_str[0],0,m_sz*sizeof(unsigned short));
    memcpy(&m_str[0],&str.m_str[0],sl*sizeof(unsigned short));
    if (cpstr) {
      memcpy(&m_str[sl],&cpstr[0],ll*sizeof(unsigned short));
      free (cpstr);
    }
  }
  else {
    memmove(&m_str[sl],&m_str[0],ll*sizeof(unsigned short));
    memcpy(&m_str[0],&str.m_str[0],sl*sizeof(unsigned short));
  }
  return *this;

}

int Ucsstr::toAscii(mk_string str) const {

  mk_stringset(str,0);
  int ii=0,ll=length();
  for (ii=0;ii<ll && ii<mk_sz;ii++)
    mk_stringsetat(str,mk_sz,m_str[ii]&255);
  return 0;

}

} // namespace
