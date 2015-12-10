#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>
#include <vector>
#include "gridding.h"

#ifndef ZGL_GRIDDING_IMPLICITLY
#define ZGL_GRIDDING_IMPLICITLY

_ZGL_BEGIN
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
	class gridding_implicitly
		: public Iseparable {
		typedef Titem _Titem;
		typedef gridding_implicitly< dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Targ;
		typedef affine_vector < dim, _Titem > _Tv;
		typedef gridding< dim, dim - 1, _Titem > _Tgrid;

		const z_size_t _peak_count = (z_size_t)pow(d_dim, 2);

		_Titem _step = _Titem(0.1);

	public :

		// Equation implicitly argument type
		// 坐标函数参数类型
		typedef _Targ Targ;

	public :

		// segment result of gridding
		// 表格段结构
		struct segment {
			_Tv first;
			_Tv last;

			segment() { }

			segment(const _Tv& sFrist, const _Tv& sLast)
				: first(sFrist), last(sLast) { }
		};

		// Range for making Gridding
		// 用于生成网格的范围范围
		class grid_range {

			// range of coordinate in implicit function graphic.
			// 隐式方程的图形范围
			_Titem _range[2];

			friend class gridding_implicitly< dim, d_dim, _Titem >;

		private :
			grid_range() { }

		public:
			grid_range(_Titem min, _Titem max) {
				_range[0] = min;
				_range[1] = max;
			}

			grid_range& operator = (const grid_range& src) {
				_range[0] = src._range[0];
				_range[1] = src._range[1];

				return *this;
			}

			// Min of coordinate
			// 坐标最小值
			_Titem mini() const {
				return _range[0];
			}

			// Max of coordinate
			// 坐标最大值
			_Titem maxi() const {
				return _range[1];
			}
		};

		///** Result **///
		
		// nodes
		// 节点
		std::vector< _Tv > _root;

		///** coordinate Range and implicit function **///

		// Range of coordinate in implicit function
		// 隐式方法的坐标范围
		grid_range _range[dim - 1];

		// implicit function
		// 隐式函数
		std::function< _Titem(_Targ) > _func;

	private :
		gridding_implicitly() { }

	public :
		gridding_implicitly(const grid_range range[dim - 1], const std::function< _Titem(_Targ) > func)
			: gridding() {
			z_size_t i = dim - 1;
			while (i--)
				_range[i] = range[i];

			_func = func;

			_dis();
		}

		gridding_implicitly(const std::initializer_list< grid_range > range, const std::function< _Titem(_Targ) > func) {
			z_size_t i = 0;
			for (auto r : range) {
				_range[i] = r;
				i++;
			}

			_func = func;

			_dis();
		}

		_Tself& operator = (const _Tself& src) {
			z_size_t i = dim - 1;
			while (i--)
				_range[i] = src._range[i];

			_func = src._func;

			_dis();
		}

		void _dis() {
			if (_root.size()) {
				_root.clear();
			}

			_Tgrid rect = make_range(_range);
			for (auto it = rect.begin(); it != rect.end(); ++it) {
				_Titem v = _func(*it);
				for (z_size_t i = 0; i < dim - 1; i++) {
					auto nit(it);
					nit._idx[i] = nit._idx[i] + 1;
					if (nit._idx[i] >= nit._max[i]) {
						continue;
					}
					_Titem nv = _func(*nit);
					if (v <= _Titem(0) && nv > _Titem(0) || v > _Titem(0) && nv <= _Titem(0)) {
						_Tv t(*it);
						t[i] = t[i] + _step * .5;
						_root.push_back(t);
					}
				}
			}
		}

		void _loop(const grid_range range[dim - 1], z_size_t deep) {
			_Tgrid rect = make_range(range);

			for (auto it = rect.begin(); it != rect.end(); ++it) {
				// 计算规范网格点的梯度信息
				_Titem v = _func(*it);
				for (z_size_t i = 0; i < d_dim; i++) {
					auto nit(it);
					nit._idx[i] = (nit._idx[i] == 0 ? 1 : 0);
					_Titem nv = _func(*nit);
					// 若点与相邻的点不同时在隐式曲面一侧，则隐式曲面的边与两点之间的线段相交
					if (v <= _Titem(0) && nv > _Titem(0) || v > _Titem(0) && nv <= _Titem(0)) {
						// 缩小范围
						grid_range nrange[d_dim];
						for (z_size_t d = 0; d < d_dim; d++) {
							_Titem len = range[d].maxi() - range[d].mini();
							if (nit._idx[d]) {
								nrange[d] = grid_range(range[d].mini(), range[d].maxi() - len);
							} else {
								nrange[d] = grid_range(range[d].mini() + len, range[d].maxi());
							}
						}
						_loop(nrange, deep - 1);	// 使用二分法继续划分
					}
				}

				// 若点与相邻的所有点都在隐式曲面的一侧，则隐式曲面的边不与此区域相交，放弃继续划分
				// 如果需要计算体素内外，则还需进一步判断
			}
		}

		void set_step(_Titem step) {
			_step = step;
		}

	private :

		_Tgrid make_range(const grid_range range[dim - 1]) {
			_Tgrid::grid_data data[dim - 1];
			for (z_size_t i = 0; i < dim - 1; i++)
				data[i] = _Tgrid::grid_data(_range[i].mini(), _step, _range[i].maxi());

			std::function< _Titem(typename _Tgrid::Targ, z_size_t) > funcs[dim - 1];
			for (z_size_t i = 0; i < dim - 1; i++)
				funcs[i] = [](typename _Tgrid::Targ arg, z_size_t idx) { return arg[idx]; };

			return STD_MOVE(_Tgrid(data, funcs));
		}
	};

_ZGL_END

#endif // !ZGL_GRIDDING_IMPLICITLY
