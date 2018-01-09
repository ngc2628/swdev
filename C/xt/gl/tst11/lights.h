
#ifndef LIGHTS_H
#define LIGHTS_H

#include <GL/gl.h>
#include <xt/gl/tst11/globals.h>
#include <auxx/vertex.h>

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

class LightSpec {
  public:
    aux::Vertex m_value;
    char m_snature[buflen];
    LightSpec() : m_value(aux::Vertex(.0,.0,.0,1.)) {
      memset(&m_snature[0],0,buflen);
    }
    LightSpec(const LightSpec &ass) {
      if (&ass!=this) {
        m_value=ass.m_value;
        memcpy(&m_snature[0],&ass.m_snature[0],buflen);
      }
    }
    LightSpec &operator=(const LightSpec &ass) {
      if (&ass!=this) {
        m_value=ass.m_value;
        memcpy(&m_snature[0],&ass.m_snature[0],buflen);
      }
      return *this;
    }
};

class LightSource {
  public:
    unsigned int m_on;
    int m_sidx,m_midx;
    LightSpec m_specL[lightspec_n];
    LightSpec m_materialL[lightspec_n];
    LightSource() : m_on(0),m_sidx(-1),m_midx(-1) {
      int ii=0;
      for (ii=0;ii<lightspec_n;ii++) {
        m_specL[ii]=LightSpec();
        m_materialL[ii]=LightSpec();
      }
    }
    LightSource(const LightSource &ass) :
      m_on(ass.m_on),m_sidx(ass.m_sidx),m_midx(ass.m_midx) {
      if (&ass!=this) {
        int ii=0;
        for (ii=0;ii<lightspec_n;ii++) {
          m_specL[ii]=ass.m_specL[ii];
          m_materialL[ii]=ass.m_materialL[ii];
        }
      }
    }
    LightSource &operator=(const LightSource &ass) {
      m_on=ass.m_on;
      m_sidx=ass.m_sidx;
      m_midx=ass.m_midx;
      if (&ass!=this) {
        int ii=0;
        for (ii=0;ii<lightspec_n;ii++) {
          m_specL[ii]=ass.m_specL[ii];
          m_materialL[ii]=ass.m_materialL[ii];
        }
      }
      return *this;
    }
};

extern LightSource lightL[light_n];
extern LightSource wrklightL[light_n];

extern LightSpec *find_nature(const char *,LightSource *,unsigned int *);
extern LightSpec *find_material(const char *,LightSource *,unsigned int *);
extern void light_conf();

#endif
