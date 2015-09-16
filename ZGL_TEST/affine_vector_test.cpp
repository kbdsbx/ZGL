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
			// Ĭ�Ϲ��캯��
			ZGL::affine_vector< 3, double > v1;

			double ds[3] = { 1, 2, 3 };
			// The constructor that using item array;
			// ʹ������Ĺ��캯��
			ZGL::affine_vector< 3, double > v2(ds);

			// The constructor that using initializer_list in cpp-11;
			// ʹ��C++11�ĳ�ʼ���б�initializer_list�Ĺ��캯��
			ZGL::affine_vector< 3, double > v3(v2);

			// The constructor that using copy constructor
			// ʹ�ÿ������캯��
			ZGL::affine_vector< 3, double > v4{ 1, 2, 3 };

			// The constructor that using move constructor
			// ʹ��ת�ƹ��캯��
			ZGL::affine_vector< 3, double > v5(v1 + v2);
		}

		TEST_METHOD(affine_vector_operator)
		{
			// Dot metrix
			// ���
			Assert::AreEqual(ZGL::affine_vector< 4, int > { 1, 2, 3, 4 } PRO_DOT ZGL::affine_vector< 4, int > { 2, 3, 4, 5 }, 40);

			// Parallel projection
			// ƽ��ͶӰ
			Assert::IsTrue((ZGL::affine_vector< 4, double > { 3, 4, 1, 0 } PRO_PARALLEL ZGL::affine_vector< 4, double > { 2, 3, -4, 2 }) == ZGL::affine_vector< 4, double > { 28.0 / 33, 42.0 / 33, -56.0 / 33, 28.0 / 33 });

			// Upright projection
			// ��ֱͶӰ
			Assert::IsTrue((ZGL::affine_vector< 4, double > { 3, 4, 1, 0 } PRO_UPRIGHT ZGL::affine_vector< 4, double > { 2, 3, -4, 2 }) == ZGL::affine_vector< 4, double > { 3 - 28.0 / 33, 4 - 42.0 / 33, 1 - -56.0 / 33, 0 - 28.0 / 33 });
		}

		TEST_METHOD(affine_vector_module)
		{
			// affine_vector module
			// ��������
			Assert::AreEqual(ZGL::affine_vector< 4, double> { 4, 1, -5, 0 }.module(), sqrt(42.0));
		}

		TEST_METHOD(affine_vector_determinant)
		{
			// determinant value for affine_vector array
			// ����������ʽֵ
			Assert::AreEqual(ZGL::affine_vector< 3, int >::determinant({
				{ 3, -5, 23 },
				{ 5, 0, 12 },
				{ -7, 8, 100 },
			}), 3552);
		}

		TEST_METHOD(affine_vector_angle)
		{
			// The angle between the both of affine_vector
			// �������ļн�
			Assert::AreEqual((ZGL::affine_vector< 4, double > { 5, -4, 3, 1 }).angle(ZGL::affine_vector< 4, double > { 1, 5, 8, -10 }), acos(-1.0 / (sqrt(51.0) * sqrt(190.0))));
		}

		TEST_METHOD(affine_vector_cross)
		{
			// affine_vector cross product
			// ������
			Assert::IsTrue(ZGL::affine_vector< 4, double >::cross({
				{ 3, -5, 6, 0 },
				{ 1, 8, -3, 0 },
			}) == ZGL::affine_vector< 4, double > { -33, 15, 29, 0 });
		}

		TEST_METHOD(affine_vector_cofactor)
		{
			// affine_vector cofactor
			// ��������ʽ
			Assert::IsTrue(ZGL::affine_vector< 4, double >::cofactor(ZGL::affine_vector< 4, double > { 3, 4, 5, 6 }, 2) == ZGL::affine_vector< 3, double > { 3, 4, 6 });
		}

		TEST_METHOD(affine_vector_affine)
		{
			// affine_vector to be affine vector
			// ��������ת��
			Assert::IsTrue(ZGL::affine_vector< 4, double >::affine(ZGL::vector< 3, double > { 3, -5.6, 12 }, 25) == ZGL::affine_vector< 4, double > { 3, -5.6, 12, 25 });
		}

		TEST_METHOD(affine_vector_normalize) {
			// affine_vector normalize
			// ������һ
			Assert::IsTrue(ZGL::affine_vector< 4, double >::normalize(ZGL::affine_vector< 4, double > { -5.3, 6, 0.15, 0 }) == ZGL::affine_vector< 4, double > { -0.66191849113483718719208834179223, 0.74934168807717417417972265108554, 0.01873354220192935435449306627714 });
		}

		TEST_METHOD(affine_vector_translate)
		{
			// dot translate in R2
			// ��ά��ƽ��
			Assert::IsTrue(ZGL::affine_vector< 3, double >{ 3, 2, 1 } * ZGL::affine_vector< 3, double >::translate(ZGL::affine_vector< 3, double >{ 4, 3, 0 }) == ZGL::affine_vector< 3, double > { 7, 5, 1 });

			// vector translate in R2
			// ��ά����ƽ��
			Assert::IsTrue(ZGL::affine_vector< 3, double >{ 3, 2, 0 } * ZGL::affine_vector< 3, double >::translate(ZGL::affine_vector< 3, double >{ 4, 3, 0 }) == ZGL::affine_vector< 3, double > { 3, 2, 0 });

			// vector translate in R3
			// ��ά��ƽ��
			Assert::IsTrue(ZGL::affine_vector< 4, double >{ 5.3, 8.9, 3.86, 1 } * ZGL::affine_vector< 4, double >::translate(ZGL::affine_vector< 4, double >{ 5.5, 1.1, -6.22, 0 }) == ZGL::affine_vector< 4, double > { 10.8, 10.0, -2.36, 1 });

			// vector translate in R3
			// ��ά����ƽ��
			Assert::IsTrue(ZGL::affine_vector< 4, double >{ -5, 3.2, 6, 0 } * ZGL::affine_vector< 4, double >::translate(ZGL::affine_vector< 4, double >{ 1, 42, -5.2, 0 }) == ZGL::affine_vector< 4, double > { -5, 3.2, 6, 0 });
		}

		TEST_METHOD(affine_vector_rotate)
		{
		}

		TEST_METHOD(affine_vector_scale)
		{
			// the matrix is vector or dot scaling relative orighinal
			// ���������ԭ����ת����
			Assert::IsTrue(ZGL::affine_vector< 3, double > { 1, 2, 0 } * ZGL::affine_vector< 3, double >::scale(2.0) == ZGL::affine_vector < 3, double > { 2, 4, 0 });

			// vector or dot scaling relative other dot
			// ����������������ת����
			Assert::IsTrue(
				ZGL::affine_vector< 3, double >::scale(1.5, ZGL::affine_vector < 3, double >{ 3.2, 5.1, 1 }) == ZGL::square< 3, double > {
					{ 1.5, 0, 0 },
					{ 0, 1.5, 0 },
					{ 1.6, 2.55, 1 },
			});

			// vector or dot scaling to directions relative other dot
			// ����������һ�������ⷽ������
			Assert::IsTrue((ZGL::affine_vector< 4, double > { 1, 2, 3, 4 } * ZGL::affine_vector < 4, double >::scale(3.0, ZGL::graph< 4, 1, double >({ 0, 0, 0, 1 }, { { 0, 0, 1, 0 } }))) == ZGL::affine_vector< 4, double > { 1, 2, 9, 4 });

			Assert::IsTrue((ZGL::affine_vector< 4, double > { 1, 2, 3, 4 } *ZGL::affine_vector < 4, double >::scale(3.0, ZGL::graph< 4, 2, double >({ 0, 0, 0, 1 }, { { 0, 0, 1, 0 }, { 0, 1, 0, 0 } }))) == ZGL::affine_vector< 4, double > { 1, 6, 9, 4 });
		}

		TEST_METHOD(affine_vector_mirror)
		{
			// vector mirror
			// ��������
			Assert::IsTrue(ZGL::affine_vector< 3, double >{ 3, 1, 0 } *ZGL::affine_vector< 3, double >::mirror(ZGL::graph< 3, 1, double >({ 0, 0, 1 }, { { 0, 1, 0 } })) == ZGL::affine_vector< 3, double >{ -3, 1, 0 });
		}
	};
}