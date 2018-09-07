#pragma once
#include "../inc.h"
#include "../linear/matrix.h"
#include "../linear/vector.h"
#include "../linear/square.h"
#include <initializer_list>

_ZGL_BEGIN

class vec4
	:public vector< 4, z_item_t > {
	typedef z_size_t				size;
	typedef z_item_t				item;
	typedef vec4					self;
	typedef vector< 4, item >		base;
	typedef square< 4, item >		squ4;
	typedef square< 3, item >		m11;
	typedef matrix< 3, 1, item >	m12;
	typedef matrix< 1, 3, item >	m21;
	typedef square< 1, item >		m22;

public :
	self()
		: base() {}

	self(const item src[4])
		: base(src) {}

	self(const self& src)
		: base(src) {}

#ifdef ZGL_ENABLE_RVALUE
	// constructor that using Rvalue params
	// 使用右值构造
	self(self&& src)
		: base(src) { }
#endif

	self(std::initializer_list<item> src)
		: base({ src }) {}

	inline item& operator [] (size opt) const {
		return (*(const base*)this)[opt];
	}

	inline self& operator = (const self& opt) {
		(*(base*)this) = opt;
		return *this;
	}

#ifdef ZGL_ENABLE_RVALUE
	inline self& operator = (self&& src) {
		(base&)*this = (base&&)src;
		return *this;
	}
#endif
	inline bool operator == (const self& opt) const {
		return (*(const base*)this) == opt;
	}

	inline bool operator != (const self& opt) const {
		return (*(const base*)this) != opt;
	}

	inline self operator + (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this + opt));
	}

	inline self operator - (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this - opt));
	}

	inline self operator * (const item opt) const {
		return STD_MOVE((self&)(*(const base*)this * opt));
	}

	inline self operator / (const item& opt) const {
		return STD_MOVE((self&)(*(const base*)this / opt));
	}

	inline self operator / (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this / opt));
	}

	inline self operator * (const square< 4, item >& opt) const {
		return STD_MOVE((self&)(*(const base*)this * opt));
	}

	inline item operator PRO_DOT (const self& opt) const {
		return *(const base*)this PRO_DOT opt;
	}

	inline self operator PRO_PARALLEL (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this PRO_PARALLEL opt));
	}

	inline self operator PRO_UPRIGHT (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this PRO_UPRIGHT opt));
	}

	// Vector module
	// 向量长度(模)
	inline item module() const {
		return ((const base*)this)->module();
	}

	// The angle between the both of vector
	// 与向量的夹角
	inline item angle(const self& opt) const {
		return ((const base*)this)->angle(opt);
	}

	// normalize
	// 归一化
	inline void normalize() {
		(*this) = self::normalize(*this);
	}

	// Cross product
	// 向量积，外积，混合积，楔积
	static self cross(const self opt[2]) {
		// 去掉w
		vector< 3, item > _t[2], _r;
		for (size i = 0; i < 2; i++) {
			_t[i] = (vector< 3, item >)vector< 4, item >::cofactor((base)opt[i], 3);
		}
		_r = vector< 3, item >::cross(_t);
		return STD_MOVE(self::affine(_r, item(0)));
	}

	// Cross product
	// 向量积，外积，混合积，楔积
	static self cross(const std::initializer_list< self >& opt) {
		self _t[2];
		size i = 0;
		for (const self& o : opt) {
			_t[i] = o;
			i++;
		}
		return STD_MOVE(cross(_t));
	}

	// To be affine vector
	// 转换为仿射向量
	static self affine(const vector< 3, item > opt, item w) {
		self _r;
		for (size i = 0; i < 3; i++) {
			_r[i] = opt[i];
		}
		_r[3] = w;
		return STD_MOVE(_r);
	}

	// Vector normalize
	// 向量归一
	static self normalize(const self& opt) {
		self t;

		if (opt[3]) {
			for (size i = 0; i < 4; i++) {
				t[i] = opt[i] / opt[3];
			}
		} else {
			item _m = opt.module();
			if (_m) {
				for (size i = 0; i < 4; i++) {
					t[i] = opt[i] / _m;
				}
			}
		}

		return STD_MOVE(t);
	}

	static squ4 Translate(const self& normal) {
		return {
			m11(IDENTITY), m12(),
			m21(self::cofactor(normal, 3)), m22(IDENTITY)
		};
	}

	static squ4 Rotate(item yaw, item pitch, item roll) {
		typedef square<  3, item > squ3;
		squ3 mYaw = {
			{ cos(yaw), -sin(yaw), 0 },
			{ sin(yaw), cos(yaw), 0 },
			{ 0, 0, 1 },
		};
		squ3 mPitch = {
			{ cos(pitch), 0, sin(pitch) },
			{ 0, 1, 0 },
			{ -sin(pitch), 0, cos(pitch)},
		};
		squ3 mRoll = {
			{ 1, 0, 0 },
			{ 0, cos(roll), -sin(roll) },
			{ 0, sin(roll), cos(roll) },
		};

		squ3 m = mYaw * mPitch * mRoll;

		squ4 r(m, m12(), m21(), m22(IDENTITY));

		return STD_MOVE(r);
	}

	static squ4 Rotate(const vec4& position, const vec4& normal, item radian) {
		typedef vector< 3, item > vec3;

		vec3 dir = vec4::cofactor(normal, 3);
		m11 x{
			{0, normal[2], -normal[1]},
			{-normal[2], 0, normal[0]},
			{normal[1], -normal[0], 0},
		};
		// 0  c  -b
		// -c 0  a
		// b  -a 0

		m11 m_11 = m11(IDENTITY) * cos(radian) + ((m11&)(vec3::transpose(dir) * dir)) * (1.0 - cos(radian)) + x * sin(radian);
		m12 m_12;
		m21 m_21 = vec4::cofactor(position, 3) * (m_11 * -1 + m11(IDENTITY));
		m22 m_22(IDENTITY);

		return {
			m_11, m_12,
			m_21, m_22
		};
	}

	static squ4 Scale(item x, item y, item z) {
		return squ4{
			{ x, 0, 0, 0 },
			{ 0, y, 0, 0 },
			{ 0, 0, z, 0 },
			{ 0, 0, 0, 1 }
		};
	}

	static squ4 Scale(item opt) {
		return vec4::Scale(opt, opt, opt);
	}

	static squ4 perspective(const self& position, const self& normal, const self& view) {
		typedef vector< 3, item > vec3;
		vec3 
			_p = self::cofactor(position, 3),
			_n = self::cofactor(normal, 3),
			_v = self::cofactor(view, 3);

		m11 m_11 = (m11&)(vec3::transpose(_n) * _v) * item(-1) + m11(IDENTITY) * ((_v - _p) PRO_DOT _n);
		m12 m_12 = vec3::transpose(_n) * -1;
		m21 m_21 = _v * (_p PRO_DOT _n);
		m22 m_22{ { _v PRO_DOT _n } };

		return STD_MOVE(squ4(m_11, m_12, m_21, m_22));
	}
};

_ZGL_END

