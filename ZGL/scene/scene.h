#include "../interface/Igeometry.h"
#include "../interface/Icamera.h"
#include "../interface/Ilight.h"
#include "../interface/Irenderer.h"

#ifndef ZGL_SCENE
#define ZGL_SCENE

namespace ZGL {
	
	// Scene
	// ����
	class scene {

	public :
		// Add solid geometry
		// ���ʵ�弸��
		void add_solid(const Igeometry* geometry) {
		}

		// Add cameras
		// ������
		void add_camera(const Icamera* camera) {
		}

		// Add light source
		// ��ӹ�Դ
		void add_light(const Ilight* light) {
		}

		void shadow() {
			// 1. ָ�����ζ���

			// 2. ��ͼ��ͶӰ�任

			// 3. ���ռ���

			// 4. ����������

			// 5. ���ڱ任

			// 6. դ�񻯲���
		}
	};
}

#endif // !ZGL_SCENE