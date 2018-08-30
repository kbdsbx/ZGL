#include "../inc.h"
#include "../linear/matrix.h"
#include "../linear/vector.h"
#include "../linear/square.h"
#include <initializer_list>

#ifndef ZGL_VEC3
#define ZGL_VEC3

_ZGL_BEGIN

class vec3 
	: public vector< 3, z_item_t > {
	typedef z_size_t				size;
	typedef z_item_t				item;
	typedef vec3					self;
	typedef vector< 3, z_item_t >	base;
	typedef square< 3, item >		squ3;
	// 左上
	typedef square< 2, item >		m11;
	// 右上
	typedef matrix< 2, 1, item >	m12;
	// 左下
	typedef matrix< 1, 2, item >	m21;
	// 右下
	typedef square< 1, item >		m22;

public :
	self ()
		: base () {}

	self(const item src[3])
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
	
	item& operator [] (size opt) const {
		return (*(const base*)this)[opt];
	}
	
	self& operator = (const self& opt) {
		for (size i = 0; i < 3; i++) {
			(*this)[i] = opt[i];
		}
		return *this;
	}

#ifdef ZGL_ENABLE_RVALUE
	self& operator = (self&& src) {
		(*(base*)this = src);
		return *this;
	}
#endif
	bool operator == (const self& opt) const {
		return (*(const base*)this) == opt;
	}

	bool operator != (const self& opt) const {
		return (*(const base*)this) != opt;
	}

	self operator + (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this + opt));
	}

	self operator - (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this - opt));
	}

	self operator * (const item opt) const {
		return STD_MOVE((self&)(*(const base*)this * opt));
	}

	self operator / (const item& opt) const {
		return STD_MOVE((self&)(*(const base*)this / opt));
	}

	self operator / (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this / opt));
	}

	self operator * (const square< 3, item >& opt) const {
		return STD_MOVE((self&)(*(const base*)this * opt));
	}

	item operator PRO_DOT (const self& opt) const {
		return *(const base*)this PRO_DOT opt;
	}

	self operator PRO_PARALLEL (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this PRO_PARALLEL opt));
	}

	self operator PRO_UPRIGHT (const self& opt) const {
		return STD_MOVE((self&)(*(const base*)this PRO_UPRIGHT opt));
	}

	// Vector module
	// 向量长度(模)
	item module() const {
		return ((const base*)this)->module();
	}

	// The angle between the both of vector
	// 与向量的夹角
	item angle(const self& opt) const {
		return ((const base*)this)->angle(opt);
	}

	// normalize
	// 归一化
	void normalize() {
		(*this) = self::normalize(*this);
	}

	// Cross product
	// 向量积，外积，混合积，楔积
	static self cross(const self opt[2]) {
		// 去掉w
		vector< 2, item > _t[2], _r;
		for (size i = 0; i < 2; i++) {
			_t[i] = (vector< 2, item >)vector< 3, item >::cofactor((base)opt[i], 2);
		}
		_r = vector< 2, item >::cross(_t);
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

	// determinant value for vector array
	// 向量组行列式值
	static item determinant(const self opt[3]) {
		return base::determinant(opt);
	}

	// determinant value for vector array
	// 向量组行列式值
	static item determinant(const std::initializer_list< self >& opt) {
		return base::determinant((const std::initializer_list< base >&)opt);
	}

	// Vector Cofactor
	// 向量余子式
	static vector < 2, item > cofactor(const self & opt, size c) {
		return STD_MOVE((vector < 2, item > &)base::cofactor(opt, c));
	}

	// To be affine vector
	// 转换为仿射向量
	static self affine(const vector< 2, item > opt, item w) {
		self _r;
		for (size i = 0; i < 2; i++) {
			_r[i] = opt[i];
		}
		_r[2] = w;
		return STD_MOVE(_r);
	}

	// Vector normalize
	// 向量归一
	static self normalize(const self& opt) {
		self t;

		if (opt[2]) {
			for (size i = 0; i < 2; i++) {
				t[i] = opt[i] / opt[2];
			}
		} else {
			item _m = opt.module();
			if (_m) {
				for (size i = 0; i < 2; i++) {
					t[i] = opt[i] / _m;
				}
			}
		}

		return STD_MOVE(t);
	}

	// Vector group normalize
	// 向量组归一
	template < size rows, size cols, typename Titem >
	static matrix< rows, cols, Titem > normalize(const matrix< rows, cols, Titem >& opt) {
		matrix< rows, cols, Titem > _t(opt);

		for (size i = 0; i < rows; i++) {
			item _m = self(opt[i]).module();
			if (_m) {
				for (size j = 0; j < cols; j++) {
					_t[i][j] = _t[i][j] / _m;
				}
			}
		}

		return STD_MOVE(_t);
	}
	
	// dot or vector translation
	// 点或向量平移
	static squ3 translate(const self& trans_vector) {
		return STD_MOVE(squ3(
			m11(IDENTITY),
			m12(),
			m21(base::cofactor(trans_vector, 2)),
			m22(IDENTITY)
		));
	}
};

_ZGL_END

#endif