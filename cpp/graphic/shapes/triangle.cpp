
#include <mkbase/mkla.h>
#include <mkbase/mkmath.h>
#include <numeric/matrix.h>
#include <graphic/shapes/triangle.h>
#include <graphic/shapes/line.h>
#include <stdio.h>
#include <string.h>

using namespace num;

namespace shapes {

static const int defCntPoints=3;

Triangle::Triangle() : Shape2("triangle") {

  mk_listrealloc(&m_vertices,3);
  mk_vertexzero(pp);
  mk_listappend(&m_vertices,pp);
  mk_listappend(&m_vertices,pp);
  mk_listappend(&m_vertices,pp);

}
    
Triangle::Triangle(const Triangle &ass) : Shape2((const Shape2 &)ass) {

}
 
Triangle &Triangle::operator=(const Triangle &ass) {
      
  if (&ass!=this) {
    m_scale=ass.m_scale;
    m_rotate=ass.m_rotate;
    mk_vertexcopy(m_translate,ass.m_translate);
    mk_stringset(m_descr,&ass.m_descr[0]);
    m_styleO=ass.m_styleO;
    m_styleF=ass.m_styleF;
    mk_listcopy(&m_vertices,&ass.m_vertices);
  }
  return *this;
  
}
   
bool Triangle::operator==(const Triangle &cmp) const {
      
  return (circradius()==cmp.circradius());
  
}
    
bool Triangle::operator<(const Triangle &cmp) const {

  return (circradius()<cmp.circradius());
  
}

int Triangle::setVertex(int idx,mk_vertex vertex) {

  int res=Shape2::setVertex(idx,vertex);
  if (res!=0)
    return res;
  mk_vertexzero(t1);
  mk_vertexzero(t2);
  mk_listat(&m_vertices,0,(void*)&t1);
  mk_listat(&m_vertices,1,(void*)&t2);
  if (t2[0]<t1[0]) {
    mk_listsetat(&m_vertices,(void*)&t1,1,0);
    mk_listsetat(&m_vertices,(void*)&t2,0,0);
  }
  return res;

}

double Triangle::circradius() const { 

  Line2 aa,bb,cc;
  mk_vertexzero(t1);
  mk_vertexzero(t2);
  mk_vertexzero(t3);
  mk_listat(&m_vertices,0,(void*)&t1);
  mk_listat(&m_vertices,1,(void*)&t2);
  mk_listat(&m_vertices,2,(void*)&t3);
  aa.setVertex(0,t1);
  aa.setVertex(1,t2);
  bb.setVertex(0,t2);
  bb.setVertex(1,t3);
  cc.setVertex(0,t3);
  cc.setVertex(1,t1); 
  double la=aa.len(),lb=bb.len(),lc=cc.len(),
    ss=(la+lb+lc)/2.,A4=4*sqrt(ss*(ss-la)*(ss-lb)*(ss-lc));
  return (la*lb*lc/A4);
  
}

int Triangle::center(mk_vertex vc) const {

  TransformMatrix mm;
  mk_vertexzero(mt1);
  mk_vertexzero(mt2);
  mk_vertexzero(mt3);
  mk_listat(&m_vertices,0,(void*)&mt1);
  mk_listat(&m_vertices,1,(void*)&mt2);
  mk_listat(&m_vertices,2,(void*)&mt3);
  mk_vertexzero(m21); 
  mk_vertexsubs(mt2,mt1,m21);
  mk_vertexzero(m32); 
  mk_vertexsubs(mt3,mt2,m32);
  mk_vertex nn={.0,.0,1.,1.}; 
  mk_vertexzero(n21); 
  mk_vertexcross(m21,nn,n21);
  mk_vertexzero(n32); 
  mk_vertexcross(m32,nn,n32);
  double xm21=(mt2[0]+mt1[0])/2.,ym21=(mt2[1]+mt1[1])/2.,
         xm32=(mt3[0]+mt2[0])/2.,ym32=(mt3[1]+mt2[1])/2.;
  mk_vertex vm21={xm21,ym21,.0,1.};
  mk_vertex vm32={xm32,ym32,.0,1.};
  mm.translate(xm21,ym21);
  mm.transform(n21);
  mm.reset();
  mm.translate(xm32,ym32);
  mm.transform(n32);
  mm.reset();
  mk_vertexzero(vvc);
  mk_linesintersection(vm21,n21,vm32,n32,vvc,3,0);
  mk_vertexcopy(vc,vvc);
  return 0;

}

int Triangle::eval(struct mk_list *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  Shape2::eval(pointL,npoints);
  mk_vertexzero(vc);
  center(vc);
  TransformMatrix mm;
  mk_vertexzero(mt1);
  mk_vertexzero(mt2);
  mk_vertexzero(mt3);
  mk_listat(&m_vertices,0,(void*)&mt1);
  mk_listat(&m_vertices,1,(void*)&mt2);
  mk_listat(&m_vertices,2,(void*)&mt3);
  mm.translate(-vc[0],-vc[1]);
  mm.scale(m_scale,m_scale,1.);
  mm.rotateZ(m_rotate);
  mm.translate(vc[0]+m_translate[0],vc[1]+m_translate[1]);
  mm.transform(mt1);
  mk_listappend(pointL,(void*)&mt1);
  mm.transform(mt2);
  mk_listappend(pointL,(void*)&mt2);
  mm.transform(mt3);
  mk_listappend(pointL,(void*)&mt3);
  return npoints;

}

int Triangle::toString(mk_string str) const {

  Shape2::toString(str);
  mk_vertexzero(mt1);
  mk_vertexzero(mt2);
  mk_vertexzero(mt3);
  mk_listat(&m_vertices,0,(void*)&mt1);
  mk_listat(&m_vertices,1,(void*)&mt2);
  mk_listat(&m_vertices,2,(void*)&mt3);
  mk_stringappend(str,"p0, ");
  mk_string numstr;
  mk_stringset(numstr,0);
  mk_vertexdbg(mt1,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\np1, ");
  mk_stringset(numstr,0);
  mk_vertexdbg(mt2,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\np2, ");
  mk_stringset(numstr,0);
  mk_vertexdbg(mt3,numstr);
  mk_stringappend(str,numstr);
  mk_stringappend(str,"\n");
  return 0;

}

TriangleEq::TriangleEq(double aa) : Shape2("triangleeq") {

  mk_listrealloc(&m_vertices,1);
  if (mk_isbusted(aa)!=0 || aa<.0)
    aa=.0;
  mk_vertex vv={aa,.0,.0,1.};
  mk_listappend(&m_vertices,vv);

}
    
TriangleEq::TriangleEq(const TriangleEq &ass) : Shape2((const Shape2 &)ass) {

}

TriangleEq &TriangleEq::operator=(const TriangleEq &ass) {
      
  if (&ass!=this) {
    m_scale=ass.m_scale;
    m_rotate=ass.m_rotate;
    mk_vertexcopy(m_translate,ass.m_translate);
    mk_stringset(m_descr,&ass.m_descr[0]);
    m_styleO=ass.m_styleO;
    m_styleF=ass.m_styleF;
    mk_listcopy(&m_vertices,&ass.m_vertices);
  }
  return *this;
  
}

bool TriangleEq::operator==(const TriangleEq &cmp) const {
      
  mk_vertexzero(aa1);
  mk_listat(&m_vertices,0,(void*)&aa1);
  mk_vertexzero(aa2);
  mk_listat(&cmp.m_vertices,0,(void*)&aa2);
  return (aa1[0]==aa2[0]);
  
}
    
bool TriangleEq::operator<(const TriangleEq &cmp) const {
      
  mk_vertexzero(aa1);
  mk_listat(&m_vertices,0,(void*)&aa1);
  mk_vertexzero(aa2);
  mk_listat(&cmp.m_vertices,0,(void*)&aa2);
  return (aa1[0]<aa2[0]);
  
}
    

int TriangleEq::eval(struct mk_list *pointL,int npoints) {

  if (npoints!=defCntPoints)
    npoints=defCntPoints;
  Shape2::eval(pointL,npoints);
  mk_vertexzero(aa);
  mk_listat(&m_vertices,0,(void*)&aa);
  double xx=aa[0]/2.,yy=aa[0]*sqrt(3.)/6.;
  mk_vertexzero(mt1);
  mk_vertexzero(mt2);
  mk_vertexzero(mt3);
  mt1[0]=-xx;
  mt1[1]=-yy;
  mt2[0]=xx;
  mt2[1]=-yy;
  mt3[0]=.0;
  mt3[1]=2.*yy;
  TransformMatrix mm;
  mm.rotateZ(m_rotate);
  mm.translate(m_translate[0],m_translate[1]);
  mm.transform(mt1);
  mk_listappend(pointL,(void*)&mt1);
  mm.transform(mt2);
  mk_listappend(pointL,(void*)&mt2);
  mm.transform(mt3);
  mk_listappend(pointL,(void*)&mt3);
  return npoints;

}

} // namespace
