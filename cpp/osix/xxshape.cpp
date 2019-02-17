
#include <numeric/matrix.h>
#include <numeric/vertex.h>
#include <osix/xxshape.h>
#include <mkbase/mkconv.h>
#include <stdio.h>
#include <string.h>
//#include <sys/param.h>

namespace osix {

xxRectSize &xxRectSize::operator=(const xxRectSize &ass) {
      
  m_width=ass.m_width;
  m_height=ass.m_height;
  return *this;
  
}

bool xxRectSize::operator==(const xxRectSize &cmp) const {

  return (m_width==cmp.m_width && m_height==cmp.m_height);

}

bool xxRectSize::operator<(const xxRectSize &cmp) const {

  return (m_width*m_height<cmp.m_width*cmp.m_height);

}

double xxRectSize::setWidth(double ww) {

  m_width=(mk_isbusted(ww)!=0 || ww<0. ? 0. : ww);
  return m_width;

}

double xxRectSize::setHeight(double hh) {

  m_height=(mk_isbusted(hh)!=0 || hh<0. ? 0. : hh);
  return m_height;

}

xxRectSize xxRectSize::set(double ww,double hh) {

  m_width=(mk_isbusted(ww)!=0 || ww<0. ? 0. : ww);
  m_height=(mk_isbusted(hh)!=0 || hh<0. ? 0. : hh); 
  return *this;
  
}

double xxRectSize::width() const {

  return m_width;

}

double xxRectSize::height() const {

  return m_height;

}

bool xxRectSize::empty() const {

  return (m_width<=.0 || m_height<=.0);

}

int xxRectSize::toString(mk_string str) const {

  mk_stringappend(str,"w=");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_d2a(m_width,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," , h=");
  mk_d2a(m_height,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\n");
  return 0;
    
}

void xxLine::set(num::Vector3 p0,num::Vector3 p1) {
      
  if (mk_dlt(p1.x(),p0.x()))
    num::swapvertex(&p0,&p1);
  m_l[0]=p0; 
  m_l[1]=p1;
  
}

void xxLine::setP0(num::Vector3 p) {

  if (p.busted(num::typeX|num::typeY)!=0) {
    set();
    return;
  }
  if (busted()!=0)
    m_l[1]=p;
  m_l[0]=p;
  if (m_l[1].x()<m_l[0].x())
    num::swapvertex(&m_l[0],&m_l[1]);

}

void xxLine::setP1(num::Vector3 p) {

  if (p.busted(num::typeX|num::typeY)!=0) {
    set();
    return;
  }
  if (busted()!=0)
    m_l[0]=p;
  m_l[1]=p;
  if (m_l[1].x()<m_l[0].x())
    num::swapvertex(&m_l[0],&m_l[1]);

}
 
double xxLine::len() const {

  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_l[0].x(),-m_l[0].y());
  num::Vector3 p1(m_l[1]);
  m.transform(&p1);
  return p1.len();  

}

int xxLine::busted() const {
      
  return ((m_l[0].busted(num::typeX|num::typeY)>0 || 
          m_l[1].busted(num::typeX|num::typeY)>0) ? 1 : 0);
    
}

int xxLine::empty() const {
      
  return ((busted()>0 || m_l[0]==m_l[1]) ? 1 : 0);
  
}

double xxLine::angdeg() const {

  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_l[0].x(),-m_l[0].y());
  num::Vector3 p0(0.,1.),p1(m_l[1]);
  m.transform(&p1);
  return p1.angdeg(p0);

}
 
double xxLine::angrad() const {

  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_l[0].x(),-m_l[0].y());
  num::Vector3 p0(0.,1.),p1(m_l[1]);
  m.transform(&p1);
  return p1.angrad(p0);

}

int xxLine::toString(mk_string str) const {

  mk_stringappend(str,"p0=");
  mk_string vstr;
  m_l[0].toString(vstr);
  mk_stringappend(str,vstr);
  mk_stringappend(str," ; p1=");
  m_l[1].toString(vstr);
  mk_stringappend(str,vstr);
  return 0;

}

xxRect::xxRect(double left,double top,double right,double bottom) {
  
  if (mk_isbusted(left)!=0 || mk_isbusted(top)!=0 || 
      mk_isbusted(right)!=0 || mk_isbusted(bottom)!=0) {
    set();
    return;
  }
  if (right<left)
    mk_swapf(&left,&right);
  if (bottom<top)
    mk_swapf(&top,&bottom);
  m_r[0]=left;
  m_r[1]=top;
  m_r[2]=right;
  m_r[3]=bottom;

}

xxRect::xxRect(num::Vector3 lt,num::Vector3 br) {

  if (lt.busted(num::typeX|num::typeY)!=0 || br.busted(num::typeX|num::typeY)!=0) {
    set();
    return;
  }
  m_r[0]=MIN(lt[0],br[0]);
  m_r[1]=MIN(lt[1],br[1]);
  m_r[2]=MAX(lt[0],br[0]);
  m_r[3]=MAX(lt[1],br[1]);
       
}

xxRect::xxRect(double left,double top,xxRectSize size) {

  if (mk_isbusted(left)!=0 || mk_isbusted(top)!=0) {
    set();
    return;
  }
  m_r[0]=left;
  m_r[1]=top;
  m_r[2]=left+size.width();
  m_r[3]=top+size.height();
  
} 
      
xxRect::xxRect(num::Vector3 lt,xxRectSize size) {

  if (lt.busted(num::typeX|num::typeY)!=0) {
    set();
    return;
  }
  m_r[0]=lt[0];
  m_r[1]=lt[1];
  m_r[2]=m_r[0]+size.width();
  m_r[3]=m_r[1]+size.height();

}

xxRect xxRect::operator+(double add) {

  if (busted()!=0)
    return xxRect(*this);
  return xxRect(m_r[0]-add,m_r[1]-add,m_r[2]+add,m_r[3]+add);

}

xxRect xxRect::operator-(double add) {

  if (busted()!=0)
    return xxRect(*this);
  return xxRect(m_r[0]+add,m_r[1]+add,m_r[2]-add,m_r[3]-add);

}
    
bool xxRect::operator==(const xxRect &cmp) const {

  if (busted()!=0) {
    if (cmp.busted()!=0)
      return true;
    return false;
  }
  if (cmp.busted()!=0)
    return false;
  int i=0;
  for (i=0;i<4;i++) {
    if (m_r[i]!=cmp.m_r[i])
      return false;
  }
  return true;

}

bool xxRect::operator<(const xxRect &cmp) const {
      
  if (busted()!=0)
    return false;
  if (cmp.busted()!=0)
    return true;
  double tmp1=area(),tmp2=cmp.area();
  if (tmp1<tmp2)
    return true;
  if (tmp1==tmp2) {
    tmp1=circ();
    tmp2=cmp.circ();
    if (tmp1<tmp2)
      return true;
    if (tmp1==tmp2)
      return (m_r[0]<cmp.m_r[0] || (m_r[0]==cmp.m_r[0] && m_r[1]<cmp.m_r[1]));
  }
  return false;
  
}

int xxRect::busted() const {
      
  return ((mk_isbusted(m_r[0])!=0 || mk_isbusted(m_r[1])!=0 || 
          mk_isbusted(m_r[2])!=0 || mk_isbusted(m_r[3])!=0) ? 1 : 0);
   
}

int xxRect::empty() const {
      
  return (busted()!=0 ? 1 : (m_r[2]<=m_r[0] || m_r[3]<=m_r[1] ? 1 : 0));
  
}

void xxRect::set(double left,double top,double right,double bottom) {
      
  if (mk_dlt(right,left))
    mk_swapf(&left,&right);
  if (mk_dlt(bottom,top))
    mk_swapf(&top,&bottom);
  m_r[0]=left;
  m_r[1]=top;
  m_r[2]=right;
  m_r[3]=bottom;
  
}

void xxRect::set(double left,double top,xxRectSize sz) {
      
  if (mk_isbusted(left)!=0 || mk_isbusted(top)!=0) {
    set();
    return;
  }
  m_r[0]=left;
  m_r[1]=top;
  m_r[2]=m_r[0]+sz.width();
  m_r[3]=m_r[1]+sz.height();
  
}

void xxRect::set(num::Vector3 lt,num::Vector3 br) {

  if (lt.busted(num::typeX|num::typeY)!=0 || br.busted(num::typeX|num::typeY)!=0) {
    set();
    return;
  }
  m_r[0]=MIN(lt[0],br[0]);
  m_r[1]=MIN(lt[1],br[1]);
  m_r[2]=MAX(lt[0],br[0]);
  m_r[3]=MAX(lt[1],br[1]);

}

double xxRect::setLeft(double left) {

  if (mk_isbusted(left)!=0) {
    set();
    return mk_dnan;
  }
  if (busted()!=0) {
    m_r[0]=m_r[1]=m_r[2]=m_r[3]=left;
    return .0;
  }
  m_r[0]=left; 
  if (m_r[2]<m_r[0])
    m_r[2]=m_r[0];
  return (m_r[2]-m_r[0]); 
  
}

double xxRect::setTop(double top) {

  if (mk_isbusted(top)!=0) {
    set();
    return mk_dnan;
  }
  if (busted()!=0) {
    m_r[0]=m_r[1]=m_r[2]=m_r[3]=top;
    return .0;
  }
  m_r[1]=top; 
  if (m_r[3]<m_r[1])
    m_r[3]=m_r[1];
  return (m_r[3]-m_r[1]);
  
}

double xxRect::setRight(double right) {

  if (mk_isbusted(right)!=0) {
    set();
    return mk_dnan;
  }
  if (busted()!=0) {
    m_r[0]=m_r[1]=m_r[2]=m_r[3]=right;
    return .0;
  }
  m_r[2]=right; 
  if (m_r[2]<m_r[0])
    m_r[0]=m_r[2];
  return (m_r[2]-m_r[0]);

}

double xxRect::setBottom(double bottom) {

  if (mk_isbusted(bottom)!=0) {
    set();
    return mk_dnan;
  }
  if (busted()!=0) {
    m_r[0]=m_r[1]=m_r[2]=m_r[3]=bottom;
    return .0;
  }
  m_r[3]=bottom; 
  if (m_r[3]<m_r[1])
    m_r[1]=m_r[3];
  return (m_r[3]-m_r[1]); 

}

xxRectSize xxRect::setLeftTop(num::Vector3 lt) {

  if (lt.busted(num::typeX|num::typeY)!=0) {
    set();
    return xxRectSize();
  }
  if (busted()!=0) {
    m_r[0]=m_r[2]=lt[0];
    m_r[1]=m_r[3]=lt[1];
    return .0;
  }
  m_r[0]=lt[0]; m_r[1]=lt[1];
  if (m_r[2]<m_r[0])
    m_r[2]=m_r[0];
  if (m_r[3]<m_r[1])
    m_r[3]=m_r[1];
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);
  
}

xxRectSize xxRect::setLeftBottom(num::Vector3 lb) {

  if (lb.busted(num::typeX|num::typeY)!=0) {
    set();
    return xxRectSize();
  }
  if (busted()!=0) {
    m_r[0]=m_r[2]=lb[0];
    m_r[1]=m_r[3]=lb[1];
    return .0;
  }
  m_r[0]=lb[0]; m_r[3]=lb[1];
  if (m_r[2]<m_r[0])
    m_r[2]=m_r[0];
  if (m_r[3]<m_r[1])
    m_r[1]=m_r[3];
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);

}

xxRectSize xxRect::setRightTop(num::Vector3 rt) {

  if (rt.busted(num::typeX|num::typeY)!=0) {
    set();
    return xxRectSize();
  }
  if (busted()!=0) {
    m_r[0]=m_r[2]=rt[0];
    m_r[1]=m_r[3]=rt[1];
    return .0;
  }
  m_r[2]=rt[0]; m_r[1]=rt[1];
  if (m_r[2]<m_r[0])
    m_r[0]=m_r[2];
  if (m_r[3]<m_r[1])
    m_r[3]=m_r[1];
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);

}

xxRectSize xxRect::setRightBottom(num::Vector3 rb) {

  if (rb.busted(num::typeX|num::typeY)!=0) {
    set();
    return xxRectSize();
  }
  if (busted()!=0) {
    m_r[0]=m_r[2]=rb[0];
    m_r[1]=m_r[3]=rb[1];
    return .0;
  }
  m_r[2]=rb[0]; m_r[3]=rb[1];
  if (m_r[2]<m_r[0])
    m_r[0]=m_r[2];
  if (m_r[3]<m_r[1])
    m_r[1]=m_r[3];
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);

}

xxRect xxRect::unite(const xxRect &r) const {

  if (busted()!=0) {
    if (r.busted()!=0)
      return xxRect();
    return r;
  }
  if (r.busted()!=0)
    return *this;
  return xxRect(MIN(m_r[0],r.m_r[0]),MIN(m_r[1],r.m_r[1]),MAX(m_r[2],r.m_r[2]),MAX(m_r[3],r.m_r[3]));
        
}

xxRect xxRect::intersect(const xxRect &r) const {

  if (busted()!=0 || r.busted()!=0)
    return xxRect();
  xxRect rect;
  if (m_r[2]<r.m_r[0] || r.m_r[2]<m_r[0] ||
      m_r[3]<r.m_r[1] || r.m_r[3]<m_r[1])
    return rect;
  if (r.m_r[0]<=m_r[2] && m_r[0]<=r.m_r[0]) {
    rect.setLeft(r.m_r[0]);
    rect.setRight(MIN(m_r[2],r.m_r[2]));
  }
  else if (m_r[0]<=r.m_r[2] && r.m_r[0]<=m_r[0]) {
    rect.setLeft(m_r[0]);
    rect.setRight(MIN(r.m_r[2],m_r[2]));
  }
  if (r.m_r[1]<=m_r[3] && m_r[1]<=r.m_r[1]) {
    rect.setTop(r.m_r[1]);
    rect.setBottom(MIN(m_r[3],r.m_r[3]));
  }
  else if (m_r[1]<=r.m_r[3] && r.m_r[1]<=m_r[1]) {
    rect.setTop(m_r[1]);
    rect.setBottom(MIN(r.m_r[3],m_r[3]));
  }
  return rect;  

}

xxRectSize xxRect::resize(double w,double h,unsigned char type) {
     
  if (busted()!=0)
    return xxRectSize();
  if (mk_isbusted(w)!=0 || mk_isbusted(h)!=0) {
    set();
    return xxRectSize();
  }
  double myw=m_r[2]-m_r[0],myh=m_r[3]-m_r[1];
  if (w<.0)
    w=myw;
  if (h<.0)
    h=myh;
  if ((type&15)>0) {
    if ((type&1)>0)
      m_r[2]=m_r[0]+w;
    else if ((type&4)>0)
      m_r[0]=m_r[2]-w;
    if ((type&2)>0)
      m_r[3]=m_r[1]+h;
    else if ((type&8)>0)
      m_r[1]=m_r[3]-h;
  }
  else {
    num::Vector3 c(m_r[0]+myw/2.,m_r[1]+myh/2.);
    m_r[0]=c[0]-w/2.; 
    m_r[2]=c[0]+w/2.;
    m_r[1]=c[1]-h/2.; 
    m_r[3]=c[1]+h/2.;
  }
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);
  
}

num::Vector3 xxRect::translate(double hor,double ver,unsigned char type) {

  if (busted()!=0)
    return num::Vector3();
  if (mk_isbusted(hor)!=0 || mk_isbusted(ver)!=0) {
    set();
    return num::Vector3();
  }
  double w=m_r[2]-m_r[0],h=m_r[3]-m_r[1];
  if ((type&15)>0) {
    if ((type&1)>0) {
      m_r[0]=hor;
      m_r[2]=m_r[0]+w;
    }
    else if ((type&4)>0) {
      m_r[2]=hor;
      m_r[0]=m_r[2]-w;
    }
    if ((type&2)>0) {
      m_r[1]=ver;
      m_r[3]=m_r[1]+h;
    }
    else if ((type&8)>0) {
      m_r[3]=ver;
      m_r[1]=m_r[3]-h;
    }
  }
  else {
    num::Vector3 c(hor+m_r[0]+w/2.,ver+m_r[1]+h/2.);
    m_r[0]=c[0]-w/2.;
    m_r[1]=c[1]-h/2.;
    m_r[2]=c[0]+w/2.;
    m_r[3]=c[1]+h/2.;
  }
  return num::Vector3(m_r[0],m_r[1]);
  
}

void xxRect::rotate(double rot,num::Vector3 *lt,num::Vector3 *rt,num::Vector3 *lb,num::Vector3 *rb) const {

  if (busted()!=0 || mk_isbusted(rot)!=0) {
    if (lt)
      *lt=num::Vector3();
    if (rt)
      *rt=num::Vector3();
    if (lb)
      *lb=num::Vector3();
    if (rb)
      *rb=num::Vector3();
    return;
  }
  rot=mk_dsign(rot)*fmod(fabs(rot),180.); 
  double wexc=(m_r[2]-m_r[0])/2.,hexc=(m_r[3]-m_r[1])/2.; 
  num::TransformMatrix m;
  m.rotateZ(rot);
  m.translate((m_r[2]+m_r[0])/2.,(m_r[3]+m_r[1])/2.);
  if (lt) {
    lt->setXY(-wexc,-hexc);
    m.transform(lt);
  }
  if (rt) {
    rt->setXY(wexc,-hexc);
    m.transform(rt);
  }
  if (lb) {
    lb->setXY(-wexc,hexc);
    m.transform(lb);
  }
  if (rb) {
    rb->setXY(wexc,hexc);
    m.transform(rb);
  }
  
}

double xxRect::scale(double sc,unsigned char type) {

  if (busted()!=0 || m_r[0]<=m_r[2] || m_r[1]<=m_r[3])
    return .0;
  if (mk_isbusted(sc)!=0) {
    set();
    return .0;
  }
  if (sc<.0) sc=.0;
  double w=sqrt(sc*(m_r[2]-m_r[0])*(m_r[2]-m_r[0])),h=sqrt(sc*(m_r[3]-m_r[1])*(m_r[3]-m_r[1]));
  if ((type&15)>0) {
    if ((type&1)>0) {
      m_r[2]=m_r[0]+w;
    }
    else if ((type&4)>0) {
      m_r[0]=m_r[2]-w;
    }
    if ((type&2)>0) {
      m_r[3]=m_r[1]+h;
    }
    else if ((type&8)>0) {
      m_r[1]=m_r[3]-h;
    }
  }
  else {
    num::Vector3 c((m_r[0]+m_r[2])/2.,(m_r[3]+m_r[1])/2.);
    m_r[0]=c[0]-w/2.; m_r[1]=c[1]-h/2.; m_r[2]=c[0]+w/2.; m_r[3]=c[1]+h/2.;
  }
  return ((m_r[2]-m_r[0])*(m_r[3]-m_r[1]));

}

int xxRect::toString(mk_string str) const {
    
  mk_stringappend(str,"left=");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_d2a(m_r[0],numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," , top=");
  mk_d2a(m_r[1],numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," , right=");
  mk_d2a(m_r[2],numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str," , bottom=");
  mk_d2a(m_r[3],numstr);
  mk_stringappend(str,numstr);
  return 0;

}

} // namespace
