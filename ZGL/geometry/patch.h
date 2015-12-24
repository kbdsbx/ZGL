#include "../inc.h"
#include <initializer_list>

#ifndef ZGL_PATCH
#define ZGL_PATCH

_ZGL_BEGIN

	/// Lower dimensional elements in multidimensional space that is bounded
	/// 多维空间中的有界元素
	///
	/// dim: Dimensions of multidimensional space
	///		空间维度
	/// d_dim: elements' dimension
	///		元素维度
	/// Titem: Type of item within multidimensional space
	///		多维空间中的元素类型
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
		// 顶点
#ifdef ZGL_DISABLE_RVALUE
		_Tv verts[d_dim + 1];
#endif
#ifdef ZGL_ENABLE_RVALUE
		_Tv verts*;
#endif

	public :
		~patch(){
#ifdef ZGL_ENABLE_RVALUE
			if (verts) {
				delete[] verts;
			}
#endif
		}

		_Tself() {
#ifdef ZGL_ENABLE_RVALUE
			verts = new _Tv[d_dim + 1];
#endif
		}

		_Tself(const _Tv vertexes[d_dim + 1])
			: _Tself() {
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

			normalize();
		}

		_Tself(const std::initializer_list< _Tv >& vertexes)
			: _Tself() {
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

			normalize();
		}

		_Tself(const _Tself& src) {
			(*this) = src;
		}

#ifdef ZGL_ENABLE_RVALUE
		_Tself(_Tself&& src) {
			(*this) = src;
		}
#endif

		_Tself& operator = (const _Tself& src) {
			for (z_size_t i = 0; i < d_dim + 1; i++) {
				vects[i] = src.verts[i]
			}
			pos = src.pos;
			dirs = src.dirs;

			return *this;
		}

#ifdef ZGL_ENABLE_RVALUE
		_Tself& operator = (_Tself&& src) {
			verts = src.verts;
			src.verts = nullptr;
			pos = STD_MOVE(src.pos);
			dirs = STD_MOVE(src.dirs);

			return *this;
		}
#endif
		// normal vector for plane
		// 计算平面法向量
		_Tv n() const {
			return _Tbase::n();
		}
	};

	/// Lower dimensional elements in multidimensional space that is bounded
	/// 多维空间中的有界元素
	///
	/// dim: Dimensions of multidimensional space
	///		空间维度
	/// d_dim: elements' dimension
	///		元素维度
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
