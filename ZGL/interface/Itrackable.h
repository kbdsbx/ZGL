#include "../inc.h"
#include "Ishadowable.h"

#ifndef ZGL_ITRACKABLE
#define ZGL_ITRACKABLE

namespace ZGL {
	// Trackable interface
	// 允许被光线追踪渲染
	class Itrackable
		: public Ishadowable {
	};
}

#endif // !ZGL_ITRACKABLE
