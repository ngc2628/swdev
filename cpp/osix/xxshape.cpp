
#include <numeric/matrix.h>
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

  mk_stringappend(str,"w [");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_d2a(m_width,numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"] , h [");
  mk_stringset(numstr,0);
  mk_d2a(m_height,numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str,"]");
  return 0;
    
}

xxLine::xxLine() {

  mk_vertexset(m_p0,mk_dnan);
  mk_vertexset(m_p1,mk_dnan);

}
    
xxLine::xxLine(const xxLine &ass) {
   
  mk_vertexcopy(m_p0,ass.m_p0);
  mk_vertexcopy(m_p1,ass.m_p1);
    
}
    
xxLine &xxLine::operator=(const xxLine &ass) {
 
  mk_vertexcopy(m_p0,ass.m_p0);
  mk_vertexcopy(m_p1,ass.m_p1);
  return *this;
  
}

bool xxLine::operator==(const xxLine &cmp) const {
      
  return (mk_deq(len(),cmp.len())==0 ? true : false);
  
}

bool xxLine::operator<(const xxLine &cmp) const {

  return (mk_dlt(len(),cmp.len())==0 ? false : true);
  
}

int xxLine::p0(mk_vertex p0) const {
  
  mk_vertexcopy(p0,m_p0);    
  return 0;

}

int xxLine::p1(mk_vertex p1) const {
  
  mk_vertexcopy(p1,m_p1);    
  return 0;

}

void xxLine::set(mk_vertex p0,mk_vertex p1) {
    
  mk_vertexcopy(m_p0,p0);
  mk_vertexcopy(m_p1,p1);
  if (m_p1[0]<m_p0[0]) 
    mk_vertexswap(m_p0,m_p1);
  
}

void xxLine::setP0(mk_vertex p0) {

  mk_vertexcopy(m_p0,p0);
  if (m_p1[0]<m_p0[0]) 
    mk_vertexswap(m_p0,m_p1);

}

void xxLine::setP1(mk_vertex p1) {

  mk_vertexcopy(m_p1,p1);
  if (m_p1[0]<m_p0[0]) 
    mk_vertexswap(m_p0,m_p1);

}
 
double xxLine::len() const {

  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_p0[0],-m_p0[1]);
  mk_vertexnan(p1);
  mk_vertexcopy(p1,m_p1);
  m.transform(p1);
  return mk_vertexlen(p1);  

}

int xxLine::busted() const {
     
  int res=mk_isbusted(m_p0[0])|mk_isbusted(m_p0[1])|
          mk_isbusted(m_p1[0])|mk_isbusted(m_p1[1]);
  return (res==0 ? 0 : 1);
    
}

int xxLine::empty() const {
      
  return ((busted()>0 || (m_p0[0]==m_p1[0] && m_p0[1]==m_p1[1])) ? 1 : 0);
  
}

double xxLine::angdeg() const {

  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_p0[0],-m_p0[1]);
  mk_vertex p0={0.,1.,mk_dnan,1.};
  mk_vertex p1={m_p1[0],m_p1[1],mk_dnan,1.};
  m.transform(p1);
  return mk_vertexangdeg(p1,p0);
  
}
 
double xxLine::angrad() const {

  
  if (empty()!=0)
    return .0;
  num::TransformMatrix m;
  m.translate(-m_p0[0],-m_p0[1]);
  mk_vertex p0={0.,1.,mk_dnan,1.};
  mk_vertex p1={m_p1[0],m_p1[1],mk_dnan,1.};
  m.transform(p1);
  return mk_vertexangrad(p1,p0);

}

int xxLine::toString(mk_string str) const {

  mk_stringappend(str,"p0=");
  mk_string vstr;
  mk_stringset(vstr,0);
  mk_vertexdbg(m_p0,vstr);
  mk_stringappend(str,&vstr[0]);
  mk_stringappend(str," ; p1=");
  mk_stringset(vstr,0);
  mk_vertexdbg(m_p1,vstr);
  mk_stringappend(str,&vstr[0]);
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

xxRect::xxRect(mk_vertex lt,mk_vertex br) {

  m_r[0]=MIN(lt[0],br[0]);
  m_r[1]=MIN(lt[1],br[1]);
  m_r[2]=MAX(lt[0],br[0]);
  m_r[3]=MAX(lt[1],br[1]);
       
}

xxRect::xxRect(double left,double top,xxRectSize size) {

  m_r[0]=left;
  m_r[1]=top;
  m_r[2]=left+size.width();
  m_r[3]=top+size.height();
  
} 
      
xxRect::xxRect(mk_vertex lt,xxRectSize size) {

  m_r[0]=lt[0];
  m_r[1]=lt[1];
  m_r[2]=m_r[0]+size.width();
  m_r[3]=m_r[1]+size.height();

}

xxRect xxRect::operator+(double add) {

  return xxRect(m_r[0]-add,m_r[1]-add,m_r[2]+add,m_r[3]+add);

}

xxRect xxRect::operator-(double add) {

  return xxRect(m_r[0]+add,m_r[1]+add,m_r[2]-add,m_r[3]-add);

}
    
bool xxRect::operator==(const xxRect &cmp) const {

  int ii=0;
  for (ii=0;ii<4;ii++) {
    if (m_r[ii]!=cmp.m_r[ii])
      return false;
  }
  return true;

}

bool xxRect::operator<(const xxRect &cmp) const {
      
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
      
  m_r[0]=left;
  m_r[1]=top;
  m_r[2]=m_r[0]+sz.width();
  m_r[3]=m_r[1]+sz.height();
  
}

void xxRect::set(mk_vertex lt,mk_vertex br) {

  m_r[0]=MIN(lt[0],br[0]);
  m_r[1]=MIN(lt[1],br[1]);
  m_r[2]=MAX(lt[0],br[0]);
  m_r[3]=MAX(lt[1],br[1]);

}

double xxRect::setLeft(double left) {

  m_r[0]=left; 
  if (m_r[2]<m_r[0])
    m_r[2]=m_r[0];
  return (m_r[2]-m_r[0]); 
  
}

double xxRect::setTop(double top) {

  m_r[1]=top; 
  if (m_r[3]<m_r[1])
    m_r[3]=m_r[1];
  return (m_r[3]-m_r[1]);
  
}

double xxRect::setRight(double right) {

  m_r[2]=right; 
  if (m_r[2]<m_r[0])
    m_r[0]=m_r[2];
  return (m_r[2]-m_r[0]);

}

double xxRect::setBottom(double bottom) {

  m_r[3]=bottom; 
  if (m_r[3]<m_r[1])
    m_r[1]=m_r[3];
  return (m_r[3]-m_r[1]); 

}

xxRectSize xxRect::setLeftTop(mk_vertex lt) {

  m_r[0]=lt[0]; 
  m_r[1]=lt[1];
  if (m_r[2]<m_r[0])
    m_r[2]=m_r[0];
  if (m_r[3]<m_r[1])
    m_r[3]=m_r[1];
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);
  
}

xxRectSize xxRect::setLeftBottom(mk_vertex lb) {

  m_r[0]=lb[0]; 
  m_r[3]=lb[1];
  if (m_r[2]<m_r[0])
    m_r[2]=m_r[0];
  if (m_r[3]<m_r[1])
    m_r[1]=m_r[3];
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);

}

xxRectSize xxRect::setRightTop(mk_vertex rt) {

  m_r[2]=rt[0]; 
  m_r[1]=rt[1];
  if (m_r[2]<m_r[0])
    m_r[0]=m_r[2];
  if (m_r[3]<m_r[1])
    m_r[3]=m_r[1];
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);

}

xxRectSize xxRect::setRightBottom(mk_vertex rb) {

  m_r[2]=rb[0]; 
  m_r[3]=rb[1];
  if (m_r[2]<m_r[0])
    m_r[0]=m_r[2];
  if (m_r[3]<m_r[1])
    m_r[1]=m_r[3];
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);

}

xxRect xxRect::unite(const xxRect &rr) const {

  if (rr.busted()!=0)
    return *this;
  if (busted()!=0)
    return rr;
  return xxRect(MIN(m_r[0],rr.m_r[0]),MIN(m_r[1],rr.m_r[1]),
    MAX(m_r[2],rr.m_r[2]),MAX(m_r[3],rr.m_r[3]));
        
}

xxRect xxRect::intersect(const xxRect &rr) const {

  xxRect rect;
  if (m_r[2]<rr.m_r[0] || rr.m_r[2]<m_r[0] ||
      m_r[3]<rr.m_r[1] || rr.m_r[3]<m_r[1])
    return rect;
  if (rr.m_r[0]<=m_r[2] && m_r[0]<=rr.m_r[0]) {
    rect.setLeft(rr.m_r[0]);
    rect.setRight(MIN(m_r[2],rr.m_r[2]));
  }
  else if (m_r[0]<=rr.m_r[2] && rr.m_r[0]<=m_r[0]) {
    rect.setLeft(m_r[0]);
    rect.setRight(MIN(rr.m_r[2],m_r[2]));
  }
  if (rr.m_r[1]<=m_r[3] && m_r[1]<=rr.m_r[1]) {
    rect.setTop(rr.m_r[1]);
    rect.setBottom(MIN(m_r[3],rr.m_r[3]));
  }
  else if (m_r[1]<=rr.m_r[3] && rr.m_r[1]<=m_r[1]) {
    rect.setTop(m_r[1]);
    rect.setBottom(MIN(rr.m_r[3],m_r[3]));
  }
  return rect;  

}

xxRectSize xxRect::resize(double ww,double hh,unsigned char type) {
     
  double myw=m_r[2]-m_r[0],myh=m_r[3]-m_r[1];
  if (ww<.0)
    ww=myw;
  if (hh<.0)
    hh=myh;
  if ((type&15)>0) {
    if ((type&1)>0)
      m_r[2]=m_r[0]+ww;
    else if ((type&4)>0)
      m_r[0]=m_r[2]-ww;
    if ((type&2)>0)
      m_r[3]=m_r[1]+hh;
    else if ((type&8)>0)
      m_r[1]=m_r[3]-hh;
  }
  else {
    mk_vertex cc={m_r[0]+myw/2.,m_r[1]+myh/2.,mk_dnan,1.};
    m_r[0]=cc[0]-ww/2.; 
    m_r[2]=cc[0]+ww/2.;
    m_r[1]=cc[1]-hh/2.; 
    m_r[3]=cc[1]+hh/2.;
  }
  return xxRectSize(m_r[2]-m_r[0],m_r[3]-m_r[1]);
  
}

int xxRect::translate(double hor,double ver,unsigned char type) {

  double ww=m_r[2]-m_r[0],hh=m_r[3]-m_r[1];
  if ((type&15)>0) {
    if ((type&1)>0) {
      m_r[0]=hor;
      m_r[2]=m_r[0]+ww;
    }
    else if ((type&4)>0) {
      m_r[2]=hor;
      m_r[0]=m_r[2]-ww;
    }
    if ((type&2)>0) {
      m_r[1]=ver;
      m_r[3]=m_r[1]+hh;
    }
    else if ((type&8)>0) {
      m_r[3]=ver;
      m_r[1]=m_r[3]-hh;
    }
  }
  else {
    mk_vertex cc={hor+m_r[0]+ww/2.,ver+m_r[1]+hh/2.,mk_dnan,1.};
    m_r[0]=cc[0]-ww/2.;
    m_r[1]=cc[1]-hh/2.;
    m_r[2]=cc[0]+ww/2.;
    m_r[3]=cc[1]+hh/2.;
  }
  return 0;
  
}

void xxRect::rotate(double rot,mk_vertex lt,mk_vertex rt,mk_vertex lb,mk_vertex rb) const {

  rot=mk_dsign(rot)*fmod(fabs(rot),180.); 
  double wexc=(m_r[2]-m_r[0])/2.,hexc=(m_r[3]-m_r[1])/2.; 
  num::TransformMatrix m;
  m.rotateZ(rot);
  m.translate((m_r[2]+m_r[0])/2.,(m_r[3]+m_r[1])/2.);
  lt[0]=-wexc;
  lt[1]=-hexc;
  m.transform(lt);
  rt[0]=wexc;
  rt[1]=-hexc;
  m.transform(rt);
  lb[0]=-wexc;
  lb[1]=hexc;  
  m.transform(lb);
  rb[0]=wexc;
  rb[1]=hexc;
  m.transform(rb);
    
}

double xxRect::scale(double sc,unsigned char type) {

  if (sc<.0) 
    sc=.0;
  double ww=sqrt(sc*(m_r[2]-m_r[0])*(m_r[2]-m_r[0])),hh=sqrt(sc*(m_r[3]-m_r[1])*(m_r[3]-m_r[1]));
  if ((type&15)>0) {
    if ((type&1)>0) {
      m_r[2]=m_r[0]+ww;
    }
    else if ((type&4)>0) {
      m_r[0]=m_r[2]-ww;
    }
    if ((type&2)>0) {
      m_r[3]=m_r[1]+hh;
    }
    else if ((type&8)>0) {
      m_r[1]=m_r[3]-hh;
    }
  }
  else {
    mk_vertex cc={(m_r[0]+m_r[2])/2.,(m_r[3]+m_r[1])/2.,mk_dnan,1.};
    m_r[0]=cc[0]-ww/2.; 
    m_r[1]=cc[1]-hh/2.; 
    m_r[2]=cc[0]+ww/2.; 
    m_r[3]=cc[1]+hh/2.;
  }
  return ((m_r[2]-m_r[0])*(m_r[3]-m_r[1]));

}

int xxRect::toString(mk_string str) const {
    
  mk_stringappend(str,"left=");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_d2a(m_r[0],numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str," , top=");
  mk_stringset(numstr,0);
  mk_d2a(m_r[1],numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str," , right=");
  mk_stringset(numstr,0);
  mk_d2a(m_r[2],numstr);
  mk_stringappend(str,&numstr[0]);
  mk_stringappend(str," , bottom=");
  mk_stringset(numstr,0);
  mk_d2a(m_r[3],numstr);
  mk_stringappend(str,&numstr[0]);
  return 0;

}

} // namespace
