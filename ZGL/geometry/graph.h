#include "../inc.h"
#include <initializer_list>

#ifndef ZGL_GRAPH
#define ZGL_GRAPH

_ZGL_BEGIN

	/// Lower dimensional elements in a multidimensional space
	/// 多维空间中的低纬度元素
	///
	/// dim: Dimensions of multidimensional space
	///       空间维度
	/// d_dim: elements' dimension
	///       元素维度
	/// Titem: Type of item within multidimensional space
	///		  多维空间中的元素类型
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class graph {
		typedef Titem _Titem;
		// The rank of base vector
		// 父级向量的秩
		const static z_size_t rank = dim + 1;
		typedef graph< dim, d_dim, Titem > _Tself;
		typedef affine_vector< dim, _Titem > _Tv;
		typedef matrix< d_dim, rank, _Titem, void > _Tax;

	public :
		// fixed point that is position of elements at
		// 表示元素位置的不动点
		_Tv pos;
		// the directions those are linear independent vector group
		// 表示元素方向的线性无关向量组
		_Tax dirs;

	public :
		// default constructor
		// 默认构造
		_Tself() { };

		_Tself(const _Tv& pos, const _Tax& directions)
			: pos(pos), dirs(directions) {
			normalize();
		}

		_Tself(_Tv&& pos, const _Tax& directions)
			: pos(pos), dirs(directions) {
			normalize();
		}

		_Tself(const _Tv& pos, _Tax&& directions)
			: pos(pos), dirs(directions) {
			normalize();
		}

		_Tself(_Tv&& pos, _Tax&& directions)
			: pos(pos), dirs(directions) {
			normalize();
		}

		void normalize() {
			dirs = _Tv::normalize(dirs);
		}

		// orthogonalize linear independent vector group
		// 将线性无关向量组正交化
		void orthogonalize() {
			for (z_size_t i = 1; i < d_dim; i++) {
				_Tv t(dirs[i]);
				for (z_size_t j = (i - 1); j >= 0; j--)
					t = t PRO_UPRIGHT _Tv(dirs[j]);
				for (z_size_t j = 0; j < rank; j++)
					dirs[i][j] = t[j];
			}

			normalize();
		}

		// normal vector for plane
		// 计算平面法向量
		// [exception] : Just only 'plane' in any dimensional space.
		//          仅允许多维中的平面
		_Tv n() const {
			if (d_dim != dim - 1)
				throw "Just only 'plane' in any dimensional space.";

			_Tv vectors[d_dim];
			for (z_size_t i = 0; i < d_dim; i++) {
				vectors[i] = _Tv(dirs[i]);
			}

			return STD_MOVE(_Tv::normalize(_Tv::cross(vectors)));
		}
	};

	/// Lower dimensional elements in a multidimensional space and only positions
	/// 多维空间中的低纬度元素并且仅仅只有位置
	///
	/// dim: Dimensions of multidimensional space
	///       空间维度
	/// Titem: Type of item within multidimensional space
	///		  多维空间中的元素类型
	template < z_size_t dim, typename Titem >
	class graph < dim, 0, Titem > {
		typedef Titem _Titem;
		const static z_size_t rank = dim + 1;
		typedef affine_vector< dim, _Titem > _Tv;
		typedef matrix< 1, rank, _Titem > _Tax;

	public:
		// fixed point that is position of elements at
		// 表示元素位置的不动点
		_Tv pos;

	public:
		// default constructor
		// 默认构造
		graph < dim, 0, Titem >() { };

		graph < dim, 0, Titem >(const _Tv& pos, const _Tax& directions)
			: pos(pos) { }

		graph < dim, 0, Titem >(_Tv&& pos, const _Tax& directions)
			: pos(pos) { }

		graph < dim, 0, Titem >(const _Tv& pos, _Tax&& directions)
			: pos(pos) { }

		graph < dim, 0, Titem >(_Tv&& pos, _Tax&& directions)
			: pos(pos) { }
	};

	template < int d_dim, typename Titem >
	class graph < 0, d_dim, Titem > {
	};

_ZGL_END

#endif // !ZGL_GRAPH
