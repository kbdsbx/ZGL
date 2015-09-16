#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/geometry/affine_vector.h"
#include "../ZGL/geometry/graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(affine_vector_test)
	{
	public:

		TEST_METHOD(affine_vector_init)
		{
			// The defalut constructor
			// 默认构造函数
			ZGL::affine_vector< 3, double > v1;

			double ds[3] = { 1, 2, 3 };
			// The constructor that using item array;
			// 使用数组的构造函数
			ZGL::affine_vector< 3, double > v2(ds);

			// The constructor that using initializer_list in cpp-11;
			// 使用C++11的初始化列表initializer_list的构造函数
			ZGL::affine_vector< 3, double > v3(v2);

			// The constructor that using copy constructor
			// 使用拷贝构造函数
			ZGL::affine_vector< 3, double > v4{ 1, 2, 3 };

			// The constructor that using move constructor
			// 使用转移构造函数
			ZGL::affine_vector< 3, double > v5(v1 + v2);
		}

		TEST_METHOD(affine_vector_operator)
		{
			// Dot metrix
			// 点积
			Assert::AreEqual(ZGL::affine_vector< 4, int > { 1, 2, 3, 4 } PRO_DOT ZGL::affine_vector< 4, int > { 2, 3, 4, 5 }, 40);

			// Parallel projection
			// 平行投影
			Assert::IsTrue((ZGL::affine_vector< 4, double > { 3, 4, 1, 0 } PRO_PARALLEL ZGL::affine_vector< 4, double > { 2, 3, -4, 2 }) == ZGL::affine_vector< 4, double > { 28.0 / 33, 42.0 / 33, -56.0 / 33, 28.0 / 33 });

			// Upright projection
			// 垂直投影
			Assert::IsTrue((ZGL::affine_vector< 4, double > { 3, 4, 1, 0 } PRO_UPRIGHT ZGL::affine_vector< 4, double > { 2, 3, -4, 2 }) == ZGL::affine_vector< 4, double > { 3 - 28.0 / 33, 4 - 42.0 / 33, 1 - -56.0 / 33, 0 - 28.0 / 33 });
		}

		TEST_METHOD(affine_vector_module)
		{
			// affine_vector module
			// 向量长度
			Assert::AreEqual(ZGL::affine_vector< 4, double> { 4, 1, -5, 0 }.module(), sqrt(42.0));
		}

		TEST_METHOD(affine_vector_determinant)
		{
			// determinant value for affine_vector array
			// 向量组行列式值
			Assert::AreEqual(ZGL::affine_vector< 3, int >::determinant({
				{ 3, -5, 23 },
				{ 5, 0, 12 },
				{ -7, 8, 100 },
			}), 3552);
		}

		TEST_METHOD(affine_vector_angle)
		{
			// The angle between the both of affine_vector
			// 与向量的夹角
			Assert::AreEqual((ZGL::affine_vector< 4, double > { 5, -4, 3, 1 }).angle(ZGL::affine_vector< 4, double > { 1, 5, 8, -10 }), acos(-1.0 / (sqrt(51.0) * sqrt(190.0))));
		}

		TEST_METHOD(affine_vector_cross)
		{
			// affine_vector cross product
			// 向量积
			Assert::IsTrue(ZGL::affine_vector< 4, double >::cross({
				{ 3, -5, 6, 0 },
				{ 1, 8, -3, 0 },
			}) == ZGL::affine_vector< 4, double > { -33, 15, 29, 0 });
		}

		TEST_METHOD(affine_vector_cofactor)
		{
			// affine_vector cofactor
			// 向量余子式
			Assert::IsTrue(ZGL::affine_vector< 4, double >::cofactor(ZGL::affine_vector< 4, double > { 3, 4, 5, 6 }, 2) == ZGL::affine_vector< 3, double > { 3, 4, 6 });
		}

		TEST_METHOD(affine_vector_affine)
		{
			// affine_vector to be affine vector
			// 仿射向量转化
			Assert::IsTrue(ZGL::affine_vector< 4, double >::affine(ZGL::vector< 3, double > { 3, -5.6, 12 }, 25) == ZGL::affine_vector< 4, double > { 3, -5.6, 12, 25 });
		}

		TEST_METHOD(affine_vector_normalize) {
			// affine_vector normalize
			// 向量归一
			Assert::IsTrue(ZGL::affine_vector< 4, double >::normalize(ZGL::affine_vector< 4, double > { -5.3, 6, 0.15, 0 }) == ZGL::affine_vector< 4, double > { -0.66191849113483718719208834179223, 0.74934168807717417417972265108554, 0.01873354220192935435449306627714 });
		}

		TEST_METHOD(affine_vector_translate)
		{
			// dot translate in R2
			// 二维点平移
			Assert::IsTrue(ZGL::affine_vector< 3, double >{ 3, 2, 1 } * ZGL::affine_vector< 3, double >::translate(ZGL::affine_vector< 3, double >{ 4, 3, 0 }) == ZGL::affine_vector< 3, double > { 7, 5, 1 });

			// vector translate in R2
			// 二维向量平移
			Assert::IsTrue(ZGL::affine_vector< 3, double >{ 3, 2, 0 } * ZGL::affine_vector< 3, double >::translate(ZGL::affine_vector< 3, double >{ 4, 3, 0 }) == ZGL::affine_vector< 3, double > { 3, 2, 0 });

			// vector translate in R3
			// 三维点平移
			Assert::IsTrue(ZGL::affine_vector< 4, double >{ 5.3, 8.9, 3.86, 1 } * ZGL::affine_vector< 4, double >::translate(ZGL::affine_vector< 4, double >{ 5.5, 1.1, -6.22, 0 }) == ZGL::affine_vector< 4, double > { 10.8, 10.0, -2.36, 1 });

			// vector translate in R3
			// 三维向量平移
			Assert::IsTrue(ZGL::affine_vector< 4, double >{ -5, 3.2, 6, 0 } * ZGL::affine_vector< 4, double >::translate(ZGL::affine_vector< 4, double >{ 1, 42, -5.2, 0 }) == ZGL::affine_vector< 4, double > { -5, 3.2, 6, 0 });
		}

		TEST_METHOD(affine_vector_rotate)
		{
		}

		TEST_METHOD(affine_vector_scale)
		{
			// the matrix is vector or dot scaling relative orighinal
			// 向量或点绕原点旋转矩阵
			Assert::IsTrue(ZGL::affine_vector< 3, double > { 1, 2, 0 } * ZGL::affine_vector< 3, double >::scale(2.0) == ZGL::affine_vector < 3, double > { 2, 4, 0 });

			// vector or dot scaling relative other dot
			// 向量或点绕任意点旋转矩阵
			Assert::IsTrue(
				ZGL::affine_vector< 3, double >::scale(1.5, ZGL::affine_vector < 3, double >{ 3.2, 5.1, 1 }) == ZGL::square< 3, double > {
					{ 1.5, 0, 0 },
					{ 0, 1.5, 0 },
					{ 1.6, 2.55, 1 },
			});

			// vector or dot scaling to directions relative other dot
			// 向量或点相对一点沿任意方向缩放
			Assert::IsTrue((ZGL::affine_vector< 4, double > { 1, 2, 3, 4 } * ZGL::affine_vector < 4, double >::scale(3.0, ZGL::graph< 4, 1, double >({ 0, 0, 0, 1 }, { { 0, 0, 1, 0 } }))) == ZGL::affine_vector< 4, double > { 1, 2, 9, 4 });

			Assert::IsTrue((ZGL::affine_vector< 4, double > { 1, 2, 3, 4 } *ZGL::affine_vector < 4, double >::scale(3.0, ZGL::graph< 4, 2, double >({ 0, 0, 0, 1 }, { { 0, 0, 1, 0 }, { 0, 1, 0, 0 } }))) == ZGL::affine_vector< 4, double > { 1, 6, 9, 4 });
		}

		TEST_METHOD(affine_vector_mirror)
		{
			// vector mirror
			// 向量镜像
			Assert::IsTrue(ZGL::affine_vector< 3, double >{ 3, 1, 0 } *ZGL::affine_vector< 3, double >::mirror(ZGL::graph< 3, 1, double >({ 0, 0, 1 }, { { 0, 1, 0 } })) == ZGL::affine_vector< 3, double >{ -3, 1, 0 });
		}
	};
}