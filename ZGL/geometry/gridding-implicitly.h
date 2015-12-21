#include "../inc.h"
#include "../interface/Iseparable.h"
#include <math.h>
#include <functional>
#include <initializer_list>
#include <vector>
#include <map>
#include "gridding.h"
#include "patch.h"

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
		typedef gridding< dim, dim - 1, _Titem > _Tgrid;
		typedef iterator< dim - 1, _Tv > _Tit;
		typedef unsigned int _Tmk;
		typedef patch< dim, 2, _Titem > _Tsegment;

	private :
		_Titem _start;
		_Titem _step;
		_Titem _end;

	public :

		// Equation implicitly argument type
		// ���꺯����������
		typedef _Targ Targ;

	public :
		///** Result **///
		
		// nodes
		// �ڵ�
		std::vector< _Tv > _root;

		///** coordinate Range and implicit function **///

		// implicit function
		// ��ʽ����
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

		// it : ���������
		// rect_it : �������������
		// res : ��ǰ·����ļ���
		// used : ��ʹ�ù��ĵ�
		void _path_find(const _Tit& it, const _Tit& rect_it, std::map< _Tmk, _Tv >& res, std::vector< _Tmk >& used) {
			_Titem v = _func(*it);
			auto nit = it + rect_it;
			_Tmk k(0); // ʹ��λ��Ǵ����������¼
			for (z_size_t i = 0; i < dim - 1; i++) {
				k |= ((_Tmk)(nit._idx[i]) << i);
			}

			// ����ǰ���ѱ�ʹ�ã���ǰ·������Ѱ·��������ֹ
			if (std::find(used.begin(), used.end(), k) != used.end() || res[k] != _Tv()) {
				return;
			}

			_Titem nv = _func(*nit);

			// ����ǰ����ͼ���ⲿ���������λ�ò���ֹ������
			if (nv >= _Titem(0)) {
				// ���Բ�ֵ�����������λ��
				_Tv t = (*it) + ((*nit) - (*it)) / (nv - v) * v;
				res[k] = t;
				used.push_back(k);
			}

			// ����ǰ����ͼ���ڲ�����������ڵ�
			if (nv < _Titem(0)) {
				for (z_size_t i = 0; i < dim - 1; i++) {
					auto nrect_it(rect_it);
					nrect_it._idx[i] ^= 1;

					_path_find(it, nrect_it, res, used);
				}
			}
		}

		void _dis() {
			if (_root.size()) {
				_root.clear();
			}

			// ������������
			_Tgrid rect = make_range();

			// �������������
			auto it = rect.begin();
			for (z_size_t i = 0; i < dim - 1; i++) {
				it._max[i] = it._max[i] - 1;
			}
			// ����ÿһ����׼����
			for (; it != rect.end(); ++it) {
				// ��ǰ���������
				_Titem v = _func(*it);
				if (v > _Titem(0)) {
					continue;
				}
				vector< dim, z_size_t > rect_idx;
				for (z_size_t i = 0; i < dim - 1; i++) {
					rect_idx[i] = 2;
				}
				// �������������
				iterator< dim - 1, _Tv > rect_it(rect_idx, it._root);
				iterator< dim - 1, _Tv > rect_it_end(rect_idx, it._root);
				rect_it_end._idx[dim] = 1;
				std::vector< std::map< _Tmk, _Tv > >& res;
				std::vector< _Tmk >& used;
				// �����������ж��㣬�������Ϊ2 ^ dim��
				for (; rect_it != rect_it_end; ++rect_it) {
					std::map< _Tmk, _Tv > re;
					_path_find(it, rect_it, re, used);
					if (re.size()) {
						res.push_back(re);
					}
				}

				for (decltype(auto) mp : res) {
				}

				/*
				for (z_size_t i = 0; i < dim - 1; i++) {
					auto nit(it);
					nit._idx[i] = nit._idx[i] + 1;
					if (nit._idx[i] >= nit._max[i]) {
						continue;
					}
					_Titem nv = _func(*nit);
					if (v <= _Titem(0) && nv > _Titem(0) || v > _Titem(0) && nv <= _Titem(0)) {
						_Tv t(*it);
						t[i] = t[i] + _step * .5;
						_root.push_back(t);
					}
				}
				*/
			}
		}

		void _loop(const grid_range range[dim - 1], z_size_t deep) {
			_Tgrid rect = make_range(range);

			for (auto it = rect.begin(); it != rect.end(); ++it) {
				// ����淶�������ݶ���Ϣ
				_Titem v = _func(*it);
				for (z_size_t i = 0; i < d_dim; i++) {
					auto nit(it);
					nit._idx[i] = (nit._idx[i] == 0 ? 1 : 0);
					_Titem nv = _func(*nit);
					// ���������ڵĵ㲻ͬʱ����ʽ����һ�࣬����ʽ����ı�������֮����߶��ཻ
					if (v <= _Titem(0) && nv > _Titem(0) || v > _Titem(0) && nv <= _Titem(0)) {
						// ��С��Χ
						grid_range nrange[d_dim];
						for (z_size_t d = 0; d < d_dim; d++) {
							_Titem len = range[d].maxi() - range[d].mini();
							if (nit._idx[d]) {
								nrange[d] = grid_range(range[d].mini(), range[d].maxi() - len);
							} else {
								nrange[d] = grid_range(range[d].mini() + len, range[d].maxi());
							}
						}
						_loop(nrange, deep - 1);	// ʹ�ö��ַ���������
					}
				}

				// ���������ڵ����е㶼����ʽ�����һ�࣬����ʽ����ı߲���������ཻ��������������
				// �����Ҫ�����������⣬�����һ���ж�
			}
		}

	private :

		_Tgrid make_range() {
			_Tgrid::grid_data data[dim - 1];
			for (z_size_t i = 0; i < dim - 1; i++)
				data[i] = _Tgrid::grid_data(_start, _step, _end);

			std::function< _Titem(typename _Tgrid::Targ, z_size_t) > funcs[dim - 1];
			for (z_size_t i = 0; i < dim - 1; i++)
				funcs[i] = [](typename _Tgrid::Targ arg, z_size_t idx) { return arg[idx]; };

			return STD_MOVE(_Tgrid(data, funcs));
		}
	};

_ZGL_END

#endif // !ZGL_GRIDDING_IMPLICITLY
