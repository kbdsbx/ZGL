#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/geometry/affine_vector.h"
#include "../ZGL/geometry/gridding-implicitly.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(gridding_implicitly_test)
	{
	public:

		TEST_METHOD(gridding_implicitly_make_range)
		{
			ZGL::gridding_implicitly< 3, 3, double > g(1, 1, 2, [](ZGL::gridding_implicitly< 3, 3, double >::Targ arg) {
				return arg[0] * arg[0] + arg[1] * arg[1] + arg[2] * arg[2] - 1;
			});

			ZGL::gridding< 3, 3, double > gd = g.make_range();
			auto it = gd.begin();
			Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 1, 1, 1, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 2, 1, 1, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 1, 2, 1, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 2, 2, 1, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 1, 1, 2, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 2, 1, 2, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 1, 2, 2, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 2, 2, 2, 1 });
		}

		TEST_METHOD(gridding_implicitly__dis)
		{
			ZGL::gridding_implicitly< 3, 3, double > g(-2, .5, 2, [](ZGL::gridding_implicitly< 3, 3, double >::Targ arg) {
				return arg[0] * arg[0] + arg[1] * arg[1] + arg[2] * arg[2] - 2;
			});

			Assert::AreEqual(g._root.size(), 400U);
		}

		/*
		TEST_METHOD(gridding_implicitly__loop) {
			ZGL::gridding_implicitly< 4, 3, double > g({
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-2, 2),
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-2, 2),
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-2, 2),
			}, [](ZGL::gridding_implicitly< 4, 3, double >::Targ arg) {
				return arg[0] * arg[0] + arg[1] * arg[1] + arg[2] * arg[2] - 1;
			});
			g._loop(g._range, 5);
			Assert::AreEqual(g._root.size(), 15U);
		}
		*/
	};
}