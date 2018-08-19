
#include <auxx/auxx.h>
#include <numeric/matrix.h>
#include <osix/xxshape.h>
#include <stdio.h>
#include <string.h>
//#include <sys/param.h>

namespace osix {

void xxLine::setP0(aux::Vector3 p) {

  if (p.busted(aux::typeX|aux::typeY)!=0) {
    set();
    return;
  }
  if (busted()!=0)
    m_l[1]=p;
  m_l[0]=p;
  if (m_l[1].x()<m_l[0].x())
    aux::swap(&m_l[0],&m_l[1]);

}

void xxLine::setP1(aux::Vector3 p) {

  if (p.busted(aux::typeX|aux::typeY)!=0) {
    set();
    return;
  }
  if (busted()!=0)
    m_l[0]=p;
  m_l[1]=p;
  if (m_l[1].x()<m_l[0].x())
    aux::swap(&m_l[0],&m_l[1]);

}
 
double xxLine::len() const {

  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_l[0].x(),-m_l[0].y());
  aux::Vector3 p1(m_l[1]);
  m.transform(&p1);
  return p1.len();  

}

int xxLine::busted() const {
      
  return ((m_l[0].busted(aux::typeX|aux::typeY)>0 || 
          m_l[1].busted(aux::typeX|aux::typeY)>0) ? 1 : 0);
    
}

int xxLine::empty() const {
      
  return ((busted()>0 || m_l[0]==m_l[1]) ? 1 : 0);
  
}

double xxLine::angdeg() const {

  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_l[0].x(),-m_l[0].y());
  aux::Vector3 p0(0.,1.),p1(m_l[1]);
  m.transform(&p1);
  return p1.angdeg(p0);

}
 
double xxLine::angrad() const {

  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_l[0].x(),-m_l[0].y());
  aux::Vector3 p0(0.,1.),p1(m_l[1]);
  m.transform(&p1);
  return p1.angrad(p0);

}

void xxLine::toString(aux::Asciistr *str) const {

  if (!str)
    return;
  str->reserve(256);
  str->append("p0=");
  aux::Asciistr numstr;
  m_l[0].toString(&numstr);
  str->append((const char *)numstr);
  numstr=0;
  str->append(" ; p1=");
  m_l[1].toString(&numstr);
  str->append((const char *)numstr);

}

xxRect::xxRect(double left,double top,double right,double bottom) {
  
  if (mk_isBusted(left)!=0 || mk_isBusted(top)!=0 || 
      mk_isBusted(right)!=0 || mk_isBusted(bottom)!=0) {
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

xxRect::xxRect(aux::Vector3 lt,aux::Vector3 br) {

  if (lt.busted(aux::typeX|aux::typeY)!=0 || br.busted(aux::typeX|aux::typeY)!=0) {
    set();
    return;
  }
  m_r[0]=MIN(lt[0],br[0]);
  m_r[1]=MIN(lt[1],br[1]);
  m_r[2]=MAX(lt[0],br[0]);
  m_r[3]=MAX(lt[1],br[1]);
       
}

xxRect::xxRect(double left,double top,xxRectSize size) {

  if (mk_isBusted(left)!=0 || mk_isBusted(top)!=0) {
    set();
    return;
  }
  m_r[0]=left;
  m_r[1]=top;
  m_r[2]=left+size.width();
  m_r[3]=top+size.height();
  
} 
      
xxRect::xxRect(aux::Vector3 lt,xxRectSize size) {

  if (lt.busted(aux::typeX|aux::typeY)!=0) {
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
      
  return ((mk_isBusted(m_r[0])!=0 || mk_isBusted(m_r[1])!=0 || 
          mk_isBusted(m_r[2])!=0 || mk_isBusted(m_r[3])!=0) ? 1 : 0);
   
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
      
  if (mk_isBusted(left)!=0 || mk_isBusted(top)!=0) {
    set();
    return;
  }
  m_r[0]=left;
  m_r[1]=top;
  m_r[2]=m_r[0]+sz.width();
  m_r[3]=m_r[1]+sz.height();
  
}

void xxRect::set(aux::Vector3 lt,aux::Vector3 br) {

  if (lt.busted(aux::typeX|aux::typeY)!=0 || br.busted(aux::typeX|aux::typeY)!=0) {
    set();
    return;
  }
  m_r[0]=MIN(lt[0],br[0]);
  m_r[1]=MIN(lt[1],br[1]);
  m_r[2]=MAX(lt[0],br[0]);
  m_r[3]=MAX(lt[1],br[1]);

}

double xxRect::setLeft(double left) {

  if (mk_isBusted(left)!=0) {
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

  if (mk_isBusted(top)!=0) {
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

  if (mk_isBusted(right)!=0) {
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

  if (mk_isBusted(bottom)!=0) {
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

xxRectSize xxRect::setLeftTop(aux::Vector3 lt) {

  if (lt.busted(aux::typeX|aux::typeY)!=0) {
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

xxRectSize xxRect::setLeftBottom(aux::Vector3 lb) {

  if (lb.busted(aux::typeX|aux::typeY)!=0) {
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

xxRectSize xxRect::setRightTop(aux::Vector3 rt) {

  if (rt.busted(aux::typeX|aux::typeY)!=0) {
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

xxRectSize xxRect::setRightBottom(aux::Vector3 rb) {

  if (rb.busted(aux::typeX|aux::typeY)!=0) {
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
  if (mk_isBusted(w)!=0 || mk_isBusted(h)!=0) {
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
    aux::Vector3 c(m_r[0]+myw/2.,m_r[1]+myh/2.);
    m_r[0]=c[0]-w/2.; 
    m_r[2]=c[0]+w/2.;
    m_r[1]=c[1]-h/2.; 
    m_r[3]=c[1]+h/2.;
  }
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);
  
}

aux::Vector3 xxRect::translate(double hor,double ver,unsigned char type) {

  if (busted()!=0)
    return aux::Vector3();
  if (mk_isBusted(hor)!=0 || mk_isBusted(ver)!=0) {
    set();
    return aux::Vector3();
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
    aux::Vector3 c(hor+m_r[0]+w/2.,ver+m_r[1]+h/2.);
    m_r[0]=c[0]-w/2.;
    m_r[1]=c[1]-h/2.;
    m_r[2]=c[0]+w/2.;
    m_r[3]=c[1]+h/2.;
  }
  return aux::Vector3(m_r[0],m_r[1]);
  
}

void xxRect::rotate(double rot,aux::Vector3 *lt,aux::Vector3 *rt,aux::Vector3 *lb,aux::Vector3 *rb) const {

  if (busted()!=0 || mk_isBusted(rot)!=0) {
    if (lt)
      *lt=aux::Vector3();
    if (rt)
      *rt=aux::Vector3();
    if (lb)
      *lb=aux::Vector3();
    if (rb)
      *rb=aux::Vector3();
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
  if (mk_isBusted(sc)!=0) {
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
    aux::Vector3 c((m_r[0]+m_r[2])/2.,(m_r[3]+m_r[1])/2.);
    m_r[0]=c[0]-w/2.; m_r[1]=c[1]-h/2.; m_r[2]=c[0]+w/2.; m_r[3]=c[1]+h/2.;
  }
  return ((m_r[2]-m_r[0])*(m_r[3]-m_r[1]));

}

void xxRect::toString(aux::Asciistr *str) const {
      
  if (!str)
    return;
  str->reserve(256);
  str->append("left=");
  aux::Asciistr numstr;
  aux::d2a(m_r[0],&numstr);
  str->append((const char *)numstr);
  numstr=0;
  str->append(" , top=");
  aux::d2a(m_r[1],&numstr);
  str->append((const char *)numstr);
  numstr=0;
  str->append(" , right=");
  aux::d2a(m_r[2],&numstr);
  str->append((const char *)numstr);
  numstr=0;
  str->append(" , bottom=");
  aux::d2a(m_r[3],&numstr);
  str->append((const char *)numstr);

}

} // namespace
