#include "../inc.h"
#include "../linear/vector.h"
#include "../geometry/graph.h"
#include <math.h>

#ifndef ZGL_AFFINE_VECTOR
#define ZGL_AFFINE_VECTOR

_ZGL_BEGIN

	/// The vector or dot that is in affine transfromations
	/// 仿射变换中的向量或点
	///
	/// dim: Dimensions of vector or dot
	///       向量或点维度
	/// Titem: Type of item within vector or dot
	///       向量项或点项类型
	template < z_size_t dim, typename Titem >
	class affine_vector
		: public vector< dim + 1, Titem > {
		typedef Titem _Titem;
		// The rank of base vector
		// 父级向量的秩
		const static z_size_t rank = dim + 1;
		typedef affine_vector< dim, _Titem > _Tself;
		typedef vector< rank, _Titem > _Tbase;
		typedef square< rank, _Titem > _Tsqu;
		typedef square< rank - 1, _Titem > _Tm11;
		typedef matrix< rank - 1, 1, _Titem > _Tm12;
		typedef matrix< 1, rank - 1, _Titem > _Tm21;
		typedef square< 1, _Titem > _Tm22;
		typedef graph< dim, dim - 1, _Titem > _Tpl;
		typedef graph< dim, dim - 2, _Titem > _Taxis;

	public :
		// default constructor
		// 默认构造
		_Tself()
			: _Tbase() { }

		// constructor that using item array
		// 使用数组的构造函数
		_Tself(const _Titem src[rank])
			: _Tbase(src) { }

		// constructor that using type of itself
		// copy constructor
		// 拷贝构造函数
		_Tself(const _Tself& src)
			: _Tbase(src) { }

#ifdef ZGL_ENABLE_RVALUE
		// constructor that using Rvalue params
		// 使用右值构造
		_Tself(_Tself&& src)
			: _Tbase(src) { }
#endif

		// constructor that using initializer_list in cpp-11
		// 使用C++11的初始化列表
		_Tself(std::initializer_list< _Titem > src)
			: _Tbase({ src }) { }

		_Titem& operator [] (z_size_t opt) const {
			return (*(const _Tbase*)this)[opt];
		}

		virtual _Tself& operator = (const _Tself& opt) {
			for (z_size_t i = 0; i < rank; i++)
				v[0][i] = opt[i];
			return *this;
		}

#ifdef ZGL_ENABLE_RVALUE
		virtual _Tself& operator = (_Tself&& src) {
			(*(_Tbase*)this = src);
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

		_Tself operator * (const _Titem opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this * opt));
		}

		_Tself operator / (const _Titem& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this / opt));
		}

		_Tself operator / (const _Tself& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this / opt));
		}

		template < z_size_t rows >
		_Tself operator * (const matrix < rank, rows, _Titem >& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this * opt));
		}

		_Titem operator PRO_DOT (const _Tself& opt) const {
			return *(const _Tbase*)this PRO_DOT opt;
		}

		_Tself operator PRO_PARALLEL (const _Tself& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this PRO_PARALLEL opt));
		}

		_Tself operator PRO_UPRIGHT (const _Tself& opt) const {
			return STD_MOVE((_Tself&)(*(const _Tbase*)this PRO_UPRIGHT opt));
		}

		// Vector module
		// 向量长度(模)
		_Titem module() const {
			return ((const _Tbase*)this)->module();
		}

		// The angle between the both of vector
		// 与向量的夹角
		_Titem angle(const _Tself& opt) const {
			return ((const _Tbase*)this)->angle(opt);
		}

		// normalize
		// 归一化
		void normalize() {
			(*this) = _Tself::normalize(*this);
		}

		// Cross product
		// 向量积，外积，混合积，楔积
		static _Tself cross(const _Tself opt[dim - 1]) {
			vector< rank - 1, _Titem > _t[dim - 1], _r;
			for (z_size_t i = 0; i < dim - 1; i++) {
				_t[i] = (vector< rank - 1, _Titem >)affine_vector< dim, _Titem >::cofactor(opt[i], rank - 1);
			}
			_r = vector< rank - 1, _Titem >::cross(_t);
			return STD_MOVE(_Tself::affine(_r, _Titem(0)));
		}

		// Cross product
		// 向量积，外积，混合积，楔积
		static _Tself cross(const std::initializer_list< _Tself >& opt) {
			_Tself _t[dim - 1];
			z_size_t i = 0;
			for (const _Tself& o : opt) {
				_t[i] = o;
				i++;
			}
			return STD_MOVE(cross(_t));
		}

		// determinant value for vector array
		// 向量组行列式值
		static _Titem determinant(const _Tself opt[rank]) {
			return _Tbase::determinant(opt);
		}

		// determinant value for vector array
		// 向量组行列式值
		static _Titem determinant(const std::initializer_list< _Tself >& opt) {
			return _Tbase::determinant((const std::initializer_list< _Tbase >&)opt);
		}

		// Vector Cofactor
		// 向量余子式
		static affine_vector < dim - 1, _Titem > cofactor(const _Tself & opt, z_size_t c) {
			return STD_MOVE((affine_vector < dim - 1, _Titem > &)_Tbase::cofactor(opt, c));
		}

		// To be affine vector
		// 转换为仿射向量
		static _Tself affine(const vector< rank - 1, _Titem > opt, _Titem w) {
			_Tself _r;
			for (z_size_t i = 0; i < rank - 1; i++) {
				_r[i] = opt[i];
			}
			_r[rank - 1] = w;
			return STD_MOVE(_r);
		}

		// Vector normalize
		// 向量归一
		static _Tself normalize(const _Tself& opt) {
			_Tself t;

			if (opt[rank - 1]) {
				for (z_size_t i = 0; i < rank - 1; i++) {
					t[i] = opt[i] / opt[rank - 1];
				}
			} else {
				_Titem _m = opt.module();
				if (_m) {
					for (z_size_t i = 0; i < rank - 1; i++) {
						t[i] = opt[i] / _m;
					}
				}
			}

			return STD_MOVE(t);
		}

		// Vector group normalize
		// 向量组归一
		template < z_size_t rows, z_size_t cols, typename Titem >
		static matrix< rows, cols, Titem > normalize(const matrix< rows, cols, Titem >& opt) {
			matrix< rows, cols, Titem > _t(opt);

			for (z_size_t i = 0; i < rows; i++) {
				_Titem _m = _Tself(opt[i]).module();
				if (_m) {
					for (z_size_t j = 0; j < cols; j++) {
						_t[i][j] = _t[i][j] / _m;
					}
				}
			}

			return STD_MOVE(_t);
		}
		
		/********** Affine Transformation 仿射变换 **********/

		// dot or vector translation
		// 点或向量平移
		static _Tsqu translate(const _Tself& trans_vector) {
			return STD_MOVE(_Tsqu(
				_Tm11(IDENTITY),
				_Tm12(),
				_Tm21(cofactor(trans_vector, rank - 1)),
				_Tm22(IDENTITY)
			));
		}

		// unfinished about higher-dimension

		// vector rotate for 3-dim
		// 3D向量旋转
		static _Tsqu rotate(const _Taxis& axis, double radian) {
			typedef square< 4, _Titem > _Tsqu;
			typedef square< 3, _Titem > _Tm11;
			typedef matrix< 3, 1, _Titem > _Tm12;
			typedef matrix< 1, 3, _Titem > _Tm21;
			typedef square< 1, _Titem > _Tm22;
			typedef affine_vector< 3, double > _Tv4;
			typedef affine_vector< 2, double > _Tv3;

			_Tv3 dir = STD_MOVE((_Tv3&)matrix< 1, 4, double >::cofactor< 0, 4 >(axis.dirs));
			_Tm11 _x;
			_x[0][1] = axis.dirs[0][2];
			_x[0][2] = axis.dirs[0][1] * -1;
			_x[1][0] = axis.dirs[0][2] * -1;
			_x[1][2] = axis.dirs[0][0];
			_x[2][0] = axis.dirs[0][1];
			_x[2][1] = axis.dirs[0][0] * -1;

			_Tm11 _m11 = _Tm11(IDENTITY) * cos(radian) + STD_MOVE((_Tm11&)(_Tv3::transpose(dir) * dir)) * (1.0 - cos(radian)) + _x * sin(radian);
			_Tm12 _m12;
			_Tm21 _m21 = _Tv4::cofactor(axis.pos, 3) * (_m11 * -1 + _Tm11(IDENTITY));
			_Tm22 _m22(IDENTITY);

			_Tsqu _rm(_m11, _m12, _m21, _m22);

			return STD_MOVE(_rm);
		}

		// vector scaling or dot scaling relative origin
		// 向量缩放或点相对原点缩放
		static _Tsqu scale(const _Titem& scaling) {
			matrix < dim, dim + 1, _Titem > vis;
			for (z_size_t i = 0; i < dim; i++)
				vis[i][i] = _Titem(1);

			return scale(scaling, graph< dim, dim, _Titem >(_Tself(), vis));
		}

		// vector or dot scaling relative other dot
		// 向量点相对于另一点缩放
		static _Tsqu scale(const _Titem& scaling, const _Tself& fixed_point) {
			matrix < dim, dim + 1, _Titem > vis;
			for (z_size_t i = 0; i < dim; i++)
				vis[i][i] = _Titem(1);

			return scale(scaling, graph< dim, dim, _Titem >(fixed_point, vis));
		}

		// vector or dot scaling to directions relative other dot
		// 向量或点相对一点沿任意方向缩放
		template < z_size_t dim, z_size_t d_dim, typename _Titem >
		static _Tsqu scale(const _Titem& scaling, const graph< dim, d_dim, _Titem >& scaling_params) {
			_Tsqu _tm = translate(scaling_params.pos);
			_Tsqu _sm(IDENTITY);

			for (z_size_t i = 0; i < d_dim; i++) {
				affine_vector< dim - 1, _Titem > _w = _Tself::cofactor(_Tself(scaling_params.dirs[i]), dim);
				_sm = _sm * _Tsqu(
					_Tm11(IDENTITY) + STD_MOVE((_Tm11&)(matrix< dim, dim, _Titem >::transpose(_w) * _w * (scaling - _Titem(1)))),
					_Tm12(),
					_Tm21(),
					_Tm22(IDENTITY)
				);
			}
			return (_tm ^ -1) * _sm * _tm;
		}

		// vector or dot mirror with one plane
		// 向量或点相对于某一平面的镜像
		static _Tsqu mirror(const _Tpl& mpl) {
			typedef matrix < rank - 2, rank - 1, _Titem > _mtx;
			typedef square < rank - 2, _Titem > _sqr;
			// translate matrix
			// 平移矩阵
			_Tsqu _tm = translate(mpl.pos);
			// I + 2 A^{T}(AA^{T})^{-1}A
			// mirror matrix
			// 镜像矩阵
			_mtx _a = matrix < rank - 2, rank, _Titem >::cofactor < 0, rank >(mpl.dirs);

			_Tsqu _mm (
				STD_MOVE((_Tm11&)(_mtx::transpose(_a) * STD_MOVE((_sqr&)(_a * _mtx::transpose(_a)) ^ -1) * _a)) * 2 - _Tm11(IDENTITY),
				_Tm12(),
				_Tm21(),
				_Tm22(IDENTITY)
			);
			return (_tm ^ -1) * _mm * _tm;
		}

		// vector or dot shear along one plane
		// 向量或点沿着某一平面错切
		static _Tsqu shear(const _Tpl& shear_plane, const _Tself& shear_vector, const _Titem& radian) {
			throw "";
		}

		// vector or dot orthogonal projection to one plane
		// 向量或点正交投影至某一平面
		static _Tsqu orthogonal(const _Tpl& opl) {
			typedef matrix < rank - 2, rank - 1, _Titem > _mtx;
			typedef square < rank - 2, _Titem > _sqr;
			// translate matrix
			// 平移矩阵
			_Tsqu _tm = translate(opl.pos);
			// A^{T}(AA^{T})^{-1}A
			// mirror matrix
			// 镜像矩阵
			_mtx _a = matrix < rank - 2, rank, _Titem >::cofactor < 0, rank >(opl.dirs);
			_Tsqu _om(
				STD_MOVE((_Tm11&)(_mtx::transpose(_a) * STD_MOVE((_sqr&)(_a * _mtx::transpose(_a)) ^ -1) * _a)),
				_Tm12(),
				_Tm21(),
				_Tm22(IDENTITY)
			);

			return (_tm ^ -1) * _om * _tm;
		}

		// vector or dot perspective projection to one plane
		// 向量或点透视投影至某一平面
		static _Tsqu perspective(const _Tpl& persp_plane, const _Tself& view) {
			affine_vector< dim - 1, _Titem >
				_n = _Tself::cofactor(persp_plane.n(), rank - 1),
				_v = _Tself::cofactor(view, rank - 1),
				_p = _Tself::cofactor(persp_plane.pos, rank - 1);

			_Tsqu _pm(
				STD_MOVE((_Tm11&)(matrix< 1, rank - 1, _Titem >::transpose(_n) * _v) * _Titem(-1) + _Tm11(IDENTITY) * ((_v - _p) PRO_DOT _n)),
				STD_MOVE((_Tm12&)(matrix< 1, rank - 1, _Titem >::transpose(_n) * -1)),
				STD_MOVE((_Tm21&)(_v * (_p PRO_DOT _n))),
				_Tm22 { { _v PRO_DOT _n } }
			);

			return STD_MOVE(_pm);
		}
	};

_ZGL_END

#endif // !ZGL_AFFINE_VECTOR
