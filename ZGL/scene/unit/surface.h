#include "../../inc.h"
#include <vector>
#include "../../geometry/gridding.h"
#include "../../geometry/gridding-implicitly.h"
#include "../../geometry/patch.h"

#ifndef ZGL_SCENE_UNIT_SURFACE
#define ZGL_SCENE_UNIT_SURFACE

_ZGL_BEGIN

	// Surface in 3-dim
	// 3DÇúÃæ
	template < z_size_t dim, typename Titem >
	class surface
		: public gridding< dim, 2, Titem >, public Iseparable {
	protected :
		typedef Titem _Titem;
		typedef surface< dim, _Titem > _Tself;
		typedef gridding< dim, 2, _Titem > _Tbase;
		typedef patch< dim, 1, _Titem > _Tseg;
		typedef std::vector< _Tseg > _Tcol;

		typedef typename _Tbase::_Tval _Tval;
		typedef typename _Tseg::_Tv _Tv;

	public :

		typedef _Tseg Tseg;

	private :
		_Tcol _res;

	public :
		~surface() { }

		surface(const grid_data data[2], const std::function< _Titem(_Tval, z_size_t = 0) > funcs[dim])
			: _Tbase (data, funcs) {
			_dis();
		}

		surface(const std::initializer_list< grid_data > data, const std::initializer_list< std::function< _Titem(_Tval, z_size_t = 0) > > funcs)
			: _Tbase (data, funcs) {
			_dis();
		}

		surface(const _Tself& src)
			: _Tbase (src) {
			_dis();
		}
		
		_Tcol::Iterator begin() {
			return _res.begin();
		}

		_Tcol::Iterator end() {
			return _res.end();
		}

		Tseg operator [] (z_size_t idx) {
			return _res[idx];
		}

	private :
		void _dis() {
			for (typename _Tbase::_Tit it = _Tbase::begin(); it != _Tbase::end(); ++it) {
				typedef typename _Tbase::_Tit _Tit;

				_Tit _rect_it(2);
				_Tit _rect_it_end = _rect_it.end();
				std::map< _Tit, _Tv > re;

				z_size_t i = dim;
				while (i--) {
					if (it.idxi(i) < maxi(i) - 1) {
						goto it_continue;
					}
				}
				for (; _rect_it != _rect_it_end; ++_rect_it) {
					re[_rect_it] = *_rect_it;
				}

				_petching< _Tv, _Tseg, 2 >(re, _res);

			it_continue:
				continue;
			}
		}
	};

_ZGL_END

#endif // !ZGL_SCENE_UNIT_SURFACE
