#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>

#ifndef ZGL_GRIDDING
#define ZGL_GRIDDING

namespace ZGL {
	/// Gridding
	/// 网格
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding
		: public Iseparable {
	protected :
		typedef Titem _Titem;
		typedef gridding < dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;
		typedef vector < dim, z_size_t, void > _Tidx;

	public :

		// Discrete data
		// 离散数据
		class grid_data {
			_Titem* data = nullptr;
			z_size_t _c = 0;
			std::function< _Titem(const _Tv&) > f;

		public :
			~grid_data() {
				if (data)
					delete[] data;
			}
			// Make discrete date use interval and step
			// 使用区间和步长生成离散数据
			grid_data(_Titem begin, _Titem end, _Titem step) {
				_c = (z_size_t)std::floor((end - begin) / step) + 1;
				if (_c > 0)
					data = new _Titem[_c];
				for (z_size_t i = 0; i < _c; i++, begin += step)
					data[i] = begin;
			}
			// Add discrete data at array
			// 数组添加数据
			grid_data(_Titem* src, z_size_t len) {
				_c = len;
				if (len > 0)
					data = new _Titem[len];
				while (len--)
					data[len] = src[len];
			}
			// Generate discrete date use function
			// 使用方法生成离散数据
			grid_data(const std::function< _Titem(const _Tv&) >& func) {
				f = func;
			}
			// Copy construction
			// 拷贝构造
			grid_data(const grid_data& src) {
				if (src.data) {
					data = new _Titem[_c];
					for (z_size_t i = 0; i < _c; i++)
						data[i] = src.data[i];
				}
				if (src.f)
					f = src.f;
			}
			// R-value construction
			// 右值构造函数
			grid_data(grid_data&& src) {
				if (src.data) {
					data = src.data;
					src.data = nullptr;
				}
				if (src.f)
					f = src.f;
			}

			// whether discrete data
			// 是否是离散数据
			bool is_data() const {
				return (data != nullptr);
			}

			// whether function
			// 是否是方法
			bool is_function() const {
				return (f != nullptr);
			}

			// Get the I-th data
			// 获取第I个数据
			_Titem operator[] (z_size_t idx) const {
				return data[idx];
			}

			// Generate the I-th data
			// 生成第I个数据
			_Titem operator[] (const _Tv& idx) const {
				return f(idx);
			}
		} ** _grid_data;

		// Grid node
		// 网格节点
		class grid_node {
			grid_node** _nodes;
			_Tv _dot;
			const z_size_t _c = 2 * d_dim;

		public :
			~grid_node() {
				for (z_size_t i = 0; i < _c; i++) {
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

			grid_node* operator [] (z_size_t opt) {
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
		} _root;

		~gridding() {
			for (z_size_t i = 0; i < dim - 1; i++) {
				if (_grid_data[i])
					delete _grid_data[i];
				_grid_data[i] = nullptr;
			}
			delete[] _grid_data;
		}

		gridding() {
			_grid_data = new grid_data*[dim - 1]{ nullptr };
		}

		gridding(grid_data src[dim - 1])
			: gridding() {
			for (z_size_t i = 0; i < dim - 1, i++)
				_grid_data[i] = new grid_data(src[i]);
		}

		gridding(std::initializer_list< grid_data > src)
			: gridding() {
			z_size_t i = 0;
			for (const grid_data& s : src) {
				_grid_data[i] = new grid_data(s);
				i++;
			}
		}

		// Generate discrete dots' map
		// 生成离散图
		grid_node& discrete() {
			_Tidx idx;

			_dis(_root, idx);

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
				if (_grid_data[i].is_data())
					_dt[i] = _grid_data[i][idx[i]];
				else if (_grid_data[i].is_function())
					_dt[i] = _grid_data[i][_dt];
				else
					throw "";
			}
			_dt[dim - 1] = _Titem(1);
			node.set_dot(_dt);

			z_size_t _st = d_dim >> 1;
			for (z_size_t i = 0; i < _st; i++) {
				if (!node[i]) {
					node[i] = new grid_node();
					// set point to current nodes' pointer at next node
					// 设置下一个节点指向本节点指针
					(*node[i])[i + _st] = node;
					_Tidx _idx(idx);
					idx[i]++;
					_dis(node[i], idx);
				}
			}
		}
	};
}

#endif // !ZGL_GRIDDING
