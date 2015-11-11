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
		typedef graph< dim, d_dim, Titem > _Tself;
		typedef affine_vector< dim, _Titem > _Tv;
		typedef matrix< d_dim, dim, _Titem, void > _Tax;

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
				for (z_size_t j = 0; j < dim; j++)
					dirs[i][j] = t[j];
			}

			normalize();
		}

		// normal vector for plane
		// 计算平面法向量
		//      p : the point that out of this plane
		//          平面外的一点
		// [exception] : this point is in the plane.
		//          点再平面内
		_Tv n(const _Tv& p) const {
			if ((dim - d_dim) != 2)
				throw "";

			_Tv _t(pos);
			_Tself _plane(*this);

			_plane.orthogonalize();

			for (z_size_t i = 0; i < d_dim; i++) {
				_t = _t + ((p - _plane.pos) PRO_PARALLEL _Tv(_plane.dirs[i]));
			}

			if (_t == _Tv{ _Titem(0), _Titem(0), _Titem(0), _Titem(1) })
				throw "this point is in the plane.";

			return STD_MOVE(_Tv::normalize(_t));
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
		typedef affine_vector< dim, _Titem > _Tv;
		typedef matrix< 1, dim, _Titem > _Tax;

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
