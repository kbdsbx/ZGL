#include "../interface/Ilight.h"
#include "../interface/Ishadowable.h"

#include "../scene/camera.h"

#ifndef ZGL_SCENE
#define ZGL_SCENE

_ZGL_BEGIN
	
	// Scene
	// 场景
	class scene {
	public:
		// Add cameras
		// 添加相机
		virtual int add_camera(const camera* cm) { throw ""; }

		// Add solid
		// 添加实体几何
		virtual int add_solid(const Ishadowable* sd) { throw ""; }

		// Add light
		// 添加光源
		virtual int add_light(const Ilight* lt) { throw ""; }

		// Modeling transformation
		// 模型变换，将模型坐标变换为世界坐标
		virtual int modeling_trans() { throw ""; }

		virtual int after_modeling() { throw ""; }

		// Viewing Transformation
		// 视点变换，将世界坐标变换为相机坐标
		virtual int viewing_trans() { throw ""; }

		virtual int after_viewing() { throw ""; }

		// Projection Transformation
		// 投影变换，降维
		virtual int projection_trans() { throw "";  }

		virtual int after_projection() { throw ""; }

		// Viewport Transformation
		// 视口变换，将平面坐标变换为屏幕坐标
		virtual int viewport_trans() { throw ""; }

		virtual int after_viewport() { throw ""; }

		// Shadow
		// 渲染
		virtual int shadow() {

			// 1. 指定几何对象

			// 2. 视图与投影变换

			// 3. 光照计算

			// 4. 隐藏面消除

			// 5. 窗口变换

			// 6. 栅格化操作

			throw "";
		}
	};
}

#endif // !ZGL_SCENE