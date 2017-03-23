#include "../inc.h"
#include "../linear/square.h"
#include <math.h>

#ifndef ZGL_3D_MATRIX
#define ZGL_3D_MATRIX
_ZGL_3D_BEGIN

class matrix
	: public ZGL::square< 4, ZGL::z_item_t > {
	typedef ZGL::z_item_t _Titem;
	typedef ZGL::z_size_t z_size_t;
};

_ZGL_3D_END
#endif // !ZGL_3D_MATRIX
