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
	/// ����
	///
	/// dim: Dimensions of space for gridding in
	///       ��������ά��
	/// Titem: Type of item within gridding
	///       ����������
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
		// ��ɢ���ݽӿ�
		class Igrid_data {
		public :
			// Count for data
			// ��������
			z_size_t count;

			// What data maker type is
			// ����������
			virtual GRID_DATA_TYPE type() const = 0;

			virtual ~Igrid_data() = 0;
		};

		// Discrete data is produced using range
		// ʹ������������ɢ����
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
		// ��ɢ��������
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
		// ����
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
		// ����ڵ�
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
			// ���õ�
			void set_dot(const _Tv& dot) {
				_dot = dot;
			}

			// Set dot
			// ���õ�
			void set_dot(_Tv&& dot) {
				_dot = dot;
			}

			// Get dot
			// ��ȡ��
			_Tv get_dot() const {
				return _dot;
			}
		};

		/*************** Gridding ***************/

		// Gridding root;
		// ������ڵ�
		grid_node* _root;

		// Argument dimension
		// ����ά��
		z_size_t arg_dim;

		// Data maker
		// ����������
		Igrid_data ** _grid_data;

		~gridding() {
			delete[] _grid_data;
			delete _root;
		}

	private :
		// default constructor
		// Ĭ�Ϲ��캯��
		gridding() {
			_grid_data = new Igrid_data * [dim - 1]{ nullptr };
			arg_dim = 0;
		}

	public :
		// consturctor that using array data or function to initialize
		// ʹ�����������ʼ��
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
		// ʹ�����������ʼ��
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
		// ʹ����ʽ���̳�ʼ��
		gridding(std::function< _Titem(_Tv) > func, _Tv max, _Tv min) {
		}

		// Generate discrete dots' map
		// ������ɢͼ
		grid_node& discrete() {
			z_size_t* idx = new z_size_t[arg_dim]{ 0 };
			_dis(_root, idx);
			return _root;
		}

		/// Recursion nodes within map
		/// �ݹ�ͼ�ڵ�
		///
		/// node : current node
		///	       ��ǰ�ڵ�
		/// idx  : current index of node
		///        ��ǰ�ڵ�����
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
					// ������һ���ڵ�ָ�򱾽ڵ�ָ��
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
