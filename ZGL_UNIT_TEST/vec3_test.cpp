#include "stdafx.h"
#include "CppUnitTest.h"
#include  "../ZGL/geometry/vec3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef ZGL::vec3  vec3;
typedef ZGL::square< 3, float > sqr3;

namespace ZGL_TEST
{		
	TEST_CLASS(vec3_test)
	{
	public:
		
		TEST_METHOD(vec3_translation)
		{
			vec3 v1{ 2, 3, 1 };
			sqr3 s1{ { 1, 0, 0}, { 0, 1, 0}, { 2, 3, 1} };

			Assert::IsTrue(vec3::translate(v1) == s1);
		}

		TEST_METHOD(vec3_affine)
		{
			vec3 v1{ 2, 3, 1 };
			ZGL::vector< 2, float> v2{ 2, 3 };
			Assert::IsTrue(vec3::affine(v2, float(1)) == v1);
		}
	};
}