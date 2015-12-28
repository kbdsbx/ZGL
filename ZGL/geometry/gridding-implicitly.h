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
		typedef iterator< Pow< 2, dim - 1 >::result, _Tv > _Tkey;
		typedef patch< dim, dim - 2, _Titem > _Tpat;

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
		std::vector< _Tpat > _root;

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
		void _path_find(const _Tit& it, const _Tit& rect_it, std::map< _Tkey, _Tv >& res, std::vector< _Tit >& used) {
			_Tit _begin_it = it + rect_it;
			_Titem _begin = _func(*_begin_it);

			// �������ڵ�
			for (z_size_t i = 0; i < dim - 1; i++) {
				// �������ڵ�ĵ�����
				_Tit n_rect_it(rect_it);
				n_rect_it._idx[i] ^= z_size_t(1);

				// ����ǰ���ѱ�ʹ�ã�����ֹ
				if (std::find(used.begin(), used.end(), n_rect_it) != used.end()) {
					return;
				}

				_Tit _end_it = it + n_rect_it;
				_Titem _end = _func(*_end_it);

				// ����ǰ����ͼ���ⲿ���������λ�ò���ֹ������
				if (_end >= _Titem(0)) {
					_Tkey k(2);
					// �߶δӵ�&rect_it����..
					k._idx[&rect_it] = z_size_t(1);
					// ..����&n_rect_it����
					k._idx[&n_rect_it] = z_size_t(1);

					// ���Բ�ֵ�����������λ��
					_Tv t = (*_begin_it) + ((*_end_it) - (*_begin_it)) / (_end - _begin) * (_Titem(0) - _begin);

					res[k] = t;
					used.push_back(n_rect_it);
				}

				// ����ǰ����ͼ���ڲ�����������ڵ�
				if (_end < _Titem(0)) {
					_path_find(it, n_rect_it, res, used);
				}
			}
		}

		void _petching(const std::map< _Tkey, _Tv >& dots, std::vector< _Tpat >& res) {
			_Tpat first_pat;
			z_size_t i = 0;
			_Tv vecs[dim - 1];
			for (decltype(auto) dot : dots) {
				if (i < dim - 1) {
					// ʹ��ǰ dim - 1 ���㴴���һ��patch
					vecs[i] = dot.second;
					if (i == dim - 2) {
						first_pat = _Tpat(vecs);
						res.push_back(first_pat);
					}
				} else {
					// �ӵ� dim �����𣬼��㵱ǰ��ֱ����һ��patch�����ɵĵ���ɵ� dim ��petch����ɵļнǣ�ȥ�н����ֵ��Ϊ�µ�patch
					_Tv temp;		// ������ѹջ
					_Tpat next_pat;	// ��patch
					_Titem arc(0);	// ��patch���һ��patch�Ķ����
					for (z_size_t j = 0; j < dim - 1; j++) {
						temp = vecs[j];

						vecs[j] = dot.second;
						_Tpat t_pat(vecs);
						// ����ƽ��ļнǵ�ͬ���䷨�����ļнǣ������ڸ���ά�ȵġ�ƽ�桱
						_Titem narc = first_pat.n().angle(t_pat.n());
						// �н�����patchΪ�µ�patch���Ա�֤��ƴ�ӵ�������
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

		void _dis() {
			if (_root.size()) {
				_root.clear();
			}

			// ������������
			_Tgrid rect = make_range();

			// �������������
			auto it = rect.begin();
			for (z_size_t i = 0; i < dim - 1; i++) {
				it._max[i]--;
			}
			// ����ÿһ����׼����
			for (; it != rect.end(); ++it) {
				// ��ǰ���������
				_Titem v = _func(*it);
				if (v > _Titem(0)) {
					continue;
				}
				// �������������
				_Tit rect_it(2, it._root);
				_Tit rect_it_end = rect_it.end();

				std::vector< _Tit > used;
				used.push_back(rect_it);
				// �����������ж��㣬�������ΪZGL::Pow( 2, dim - 1 )��
				for (; rect_it != rect_it_end; ++rect_it) {
					std::map< _Tkey, _Tv > re;
					if (std::find(used.begin(), used.end(), rect_it) == used.end()) {
						_path_find(it, rect_it, re, used);
					}
					if (re.size()) {
						// ���㶥������ɵ�petch
						_petching(re, _root);
					}
				}
			}
		}

	private :

		_Tgrid make_range() {
			_Tgrid::grid_data data[dim - 1];
			for (z_size_t i = 0; i < dim - 1; i++) {
				data[i] = _Tgrid::grid_data(_start, _step, _end);
			}

			std::function< _Titem(typename _Tgrid::Targ, z_size_t) > funcs[dim - 1];
			for (z_size_t i = 0; i < dim - 1; i++) {
				funcs[i] = [](typename _Tgrid::Targ arg, z_size_t idx) { return arg[idx]; };
			}

			return STD_MOVE(_Tgrid(data, funcs));
		}
	};

_ZGL_END

#endif // !ZGL_GRIDDING_IMPLICITLY
