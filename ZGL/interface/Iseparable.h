#include "../inc.h"
#include "Isolid.h"
#include <iterator>
#include <vector>

#ifndef ZGL_ISEPARABLE
#define ZGL_ISEPARABLE

_ZGL_BEGIN

	// Separable interface
	// �������񻯵�
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
					// ʹ��ǰ dim ���㴴���һ��patch
					vecs[i] = dots[i];
					first_pat = _Tpat(vecs);
					res.push_back(first_pat);
				} else {
					// �ӵ� dim �����𣬼��㵱ǰ��ֱ����һ��patch�����ɵĵ���ɵ� dim ��petch����ɵļнǣ�ȥ�н����ֵ��Ϊ�µ�patch
					_Tv temp;				// ������ѹջ
					_Tpat next_pat, t_pat;
					double arc(0), narc;
					for (z_size_t j = 0; j < dim; j++) {
						// ����ƽ��ļнǵ�ͬ���䷨�����ļнǣ������ڸ���ά�ȵġ�ƽ�桱
						temp = vecs[j];
						vecs[j] = dots[i];
						t_pat = _Tpat(vecs);
						
						narc = first_pat.n().angle(t_pat.n());

						// �н�����patchΪ�µ�patch���Ա�֤��ƴ�ӵ�������
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
