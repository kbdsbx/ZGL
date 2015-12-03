#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>
#include <vector>
#include "gridding.h"

#ifndef ZGL_GRIDDING_IMPLICITLY
#define ZGL_GRIDDING_IMPLICITLY

_ZGL_BEGIN
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
	class gridding_implicitly
		: public Iseparable {
		typedef Titem _Titem;
		typedef gridding_implicitly< dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Targ;
		typedef affine_vector < dim, _Titem > _Tv;
		typedef gridding< dim, d_dim, _Titem > _Tgrid;

		const z_size_t _peak_count = (z_size_t)pow(d_dim, 2);

		z_size_t _deep = 5;

	public :

		// Equation implicitly argument type
		// ���꺯����������
		typedef _Targ Targ;

	public :

		// Range for making Gridding
		// ������������ķ�Χ��Χ
		class grid_range {
			// range of coordinate in implicit function graphic.
			// ��ʽ���̵�ͼ�η�Χ
			_Titem _range[2];

			friend class gridding_implicitly< dim, d_dim, _Titem >;

		private :
			grid_range() { }

		public:
			grid_range(_Titem min, _Titem max) {
				_range[0] = min;
				_range[1] = max;
			}

			grid_range& operator = (const grid_range& src) {
				_range[0] = src._range[0];
				_range[1] = src._range[1];

				return *this;
			}

			// Min of coordinate
			// ������Сֵ
			_Titem mini() const {
				return _range[0];
			}

			// Max of coordinate
			// �������ֵ
			_Titem maxi() const {
				return _range[1];
			}
		};

		///** Result **///
		
		// nodes
		// �ڵ�
		std::vector< _Tv > _root;

		///** coordinate Range and implicit function **///

		// Range of coordinate in implicit function
		// ��ʽ���������귶Χ
		grid_range _range[d_dim];

		// implicit function
		// ��ʽ����
		std::function< _Titem(_Targ) > _func;

	private :
		gridding_implicitly() { }

	public :
		gridding_implicitly(const grid_range range[d_dim], const std::function< _Titem(_Targ) > func)
			: gridding() {
			z_size_t i = d_dim;
			while (i--)
				_range[i] = range[i];

			_func = func;

			_dis();
		}

		gridding_implicitly(const std::initializer_list< grid_range > range, const std::function< _Titem(_Targ) > func) {
			z_size_t i = 0;
			for (auto r : range) {
				_range[i] = r;
				i++;
			}

			_func = func;

			_dis();
		}

		_Tself& operator = (const _Tself& src) {
			z_size_t i = d_dim;
			while (i--)
				_range[i] = src._range[i];

			_func = src._func;

			_dis();
		}

		void _dis() {
		}

		void _loop(const _Tgrid& rect, z_size_t deep) {
			if (!deep) {
			}
			for (auto it = rect.begin(); it != rect.end(); ++it) {
				// ����淶�������ݶ���Ϣ
				_Titem v = _func(*it);
				for (z_size_t i = 0; i < d_dim; i++) {
					auto nit(it);
					nit._idx[i] = (nit.idx[i] == 0 ? 1 : 0);
					_Titem nv = _func(*nit);
					// ���������ڵĵ㲻ͬʱ����ʽ����һ�࣬����ʽ����ı�������֮����߶��ཻ
					if (v <= _Titem(0) && nv > _Titem(0) || v > _Titem(0) && nv <= _Titem(0)) {
						; // TODO; // ʹ�ö��ַ���������
					}
				}

				// ���������ڵ����е㶼����ʽ�����һ�࣬����ʽ����ı߲���������ཻ��������������
			}
		}

	private :

		_Tgrid make_range(const grid_range range[d_dim]) {
			_Tgrid::grid_data data[d_dim];
			for (z_size_t i = 0; i < d_dim; i++)
				data[i] = _Tgrid::grid_data(_range[i]._range, 2);

			std::function< _Titem(typename _Tgrid::Targ, z_size_t) > funcs[dim - 1];
			for (z_size_t i = 0; i < dim - 1; i++)
				funcs[i] = [](typename _Tgrid::Targ arg, z_size_t idx) { return arg[idx]; };

			return STD_MOVE(_Tgrid(data, funcs));
		}
	};

_ZGL_END

#endif // !ZGL_GRIDDING_IMPLICITLY
