#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ZGL/geometry/affine_vector.h"
#include "../ZGL/geometry/graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZGL_TEST
{
	TEST_CLASS(graph_test)
	{
	public:

		TEST_METHOD(graph_orthogonalize)
		{
			ZGL::graph< 4, 3, double > g({ 0, 0, 0, 1 }, { { 1, 0, 0, 0 }, { 2, 2, 0, 0 }, { 1, 2, 3, 0 } });
			g.orthogonalize();
				
			Assert::IsTrue(g.dirs == ZGL::matrix< 3, 4, double >{ { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 } });
		}

		TEST_METHOD(graph_n)
		{
			ZGL::graph< 4, 2, double > g({ 1, 1, 1, 0 }, { { -5, .3, 2, 0 }, { 1.5, -3, -5, 0 } });

			auto v = ZGL::affine_vector< 4, double >::normalize(ZGL::affine_vector< 4, double >::cross({
				ZGL::affine_vector< 4, double >{ -5, .3, 2, 0 },
				ZGL::affine_vector< 4, double >{ 1.5, -3, -5, 0 },
			}));

			auto n = g.n({ 0, 0, 0, 1 });

			Assert::IsTrue(n == v * -1);
		}
	};
}