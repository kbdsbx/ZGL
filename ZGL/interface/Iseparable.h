#include "Isolid.h"
#include <iterator>

#ifndef ZGL_ISEPARABLE
#define ZGL_ISEPARABLE

namespace ZGL {
	// Iterator in gridding
	// 网格迭代器
	/// d_dim: elements' dimension
	///	      元素维度
	/// Tv: Type of element
	///       元素类型
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
			z_size_t c = 0;
			for (z_size_t i = 0; i < d_dim - 1; i++)
				c += _idx[i] * _max[i];
			c += _idx[d_dim - 1];
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
	// 允许被网格化的
	class Iseparable
		: public Isolid {
	};
}

#endif // !ZGL_ISEPARABLE
