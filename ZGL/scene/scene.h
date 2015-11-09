#include "../interface/Ilight.h"
#include "../interface/Ishadowable.h"

#include "../scene/camera.h"

#ifndef ZGL_SCENE
#define ZGL_SCENE

_ZGL_BEGIN
	
	// Scene
	// ����
	class scene {
	public:
		// Add cameras
		// ������
		virtual int add_camera(const camera* cm) { throw ""; }

		// Add solid
		// ���ʵ�弸��
		virtual int add_solid(const Ishadowable* sd) { throw ""; }

		// Add light
		// ��ӹ�Դ
		virtual int add_light(const Ilight* lt) { throw ""; }

		// Modeling transformation
		// ģ�ͱ任����ģ������任Ϊ��������
		virtual int modeling_trans() { throw ""; }

		virtual int after_modeling() { throw ""; }

		// Viewing Transformation
		// �ӵ�任������������任Ϊ�������
		virtual int viewing_trans() { throw ""; }

		virtual int after_viewing() { throw ""; }

		// Projection Transformation
		// ͶӰ�任����ά
		virtual int projection_trans() { throw "";  }

		virtual int after_projection() { throw ""; }

		// Viewport Transformation
		// �ӿڱ任����ƽ������任Ϊ��Ļ����
		virtual int viewport_trans() { throw ""; }

		virtual int after_viewport() { throw ""; }

		// Shadow
		// ��Ⱦ
		virtual int shadow() {

			// 1. ָ�����ζ���

			// 2. ��ͼ��ͶӰ�任

			// 3. ���ռ���

			// 4. ����������

			// 5. ���ڱ任

			// 6. դ�񻯲���

			throw "";
		}
	};
}

#endif // !ZGL_SCENE