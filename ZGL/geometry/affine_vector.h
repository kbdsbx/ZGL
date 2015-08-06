#include "../inc.h"
#include "../linear/vector.h"
#include <math.h>

#ifndef ZGL_AFFINE_VECTOR
#define ZGL_AFFINE_VECTOR

namespace ZGL {

	/// The vector or dot that is in affine transfromations
	/// ����任�е��������
	///
	/// dim: Dimensions of vector or dot
	///       �������ά��
	/// Titem: Type of item within vector or dot
	///       ��������������
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
		typedef graph< dim, 1, _Titem > _Taxis;
		typedef graph< dim, dim - 2, _Titem > _Tpl;

	public :
		// default constructor
		// Ĭ�Ϲ���
		_Tself()
			: _Tbase() { }

		// constructor that using item array
		// ʹ������Ĺ��캯��
		_Tself(const _Titem src[dim])
			: _Tbase(src) { }

		// constructor that using type of itself
		// copy constructor
		// �������캯��
		_Tself(const _Tself& src)
			: _Tbase(src) { }

		// constructor that using Rvalue params
		// ʹ����ֵ����
		_Tself(_Tself&& src)
			: _Tbase(src) { }

		// constructor that using initializer_list in cpp-11
		// ʹ��C++11�ĳ�ʼ���б�
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
		// ��������(ģ)
		_Titem module() const {
			return ((const _Tbase*)this)->module();
		}

		// The angle between the both of vector
		// �������ļн�
		_Titem angle(const _Tself& opt) const {
			return ((const _Tbase*)this)->angle(opt);
		}

		// Cross product
		// only 3 dimension and 7 dimension
		// ������
		// ��������ά����ά
		static _Tself cross(const _Tself opt[dim - 1]) {
			return (_Tself&&)_Tbase::cross(opt);
		}

		// Cross product
		// only 3 dimension and 7 dimension
		// ������
		// ��������ά����ά
		static _Tself cross(const std::initializer_list< _Tself >& opt) {
			return (_Tself&&)_Tbase::cross((const std::initializer_list< _Tbase >&)opt);
		}

		// determinant value for vector array
		// ����������ʽֵ
		static _Titem determinant(const _Tself opt[dim]) {
			return _Tbase::determinant(opt);
		}

		// determinant value for vector array
		// ����������ʽֵ
		static _Titem determinant(const std::initializer_list< _Tself >& opt) {
			return _Tbase::determinant((const std::initializer_list< _Tbase >&)opt);
		}

		// Vector Cofactor
		// ��������ʽ
		static affine_vector < dim - 1, _Titem > cofactor(const _Tself & opt, z_size_t c) {
			return (affine_vector < dim - 1, _Titem > &&)_Tbase::cofactor(opt, c);
		}
		
		/********** Affine Transformation ����任 **********/

		// dot or vector translation
		// �������ƽ��
		static _Tsqu translate(const _Tself& trans_vector) {
			return (_Tsqu&&)_Tsqu(
				_Tm11(IDENTITY),
				_Tm12(),
				_Tm21((_Tm21&&)cofactor(trans_vector, dim - 1)),
				_Tm22(IDENTITY)
			);
		}

		// dot or vector translates arround one axis
		// �������������ת
		template < z_size_t dim, typename _Titem >
		static _Tsqu rotate(const _Taxis& axis, double radian) {
			// translate matrix
			// ƽ�ƾ���
			_Tsqu _tm = translate(axis.pos);
			// rotate orthogonal matrix
			// ��ת��������
			_Tsqu _rm[dim - 1] = { _Tsqu(IDENTITY) };
			// result of rotate matrix 
			// ��ת�����
			_Tsqu _sm(IDENTITY);

			for (z_size_t i = 0; i < dim - 1; i++) {
				_Tself _ap(axis.dirs[0]), _ax;
				// list
				// ����
				_ap[i] = 0;
				_ax[i == dim - 2 ? 0 : i + 1] = 1;

				// makes new axis with sub vector rotate
				// Ϊ��������ת�����µ���ת��
				matrix< 1, dim - 1, _Titem > _dir;
				for (z_size_t si = 0; si < dim - 2; si++)
					_dir[0][si] = axis.dirs[0][si];

				double _rd = (i == dim - 1 ? radian : _ap.angle(_ax));

				square< dim - 1, _Titem > _sub_vect = affine_vector< dim - 1, _Titem >::rotate(graph< dim - 1, 1, _Titem >(affine_vector< dim - 1, _Titem >(), _dir), _rd);

				for (z_size_t si = 0, si_t = 0; si < dim; si++)
					if (si == i)
						si_t = 1;
					else for (z_size_t sj = 0, sj_t = 0; sj < dim; sj++)
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
			// T:		������ƽ����ԭ��ľ���
			// R_{i}:	����������������ת�ľ���
			// j:		���������ͬʱҲ��ά��
			// ʹ��������վ����LateX��ʽ
			_sm = _sm * _tm;
			for (z_size_t i = 0; i < dim - 1; i++)
				_sm = _sm * _rm[i];
			for (z_size_t i = dim - 2; i >= 0; i--)
				_sm = _sm * (_rm[i] ^ -1);
			_sm = _sm * (_tm ^ -1);

			return std::move(_sm);
		}

		// only clockwise with rotate of 2 dimension
		// 2ά˳ʱ����ת
		template < typename _Titem >
		static square< 2, _Titem > rotate (const graph< 2, 1, _Titem >& axis, double radian) {
			return square < 2, _Titem > {
				{ cos(radian), -sin(radian) },
				{ sin(radian), cos(radian) }
			};
		}

		// vector scaling or dot scaling relative origin
		// �������Ż�����ԭ������
		static _Tsqu scale(const _Titem& scaling) {
			return scale(scaling, graph< dim, dim, _Titem >(_Tself(), _Tsqu(IDENTITY)));
		}

		// vector or dot scaling relative other dot
		// �������������һ������
		static _Tsqu scale(const _Titem& scaling, const _Tself& fixed_point) {
			return scale(scaling, graph< dim, dim, _Titem >(fixed_point, _Tsqu(IDENTITY)));
		}

		// vector or dot scaling to directions relative other dot
		// ����������һ�������ⷽ������
		template < z_size_t dim, z_size_t d_dim, typename _Titem >
		static _Tsqu scale(const _Titem& scaling, const graph< dim, d_dim, _Titem >& scaling_params) {
			_Tsqu _tm = translate(scaling_params.pos);
			_Tsqu _s(IDENTITY);

			for (z_size_t i = 0; i < d_dim; i++) {
				affine_vector< dim - 1, _Titem > _w = _Tself::cofactor(_Tself(scaling_params.dirs[i]), dim - 1);
				_s = _s * _Tsqu(
					_Tm11(IDENTITY) + (_Tm11&&)(matrix< dim - 1, dim - 1, _Titem >::transpose(_w) * _w * (scaling - _Titem(1))),
					_Tm12(),
					_Tm21(),
					_Tm22(IDENTITY)
				);
			}
			return (_Tsqu&&)(_tm * _s * (_tm ^ -1));
		}

		// vector or dot mirror with one plane
		// ������������ĳһƽ��ľ���
		static _Tsqu mirror(const _Tpl& mpl) {
			typedef matrix < dim - 2, dim - 1, _Titem > _mtx;
			typedef square < dim - 2, _Titem > _sqr;
			// translate matrix
			// ƽ�ƾ���
			_Tsqu _tm = translate(mpl.pos);
			// I + 2 A^{T}(AA^{T})^{-1}A
			// mirror matrix
			// �������
			_mtx _a = matrix < dim - 2, dim, _Titem >::cofactor < 0, dim >(mpl.dirs);

			_Tsqu _mm (
				_Tm11(IDENTITY) + (_Tm11&&)(_mtx::transpose(_a) * ((_sqr&&)(_a * _mtx::transpose(_a)) ^ -1) * _a) * 2,
				_Tm12(),
				_Tm21(),
				_Tm22(IDENTITY)
			);
			return (_Tsqu&&)(_tm * _mm * (_tm ^ -1));
		}

		// vector or dot shear along one plane
		// �����������ĳһƽ�����
		static _Tsqu shear(const _Tpl& shear_plane, const _Tself& shear_vector, const _Titem& radian) {
			throw "";
		}

		// vector or dot orthogonal projection to one plane
		// �����������ͶӰ��ĳһƽ��
		static _Tsqu orthogonal(const _Tpl& opl) {
			typedef matrix < dim - 2, dim - 1, _Titem > _mtx;
			typedef square < dim - 2, _Titem > _sqr;
			// translate matrix
			// ƽ�ƾ���
			_Tsqu _tm = translate(opl.pos);
			// A^{T}(AA^{T})^{-1}A
			// mirror matrix
			// �������
			_mtx _a = matrix < dim - 2, dim, _Titem >::cofactor < 0, dim >(opl.dirs);
			_Tsqu _om(
				(_Tm11)(_mtx::transpose(_a) * ((_sqr)(_a * _mtx::transpose(_a)) ^ -1) * _a),
				_Tm12(),
				_Tm21(),
				_Tm22(IDENTITY)
			);

			return (_Tsqu&&)(_tm * _om * (_tm ^ -1));
		}

		// vector or dot perspective projection to one plane
		// �������͸��ͶӰ��ĳһƽ��
		static _Tsqu perspective(const _Tpl& persp_plane, const _Tself& view) {
			throw "";
		}
	};
}

#endif // !ZGL_AFFINE_VECTOR
