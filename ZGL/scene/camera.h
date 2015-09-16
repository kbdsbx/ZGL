#include "../inc.h"
#include "../interface/Icamera.h"
#include "../geometry/affine_vector.h"
#include "../geometry/graph.h"

#ifndef ZGL_CAMERA
#define ZGL_CAMERA

namespace ZGL {

	// Camera in 3D
	// 3D相机
	class camera
		: Icamera {
		typedef camera _Tself;
		typedef double _Titem;
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
			: _position(std::move(src._position)), _forward(std::move(src._forward)), _upward(std::move(src._upward)) { }

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
		_Tm4 view() {
			normalize();
			_Tm4 r{
				{ _rightward[0], _upward[0], _forward[0], 0 },
				{ _rightward[1], _upward[1], _forward[1], 0 },
				{ _rightward[2], _upward[2], _forward[2], 0 },
				{ 0, 0, 0, 1 },
			};
			return r * (_Tv4::translate(_position) ^ -1);
		}
	};
}

#endif;