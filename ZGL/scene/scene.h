#include "../interface/Ilight.h"
#include "../interface/Ishadowable.h"

#include "../scene/camera.h"

#ifndef ZGL_SCENE
#define ZGL_SCENE

namespace ZGL {
	
	// Scene
	// 场景
	class scene {
	public:
		// Add cameras
		// 添加相机
		virtual int add_camera(const camera* cm) = 0;

		// Add solid
		// 添加实体几何
		virtual int add_solid(const Ishadowable* sd) = 0;

		// Add light
		// 添加光源
		virtual int add_light(const Ilight* lt) = 0;

		// Dimension reduction
		// 降维
		virtual int dimension_reduction() { };

		// hidden-x removal
		// 隐藏消除
		virtual int hidden_removal() { };

		// Shadow
		// 渲染
		virtual int shadow() {

			// 1. 指定几何对象

			// 2. 视图与投影变换

			// 3. 光照计算

			// 4. 隐藏面消除

			// 5. 窗口变换

			// 6. 栅格化操作
		}
	};
}

#endif // !ZGL_SCENE