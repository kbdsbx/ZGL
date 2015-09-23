#include "../interface/Igeometry.h"
#include "../interface/Icamera.h"
#include "../interface/Ilight.h"
#include "../interface/Irenderer.h"

#ifndef ZGL_SCENE
#define ZGL_SCENE

namespace ZGL {
	
	// Scene
	// 场景
	class scene {

	public :
		// Add solid geometry
		// 添加实体几何
		void add_solid(const Igeometry* geometry) {
		}

		// Add cameras
		// 添加相机
		void add_camera(const Icamera* camera) {
		}

		// Add light source
		// 添加光源
		void add_light(const Ilight* light) {
		}

		void shadow() {
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