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
	/// d_dim: elements' dimension
	///	      Ԫ��ά��
	/// Titem: Type of item within gridding
	///       ����������
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding
		: public Iseparable {
		typedef Titem _Titem;
		typedef gridding< dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;
		typedef vector< d_dim, _Titem > _Tval;
		typedef vector< d_dim + 1, z_size_t > _Tidx;

	public :

		// Data for making Gridding, The count equal of d_dim param.
		// �����������������
		class grid_data {
			// Discrete data is produced using range
			// ʹ������������ɢ����
			_Titem _start;
			_Titem _end;
			_Titem _step;

			// Discrete data array
			// ��ɢ��������
			_Titem* _arr = nullptr;

			// Length of array or making data with range
			// ��ȡ�����ɵ�������
			z_size_t _len;

			// Type of data
			//		1: Array
			//		2: Range
			// ��������
			z_size_t _flag;

		public :

			grid_data() { }

			grid_data(_Titem start, _Titem step, _Titem end)
				: _start(start), _end(end), _step(step) {
				_len = (z_size_t)floor((_end - _start) / _step) + 1;
				_flag = 2;
			}

			grid_data(_Titem* arr, z_size_t len) {
				_len = len;
				_arr = new _Titem[_len];
				while (len--)
					_arr[len] = arr[len];
				_flag = 1;
			}

			grid_data(const grid_data& src) {
				*this = src;
			}

			grid_data(grid_data&& src) {
				*this = src;
			}

			grid_data& operator = (const grid_data& src) {
				_flag = src._flag;

				_start = src._start;
				_end = src._end;
				_step = src._step;

				_len = src._len;
				if (src._flag == 1) {
					_arr = new _Titem[_len];
					z_size_t i = src._len;
					while (i--)
						_arr[i] = src._arr[i];
				}

				return *this;
			}
			
			grid_data& operator = (grid_data&& src) {
				_flag = src._flag;

				_start = src._start;
				_end = src._end;
				_step = src._step;

				_len = src._len;
				_arr = src._arr;
				src._arr = nullptr;

				return *this;
			}

			_Titem operator [](z_size_t idx) {
				if (idx > _len)
					throw std::out_of_range( "Index of this data is out of range." );

				switch (_flag) {
				case 1:
					return _arr[idx];
				case 2:
					return _start + idx * _step;
				default :
					throw "";
				}
			}

			// Length of array or making data with range, Readonly
			// ��ȡ�����ɵ���������ֻ��
			z_size_t len() const {
				return _len;
			}
		};

		// Range for making Gridding
		// ������������ķ�Χ��Χ
		class grid_range {
			// range of coordinate in implicit function graphic.
			// ��ʽ���̵�ͼ�η�Χ
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
			// ������Сֵ
			_Titem min_s() const {
				return _min;
			}

			// Max of coordinate
			// �������ֵ
			_Titem max_s() const {
				return _max;
			}
		};

		// Iterator in gridding
		// ���������
		class iterator {
			typedef iterator _Tit;
		public :
			_Tidx _idx;
			const _Tself* _grid;

			iterator(const _Tself* grid) :
				_grid(grid) { }

			iterator(const _Tidx& idx, const _Tself* grid)
				: _idx(idx), _grid(grid) { }

			iterator(_Tidx&& idx, const _Tself* grid)
				: _idx(idx), _grid(grid) { }

			iterator(const _Tit& it)
				: _idx(it._idx), _grid(it._grid) { }

			iterator(_Tit&& it)
				: _idx(it._idx), _grid(it._grid) { }

			_Tv& operator * () {
				z_size_t c = 0;
				for (z_size_t i = 0; i < d_dim - 1; i++)
					c += _idx[i] * _grid->_data[i].len();
				c += _idx[d_dim - 1];
				return _grid->_root[c];
			}

			iterator& operator ++ () {
				for (z_size_t i = 0; i < d_dim; i++) {
					if ((_idx[i] + 1) < _grid->_data[i].len()) {
						_idx[i]++;
						goto end;
					}
					_idx[i] = 0;
				}
				_idx[d_dim] = 1;
			end :
				return *this;
			}

			bool operator != (const _Tit& it) const {
				return _idx != it._idx;
			}
		};

		///** Result **///

		// root node
		// ���ڵ�
		_Tv * _root;

		///** Data and equation **///

		// Data making
		// ��������
		grid_data _data[d_dim];

		// equation in coordinate
		// ���꺯��
		std::function< _Titem(_Tval) > _funcs[dim - 1];

		///** coordinate Range and implicit function **///

		// Range of coordinate in implicit function
		// ��ʽ���������귶Χ
		grid_range _range[d_dim];

		// implicit function
		// ��ʽ����
		std::function< bool(_Tv) > _i_func;

	private :
		gridding() {
		}

	public :
		~gridding() {
			delete[] _root;
		}

		gridding(const grid_data data[d_dim], const std::function< _Titem(_Tval) > funcs[dim])
			: gridding() {
			z_size_t i = d_dim, c = 1;
			while (i--)
				_data[i] = data[i];

			i = dim;
			while (i--)
				_funcs[i] = funcs[i];

			i = d_dim;
			while (i--)
				c *= -data[i].len();

			_dis();
		}

		gridding(const std::initializer_list< grid_data > data, const std::initializer_list< std::function< _Titem(_Tval) > > funcs) {
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
			}

			_root = new _Tv[c];

			_dis();
		}

		gridding(const grid_range range[d_dim], const std::function< bool(_Tv) > i_func)
			: gridding() {
			z_size_t i = d_dim;
			while (i--)
				_range[i] = range[i];

			_i_func = i_func;

			_im_dis();
		}

		void _dis() {
			for (iterator it = begin(); it != end(); ++it) {
				_Tv _dt;
				_Tval val;
				// Getting params for coordinate function;
				// ��ȡ���귽�̲���
				for (z_size_t i = 0; i < d_dim; i++) {
					val[i] = _data[i][it._idx[i]];
				}
				// Sent params into coordinate function to compute.
				// ���������������귽�̽��м���
				for (z_size_t i = 0; i < dim - 1; i++) {
					_dt[i] = _funcs[i](val);
				}
				// Set 1 because it is the dot.
				// ���ýڵ�Ϊ��Ԫ��
				_dt[dim - 1] = 1;
				(*it) = _dt;
			}
		}

		void _im_dis() {
			// TODO;
		}

		///*** Iterator ***///

		// iterator begin, the index is { 0, 0, 0, 0 } in 3D gridding etc.
		// ������ʼλ�ã�����3D��������Ϊidx{ 0, 0, 0, 0 }
		iterator begin() const {
			return iterator(_Tidx(), this);
		}

		// iterator end, the index is { 0, 0, 0, 1 } in 3D gridding etc.
		// ��������λ�ã�����3D����Ϊidx{ 0, 0, 0, 1 }
		iterator end() const {
			iterator _it(_Tidx(), this);
			_it._idx[d_dim] = 1;

			return std::move(_it);
		}
	};
}

#endif // !ZGL_GRIDDING
