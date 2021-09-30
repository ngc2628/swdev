
#include <xt/gl/tst11/objects.h>
#include <xt/gl/tst11/glw.h>
#include <stdio.h>

static const float tstcolor[5][4]={
{.5,.9,.7,1.},{.5,.7,.9,1.},{.4,.4,.4,1.},{.9,.7,.5,1.},{.9,.1,.6,1.}
};

int sphere_index[sphere_n][3]={
{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11} };

mk_vertex sphere_vertex[spherevertex_n]={
  {-sphere_dist_0,.0,sphere_dist_1},{sphere_dist_0,.0,sphere_dist_1},
  {-sphere_dist_0,.0,-sphere_dist_1},{sphere_dist_0,.0,-sphere_dist_1},
  {.0,sphere_dist_1,sphere_dist_0},{.0,sphere_dist_1,-sphere_dist_0},
  {.0,-sphere_dist_1,sphere_dist_0},{.0,-sphere_dist_1,-sphere_dist_0},
  {sphere_dist_1,sphere_dist_0,.0},{-sphere_dist_1,sphere_dist_0,.0},
  {sphere_dist_1,-sphere_dist_0,.0},{-sphere_dist_1,-sphere_dist_0,.0}
};

mk_vertex cube_vertex[cube_n]={
  {-cube_dist,-cube_dist,-cube_dist},{-cube_dist,-cube_dist,cube_dist},
  {-cube_dist,cube_dist,cube_dist},{cube_dist,cube_dist,-cube_dist},
  {-cube_dist,-cube_dist,-cube_dist},{-cube_dist,cube_dist,-cube_dist},
  {cube_dist,-cube_dist,cube_dist},{-cube_dist,-cube_dist,-cube_dist},
  {cube_dist,-cube_dist,-cube_dist},{cube_dist,cube_dist,-cube_dist},
  {cube_dist,-cube_dist,-cube_dist},{-cube_dist,-cube_dist,-cube_dist},
  {-cube_dist,-cube_dist,-cube_dist},{-cube_dist,cube_dist,cube_dist},
  {-cube_dist,cube_dist,-cube_dist},{cube_dist,-cube_dist,cube_dist},
  {-cube_dist,-cube_dist,cube_dist},{-cube_dist,-cube_dist,-cube_dist},
  {-cube_dist,cube_dist,cube_dist},{-cube_dist,-cube_dist,cube_dist},
  {cube_dist,-cube_dist,cube_dist},{cube_dist,cube_dist,cube_dist},
  {cube_dist,-cube_dist,-cube_dist},{cube_dist,cube_dist,-cube_dist},
  {cube_dist,-cube_dist,-cube_dist},{cube_dist,cube_dist,cube_dist},
  {cube_dist,-cube_dist,cube_dist},{cube_dist,cube_dist,cube_dist},
  {cube_dist,cube_dist,-cube_dist},{-cube_dist,cube_dist,-cube_dist},
  {cube_dist,cube_dist,cube_dist},{-cube_dist,cube_dist,-cube_dist},
  {-cube_dist,cube_dist,cube_dist},{cube_dist,cube_dist,cube_dist},
  {-cube_dist,cube_dist,cube_dist},{cube_dist,-cube_dist,cube_dist}
};

int sphere_level=0,cube_level=0;

static int trianglenormal(
  mk_vertex v1,mk_vertex v2,mk_vertex v3,
  mk_vertex trianglenn,mk_vertex trianglecenter) {

  trianglecenter[0]=(v1[0]+v2[0]+v3[0])/3.;
  trianglecenter[1]=(v1[1]+v2[1]+v3[1])/3.;
  trianglecenter[2]=(v1[2]+v2[2]+v3[2])/3.;

  mk_vertexzero(v12);
  mk_vertexsubs(v1,v2,v12);
  mk_vertexzero(v23);
  mk_vertexsubs(v2,v3,v23);
  mk_vertexzero(vc);
  mk_vertexcross(v12,v23,vc);
  mk_vertexnorm(vc,trianglenn);  
  return 0;

}

static void obj_trianglenormal(
  mk_vertex v1,mk_vertex v2,mk_vertex v3) {

  mk_vertexzero(normal);
  mk_vertexzero(center);
  trianglenormal(v1,v2,v3,normal,center);
  glVertex3dv(&center[0]);
  mk_vertexzero(vnc);
  mk_vertexadd(center,normal,vnc);
  glVertex3dv(&vnc[0]);

}

static void obj_triangle(
  mk_vertex v1,mk_vertex v2,mk_vertex v3) {

  mk_vertexzero(normal);
  mk_vertexzero(center);
  trianglenormal(v1,v2,v3,normal,center);
  glNormal3dv(&normal[0]);
  glVertex3dv(&v1[0]);
  glVertex3dv(&v2[0]);
  glVertex3dv(&v3[0]);

}

static void subtriangle(
  mk_vertex vv1,mk_vertex vv2,mk_vertex vv3,
  int level,struct mk_list *vL) {

  if (level<0)
    return;

  mk_vertexzero(v1);
  mk_vertexcopy(v1,vv1);
  mk_vertexzero(v2);
  mk_vertexcopy(v2,vv2);
  mk_vertexzero(v3);
  mk_vertexcopy(v3,vv3);

  if (level==0) {
    mk_listsetat(vL,(void*)&v1,vL->count,1);
    mk_listsetat(vL,(void*)&v2,vL->count,1);
    mk_listsetat(vL,(void*)&v3,vL->count,1);
    return;
  }

  mk_vertexzero(v12);
  mk_vertexzero(v23);
  mk_vertexzero(v31);
  mk_vertexzero(v12n);
  mk_vertexzero(v23n);
  mk_vertexzero(v31n);  

  mk_vertexadd(v1,v2,v12);
  mk_vertexadd(v2,v3,v23);
  mk_vertexadd(v3,v1,v31);
  mk_vertexnorm(v12,v12n);  
  mk_vertexnorm(v23,v23n);  
  mk_vertexnorm(v31,v31n);  

  subtriangle(v1,v12n,v31n,level-1,vL);
  subtriangle(v2,v23n,v12n,level-1,vL);
  subtriangle(v3,v31n,v23n,level-1,vL);
  subtriangle(v12n,v23n,v31n,level-1,vL);

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
  struct mk_list vL; 
  mk_listalloc(&vL,sizeof(mk_vertex),4*20*(int)pow(4.,level)); /* 4 : do not exhaust */
  for (ii=0;ii<sphere_n;ii++) {
    subtriangle(
      sphere_vertex[sphere_index[ii][2]],
      sphere_vertex[sphere_index[ii][1]],
      sphere_vertex[sphere_index[ii][0]],level,&vL
    );
  }
  mk_vertexzero(v1);
  mk_vertexzero(v2);
  mk_vertexzero(v3);
  int vcnt=vL.count;

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
      mk_listat(&vL,ii,&v1);
      mk_listat(&vL,ii+1,&v2);
      mk_listat(&vL,ii+2,&v3);
      obj_triangle(v1,v2,v3);
    }
    glEnd();
  }

  if ((drawstyle&draw_wireframe)>0) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if (!ison[0])
      glColor4fv(tstcolor[2]);
    glBegin(GL_TRIANGLES);
    for (ii=0;ii<vcnt;ii+=3) {
      mk_listat(&vL,ii,&v1);
      mk_listat(&vL,ii+1,&v2);
      mk_listat(&vL,ii+2,&v3);
      obj_triangle(v1,v2,v3);
    }
    glEnd();
  }

  if ((drawstyle&draw_normals)>0) {
    glColor4fv(tstcolor[4]);
    glBegin(GL_LINES);
    for (ii=0;ii<vcnt;ii+=3) {
      mk_listat(&vL,ii,&v1);
      mk_listat(&vL,ii+1,&v2);
      mk_listat(&vL,ii+2,&v3);
      obj_trianglenormal(v1,v2,v3);
    }
    glEnd();
  }

  glPopMatrix();
  mk_listfree(&vL);

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
      obj_triangle(cube_vertex[ii],cube_vertex[ii+1],cube_vertex[ii+2]);
    }
    glEnd();
  }

  if ((drawstyle&draw_wireframe)>0) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if (!ison[0])
      glColor4fv(tstcolor[2]);
    glBegin(GL_TRIANGLES);
    for (ii=0;ii<cube_n;ii+=3) {
      obj_triangle(cube_vertex[ii],cube_vertex[ii+1],cube_vertex[ii+2]);
    }
    glEnd();
  }

  if ((drawstyle&draw_normals)>0) {
    glColor4fv(tstcolor[4]);
    glBegin(GL_LINES);
    for (ii=0;ii<cube_n;ii+=3) {
      obj_trianglenormal(cube_vertex[ii],cube_vertex[ii+1],cube_vertex[ii+2]);
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
  mk_vertexzero(vv);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  if (!ison[0])
    glColor4fv(tstcolor[0]);

  glBegin(GL_TRIANGLES);
  for (ii=0;ii<sphere_n;ii++) {
    if (ii==sphere_n-1 && !ison[0])
      glColor4fv(tstcolor[2]);
    for (jj=0;jj<3;jj++) {
      mk_vertexcopy(vv,sphere_vertex[sphere_index[ii][jj]]);
      glVertex3d(vv[0],vv[1],vv[2]);
    }
  }
  glEnd();

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  if (!ison[0])
    glColor4fv(tstcolor[1]);

  glBegin(GL_TRIANGLES);
  for (ii=0;ii<sphere_n;ii++) {
    for (jj=0;jj<3;jj++) {
      mk_vertexcopy(vv,sphere_vertex[sphere_index[ii][jj]]);
      glVertex3d(vv[0],vv[1],vv[2]);
    }
  }
  glEnd();

}
// check

void draw_obj() {

  glTranslated(
    transformV[idxtr][0],transformV[idxtr][1],transformV[idxtr][2]);
  glRotated(transformV[idxrot][0],1.,.0,1.);
  glRotated(transformV[idxrot][1],.0,1.,1.);
  glRotated(transformV[idxrot][2],.0,.0,1.);
  glScaled(
    transformV[idxsc][0],transformV[idxsc][1],transformV[idxsc][2]);

  obj_sphere(sphere_level-1);
  obj_cube(cube_level-1);

}
