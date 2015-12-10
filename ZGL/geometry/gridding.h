#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>
#include <vector>

#ifndef ZGL_GRIDDING
#define ZGL_GRIDDING

_ZGL_BEGIN
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

		typedef iterator< d_dim, _Tv > _Tit;
		typedef typename _Tit::Tidx _Tidx;

	public :
		// Equation in coordinate' argument type
		// ���꺯����������
		typedef _Tval Targ;
		// Iterator values' type
		// ������ֵ����
		typedef _Tv Tval;
		// Iterator indexs' type
		// ��������������
		typedef _Tidx Tidx;

	public :
		// segment result of gridding
		// ���νṹ
		struct segment{
			_Tv first;
			_Tv last;

			segment() { }

			segment(const _Tv& sFrist, const _Tv& sLast)
				: first(sFrist), last(sLast) { }
		};

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
		std::function< _Titem(_Tval, z_size_t) > _funcs[dim - 1];

		_Tidx _max;

	private :
		gridding() { }

	public :
		~gridding() {
			delete[] _root;
		}

		gridding(const grid_data data[d_dim], const std::function< _Titem(_Tval, z_size_t = 0) > funcs[dim - 1])
			: gridding() {
			z_size_t i = d_dim, c = 1;
			while (i--)
				_data[i] = data[i];

			i = dim - 1;
			while (i--)
				_funcs[i] = funcs[i];

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
			
			i = dim - 1;
			while (i--)
				_funcs[i] = src._funcs[i];

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
			while (i--)
				_data[i] = src._data[i];

			i = dim - 1;
			while (i--)
				_funcs[i] = src._funcs[i];

			i = d_dim;
			while (i--) {
				c *= _data[i].len();
				_max[i] = _data[i].len();
			}

			_root = new _Tv[c];

			_dis();

			return *this;
		}

		void _dis() {
			for (auto it = begin(); it != end(); ++it) {
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
					_dt[i] = _funcs[i](val, i);
				}
				// Set 1 because it is the dot.
				// ���ýڵ�Ϊ��Ԫ��
				_dt[dim - 1] = 1;
				(*it) = _dt;
			}
		}

		_Tv operator [] (_Tidx idx) {
			z_size_t c = 1;
			for (z_size_t i = 0; i < d_dim; i++)
				c *= idx[d_dim];
		}

		///*** Iterator ***///

		// iterator begin, the index is { 0, 0, 0, 0 } in 3D gridding etc.
		// ������ʼλ�ã�����3D��������Ϊidx{ 0, 0, 0, 0 }
		_Tit begin() const {
			return _Tit(_max, _root);
		}

		// iterator end, the index is { 0, 0, 0, 1 } in 3D gridding etc.
		// ��������λ�ã�����3D����Ϊidx{ 0, 0, 0, 1 }
		const _Tit end() const {
			_Tit _it(_max, _root);
			_it._idx[d_dim] = 1;

			return STD_MOVE(_it);
		}

		void each(std::vector< segment >& res) const {
			for (_Tit it = begin(); it != end(); ++it) {
				for (z_size_t d = 0; d < d_dim; d++) {
					_Tidx _idx = it._idx;
					_idx[d] = _idx[d] + 1;
					if (_idx[d] < _max[d]) {
						res.push_back(segment(*it, *_Tit(_idx, _max, _root)));
					}
				}
			}
		}
	};
_ZGL_END

#endif // !ZGL_GRIDDING
