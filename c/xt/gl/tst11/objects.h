
#ifndef OBJECTS_H
#define OBJECTS_H

#include <mkbase/mkbase.h>
#include <mkbase/mkla.h>
#include <GL/gl.h>

#define sphere_n 20
#define spherevertex_n 12
#define cube_n 36
#define sphere_dist_0 .525731112119133
#define sphere_dist_1 .850650808352040
#define cube_dist 1.

extern int sphere_index[sphere_n][3];

extern mk_vertex sphere_vertex[spherevertex_n];
extern mk_vertex cube_vertex[cube_n];

extern int sphere_level;
extern int cube_level;
extern void draw_obj();

#endif
