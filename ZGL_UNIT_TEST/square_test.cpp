#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/linear/square.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(square_test)
	{
	public:

		TEST_METHOD(square_init)
		{
			// The defalut constructor
			// 默认构造函数
			ZGL::square< 3, double > m1;

			double ds[3][3] = {
				{ 1, 2, 3 },
				{ 2, 3, 4 },
				{ 4, 5, 6 },
			};
			// The constructor that using item array;
			// 使用数组的构造函数
			ZGL::square< 3, double > m2(ds);

			// The constructor that using initializer_list in cpp-11;
			// 使用C++11的初始化列表initializer_list的构造函数
			ZGL::square< 3, double > m3{
				{ 1, 2, 3 },
				{ 2, 3, 4 },
				{ 4, 5, 6 },
			};

			// The constructor that using copy constructor
			// 使用拷贝构造函数
			ZGL::square< 3, double > m4(m3);

			// The constructor that using move constructor
			// 使用转移构造函数
			ZGL::square< 3, double > m5(m4 + m3);
		}

		TEST_METHOD(square_operator)
		{
			// Square matrix
			// 矩阵平方
			Assert::IsTrue((ZGL::square< 3, int > {
				{ 3, -5, 3 },
				{ 6, 3, 0 },
				{ -3, 2, 12 },
			} ^ 2) == ZGL::square< 3, int >{
				{ -30, -24, 45 },
				{ 36, -21, 18 },
				{ -33, 45, 135 },
			});

			// Inverse matrix
			// 矩阵求逆
			Assert::IsTrue((ZGL::square< 3, double > {
				{ 3, -5, 3 },
				{ 6, 3, 0 },
				{ -3, 2, 12 },
			} ^ -1) == ZGL::square< 3, double >::inverse(ZGL::square< 3, double > {
				{ 3, -5, 3 },
				{ 6, 3, 0 },
				{ -3, 2, 12 },
			}));
		}

		TEST_METHOD(square_transpose)
		{
			// Matrix transpose
			// 矩阵转置
			Assert::IsTrue(ZGL::square< 3, double >::transpose(ZGL::square< 3, double > {
				{ 3, -5, 3 },
				{ 6, 3, 0 },
				{ -3, 2, 12 },
			}) == ZGL::square< 3, double >{
				{ 3, 6, -3 },
				{ -5, 3, 2 },
				{ 3, 0, 12 },
			});

			// Matrix transpose
			// 矩阵转置
			Assert::IsTrue(ZGL::square< 3, double > {
				{ 3, -5, 3 },
				{ 6, 3, 0 },
				{ -3, 2, 12 },
			}.transpose() == ZGL::square< 3, double > {
				{ 3, 6, -3 },
				{ -5, 3, 2 },
				{ 3, 0, 12 },
			});
		}

		TEST_METHOD(square_determinant)
		{
			// The value of matrix determinant
			// 矩阵行列式值
			Assert::AreEqual(ZGL::square< 3, int > {
				{ 3, -5, 23 },
				{ 5, 0, 12 },
				{ -7, 8, 100 },
			}.determinant(), 3552);
		}

		TEST_METHOD(square_cofactor)
		{
			// Matrix cofactor
			// 矩阵余子式
			Assert::IsTrue(ZGL::square< 5, int >::cofactor (ZGL::square< 5, int > {
				{ 2, 4, 6, 8, 10 },
				{ 1, 3, 5, 7, 9 },
				{ -5, 83, 1, 0, 64 },
				{ 911, 3, 55, 1, 30 },
				{ 0, 0, 0, 0, 1 },
			}, 2, 3) == ZGL::square< 4, int > {
				{ 2, 4, 8, 10 },
				{ -5, 83, 0, 64 },
				{ 911, 3, 1, 30 },
				{ 0, 0, 0, 1 },
			});
		}

		TEST_METHOD(square_inverse)
		{
			// Inverse matrix
			// 逆矩阵
			Assert::IsTrue(ZGL::square< 3, double >::inverse(ZGL::square< 3, double > {
				{ 1, 0, 1 },
				{ 2, 1, 0 },
				{ -3, 2, -5 },
			}) == ZGL::square< 3, double > {
				{ -2.5, 1, -0.5 },
				{ 5, -1, 1 },
				{ 3.5, -1, 0.5 },
			});
		}
	};
}
