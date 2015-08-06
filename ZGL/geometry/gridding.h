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
			std::function< void(const _Tv&) > f = nullptr;

		public :
			~grid_data() {
				if (data)
					delete[] data;
			}
			// Make discrete date use interval and step
			// 使用区间和步长生成离散数据
			grid_data(_Titem begin, _Titem end, _Titem step) {
				z_size_t ct = (z_size_t)floor< _Titem >((end - begin) / step);
				if (ct > 0)
					data = new _Titem[ct];
				for (z_size_t i = 0; i < ct; i++)
					data[i] = begin += step;
			}
			// Add discrete data at array
			// 数组添加数据
			grid_data(_Titem* src, z_size_t len) {
				if (len > 0)
					data = new _Titem[len];
				while (len)
					data[len] = src[len];
			}
			// Make discrete date use function
			// 使用方法生成离散数据
			grid_data(const std::function< void(const _Tv&) >& func) {
				f = func;
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
		};
		grid_data _grid_data[dim - 1];

		class grid_node {
			grid_node** nodes;
			_Tv _dot;
		public :
			~grid_node() {
				z_size_t d_size = (z_size_t)pow(2, d_dim);
				for (z_size_t i = 0; i < d_size; i++) {
					if (nodes[i])
						delete nodes[i];
					nodes[i] = nullptr;
				}
				delete[] nodes;
			}

			grid_node() {
				nodes = new grid_node*[(z_size_t)pow(2, d_dim)]{ nullptr };
			}

			grid_node(const _Tv& dot)
				: grid_node(), _dot(dot) { }

			grid_node(_Tv&& dot)
				: grid_node(), _dot(dot) { }

			grid_node* operator [] (z_size_t opt) {
				return nodes[opt];
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
		}

		gridding(grid_data data[dim - 1]) {
			for (z_size_t i = 0; i < dim - 1, i++)
				_grid_data[i] = data[i];
		}

		gridding(std::initializer_list< grid_data > data) {
			for (z_size_t i = 0; i < dim - 1, i++)
				_grid_data[i] = data[i];
		}

		// Generate discrete dots' map
		// 生成离散图
		grid_node discrete() {
			grid_node _root;

			_Tidx idx;

			_dis(_root, idx);

			return std::move(_root);
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

			for (z_size_t i = 0; i < dim - 1; i++) {
				if (!node[i]) {
					node[i] = new grid_node();
					_Tidx _idx(idx);
					idx[i]++;
					_dis(node[i], idx);
				}
			}
		}
	};
}

#endif // !ZGL_GRIDDING
