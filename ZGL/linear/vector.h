#include "../inc.h"
#include "matrix.h"
#include "square.h"
#include <math.h>
#include <initializer_list>

#ifndef ZGL_VECTOR
#define ZGL_VECTOR

namespace ZGL {

	/// Vector
	/// 向量
	///
	/// dim: Dimensions of vector
	///       向量维度
	/// Titem: Type of item within vector or dot
	///       向量项类型
	template < z_size_t dim, typename Titem, typename = void >
	class vector
		: public matrix < 1, dim, Titem > {
		typedef Titem _Titem;
		typedef vector < dim, _Titem > _Tself;
		typedef matrix < 1, dim, _Titem > _Tbase;
		typedef square < dim, _Titem > _Tsquare;

	public:
		~vector() { }

		// default constructor
		// 默认构造
		vector()
			: _Tbase() { }
		
		// constructor that using item array
		// 使用数组的构造函数
		vector(const _Titem src[dim])
			: _Tbase() {
			for (z_size_t i = 0; i < dim; i++)
				v[0][i] = src[i];
		}

		// constructor that using type of itself
		// copy constructor
		// 拷贝构造函数
		vector(const _Tself& src)
			: _Tbase(src) { }

		// constructor that using Rvalue params
		// 使用右值构造
		vector(_Tself&& src)
			: _Tbase(src) { }

		// constructor that using initializer_list in cpp-11
		// 使用C++11的初始化列表
		vector(std::initializer_list< _Titem > src)
			: _Tbase({ src }) { }

		_Titem& operator [] (const z_size_t opt) const {
			return const_cast< _Titem& >(v[0][opt]);
		}

		virtual _Tself& operator = (const _Tself& opt) {
			for (z_size_t i = 0; i < dim; i++)
				v[0][i] = opt[i];
			return *this;
		}

		virtual _Tself& operator = (_Tself&& src) {
			v = src.v;
			src.v = nullptr;

			return *this;
		}

		bool operator == (const _Tself& opt) const {
			return (*(const _Tbase*)this) == opt;
		}

		bool operator != (const _Tself& opt) const {
			return (*(const _Tbase*)this) != opt;
		}

		_Tself operator + (const _Tself& opt) const {
			return (_Tself&&)(*(const _Tbase*)this + opt);
		}

		_Tself operator - (const _Tself& opt) const {
			return (_Tself&&)(*(const _Tbase*)this - opt);
		}

		_Tself operator * (const _Titem opt) const {
			return (_Tself&&)(*(const _Tbase*)this * opt);
		}

		template < z_size_t rows >
		vector < rows, _Titem > operator * (const matrix < dim, rows, _Titem >& opt) const {
			return (vector< rows, _Titem >&&)(*(const _Tbase *)this * opt);
		}

		_Tself operator / (const _Titem& opt) const {
			return (_Tself&&)(*(const _Tbase*)this / opt);
		}

		_Tself operator / (const _Tself& opt) const {
			return (_Tself&&)(*(const _Tbase*)this / opt);
		}

		_Titem operator PRO_DOT (const _Tself& opt) const {
			return (*this * _Tbase::transpose(opt))[0];
		}

		_Tself operator PRO_PARALLEL (const _Tself& opt) const {
			return opt * ((*this PRO_DOT opt) / (opt PRO_DOT opt));
		}

		_Tself operator PRO_UPRIGHT (const _Tself& opt) const {
			return *this - (*this PRO_PARALLEL opt);
		}

		// Vector module
		// 向量长度(模)
		_Titem module() const {
			return sqrt(*this PRO_DOT *this);
		}

		// The angle between the both of vector
		// 与向量的夹角
		_Titem angle(const _Tself& opt) const {
			return acos((*this PRO_DOT opt) / (this->module() * opt.module()));
		}

		// Cross product
		// only 3 dimension and 7 dimension
		// 向量积
		// 仅用于三维或七维
		static _Tself cross(const _Tself opt[dim - 1]) {
			if (dim != 3 && dim != 7)
				throw "";

			_Tsquare _t[dim][dim];

			for (z_size_t i = 0; i < dim; i++)
				for (z_size_t j = 0; j < dim; j++)
					if (0 == i) {
						_Tsquare _m(ZERO);
						for (z_size_t k = 0; k < dim; k++)
							_m[j][k] = _Titem(1);

						_t[i][j] = std::move(_m);
					} else {
						_Tsquare _m(ZERO);
						for (z_size_t k = 0; k < dim; k++)
							_m[k][k] = opt[i - 1][j];

						_t[i][j] = std::move(_m);
					}

			square< dim, _Tsquare > _ms(_t);
			_Tsquare _rs = _ms.determinant();

			_Tself _ft;
			for (z_size_t i = 0; i < dim; i++)
				_ft[i] = _rs[i][i];

			return std::move(_ft);
		}

		// Cross product
		// only 3 dimension and 7 dimension
		// 向量积
		// 仅用于三维或七维
		static _Tself cross(const std::initializer_list< _Tself >& opt) {
			_Tself _t[dim - 1];
			z_size_t i = 0;
			for (const _Tself& it : opt) {
				_t[i] = it;
				i++;
			}
			return cross(_t);
		}

		// determinant value for vector array
		// 向量组行列式值
		static _Titem determinant(const _Tself opt[dim]) {
			_Tsquare _m;

			z_size_t i = 0;
			for (z_size_t i = 0; i < dim; i++)
				for (z_size_t j = 0; j < dim; j++)
					_m[i][j] = opt[i][j];

			return _m.determinant();
		}

		// determinant value for vector array
		// 向量组行列式值
		static _Titem determinant(const std::initializer_list< _Tself >& opt) {
			_Tsquare _m;

			z_size_t i = 0;
			for (const _Tself& _r : opt) {
				for (z_size_t j = 0; j < dim; j++)
					_m[i][j] = _r[j];
				i++;
			}

			return _m.determinant();
		}

		// Vector Cofactor
		// 向量余子式
		template < z_size_t dim, typename _Titem >
		static vector < dim - 1, _Titem > cofactor(const vector < dim, _Titem > & opt, z_size_t c) {
			vector < dim - 1, _Titem > _t;
			z_size_t _mc = 0;
			for (z_size_t i = 0; i < dim; i++)
				if (i != c)
					_t[i - _mc] = opt[i];
				else
					_mc = 1;

			return std::move(_t);
		}
	};
}

#endif // !ZGL_VECTOR