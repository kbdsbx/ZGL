#include <type_traits>
#include <stdexcept>

#ifndef ZGL_INC
#define ZGL_INC

#define _ZGL_BEGIN namespace ZGL {
#define _ZGL_END }

_ZGL_BEGIN

	/**
	 * macros
	 * 宏
	 **/

	// about type
	// 类型相关
#ifdef _WIN64
	typedef __int64 z_size_t;
	typedef long double z_item_t;
#else
	typedef __int32 z_size_t;
	typedef double z_item_t;
#endif // !_WIN64

	// Right handed system
	// 右手坐标系
#define RIGHT_HANDED_SYSTEM


	// debugging situation and rvalue references
	// 调试环境与右值
#if defined _DEBUG || ! defined __cpp_rvalue_references

	#define ZGL_DISABLE_RVALUE
	#define STD_MOVE
	#define ZGL_RVALUE(T) ((T)&)
	
#else

	#define ZGL_ENABLE_RVALUE
	#define STD_MOVE std::move
	#define ZGL_RVALUE(T) (T&&)

#endif

#if defined _DEBUG
#define private public
#endif


	/**
	 * enums
	 * 枚举
	 **/

	// types of square matrix
	// 初始化矩阵类型
	enum ZGL_SQUARE_MATRIX_TYPE {
		// Zero matrix
		// 零矩阵
		ZERO = 0,
		// Identity matrix
		// 单位矩阵
		IDENTITY = 1,
	};

	/**
	 * operator
	 * 运算符
	 **/

	// vector operations
	// 向量运算

	// dot metrix
	// 点积
	#define PRO_DOT %

	// cross product
	// 叉积
	#define PRO_CROSS |

	// parallel projection
	// 平行投影
	#define PRO_PARALLEL ||

	// upright projection
	// 垂直投影
	#define PRO_UPRIGHT &&

	/**
	 * const
	 * 常量
	 **/

	// π
	#define PI 3.14159265358979

	/**
	 * About precision
	 * 精度相关
	 **/

	// float epsinon
	// 浮点精度差
	#define FLOAT_EPSINON 10e-5
	// compare two float if they are equality
	// 比较两个浮点数是否相等
	#define FLOAT_EQ( x, y ) (((x) - (y) >= -FLOAT_EPSINON) && ((x) - (y) <= FLOAT_EPSINON))

	// double epsinon
	// 双精度浮点精度差
	#define DOUBLE_EPSINON 10e-13
	// compare two double if they are equality
	// 比较两个双精度浮点数是否相等
	#define DOUBLE_EQ( x, y ) (((x) - (y) >= -DOUBLE_EPSINON) && ((x) - (y) <= DOUBLE_EPSINON))

	// long double epsinon
	// 长双精度浮点精度差
	#define LONG_DOUBLE_EPSINON 10e-15
	// compare two long double if they are equality
	// 比较两个长双精度浮点数是否相等
	#define LONG_DOUBLE_EQ( x, y ) (((x) - (y) >= -LONG_DOUBLE_EPSINON) && ((x) - (y) <= LONG_DOUBLE_EPSINON))

	using invalid_type = std::invalid_argument;

	template < typename T >
	inline bool floating_eq(T x, T y) { throw invalid_type( "Comparing floating type only." ); }

	template <>
	inline bool floating_eq(float x, float y) { return (x - y) >= -(10e-5) && (x - y) <= (10e-5); }

	template <>
	inline bool floating_eq(double x, double y) { return (x - y) >= -(10e-13) && (x - y) <= (10e-13); }

	template <>
	inline bool floating_eq(long double x, long double y) { return (x - y) >= -(10e-15) && (x - y) <= (10e-15); }

	/**
	 * classes declaration
	 * 类声明
	 **/

	// linear
	// 线性代数
	template < z_size_t rows, z_size_t cols, typename Titem, typename >
	class matrix;

	template < z_size_t rows, z_size_t cols, typename Titem >
	class matrix < rows, cols, Titem, typename std::enable_if_t < (rows < 0) || (cols < 0) > >;

	template < z_size_t dim, typename Titem, typename >
	class square;

	template < z_size_t dim, typename Titem >
	class square < dim, Titem, typename std::enable_if_t < (dim < 0) > >;

	template < z_size_t dim, typename Titem, typename >
	class vector;

	// geometry
	// 几何

	template < z_size_t dim, typename Titem >
	class affine_vector;

	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class graph;

	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding;

	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class gridding_implicitly;

	template < z_size_t dim, typename Titem >
	class iterator;

	template < z_size_t dim, z_size_t d_dim, typename Titem >
	class patch;

	// Interface
	// 接口

	// Math
	// 数学
	template < z_size_t Base, z_size_t Exp >
	class Pow {
	public :
		enum { result = Base * Pow < Base, Exp - 1 >::result };
	};

	template < z_size_t Base >
	class Pow < Base, 0 > {
	public :
		enum { result = 1 };
	};

_ZGL_END

#endif // !ZGL_INC
