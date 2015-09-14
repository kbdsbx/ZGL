#ifndef ZGL_H
#define ZGL_H

#include "linear/matrix.h"
#include "linear/square.h"
#include "linear/vector.h"

#include "geometry/affine_vector.h"
#include "geometry/graph.h"
#include "geometry/gridding.h"


#endif // !ZGL_H

typedef double zgl_item;

#if defined ZGL_2D

typedef ZGL::affine_vector< 3, zgl_item > dot;
typedef ZGL::affine_vector< 3, zgl_item > vector;
const dot dot_zero { 0, 0, 1 };
const vector vector_zero;

typedef ZGL::graph< 3, 3, zgl_item > Graph;
typedef ZGL::graph< 3, 2, zgl_item > Plane;
typedef ZGL::graph< 3, 1, zgl_item > Line;


#elif defined ZGL_3D

typedef ZGL::affine_vector< 4, zgl_item > dot;
typedef ZGL::affine_vector< 4, zgl_item > vector;
const dot dot_zero { 0, 0, 1 };
const vector vector_zero;

#elif defined ZGL_2D && defined ZGL_3D

#endif