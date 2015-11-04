#include "../interface/Ilight.h"
#include "../interface/Ishadowable.h"

#include "../scene/camera.h"

#ifndef ZGL_SCENE
#define ZGL_SCENE

namespace ZGL {
	
	// Scene
	// ����
	class scene {
	public:
		// Add cameras
		// ������
		virtual int add_camera(const camera* cm) = 0;

		// Add solid
		// ���ʵ�弸��
		virtual int add_solid(const Ishadowable* sd) = 0;

		// Add light
		// ��ӹ�Դ
		virtual int add_light(const Ilight* lt) = 0;

		// Dimension reduction
		// ��ά
		virtual int dimension_reduction() { };

		// hidden-x removal
		// ��������
		virtual int hidden_removal() { };

		// Shadow
		// ��Ⱦ
		virtual int shadow() {

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