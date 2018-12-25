
#include <mkbase/mkconv.h>
#include <auxx/auxx.h>

namespace aux {

int ui2a(mk_ulreal number,Asciistr *numstr,int base,int width,int fillzero,const char *group) {

  if (!numstr)
    return 1;
  mk_str1k(astr);
  mk_str1k(groupstr);
  if (group)
    strcpy(groupstr,group);
  int res=mk_ui2str(number,astr,base,width,fillzero,groupstr);
  *numstr=astr;
  return res;

}

int i2a(mk_lreal number,Asciistr *numstr,int base,int width,int fillzero,const char *group) {

  if (!numstr)
    return 1;
  mk_str1k(astr);
  mk_str1k(groupstr);
  if (group)
    strcpy(groupstr,group);
  int res=mk_i2str(number,astr,base,width,fillzero,groupstr);
  *numstr=astr;
  return res;

}

int d2a(double d,Asciistr *str,int p,char fmt,int pad,const char *dec,const char *group) {

  if (!str) 
    return 1;
  mk_str1k(astr);
  mk_str1k(decstr);
  mk_str1k(groupstr);
  if (decstr)
    strcpy(decstr,dec);
  if (group)
    strcpy(groupstr,group);
  int res=mk_d2str(d,astr,p,fmt,pad,decstr,groupstr);
  *str=astr;
  return res;

}

mk_ulreal nextT(Asciistr *str) {

  mk_ulreal t=mk_nextt();
  if (str)
    ui2a (t,str);
  return t;

}

Asciistr::Asciistr(const char *str) : m_str(0),m_sz(0) {

  if (str) {
    m_sz=(int)strlen(str)+1;
    m_str=(char*)malloc((size_t)m_sz);
    memset(&m_str[0],0,(size_t)m_sz);
    strcpy(m_str,str);
  }

}

Asciistr::Asciistr(const Asciistr &ass) {

  if (&ass!=this) {
    if (ass.m_str) {
      m_sz=(int)strlen(ass.m_str)+1;
      m_str=(char*)malloc((size_t)m_sz);
      memset(&m_str[0],0,(size_t)m_sz);
      strcpy(m_str,ass.m_str);
    }
    else {
      m_str=0;
      m_sz=0;
    }
  }

}

Asciistr::~Asciistr() {

  if (m_str)
    free(m_str);

}

Asciistr &Asciistr::operator=(const Asciistr &ass) {

  int ll=(ass.m_str ? (int)strlen(ass.m_str) : 0);
  if (&ass==this || (!m_str && ll==0))
    return *this;
  if (ll>=m_sz) {
    if (m_str)
      free (m_str);
    m_sz=ll+1;
    m_str=(char*)malloc((size_t)m_sz);
  }
  memset(&m_str[0],0,(size_t)m_sz);
  if (ll>0)
    strcpy(m_str,ass.m_str);
  return *this;

}

Asciistr &Asciistr::operator=(const char *str) {

  int ll=(str ? (int)strlen(str) : 0);
  if (!m_str && ll==0)
    return *this;
  if (ll>=m_sz) {
    if (m_str)
      free (m_str);
    m_sz=ll+1;
    m_str=(char*)malloc((size_t)m_sz);
  }
  memset(&m_str[0],0,(size_t)m_sz);
  if (ll>0)
    strcpy(m_str,str);
  return *this;

}

Asciistr::operator const char *() const {

  return (m_str ? m_str : nullAsciistr);

}

char Asciistr::operator[](int ii) const {

  if (!m_str || ii<0 || ii>=m_sz)
    return 0;
  return m_str[ii];

}

bool Asciistr::operator==(const Asciistr &cmp) const {

  if (len()!=cmp.len())
    return false;
  if (!m_str || !cmp.m_str)
    return true;
  return (strcmp(m_str,cmp.m_str)==0);

}

bool Asciistr::operator<(const Asciistr &cmp) const {

  int ll=len(),cmpll=cmp.len();
  if (ll!=cmpll)
    return (ll<cmpll);
  return (ll==0 ? false : strcmp(m_str,cmp.m_str)<0);

}

const char *Asciistr::data() const {

  return (m_str ? m_str : nullAsciistr);

}

char *Asciistr::rawdata() {

  return m_str;

}

int Asciistr::len() const {

  return (m_str ? (int)strlen(m_str) : 0);

}

int Asciistr::reserve(int cnt) {

  if (cnt<=m_sz)
    return m_sz;
  m_sz=cnt;
  char *cpstr=0;
  if (m_str) {
    int mylen=(int)strlen(m_str);
    cpstr=(char*)malloc((size_t)(mylen+1));
    strcpy(cpstr,m_str);
    cpstr[mylen]=0;
    free (m_str);
  }
  m_str=(char*)malloc((size_t)m_sz);
  memset(m_str,0,(size_t)m_sz);
  if (cpstr) {
    strcpy(m_str,cpstr);
    free (cpstr);
  }
  return m_sz;

}

int Asciistr::cut(int newl) {

  if (newl<0)
    newl=0;
  int ll=(m_str ? (int)strlen(m_str) : 0);
  if (ll<=newl)
    return ll;
  memset(&m_str[newl],0,(size_t)(ll-newl));
  return (int)strlen(m_str);

}

int Asciistr::set(int ii,char cc) {

  int oc=(m_str && ii>=0 && ii<len() ? m_str[ii] : -1);
  if (oc>=0)
    m_str[ii]=cc;
  return oc;

}

int Asciistr::find(char cc,int idx,unsigned char dir,unsigned char ci) const {

  char cci=0;
  int ll=len(),ss=(idx<0 ? ll+idx : idx),ee=(dir==mk_asciib ? -1 : ll);
  if (ll==0 || ss<0 || ss>=ll)
    return -1;
  if (ci==(char)mk_asciii)
    cc=(char)tolower(cc);
  do {
    cci=(ci==(char)mk_asciii ? tolower(m_str[ss]) : m_str[ss]);
    if (cc==cci)
      return ss;
    ss+=(dir==mk_asciib ? -1 : 1);
  } while (ss!=ee);

  return -1;

}

Asciistr Asciistr::substr(int idx,int cnt) const {

  if (idx<0)
    idx=0;
  int ll=len();
  if (ll==0 || cnt==0 || ll<=idx)
    return Asciistr();
  if (cnt<0 || ll<idx+cnt)
    cnt=ll-idx;
  char *buf=(char*)malloc((size_t)(cnt+1));
  memcpy(&buf[0],&m_str[idx],(size_t)cnt);
  buf[cnt]=0;
  Asciistr str(buf);
  free(buf);
  return str;

}

Asciistr &Asciistr::append(char cc) {

  if (cc==0)
    return *this;
  int ll=len();
  if (ll+1>=m_sz) {
    char *cpstr=0;
    if (m_str) {
      cpstr=(char*)malloc((size_t)ll);
      memcpy(&cpstr[0],&m_str[0],(size_t)ll);
      free (m_str);
    }
    m_sz=ll+2;
    m_str=(char*)malloc((size_t)m_sz);
    memset(&m_str[0],0,(size_t)m_sz);
    if (cpstr) {
      memcpy(&m_str[0],&cpstr[0],(size_t)ll);
      free (cpstr);
    }
  }
  m_str[ll]=cc;

  return *this;

}

Asciistr &Asciistr::append(const char *str) {

  int ll=len(),sl=(str ? (int)strlen(str) : 0);
  if (sl==0)
    return *this;
  if (ll+sl>=m_sz) {
    char *cpstr=0;
    if (m_str) {
      cpstr=(char*)malloc((size_t)ll);
      memcpy(&cpstr[0],&m_str[0],(size_t)ll);
      free (m_str);
    }
    m_sz=ll+sl+1;
    m_str=(char*)malloc((size_t)m_sz);
    memset(&m_str[0],0,(size_t)m_sz);
    if (cpstr) {
      memcpy(&m_str[0],&cpstr[0],(size_t)ll);
      free (cpstr);
    }
  }
  strcat(m_str,str);

  return *this;

}

Asciistr &Asciistr::prepend(char cc) {

  if (cc==0)
    return *this;
  int ll=len();
  if (ll+1>=m_sz) {
    char *cpstr=0;
    if (m_str) {
      cpstr=(char*)malloc((size_t)ll);
      memcpy(&cpstr[0],&m_str[0],(size_t)ll);
      free (m_str);
    }
    m_sz=ll+2;
    m_str=(char*)malloc((size_t)m_sz);
    memset(&m_str[0],0,(size_t)m_sz);
    m_str[0]=cc;
    if (cpstr) {
      memcpy(&m_str[1],&cpstr[0],(size_t)ll);
      free (cpstr);
    }
  }
  else {
    memmove(&m_str[1],&m_str[0],(size_t)ll);
    m_str[0]=cc;
  }
  return *this;

}

Asciistr &Asciistr::prepend(const char *str) {

  int ll=len(),sl=(str ? (int)strlen(str) : 0);
  if (sl==0)
    return *this;
  if (ll+sl>=m_sz) {
    char *cpstr=0;
    if (m_str) {
      cpstr=(char*)malloc((size_t)ll);
      memcpy(&cpstr[0],&m_str[0],(size_t)ll);
      free (m_str);
    }
    m_sz=ll+sl+1;
    m_str=(char*)malloc((size_t)m_sz);
    memset(&m_str[0],0,(size_t)m_sz);
    memcpy(&m_str[0],&str[0],(size_t)sl);
    if (cpstr) {
      memcpy(&m_str[sl],&cpstr[0],(size_t)ll);
      free (cpstr);
    }
  }
  else {
    memmove(&m_str[sl],&m_str[0],(size_t)ll);
    memcpy(&m_str[0],&str[0],(size_t)sl);
  }
  return *this;

}

Asciistr &Asciistr::lower() {

  int ii=0,ll=len();
  for (ii=0;ii<ll;ii++)
    m_str[ii]=(char)tolower(m_str[ii]);
  return *this;

}

Asciistr &Asciistr::upper() {

  int ii=0,ll=len();
  for (ii=0;ii<ll;ii++)
    m_str[ii]=(char)toupper(m_str[ii]);
  return *this;

}

Asciistr &Asciistr::strip() {

  if (!m_str)
    return *this;
  int ih=(int)strlen(m_str),il=0,ic=0;
  while (--ih>-1) {
    ic=(int)m_str[ih];
    if (mk_asciispace<ic && ic<mk_asciidel)
      break;
    m_str[ih]=0;
  }
  while (il<=ih) {
    ic=(int)m_str[il];
    if (mk_asciispace<ic && ic<mk_asciidel)
      break;
    il++;
  }
  if (il>0) {
    ih-=(il-1);
    memmove(&m_str[0],&m_str[il],(size_t)ih);
    memset(&m_str[ih],0,(size_t)il);
  }
  return *this;

}

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
    int ll=ass.len();
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

  int ll=ass.len();
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

  int ii=0,ll=len();
  if (ll!=cmp.len())
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

  int ii=0,ll=len(),cmpl=cmp.len();
  if (ll!=cmpl)
    return (ll<cmpl);
  for (ii=0;ii<ll;ii++) {
    if (m_str[ii]==cmp.m_str[ii])
      continue;
    return (m_str[ii]<cmp.m_str[ii] ? true : false);
  }
  return false;

}

const unsigned short *Ucsstr::data() const {

  return (m_str ? m_str : &nullUcsstr[0]);

}

int Ucsstr::len() const {

  if (m_str) {
    int ii=0;
    for (ii=0;ii<m_sz;ii++) {
      if (m_str[ii]==0)
        return ii;
    }
  }
  return 0;

}

int Ucsstr::reserve(int cnt) {

  if (cnt<=m_sz)
    return m_sz;
  m_sz=cnt;
  unsigned short *cpstr=0;
  int ll=len();
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

  int oc=(m_str && ii>=0 && ii<len() ? m_str[ii] : -1);
  if (oc>=0)
    m_str[ii]=cc;
  return oc;

}

int Ucsstr::find(unsigned short cc,int idx,unsigned char dir) const {

  int ll=len(),ss=(idx<0 ? ll+idx : idx),ee=(dir==mk_asciib ? -1 : ll);
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
  int ll=len();
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

  int ii=0,ll=len(),sl=(str ? (int)strlen(str) : 0);
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
  int ll=len();
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

  int ll=len(),sl=str.len();
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

  int ii=0,ll=len(),sl=(str ? (int)strlen(str) : 0);
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
  int ll=len();
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

  int ll=len(),sl=str.len();
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

Asciistr Ucsstr::asciistr() const {

  int ii=0,ll=len();
  char *str=(char*)malloc((size_t)(ll+1));
  for (ii=0;ii<ll;ii++)
    str[ii]=(char)(m_str[ii]&255);
  str[ll]=0;
  Asciistr astr(str);
  free (str);
  return astr;

}

int cmpType(const void *t1,const void *t2) {

  return strcmp(((const TypeId*)t1)->type(),((const TypeId*)t2)->type());

}

int cmpIdd(const void *t1,const void *t2) {
 
  if (((const TypeId*)t1)->idd()<((const TypeId*)t2)->idd())
    return -1;
  if (((const TypeId*)t2)->idd()<((const TypeId*)t1)->idd())
    return 1;
  return 0;
 
}

TypeId::TypeId(const char *name,mk_ulreal idd) : m_idd(idd)  {

  memset(&m_type[0],0,mk_sz);
  if (name && strlen(name)>0)
    strncpy(&m_type[0],&name[0],mk_sz-1);
  if (m_idd==0 && strlen(m_type)>0)
    m_idd=mk_nextcnt();

}

TypeId::TypeId(const TypeId &ass) : m_idd(ass.m_idd) {

  if (&ass!=this) {
    memset(&m_type[0],0,mk_sz);
    strcpy(&m_type[0],&ass.m_type[0]);
  }

}

TypeId::~TypeId() {

}

TypeId &TypeId::operator=(const TypeId &ass) {

  m_idd=ass.m_idd;
  if (this!=&ass) {
    memset(&m_type[0],0,mk_sz);
    strcpy(&m_type[0],&ass.m_type[0]);
  }
  return *this;

}

bool TypeId::operator==(const TypeId &cmp) const {

  return (m_idd==cmp.m_idd && strcmp(m_type,cmp.m_type)==0);

}

bool TypeId::operator<(const TypeId &cmp) const {

  int typecmp=strcmp(m_type,cmp.m_type);
  return (typecmp<0 || (typecmp==0 && m_idd<cmp.m_idd));

}

TypeId *TypeId::clone() const {

  return new TypeId(&m_type[0],m_idd);

}

int TypeId::busted() const {

  return ((m_idd==0 || strlen(m_type)==0) ? 1 : 0);

}

const char *TypeId::type() const {

  return (const char*)(&m_type[0]);

}

mk_ulreal TypeId::idd() const {

  return m_idd;

}

static const unsigned short alnumlen=13;

void TypeId::toString(Asciistr *str) const {

  if (!str)
    return;
  str->reserve(mk_sz+alnumlen);
  str->append(m_type);
  Asciistr nstr;
  ui2a (m_idd,&nstr,mk_maxintbase,alnumlen);
  str->append(nstr);

}

int TypeId::fromString(const char *s) {

  if (!s)
    return 0;
  int ii=0,jj=0,base=-1,len=(int)strlen(s);
  if (len<alnumlen)
    return 0;
  char iddbuf[alnumlen+1];
  for (ii=len-alnumlen,jj=0;ii<len;ii++)
    iddbuf[jj++]=s[ii];
  iddbuf[alnumlen]=0;
  mk_ulreal idd=mk_a2ui(iddbuf,&base,0,0);
  if (base<0)
    return 0;
  m_idd=idd;
  memset(&m_type[0],0,mk_sz);
  len-=alnumlen;
  if (len>mk_sz)
    len=mk_sz;
  strncpy(&m_type[0],s,(size_t)len);
  return 1;

}

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

Asciistr Rounded::toString(char fmt,int pad,const char *dec,const char *group) const {

  aux::Asciistr str;
  d2a(m_d,&str,m_a,fmt,pad,dec,group);
  return str;

}


} // namespace
