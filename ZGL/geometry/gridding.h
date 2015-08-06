#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>

#ifndef ZGL_GRIDDING
#define ZGL_GRIDDING

namespace ZGL {
	// Gridding
	// 网格
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding
		: public Iseparable {
		typedef Titem _Titem;
		typedef gridding< dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;
	public :

		// Discrete data
		// 离散数据
		class grid_data {
			_Titem* data = nullptr;
			std::function< void(_Titem[dim - 1]) > f = nullptr;

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
			// Add discrete date at array
			// 数组添加数据
			grid_data(_Titem* src, z_size_t len) {
				if (len > 0)
					data = new _Titem[len];
				while (len)
					data[len] = src[len];
			}
			// Make discrete date use function
			// 使用方法生成离散数据
			grid_data(const std::function< void(_Titem*, z_size_t) >& func) {
				f = func;
			}
		} _grid_data[dim - 1];

		class grid_node {
			grid_node** nodes;
			_Tv _dot;
		public :
			~grid_node() {
				z_size_t d_size = (z_size_t)pow(2, d_dim);
				for (z_size_t i = 0; i < d_size; i++)
					if (nodes[i])
						delete nodes[i];
				delete[] nodes;
			}

			grid_node() {
				nodes = new grid_node*[(z_size_t)pow(2, d_dim)];
			}

			grid_node(const _Tv& dot)
				: grid_node(), _dot(dot) { }

			grid_node(_Tv&& dot)
				: grid_node(), _dot(dot) { }

			grid_node* operator [] (z_size_t opt) {
				return nodes[opt];
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

		grid_node& discrete() {
			grid_node _root;

			_Tv _t;
			for (z_size_t i = 0; i < dim - 1; i++) {
			}

			return std::move(_root);
		}
	};
}

#endif // !ZGL_GRIDDING
