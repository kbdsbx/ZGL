#ifndef ZGL_H
#define ZGL_H

#include "linear/matrix.h"
#include "linear/square.h"
#include "linear/vector.h"

#include "geometry/affine_vector.h"
#include "geometry/graph.h"
#include "geometry/gridding.h"

#include "scene/camera.h"
#include "scene/scene_grid.h"

#endif // !ZGL_H

typedef double zgl_item;

// 2D

typedef ZGL::affine_vector< 3, zgl_item > Dot2;
typedef ZGL::affine_vector< 3, zgl_item > Vector2;

typedef ZGL::graph< 3, 3, zgl_item > Graph2;
typedef ZGL::graph< 3, 2, zgl_item > Plane2;
typedef ZGL::graph< 3, 1, zgl_item > Line2;
typedef ZGL::graph< 3, 0, zgl_item > Point2;

typedef ZGL::gridding< 3, 2, zgl_item > Surface2;
typedef ZGL::gridding< 3, 1, zgl_item > Curved2;

// 3D

typedef ZGL::affine_vector< 4, zgl_item > Dot3;
typedef ZGL::affine_vector< 4, zgl_item > Vector3;

typedef ZGL::graph< 4, 4, zgl_item > Graph3;
typedef ZGL::graph< 4, 3, zgl_item > Solid3;
typedef ZGL::graph< 4, 2, zgl_item > Line3;
typedef ZGL::graph< 4, 1, zgl_item > Point3;

typedef ZGL::gridding< 4, 3, zgl_item > Grid3;
typedef ZGL::gridding< 4, 2, zgl_item > Surface3;
typedef ZGL::gridding< 4, 1, zgl_item > Curved3;