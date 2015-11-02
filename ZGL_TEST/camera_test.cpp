#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/scene/camera.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(camera_test)
	{
	public:

		TEST_METHOD(camera_init)
		{
			ZGL::camera cm({ 5, 0.3, 1, 1 }, { -3, 1, 0.5, 0 }, { 5.6, 3, 0, 0 });

			// Camera Position
			// 相机位置
			Assert::IsTrue(cm.position() == ZGL::affine_vector< 4, double >{ 5, 0.3, 1, 1 });
			// Camera forward vector
			// 相机向前向量
			Assert::IsTrue(cm.forward() == ZGL::affine_vector< 4, double >{ -0.93704257133163639314461722067636, 0.31234752377721213104820574022545, 0.15617376188860606552410287011273, 0 });
			// -1.5, 2.8, -14.6, 0
			// Camera rightward vector
			// 相机向右向量
			Assert::IsTrue(cm.rightward() == ZGL::affine_vector< 4, double > { -0.10039117221153819620824625906472, 0.18739685479487129958872635025414, -0.9771407428589717764269302548966, 0 });
			// 16.0, 44.55, 6.9
			// Camera upward vector
			// 相机向上向量
			Assert::IsTrue(cm.upward() == ZGL::affine_vector< 4, double > { 0.33447396319323332798390107972635, 0.93130094126615904760517456886306, 0.14424189662708187269305734063199, 0 });
		}

		TEST_METHOD(camera_move)
		{
			ZGL::camera cm({ 5, 0.3, 1, 1 }, { -3, 1, 0.5, 0 }, { 5.6, 3, 0, 0 });

			// Camera traverse
			// 相机横移
			cm.traverse(1.5);
			Assert::IsTrue(cm.position() == ZGL::affine_vector< 4, double >{ 4.8494132416826927057, 0.58109528219230694938, -0.46571111428845766464, 1 });

			// Camera retreat
			// 相机前进
			cm.retreat(1.5);
			Assert::IsTrue(cm.position() == ZGL::affine_vector< 4, double > { 3.443849384685238116, 1.049616567858125146, -0.23145047145554856635, 1 });

			// Camera lift
			// 相机上升
			cm.lift(1.5);
			Assert::IsTrue(cm.position() == ZGL::affine_vector< 4, double > { 3.945560329475088108, 2.4465679797573637174, -0.01508762651492575731, 1 });
		}

		TEST_METHOD(camera_rotate)
		{
			ZGL::camera cm({ 5, 0.3, 1, 1 }, { -3, 1, 0.5, 0 }, { 5.6, 3, 0, 0 });
		}
	};
}