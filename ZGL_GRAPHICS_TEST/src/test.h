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
#include "../../ZGL/zgl.h"

// 2D-vector translate successfull;
void test_2d_vector_translate() {
	vector v1_1{ 1.5, 0.5, 1 };
	vector v1_2{ -0.5, 0.85, 1 };
	vector v2_1 = v1_1 * vector::translate({ -0.3, -1.5, 0});
	vector v2_2 = v1_2 * vector::translate({ -0.3, -1.5, 0});


	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale for original point is successful;
void test_2d_vector_scale_original() {
	vector v1_1{ 1.5, 0.5, 1 };
	vector v1_2{ -0.5, 0.85, 1 };
	vector v2_1 = v1_1 * vector::scale(2);
	vector v2_2 = v1_2 * vector::scale(2);

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale for fix point is successful;
void test_2d_vector_scale_fix_point() {
	vector v1_1{ 1.5, 0.5, 1 };
	vector v1_2{ -0.5, 0.85, 1 };
	vector v2_1 = v1_1 * vector::scale(2, { -1.0, -1.0, 0 });
	vector v2_2 = v1_2 * vector::scale(2, { -1.0, -1.0, 0 });

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale along the other direction is sunccessful;
void test_2d_vector_scale() {
	vector v1_1{ 1.5, 0.5, 1 };
	vector v1_2{ -0.5, 0.85, 1 };
	vector v2_1 = v1_1 * vector::scale(2.0, Graph(v1_2, {
		{ 1, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 1 }
	}));
	vector v2_2 = v1_2 * vector::scale(2.0, Graph(v1_2, {
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
	vector v1_1{ 1.5, 0.5, 1 };
	vector v1_2{ -0.5, 0.85, 1 };
	vector v2_1 = v1_1 * vector::mirror(Line({ -1.0, 0.0, 0 }, {
		{ 1.0, 1.0, 0 },
	}));
	vector v2_2 = v1_2 * vector::mirror(Line({ -1.0, 0.0, 0 }, {
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
	vector v1_1{ 1.5, 0.5, 1 };
	vector v1_2{ -0.5, 0.85, 1 };
	vector v2_1 = v1_1 * vector::orthogonal(Line({ -1.0, 0.0, 1 }, {
		{ 0.0, 1.0, 0 },
	}));
	vector v2_2 = v1_2 * vector::orthogonal(Line({ -1.0, 0.0, 1 }, {
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
	vector v1_1{ 1.5, 0.5, 1 };
	vector v1_2{ -0.5, 0.85, 1 };
	vector v2_1 = v1_1 * vector::rotate(Point({ 0.0, 0.0, 1 }), 1.5);
	vector v2_2 = v1_2 * vector::rotate(Point({ 0.0, 0.0, 1 }), 1.5);

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

#undef ZGL_2D
#endif;