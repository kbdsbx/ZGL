#pragma once

#include "../inc.h"
#include "../3D/vec4.h"
#include "../3D/squ4.h"

_ZGL_BEGIN

class camera {
	typedef camera		self;
	typedef z_item_t	item;
	typedef square< 3, item>	m11;
	typedef matrix< 3, 1, item>	m12;
	typedef matrix< 1, 3, item>	m21;
	typedef square< 1, item>	m22;

	vec4 _position;
	vec4 _upward;
	vec4 _forward;
	vec4 _rightward;

public :
	item zoom_in = 200;
	item zoom_out = 9999999;

	camera() {
	}


	camera(const vec4& position, const vec4& forward, const vec4& upward)
		: _position(position),
		_forward(forward),
		_upward(upward) {
		this->normalize();
	}

#ifdef ZGL_ENABLE_RVALUE

	camera(vec4&& position, vec4&& forward, vec4&& upward)
		:_position(position),
		_forward(forward),
		_upward(upward) {
		this->normalize();
	}

#endif

	camera(const self& src)
		:_position(src._position),
		_forward(src._forward),
		_upward(src._upward) {
		this->normalize();
	}

	squ4 view() const {
		return STD_MOVE(squ4{
			{ this->_rightward[0], this->_rightward[1], this->_rightward[2], 0 },
			{ this->_upward[0], this->_upward[1], this->_upward[2], 0 },
			{ this->_forward[0], this->_forward[1], this->_forward[2], 0 },
			{ this->_position[0], this->_position[1], this->_position[2], 1 }
		} ^ -1);
	}

	const vec4& position() {
		return this->_position;
	}

	const vec4& upward() {
		return this->_upward;
	}

	const vec4& forward() {
		return this->_forward;
	}

	const vec4& rightward() {
		return this->_rightward;
	}

	// 摄像机所关联的视口位置
	const vec4& screen_position() {
		return this->_position + this->_forward * this->zoom_in;
	}

	// 摄像机所关联的视口法向量
	const vec4& screen_normal() {
		return this->_forward * -1;
	}

	void normalize() {
		this->_rightward = vec4::cross({
			this->_forward,
			this->_upward,
		});

		this->_upward = vec4::cross({
			this->_rightward,
			this->_forward,
		});

		this->_upward.normalize();
		this->_forward.normalize();
		this->_rightward.normalize();
	}

	// 偏航
	squ4& yaw(item rad) {
		squ4 m = vec4::Rotate(this->_position, this->_upward, rad);
		this->_forward = this->_forward * m;
		this->normalize();

		return STD_MOVE(m ^ -1);
	}

	// 翻滚
	squ4& roll(item rad) {
		squ4 m = vec4::Rotate(this->_position, this->_forward, rad);
		this->_upward = this->_upward * m;
		this->normalize();
		return STD_MOVE(m ^ -1);
	}

	// 俯仰
	squ4& pitch(item rad) {
		squ4 m = vec4::Rotate(this->_position, this->_rightward, rad);
		this->_forward = this->_forward * m;
		this->normalize();

		return STD_MOVE(m ^ -1);
	}

	// 左右移动
	squ4& traverse(item len) {
		vec4 m = this->_rightward * len;
		this->_position = this->_position + m;

		return squ4{
			m11(IDENTITY), m12(),
			vec4::cofactor(m, 3) * -1, m22(),
		};
	}

	// 上下移动
	squ4& lift(item len) {
		vec4 m = this->_upward * len;
		this->_position = this->_position + m;

		return squ4{
			m11(IDENTITY), m12(),
			vec4::cofactor(m, 3) * -1, m22(),
		};
	}

	// 前后移动
	squ4& retreat(item len) {
		vec4 m = this->_forward * len;
		this->_position = this->_position + m;

		return squ4{
			m11(IDENTITY), m12(),
			vec4::cofactor(m, 3) * -1, m22(),
		};
	}
};

_ZGL_END