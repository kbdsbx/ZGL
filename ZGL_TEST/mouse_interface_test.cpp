#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/io/mouse.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(mouse_test)
	{
	public:

		TEST_METHOD(mouse_on)
		{
			ZGL::mouse mouse;
			Assert::AreEqual(mouse.on(ZGL::ZGL_MOUSE_TYPE::LEFT_CLICK | ZGL::ZGL_MOUSE_TYPE::MOVE, [](ZGL::mouse::mouse_status st) {
				; // nothing
			}), 1U);
		}

		TEST_METHOD(mouse_off)
		{
			ZGL::mouse mouse;
			size_t id = mouse.on(ZGL::ZGL_MOUSE_TYPE::LEFT_CLICK | ZGL::ZGL_MOUSE_TYPE::RIGHT_CLICK, [](ZGL::mouse::mouse_status st) {
				; // nothing
			});
			Assert::AreEqual(mouse.off(ZGL::ZGL_MOUSE_TYPE::LEFT_CLICK | ZGL::ZGL_MOUSE_TYPE::RIGHT_CLICK, 1), 1U);

			mouse.on(ZGL::ZGL_MOUSE_TYPE::LEFT_CLICK | ZGL::ZGL_MOUSE_TYPE::RIGHT_CLICK, [](ZGL::mouse::mouse_status st) {
				; // nothing
			});
			mouse.on(ZGL::ZGL_MOUSE_TYPE::LEFT_CLICK | ZGL::ZGL_MOUSE_TYPE::RIGHT_CLICK, [](ZGL::mouse::mouse_status st) {
				; // nothing
			});

			Assert::AreEqual(mouse.off(ZGL::ZGL_MOUSE_TYPE::LEFT_CLICK | ZGL::ZGL_MOUSE_TYPE::RIGHT_CLICK), 2U);
		}
	};
}
