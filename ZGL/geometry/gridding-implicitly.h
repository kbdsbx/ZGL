#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>
#include <vector>
#include <map>
#include <array>
#include "gridding.h"
#include "iterator.h"
#include "patch.h"

#ifndef ZGL_GRIDDING_IMPLICITLY
#define ZGL_GRIDDING_IMPLICITLY

_ZGL_BEGIN

	/// Gridding Implicitly
	/// 隐式网格
	///
	/// dim: Dimensions of space for gridding in
	///       网格所在维度
	/// d_dim: elements' dimension
	///	      元素维度
	/// Titem: Type of item within gridding
	///       网格项类型
	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding_implicitly
		: public Iseparable {
		typedef Titem _Titem;
		// The rank of affine vector
		// 仿射向量的秩
		const static z_size_t rank = dim + 1;

		typedef gridding_implicitly< dim, d_dim, _Titem > _Tself;
		typedef affine_vector < dim, _Titem > _Targ;
		typedef affine_vector < dim, _Titem > _Tv;
		typedef gridding< dim, dim, _Titem > _Tgrid;
		typedef iterator< dim, _Tv > _Tit;
		typedef patch< dim, dim - 1, _Titem > _Tpat;
		typedef std::vector< _Tpat > _Tcol;

	private :
		_Titem _start;
		_Titem _step;
		_Titem _end;

	public :

		// Equation implicitly argument type
		// 坐标函数参数类型
		typedef _Targ Targ;

	public :
		///** Result **///
		
		// nodes
		// 节点
		_Tcol _root;

		///** coordinate Range and implicit function **///

		// implicit function
		// 隐式函数
		std::function< _Titem(_Targ) > _func;

	private :
		_Tself() { }

	public :
		_Tself(_Titem start, _Titem step, _Titem end, const std::function< _Titem(_Targ) > func) {
			_start = start;
			_step = step;
			_end = end;
			_func = func;

			_dis();
		}

		_Tself& operator = (const _Tself& src) {
			_func = src._func;
			_start = src._start;
			_end = src._end;
			_step = src._step;

			_dis();
		}

		void _dis() {
			if (_root.size()) {
				_root.clear();
			}

			// 创建均质网格
			_Tgrid rect = make_range();

			// 迭代每一个标准网格
			for (auto it = rect.begin(); it != rect.end(); ++it) {
				// 创建顶点迭代器
				_Tit rect_it(2);
				_Tit rect_it_end = rect_it.end();
				std::vector< _Tit > used;

				z_size_t i = dim;
				while (i--) {
					if (it.idxi(i) == it.maxi(i) - 1) {
						goto it_continue;
					}
				}

				// 遍历网格所有顶点，顶点个数为ZGL::Pow( 2, dim )个
				for (; rect_it != rect_it_end; ++rect_it) {
					std::vector< _Tv > re;

					_Titem v = _func(*(it + rect_it));
					if (v == _Titem(0)) {
						continue;
					}
					if (std::find(used.begin(), used.end(), rect_it) == used.end() && v < _Titem(0)) {
						_path_find(it, rect_it, re, used);
					}
					if (re.size()) {
						// 计算顶点组组成的petch
						_petching< _Tv, _Tpat, dim >(re, _root);
						// _petching(re, _root);
					}
				}

			it_continue :
				;
			}
		}

	private :

		// it : 网格迭代器
		// rect_it : 均质网格迭代器
		// res : 当前路径点的集合
		// used : 已使用过的点
		void _path_find(const _Tit& it, const _Tit& rect_it, std::vector< _Tv >& res, std::vector< _Tit >& used) {
			_Tit _begin_it = it + rect_it;
			_Titem _begin = _func(*_begin_it);

			// 迭代相邻点
			for (z_size_t i = 0; i < dim; i++) {
				// 创建相邻点的迭代器
				_Tit n_rect_it(rect_it);
				n_rect_it._idx[i] ^= z_size_t(1);

				if (std::find(used.begin(), used.end(), n_rect_it) != used.end()) {
					// 若当前点已被扫描，则中止
					continue;
				}

				_Tit _end_it = it + n_rect_it;
				_Titem _end = _func(*_end_it);

				if (_end >= _Titem(0)) {
					// 线性插值计算网格间点的位置
					res.push_back((*_begin_it) + ((*_end_it) - (*_begin_it)) * (_Titem(0) - _begin) / (_end - _begin));
				} else {
					// 若当前点在图形内部，则迭代相邻点
					used.push_back(n_rect_it);
					_path_find(it, n_rect_it, res, used);
				}
			}
		}

		/*
		void _petching(const std::vector< _Tv >& dots, std::vector< _Tpat >& res) {
			_Tpat first_pat;
			z_size_t i = 0;
			_Tv vecs[dim];
			for (decltype(auto) dot : dots) {
				if (i < dim) {
					// 使用前 dim 个点创造第一个patch
					vecs[i] = dot.second;
					if (i == (dim - 1)) {
						first_pat = _Tpat(vecs);
						res.push_back(first_pat);
					}
				} else {
					// 从第 dim 个点起，计算当前点分别与第一个patch所构成的点组成的 dim 个petch所组成的夹角，去夹角最大值作为新的patch
					_Tv temp;		// 上下文压栈
					_Tpat next_pat;	// 新patch
					_Titem arc(0);	// 新patch与第一个patch的二面角
					for (z_size_t j = 0; j < dim; j++) {
						temp = vecs[j];

						vecs[j] = dot.second;
						_Tpat t_pat(vecs);
						// 两个平面的夹角等同于其法向量的夹角，适用于更高维度的“平面”
						_Titem narc = first_pat.n().angle(t_pat.n());
						// 夹角最大的patch为新的patch，以保证其拼接的完整性
						if (!arc || narc > arc) {
							next_pat = t_pat;
							arc = narc;
						}

						vecs[j] = temp;
					}

					res.push_back(next_pat);
				}
				i++;
			}
		}
		*/

		_Tgrid make_range() {
			_Tgrid::grid_data data[dim];
			for (z_size_t i = 0; i < dim; i++) {
				data[i] = _Tgrid::grid_data(_start, _step, _end);
			}

			std::function< _Titem(typename _Tgrid::Targ, z_size_t) > funcs[dim];
			for (z_size_t i = 0; i < dim; i++) {
				funcs[i] = [](typename _Tgrid::Targ arg, z_size_t idx) { return arg[idx]; };
			}

			return STD_MOVE(_Tgrid(data, funcs));
		}
	};

_ZGL_END

#endif // !ZGL_GRIDDING_IMPLICITLY
