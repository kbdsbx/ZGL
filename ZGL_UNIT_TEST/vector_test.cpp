#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/linear/vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(vector_test)
	{
	public:

		TEST_METHOD(vector_init)
		{
			// The defalut constructor
			// Ĭ�Ϲ��캯��
			ZGL::vector< 3, double > v1;

			double ds[3] = { 1, 2, 3 };
			// The constructor that using item array;
			// ʹ������Ĺ��캯��
			ZGL::vector< 3, double > v2(ds);

			// The constructor that using initializer_list in cpp-11;
			// ʹ��C++11�ĳ�ʼ���б�initializer_list�Ĺ��캯��
			ZGL::vector< 3, double > v3(v2);

			// The constructor that using copy constructor
			// ʹ�ÿ������캯��
			ZGL::vector< 3, double > v4{ 1, 2, 3 };

			// The constructor that using move constructor
			// ʹ��ת�ƹ��캯��
			ZGL::vector< 3, double > v5(v1 + v2);
		}

		TEST_METHOD(vector_operator)
		{
			// Dot metrix
			// ���
			Assert::AreEqual(ZGL::vector< 4, int > { 1, 2, 3, 4 } PRO_DOT ZGL::vector< 4, int > { 2, 3, 4, 5 }, 40);

			// Parallel projection
			// ƽ��ͶӰ
			Assert::IsTrue((ZGL::vector< 4, double > { 3, 4, 1, 0 } PRO_PARALLEL ZGL::vector< 4, double > { 2, 3, -4, 2 }) == ZGL::vector< 4, double > { 28.0 / 33, 42.0 / 33, -56.0 / 33, 28.0 / 33 });

			// Upright projection
			// ��ֱͶӰ
			Assert::IsTrue((ZGL::vector< 4, double > { 3, 4, 1, 0 } PRO_UPRIGHT ZGL::vector< 4, double > { 2, 3, -4, 2 }) == ZGL::vector< 4, double > { 3 - 28.0 / 33, 4 - 42.0 / 33, 1 - -56.0 / 33, 0 - 28.0 / 33 });
		}

		TEST_METHOD(vector_module)
		{
			// Vector module
			// ��������
			Assert::AreEqual(ZGL::vector< 4, double> { 4, 1, -5, 0 }.module(), sqrt(42.0));
		}

		TEST_METHOD(vector_determinant)
		{
			// determinant value for vector array
			// ����������ʽֵ
			Assert::AreEqual(ZGL::vector< 3, int >::determinant({
				{ 3, -5, 23 },
				{ 5, 0, 12 },
				{ -7, 8, 100 },
			}), 3552);
		}

		TEST_METHOD(vector_angle)
		{
			// The angle between the both of vector
			// �������ļн�
			Assert::AreEqual((ZGL::vector< 4, double > { 5, -4, 3, 1 }).angle(ZGL::vector< 4, double > { 1, 5, 8, -10 }), acos(-1.0 / (sqrt(51.0) * sqrt(190.0))));
		}

		TEST_METHOD(vector_cross)
		{
			// Vector cross product
			// ������
			Assert::IsTrue(ZGL::vector< 3, double >::cross({
				{ 3, -5, 6 },
				{ 1, 8, -3 },
			}) == ZGL::vector< 3, double > { -33, 15, 29 });
		}

		TEST_METHOD(vector_cofactor)
		{
			// Vector cofactor
			// ��������ʽ
			Assert::IsTrue(ZGL::vector< 4, double >::cofactor(ZGL::vector< 4, double > { 3, 4, 5, 6 }, 2) == ZGL::vector< 3, double > { 3, 4, 6 });
		}
	};
}