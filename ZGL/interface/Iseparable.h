#include "../inc.h"
#include "Isolid.h"
#include <iterator>
#include <vector>

#ifndef ZGL_ISEPARABLE
#define ZGL_ISEPARABLE

_ZGL_BEGIN

	// Separable interface
	// 允许被网格化的
	class Iseparable
		: public Isolid {

	protected :
		template < typename _Tv, typename _Tpat, z_size_t dim >
		void _petching(const std::vector< _Tv >& dots, std::vector< _Tpat >& res) {
			_Tpat first_pat;
			_Tv vecs[dim];

			for (size_t i = 0; i < dots.size(); i++) {
				if (i < dim - 1) {
					vecs[i] = dots[i];
				} else if (i == dim - 1) {
					// 使用前 dim 个点创造第一个patch
					vecs[i] = dots[i];
					first_pat = _Tpat(vecs);
					res.push_back(first_pat);
				} else {
					// 从第 dim 个点起，计算当前点分别与第一个patch所构成的点组成的 dim 个petch所组成的夹角，去夹角最大值作为新的patch
					_Tv temp;				// 上下文压栈
					_Tpat next_pat, t_pat;
					double arc(0), narc;
					for (z_size_t j = 0; j < dim; j++) {
						// 两个平面的夹角等同于其法向量的夹角，适用于更高维度的“平面”
						temp = vecs[j];
						vecs[j] = dots[i];
						t_pat = _Tpat(vecs);
						
						narc = first_pat.n().angle(t_pat.n());

						// 夹角最大的patch为新的patch，以保证其拼接的完整性
						if (!arc || narc > arc) {
							next_pat = t_pat;
							arc = narc;
						}

						vecs[j] = temp;
					}

					res.push_back(next_pat);
				}
			}
		}
	};

_ZGL_END

#endif // !ZGL_ISEPARABLE
