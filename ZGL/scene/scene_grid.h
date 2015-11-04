#include "scene.h"
#include "../interface/Iseparable.h"
#include <map>
#include <vector>

#ifndef ZGL_SCENE_GRID
#define ZGL_SCENE_GRID

namespace ZGL {
	class scene_grid
		: public scene {
		// Cameras in scene
		// 场景中的摄像机
		std::map< z_size_t, const camera* > cameras;
		std::vector< const Iseparable* > grids;

	public :
		int add_camera(const camera* cm) {
			for (z_size_t i = 0; i < INT32_MAX; i++) {
				if (!cameras[i]) {
					cameras[i] = cm;
					return i;
				}
			}
			throw "so much camera!";
		}

		int add_solid(const Iseparable* sd) {
			grids.push_back(sd);
		}

		int add_light(const Ilight* lt) {
			throw "";
		}

		int dimension_reduction() {
		};
	};
}

#endif // ZGL_SCENE_GRID
