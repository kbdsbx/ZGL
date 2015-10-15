#pragma once

#define screen_width 800
#define screen_height 600
#define X(x) ( (x) * 100 + ( screen_width / 2 ) )
#define Y(y) ( (-(y)) * 100 + ( screen_height / 2 ) )

void axis() {
	// X Axis;
	ege_line(X(-screen_width / 2), Y(0), X(screen_width / 2), Y(0));
	ege_line(X(0), Y(-screen_height / 2), X(0), Y(screen_height / 2));
}

// successful.
void loop_test () {
	for (; is_run(); delay_fps(60), cleardevice()) {
		ege_line(100, 200, 300, 400);
	}
}

#ifndef ZGL_2D
#define ZGL_2D
#undef  ZGL_3D
#include "../../ZGL/zgl.h"

// 2D-vector translate successfull;
void test_2d_vector_translate() {
	Vector v1_1{ 1.5, 0.5, 1 };
	Vector v1_2{ -0.5, 0.85, 1 };
	Vector v2_1 = v1_1 * Vector::translate({ -0.3, -1.5, 0});
	Vector v2_2 = v1_2 * Vector::translate({ -0.3, -1.5, 0});


	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale for original point is successful;
void test_2d_vector_scale_original() {
	Vector v1_1{ 1.5, 0.5, 1 };
	Vector v1_2{ -0.5, 0.85, 1 };
	Vector v2_1 = v1_1 * Vector::scale(2);
	Vector v2_2 = v1_2 * Vector::scale(2);

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale for fix point is successful;
void test_2d_vector_scale_fix_point() {
	Vector v1_1{ 1.5, 0.5, 1 };
	Vector v1_2{ -0.5, 0.85, 1 };
	Vector v2_1 = v1_1 * Vector::scale(2, { -1.0, -1.0, 0 });
	Vector v2_2 = v1_2 * Vector::scale(2, { -1.0, -1.0, 0 });

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale along the other direction is sunccessful;
void test_2d_vector_scale() {
	Vector v1_1{ 1.5, 0.5, 1 };
	Vector v1_2{ -0.5, 0.85, 1 };
	Vector v2_1 = v1_1 * Vector::scale(2.0, Graph(v1_2, {
		{ 1, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 1 }
	}));
	Vector v2_2 = v1_2 * Vector::scale(2.0, Graph(v1_2, {
		{ 1, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 1 }
	}));

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector mirror with line is successfull;
void test_2d_vector_mirror() {
	Vector v1_1{ 1.5, 0.5, 1 };
	Vector v1_2{ -0.5, 0.85, 1 };
	Vector v2_1 = v1_1 * Vector::mirror(Line({ -1.0, 0.0, 0 }, {
		{ 1.0, 1.0, 0 },
	}));
	Vector v2_2 = v1_2 * Vector::mirror(Line({ -1.0, 0.0, 0 }, {
		{ 1.0, 1.0, 0 },
	}));

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector orthogonal is successfull;
void test_2d_vector_orthogonal() {
	Vector v1_1{ 1.5, 0.5, 1 };
	Vector v1_2{ -0.5, 0.85, 1 };
	Vector v2_1 = v1_1 * Vector::orthogonal(Line({ -1.0, 0.0, 1 }, {
		{ 0.0, 1.0, 0 },
	}));
	Vector v2_2 = v1_2 * Vector::orthogonal(Line({ -1.0, 0.0, 1 }, {
		{ 0.0, 1.0, 0 },
	}));

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector rotate is successfull;
void test_2d_vector_rotate() {
	Vector v1_1{ 1.5, 0.5, 1 };
	Vector v1_2{ -0.5, 0.85, 1 };
	Vector v2_1 = v1_1 * Vector::rotate(Point({ 0.0, 0.0, 1.0 }, { { 0, 0, 0 } }), 1.5);
	Vector v2_2 = v1_2 * Vector::rotate(Point({ 0.0, 0.0, 1.0 }, { { 0, 0, 0 } }), 1.5);

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

#undef ZGL_2D
#endif;

#ifndef ZGL_3D
#define ZGL_3D
#undef  ZGL_2D
#include "../../ZGL/zgl.h"

void test_3d_vector_rotate() {
	Vector v1_1{ 1.5, 0.5, -.8, 1 };
	Vector v1_2{ -0.5, 0.85, 0.6, 1 };
	Vector v2_1 = v1_1 * Vector::rotate(Point({ 1.0, 0.5, -0.5, 1.0 }, { { -1, 1.0, 0.0, 0 } }), 1.5);
	Vector v2_2 = v1_2 * Vector::rotate(Point({ 1.0, 0.5, -0.5, 1.0 }, { { -1, 1.0, 0.0, 0 } }), 1.5);
}

#undef ZGL_3D
#endif;