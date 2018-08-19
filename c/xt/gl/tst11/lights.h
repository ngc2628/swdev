
#ifndef LIGHTS_H
#define LIGHTS_H

#include <GL/gl.h>
#include <xt/gl/tst11/globals.h>
#include <mkbase/mkbase.h>

const int light_n=8;
const int lightspec_n=10;
const int glight[light_n]={
GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,GL_LIGHT5,GL_LIGHT6,GL_LIGHT7
};
const unsigned int glightspec[lightspec_n]={
GL_AMBIENT,GL_DIFFUSE,GL_SPECULAR,GL_POSITION,GL_SPOT_DIRECTION,GL_SPOT_EXPONENT,
GL_SPOT_CUTOFF,GL_CONSTANT_ATTENUATION,GL_LINEAR_ATTENUATION,GL_QUADRATIC_ATTENUATION
};
const unsigned int gmaterialspec[lightspec_n]={
GL_AMBIENT,GL_DIFFUSE,GL_AMBIENT_AND_DIFFUSE,GL_SPECULAR,GL_SHININESS,
GL_EMISSION,GL_COLOR_INDEXES,0,0,0
};

extern char strLightSpecName[lightspec_n][buflen];
extern char strMaterialSpecName[lightspec_n][buflen];

struct LightSpec {
  struct mk_vertex m_value;
  char m_snature[buflen];
};

struct LightSource {
  unsigned int m_on;
  int m_sidx,m_midx;
  struct LightSpec m_specL[lightspec_n];
  struct LightSpec m_materialL[lightspec_n];
};

extern LightSource lightL[light_n];
extern LightSource wrklightL[light_n];

extern LightSpec *find_nature(const char *,LightSource *,unsigned int *);
extern LightSpec *find_material(const char *,LightSource *,unsigned int *);
extern void light_conf();

#endif
