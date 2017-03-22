#include "../inc.h"
#include "matrix.h"
#include <math.h>
#include <initializer_list>
#include <ctgmath>

#ifndef ZGL_SQUARE
#define ZGL_SQUARE

_ZGL_BEGIN

	/// Square matrix
	/// 方阵
	///
	/// dim: Dimensions of square matrix
	///       矩阵行列数
	/// Titem: Type of item within square matrix
	///       矩阵项类型
	template < z_size_t dim, typename Titem, typename = void >
	class square
		: public matrix < dim, dim, Titem > {
		typedef Titem _Titem;
		typedef square < dim, _Titem > _Tself;
		typedef matrix < dim, dim, _Titem > _Tbase;

	public:
		~square() { }

		// With or without a parameter to constructed identity square matrix or zero square matrix
		// 使用或不使用参数构造单位矩阵或零矩阵
		square(z_size_t opt = (z_size_t)ZGL_SQUARE_MATRIX_TYPE::ZERO)
			: _Tbase() {
			_Titem t = _Titem((z_size_t)opt);
			z_size_t i = dim;
			while (i--)
				v[i][i] = t;
		}

		// constructor that using item array
		// 使用数组的构造函数
		template < typename = std::enable_if_t < (dim > 0) > >
		square(const _Titem src[dim][dim])
			: _Tbase(src) { }

		// constructor that using type of itself
		// copy constructor
		// 拷贝构造函数
		square(const _Tself& src)
			: _Tbase(src) { }

#ifdef ZGL_ENABLE_RVALUE
		// constructor that using Rvalue params
		// 使用右值构造
		square(_Tself&& src)
			: _Tbase(src) { }
#endif

		// constructor that using initializer_list in cpp-11
		// 使用C++11的初始化列表
		square(const std::initializer_list< std::initializer_list< _Titem > >& src)
			: _Tbase(src) { }

		// constructor that using 4 submatrix to initialize
		// 使用四个子矩阵构造矩阵
		template < z_size_t rows_s, z_size_t rows_t, z_size_t cols_u, z_size_t cols_v, typename = std::enable_if_t < (rows_s + rows_t) == dim && (cols_u + cols_v) == dim > >
		square(const matrix < rows_s, cols_u, _Titem > & m11,
			const matrix < rows_s, cols_v, _Titem > & m12,
			const matrix < rows_t, cols_u, _Titem > & m21,
			const matrix < rows_t, cols_v, _Titem > & m22)
			: _Tbase(m11, m12, m21, m22) { }

		_Titem* operator [] (z_size_t opt) const {
			return (*(_Tbase*)this)[opt];
		}

		virtual _Tself& operator = (const _Tself& src) {
			for (z_size_t i = 0; i < dim; i++)
				for (z_size_t j = 0; j < dim; j++)
					v[i][j] = src[i][j];

			return *this;
		}

#ifdef ZGL_ENABLE_RVALUE
		virtual const _Tself& operator = (_Tself&& src) {
			(*(const _Tbase*)this) == opt;
			return *this;
		}
#endif

		bool operator == (const _Tself& opt) const {
			return (*(const _Tbase*)this) == opt;
		}

		bool operator != (const _Tself& opt) const {
			return (*(const _Tbase*)this) != opt;
		}

		_Tself operator + (const _Tself& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this + opt));
		}

		_Tself operator - (const _Tself& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this - opt));
		}

		_Tself operator * (const _Titem& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this * opt));
		}

		_Tself operator * (const _Tself& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this * opt));
		}

		_Tself operator / (const _Titem& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this / opt));
		}

		_Tself operator / (const _Tself& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this / opt));
		}

		_Tself operator ^ (int opt) const {
			if (opt == 0) {
				return _Tself(ZGL_SQUARE_MATRIX_TYPE::IDENTITY);
			} else {
				_Tself _s(*this);
				for (z_size_t i = 1; i < (z_size_t)abs(opt); i++)
					_s = _s * *this;
				return STD_MOVE(opt > 0 ? _s : _Tself::inverse(_s));
			}
		}

		// value of determinant as from as square matrix
		// 方阵行列式值
		_Titem determinant() const {
			return _Tself::determinant(*this);
		}

		// matrix transpose
		// 矩阵转置
		const _Tself& transpose() {
			return (*this = _Tself::transpose(*this));
		}

		// determinant value for square matrix
		// 方阵行列式值
		template < z_size_t dim, typename _Titem >
		static _Titem determinant (const square < dim, _Titem > & opt) {
			_Titem _v = _Titem(0);
			for (z_size_t i = 0; i < dim; i++) {
				_v = _v + (opt[0][i] * square::determinant(square::cofactor(opt, 1, i + 1)) * _Titem(pow(-1, 2 + i)));
			}
			return _v;
		}

		// determinant value for square matrix
		// 方阵行列式值
		template <>
		static _Titem determinant (const square < 1, _Titem > & opt) {
			return opt[0][0];
		}

		// matrix transpose
		// 矩阵转置
		template < z_size_t dim, typename _Titem >
		static square < dim, _Titem > transpose(const square < dim, _Titem >& opt) {
			return STD_MOVE((_Tself&)_Tbase::transpose(opt));
		}

		// Cofactor
		// 余子式
		// r, c : Base in 1
		template < z_size_t dim, typename _Titem >
		static square < dim - 1, _Titem > cofactor(const square < dim, _Titem > & opt, z_size_t r, z_size_t c) {
			square < dim - 1, _Titem > _t;
			for (z_size_t i = 0, _mr = 0; i < dim; i++)
				if (i != (r - 1))
					for (z_size_t j = 0, _mc = 0; j < dim; j++)
						if (j != (c - 1))
							_t[i - _mr][j - _mc] = opt[i][j];
						else
							_mc = 1;
				else
					_mr = 1;

			return STD_MOVE(_t);
		}

		// Matrix inverse
		// 矩阵求逆
		template < z_size_t dim, typename _Titem >
		static square < dim, _Titem > inverse(const square < dim, _Titem > & opt) {
			_Titem _det = opt.determinant();
			if (_det != _Titem(0)) {
				_Tself _t;
				for (z_size_t i = 0; i < dim; i++)
					for (z_size_t j = 0; j < dim; j++)
						_t[j][i] = determinant(cofactor(opt, i + 1, j + 1)) * (z_size_t)pow(-1.0, i + j + 2);

				_t = _t * (_Titem(1) / _det);
				return STD_MOVE(_t);
			} else {
				throw "";
			}
		}

		template < typename _Titem >
		static square < 1, _Titem > inverse(const square < 1, _Titem > & opt) {
			_Titem _det = opt.determinant();
			if (_det != _Titem(0))
				return opt * (_Titem(1) / _det);
			else
				throw "";
		}

		template <>
		static square < 0, _Titem > cofactor(const square < 1, _Titem > & opt, z_size_t r, z_size_t c) {
			throw "";
		}
	};

	template < z_size_t dim, typename Titem >
	class square < dim, Titem, typename std::enable_if_t < (dim < 0) > > {
		typedef Titem _Titem;
	public :
		_Titem* operator [] (z_size_t opt) const {
			throw "";
		}
	};

_ZGL_END

#endif // !ZGL_SQUARE