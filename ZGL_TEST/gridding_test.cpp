#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/geometry/affine_vector.h"
#include "../ZGL/geometry/gridding.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(gridding_test)
	{
	public:

		TEST_METHOD(gridding_grid_data)
		{
			// Init grid data use interval and step
			// 使用范围与步长初始化网格数据
			ZGL::gridding< 3, 2, double >::grid_data g1(0, 2, 10);
			Assert::AreEqual(g1[0], 0.0);
			Assert::AreEqual(g1[1], 2.0);
			Assert::AreEqual(g1[2], 4.0);
			Assert::AreEqual(g1[3], 6.0);
			Assert::AreEqual(g1[4], 8.0);
			Assert::AreEqual(g1[5], 10.0);
			Assert::AreEqual(g1.len(), ZGL::z_size_t(6));

			// Init grid data use array
			// 使用数组初始化网格数据
			double _data[6] = { 0.0, 2.0, 4.0, 6.0, 8.0, 10.0 };
			ZGL::gridding< 3, 2, double >::grid_data g2(_data, 6);
			Assert::AreEqual(g2[0], 0.0);
			Assert::AreEqual(g2[1], 2.0);
			Assert::AreEqual(g2[2], 4.0);
			Assert::AreEqual(g2[3], 6.0);
			Assert::AreEqual(g2[4], 8.0);
			Assert::AreEqual(g2[5], 10.0);
			Assert::AreEqual(g2.len(), ZGL::z_size_t(6));
		}

		TEST_METHOD(gridding_init)
		{
			// Init gridding
			// 初始化网格
			ZGL::gridding< 3, 1, double > g1({
				ZGL::gridding< 3, 1, double >::grid_data(0, 2, 10)
			}, {
				[](ZGL::vector< 1, double > v, ZGL::z_size_t idx) { return sin(v[0]); },
				[](ZGL::vector< 1, double > v, ZGL::z_size_t idx) { return cos(v[0]); },
				[](ZGL::vector< 1, double > v, ZGL::z_size_t idx) { return 0.0; }
			});
		}

		TEST_METHOD(gridding_iterator)
		{
			ZGL::gridding< 3, 1, double > g1({
				ZGL::gridding< 3, 1, double >::grid_data(0, 2, 10)
			}, {
				[](ZGL::vector< 1, double > v, ZGL::z_size_t idx) { return v[0]; },
				[](ZGL::vector< 1, double > v, ZGL::z_size_t idx) { return v[0]; },
				[](ZGL::vector< 1, double > v, ZGL::z_size_t idx) { return 0.0; }
			});

			int i = 0;

			Assert::AreEqual(g1._data[0].len(), ZGL::z_size_t(6));

			for (auto it = g1.begin(); it != g1.end(); ++it) {
				// Debuggggggggggggging;
				switch (i) {
				case 0:
					Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 0, 0, 0, 1 });
					break;
				case 1:
					Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 2, 2, 0, 1 });
					break;
				case 2:
					Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 4, 4, 0, 1 });
					break;
				case 3:
					Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 6, 6, 0, 1 });
					break;
				case 4:
					Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 8, 8, 0, 1 });
					break;
				case 5:
					Assert::IsTrue(*it == ZGL::affine_vector< 3, double >{ 10, 10, 0, 1 });
					break;
				case 6:
					throw "";
					break;
				}
				i++;
			}

			// If iterator is working and count of gridding nodes is right.
			// 迭代器工作与否并且计数是否正确
			Assert::AreEqual(i, 6);

			double d[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
			ZGL::gridding< 3, 3, double > g2({
				ZGL::gridding< 3, 3, double >::grid_data(0, 2, 10), // 6
				ZGL::gridding< 3, 3, double >::grid_data(-5, 1, 5), // 11
				ZGL::gridding< 3, 3, double >::grid_data(d, 10),	// 10
			}, {
				[](ZGL::vector< 3, double > v, ZGL::z_size_t idx) { return v[0]; },
				[](ZGL::vector< 3, double > v, ZGL::z_size_t idx) { return v[1]; },
				[](ZGL::vector< 3, double > v, ZGL::z_size_t idx) { return v[2]; },
			});

			i = 0;

			for (auto it = g2.begin(); it != g2.end(); ++it)
				i++;

			Assert::AreEqual(i, 660);
		}
	};
}
