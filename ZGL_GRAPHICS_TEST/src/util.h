#pragma once

#include <iostream>
#include "../../ZGL/zgl.h"

#define screen_width 800
#define screen_height 600
#define X(x) ( (x) * 1 + ( screen_width / 2 ) )
#define Y(y) ( (-(y)) * 1 + ( screen_height / 2 ) )


void log(ZGL::squ4& input) {
	std::cout << input[0][0] << " " << input[0][1] << " " << input[0][2] << " " << input[0][3] << "\n";
	std::cout << input[1][0] << " " << input[1][1] << " " << input[1][2] << " " << input[1][3] << "\n";
	std::cout << input[2][0] << " " << input[2][1] << " " << input[2][2] << " " << input[2][3] << "\n";
	std::cout << input[3][0] << " " << input[3][1] << " " << input[3][2] << " " << input[3][3] << "\n";
}

void log(ZGL::vec4& input) {
	std::cout << input[0] << " " << input[1] << " " << input[2] << " " << input[3] << "\n";
}

// 将ege鼠标触发型事件转换为响应型事件
void mouse_bind( ZGL::mouse& zmouse ) {
	ege::mouse_msg emsg = { 0 };

	while (mousemsg()) {
		emsg = getmouse();

		zmouse.monitoring([&](ZGL::mouse_status msg) {
			msg.x = emsg.x;
			msg.y = emsg.y;
			msg.wheel = emsg.wheel;

			if (emsg.is_down()) {
				if (emsg.is_left()) {
					msg.set_left();
				}
				if (emsg.is_right()) {
					msg.set_right();
				}
				if (emsg.is_mid()) {
					msg.set_mid();
				}
			}

			if (emsg.is_up()) {
				if (emsg.is_left()) {
					msg.reset_left();
				}
				if (emsg.is_right()) {
					msg.reset_right();
				}
				if (emsg.is_mid()) {
					msg.reset_mid();
				}
			}

			if (emsg.is_move()) {
				msg.set_move();
			}
			else {
				msg.reset_move();
			}

			if (emsg.is_wheel()) {
				msg.set_wheel();
			}
			else {
				msg.reset_wheel();
			}


#ifdef _DEBUG
			{
				char str[50];
				sprintf(str, "Flag: %d, %d, %d, %d", msg.flags, emsg.flags, emsg.x, emsg.y);
				outtextxy(0, 30, str);
			}
#endif


			return STD_MOVE(msg);
		});
	}
}
