
#include <xt/gl/tst11/objects.h>
#include <xt/gl/tst11/glw.h>
#include <stdio.h>

static const float tstcolor[5][4]={
{.5,.9,.7,1.},{.5,.7,.9,1.},{.4,.4,.4,1.},{.9,.7,.5,1.},{.9,.1,.6,1.}
};

struct mk_vertex sphere_vertex[12]=
{ {{-sphere_dist[0],.0,sphere_dist[1],1.}},{{sphere_dist[0],.0,sphere_dist[1],1.}},
  {{-sphere_dist[0],.0,-sphere_dist[1],1.}},{{sphere_dist[0],.0,-sphere_dist[1],1.}},
  {{.0,sphere_dist[1],sphere_dist[0],1.}},{{.0,sphere_dist[1],-sphere_dist[0],1.}},
  {{.0,-sphere_dist[1],sphere_dist[0],1.}},{{.0,-sphere_dist[1],-sphere_dist[0],1.}},
  {{sphere_dist[1],sphere_dist[0],.0,1.}},{{-sphere_dist[1],sphere_dist[0],.0,1.}},
  {{sphere_dist[1],-sphere_dist[0],.0,1.}},{{-sphere_dist[1],-sphere_dist[0],.0,1.}} };

struct mk_vertex cube_vertex[36]=
  { {{-cube_dist,-cube_dist,-cube_dist,1.}},{{-cube_dist,-cube_dist,cube_dist,1.}},
    {{-cube_dist,cube_dist,cube_dist,1.}},{{cube_dist,cube_dist,-cube_dist,1.}},
    {{-cube_dist,-cube_dist,-cube_dist,1.}},{{-cube_dist,cube_dist,-cube_dist,1.}},
    {{cube_dist,-cube_dist,cube_dist,1.}},{{-cube_dist,-cube_dist,-cube_dist,1.}},
    {{cube_dist,-cube_dist,-cube_dist,1.}},{{cube_dist,cube_dist,-cube_dist,1.}},
    {{cube_dist,-cube_dist,-cube_dist,1.}},{{-cube_dist,-cube_dist,-cube_dist,1.}},
    {{-cube_dist,-cube_dist,-cube_dist,1.}},{{-cube_dist,cube_dist,cube_dist,1.}},
    {{-cube_dist,cube_dist,-cube_dist,1.}},{{cube_dist,-cube_dist,cube_dist,1.}},
    {{-cube_dist,-cube_dist,cube_dist,1.}},{{-cube_dist,-cube_dist,-cube_dist,1.}},
    {{-cube_dist,cube_dist,cube_dist,1.}},{{-cube_dist,-cube_dist,cube_dist,1.}},
    {{cube_dist,-cube_dist,cube_dist,1.}},{{cube_dist,cube_dist,cube_dist,1.}},
    {{cube_dist,-cube_dist,-cube_dist,1.}},{{cube_dist,cube_dist,-cube_dist,1.}},
    {{cube_dist,-cube_dist,-cube_dist,1.}},{{cube_dist,cube_dist,cube_dist,1.}},
    {{cube_dist,-cube_dist,cube_dist,1.}},{{cube_dist,cube_dist,cube_dist,1.}},
    {{cube_dist,cube_dist,-cube_dist,1.}},{{-cube_dist,cube_dist,-cube_dist,1.}},
    {{cube_dist,cube_dist,cube_dist,1.}},{{-cube_dist,cube_dist,-cube_dist,1.}},
    {{-cube_dist,cube_dist,cube_dist,1.}},{{cube_dist,cube_dist,cube_dist,1.}},
    {{-cube_dist,cube_dist,cube_dist,1.}},{{cube_dist,-cube_dist,cube_dist,1.}} };

int sphere_level=0,cube_level=0;

static int trianglenormal(
  struct mk_vertex *v1,struct mk_vertex *v2,struct mk_vertex *v3,
  struct mk_vertex *trianglenormal,struct mk_vertex *trianglecenter) {

  if (trianglecenter) {
    trianglecenter->xyzw[0]=((v1->xyzw[0]+v2->xyzw[0]+v3->xyzw[0])/3.);
    trianglecenter->xyzw[1]=((v1->xyzw[1]+v2->xyzw[1]+v3->xyzw[1])/3.);
    trianglecenter->xyzw[2]=((v1->xyzw[2]+v2->xyzw[2]+v3->xyzw[2])/3.);
  }

  struct mk_vertex vv23;
  mk_vertexcopy(trianglenormal,v1);
  mk_vertexsubs(trianglenormal,v2);
  mk_vertexcopy(&vv23,v2);
  mk_vertexsubs(&vv23,v3);
  mk_vertexcross(trianglenormal,&vv23);
  mk_vertexnorm(trianglenormal);
  return 0;

}

static void obj_trianglenormal(
  struct mk_vertex *v1,struct mk_vertex *v2,struct mk_vertex *v3) {

  struct mk_vertex normal,center;
  trianglenormal(v1,v2,v3,&normal,&center);
  glVertex3dv(&center.xyzw[0]);
  mk_vertexadd(&center,&normal);
  glVertex3dv(&center.xyzw[0]);

}

static void obj_triangle(
  struct mk_vertex *v1,struct mk_vertex *v2,struct mk_vertex *v3) {

  struct mk_vertex normal;
  trianglenormal(v1,v2,v3,&normal,0);
  glNormal3dv(&normal.xyzw[0]);
  glVertex3dv(&(v1->xyzw)[0]);
  glVertex3dv(&(v2->xyzw[0]));
  glVertex3dv(&(v3->xyzw[0]));

}

static void subtriangle(
  struct mk_vertex *v1,struct mk_vertex *v2,struct mk_vertex *v3,
  int level,struct mk_vertices *vL) {

  if (level<0)
    return;

  if (level==0) {
    mk_verticesappend(vL,v1);
    mk_verticesappend(vL,v2);
    mk_verticesappend(vL,v3);
    return;
  }

  struct mk_vertex v12,v23,v31;
  mk_vertexcopy(&v12,v1);
  mk_vertexadd(&v12,v2);
  mk_vertexcopy(&v23,v2);
  mk_vertexadd(&v23,v3);
  mk_vertexcopy(&v31,v3);
  mk_vertexadd(&v31,v1);

  mk_vertexnorm(&v12);
  mk_vertexnorm(&v23);
  mk_vertexnorm(&v31);

  subtriangle(v1,&v12,&v31,level-1,vL);
  subtriangle(v2,&v23,&v12,level-1,vL);
  subtriangle(v3,&v31,&v23,level-1,vL);
  subtriangle(&v12,&v23,&v31,level-1,vL);

}

static void obj_sphere(int level) {

  if (level<0 ||
       ((drawstyle&draw_fill)==0 &&
        (drawstyle&draw_wireframe)==0 &&
        (drawstyle&draw_normals)==0)
     )
    return;

  unsigned char ison[1]={0};
  glGetBooleanv(GL_LIGHTING,&ison[0]);

  int ii=0;
  struct mk_vertices vL={0,0,4*20*(int)pow(4.,level)}; /* 4 : do not exhaust */
  mk_verticesalloc(&vL);
  for (ii=0;ii<sphere_n;ii++) {
    subtriangle(
      &(sphere_vertex[sphere_index[ii][2]]),
      &(sphere_vertex[sphere_index[ii][1]]),
      &(sphere_vertex[sphere_index[ii][0]]),level,&vL
    );
  }
  int vcnt=vL.cnt;

  glPushMatrix();
  glTranslated(-1.,0.,0.);

  if ((drawstyle&draw_fill)>0) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    if (!ison[0])
      glColor4fv(tstcolor[0]);
    glBegin(GL_TRIANGLES);
    for (ii=0;ii<vcnt;ii+=3) {
      if (ii==95*vcnt/100 && !ison[0])
        glColor4fv(tstcolor[3]);
      obj_triangle(mk_verticesget(&vL,ii),mk_verticesget(&vL,ii+1),mk_verticesget(&vL,ii+2));
    }
    glEnd();
  }

  if ((drawstyle&draw_wireframe)>0) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if (!ison[0])
      glColor4fv(tstcolor[2]);
    glBegin(GL_TRIANGLES);
    for (ii=0;ii<vcnt;ii+=3) {
      obj_triangle(mk_verticesget(&vL,ii),mk_verticesget(&vL,ii+1),mk_verticesget(&vL,ii+2));
    }
    glEnd();
  }

  if ((drawstyle&draw_normals)>0) {
    glColor4fv(tstcolor[4]);
    glBegin(GL_LINES);
    for (ii=0;ii<vcnt;ii+=3) {
      obj_trianglenormal(mk_verticesget(&vL,ii),mk_verticesget(&vL,ii+1),mk_verticesget(&vL,ii+2));
    }
    glEnd();
  }

  glPopMatrix();

}

static void obj_cube(int level) {

  if (level<0 ||
       ((drawstyle&draw_fill)==0 &&
        (drawstyle&draw_wireframe)==0 &&
        (drawstyle&draw_normals)==0)
     )
    return;

  unsigned char ison[1]={0};
  glGetBooleanv(GL_LIGHTING,&ison[0]);

  int ii=0,jj=0;

  glPushMatrix();
  glTranslated(1.,0.,0.);

  if ((drawstyle&draw_fill)>0) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    if (!ison[0])
      glColor4fv(tstcolor[1]);
    glBegin(GL_TRIANGLES);
    for (ii=0;ii<cube_n;ii+=3) {
      if (ii>cube_n-4 && !ison[0])
        glColor4fv(tstcolor[2]);
      obj_triangle(&(cube_vertex[ii]),&(cube_vertex[ii+1]),&(cube_vertex[ii+2]));
    }
    glEnd();
  }

  if ((drawstyle&draw_wireframe)>0) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if (!ison[0])
      glColor4fv(tstcolor[2]);
    glBegin(GL_TRIANGLES);
    for (ii=0;ii<cube_n;ii+=3) {
      obj_triangle(&(cube_vertex[ii]),&(cube_vertex[ii+1]),&(cube_vertex[ii+2]));
    }
    glEnd();
  }

  if ((drawstyle&draw_normals)>0) {
    glColor4fv(tstcolor[4]);
    glBegin(GL_LINES);
    for (ii=0;ii<cube_n;ii+=3) {
      obj_trianglenormal(&(cube_vertex[ii]),&(cube_vertex[ii+1]),&(cube_vertex[ii+2]));
    }
    glEnd();
  }

  glPopMatrix();

}

// check
static void obj_sphere20() {

  unsigned char ison[1]={0};
  glGetBooleanv(GL_LIGHTING,&ison[0]);

  int ii=0,jj=0;
  struct mk_vertex *vv=0;

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  if (!ison[0])
    glColor4fv(tstcolor[0]);

  glBegin(GL_TRIANGLES);
  for (ii=0;ii<sphere_n;ii++) {
    if (ii==sphere_n-1 && !ison[0])
      glColor4fv(tstcolor[2]);
    for (jj=0;jj<3;jj++) {
      vv=&(sphere_vertex[sphere_index[ii][jj]]);
      glVertex3d(vv->xyzw[0],vv->xyzw[1],vv->xyzw[2]);
    }
  }
  glEnd();

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  if (!ison[0])
    glColor4fv(tstcolor[1]);

  glBegin(GL_TRIANGLES);
  for (ii=0;ii<sphere_n;ii++) {
    for (jj=0;jj<3;jj++) {
      vv=&(sphere_vertex[sphere_index[ii][jj]]);
      glVertex3d(vv->xyzw[0],vv->xyzw[1],vv->xyzw[2]);
    }
  }
  glEnd();

}
// check

void draw_obj() {

  glTranslated(
    transformV[idxtr].xyzw[0],transformV[idxtr].xyzw[1],transformV[idxtr].xyzw[2]);
  glRotated(transformV[idxrot].xyzw[0],1.,.0,1.);
  glRotated(transformV[idxrot].xyzw[1],.0,1.,1.);
  glRotated(transformV[idxrot].xyzw[2],.0,.0,1.);
  glScaled(
    transformV[idxsc].xyzw[0],transformV[idxsc].xyzw[1],transformV[idxsc].xyzw[2]);

  obj_sphere(sphere_level-1);
  obj_cube(cube_level-1);

}
