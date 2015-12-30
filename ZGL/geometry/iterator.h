#include "../inc.h"
#include "../linear/vector.h"

#ifndef ZGL_ITERATOR
#define ZGL_ITERATOR

_ZGL_BEGIN

	// Iterator in gridding
	// 网格迭代器
	/// dim: iterator's dimension
	///	      迭代器维度
	/// Tv: Type of element
	///       元素类型
	template < z_size_t dim, typename Tv = size_t >
	class iterator {
		// The rank of index vector
		// 索引向量的秩
		const static z_size_t idx_rank = dim + 1;

		typedef vector< idx_rank, z_size_t > _Tidx;
		typedef iterator< dim, Tv > _Tself;

	public:
		typedef _Tidx Tidx;

	public:
		_Tidx _idx;
		_Tidx _max;
		Tv* _root;

		_Tself(z_size_t max = 0, Tv* root = nullptr)
			: _idx(), _max(), _root(root) {
			z_size_t i = dim;
			while (i--) {
				_max[i] = max;
			}
		}

		_Tself(const _Tidx& max, Tv* root)
			: _idx(), _max(max), _root(root) { }

		_Tself(const _Tidx& idx, const _Tidx& max, Tv* root)
			: _idx(idx), _max(max), _root(root) { }

		_Tself(_Tidx&& idx, const _Tidx& max, Tv* root)
			: _idx(idx), _max(max), _root(root) { }

		_Tself(const _Tself& it)
			: _idx(it._idx), _max(it._max), _root(it._root) { }

		_Tself(_Tself&& it)
			: _idx(STD_MOVE(it._idx)), _max(STD_MOVE(it._max)), _root(STD_MOVE(it._root)) { }

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

			return *this;
		}

		// The iterator addition
		// 迭代器相加
		_Tself operator + (const _Tself& opt) const {
			_Tself _t(*this);

			_t._idx = _idx + opt._idx;
			for (z_size_t i = 0; i < dim; i++) {
				if (_t._idx[i] > _t._max[i]) {
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

		// Xor
		// 异或
		_Tself operator ^ (const _Tself& opt) const {
			_Tself _t(_max, _root);
			for (z_size_t i = 0; i < idx_rank; i++) {
				_t._idx[i] = (_idx[i] != opt._idx[i] ? _Titem(1) : _Titem(0));
			}

			return STD_MOVE(_t);
		}

		_Tself& operator ^= (const _Tself& opt) {
			(*this) = (*this) ^ opt;
			return *this;
		}

		// Position of current index in collective object
		// 当前索引在集合对象的位置
		z_size_t operator & () const {
			z_size_t c = 0, _t;
			// 计数器进制
			for (z_size_t i = 0; i < dim; i++) {
				_t = _idx[i];
				// 如果不是个位数，则循环累乘低位进制
				if (i) {
					for (z_size_t j = (i - 1); j >= 0; --j) {
						_t *= _max[j];
					}
				}
				c += _t;
			}

			return c;
		}

		const Tv& operator *() const {
			auto c = &(*this);
			return _root[c];
		}

		// Value of current index in collective object
		// 当前索引在集合对象的取值
		Tv& operator *() {
			/*
			z_size_t c = 0, _t;
			// 计数器进制
			for (z_size_t i = 0; i < dim; i++) {
				_t = _idx[i];
				// 如果不是个位数，则循环累乘低位进制
				if (i) {
					for (z_size_t j = (i - 1); j >= 0; j--) {
						_t *= _max[j];
					}
				}
				c += _t;
			}
			*/
			auto c = &(*this);

			return _root[c];
		}

		// Index incresing
		// 索引自增
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

		_Tself operator ++ (int) {
			_Tself _t(*this);
			++(*this);
			return STD_MOVE(_t);
		}

		bool operator != (const _Tself& it) const {
			return _idx != it._idx;
		}

		bool operator == (const _Tself& it) const {
			return _idx == it._idx;
		}

		operator bool() const {
			return _idx == _Tidx();
		}

		bool operator < (const _Tself& it) const {
			z_size_t i = idx_rank;
			while (i--) {
				if (_idx[i] < it._idx[i]) {
					return true;
				} else if (_idx[i] > it._idx[i]) {
					return false;
				}
			}

			return false;
		}

		bool operator > (const _Tself& it) const {
			return (*this) != it && !((*this) < it);
		}

		_Tself begin() const {
			_Tself _t(_max, _root);
			return STD_MOVE(_t);
		}

		_Tself end() const {
			_Tself _t(_max, _root);
			_t._idx[dim] = 1;
			return STD_MOVE(_t);
		}
	};

_ZGL_END

#endif // !ZGL_ITERATOR
