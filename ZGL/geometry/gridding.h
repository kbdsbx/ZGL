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
	/// Titem: Type of item within gridding
	///       网格项类型
	template < z_size_t dim, typename Titem >
	class gridding
		: public Iseparable {
	protected :
		typedef Titem _Titem;
		typedef gridding < dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;

	protected :

	public :
		enum GRID_DATA_TYPE {
			RANGE = 1,
			ARRAY = 2,
			FUNC = 3,
		};

		// Interface for discrete data
		// 离散数据接口
		class Igrid_data {
		public :
			// Count for data
			// 数据数量
			z_size_t count;

			// What data maker type is
			// 数据器类型
			virtual GRID_DATA_TYPE type() const = 0;

			virtual ~Igrid_data() = 0;
		};

		// Discrete data is produced using range
		// 使用区间生成离散数据
		class grid_data_range
			: Igrid_data {

			_Titem _begin;
			_Titem _end;
			_Titem _step;
		public :
			grid_data_range() { }
			grid_data_range(_Titem begin, _Titem end, _Titem step) {
				count = (z_size_t)((end - begin) / step);
				_begin = begin;
				_end = end;
				_step = step;
			}
			_Titem operator [] (z_size_t idx) const {
				return _begin + idx * _step;
			}
			GRID_DATA_TYPE type() {
				return GRID_DATA_TYPE::RANGE;
			}
		};

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
		};

		// Function 
		// 函数
		class grid_data_function
			: Igrid_data {
			std::function< _Titem(const _Tv&) > _func = nullptr;

		public :
			grid_data_function() { }
			grid_data_function(const std::function< _Titem(const _Tv&) >& func) {
				count = 0;
				_func = func;
			}

			_Titem operator[] (const _Tv& v) const {
				return _func(v);
			}

			GRID_DATA_TYPE type() {
				return GRID_DATA_TYPE::FUNC;
			}
		};

		// Grid node
		// 网格节点
		class grid_node {
			grid_node** _nodes;
			_Tv _dot;
			const _Tself* _grid;

		public :
			~grid_node() {
				for (z_size_t i = 0; i < _grid->arg_dim; i++) {
					if (_nodes[i])
						delete _nodes[i];
					_nodes[i] = nullptr;
				}
				delete[] _nodes;
			}

			grid_node(const _Tself* grid) {
				_grid = grid;
				_nodes = new grid_node* [_grid->arg_dim << 1]{ nullptr };
			}

			grid_node(const _Tv& dot, const _Tself* grid)
				: grid_node(grid), _dot(dot) { }

			grid_node(_Tv&& dot, const _Tself* grid)
				: grid_node(grid), _dot(dot) { }

			grid_node*& operator [] (z_size_t opt) {
				if (opt >= (_grid->arg_dim << 1))
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

		/*************** Gridding ***************/

		// Gridding root;
		// 网格根节点
		grid_node* _root;

		// Argument dimension
		// 变量维度
		z_size_t arg_dim;

		// Data maker
		// 数据生成器
		Igrid_data ** _grid_data;

		~gridding() {
			delete[] _grid_data;
			delete _root;
		}

	private :
		// default constructor
		// 默认构造函数
		gridding() {
			_grid_data = new Igrid_data * [dim - 1]{ nullptr };
			arg_dim = 0;
		}

	public :
		// consturctor that using array data or function to initialize
		// 使用数组或函数初始化
		gridding(Igrid_data src[dim - 1])
			: gridding() {
			for (z_size_t i = 0; i < dim - 1, i++) {
				_grid_data[i] = src[i];
				if (_grid_data[i]->type() != GRID_DATA_TYPE::FUNC)
					arg_dim++;
			}

			if (arg_dim == dim)
				throw "";

			_root = new grid_node(this);

			_dis(_root, _Tidx());
		}

		// consturctor that using array data or function to initialize
		// 使用数组或函数初始化
		gridding(std::initializer_list< Igrid_data* > src)
			: gridding() {
			z_size_t i = 0;
			for (Igrid_data* s : src) {
				_grid_data[i] = s;
				if (_grid_data[i]->type() != GRID_DATA_TYPE::FUNC)
					arg_dim++;
				i++;
			}

			if (arg_dim == dim)
				throw "";

			_root = new grid_node(this);

			_dis(_root, _Tidx());
		}

		// constructor that using Implicit equation to initialize
		// 使用隐式方程初始化
		gridding(std::function< _Titem(_Tv) > func, _Tv max, _Tv min) {
		}

		// Generate discrete dots' map
		// 生成离散图
		grid_node& discrete() {
			z_size_t* idx = new z_size_t[arg_dim]{ 0 };
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
		void _dis(grid_node* node, z_size_t idx) {
			_Tv _dt;
			for (z_size_t i = 0; i < dim - 1; i++) {
				switch (_grid_data[i]->type()) {
				case GRID_DATA_TYPE::ARRAY :
				case GRID_DATA_TYPE::RANGE :
					_dt[i] = (*(_grid_data[i]))[idx[i]];
					break;
				case GRID_DATA_TYPE::FUNC :
					_dt[i] = (*(_grid_data[i]))[_dt];
					break;
				default:
					throw "";
				}
			}
			_dt[dim - 1] = _Titem(1);
			node.set_dot(_dt);

			for (z_size_t i = 0; i < arg_dim; i++) {
				if (idx[i] >= _grid_data[i]->count)
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
