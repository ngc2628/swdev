
#ifndef LIGHTS_H
#define LIGHTS_H

#include <GL/gl.h>
#include <xt/gl/tst11/statics.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>

#define light_n 8
#define lightspec_n 10

extern int glight[light_n];
extern unsigned int glightspec[lightspec_n];
extern unsigned int gmaterialspec[lightspec_n];

extern char strLightSpecName[lightspec_n][buflen];
extern char strMaterialSpecName[lightspec_n][buflen];

struct LightSpec {
  mk_vertex m_value;
  char m_snature[buflen];
};

struct LightSource {
  unsigned int m_on;
  int m_sidx,m_midx;
  struct LightSpec m_specL[lightspec_n];
  struct LightSpec m_materialL[lightspec_n];
};

extern struct LightSource lightL[light_n];
extern struct LightSource wrklightL[light_n];

extern struct LightSpec *find_nature(const char *,struct LightSource *,unsigned int *);
extern struct LightSpec *find_material(const char *,struct LightSource *,unsigned int *);
extern void light_conf();

#endif
