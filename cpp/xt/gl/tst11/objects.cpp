
#include <xt/gl/tst11/objects.h>
#include <xt/gl/tst11/glw.h>
#include <auxx/auxx.h>
#include <stdio.h>

static const float tstcolor[5][4]={
{.5,.9,.7,1.},{.5,.7,.9,1.},{.4,.4,.4,1.},{.9,.7,.5,1.},{.9,.1,.6,1.}
};

aux::Vertex sphere_vertex[12]={
aux::Vertex(-sphere_dist[0],.0,sphere_dist[1]),aux::Vertex(sphere_dist[0],.0,sphere_dist[1]),
aux::Vertex(-sphere_dist[0],.0,-sphere_dist[1]),aux::Vertex(sphere_dist[0],.0,-sphere_dist[1]),
aux::Vertex(.0,sphere_dist[1],sphere_dist[0]),aux::Vertex(.0,sphere_dist[1],-sphere_dist[0]),
aux::Vertex(.0,-sphere_dist[1],sphere_dist[0]),aux::Vertex(.0,-sphere_dist[1],-sphere_dist[0]),
aux::Vertex(sphere_dist[1],sphere_dist[0],.0),aux::Vertex(-sphere_dist[1],sphere_dist[0],.0),
aux::Vertex(sphere_dist[1],-sphere_dist[0],.0),aux::Vertex(-sphere_dist[1],-sphere_dist[0],.0)
};

aux::Vertex cube_vertex[36]={
aux::Vertex(-cube_dist,-cube_dist,-cube_dist),aux::Vertex(-cube_dist,-cube_dist,cube_dist),
aux::Vertex(-cube_dist,cube_dist,cube_dist),aux::Vertex(cube_dist,cube_dist,-cube_dist),
aux::Vertex(-cube_dist,-cube_dist,-cube_dist),aux::Vertex(-cube_dist,cube_dist,-cube_dist),
aux::Vertex(cube_dist,-cube_dist,cube_dist),aux::Vertex(-cube_dist,-cube_dist,-cube_dist),
aux::Vertex(cube_dist,-cube_dist,-cube_dist),aux::Vertex(cube_dist,cube_dist,-cube_dist),
aux::Vertex(cube_dist,-cube_dist,-cube_dist),aux::Vertex(-cube_dist,-cube_dist,-cube_dist),
aux::Vertex(-cube_dist,-cube_dist,-cube_dist),aux::Vertex(-cube_dist,cube_dist,cube_dist),
aux::Vertex(-cube_dist,cube_dist,-cube_dist),aux::Vertex(cube_dist,-cube_dist,cube_dist),
aux::Vertex(-cube_dist,-cube_dist,cube_dist),aux::Vertex(-cube_dist,-cube_dist,-cube_dist),
aux::Vertex(-cube_dist,cube_dist,cube_dist),aux::Vertex(-cube_dist,-cube_dist,cube_dist),
aux::Vertex(cube_dist,-cube_dist,cube_dist),aux::Vertex(cube_dist,cube_dist,cube_dist),
aux::Vertex(cube_dist,-cube_dist,-cube_dist),aux::Vertex(cube_dist,cube_dist,-cube_dist),
aux::Vertex(cube_dist,-cube_dist,-cube_dist),aux::Vertex(cube_dist,cube_dist,cube_dist),
aux::Vertex(cube_dist,-cube_dist,cube_dist),aux::Vertex(cube_dist,cube_dist,cube_dist),
aux::Vertex(cube_dist,cube_dist,-cube_dist),aux::Vertex(-cube_dist,cube_dist,-cube_dist),
aux::Vertex(cube_dist,cube_dist,cube_dist),aux::Vertex(-cube_dist,cube_dist,-cube_dist),
aux::Vertex(-cube_dist,cube_dist,cube_dist),aux::Vertex(cube_dist,cube_dist,cube_dist),
aux::Vertex(-cube_dist,cube_dist,cube_dist),aux::Vertex(cube_dist,-cube_dist,cube_dist)
};

int sphere_level=0,cube_level=0;

static aux::Vertex trianglenormal(
  aux::Vertex v1,aux::Vertex v2,aux::Vertex v3,aux::Vertex *trianglecenter) {

  if (trianglecenter) {
    trianglecenter->setX((v1.x()+v2.x()+v3.x())/3.);
    trianglecenter->setY((v1.y()+v2.y()+v3.y())/3.);
    trianglecenter->setZ((v1.z()+v2.z()+v3.z())/3.);
  }

  return (v1-v2).cross(v2-v3).norm();

}

static void obj_trianglenormal(aux::Vertex v1,aux::Vertex v2,aux::Vertex v3) {

  aux::Vertex nncenter,nn=trianglenormal(v1,v2,v3,&nncenter);
  glVertex3dv(nncenter.data());
  glVertex3dv((nncenter+nn).data());

}

static void obj_triangle(aux::Vertex v1,aux::Vertex v2,aux::Vertex v3) {

  glNormal3dv(trianglenormal(v1,v2,v3,0).data());
  glVertex3dv(v1.data());
  glVertex3dv(v2.data());
  glVertex3dv(v3.data());

}

static void subtriangle(
  aux::Vertex v1,aux::Vertex v2, aux::Vertex v3,int level,aux::TVList<aux::Vertex> *vL) {

  if (level<0)
    return;

  if (level==0) {
    vL->append(v1);
    vL->append(v2);
    vL->append(v3);
    return;
  }

  aux::Vertex v12=v1+v2,v23=v2+v3,v31=v3+v1;
  v12=v12.norm();
  v23=v23.norm();
  v31=v31.norm();

  subtriangle(v1,v12,v31,level-1,vL);
  subtriangle(v2,v23,v12,level-1,vL);
  subtriangle(v3,v31,v23,level-1,vL);
  subtriangle(v12,v23,v31,level-1,vL);

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
  aux::TVList<aux::Vertex> vL(20*(int)pow(4.,level));
  for (ii=0;ii<sphere_n;ii++) {
    subtriangle(
      sphere_vertex[sphere_index[ii][2]],
      sphere_vertex[sphere_index[ii][1]],
      sphere_vertex[sphere_index[ii][0]],level,&vL
    );
  }
  int vcnt=vL.count();

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
      obj_triangle(vL[ii],vL[ii+1],vL[ii+2]);
    }
    glEnd();
  }

  if ((drawstyle&draw_wireframe)>0) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if (!ison[0])
      glColor4fv(tstcolor[2]);
    glBegin(GL_TRIANGLES);
    for (ii=0;ii<vcnt;ii+=3) {
      obj_triangle(vL[ii],vL[ii+1],vL[ii+2]);
    }
    glEnd();
  }

  if ((drawstyle&draw_normals)>0) {
    glColor4fv(tstcolor[4]);
    glBegin(GL_LINES);
    for (ii=0;ii<vcnt;ii+=3) {
      obj_trianglenormal(vL[ii],vL[ii+1],vL[ii+2]);
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
  aux::Vertex vv;

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  if (!ison[0])
    glColor4fv(tstcolor[0]);

  glBegin(GL_TRIANGLES);
  for (ii=0;ii<sphere_n;ii++) {
    if (ii==sphere_n-1 && !ison[0])
      glColor4fv(tstcolor[2]);
    for (jj=0;jj<3;jj++) {
      vv=sphere_vertex[sphere_index[ii][jj]];
      glVertex3d(vv.x(),vv.y(),vv.z());
    }
  }
  glEnd();

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  if (!ison[0])
    glColor4fv(tstcolor[1]);

  glBegin(GL_TRIANGLES);
  for (ii=0;ii<sphere_n;ii++) {
    for (jj=0;jj<3;jj++) {
      vv=sphere_vertex[sphere_index[ii][jj]];
      glVertex3d(vv.x(),vv.y(),vv.z());
    }
  }
  glEnd();

}
// check

void draw_obj() {

  glTranslated(transformV[idxtr].x(),transformV[idxtr].y(),transformV[idxtr].z());
  glRotated(transformV[idxrot].x(),1.,.0,.0);
  glRotated(transformV[idxrot].y(),.0,1.,.0);
  glRotated(transformV[idxrot].z(),.0,.0,1.);
  glScaled(transformV[idxsc].x(),transformV[idxsc].y(),transformV[idxsc].z());

  obj_sphere(sphere_level-1);
  obj_cube(cube_level-1);

}
