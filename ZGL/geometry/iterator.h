#include "../inc.h"

#ifndef ZGL_ITERATOR
#define ZGL_ITERATOR

_ZGL_BEGIN

	// Iterator in gridding
	// ���������
	/// dim: elements' dimension
	///	      Ԫ��ά��
	/// Tv: Type of element
	///       Ԫ������
	template < z_size_t dim, typename Tv >
	class iterator
		: public std::iterator< std::input_iterator_tag, Tv > {
		typedef vector< dim + 1, z_size_t > _Tidx;
		typedef iterator< dim, Tv > _Tself;
	public:
		typedef _Tidx Tidx;
	public:
		_Tidx _idx;
		_Tidx _max;
		Tv* _root;

		_Tself(const _Tidx& max, Tv* root) :
			_max(max), _root(root) { }

		_Tself(const _Tidx& idx, const _Tidx& max, Tv* root)
			: _idx(idx), _max(max), _root(root) { }

		_Tself(_Tidx&& idx, const _Tidx& max, Tv* root)
			: _idx(idx), _max(max), _root(root) { }

		_Tself(const _Tself& it)
			: _idx(it._idx), _max(it._max), _root(it._root) { }

		_Tself(_Tself&& it)
			: _idx(it._idx), _max(it._max), _root(it._root) { }

		_Tself& operator = (const _Tself& src) {
			_root = src._root;
			_max = src._max;
			_idx = src._idx;

			return *this;
		}

		_Tself& operator = (_Tself&& src) {
			_root = STD_MOVE(src._root);
			_max = STD_MOVE(src._max);
			_idx = STD_MOVE(src._idx);
		}

		// The iterator addition
		// ���������
		_Tself operator + (const _Tself& opt) const {
			_Tself _t(*this);

			_t._idx = _idx + opt._idx;
			for (z_size_t i = 0; i < dim; i++) {
				if (_t._idx[i] >= _t._max[i]) {
					// �����λ ?
					throw std::out_of_range("The index of iterator is out of range.");
				}
			}

			return STD_MOVE(_t);
		}

		_Tself& operator += (const _Tself& opt) {
			(*this) = (*this) + opt;
			return *this;
		}

		_Tself operator ^ (const _Tself& opt) const {
			_Tself _t(_max, _root);
			for (z_size_t i = 0; i < dim + 1; i++) {
				_t._idx[i] = (_idx[i] != opt._idx[i] ? _Titem(1) : _Titem(0));
			}

			return STD_MOVE(_t);
		}

		_Tself& operator ^= (const _Tself& opt) {
			(*this) = (*this) ^ opt;
			return *this;
		}

		// Value for iterator
		// ������ȡֵ
		Tv& operator * () {
			z_size_t c = 0, _t;
			// ����������
			for (z_size_t i = 0; i < dim; i++) {
				_t = _idx[i];
				// ������Ǹ�λ������ѭ���۳˵�λ����
				if (i) {
					for (z_size_t j = (i - 1); j >= 0; j--) {
						_t *= _max[j];
					}
				}
				c += _t;
			}

			return _root[c];
		}

		// Index incresing
		// ��������
		_Tself& operator ++ () {
			for (z_size_t i = 0; i < dim; i++) {
				if ((_idx[i] + 1) < _max[i]) {
					_idx[i]++;
					goto end;
				}
				_idx[i] = 0;
			}
			_idx[dim] = 1;
		end:
			return *this;
		}

		bool operator != (const _Tself& it) const {
			return _idx != it._idx;
		}

		bool operator == (const _Tself& it) const {
			return _idx == it._idx;
		}

		operator bool() const {
			return _idx == Tv();
		}
	};

_ZGL_END

#endif // !ZGL_ITERATOR
