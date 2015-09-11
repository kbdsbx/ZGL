#define SHOW_CONSOLE

#include "include/graphics.h"
#include "src/test.h"
// #include "../ZGL/zgl.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() {
	setinitmode(INIT_ANIMATION);
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
	ege_enable_aa(true);
	setbkcolor(WHITE);
	setcolor(EGERGBA(0, 0, 0, 255));

	// loops;
	test_2();

	closegraph();
}