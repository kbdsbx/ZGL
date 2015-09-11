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

typedef ZGL::gridding< 3, 1, zgl_item > gLine;
typedef ZGL::gridding< 3, 2, zgl_item > gSurf;

gLine xAxis (zgl_item w) {
	return gLine{
		gLine::grid_data(-w, w, 100),
		gLine::grid_data(0, 0, 100)
	};
}

gLine yAxis(zgl_item h) {
	return gLine {
		gLine::grid_data(0, 0, 100),
		gLine::grid_data(-h, h, 100)
	};
}

gSurf grid(zgl_item w, zgl_item h) {
	return gSurf {
		gSurf::grid_data(-1.0, 1.0, 100),
		gSurf::grid_data(-1.0, 1.0, 100)
	};
}

#elif defined ZGL_3D

typedef ZGL::affine_vector< 4, zgl_item > dot;
typedef ZGL::affine_vector< 4, zgl_item > vector;
const dot dot_zero { 0, 0, 1 };
const vector vector_zero;

typedef ZGL::gridding< 4, 1, zgl_item > gLine;
typedef ZGL::gridding< 4, 2, zgl_item > gSurf;
typedef ZGL::gridding< 4, 3, zgl_item > gFram;

#elif defined ZGL_2D && defined ZGL_3D

#endif