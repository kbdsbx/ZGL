#include "../inc.h"
#include "../linear/vector.h"
#include <math.h>

#ifndef ZGL_AFFINE_VECTOR
#define ZGL_AFFINE_VECTOR

namespace ZGL {

	/// The vector or dot that is in affine transfromations
	/// 仿射变换中的向量或点
	///
	/// dim: Dimensions of vector or dot
	///       向量或点维度
	/// Titem: Type of item within vector or dot
	///       向量项或点项类型
	template < z_size_t dim, typename Titem >
	class affine_vector
		: public vector< dim, Titem > {
		typedef Titem _Titem;
		typedef affine_vector< dim, _Titem > _Tself;
		typedef vector< dim, _Titem > _Tbase;
		typedef vector< dim, _Titem > _Tvt;
		typedef square< dim, _Titem > _Tsqu;
		typedef square< dim - 1, _Titem > _Tm11;
		typedef matrix< dim - 1, 1, _Titem > _Tm12;
		typedef matrix< 1, dim - 1, _Titem > _Tm21;
		typedef square< 1, _Titem > _Tm22;
		typedef graph< dim, dim - 2, _Titem > _Tpl;
		typedef graph< dim, dim - 3, _Titem > _Taxis;

	public :
		// default constructor
		// 默认构造
		_Tself()
			: _Tbase() { }

		// constructor that using item array
		// 使用数组的构造函数
		_Tself(const _Titem src[dim])
			: _Tbase(src) { }

		// constructor that using type of itself
		// copy constructor
		// 拷贝构造函数
		_Tself(const _Tself& src)
			: _Tbase(src) { }

		// constructor that using Rvalue params
		// 使用右值构造
		_Tself(_Tself&& src)
			: _Tbase(src) { }

		// constructor that using initializer_list in cpp-11
		// 使用C++11的初始化列表
		_Tself(std::initializer_list< _Titem > src)
			: _Tbase({ src }) { }

		_Titem& operator [] (z_size_t opt) const {
			return (*(const _Tbase*)this)[opt];
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

		_Tself operator / (const _Titem& opt) const {
			return (_Tself&&)(*(const _Tbase*)this / opt);
		}

		_Tself operator / (const _Tself& opt) const {
			return (_Tself&&)(*(const _Tbase*)this / opt);
		}

		template < z_size_t rows >
		_Tself operator * (const matrix < dim, rows, _Titem >& opt) const {
			return (_Tself&&)(*(const _Tbase*)this * opt);
		}

		_Titem operator PRO_DOT (const _Tself& opt) const {
			return *(const _Tbase*)this PRO_DOT opt;
		}

		_Tself operator PRO_PARALLEL (const _Tself& opt) const {
			return (_Tself&&)(*(const _Tbase*)this PRO_PARALLEL opt);
		}

		_Tself operator PRO_UPRIGHT (const _Tself& opt) const {
			return (_Tself&&)(*(const _Tbase*)this PRO_UPRIGHT opt);
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
		// only 3 dimension or 7 dimension
		// 向量积
		// 仅用于三维或七维
		static _Tself cross(const _Tself opt[dim - 2]) {
			vector< dim - 1, _Titem > _t[dim - 2], _r;
			for (z_size_t i = 0; i < dim - 2; i++) {
				_t[i] = (vector< dim - 1, _Titem >)affine_vector< dim, _Titem >::cofactor(opt[i], dim - 1);
			}
			_r = vector< dim - 1, _Titem >::cross(_t);
			return (_Tself&&)_Tself::affine(_r, _Titem(0));
		}

		// Cross product
		// only 3 dimension and 7 dimension
		// 向量积
		// 仅用于三维或七维
		static _Tself cross(const std::initializer_list< _Tself >& opt) {
			_Tself _t[dim - 2];
			z_size_t i = 0;
			for (const _Tself& o : opt) {
				_t[i] = o;
				i++;
			}
			return (_Tself&&)cross(_t);
		}

		// determinant value for vector array
		// 向量组行列式值
		static _Titem determinant(const _Tself opt[dim]) {
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
			return (affine_vector < dim - 1, _Titem > &&)_Tbase::cofactor(opt, c);
		}

		// To be affine vector
		// 转换为仿射向量
		static _Tself affine(const vector< dim - 1, _Titem > opt, _Titem w) {
			_Tself _r;
			for (z_size_t i = 0; i < dim - 1; i++) {
				_r[i] = opt[i];
			}
			_r[dim - 1] = w;
			return (_Tself&&)_r;
		}

		// Vector normalize
		// 向量归一
		static _Tself normalize(const _Tself& opt) {
			_Tself t;

			if (opt[dim - 1]) {
				for (z_size_t i = 0; i < dim - 1; i++) {
					t[i] = opt[i] / opt[dim - 1];
				}
			} else {
				_Titem _m = opt.module();
				if (_m) {
					for (z_size_t i = 0; i < dim - 1; i++) {
						t[i] = opt[i] / _m;
					}
				}
			}

			return std::move(t);
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

			return std::move(_t);
		}
		
		/********** Affine Transformation 仿射变换 **********/

		// dot or vector translation
		// 点或向量平移
		static _Tsqu translate(const _Tself& trans_vector) {
			return (_Tsqu&&)_Tsqu(
				_Tm11(IDENTITY),
				_Tm12(),
				_Tm21((_Tm21&&)cofactor(trans_vector, dim - 1)),
				_Tm22(IDENTITY)
			);
		}

		// dot or vector translates arround one axis
		// 点或向量绕轴旋转
		template < z_size_t dim >
		static square< dim, _Titem > rotate(const graph< dim, dim - 3, _Titem >& axis, double radian) {
			// translate matrix
			// 平移矩阵
			_Tsqu _tm = translate(axis.pos);
			// rotate orthogonal matrix
			// 旋转正交矩阵
			_Tsqu _rm[dim - 1] = { _Tsqu(IDENTITY) };
			// result of rotate matrix 
			// 旋转矩阵解
			_Tsqu _sm(IDENTITY);

			for (z_size_t i = 0; i < dim - 1; i++) {
				_Tself _ap(axis.dirs[0]), _ax;
				// list
				// 序列
				_ap[i] = 0;
				_ax[i == dim - 2 ? 0 : i + 1] = 1;

				// makes new axis with sub vector rotate
				// 为子向量旋转创建新的旋转轴
				matrix< 1, dim - 1, _Titem > _dir;
				for (z_size_t si = 0; si < dim - 2; si++)
					_dir[0][si] = axis.dirs[0][si];

				double _rd = (i == dim - 2 ? radian : _ap.angle(_ax));

				square< dim - 1, _Titem > _sub_vect =
					affine_vector< dim - 1, _Titem >::rotate(
						graph< dim - 1, dim - 4, _Titem >(
							affine_vector< dim - 1, _Titem >(), _dir), _rd);

				for (z_size_t si = 0, si_t = 0; si < dim - 2; si++)
					if (si == i)
						si_t = 1;
					else for (z_size_t sj = 0, sj_t = 0; sj < dim - 2; sj++)
						if (sj == i)
							sj_t = 1;
						else
							_rm[i][si][sj] = _sub_vect[si - si_t][sj - sj_t];
			}

			// R =  T \times \prod_{i = 0}^{j - 1} R_{i} \times R_{j} \times \prod_{i = j-1}^{1} R_{i}^{-1} \times T^{-1}
			// T:		matrix that is translate sth to original point
			// R_{i}:	matrix that is rotate sth around the axes of coordinate
			// j:		count of axes, else this is dimension
			// copy into http://latex.codecogs.com/eqneditor/editor.php
			// 
			// T:		将物体平移至原点的矩阵
			// R_{i}:	将物体绕坐标轴旋转的矩阵
			// j:		轴的数量，同时也是维度
			// 使用如上网站处理LateX公式
			_sm = _sm * (_tm ^ -1);
			for (z_size_t i = 0; i < dim - 3; i++)
				_sm = _sm * (_rm[i] ^ -1);
			for (z_size_t i = dim - 2; i >= 0; i--)
				_sm = _sm * _rm[i];
			_sm = _sm * _tm;

			return std::move(_sm);
		}

		// only clockwise with rotate of 2 dimension
		// 2维顺时针旋转
		template <>
		static square< 3, _Titem > rotate(const graph< 3, 0, _Titem >& axis, double radian) {
			return square < 3, _Titem > {
				{ cos(radian), -sin(radian), 0 },
				{ sin(radian), cos(radian), 0 },
				{ 0, 0, 1 },
			};
		}

		// vector scaling or dot scaling relative origin
		// 向量缩放或点相对原点缩放
		static _Tsqu scale(const _Titem& scaling) {
			return scale(scaling, graph< dim, dim, _Titem >(_Tself(), _Tsqu(IDENTITY)));
		}

		// vector or dot scaling relative other dot
		// 向量点相对于另一点缩放
		static _Tsqu scale(const _Titem& scaling, const _Tself& fixed_point) {
			return scale(scaling, graph< dim, dim, _Titem >(fixed_point, _Tsqu(IDENTITY)));
		}

		// vector or dot scaling to directions relative other dot
		// 向量或点相对一点沿任意方向缩放
		template < z_size_t dim, z_size_t d_dim, typename _Titem >
		static _Tsqu scale(const _Titem& scaling, const graph< dim, d_dim, _Titem >& scaling_params) {
			_Tsqu _tm = translate(scaling_params.pos);
			_Tsqu _sm(IDENTITY);

			for (z_size_t i = 0; i < d_dim; i++) {
				affine_vector< dim - 1, _Titem > _w = _Tself::cofactor(_Tself(scaling_params.dirs[i]), dim - 1);
				_sm = _sm * _Tsqu(
					_Tm11(IDENTITY) + (_Tm11&&)(matrix< dim - 1, dim - 1, _Titem >::transpose(_w) * _w * (scaling - _Titem(1))),
					_Tm12(),
					_Tm21(),
					_Tm22(IDENTITY)
				);
			}
			return (_Tsqu&&)((_tm ^ -1) * _sm * _tm);
		}

		// vector or dot mirror with one plane
		// 向量或点相对于某一平面的镜像
		static _Tsqu mirror(const _Tpl& mpl) {
			typedef matrix < dim - 2, dim - 1, _Titem > _mtx;
			typedef square < dim - 2, _Titem > _sqr;
			// translate matrix
			// 平移矩阵
			_Tsqu _tm = translate(mpl.pos);
			// I + 2 A^{T}(AA^{T})^{-1}A
			// mirror matrix
			// 镜像矩阵
			_mtx _a = matrix < dim - 2, dim, _Titem >::cofactor < 0, dim >(mpl.dirs);

			_Tsqu _mm (
				(_Tm11&&)(_mtx::transpose(_a) * ((_sqr&&)(_a * _mtx::transpose(_a)) ^ -1) * _a) * 2 - _Tm11(IDENTITY),
				_Tm12(),
				_Tm21(),
				_Tm22(IDENTITY)
			);
			return (_Tsqu&&)((_tm ^ -1) * _mm * _tm);
		}

		// vector or dot shear along one plane
		// 向量或点沿着某一平面错切
		static _Tsqu shear(const _Tpl& shear_plane, const _Tself& shear_vector, const _Titem& radian) {
			throw "";
		}

		// vector or dot orthogonal projection to one plane
		// 向量或点正交投影至某一平面
		static _Tsqu orthogonal(const _Tpl& opl) {
			typedef matrix < dim - 2, dim - 1, _Titem > _mtx;
			typedef square < dim - 2, _Titem > _sqr;
			// translate matrix
			// 平移矩阵
			_Tsqu _tm = translate(opl.pos);
			// A^{T}(AA^{T})^{-1}A
			// mirror matrix
			// 镜像矩阵
			_mtx _a = matrix < dim - 2, dim, _Titem >::cofactor < 0, dim >(opl.dirs);
			_Tsqu _om(
				(_Tm11&&)(_mtx::transpose(_a) * ((_sqr&&)(_a * _mtx::transpose(_a)) ^ -1) * _a),
				_Tm12(),
				_Tm21(),
				_Tm22(IDENTITY)
			);

			return (_Tsqu&&)((_tm ^ -1) * _om * _tm);
		}

		// vector or dot perspective projection to one plane
		// 向量或点透视投影至某一平面
		static _Tsqu perspective(const _Tpl& persp_plane, const _Tself& view) {
			affine_vector< dim - 1, _Titem > _n = _Tself::cofactor(_Tself(persp_plane.dirs), dim - 1);
			affine_vector< dim - 1, _Titem > _v = _Tself::cofactor(view, dim - 1);
			affine_vector< dim - 1, _Titem > _p = _Tself::cofactor(persp_plane.pos, dim - 1);
			_Tsqu _pm(
				(_Tm11&&)(matrix< 1, dim - 1, _Titem >::transpose(_n) * _v * _Titem(-1) + _Tm11(IDENTITY) * ((_v - _p) PRO_DOT _n)),
				(_Tm12&&)(matrix< 1, dim - 1, _Titem >::transpose(_n) * -1),
				(_Tm21&&)(_v * (_p PRO_DOT _n)),
				_Tm22({ {_v PRO_DOT _n} })
			);

			return _pm;
		}
	};
}

#endif // !ZGL_AFFINE_VECTOR
