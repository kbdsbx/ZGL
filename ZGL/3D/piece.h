#include "../inc.h"
#include "./vec4.h"
#include "./squ4.h"
#include <initializer_list>

#ifndef ZGL_PIECE
#define ZGL_PIECE

_ZGL_BEGIN

class piece {
	typedef z_size_t			size;
	typedef z_item_t			item;
	typedef piece				self;

public :

#ifdef ZGL_DISABLE_RVALUE
	vec4 dots[3];
#endif

#ifdef ZGL_ENABLE_RVALUE
	vec4* dots;
#endif

#ifdef ZGL_ENABLE_RVALUE
	~piece() {
		delete[] dots;
	}
#endif

	self() {
#ifdef ZGL_ENABLE_RVALUE
		dots = new vec4[3];
#endif
	};
	self(const vec4 src[3])
		: self() {
		for (size i = 0; i < 3; i++) {
			dots[i] = src[i];
		}
	}
	self(const self& src)
		: self() {
		for (size i = 0; i < 3; i++) {
			dots[i] = src[i];
		}
	}

	self(std::initializer_list< vec4 > src)
		: self() {
		size i = 0;
		for (auto& v : src) {
			dots[i] = v;
			i++;
		}
	}

#ifdef ZGL_ENABLE_RVALUE
	self(self&& src) {
		dots = src.dots;
		src.dots = nullptr;
	}
#endif

	self& operator = (const self& src) {
		for (size i = 0; i < 3; i++) {
			dots[i] = src[i];
		}
		return *this;
	}

#ifdef ZGL_ENABLE_RVALUE
	self& operator = (self&& src) {
		if (this != &src) {
			delete[] dots;
			dots = src.dots;
			src.dots = nullptr;
		}

		return *this;
	}
#endif

	vec4 operator [] (const size opt) const {
		return dots[opt];
	}

	self& transform(const squ4& squ) {
		for (size i = 0; i < 3; i++) {
			dots[i] = dots[i] * squ;
		}

		return *this;
	}
};

_ZGL_END

#endif