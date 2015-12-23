#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/geometry/affine_vector.h"
#include "../ZGL/geometry/patch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(patch_test)
	{
	public:

		TEST_METHOD(patch_init)
		{
			ZGL::patch< 4, 1, double > p({
				{ -5, 3, .5, 1 },
				{ 6, 0, -3.5, 1 },
			});

			Assert::IsTrue(p.pos == ZGL::affine_vector< 4, double >{ -5, 3, .5, 1 });
			Assert::IsTrue(p.dirs == ZGL::matrix< 1, 4, double > { { 0.91036647746260474487062487626633, -0.24828176658071038496471587534536, -0.33104235544094717995295450046049, 0} });
			Assert::IsTrue(p.verts[0] == ZGL::affine_vector< 4, double > { 6, 0, -3.5, 1 });
		}

		TEST_METHOD(patch_n)
		{
			ZGL::patch< 3, 1, double > p({
				{ -5, 3, 1 },
				{ 6, 0, 1 },
			});

			auto v = p.n();

			Assert::IsTrue(p.n() == ZGL::affine_vector< 3, double > { -0.26311740579210879, -0.96476382123773219, 0 });
		}
	};
}