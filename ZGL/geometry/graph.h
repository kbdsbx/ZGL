#include "../inc.h"
#include <initializer_list>

#ifndef ZGL_GRAPH
#define ZGL_GRAPH

namespace ZGL {

	/// Lower dimensional elements in a multidimensional space
	/// ��ά�ռ��еĵ�γ��Ԫ��
	///
	/// dim: Dimensions of multidimensional space
	///       �ռ�ά��
	/// d_dim: elements' dimension
	///       Ԫ��ά��
	/// Titem: Type of item within multidimensional space
	///		  ��ά�ռ��е�Ԫ������
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class graph {
		typedef Titem _Titem;
		typedef graph< dim, d_dim, Titem > _Tself;
		typedef affine_vector< dim, _Titem > _Tv;
		typedef matrix< d_dim, dim, _Titem, void > _Tax;

	public :
		// fixed point that is position of elements at
		// ��ʾԪ��λ�õĲ�����
		_Tv pos;
		// the directions those are linear independent vector group
		// ��ʾԪ�ط���������޹�������
		_Tax dirs;

	public :
		// default constructor
		// Ĭ�Ϲ���
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
	/// ��ά�ռ��еĵ�γ��Ԫ�ز��ҽ���ֻ��λ��
	///
	/// dim: Dimensions of multidimensional space
	///       �ռ�ά��
	/// Titem: Type of item within multidimensional space
	///		  ��ά�ռ��е�Ԫ������
	template < z_size_t dim, typename Titem >
	class graph < dim, 0, Titem > {
		typedef Titem _Titem;
		typedef affine_vector< dim, _Titem > _Tv;
		typedef matrix< 1, dim, _Titem > _Tax;

	public:
		// fixed point that is position of elements at
		// ��ʾԪ��λ�õĲ�����
		_Tv pos;

	public:
		// default constructor
		// Ĭ�Ϲ���
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
