#include "../inc.h"
#include <math.h>
#include <initializer_list>

#ifndef ZGL_MATRIX
#define ZGL_MATRIX

namespace ZGL {

	/// Matrix
	/// ����
	///
	/// rows: Number of matrix rows
	///       ��������
	/// cols: Number of matrix columns
	///       ��������
	/// Titem: Type of item within matrix
	///       ����������
	template < z_size_t rows, z_size_t cols, typename Titem, typename = void >
	class matrix {
		typedef Titem _Titem;
		typedef matrix< rows, cols, Titem > _Tself;

	protected :
		_Titem **v;

	public :
		~matrix() {
			if (v) {
				delete[] v[0];
				delete[] v;
			}
		}
		// default constructor as initializer as zero matrix
		// Ĭ�Ϲ����ʼ�������
		matrix() {
			v = new _Titem*[rows];
			v[0] = new _Titem[rows * cols];
			for (z_size_t i = 1; i < rows; i++)
				v[i] = v[i - 1] + cols;
			z_size_t i = rows * cols;
			while (i--)
				v[0][i] = 0;
		}

		// constructor that using item array
		// ʹ������Ĺ��캯��
		template < typename = std::enable_if_t < (rows != 0) && (cols != 0) > >
		matrix(const _Titem src[rows][cols])
			: matrix() {
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					v[i][j] = src[i][j];
		}

		// constructor that using type of itself
		// copy constructor
		// �������캯��
		matrix(const _Tself& src)
			: matrix() {
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					v[i][j] = src[i][j];
		}

		// constructor that using Rvalue params
		// ʹ����ֵ����
		matrix(matrix&& src) {
			v = src.v;
			src.v = nullptr;
		}

		// constructor that using initializer_list in cpp-11
		// ʹ��C++11�ĳ�ʼ���б�
		matrix(const std::initializer_list< std::initializer_list< _Titem > >& src)
			: matrix() {
			z_size_t i = 0;
			for (const std::initializer_list< _Titem >& _s : src) {
				for (const _Titem& _t : _s) {
					v[0][i] = _t;
					i++;
				}
			}
		}

		// constructor that using 4 submatrix to initialize
		// ʹ���ĸ��Ӿ��������
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
							v[i][j] = m11[i][j];
						else
							v[i][j] = m12[i][j - cols_u];
					else if (j < cols_u)
						v[i][j] = m21[i - rows_s][j];
					else
						v[i][j] = m22[i - rows_s][j - cols_u];
		}

		_Titem* operator [] (z_size_t opt) const {
			return v[opt];
		}

		virtual const _Tself& operator = (const _Tself& src) {
			z_size_t i = rows * cols;
			while (i--)
				v[0][i] = src[0][i];

			return *this;
		}

		virtual const _Tself& operator = (_Tself&& src) {
			v = src.v;
			src.v = nullptr;

			return *this;
		}

		template < typename _Titem >
		bool operator == (const matrix < rows, cols, _Titem >& opt) const {
			z_size_t i = rows * cols;
			while (i--)
				if (v[0][i] != opt[0][i])
					return false;

			return true;
		}

		template <>
		bool operator == (const matrix < rows, cols, float >& opt) const {
			z_size_t i = rows * cols;
			while (i--)
				if (!floating_eq(v[0][i], opt[0][i]))
					return false;

			return true;
		}

		template <>
		bool operator == (const matrix < rows, cols, double >& opt) const {
			z_size_t i = rows * cols;
			while (i--)
				if (!floating_eq(v[0][i], opt[0][i]))
					return false;

			return true;
		}

		template <>
		bool operator == (const matrix < rows, cols, long double >& opt) const {
			z_size_t i = rows * cols;
			while (i--)
				if (!floating_eq(v[0][i], opt[0][i]))
					return false;

			return true;
		}


		bool operator != (const _Tself& opt) const {
			return !(*this == opt);
		}

		_Tself operator + (const _Tself& opt) const {
			_Tself _t;
			z_size_t i = rows * cols;
			while (i--)
				_t[0][i] = v[0][i] + opt[0][i];

			return std::move(_t);
		}

		_Tself operator - (const _Tself& opt) const {
			_Tself _t = *this + opt * -1;
			return std::move(_t);
		}

		_Tself operator * (const _Titem& opt) const {
			_Tself _t;
			z_size_t i = rows * cols;
			while (i--)
				_t[0][i] = v[0][i] * opt;

			return std::move(_t);
		}

		template < z_size_t n_cols >
		matrix < rows, n_cols, _Titem > operator * (const matrix < cols, n_cols, _Titem >& opt) const {
			matrix < rows, n_cols, _Titem > _t;
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < n_cols; j++)
					for (z_size_t k = 0; k < cols; k++)
						_t[i][j] = _t[i][j] + v[i][k] * opt[k][j];

			return std::move(_t);
		}

		_Tself operator / (const _Titem& opt) const {
			return (_Tself&&)(*this * (_Titem(1) / opt));
		}

		_Tself operator / (const _Tself& opt) const {
			throw "";
		}

		// matrix transpose
		// ����ת��
		template < z_size_t rows, z_size_t cols >
		static matrix < cols, rows, _Titem > transpose (const matrix < rows, cols, _Titem >& opt) {
			matrix < cols, rows, _Titem > _t;
			for (z_size_t i = 0; i < rows; i++)
				for (z_size_t j = 0; j < cols; j++)
					_t[j][i] = opt[i][j];

			return std::move(_t);
		}

		// Cofactor
		// ����ʽ
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

			return std::move(_t);
		}
	};

	template < z_size_t rows, z_size_t cols, typename Titem >
	class matrix < rows, cols, Titem, typename std::enable_if_t < (rows < 0) || (cols < 0) > > { };
}

#endif // !ZGL_MATRIX