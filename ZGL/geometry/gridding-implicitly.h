#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>

#ifndef ZGL_GRIDDING_IMPLICITLY
#define ZGL_GRIDDING_IMPLICITLY

namespace ZGL {
	/// Gridding Implicitly
	/// 隐式网格
	///
	/// dim: Dimensions of space for gridding in
	///       网格所在维度
	/// d_dim: elements' dimension
	///	      元素维度
	/// Titem: Type of item within gridding
	///       网格项类型
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding_implicitly {
		typedef Titem _Titem;
		typedef gridding_implicitly< dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;

	public :

	public :

		// Range for making Gridding
		// 用于生成网格的范围范围
		class grid_range {
			// range of coordinate in implicit function graphic.
			// 隐式方程的图形范围
			_Titem _min;
			_Titem _max;

		public:
			grid_range() { }

			grid_range(_Titem min, _Titem max)
				: _min(min), _max(max) { }

			grid_range& operator = (const grid_range& src) {
				_min = src._min;
				_max = src._max;

				return *this;
			}

			// Min of coordinate
			// 坐标最小值
			_Titem min_s() const {
				return _min;
			}

			// Max of coordinate
			// 坐标最大值
			_Titem max_s() const {
				return _max;
			}
		};

		///** coordinate Range and implicit function **///

		// Range of coordinate in implicit function
		// 隐式方法的坐标范围
		grid_range _range[d_dim];

		// implicit function
		// 隐式函数
		std::function< bool(_Tv) > _func;

	private :
		gridding_implicitly() { }

	public :
		gridding_implicitly(const grid_range range[d_dim], const std::function< bool(_Tv) > func)
			: gridding() {
			z_size_t i = d_dim;
			while (i--)
				_range[i] = range[i];

			_i_func = func;

			_dis();
		}

		void _dis() {
		}
	};
}

#endif // !ZGL_GRIDDING_IMPLICITLY
