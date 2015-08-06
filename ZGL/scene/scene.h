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
	};
}

#endif // !ZGL_SCENE