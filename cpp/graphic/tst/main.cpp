
#include <stdio.h>
#include <string.h>
#include <graphic/shapes/rect.h>

int main(int argc,char **argv) {

  aux::Numstr numstr;
  
  shapes::Rect r(shapes::RectSize(4.,2.));
  r.rotate(45.);
  r.translate(aux::Vector3(4.,3.));
  r.toString(&numstr);
  printf ("r=%s\n",(const char*)numstr);
  numstr=0;
  aux::Vector3 c=r.center();
  c.toString(&numstr);
  printf ("c=%s\n",(const char*)numstr);
  numstr=0;
  
  shapes::SRect rs(5.,10.,9.,12.);
  //rs.setCenter(aux::Vector3(.0,.0));
  aux::Vector3 lt,rt,lb,rb;
  rs.rotate(45.,&lt,&rt,&lb,&rb);
  lt.toString(&numstr);
  printf ("lt=%s\n",(const char*)numstr);
  numstr=0;
  rt.toString(&numstr);
  printf ("rt=%s\n",(const char*)numstr);
  numstr=0;
  lb.toString(&numstr);
  printf ("lb=%s\n",(const char*)numstr);
  numstr=0;
  rb.toString(&numstr);
  printf ("rb=%s\n",(const char*)numstr);
  
  getchar();
  return 0;

}
