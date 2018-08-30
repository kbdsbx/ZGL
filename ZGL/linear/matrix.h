#include "../inc.h"
#include <cmath>
#include <initializer_list>
#include <array>

#ifndef ZGL_MATRIX
#define ZGL_MATRIX

_ZGL_BEGIN

	/// Matrix
	/// 矩阵
	///
	/// rows: Number of matrix rows
	///       矩阵行数
	/// cols: Number of matrix columns
	///       矩阵列数
	/// Titem: Type of item within matrix
	///       矩阵项类型
	template < z_size_t rows, z_size_t cols, typename Titem, typename = void >
	class matrix {
		typedef Titem _Titem;
		typedef matrix< rows, cols, Titem > _Tself;

	protected :
#ifdef ZGL_DISABLE_RVALUE
		_Titem v[rows][cols] = { };
#endif
#ifdef ZGL_ENABLE_RVALUE
		_Titem *v;
#endif

	public :
		~matrix() {
#ifdef ZGL_ENABLE_RVALUE
			if (v) {
				delete[] v;
			}
#endif
		}
		// default constructor as initializer as zero matrix
		// 默认构造初始化零矩阵
		matrix() {
#ifdef ZGL_ENABLE_RVALUE
			v = new _Titem[rows * cols]{ _Titem(0) };
#endif
		}

#ifdef ZGL_ENABLE_RVALUE
		// move constructor
		// 转移构造
		matrix(matrix&& src) {
			v = src.v;
			src.v = nullptr;
		}
#endif

		// constructor from array
		// 使用数组的构造函数
		template < typename = std::enable_if_t < (rows != 0) && (cols != 0) > >
		matrix(const _Titem src[rows][cols])
			: matrix() {
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					(*this)[i][j] = src[i][j];
		}

		// copy constructor
		// 拷贝构造函数
		matrix(const _Tself& src)
			: matrix() {
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					(*this)[i][j] = src[i][j];
		}

		// constructor from initializer_list
		// 使用C++11的初始化列表
		matrix(const std::initializer_list< std::initializer_list< _Titem > >& src)
			: matrix() {
			z_size_t i = 0;
			for (const std::initializer_list< _Titem >& _s : src) {
				z_size_t j = 0;
				for (const _Titem& _t : _s) {
					(*this)[i][j] = _t;
					j++;
				}
				i++;
			}
		}

		// constructor form sub matrix
		// 使用四个子矩阵构造矩阵
		template < z_size_t rows_s, z_size_t rows_t, z_size_t cols_u, z_size_t cols_v, typename = std::enable_if_t < (rows_s + rows_t) == rows && (cols_u + cols_v) == cols > >
		matrix(const matrix < rows_s, cols_u, _Titem > & m11,
			const matrix < rows_s, cols_v, _Titem > & m12,
			const matrix < rows_t, cols_u, _Titem > & m21,
			const matrix < rows_t, cols_v, _Titem > & m22)
			: matrix() {
			for (z_size_t i = 0; i < rows_s + rows_t; i++)
				for (z_size_t j = 0; j < cols_u + cols_v; j++)
					if (i < rows_s)
						if (j < cols_u)
							(*this)[i][j] = m11[i][j];
						else
							(*this)[i][j] = m12[i][j - cols_u];
					else if (j < cols_u)
						(*this)[i][j] = m21[i - rows_s][j];
					else
						(*this)[i][j] = m22[i - rows_s][j - cols_u];
		}

#ifdef ZGL_ENABLE_RVALUE
		inline _Titem* operator [] (z_size_t opt) const {
			return v + cols * opt;
		}
#endif

#ifdef ZGL_DISABLE_RVALUE
		inline _Titem* operator [] (z_size_t opt) const {
			return (_Titem*)(v[opt]);
		}
#endif

		inline _Tself& operator = (const _Tself& src) {
			/*
			for (z_size_t i = 0; i < rows * cols; i++) {
				v[i] = src.v[i];
			}
			*/

			for (z_size_t i = 0; i < rows; i++) {
				for (z_size_t j = 0; j < cols; j++) {
					(*this)[i][j] = src[i][j];
				}
			}

			return *this;
		}

#ifdef ZGL_ENABLE_RVALUE
		inline _Tself& operator = (_Tself&& src) {
			if (this != &src) {
				delete[] v;

				v = src.v;
				src.v = nullptr;
			}
			return *this;
		}
#endif

		template < typename _Titem >
		inline bool operator == (const matrix < rows, cols, _Titem >& opt) const {
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					if (std::is_floating_point< _Titem >::value ? (!floating_eq< _Titem >((*this)[i][j], opt[i][j])) : ((*this)[i][j] != opt[i][j]))
						return false;

			return true;
		}

		inline bool operator != (const _Tself& opt) const {
			return !(*this == opt);
		}

		inline _Tself operator + (const _Tself& opt) const {
			_Tself _t;
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					_t[i][j] = (*this)[i][j] + opt[i][j];

			return STD_MOVE(_t);
		}

		inline _Tself operator - (const _Tself& opt) const {
			_Tself _t = *this + opt * -1;
			return STD_MOVE(_t);
		}

		inline _Tself operator * (const _Titem& opt) const {
			_Tself _t;
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					_t[i][j] = (*this)[i][j] * opt;

			return STD_MOVE(_t);
		}

		template < z_size_t n_cols >
		inline matrix < rows, n_cols, _Titem > operator * (const matrix < cols, n_cols, _Titem >& opt) const {
			matrix < rows, n_cols, _Titem > _t;
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < n_cols; j++)
					for (z_size_t k = 0; k < cols; k++)
						_t[i][j] = _t[i][j] + (*this)[i][k] * opt[k][j];

			return STD_MOVE(_t);
		}

		inline _Tself operator / (const _Titem& opt) const {
			return STD_MOVE((_Tself&)(*this * (_Titem(1) / opt)));
		}

		_Tself operator / (const _Tself& opt) const {
			throw "";
		}

		// matrix transpose
		// 矩阵转置
		template < z_size_t rows, z_size_t cols >
		static matrix < cols, rows, _Titem > transpose (const matrix < rows, cols, _Titem >& opt) {
			matrix < cols, rows, _Titem > _t;
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					_t[j][i] = opt[i][j];

			return STD_MOVE(_t);
		}

		// Cofactor
		// 余子式
		// rows_ct, cols_ct : base in 1
		template < z_size_t rows_ct = 0, z_size_t cols_ct = 0 >
		static matrix < rows - (rows_ct == 0 ? 0 : 1), cols - (cols_ct == 0 ? 0 : 1), _Titem > cofactor(const matrix < rows, cols, _Titem > & opt) {
			matrix < rows - (rows_ct == 0 ? 0 : 1), cols - (cols_ct == 0 ? 0 : 1), _Titem > _t;
			for (z_size_t i = 0, _mr = 0; i < rows; i++)
				if (i != (rows_ct - 1))
					for (z_size_t j = 0, _mc = 0; j < cols; j++)
						if (j != (cols_ct - 1))
							_t[i - _mr][j - _mc] = opt[i][j];
						else
							_mc = 1;
				else
					_mr = 1;

			return STD_MOVE(_t);
		}
	};

	template < z_size_t rows, z_size_t cols, typename Titem >
	class matrix < rows, cols, Titem, typename std::enable_if_t < (rows < 0) || (cols < 0) > > { };

_ZGL_END

#endif // !ZGL_MATRIX
