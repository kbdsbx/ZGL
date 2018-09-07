#pragma once

#include "../inc.h"
#include "./object.h"
#include "./camera.h"
#include "./squ4.h"
#include "./vec4.h"

#include <map>
#include <functional>

_ZGL_BEGIN

class scene {
	typedef scene		self;
	typedef z_size_t	size;
public:
	std::map< size, object* > _objects;
	std::map< size, camera* > cameras;
	std::map< size, object > objects;
	size curr_camera_key = 0;

	self(){ };

	self& add_object(size k, object* obj) {
		this->_objects.insert(std::pair< size, object* >(k, obj));
		return *this;
	}

	self& replace_object(size k, object* obj) {
		this->_objects.insert(std::pair< size, object* >(k, obj));
		return *this;
	}

	self& remove_object(size k ) {
		this->_objects.erase(k);
		return *this;
	}

	self& add_camera(size k, camera* cma) {
		if (this->curr_camera_key == 0) {
			this->curr_camera_key = k;
		}
		this->cameras.insert(std::pair< size, camera* >(k, cma));
		return *this;
	}

	self& replace_camera(size k, camera* cma) {
		this->cameras.insert(std::pair< size, camera* >(k, cma));
		return *this;
	}

	self& remove_camera(size k) {
		this->cameras.erase(k);
		return *this;
	}

	// ���õ�ǰ���
	self& change_camera(size k) {
		this->curr_camera_key = k;
		return *this;
	}

	self& transform() {
		camera* curr_camera = this->cameras[this->curr_camera_key];

		squ4 ts;
		// �������任��ģ��������������������ϵ�任���������ϵ
		squ4 cam_s = curr_camera->view();
		// �������ϵͶӰ�任����ֱ��ͶӰ����x���y����ɵ�ƽ����
		squ4 per_s = vec4::perspective({ 0, 0, curr_camera->zoom_in, 1 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 });

		ts = cam_s * per_s;

		for (decltype(auto) i = this->_objects.begin(); i != this->_objects.end(); i++) {
			this->objects[i->first] = object( *(i->second) );
			this->objects[i->first]
				// �ӿڱ任��ͶӰ�任
				.transform(ts)
				// ʹ�����������м���
				.each([curr_camera](ZGL::piece& p) {
					if (
						p[0][3] > 0 ||
						p[1][3] > 0 ||
						p[2][3] > 0
						) {
						p.show = false;
					}
				} )
				// ���ú��һ��
				.normalize();
		}

		return *this;
	}

	self& draw_line(std::function< void(float, float, float, float) > draw) {
		camera* curr_camera = this->cameras[this->curr_camera_key];
		for (auto i = this->objects.begin(); i != this->objects.end(); i++) {
			auto obj = i->second;
			for (size i = 0; i < obj.count; i++) {
				// �����ӽ���������
				if (! obj[i].show) {
					continue;
				}
				draw( obj[i][0][0], obj[i][0][1], obj[i][1][0], obj[i][1][1]);
				draw( obj[i][1][0], obj[i][1][1], obj[i][2][0], obj[i][2][1]);
				draw( obj[i][2][0], obj[i][2][1], obj[i][0][0], obj[i][0][1]);
			}
		}

		return *this;
	}
};

_ZGL_END
