#include "../inc.h"
#include "../linear/vector.h"
#include <math.h>

#ifndef ZGL_3D_VECTOR
#define ZGL_3D_VECTOR

_ZGL_3D_BEGIN

class vector
	: public ZGL::vector< 4, ZGL::z_item_t > {
	typedef ZGL::z_item_t _Titem;
	typedef vector _Tself;
	typedef ZGL::z_size_t z_size_t;
	typedef ZGL::vector< 4, _Titem > _Tbase;
	typedef ZGL::square< 4, _Titem > _Tsqu;
	typedef ZGL::square< 3, _Titem > _Tm11;
	typedef ZGL::matrix< 3, 1, _Titem > _Tm12;
	typedef ZGL::matrix< 1, 3, _Titem > _Tm21;
	typedef ZGL::square< 1, _Titem > _Tm22;

	typedef ZGL::graph< 4, 3, _Titem > _Tpl;
	typedef ZGL::graph< 4, 2, _Titem > _Taxis;

public:
	_Tself()
		: _Tbase() { }

	_Tself(const _Titem src[4])
		: _Tbase(src) { }

	_Tself(const _Tself& src)
		: _Tbase(src) { }

#ifdef ZGL_ENABLE_RVALUE
	_Tself(_Tself&& src)
		: _Tbase(src) { }
#endif

	_Tself(std::initializer_list< _Titem > src)
		: _Tbase({ src }) { }

	_Titem& operator[] (z_size_t opt) const {
		return (*(const _Tbase*)this)[opt];
	}

	virtual _Tself& operator = (const _Tself& opt) {
		z_size_t i = 4;
		while (i--)
			v[0][i] = opt[i];
		return *this;
	}

#ifdef ZGL_ENABLE_RVALUE
	virtual _Tself& operator = (_Tself&& src) {
		(*(_Tbase*)this = src);
		return *this;
	}
#endif // ZGL_ENABLE_RVALUE
	bool operator == (const _Tself& opt) const {
		return (*(const _Tbase*)this) == opt;
	}

	bool operator != (const _Tself& opt) const {
		return (*(const _Tbase*)this) != opt;
	}

	_Tself operator + (const _Tself& opt) const {
		return STD_MOVE((_Tself&)(*(const _Tbase*)this + opt));
	}

	_Tself operator - (const _Tself& opt) const {
		return STD_MOVE((_Tself&)(*(const _Tbase*)this - opt));
	}

	_Tself operator * (const _Titem opt) const {
		return STD_MOVE((_Tself&)(*(const _Tbase*)this * opt));
	}

	_Tself operator / (const _Titem& opt) const {
		return STD_MOVE((_Tself&)(*(const _Tbase*)this / opt));
	}

	_Tself operator / (const _Tself& opt) const {
		return STD_MOVE((_Tself&)(*(const _Tbase*)this / opt));
	}
};

_ZGL_3D_END

#endif // !ZGL_3D_VECTOR
