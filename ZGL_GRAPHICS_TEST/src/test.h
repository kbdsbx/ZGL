#pragma once

#define ZGL_2D
#include "../../ZGL/zgl.h"

#define screen_width 800
#define screen_height 600
#define X(x) ( (x) * 100 + ( screen_width / 2 ) )
#define Y(y) ( (y) * 100 + ( screen_height / 2 ) )

// successful.
void loop_test () {
	for (; is_run(); delay_fps(60), cleardevice()) {
		ege_line(100, 200, 300, 400);
	}
}

// gridding to line
void test_1 () {
	gLine g1({
		gLine::grid_data(-PI * 2, PI * 2, 800),
		gLine::grid_data([](vector v) { return sin(v[0]); }),
	});
	g1.discrete();

	for (auto& g : g1) {
		g.set_dot(g.get_dot() * 3);
	}

	for (; is_run(); delay_fps(60), cleardevice()) {
		vector n, o;
		for (int i = 0; i < 800; i++) {
			n = g1[{i}].get_dot();
			if (n != vector_zero && o != vector_zero) {
				printf("%f %f %f %f \n", n[0], n[1], o[0], o[1]);
				ege_line(X(n[0]), Y(n[1]), X(o[0]), Y(o[1]));
			}
			o = n;
		}
	}
}