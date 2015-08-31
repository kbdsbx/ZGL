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
	/// d_dim: Dimensions of gridding
	///       ����ά��
	/// Titem: Type of item within gridding
	///       ����������
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

		// Discrete data
		// ��ɢ����
		class grid_data {
			_Titem* _data = nullptr;
			std::function< _Titem(const _Tv&) > _func;

		public :
			z_size_t length;

			~grid_data() {
				if (_data)
					delete[] _data;
			}
			// Make discrete date use interval and step
			// ʹ������Ͳ���������ɢ����
			grid_data(_Titem begin, _Titem step, z_size_t len) {
				length = len;
				if (length > 0) {
					_data = new _Titem[length];
				}
				for (z_size_t i = 0; i < length; i++, begin += step)
					_data[i] = begin;
			}
			// Add discrete data at array
			// �����������
			grid_data(_Titem* src, z_size_t len) {
				length = len;
				if (length > 0) {
					_data = new _Titem[length];
				}
				for (z_size_t i = 0; i < length; i++)
					_data[i] = src[i];
			}
			// Generate discrete date use function
			// ʹ�÷���������ɢ����
			grid_data(const std::function< _Titem(const _Tv&) >& func) {
				_func = func;
			}
			// Copy construction
			// ��������
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
			// ��ֵ���캯��
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
			// �Ƿ�����ɢ����
			bool is_data() const {
				return (_data != nullptr);
			}

			// whether function
			// �Ƿ��Ƿ���
			bool is_function() const {
				return (_func != nullptr);
			}

			// Get the I-th data
			// ��ȡ��I������
			_Titem operator[] (z_size_t idx) const {
				return _data[idx];
			}

			// Generate the I-th data
			// ���ɵ�I������
			_Titem operator[] (const _Tv& idx) const {
				return _func(idx);
			}
		} ** _grid_data;

		// Grid node
		// ����ڵ�
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
		} _root;

		grid_node* _cur;

		class iterator {
			typedef iterator _Tit;
		public :
			// Indexs in iterator
			// ����������
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
				for (z_size_t i = d_dim; i >= 0; i--) {
					if (_idx[i] >= _grid->_grid_data_len) {
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

		iterator begin() {
			return iterator(_Tidx());
		}

		iterator end() {
			_Tidx idx;
			for (int i = 0; i < d_dim; i++)
				idx[i] = _grid_data_len;
			return iterator(std::move(idx));
		}

		const grid_node& operator [] (const _Tidx& idx) {
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

		gridding() {
			_grid_data = new grid_data*[dim - 1]{ nullptr };
			_grid_data_len = 0;
		}

		gridding(grid_data src[dim - 1])
			: gridding() {
			for (z_size_t i = 0; i < dim - 1, i++) {
				_grid_data[i] = new grid_data(src[i]);
				if (!_grid_data_len && _grid_data[i]->len) {
					_grid_data_len = _grid_data[i]->len;
				}
			}
		}

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
		}

		// Generate discrete dots' map
		// ������ɢͼ
		grid_node& discrete() {
			_dis(_root, _Tidx());

			return _root;
		}

		/// Recursion nodes within map
		/// �ݹ�ͼ�ڵ�
		///
		/// node : current node
		///	       ��ǰ�ڵ�
		/// idx  : current index of node
		///        ��ǰ�ڵ�����
		void _dis(grid_node& node, const _Tidx& idx) {
			_Tv _dt;
			for (z_size_t i = 0; i < dim - 1; i++) {
				if (_grid_data[i]->is_data())
					_dt[i] = (*(_grid_data[i]))[idx[i]];
				else if (_grid_data[i]->is_function())
					_dt[i] = (*(_grid_data[i]))[_dt];
				else
					throw "";
			}
			_dt[dim - 1] = _Titem(1);
			node.set_dot(_dt);

			for (z_size_t i = 0; i < d_dim; i++) {
				if (idx[i] >= _grid_data_len)
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
