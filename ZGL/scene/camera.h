#include "../inc.h"
#include "../interface/Icamera.h"
#include "../geometry/affine_vector.h"
#include "../geometry/graph.h"

#ifndef ZGL_CAMERA
#define ZGL_CAMERA

_ZGL_BEGIN

	// Camera in 3D
	// 3D���
	class camera
		: Icamera {
		typedef camera _Tself;
		typedef z_item_t _Titem;
		typedef affine_vector< 4, _Titem > _Tv4;
		typedef square< 4, _Titem > _Tm4;
		typedef graph< 4, 1, _Titem > _Tline;

		_Tv4 _position;
		_Tv4 _upward;
		_Tv4 _forward;
		_Tv4 _rightward;

	public :
		camera(const _Tv4& position, const _Tv4& forward, const _Tv4& upward)
			: _position(position), _forward(forward), _upward(upward) {
			normalize();
		}

		camera(_Tv4&& position, _Tv4&& forward, _Tv4&& upward)
			: _position(position), _forward(forward), _upward(upward) {
			normalize();
		}

		camera(const _Tself& src)
			: _position(src._position), _forward(src._forward), _upward(src._upward) { }

		camera(_Tself&& src)
			: _position(STD_MOVE(src._position)), _forward(STD_MOVE(src._forward)), _upward(STD_MOVE(src._upward)) { }

		// dot for position
		// ���λ��
		_Tv4 position() {
			return _position;
		}

		// vector for upward
		// ����������
		_Tv4 upward() {
			return _upward;
		}

		// vector for forward
		// ��ǰ�����J
		_Tv4 forward() {
			return _forward;
		}

		// vector for rightward
		// ����������
		_Tv4 rightward() {
			return _rightward;
		}

		// normalize
		// ��׼��
		void normalize() {
			// right headed system
			// ��������ϵ
			_rightward = _Tv4::cross({
				_forward,
				_upward,
			});

			// refer forward vector to normalize upward vector
			// ����ǰ����Ϊ�ο���׼����������
			_upward = _Tv4::cross({
				_rightward,
				_forward,
			});

			_upward.normalize();
			_forward.normalize();
			_rightward.normalize();
		}

		// Camera transformation matrix
		// ����任����
		_Tm4 view() const {
			return _Tm4 {
				{ _rightward[0], _upward[0], _forward[0], 0 },
				{ _rightward[1], _upward[1], _forward[1], 0 },
				{ _rightward[2], _upward[2], _forward[2], 0 },
				{ _position[0] * -1, _position[1] * -1, _position[2] * -1, 1 },
			};
		}

		// Yaw
		// ƫ��
		void yaw(_Titem rad) {
			_forward = _forward * _Tv4::rotate(_Tline(_position, _upward), rad);
			normalize();
		}

		// Roll
		// ����
		void roll(_Titem rad) {
			_upward = _upward * _Tv4::rotate(_Tline(_position, _forward), rad);
			normalize();
		}

		// Pitch
		// ����
		void pitch(_Titem rad) {
			_forward = _forward * _Tv4::rotate(_Tline(_position, _rightward), rad);
			normalize();
		}

		// Traverse
		// ����
		void traverse(_Titem len) {
			_position = _position + _rightward * len;
		}

		// Lift
		// ����
		void lift(_Titem len) {
			_position = _position + _upward * len;
		}

		// Retreat
		// ����
		void retreat(_Titem len) {
			_position = _position + _forward * len;
		}
	};
}

#endif;