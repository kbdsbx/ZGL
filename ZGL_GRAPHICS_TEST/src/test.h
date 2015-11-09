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

#include "../../ZGL/zgl.h"

// 2D-vector translate successfull;
void test_2d_vector_translate() {
	Vector2 v1_1{ 1.5, 0.5, 1 };
	Vector2 v1_2{ -0.5, 0.85, 1 };
	Vector2 v2_1 = v1_1 * Vector2::translate({ -0.3, -1.5, 0});
	Vector2 v2_2 = v1_2 * Vector2::translate({ -0.3, -1.5, 0});


	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale for original point is successful;
void test_2d_vector_scale_original() {
	Vector2 v1_1{ 1.5, 0.5, 1 };
	Vector2 v1_2{ -0.5, 0.85, 1 };
	Vector2 v2_1 = v1_1 * Vector2::scale(2);
	Vector2 v2_2 = v1_2 * Vector2::scale(2);

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale for fix point is successful;
void test_2d_vector_scale_fix_point() {
	Vector2 v1_1{ 1.5, 0.5, 1 };
	Vector2 v1_2{ -0.5, 0.85, 1 };
	Vector2 v2_1 = v1_1 * Vector2::scale(2, { -1.0, -1.0, 0 });
	Vector2 v2_2 = v1_2 * Vector2::scale(2, { -1.0, -1.0, 0 });

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector scale along the other direction is sunccessful;
void test_2d_vector_scale() {
	Vector2 v1_1{ 1.5, 0.5, 1 };
	Vector2 v1_2{ -0.5, 0.85, 1 };
	Vector2 v2_1 = v1_1 * Vector2::scale(2.0, Graph2(v1_2, {
		{ 1, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 1 }
	}));
	Vector2 v2_2 = v1_2 * Vector2::scale(2.0, Graph2(v1_2, {
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
	Vector2 v1_1{ 1.5, 0.5, 1 };
	Vector2 v1_2{ -0.5, 0.85, 1 };
	Vector2 v2_1 = v1_1 * Vector2::mirror(Line2({ -1.0, 0.0, 0 }, {
		{ 1.0, 1.0, 0 },
	}));
	Vector2 v2_2 = v1_2 * Vector2::mirror(Line2({ -1.0, 0.0, 0 }, {
		{ 1.0, 1.0, 0 },
	}));

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector orthogonal is successfully;
void test_2d_vector_orthogonal() {
	Vector2 v1_1{ 1.5, 0.5, 1 };
	Vector2 v1_2{ -0.5, 0.85, 1 };
	Vector2 v2_1 = v1_1 * Vector2::orthogonal(Line2({ -1.0, 0.0, 1 }, {
		{ 0.0, 1.0, 0 },
	}));
	Vector2 v2_2 = v1_2 * Vector2::orthogonal(Line2({ -1.0, 0.0, 1 }, {
		{ 0.0, 1.0, 0 },
	}));

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		ege_line(X(v1_1[0]), Y(v1_1[1]), X(v1_2[0]), Y(v1_2[1]));
		ege_line(X(v2_1[0]), Y(v2_1[1]), X(v2_2[0]), Y(v2_2[1]));
	}
}

// 2D-vector curved line is successfully;
void test_2d_gridding_line() {
	Curved2 c1({
		Curved2::grid_data(-PI * 2, .1, PI * 2)
	}, {
		[](Curved2::Targ a) { return a[0]; },
		[](Curved2::Targ a) { return sin(a[0]); }
	});

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		Curved2::Tval v;
		for (auto c : c1) {
			if (v != Curved2::Tval())
				ege_line(X(c[0]), Y(c[1]), X(v[0]), Y(v[1]));
			v = c;
		}
	}
}

void test_3d_scene() {
	ZGL::scene_grid scene;

	ZGL::camera cm({ 4, 4, 4, 0 }, { -1, -1, -1, 0 }, { -1, -1, 0, 0 });
	ZGL::surface sf({
		ZGL::surface::grid_data(-PI * 2, .1, PI * 2),
		ZGL::surface::grid_data(-PI * 2, .1, PI * 2),
		ZGL::surface::grid_data(-5, .1, 5),
	}, {
		[](ZGL::surface::Targ a) { return sin(a[0]); },
		[](ZGL::surface::Targ a) { return cos(a[1]); },
		[](ZGL::surface::Targ a) { return a[2]; },
	});
	char cid = scene.add_camera(&cm);
	scene.add_solid(&sf);

	scene.modeling_trans();
	// scene.viewing_trans(cid);
	// scene.projection_trans(0);
	scene.viewport_trans(100, 800, 600);

	std::vector< ZGL::surface::segment > rs;
	for (auto grids : scene.material_grids) {
		grids->each(rs);
	}
	for (; is_run(); delay_fps(60), cleardevice()) {
		auto i = 0;
		for (auto r : rs) {
			auto x1 = (r.frist[0]);
			auto y1 = (r.frist[1]);
			auto x2 = (r.last[0]);
			auto y2 = (r.last[1]);
			ege_line(x1, y1, x2, y2);
			i++;
		}
	}
}