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

		TEST_METHOD(gridding_implicitly_grid_range)
		{
			ZGL::gridding_implicitly< 4, 2, double >::grid_range r(1, 5);
			Assert::AreEqual(r.mini(), 1.0);
			Assert::AreEqual(r.maxi(), 5.0);

			ZGL::gridding_implicitly< 4, 2, double >::grid_range r1 = r;
			Assert::AreEqual(r1.mini(), 1.0);
			Assert::AreEqual(r1.maxi(), 5.0);
		}
		TEST_METHOD(gridding_implicitly_make_range)
		{
			ZGL::gridding_implicitly< 4, 3, double > g({
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-1, 2),
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-3, 4),
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-5, 6),
			}, [](ZGL::gridding_implicitly< 4, 3, double >::Targ arg) {
				return arg[0] * arg[0] + arg[1] * arg[1] + arg[2] * arg[2] - 1;
			});

			ZGL::gridding< 4, 3, double > gd = g.make_range(g._range);
			auto it = gd.begin();
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double >{ -1, -3, -5, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double >{ 2, -3, -5, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double >{ -1, 4, -5, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double >{ 2, 4, -5, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double >{ -1, -3, 6, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double >{ 2, -3, 6, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double >{ -1, 4, 6, 1 });
			++it;
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double >{ 2, 4, 6, 1 });
		}

		TEST_METHOD(gridding_implicitly__dis)
		{
			ZGL::gridding_implicitly< 4, 3, double > g({
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-2, 2),
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-2, 2),
				ZGL::gridding_implicitly< 4, 3, double >::grid_range(-2, 2),
			}, [](ZGL::gridding_implicitly< 4, 3, double >::Targ arg) {
				return arg[0] * arg[0] + arg[1] * arg[1] + arg[2] * arg[2] - 1;
			});

			Assert::AreEqual(g._root.size(), 1878U);
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