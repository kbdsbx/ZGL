#include "Isolid.h"
#include <iterator>

#ifndef ZGL_ISEPARABLE
#define ZGL_ISEPARABLE

_ZGL_BEGIN
	// Iterator in gridding
	// ���������
	/// d_dim: elements' dimension
	///	      Ԫ��ά��
	/// Tv: Type of element
	///       Ԫ������
	template < z_size_t d_dim, typename Tv >
	class iterator
		: public std::iterator< std::input_iterator_tag, Tv > {
		typedef iterator _Tit;
		typedef vector< d_dim + 1, z_size_t > _Tidx;
	public :
		typedef _Tidx Tidx;
	public :
		_Tidx _idx;
		_Tidx _max;
		Tv* _root;

		iterator(const _Tidx& max, Tv* root) :
			_max(max), _root(root) { }

		iterator(const _Tidx& idx, const _Tidx& max, Tv* root)
			: _idx(idx), _max(max), _root(root) { }

		iterator(_Tidx&& idx, const _Tidx& max, Tv* root)
			: _idx(idx), _max(max), _root(root) { }

		iterator(const _Tit& it)
			: _idx(it._idx), _max(it._max), _root(it._root) { }

		iterator(_Tit&& it)
			: _idx(it._idx), _max(it._max), _root(it._root) { }

		Tv& operator * () {
			z_size_t c = 0, _t;
			// ����������
			for (z_size_t i = 0; i < d_dim; i++) {
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

		iterator& operator ++ () {
			for (z_size_t i = 0; i < d_dim; i++) {
				if ((_idx[i] + 1) < _max[i]) {
					_idx[i]++;
					goto end;
				}
				_idx[i] = 0;
			}
			_idx[d_dim] = 1;
		end:
			return *this;
		}

		bool operator != (const _Tit& it) const {
			return _idx != it._idx;
		}
	};

	// Separable interface
	// �������񻯵�
	class Iseparable
		: public Isolid {
	};
}

#endif // !ZGL_ISEPARABLE
