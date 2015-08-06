#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/linear/matrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{		
	TEST_CLASS(matrix_test)
	{
	public:
		
		TEST_METHOD(matrix_init)
		{
			// The defalut constructor
			// 默认构造函数
			ZGL::matrix< 3, 3, double > m1;

			double ds[3][3] = {
				{ 1, 2, 3 },
				{ 2, 3, 4 },
				{ 4, 5, 6 },
			};
			// The constructor that using item array;
			// 使用数组的构造函数
			ZGL::matrix< 3, 3, double > m2(ds);

			// The constructor that using initializer_list in cpp-11;
			// 使用C++11的初始化列表initializer_list的构造函数
			ZGL::matrix< 3, 3, double > m3 {
				{ 1, 2, 3 },
				{ 2, 3, 4 },
				{ 4, 5, 6 },
			};

			// The constructor that using copy constructor
			// 使用拷贝构造函数
			ZGL::matrix< 3, 3, double > m4(m3);

			// constructor that using 4 submatrix to initialize
			// 使用四个子矩阵构造矩阵
			ZGL::matrix< 3, 3, double > m5(
				ZGL::matrix< 2, 2, double > { { 1, 2 }, { 2, 3 } },
				ZGL::matrix< 2, 1, double > { { 3 }, { 4 } },
				ZGL::matrix< 1, 2, double > { { 4, 5 } },
				ZGL::matrix< 1, 1, double > { { 6 } }
			);

			Assert::IsTrue(m1 == ZGL::matrix< 3, 3, double >());
			Assert::IsTrue(m2 == m3);
			Assert::IsTrue(m3 == m4);
			Assert::IsTrue(m4 == m5);
		}

		TEST_METHOD(matrix_operator)
		{
			ZGL::matrix< 3, 3, double > m1{
				{ 1, 2, 3 },
				{ 2, 3, 4 },
				{ 4, 5, 6 },
			};
			// operator = and operator []
			ZGL::matrix< 3, 3, double > m2 = m1;
			Assert::AreEqual(m1[0][0], m2[0][0]);
			Assert::AreEqual(m1[0][1], m2[0][1]);
			Assert::AreEqual(m1[0][2], m2[0][2]);
			Assert::AreEqual(m1[1][0], m2[1][0]);
			Assert::AreEqual(m1[1][1], m2[1][1]);
			Assert::AreEqual(m1[1][2], m2[1][2]);
			Assert::AreEqual(m1[2][0], m2[2][0]);
			Assert::AreEqual(m1[2][1], m2[2][1]);
			Assert::AreEqual(m1[2][2], m2[2][2]);

			// operator = Rvalue
			// 右值赋值函数
			ZGL::matrix< 3, 3, double > m3 = m1 + m2;
			Assert::IsTrue(m3 == ZGL::matrix< 3, 3, double > {
				{ 2, 4, 6 },
				{ 4, 6, 8 },
				{ 8, 10, 12 },
			});

			// operator ==
			Assert::IsTrue(m1 == m2);

			// operator within floating types
			// 
			Assert::IsTrue(ZGL::matrix< 1, 1, double > { { 1.0 } } == ZGL::matrix< 1, 1, double > { { 0.9999999999999999 } });

			// operator !=
			Assert::IsFalse(m1 != m2);

			// operator +
			Assert::IsTrue(m1 + m2 == ZGL::matrix< 3, 3, double > {
				{ 2, 4, 6 },
				{ 4, 6, 8 },
				{ 8, 10, 12 },
			});

			// operator -
			Assert::IsTrue(m1 - m2 == ZGL::matrix< 3, 3, double >());

			// operator * number
			// 数乘
			Assert::IsTrue(m1 * 3 == ZGL::matrix< 3, 3, double> {
				{ 3, 6, 9 },
				{ 6, 9, 12 },
				{ 12, 15, 18 },
			});

			// operator * matrices
			// 矩阵乘法
			Assert::IsTrue(m1 * m2 == ZGL::matrix< 3, 3, double > {
				{ 17, 23, 29 },
				{ 24, 33, 42},
				{ 38, 53, 68 },
			});

			// operator * between both of alien matrix
			// 两个不同宽高矩阵相乘
			Assert::IsTrue(ZGL::matrix< 1, 4, double > {
				{ 3, 4, 21, 9 }
			} * ZGL::matrix< 4, 2, double > {
				{ 5, 34 },
				{ 0, 4 },
				{ -55, 54 },
				{ 102, -78 },
			} == ZGL::matrix< 1, 2, double > {
				{ -222, 550 },
			});

			// operator / number
			// 数除
			Assert::IsTrue(ZGL::matrix< 1, 4, double > {
				{ 2, 4, 5, -6 },
			} / 2 == ZGL::matrix< 1, 4, double > {
				{ 1, 2, 2.5, -3 },
			});

			// To using Rvalue copy constructor to constructed new object
			// 使用右值拷贝构造函数构造新对象
			ZGL::matrix< 3, 3, double > m4(m1 + m2);
			Assert::IsTrue(m3 == ZGL::matrix< 3, 3, double > {
				{ 2, 4, 6 },
				{ 4, 6, 8 },
				{ 8, 10, 12 },
			});
		}

		TEST_METHOD(matrix_trans)
		{
			// Matrix transpose
			// 矩阵转置
			Assert::IsTrue(ZGL::matrix< 1, 4, double > ::transpose(ZGL::matrix< 1, 4, double > {
				{ 5, 24, -6, 0 },
			}) == ZGL::matrix< 4, 1, double > {
				{ 5 },
				{ 24 },
				{ -6 },
				{ 0 },
			});
		}

		TEST_METHOD(matrix_cofactor)
		{
			// Matrix Cofactor
			// 矩阵余子式
			Assert::IsTrue(ZGL::matrix< 3, 4, double >::cofactor< 2, 2 >({ {
				{ 5, -2, .87, 9.0 },
				{ 23, -0.05, 172, 34 },
				{ 0, 34, 2, -0.334 },
			} }) == ZGL::matrix< 2, 3, double >{ {
				{ 5, .87, 9.0 },
				{ 0, 2, -0.334 },
			} });

			// Matrix cofactor with only remove one row is no.2
			// 矩阵余子式仅删除第二行
			Assert::IsTrue(ZGL::matrix< 3, 4, double >::cofactor< 2, 0 >({ {
				{ 5, -2, .87, 9.0 },
				{ 23, -0.05, 172, 34 },
				{ 0, 34, 2, -0.334 },
			} }) == ZGL::matrix< 2, 4, double >{ {
				{ 5, -2, .87, 9.0 },
				{ 0, 34, 2, -0.334 },
			} });

			// Matrix cofactor with only remove one column is no.2
			// 矩阵余子式仅删除第二列
			Assert::IsTrue(ZGL::matrix< 3, 4, double >::cofactor< 0, 2 >({ {
				{ 5, -2, .87, 9.0 },
				{ 23, -0.05, 172, 34 },
				{ 0, 34, 2, -0.334 },
			} }) == ZGL::matrix< 3, 3, double >{ {
				{ 5, .87, 9.0 },
				{ 23, 172, 34 },
				{ 0, 2, -0.334 },
			} });

			// Matrix cofactor with none remove anything
			// 矩阵余子式不删除任何行列
			Assert::IsTrue(ZGL::matrix< 3, 4, double >::cofactor< 0, 0 >({ {
				{ 5, -2, .87, 9.0 },
				{ 23, -0.05, 172, 34 },
				{ 0, 34, 2, -0.334 },
			} }) == ZGL::matrix< 3, 4, double >{ {
				{ 5, -2, .87, 9.0 },
				{ 23, -0.05, 172, 34 },
				{ 0, 34, 2, -0.334 },
			} });
		}
	};
}