#include "Isolid.h"
#include <iterator>

#ifndef ZGL_ISEPARABLE
#define ZGL_ISEPARABLE

_ZGL_BEGIN

	// Separable interface
	// 允许被网格化的
	class Iseparable
		: public Isolid {
	};

_ZGL_END

#endif // !ZGL_ISEPARABLE
