#pragma once

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

#ifndef ZGL_2D
#define ZGL_2D
#include "../../ZGL/zgl.h"

void _drow_line(const gLine& l) {
	setcolor(EGERGBA(0, 0, 0, 255));
	vector n, o;
	for (int i = 0; i < l._grid_data_len; i++) {
		n = l[{i}].get_dot();
		if (n != vector_zero && o != vector_zero) {
			printf("%f %f %f %f \n", n[0], n[1], o[0], o[1]);
			ege_line(X(n[0]), Y(n[1]), X(o[0]), Y(o[1]));
		}
		o = n;
	}
}

void _drow_grid(const gSurf& s) {
	setcolor(EGERGBA(100, 100, 100, 255));
	vector n, o;
	for (int i = 0; i < s._grid_data_len; i++) {
		for (int j = 0; j < s._grid_data_len; j++) {
			n = s[{i, j}].get_dot();
			if (n != vector_zero && o != vector_zero) {
				ege_line(X(n[0]), Y(n[1]), X(o[0]), Y(o[1]));
			}
		}
	}
	n = vector_zero, o = vector_zero;
	for (int i = 0; i < s._grid_data_len; i++) {
		for (int j = 0; j < s._grid_data_len; j++) {
			n = s[{j, i}].get_dot();
			if (n != vector_zero && o != vector_zero) {
				ege_line(X(n[0]), Y(n[1]), X(o[0]), Y(o[1]));
			}
		}
	}
}


// gridding to line
void test_1 () {
	gLine g1({
		gLine::grid_data(-PI * 2, PI * 2, 800),
		gLine::grid_data([](vector v) { return atan(v[0]) * sin(v[0]) / cos(v[0]); }),
	});

	for (auto& g : g1) {
		g.set_dot(g.get_dot() * .3);
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

void test_2() {
	gLine xax = xAxis(3);
	// gLine yax = yAxis(3);
	// gSurf suf = grid(screen_width, screen_height);

	for (; is_run(); delay_fps(60), cleardevice()) {
		_drow_line(xax);
		// _drow_line(yax);
	}
}


#undef ZGL_2D
#endif;