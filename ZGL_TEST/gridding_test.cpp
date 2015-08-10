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
			ZGL::gridding< 4, 2, double >::grid_data g1(0, 10, 2);
			Assert::AreEqual(g1[0], 0.0);
			Assert::AreEqual(g1[1], 2.0);
			Assert::AreEqual(g1[2], 4.0);
			Assert::AreEqual(g1[3], 6.0);
			Assert::AreEqual(g1[4], 8.0);
			Assert::AreEqual(g1[5], 10.0);

			// Init grid data use array
			// 使用数组初始化网格数据
			double _data[6] = { 0.0, 2.0, 4.0, 6.0, 8.0, 10.0 };
			ZGL::gridding< 4, 2, double >::grid_data g2(_data, 6);
			Assert::AreEqual(g2[0], 0.0);
			Assert::AreEqual(g2[1], 2.0);
			Assert::AreEqual(g2[2], 4.0);
			Assert::AreEqual(g2[3], 6.0);
			Assert::AreEqual(g2[4], 8.0);
			Assert::AreEqual(g2[5], 10.0);

			// Generate discrete date use function
			// 使用方法生成离散数据
			ZGL::gridding< 4, 2, double >::grid_data g3([](const ZGL::affine_vector< 4, double >& v) { return v[0] * v[1]; });
			Assert::AreEqual(g3[ZGL::affine_vector< 4, double > { 2.0, 3.0, 0, 0 }], 6.0);

			// Test about is_data or is_function
			// 测试数据类型相关
			Assert::AreEqual(g1.is_data(), true);
			Assert::AreNotEqual(g2.is_function(), true);
			Assert::AreEqual(g3.is_function(), true);
		}

		TEST_METHOD(gridding_grid_node)
		{
			// Init grid node
			// 初始化网格节点
			ZGL::gridding< 4, 2, double >::grid_node n1(ZGL::affine_vector< 4, double >{ 1.0, 2.0, 3.0, 1 });

			// Getting node
			// 获取节点
			Assert::IsTrue(n1.get_dot() == ZGL::affine_vector< 4, double >{ 1.0, 2.0, 3.0, 1 });

			// Setting node
			// 设置节点
			n1.set_dot(ZGL::affine_vector< 4, double >{ 2.0, 4.0, 8.0, 1 });
			Assert::IsTrue(n1.get_dot() == ZGL::affine_vector< 4, double >{ 2.0, 4.0, 8.0, 1 });

			// Initialied of nodes pointer
			// 初始化后的节点指针
			Assert::IsTrue(n1[3] == nullptr);
			n1[3] = new ZGL::gridding< 4, 2, double >::grid_node(ZGL::affine_vector< 4, double >{});

			// If count of node pointer is real
			// 节点指针个数是否正确
			Assert::ExpectException< std::out_of_range >([&n1]() { ZGL::gridding< 4, 2, double >::grid_node* t = n1[4]; });
		}

		TEST_METHOD(gridding_init)
		{
			// gridding init
			// 初始化网格
			ZGL::gridding< 4, 2, double > g1({
				ZGL::gridding< 4, 2, double >::grid_data(0, 10, 1),
				ZGL::gridding< 4, 2, double >::grid_data([](ZGL::affine_vector< 4, double > v) { return sin(v[0]); }),
				ZGL::gridding< 4, 2, double >::grid_data([](ZGL::affine_vector< 4, double > v) { return cos(v[0]); }),
			});
		}

		TEST_METHOD(gridding_discrete)
		{
			ZGL::gridding< 4, 2, double > g1({
				ZGL::gridding< 4, 2, double >::grid_data(0, 10, 1),
				ZGL::gridding< 4, 2, double >::grid_data([](ZGL::affine_vector< 4, double > v) { return sin(v[0]); }),
				ZGL::gridding< 4, 2, double >::grid_data([](ZGL::affine_vector< 4, double > v) { return cos(v[0]); }),
			});

			// Discrete grid
			// 离散网格
			g1.discrete();
		}
	};
}
