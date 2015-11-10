#include "scene.h"
#include "unit/surface.h"
#include <map>
#include <vector>

#ifndef ZGL_SCENE_GRID
#define ZGL_SCENE_GRID

_ZGL_BEGIN
	class scene_grid
		: public scene {
		typedef __int8 _Tid;
		typedef __int32 _Tpx;
		typedef affine_vector< 4, z_item_t > _Tv4;
		// Cameras in scene
		// 场景中的摄像机
		std::map< _Tid, const camera* > cameras;
		std::vector< const surface* > grids;

	public :

		std::vector< surface* > material_grids;

		~scene_grid() {
			/*
			// Why it mustn't be delete ?
			for (auto grids : material_grids)
				delete grids;
			*/
		}

		int add_camera(const camera* cm) {
			for (_Tid i = INT8_MIN; i < INT8_MAX; i++) {
				if (!cameras[i]) {
					cameras[i] = cm;
					return i;
				}
			}
			throw "so much camera!";
		}

		int add_solid(const surface* sd) {
			grids.push_back(sd);
			return 0;
		}

		int modeling_trans() {
			material_grids.clear();
			if (grids.size() == material_grids.size() && grids.size() != 0) {
				for (z_size_t i = 0; i < grids.size(); i++)
					*(material_grids[i]) = *(grids[i]);
			} else {
				for (auto grid : material_grids)
					delete grid;

				for (decltype(auto) grid : grids)
					material_grids.push_back(new surface(*grid));
			}

			return 0;
		}

		int viewing_trans(_Tid cm_id) {
			auto _m = cameras[cm_id]->view();

			// Translate to camera view
			// 相机空间
			for (decltype(auto) grid : material_grids) {
				for (decltype(auto) p : *grid) {
					p = p * _m;
				}
			}

			return 0;
		}

		int after_viewing(z_item_t zMin, z_item_t zMax, z_item_t xRad, z_item_t yRad) {
			// hidden-x removal
			// 隐藏消除
		}

		int projection_trans(z_item_t zMin) {
			graph< 4, 2, z_item_t > plane({ 0, 0, zMin, 1 }, {
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
			});

			auto _tm = _Tv4::orthogonal(plane);

			// projection transformation
			// 投影变换
			for (decltype(auto) grid : material_grids) {
				for (decltype(auto) p : *grid) {
					p = p * _tm;
				}
			}

			return 0;
		}

		int viewport_trans(z_item_t scale, _Tpx width, _Tpx height) {
			// viewport transformation
			// 视口变换
			for (decltype(auto) grid : material_grids) {
				for (decltype(auto) p : *grid) {
					p[0] = p[0] * scale + width / 2;
					p[1] = -p[1] * scale + height / 2;
				}
			}

			return 0;
		}
	};

_ZGL_END

#endif // ZGL_SCENE_GRID
