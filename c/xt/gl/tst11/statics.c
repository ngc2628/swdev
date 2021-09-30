
#include <GL/gl.h>
#include <xt/gl/tst11/statics.h>
#include <xt/gl/tst11/objects.h>
#include <xt/gl/tst11/lights.h>
#include <xt/gl/tst11/glw.h>

unsigned int draw_fill=(1<<designator_off);
unsigned int draw_wireframe=(2<<designator_off);
unsigned int draw_normals=(4<<designator_off);
unsigned int light_on=(8<<designator_off);

int initialgeometry[4]={400,200,1000,800};
unsigned int bgmain=(240<<16)|(250<<8)|240;
unsigned int bgmenu=(240<<16)|(240<<8)|240;
unsigned int bgchk=(200<<16)|(200<<8)|50;
double simplepow10[10]={
  1.,10.,100.,1000.,10000.,100000.,1000000.,10000000.,100000000.,1000000000.
};

int initialize_statics() {

  drawstyle=(draw_fill|draw_wireframe);

  return 0;
}

