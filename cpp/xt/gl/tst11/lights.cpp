
#include <xt/gl/tst11/lights.h>
#include <xt/gl/tst11/glw.h>

LightSource lightL[light_n]={
LightSource(),LightSource(),LightSource(),LightSource(),
LightSource(),LightSource(),LightSource(),LightSource()
};

LightSource wrklightL[light_n]={
LightSource(),LightSource(),LightSource(),LightSource(),
LightSource(),LightSource(),LightSource(),LightSource()
};

char strLightSpecName[lightspec_n][buflen]={
"ambient","diffuse","specular","position","spotdirection","spotexponent",
"spotcutoff","constantattenuation","linearattenuation","quadraticattenuation"
};

char strMaterialSpecName[lightspec_n][buflen]={
"ambient","diffuse","ambientanddiffuse","specular","shininess","emission",
"none","none","none","none"
};

LightSpec *find_nature(const char *name,LightSource *lightsource,unsigned int *gnature) {

  if (gnature)
    *gnature=0;
  if (!name || strlen(name)==0)
    return 0;
  int ii=0;

  if (gnature) {
    for (ii=0;ii<lightspec_n;ii++) {
      if (strcmp(name,strLightSpecName[ii])==0) {
        if (gnature)
          *gnature=glightspec[ii];
        break;
      }
    }
  }

  if (lightsource) {
    for (ii=0;ii<lightspec_n;ii++) {
      if (strcmp(name,lightsource->m_specL[ii].m_snature)==0)
        return &(lightsource->m_specL[ii]);
    }
  }

  return 0;

}

LightSpec *find_material(const char *name,LightSource *lightsource,unsigned int *gmaterial) {

  if (gmaterial)
    *gmaterial=0;
  if (!name || strlen(name)==0)
    return 0;
  int ii=0;

  if (gmaterial) {
    for (ii=0;ii<lightspec_n;ii++) {
      if (strcmp(name,strMaterialSpecName[ii])==0) {
        if (gmaterial)
          *gmaterial=gmaterialspec[ii];
        break;
      }
    }
  }

  if (lightsource) {
    for (ii=0;ii<lightspec_n;ii++) {
      if (strcmp(name,lightsource->m_materialL[ii].m_snature)==0)
        return &(lightsource->m_materialL[ii]);
    }
  }

  return 0;

}

void light_conf() {

  unsigned char ison[1]={0};
  glGetBooleanv(GL_LIGHTING,&ison[0]);
  if (!ison[0])
    return;

  GLfloat glf[]={.0,.0,.0,.0};
  LightSource *lightsource=0;
  LightSpec *lightspec=0;
  int ii=0,jj=0,kk=0;
  unsigned int gnature=0;
  for (ii=0;ii<light_n;ii++) {
    lightsource=&(lightL[ii]);
    if ((lightsource->m_on&light_on)>0)
      glEnable(glight[ii]);
    else {
      glDisable(glight[ii]);
      continue;
    }
    for (jj=0;jj<lightspec_n;jj++) {
      lightspec=&(lightsource->m_materialL[jj]);
      find_material(lightspec->m_snature,0,&gnature);
      if (gnature==0)
        continue;
      for (kk=0;kk<4;kk++)
        glf[kk]=(float)lightspec->m_value[kk];
//printf ("%d idx [%d] type [%s] rgba [%f,%f,%f,%f]\n",__LINE__,
//ii,lightspec->m_snature,glf[0],glf[1],glf[2],glf[3]);
      glMaterialfv(GL_FRONT,gnature,glf);
    }
    for (jj=0;jj<lightspec_n;jj++) {
      lightspec=&(lightsource->m_specL[jj]);
      find_nature(lightspec->m_snature,0,&gnature);
      if (gnature==0)
        continue;
      for (kk=0;kk<4;kk++)
        glf[kk]=(float)lightspec->m_value[kk];
//printf ("%d idx [%d] type [%s] rgba [%f,%f,%f,%f]\n",__LINE__,
//ii,lightspec->m_snature,glf[0],glf[1],glf[2],glf[3]);
      glLightfv(glight[ii],gnature,glf);
    }
  }

}
