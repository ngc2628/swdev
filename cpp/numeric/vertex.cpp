
#include <numeric/vertex.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>

namespace num {

Vertex::Vertex() {

  mk_vertexset(m_tuple,mk_dnan);

}

Vertex::Vertex(double x) {

  m_tuple[0]=x;
  m_tuple[1]=m_tuple[2]=.0;
  m_tuple[3]=1.;

}

Vertex::Vertex(double x,double y) {

  m_tuple[0]=x;
  m_tuple[1]=y;
  m_tuple[2]=.0;
  m_tuple[3]=1.;

}

Vertex::Vertex(double x,double y,double z) {

  m_tuple[0]=x;
  m_tuple[1]=y;
  m_tuple[2]=z;
  m_tuple[3]=1.;

}

Vertex::Vertex(double *xyzw) {

  mk_vertexcopy(m_tuple,xyzw);

}

Vertex::Vertex(const Vertex &ass) {

  mk_vertexcopy(m_tuple,ass.m_tuple);

}

Vertex::~Vertex() {

}

Vertex &Vertex::operator=(const Vertex &ass) {

  mk_vertexcopy(m_tuple,ass.m_tuple);
  return *this;

}

bool Vertex::operator==(const Vertex &cmp) const {

  return (mk_deq(m_tuple[0],cmp.m_tuple[0]) &&
          mk_deq(m_tuple[1],cmp.m_tuple[1]) &&
          mk_deq(m_tuple[2],cmp.m_tuple[2]) &&
          mk_deq(m_tuple[3],cmp.m_tuple[3]));

}

bool Vertex::operator<(const Vertex &cmp) const {

  return (mk_dlt(m_tuple[0],cmp.m_tuple[0]) || 
          (mk_deq(m_tuple[0],cmp.m_tuple[0]) && (mk_dlt(m_tuple[1],cmp.m_tuple[1]) ||
          (mk_deq(m_tuple[1],cmp.m_tuple[1]) && (mk_dlt(m_tuple[2],cmp.m_tuple[2]) ||
          (mk_deq(m_tuple[2],cmp.m_tuple[2]) && mk_dlt(m_tuple[3],cmp.m_tuple[3])))))));

}

double Vertex::operator[](int type) const {

  return (type<0 || type>3 ? mk_dnan : m_tuple[type]);

}

Vertex Vertex::operator+(const Vertex &other) const {

  mk_vertex vertex;
  mk_vertexcopy(vertex,m_tuple);
  mk_vertexadd(vertex,other.m_tuple);
  Vertex res(&vertex[0]);
  return res;

}

Vertex Vertex::operator-(const Vertex &other) const {

  mk_vertex vertex;
  mk_vertexcopy(vertex,m_tuple);
  mk_vertexsubs(vertex,other.m_tuple);
  Vertex res(&vertex[0]);
  return res;

}

Vertex Vertex::operator*(double sc) const {

  mk_vertex vertex;
  mk_vertexcopy(vertex,m_tuple);
  mk_vertexmult(vertex,sc);
  Vertex res(&vertex[0]);
  return res;

}

Vertex Vertex::operator/(double sc) const {

  mk_vertex vertex;
  mk_vertexcopy(vertex,m_tuple);
  mk_vertexdiv(vertex,sc);
  Vertex res(&vertex[0]);
  return res;

}

int Vertex::busted(int type) const {

  type=(abs(type)&typeXYZW);
  int res=0;
  if ((type&typeX)>0) {
    if (mk_isbusted(m_tuple[0])!=0)
      res|=typeX;
  }
  if ((type&typeY)>0) {
    if (mk_isbusted(m_tuple[1])!=0)
      res|=typeY;
  }
  if ((type&typeZ)>0) {
    if (mk_isbusted(m_tuple[2])!=0)
      res|=typeZ;
  }
  if ((type&typeW)>0) {
    if (mk_isbusted(m_tuple[3])!=0)
      res|=typeW;
  }
  return res;

}

double *Vertex::data() {

  return &m_tuple[0];

}

double Vertex::x() const {

  return m_tuple[0];

}

void Vertex::setX(double x) {

  m_tuple[0]=x;

}

double Vertex::y() const {

  return m_tuple[1];

}

void Vertex::setY(double y) {

  m_tuple[1]=y;

}

double Vertex::z() const {

  return m_tuple[2];

}

void Vertex::setZ(double z) {

  m_tuple[2]=z;

}

double Vertex::w() const {

  return m_tuple[3];

}

void Vertex::setW(double w) {

  m_tuple[3]=w;

}

void Vertex::setXY(double x,double y) {

  m_tuple[0]=x;
  m_tuple[1]=y;

}

void Vertex::setXYZ(double x,double y,double z) {

  m_tuple[0]=x;
  m_tuple[1]=y;
  m_tuple[2]=z;

}

void Vertex::set(double *xyzw) {

  if (!xyzw)
    mk_vertexset(m_tuple,mk_dnan);
  else
    mk_vertexcopy(m_tuple,xyzw);

}

double Vertex::len() const {

  return mk_vertexlen(m_tuple);

}

double Vertex::dot(const Vector3 &other) const {

  return mk_vertexdot(m_tuple,other.m_tuple);

}

Vertex Vertex::norm() const {
 
  mk_vertex vertex;
  mk_vertexcopy(vertex,m_tuple);
  mk_vertexnorm(vertex);
  return Vertex(vertex[0],vertex[1],vertex[2]);

}

Vertex Vertex::cross(const Vertex &other) const {

  mk_vertex vertex;
  mk_vertexcopy(vertex,m_tuple);
  mk_vertexcross(vertex,other.m_tuple);
  return Vertex(vertex[0],vertex[1],vertex[2]);

}

double Vertex::angrad(const Vertex &other) const {

  double den=len()*other.len();
  if (den==.0)
    return .0;
  return acos(dot(other)/den);
  //return atan2(dot(other)/den,sqrt(1.-(dot(other)/den)*(dot(other)/den)));

}

double Vertex::angdeg(const Vertex &other) const {

  return mk_vertexangdeg(m_tuple,other.m_tuple);

}

int Vertex::toString(mk_string str,int prec) const {

  mk_stringappend(str,"Vertex : [");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_d2a(m_tuple[0],numstr,prec);
  mk_stringappend(str,numstr);
  mk_stringappend(str,";");
  mk_d2a(m_tuple[1],numstr,prec);
  mk_stringappend(str,numstr);
  mk_stringappend(str,";");
  mk_d2a(m_tuple[2],numstr,prec);
  mk_stringappend(str,numstr);
  mk_stringappend(str,";");
  mk_d2a(m_tuple[3],numstr,prec);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"]");
  return 0;

}

void swapvertex(Vertex *v1,Vertex *v2) {

  mk_vertexswap(v1->data(),v2->data());

}

double lineRot(Vertex l11,Vertex l12,Vertex l21,Vertex l22,Vertex *rotax) {

  Vertex v1=l12-l11,v2=l22-l21;
  v1=v1.norm();
  v2=v2.norm();
  if (rotax)
    *rotax=v1.cross(v2);
  return v1.angrad(v2);

}

double mindistpointplane(Vertex v1,Vertex v2,Vertex v3,Vertex v4,Vertex *vinter) {
  
  Vertex vn=(v2-v1).cross(v3-v1);
  double lvn=vn.len(),dist=(lvn==.0 ? mk_dnan : vn.dot(v4-v1)/lvn);
  if (vinter)
    *vinter=(lvn==.0 ? Vertex() : v4-vn*(dist/lvn));
  return dist;
  
}

double mindistpointline(Vertex v1,Vertex v2,Vertex v3,Vertex *vinter) {

  return mindistpointplane(v1,v2,v1-(v2-v1).cross(v3-v1),v3,vinter);
  
}

VertexList::VertexList(int sz) {

  mk_listalloc(&m_list,sizeof(mk_vertex),sz);

}

VertexList::VertexList(const VertexList &ass) {

  int ii=0,sz=0,cnt=ass.count(&sz);
  mk_listalloc(&m_list,sizeof(mk_vertex),sz);
  mk_vertexnan(vv);
  for (ii=0;ii<cnt;ii++) {
    mk_listat(&ass.m_list,ii,(void *)&vv);
    mk_listsetat(&m_list,(void *)&vv,ii,1);
  }

}
 
VertexList &VertexList::operator=(const VertexList &ass) {

  if (&ass==this)
    return *this;
  mk_listfree(&m_list);
  int ii=0,sz=0,cnt=ass.count(&sz);
  mk_listalloc(&m_list,sizeof(mk_vertex),sz);
  mk_vertexnan(vv);
  for (ii=0;ii<cnt;ii++) {
    mk_listat(&ass.m_list,ii,(void *)&vv);
    mk_listsetat(&m_list,(void *)&vv,ii,1);
  }
  return *this;

}

bool VertexList::operator==(const VertexList &that) const {

  return (this==&that);

}   

bool VertexList::operator<(const VertexList &that) const {

  return (mk_ulreal)((void*)this)<(mk_ulreal)((void*)&that);

}
 
Vertex VertexList::get(int idx,int *inlist) const {

  Vertex res;
  int inlist_=mk_listat(&m_list,idx,(void *)(res.data()));
  if (inlist)
    *inlist=inlist_;
  return res;

}
 
int VertexList::set(int idx,Vertex vertex,int insert) {

  return mk_listsetat(&m_list,(void *)(vertex.data()),idx,insert);

}

int VertexList::append(Vertex vertex) {

  if (m_list.count==m_list.reserved)
    resize(m_list.count+1);
  return mk_listsetat(&m_list,(void *)(vertex.data()),m_list.count,1);

}
 
int VertexList::clear() {

  mk_vertexnan(vv);
  return mk_listclear(&m_list,(void*)&vv[0]);

}

int VertexList::resize(int sz) {

  return mk_listrealloc(&m_list,sz);

}

int VertexList::count(int *reserved) const {

  if (reserved)
    *reserved=m_list.reserved;
  return m_list.count;

}

int VertexList::sort(int idx) {

  if (idx<0 || idx>3) {
    m_list.cmp=0;
    m_list.sorted=0;
    return 1;
  }
  m_list.cmp=mk_vertexcmp[idx];
  return mk_listsort(&m_list);

}

int VertexList::sorted() const {

  return m_list.sorted;

}

int VertexList::find(Vertex vertex) const {

  if ((m_list.sorted&1)==0 || m_list.cmp==0)
    return -1;
  return mk_listfind(&m_list,(void *)(vertex.data()),0,0);

}

int VertexList::findNextIndex(Vertex vertex) const {

  if ((m_list.sorted&1)==0 || m_list.cmp==0)
    return -1;
  return mk_listfindnextindex(&m_list,(void *)(vertex.data()));

}

int VertexList::findInterval(Vertex vertex,int *idxl,int*idxh) const {

  if ((m_list.sorted&1)==0 || m_list.cmp==0)
    return 1;
  return mk_binsearchinterval(
    (void *)(vertex.data()),m_list.typesize,m_list.count,m_list.arr,m_list.cmp,idxl,idxh,0);

}

int VertexList::inSort(Vertex vertex) {

  if ((m_list.sorted&1)==0 && m_list.cmp==0)
    return -1;
  if (m_list.count==m_list.reserved)
    resize(m_list.count+1);
  return mk_listinsort(&m_list,(void *)(vertex.data()));

}

int VertexList::remove(int idx) {

  Vertex vertex;
  return mk_listremove(&m_list,idx,(void *)&vertex);
    
}

} // namespace
