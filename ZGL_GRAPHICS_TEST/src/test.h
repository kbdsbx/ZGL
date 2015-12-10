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

// 2D-vector perspective is successfully;
void test_2d_vector_perspective() {
	Vector2 v1_1{ 1.5, 0.5, 1 };
	Vector2 v1_2{ -0.5, 0.85, 1 };
	Vector2 v2_1 = v1_1 * Vector2::perspective(Line2({ -1.0, 0.0, 1 }, {
		{ 0.0, 1.0, 0 },
	}), { -1.5, 0, 0, 1 });
	Vector2 v2_2 = v1_2 * Vector2::perspective(Line2({ -1.0, 0.0, 1 }, {
		{ 0.0, 1.0, 0 },
	}), { -1.5, 0, 0, 1 });

	v2_1.normalize();
	v2_2.normalize();

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
		[](Curved2::Targ a, ZGL::z_size_t idx) { return a[0]; },
		[](Curved2::Targ a, ZGL::z_size_t idx) { return sin(a[0]); }
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

void test_2d_gridding_implicitly() {
	ImpCurved2 c1({
		ImpCurved2::grid_range(-2, 2),
		ImpCurved2::grid_range(-2, 2),
	}, [](ImpCurved2::Targ a) { return pow(a[0] * a[0] + a[1] * a[1] - 1, 3) - a[0] * a[0] * pow(a[1], 3); });
	/*
	ImpCurved2 c1({
		ImpCurved2::grid_range(-2, 2),
		ImpCurved2::grid_range(-2, 2),
	}, [](ImpCurved2::Targ a) { return pow(a[0] * a[0] + a[1] * 2 - 1, 3) - a[0] * a[0] * pow(a[1], 3) - 1; });
	*/

	c1.set_step(.01);
	c1._dis();

	for (; is_run(); delay_fps(60), cleardevice()) {
		axis();
		for (auto c : c1._root) {
			putpixel(X(c[0]), X(c[1] * -1), EGERGBA(0, 0, 0, 1));
		}
	}
}

void test_3d_scene() {
	ZGL::scene_grid scene;

	ZGL::camera cm({ 0, 6, PI, 1 }, { 0, -1, 0, 0 }, { 0, 0, 1, 0 });
	ZGL::surface sf({
		ZGL::surface::grid_data(0, PI / 15, PI),
		ZGL::surface::grid_data(0, PI / 15, PI * 10),
	}, {
		[](ZGL::surface::Targ a, ZGL::z_size_t idx) { return a[0] * sin(a[1]); },
		[](ZGL::surface::Targ a, ZGL::z_size_t idx) { return a[0] * cos(a[1]); },
		[](ZGL::surface::Targ a, ZGL::z_size_t idx) { return a[1] / 5; },
	});
	char cid = scene.add_camera(&cm);
	scene.add_solid(sf);

	ZGL::mouse _mouse;
	ZGL::mouse::mouse_status _old_msg;
	_mouse.on(ZGL::ZGL_MOUSE_TYPE::LEFT_CLICK | ZGL::ZGL_MOUSE_TYPE::MOVE, [&scene, &_old_msg, &cm](ZGL::mouse::mouse_status msg) {
		if (_old_msg.x != 0 && _old_msg.y != 0) {
			cm.traverse((msg.x - _old_msg.x) / -100.0);
			cm.lift((msg.y - _old_msg.y) / 100.0);
		}
		_old_msg = msg;
	});

	_mouse.on(ZGL::ZGL_MOUSE_TYPE::WHEEL, [&](ZGL::mouse::mouse_status msg) {
		cm.retreat(msg.wheel / 100.0);
	});

	for (; is_run(); delay_fps(60), cleardevice()) {
		ege::mouse_msg emsg = { 0 };
		while (mousemsg()) {
			emsg = getmouse();

			_mouse.monitoring([&](ZGL::mouse::mouse_status _msg) {
				_msg.x = emsg.x;
				_msg.y = emsg.y;
				_msg.wheel = emsg.wheel;

				if (emsg.is_down()) {
					if (emsg.is_left())
						_msg.set_left();
					if (emsg.is_right())
						_msg.set_right();
					if (emsg.is_mid())
						_msg.set_mid();
				}

				if (emsg.is_up()) {
					if (emsg.is_left())
						_msg.reset_left();
					if (emsg.is_right())
						_msg.reset_right();
					if (emsg.is_mid())
						_msg.reset_mid();
				}

				if (emsg.is_move()) {
					_msg.set_move();
				} else {
					_msg.reset_move();
				}

				if (emsg.is_wheel()) {
					_msg.set_wheel();
				} else {
					_msg.reset_wheel();
				}

				{
					char str[50];
					sprintf(str, "Flag: %d, %d, %d, %d", _msg.flags, emsg.flags, emsg.x, emsg.y);
					outtextxy(0, 30, str);
				}

				return STD_MOVE(_msg);
			});
		}
		_old_msg = ZGL::mouse::mouse_status();

		scene.modeling_trans();
		scene.viewing_trans(cid);
		scene.projection_trans(2);
		scene.viewport_trans(100, 800, 600);

		std::vector< ZGL::surface::segment > rs;
		for (decltype(auto) grids : scene.material_grids) {
			grids.each(rs);
		}

		auto i = 0;
		for (auto r : rs) {
			auto x1 = (r.first[0]);
			auto y1 = (r.first[1]);
			auto x2 = (r.last[0]);
			auto y2 = (r.last[1]);
			ege_line(x1, y1, x2, y2);
			i++;
		}

		{// 画帧率文字
			char str[20];
			sprintf(str, "fps %.02f", getfps()); //调用getfps取得当前帧率
			outtextxy(0, 0, str);
		}
	}
}