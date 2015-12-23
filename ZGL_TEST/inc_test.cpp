#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/inc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(inc_test)
	{
	public:

		TEST_METHOD(inc_pow)
		{
			Assert::IsTrue(ZGL::Pow< 2, 3 >::result == 8);
			Assert::IsTrue(ZGL::Pow< 14, 4 >::result == 38416);
		}
	};
}