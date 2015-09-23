#include "../inc.h"
#include <initializer_list>

#ifndef ZGL_GRAPH
#define ZGL_GRAPH

namespace ZGL {

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
}

#endif // !ZGL_GRAPH
