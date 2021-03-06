#include "../inc.h"
#include <math.h>
#include <functional>
#include <initializer_list>
#include <vector>
#include "../interface/Iseparable.h"
#include "iterator.h"

#ifndef ZGL_GRIDDING
#define ZGL_GRIDDING

_ZGL_BEGIN
	/// Gridding
	/// 网格
	///
	/// dim: Dimensions of space for gridding in
	///       网格所在维度
	/// d_dim: elements' dimension
	///	      元素维度
	/// Titem: Type of item within gridding
	///       网格项类型
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding {
	protected :
		typedef Titem _Titem;
		// The rank of affine vector
		// 仿射向量的秩
		const static z_size_t rank = dim + 1;

		typedef gridding< dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;
		typedef vector< d_dim, _Titem > _Tval;

		typedef iterator< d_dim, _Tv > _Tit;
		typedef typename _Tit::Tidx _Tidx;

	public :
		// Equation in coordinate' argument type
		// 坐标函数参数类型
		typedef _Tval Targ;
		// Iterator values' type
		// 迭代器值类型
		typedef _Tv Tval;
		// Iterator indexs' type
		// 迭代器索引类型
		typedef _Tidx Tidx;

	public :

		// Data for making Gridding, The count equal of d_dim param.
		// 用于生成网格的数据
		class grid_data {
			// Discrete data array
			// 离散数据数组
			_Titem* _arr = nullptr;

			// Length of array or making data with range
			// 获取或生成的数据量
			z_size_t _len;

		public :

			~grid_data() {
				if (_arr) {
					delete[] _arr;
				}
			}

			grid_data() { }

			grid_data(_Titem start, _Titem step, _Titem end) {
				z_size_t len = (z_size_t)floor((end - start) / step) + 1;
				_len = len;
				_arr = new _Titem[_len];
				for (z_size_t i = 0; i < len; i++) {
					if (!i) {
						_arr[i] = start;
					} else {
						_arr[i] = _arr[i - 1] + step;
					}
				}
			}

			grid_data(_Titem* arr, z_size_t len) {
				_len = len;
				_arr = new _Titem[_len];
				while (len--) {
					_arr[len] = arr[len];
				}
			}

			grid_data(const grid_data& src) {
				*this = src;
			}

			grid_data(grid_data&& src) {
				*this = src;
			}

			grid_data& operator = (const grid_data& src) {
				z_size_t len = src._len;

				_len = len;
				_arr = new _Titem[_len];
				while (len--) {
					_arr[len] = src._arr[len];
				}

				return *this;
			}
			
			grid_data& operator = (grid_data&& src) {
				_len = src._len;
				_arr = src._arr;
				src._arr = nullptr;

				return *this;
			}

			_Titem operator [](z_size_t idx) {
				if (idx > _len)
					throw std::out_of_range( "Index of this data is out of range." );

				return _arr[idx];
			}

			// Length of array or making data with range, Readonly
			// 获取或生成的数据量，只读
			z_size_t len() const {
				return _len;
			}
		};

		///** Result **///

	private :
		// root node
		// 根节点
		_Tv * _root;

		///** Data and equation **///

		// Data making
		// 构造数据
		grid_data _data[d_dim];

		// equation in coordinate
		// 坐标函数
		std::function< _Titem(_Tval, z_size_t) > _funcs[dim];

		_Tidx _max;

	public :
		~gridding() {
			delete[] _root;
		}

		gridding(const grid_data data[d_dim], const std::function< _Titem(_Tval, z_size_t = 0) > funcs[dim]) {
			z_size_t i = d_dim, c = 1;
			while (i--) {
				_data[i] = data[i];
			}

			i = dim;
			while (i--) {
				_funcs[i] = funcs[i];
			}

			i = d_dim;
			while (i--) {
				c *= _data[i].len();
				_max[i] = _data[i].len();
			}

			_root = new _Tv[c];

			_dis();
		}

		gridding(const std::initializer_list< grid_data > data, const std::initializer_list< std::function< _Titem(_Tval, z_size_t = 0) > > funcs) {
			z_size_t i = 0, c = 1;
			for (auto d : data) {
				_data[i] = d;
				i++;
			}

			i = 0;
			for (auto f : funcs) {
				_funcs[i] = f;
				i++;
			}

			i = d_dim;
			while (i--) {
				c *= _data[i].len();
				_max[i] = _data[i].len();
			}

			_root = new _Tv[c];

			_dis();
		}

		gridding(const _Tself& src) {
			z_size_t i = d_dim, c = 1;
			while (i--)
				_data[i] = src._data[i];
			
			i = dim;
			while (i--) {
				_funcs[i] = src._funcs[i];
			}

			i = d_dim;
			while (i--) {
				c *= _data[i].len();
				_max[i] = _data[i].len();
			}

			_root = new _Tv[c];

			_dis();
		}

		_Tself& operator = (const _Tself& src) {
			z_size_t i = d_dim, c = 1;
			while (i--) {
				_data[i] = src._data[i];
			}

			i = dim;
			while (i--) {
				_funcs[i] = src._funcs[i];
			}

			i = d_dim;
			while (i--) {
				c *= _data[i].len();
				_max[i] = _data[i].len();
			}

			_root = new _Tv[c];

			_dis();

			return *this;
		}

		///*** Iterator ***///

		// iterator begin, the index is { 0, 0, 0, 0 } in 3D gridding etc.
		// 迭代开始位置，例：3D网格索引为idx{ 0, 0, 0, 0 }
		_Tit begin() const {
			return _Tit(_max, _root);
		}

		// iterator end, the index is { 0, 0, 0, 1 } in 3D gridding etc.
		// 迭代结束位置，例：3D索引为idx{ 0, 0, 0, 1 }
		const _Tit end() const {
			_Tit _it(_max, _root);
			_it._idx[d_dim] = 1;

			return STD_MOVE(_it);
		}

	private :
		void _dis() {
			for (auto it = begin(); it != end(); ++it) {
				_Tv _dt;
				_Tval val;
				// Getting params for coordinate function;
				// 获取坐标方程参数
				for (z_size_t i = 0; i < d_dim; i++) {
					val[i] = _data[i][it._idx[i]];
				}
				// Sent params into coordinate function to compute.
				// 将参数传递至坐标方程进行计算
				for (z_size_t i = 0; i < rank - 1; i++) {
					_dt[i] = _funcs[i](val, i);
				}
				// Set 1 because it is the dot.
				// 设置节点为点元素
				_dt[rank - 1] = 1;
				(*it) = _dt;
			}
		}

	};
_ZGL_END

#endif // !ZGL_GRIDDING
