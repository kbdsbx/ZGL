#include "scene.h"
#include "unit/surface.h"
#include <map>
#include <vector>

#ifndef ZGL_SCENE_GRID
#define ZGL_SCENE_GRID

namespace ZGL {
	class scene_grid
		: public scene {
		typedef __int8 _Tid;
		// Cameras in scene
		// 场景中的摄像机
		std::map< _Tid, camera* > cameras;
		std::vector< const surface& > grids;

	public :
		int add_camera(camera* cm) {
			for (_Tid i = INT8_MIN; i < INT8_MAX; i++) {
				if (!cameras[i]) {
					cameras[i] = cm;
					return i;
				}
			}
			throw "so much camera!";
		}

		int add_solid(const surface& sd) {
			grids.push_back(sd);
			return 0;
		}

		int add_light(const Ilight* lt) {
			throw "";
		}

		int dimension_reduction(_Tid cm_id) {
			auto _m = cameras[cm_id]->view();
			for (auto grid : grids)
				for (auto p : grid)
					p = p * _m;
		};
	};
}

#endif // ZGL_SCENE_GRID
