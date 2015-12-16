#include "../inc.h"

#ifndef ZGL_ITERATOR
#define ZGL_ITERATOR

_ZGL_BEGIN

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
		typedef iterator< d_dim, Tv > _Tself;
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

		_Tself(const _Tit& it)
			: _idx(it._idx), _max(it._max), _root(it._root) { }

		_Tself(_Tit&& it)
			: _idx(it._idx), _max(it._max), _root(it._root) { }

		_Tself& operator = (const _Tself& src) {
			_root = src._root;
			_max = src._max;
			_idx = src._idx;

			return *this;
		}

		_Tself operator + (const _Tself& opt) const {
			if (_root != opt._root) {
				throw "Can not adding two different iterators about iteratively object.";
			}

			_Tself _t(*this);

			_t._idx = _idx + opt._idx;
			for (z_size_t i = 0; i < d_dim; i++) {
				if (_t._idx[i] >= _t._max[i]) {
					// 不会进位 ?
					throw std::out_of_range("The index of iterator is out of range.");
				}
			}

			return STD_MOVE(_t);
		}

		_Tself& operator += (const _Tself& opt) {
			(*this) = (*this) + opt;
			return *this;
		}

		Tv& operator * () {
			z_size_t c = 0, _t;
			// 计数器进制
			for (z_size_t i = 0; i < d_dim; i++) {
				_t = _idx[i];
				// 如果不是个位数，则循环累乘低位进制
				if (i) {
					for (z_size_t j = (i - 1); j >= 0; j--) {
						_t *= _max[j];
					}
				}
				c += _t;
			}

			return _root[c];
		}

		_Tself& operator ++ () {
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

_ZGL_END

#endif // !ZGL_ITERATOR
