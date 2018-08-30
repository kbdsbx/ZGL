#include "../inc.h"
#include "../geometry/affine_vector.h"
#include "../geometry/graph.h"

#ifndef ZGL_CAMERA
#define ZGL_CAMERA

_ZGL_BEGIN

	// Camera in 3D
	// 3D相机
	class camera
		 {
		typedef camera _Tself;
		typedef z_item_t _Titem;
		typedef affine_vector< 3, _Titem > _Tv4;
		typedef square< 4, _Titem > _Tm4;
		typedef graph< 3, 1, _Titem > _Tline;

		_Tv4 _position;
		_Tv4 _upward;
		_Tv4 _forward;
		_Tv4 _rightward;

	public :
		// Type of transition matrix
		// 变换矩阵类型
		typedef _Tm4 Tm;

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
		// 相机位置
		_Tv4 position() {
			return _position;
		}

		// vector for upward
		// 向上向量↑
		_Tv4 upward() {
			return _upward;
		}

		// vector for forward
		// 向前向量↗
		_Tv4 forward() {
			return _forward;
		}

		// vector for rightward
		// 向右向量→
		_Tv4 rightward() {
			return _rightward;
		}

		// normalize
		// 标准化
		void normalize() {
			// right headed system
			// 右手坐标系
			_rightward = _Tv4::cross({
				_forward,
				_upward,
			});

			// refer forward vector to normalize upward vector
			// 以向前向量为参考标准化向上向量
			_upward = _Tv4::cross({
				_rightward,
				_forward,
			});

			_upward.normalize();
			_forward.normalize();
			_rightward.normalize();
		}

		// Camera transformation matrix
		// 相机变换矩阵
		_Tm4 view() const {
			return STD_MOVE(_Tm4 {
				{ _rightward[0], _rightward[1], _rightward[2], 0 },
				{ _upward[0], _upward[1], _upward[2], 0 },
				{ _forward[0], _forward[1], _forward[2], 0 },
				{ _position[0], _position[1], _position[2], 1 },
			} ^ -1);
		}

		// Yaw
		// 偏航
		_Tm4 yaw(_Titem rad) {
			_Tm4 m = _Tv4::rotate(_Tline(_position, _upward), rad);
			_forward = _forward * m;
			normalize();
			return STD_MOVE(m ^ -1);
		}

		// Roll
		// 翻滚
		_Tm4 roll(_Titem rad) {
			_Tm4 m = _Tv4::rotate(_Tline(_position, _forward), rad);
			_upward = _upward * m;
			normalize();
			return STD_MOVE(m ^ -1);
		}

		// Pitch
		// 俯仰
		_Tm4 pitch(_Titem rad) {
			_Tm4 m = _Tv4::rotate(_Tline(_position, _rightward), rad);
			_forward = _forward * m;
			normalize();
			return STD_MOVE(m ^ -1);
		}

		// Traverse
		// 横移
		_Tm4 traverse(_Titem len) {
			_Tv4 m = _rightward * len;
			_position = _position + m;
			return STD_MOVE(_Tm4{
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ m[0] * -1, m[1] * -1, m[2] * -1, 1 },
			});
		}

		// Lift
		// 升降
		_Tm4 lift(_Titem len) {
			_Tv4 m = _upward * len;
			_position = _position + m;
			return STD_MOVE(_Tm4{
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ m[0] * -1, m[1] * -1, m[2] * -1, 1 },
			});
		}

		// Retreat
		// 进退
		_Tm4 retreat(_Titem len) {
			_Tv4 m = _forward * len;
			_position = _position + m;
			return STD_MOVE(_Tm4{
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ m[0] * -1, m[1] * -1, m[2] * -1, 1 },
			});
		}
	};

_ZGL_END

#endif;