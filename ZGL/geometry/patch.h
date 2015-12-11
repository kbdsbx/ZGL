#include "../inc.h"
#include "graph.h"
#include <initializer_list>

#ifndef ZGL_PATCH
#define ZGL_PATCH

_ZGL_BEGIN

	/// Lower dimensional elements in multidimensional space that is bounded
	/// ��ά�ռ��е��н�Ԫ��
	///
	/// dim: Dimensions of multidimensional space
	///		�ռ�ά��
	/// d_dim: elements' dimension
	///		Ԫ��ά��
	/// Titem: Type of item within multidimensional space
	///		��ά�ռ��е�Ԫ������
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class patch
		: public graph < dim, d_dim, Titem > {
		typedef Titem _Titem;
		typedef graph< dim, d_dim, Titem > _Tbase;
		typedef patch< dim, d_dim, Titem > _Tself;
		typedef affine_vector< dim, _Titem > _Tv;
		typedef affine_vector< dim, _Titem > _Tdot;

	public :
		// patch vertexes
		// ����
		_Tv verts[d_dim + 1];

		_Tdot out_p;

	public :
		_Tself() { }

		_Tself(const _Tv vertexes[d_dim + 1], _Tdot out_point) {
			for (z_size_t i = 0; i < d_dim + 1; i++) {
				verts[i] = vertexes[i];

				if (!i) {
					pos = vertexes[i];
				} else {
					for (z_size_t j = 0; j < dim; j++) {
						dirs[i - 1][j] = vertexes[i][j] - pos[j];
					}
				}
			}

			out_p = out_point;

			normalize();
		}

		_Tself(const std::initializer_list< _Tv >& vertexes, _Tdot out_point) {
			z_size_t i = 0;
			for (decltype(auto) v : vertexes) {
				if (!i) {
					pos = v;
				} else {
					for (z_size_t j = 0; j < dim; j++) {
						dirs[i - 1][j] = v[j] - pos[j];
					}
					verts[i - 1] = v;
				}
				i++;
			}

			out_p = out_point;

			normalize();
		}

		// normal vector for plane
		// ����ƽ�淨����
		//      p : the point that out of this plane
		//          ƽ�����һ��
		_Tv n(const _Tv& p) const {
			return _Tbase::n(p);
		}

		// normal vector for plane
		// ����ƽ�淨����
		_Tv n() const {
			return n(out_p);
		}
	};

	/// Lower dimensional elements in multidimensional space that is bounded
	/// ��ά�ռ��е��н�Ԫ��
	///
	/// dim: Dimensions of multidimensional space
	///		�ռ�ά��
	/// d_dim: elements' dimension
	///		Ԫ��ά��
	template < z_size_t dim, typename Titem >
	class patch < dim, 0, Titem >
		: graph< dim, 0, Titem > {
		typedef Titem _Titem;
		typedef graph< dim, 0, Titem > _Tbase;
		typedef patch< dim, 0, Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;

	public :

		_Tself() { };
	};

_ZGL_END

#endif // !ZGL_PATCH
