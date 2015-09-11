#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>
#include <map>

#ifndef ZGL_GRIDDING
#define ZGL_GRIDDING

namespace ZGL {
	/// Gridding
	/// 网格
	///
	/// dim: Dimensions of space for gridding in
	///       网格所在维度
	/// d_dim: Dimensions of gridding
	///       网格维度
	/// Titem: Type of item within gridding
	///       网格项类型
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding
		: public Iseparable {
	protected :
		typedef Titem _Titem;
		typedef gridding < dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;
		typedef vector < d_dim, z_size_t, void > _Tidx;

	protected :

	public :
		z_size_t _grid_data_len;

		enum GRID_DATA_TYPE {
			RANGE = 1,
			ARRAY = 2,
			FUNC = 3,
		}

		// Interface for discrete data
		// 离散数据接口
		class Igrid_data {
		public :
			z_size_t count;

			// What data maker type is
			// 数据器类型
			virtual GRID_DATA_TYPE type() const = 0;
		}

		// Discrete data is produced using range
		// 使用区间生成离散数据
		class grid_data_range
			: Igrid_data {

			_Titem _begin;
			_Titem _end;
			_Titem _step;
		public :
			grid_data_range() { }
			grid_data_range(_Titem begin, _Titem end, z_size_t len) {
				count = len;
				_begin = begin;
				_end = end;
				_step = (end - begin) / len;
			}
			_Titem operator [] (z_size_t idx) const {
				return begin + idx * _step;
			}
			GRID_DATA_TYPE type() {
				return GRID_DATA_TYPE::RANGE;
			}
		}

		// Discrete data array
		// 离散数据数组
		class grid_data_array
			: Igrid_data {
			_Titem * _data = nullptr;

		public :
			~grid_data_array() {
				if (_data) {
					delete[] _data;
				}
				_data = nullptr;
			}
			grid_data_array() { }
			grid_data_array(const _Titem* data, z_size_t len) {
				count = len;
				_data = new _Titem[len];
				while (len--) {
					_data[i] = data[i];
				}
			}

			_Titem operator[] (z_size_t idx) const {
				return _data[i];
			}
			GRID_DATA_TYPE type() {
				return GRID_DATA_TYPE::ARRAY;
			}
		}

		// Discrete data is produced using function
		// 使用方法生成离散数据
		class grid_data_function
			: Igrid_data {
			std::function< _Titem(const _Tv&) > _func = nullptr;

		public :
			grid_data_function() { }
			grid_data_function(const std::function< _Titem(const _Tv&) >& func) {
				_func = func;
			}

			_Titem operator[] (const _Tv& v) const {
				return func(v);
			}

			GRID_DATA_TYPE type() {
				return GRID_DATA_TYPE::FUNC;
			}
		}

		/*
		// Discrete data
		// 离散数据
		class grid_data {
			_Titem _begin;
			_Titem _end;

			_Titem * _data = nullptr;

			std::function< _Titem(const _Tv&) > _func = nullptr;

		public :
			z_size_t length;

			GRID_DATA_TYPE data_type;

			~grid_data() {
				if (_data)
					delete[] _data;
			}
			// Make discrete date use interval and step
			// 使用区间和步长生成离散数据
			grid_data(_Titem begin, _Titem end, z_size_t len) {
				length = len;

				_begin = begin;
				_end = end;
			}
			// Add discrete data at array
			// 数组添加数据
			grid_data(_Titem* src, z_size_t len) {
				length = len;
				if (length > 0) {
					_data = new _Titem[length];
				}
				for (z_size_t i = 0; i < length; i++)
					_data[i] = src[i];
			}
			// Generate discrete date use function
			// 使用方法生成离散数据
			grid_data(const std::function< _Titem(const _Tv&) >& func) {
				_func = func;
			}
			// Copy construction
			// 拷贝构造
			grid_data(const grid_data& src) {
				length = src.length;
				if (length > 0 && src._data) {
					_data = new _Titem[length];
					for (z_size_t i = 0; i < length; i++)
						_data[i] = src._data[i];
				}
				if (src._func)
					_func = src._func;
			}
			// R-value construction
			// 右值构造函数
			grid_data(grid_data&& src) {
				length = src.length;
				if (length > 0 && src._data) {
					_data = src._data;
					src._data = nullptr;
				}
				if (src._func)
					_func = src._func;
			}

			// whether discrete data
			// 是否是离散数据
			bool is_data() const {
				return (_data != nullptr);
			}

			// whether function
			// 是否是方法
			bool is_function() const {
				return (_func != nullptr);
			}

			// Get the I-th data
			// 获取第I个数据
			_Titem operator[] (z_size_t idx) const {
				return _data[idx];
			}

			// Generate the I-th data
			// 生成第I个数据
			_Titem operator[] (const _Tv& idx) const {
				return _func(idx);
			}
		} ** _grid_data;
		*/

		// Grid node
		// 网格节点
		class grid_node {
			grid_node** _nodes;
			_Tv _dot;
			const static z_size_t _c = d_dim << 1;

		public :
			~grid_node() {
				for (z_size_t i = 0; i < (_c >> 1); i++) {
					if (_nodes[i])
						delete _nodes[i];
					_nodes[i] = nullptr;
				}
				delete[] _nodes;
			}

			grid_node() {
				_nodes = new grid_node*[_c]{ nullptr };
			}

			grid_node(const _Tv& dot)
				: grid_node() {
				_dot = dot;
			}

			grid_node(_Tv&& dot)
				: grid_node() {
				_dot = dot;
			}

			grid_node*& operator [] (z_size_t opt) {
				if (opt >= _c)
					throw std::out_of_range("Node pointer in this grid-node is out of range.");
				return _nodes[opt];
			}

			// Set dot
			// 设置点
			void set_dot(const _Tv& dot) {
				_dot = dot;
			}

			// Set dot
			// 设置点
			void set_dot(_Tv&& dot) {
				_dot = dot;
			}

			// Get dot
			// 获取点
			_Tv get_dot() const {
				return _dot;
			}
		};

		class iterator {
			typedef iterator _Tit;
		public :
			// Indexs in iterator
			// 迭代器索引
			_Tidx _idx;
			const _Tself* _grid;

			iterator(const _Tidx& idx, const _Tself* grid)
				: _idx(idx), _grid(grid) { }

			iterator(_Tidx&& idx, const _Tself* grid)
				: _idx(idx), _grid(grid) { }

			iterator(const _Tit& src)
				: _idx(src._idx), _grid(src._grid) { }

			iterator(_Tit&& src)
				: _idx(src._idx), _grid(src._grid) { }

			grid_node& operator * () {
				grid_node* cur = const_cast< grid_node* >( &(_grid->_root) );
				for (z_size_t i = 0; i < d_dim; i++)
					for (z_size_t j = 0; j < _idx[i]; j++)
						cur = (*cur)[i];

				return *cur;
			}

			void operator ++ () {
				for (z_size_t i = (d_dim - 1); i >= 0; i--) {
					// todo;
					if (_idx[i] >= _grid->_grid_data_len && i != 0) {
						_idx[i] = 0;
					} else {
						_idx[i]++;
						break;
					}
				}
			}

			bool operator != (_Tit it) {
				return (_idx != it._idx);
			}
		};

		// Gridding root;
		// 网格根节点
		grid_node _root;

		// Data maker
		// 数据生成器
		Igrid_data ** _grid_data;

		iterator begin() {
			return iterator(_Tidx(), this);
		}

		iterator end() {
			_Tidx idx;
			for (int i = 0; i < d_dim; i++)
				// todo;
				idx[i] = _grid_data_len;
			return iterator(std::move(idx), this);
		}

		const grid_node& operator [] (const _Tidx& idx) const {
			return * iterator(idx, this);
		}

		~gridding() {
			for (z_size_t i = 0; i < dim - 1; i++) {
				if (_grid_data[i])
					delete _grid_data[i];
				_grid_data[i] = nullptr;
			}
			delete[] _grid_data;
		}

		// default constructor
		// 默认构造函数
		gridding() {
			_grid_data = new Igrid_data*[dim - 1]{ nullptr };
			_grid_data_len = 0;
		}

		// consturctor that using array data or function to initialize
		// 使用数组或函数初始化
		gridding(grid_data src[dim - 1])
			: gridding() {
			for (z_size_t i = 0; i < dim - 1, i++) {
				_grid_data[i] = new grid_data(src[i]);
				if (!_grid_data_len && _grid_data[i]->length) {
					_grid_data_len = _grid_data[i]->length;
				}
			}

			_dis(_root, _Tidx());
		}

		// constructor that using Implicit equation to initialize
		// 使用隐式方程初始化
		gridding(std::function< _Titem(_Tv) > func, _Tv max, _Tv min) {
		}

		// consturctor that using array data or function to initialize
		// 使用数组或函数初始化
		gridding(std::initializer_list< grid_data > src)
			: gridding() {
			z_size_t i = 0;
			for (const grid_data& s : src) {
				_grid_data[i] = new grid_data(s);
				if (!_grid_data_len && _grid_data[i]->length) {
					_grid_data_len = _grid_data[i]->length;
				}
				i++;
			}

			_dis(_root, _Tidx());
		}

		// Generate discrete dots' map
		// 生成离散图
		grid_node& discrete() {
			_dis(_root, _Tidx());
			return _root;
		}

		/// Recursion nodes within map
		/// 递归图节点
		///
		/// node : current node
		///	       当前节点
		/// idx  : current index of node
		///        当前节点索引
		void _dis(grid_node& node, const _Tidx& idx) {
			_Tv _dt;
			for (z_size_t i = 0; i < dim - 1; i++) {
				if (_grid_data[i]->is_data()) {
					_dt[i] = (*(_grid_data[i]))[idx[i]];
				} else if (_grid_data[i]->is_function()) {
					_dt[i] = (*(_grid_data[i]))[_dt];
				} else {
					throw "";
				}
			}
			_dt[dim - 1] = _Titem(1);
			node.set_dot(_dt);

			for (z_size_t i = 0; i < d_dim; i++) {
				if (idx[i] >= _grid_data_len)
					continue;
				if (!node[i]) {
					node[i] = new grid_node();
					// set point to current nodes' pointer at next node
					// 设置下一个节点指向本节点指针
					(*(node[i]))[i + d_dim] = &node;
					_Tidx _idx(idx);
					_idx[i] = _idx[i] + 1;
					_dis(*(node[i]), _idx);
				}
			}
		}
	};
}

#endif // !ZGL_GRIDDING
