#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/geometry/affine_vector.h"
#include "../ZGL/geometry/iterator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(iterator_test)
	{
	public:

		TEST_METHOD(iterator_operator)
		{
			ZGL::vector< 4, ZGL::z_size_t > max;
			for (int i = 0; i < 3; i++)
				max[i] = 2;

			ZGL::affine_vector< 4, double > val[8]{
				ZGL::affine_vector< 4, double > { 1, 2, 3, 4 },
				ZGL::affine_vector< 4, double > { 2, 3, 4, 5 },
				ZGL::affine_vector< 4, double > { 3, 4, 5, 6 },
				ZGL::affine_vector< 4, double > { 4, 5, 6, 7 },
				ZGL::affine_vector< 4, double > { 5, 6, 7, 8 },
				ZGL::affine_vector< 4, double > { 6, 7, 8, 9 },
				ZGL::affine_vector< 4, double > { 7, 8, 9, 0 },
				ZGL::affine_vector< 4, double > { 8, 9, 0, 1 },
			};

			ZGL::iterator< 3, ZGL::affine_vector< 4, double > > it(max, val);
			Assert::IsTrue(*it == ZGL::affine_vector< 4, double > { 1, 2, 3, 4 });

			++it;
			++it;

			Assert::IsTrue(*it == ZGL::affine_vector< 4, double > { 3, 4, 5, 6 });

			ZGL::iterator < 3, ZGL::affine_vector< 4, double > > nit(it);

			Assert::IsTrue(*nit == ZGL::affine_vector< 4, double > { 3, 4, 5, 6 });

			ZGL::iterator < 3, ZGL::affine_vector< 4, double > > nit2 = nit;

			Assert::IsTrue(*nit2 == ZGL::affine_vector< 4, double > { 3, 4, 5, 6 });

			Assert::ExpectException< std::out_of_range >([&]() {
				nit2 += nit;
			});
		}
	};
}