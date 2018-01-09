
#ifndef OBJECTS_H
#define OBJECTS_H

#include <auxx/vertex.h>
#include <GL/gl.h>

const int sphere_n=20,cube_n=36;
const double sphere_dist[2]={.525731112119133,.850650808352040};
const double cube_dist=1.;

const int sphere_index[sphere_n][3]={
{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11} };

extern aux::Vertex sphere_vertex[12];
extern aux::Vertex cube_vertex[36];

extern int sphere_level,cube_level;
extern void draw_obj();

#endif
