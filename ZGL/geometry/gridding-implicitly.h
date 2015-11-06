#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>

#ifndef ZGL_GRIDDING_IMPLICITLY
#define ZGL_GRIDDING_IMPLICITLY

namespace ZGL {
	/// Gridding Implicitly
	/// ��ʽ����
	///
	/// dim: Dimensions of space for gridding in
	///       ��������ά��
	/// d_dim: elements' dimension
	///	      Ԫ��ά��
	/// Titem: Type of item within gridding
	///       ����������
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding_implicitly {
		typedef Titem _Titem;
		typedef gridding_implicitly< dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Tv;

	public :

	public :

		// Range for making Gridding
		// ������������ķ�Χ��Χ
		class grid_range {
			// range of coordinate in implicit function graphic.
			// ��ʽ���̵�ͼ�η�Χ
			_Titem _min;
			_Titem _max;

		public:
			grid_range() { }

			grid_range(_Titem min, _Titem max)
				: _min(min), _max(max) { }

			grid_range& operator = (const grid_range& src) {
				_min = src._min;
				_max = src._max;

				return *this;
			}

			// Min of coordinate
			// ������Сֵ
			_Titem min_s() const {
				return _min;
			}

			// Max of coordinate
			// �������ֵ
			_Titem max_s() const {
				return _max;
			}
		};

		///** coordinate Range and implicit function **///

		// Range of coordinate in implicit function
		// ��ʽ���������귶Χ
		grid_range _range[d_dim];

		// implicit function
		// ��ʽ����
		std::function< bool(_Tv) > _func;

	private :
		gridding_implicitly() { }

	public :
		gridding_implicitly(const grid_range range[d_dim], const std::function< bool(_Tv) > func)
			: gridding() {
			z_size_t i = d_dim;
			while (i--)
				_range[i] = range[i];

			_i_func = func;

			_dis();
		}

		void _dis() {
		}
	};
}

#endif // !ZGL_GRIDDING_IMPLICITLY
