
#include <auxx/auxx.h>
#include <auxx/vertex.h>

namespace aux {

Vertex::Vertex() {

  m_xyzw[0]=m_xyzw[1]=m_xyzw[2]=m_xyzw[3]=mk_dnan;

}

Vertex::Vertex(double x) {

  m_xyzw[0]=x;
  m_xyzw[1]=m_xyzw[2]=.0;
  m_xyzw[3]=1.;

}

Vertex::Vertex(double x,double y) {

  m_xyzw[0]=x;
  m_xyzw[1]=y;
  m_xyzw[2]=.0;
  m_xyzw[3]=1.;

}

Vertex::Vertex(double x,double y,double z) {

  m_xyzw[0]=x;
  m_xyzw[1]=y;
  m_xyzw[2]=z;
  m_xyzw[3]=1.;

}

Vertex::Vertex(double x,double y,double z,double w) {

  m_xyzw[0]=x;
  m_xyzw[1]=y;
  m_xyzw[2]=z;
  m_xyzw[3]=w;

}

Vertex::Vertex(double *xyzw) {

  m_xyzw[0]=xyzw[0];
  m_xyzw[1]=xyzw[1];
  m_xyzw[2]=xyzw[2];
  m_xyzw[3]=xyzw[3];

}

Vertex::Vertex(const Vertex &ass) {

  m_xyzw[0]=ass.m_xyzw[0];
  m_xyzw[1]=ass.m_xyzw[1];
  m_xyzw[2]=ass.m_xyzw[2];
  m_xyzw[3]=ass.m_xyzw[3];

}

Vertex::~Vertex() {

}

Vertex &Vertex::operator=(const Vertex &ass) {

  m_xyzw[0]=ass.m_xyzw[0];
  m_xyzw[1]=ass.m_xyzw[1];
  m_xyzw[2]=ass.m_xyzw[2];
  m_xyzw[3]=ass.m_xyzw[3];
  return *this;

}

bool Vertex::operator==(const Vertex &cmp) const {

  return (mk_deq(m_xyzw[0],cmp.m_xyzw[0]) &&
          mk_deq(m_xyzw[1],cmp.m_xyzw[1]) &&
          mk_deq(m_xyzw[2],cmp.m_xyzw[2]) &&
          mk_deq(m_xyzw[3],cmp.m_xyzw[3]));

}

bool Vertex::operator<(const Vertex &cmp) const {

  return (mk_dlt(m_xyzw[0],cmp.m_xyzw[0]) || 
          (mk_deq(m_xyzw[0],cmp.m_xyzw[0]) && (mk_dlt(m_xyzw[1],cmp.m_xyzw[1]) ||
          (mk_deq(m_xyzw[1],cmp.m_xyzw[1]) && (mk_dlt(m_xyzw[2],cmp.m_xyzw[2]) ||
          (mk_deq(m_xyzw[2],cmp.m_xyzw[2]) && mk_dlt(m_xyzw[3],cmp.m_xyzw[3])))))));

}

double Vertex::operator[](int type) const {

  return (type<0 || type>3 ? mk_dnan : m_xyzw[type]);

}

Vertex Vertex::operator+(const Vertex &other) const {

  return Vertex (
    mk_isBusted(m_xyzw[0])==0 && mk_isBusted(other.m_xyzw[0])==0 ? 
      m_xyzw[0]+other.m_xyzw[0] : mk_dnan,
    mk_isBusted(m_xyzw[1])==0 && mk_isBusted(other.m_xyzw[1])==0 ? 
      m_xyzw[1]+other.m_xyzw[1] : mk_dnan,
    mk_isBusted(m_xyzw[2])==0 && mk_isBusted(other.m_xyzw[2])==0 ? 
      m_xyzw[2]+other.m_xyzw[2] : mk_dnan,
    mk_isBusted(m_xyzw[3])==0 && mk_isBusted(other.m_xyzw[3])==0 ? 
      m_xyzw[3]+other.m_xyzw[3] : mk_dnan
  );

}

Vertex Vertex::operator-(const Vertex &other) const {

  return Vertex (
    mk_isBusted(m_xyzw[0])==0 && mk_isBusted(other.m_xyzw[0])==0 ? 
      m_xyzw[0]-other.m_xyzw[0] : mk_dnan,
    mk_isBusted(m_xyzw[1])==0 && mk_isBusted(other.m_xyzw[1])==0 ? 
      m_xyzw[1]-other.m_xyzw[1] : mk_dnan,
    mk_isBusted(m_xyzw[2])==0 && mk_isBusted(other.m_xyzw[2])==0 ? 
      m_xyzw[2]-other.m_xyzw[2] : mk_dnan,
    mk_isBusted(m_xyzw[3])==0 && mk_isBusted(other.m_xyzw[3])==0 ? 
      m_xyzw[3]-other.m_xyzw[3] : mk_dnan
  );

}

Vertex Vertex::operator*(double sc) const {

  int bsc=mk_isBusted(sc);
  Vertex vsc(
    mk_isBusted(m_xyzw[0])==0 && bsc==0 ? m_xyzw[0]*sc : mk_dnan,
    mk_isBusted(m_xyzw[1])==0 && bsc==0 ? m_xyzw[1]*sc : mk_dnan,
    mk_isBusted(m_xyzw[2])==0 && bsc==0 ? m_xyzw[2]*sc : mk_dnan,
    mk_isBusted(m_xyzw[3])==0 && bsc==0 ? m_xyzw[3]*sc : mk_dnan
  );
  return vsc;

}

Vertex Vertex::operator/(double sc) const {

  int bsc=mk_isBusted(sc);
  Vertex vsc(
    mk_isBusted(m_xyzw[0])==0 && bsc==0 && sc!=.0 ? m_xyzw[0]/sc : mk_dnan,
    mk_isBusted(m_xyzw[1])==0 && bsc==0 && sc!=.0 ? m_xyzw[1]/sc : mk_dnan,
    mk_isBusted(m_xyzw[2])==0 && bsc==0 && sc!=.0 ? m_xyzw[2]/sc : mk_dnan,
    mk_isBusted(m_xyzw[3])==0 && bsc==0 && sc!=.0 ? m_xyzw[3]/sc : mk_dnan
  );
  return vsc;

}

int Vertex::busted(int type) const {

  type=(abs(type)&typeXYZW);
  int res=0;
  if ((type&typeX)>0) {
    if (mk_isBusted(m_xyzw[0])!=0)
      res|=typeX;
  }
  if ((type&typeY)>0) {
    if (mk_isBusted(m_xyzw[1])!=0)
      res|=typeY;
  }
  if ((type&typeZ)>0) {
    if (mk_isBusted(m_xyzw[2])!=0)
      res|=typeZ;
  }
  if ((type&typeW)>0) {
    if (mk_isBusted(m_xyzw[3])!=0)
      res|=typeW;
  }
  return res;

}

double *Vertex::data() {

  return &m_xyzw[0];

}

double Vertex::x() const {

  return m_xyzw[0];

}

void Vertex::setX(double x) {

  m_xyzw[0]=x;

}

double Vertex::y() const {

  return m_xyzw[1];

}

void Vertex::setY(double y) {

  m_xyzw[1]=y;

}

double Vertex::z() const {

  return m_xyzw[2];

}

void Vertex::setZ(double z) {

  m_xyzw[2]=z;

}

double Vertex::w() const {

  return m_xyzw[3];

}

void Vertex::setW(double w) {

  m_xyzw[3]=w;

}

void Vertex::setXY(double x,double y) {

  m_xyzw[0]=x;
  m_xyzw[1]=y;

}

void Vertex::setXYZ(double x,double y,double z) {

  m_xyzw[0]=x;
  m_xyzw[1]=y;
  m_xyzw[2]=z;

}

void Vertex::set(double x,double y,double z,double w) {

  m_xyzw[0]=x;
  m_xyzw[1]=y;
  m_xyzw[2]=z;
  m_xyzw[3]=w;

}

double Vertex::len() const {

  double p[3]={mk_isBusted(m_xyzw[0])==0 ? m_xyzw[0] : .0,
               mk_isBusted(m_xyzw[1])==0 ? m_xyzw[1] : .0,
               mk_isBusted(m_xyzw[2])==0 ? m_xyzw[2] : .0};
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);

}

double Vertex::dot(const Vector3 &other) const {

  double p[2][3]={{mk_isBusted(m_xyzw[0])==0 ? m_xyzw[0] : .0,
                   mk_isBusted(m_xyzw[1])==0 ? m_xyzw[1] : .0,
                   mk_isBusted(m_xyzw[2])==0 ? m_xyzw[2] : .0},
                  {mk_isBusted(other.m_xyzw[0])==0 ? other.m_xyzw[0] : .0,
                   mk_isBusted(other.m_xyzw[1])==0 ? other.m_xyzw[1] : .0,
                   mk_isBusted(other.m_xyzw[2])==0 ? other.m_xyzw[2] : .0}};
  return (p[0][0]*p[1][0]+p[0][1]*p[1][1]+p[0][2]*p[1][2]);

}

Vertex Vertex::norm() const {

  double nn=len();
  return Vertex((nn>.0 && mk_isBusted(m_xyzw[0])==0) ? m_xyzw[0]/nn : mk_dnan,
                 (nn>.0 && mk_isBusted(m_xyzw[1])==0) ? m_xyzw[1]/nn : mk_dnan,
                 (nn>.0 && mk_isBusted(m_xyzw[2])==0) ? m_xyzw[2]/nn : mk_dnan);

}

Vertex Vertex::cross(const Vertex &other) const {

  return Vertex(
          ((mk_isBusted(m_xyzw[1])==0 && mk_isBusted(m_xyzw[2])==0 && 
            mk_isBusted(other.m_xyzw[1])==0 && mk_isBusted(other.m_xyzw[2])==0) ?  
           m_xyzw[1]*other.m_xyzw[2]-m_xyzw[2]*other.m_xyzw[1] : mk_dnan),
          ((mk_isBusted(m_xyzw[0])==0 && mk_isBusted(m_xyzw[2])==0 && 
            mk_isBusted(other.m_xyzw[0])==0 && mk_isBusted(other.m_xyzw[2])==0) ?  
           m_xyzw[2]*other.m_xyzw[0]-m_xyzw[0]*other.m_xyzw[2] : mk_dnan),
          ((mk_isBusted(m_xyzw[0])==0 && mk_isBusted(m_xyzw[1])==0 && 
            mk_isBusted(other.m_xyzw[0])==0 && mk_isBusted(other.m_xyzw[1]))==0 ?  
           m_xyzw[0]*other.m_xyzw[1]-m_xyzw[1]*other.m_xyzw[0] : mk_dnan)
  );

}

double Vertex::angrad(const Vertex &other) const {

  double den=len()*other.len();
  if (den==.0)
    return .0;
  return acos(dot(other)/den);
  //return atan2(dot(other)/den,sqrt(1.-(dot(other)/den)*(dot(other)/den)));

}

double Vertex::angdeg(const Vertex &other) const {

  double den=len()*other.len();
  if (den==.0)
    return .0;
  double arg=dot(other)/den;
  return acos(arg)/mk_rad;
  //return atan2(sqrt(1.-arg*arg),arg)/rad;

}

void Vertex::toString(Asciistr *str,int prec) const {

  if (!str)
    return;
  char buf[mk_klen];
  Asciistr numstr;
  strcpy(buf,"Vertex : [");
  d2a(m_xyzw[0],&numstr,(short)prec);
  strcat(buf,numstr);
  strcat(buf,";");
  d2a(m_xyzw[1],&numstr,(short)prec);
  strcat(buf,numstr);
  strcat(buf,";");
  d2a(m_xyzw[2],&numstr,(short)prec);
  strcat(buf,numstr);
  strcat(buf,";");
  d2a(m_xyzw[3],&numstr,(short)prec);
  strcat(buf,numstr);
  strcat(buf,"]");
  *str=buf;

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
  
  aux::Vertex vn=(v2-v1).cross(v3-v1);
  double lvn=vn.len(),dist=(lvn==.0 ? mk_dnan : vn.dot(v4-v1)/lvn);
  if (vinter)
    *vinter=(lvn==.0 ? Vertex() : v4-vn*(dist/lvn));
  return dist;
  
}

double mindistpointline(Vertex v1,Vertex v2,Vertex v3,Vertex *vinter) {

  return mindistpointplane(v1,v2,v1-(v2-v1).cross(v3-v1),v3,vinter);
  
}

int cmpVectorI(const void *v,const void *w,int i) {

  if (mk_dlt(((const Vertex*)v)->operator[](i),((const Vertex*)w)->operator[](i)))
    return -1;
  if (mk_dlt(((const Vertex*)w)->operator[](i),((const Vertex*)v)->operator[](i)))
    return 1;
  return 0;

}

int cmpVector0(const void *v,const void *w) {

  return cmpVectorI(v,w,0);

}

int cmpVector1(const void *v,const void *w) {

  return cmpVectorI(v,w,1);

}

int cmpVector2(const void *v,const void *w) {

  return cmpVectorI(v,w,2);

}

int cmpVector3(const void *v,const void *w) {

  return cmpVectorI(v,w,3);

}

} // namespace
