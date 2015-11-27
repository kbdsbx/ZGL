#include "../inc.h"
#include <map>
#include <functional>
#include <vector>

#ifndef ZGL_MOUSE
#define ZGL_MOUSE

_ZGL_BEGIN

	enum ZGL_MOUSE_TYPE {
		LEFT_CLICK = 0x01U,
		RIGHT_CLICK = 0x02U,
		MIDDLE_CLICK = 0x04U,
		MOVE = 0x08U,
		WHEEL = 0x10U,
	};

	class mouse {
		typedef unsigned int _Tflag;
		typedef int _Tpx;
	public :
		struct mouse_status {
			_Tpx x = 0;
			_Tpx y = 0;
			_Tpx wheel = 0;
			_Tflag flags = 0U;

			void set_left() {
				flags |= LEFT_CLICK;
			}

			void reset_left() {
				flags &= ~LEFT_CLICK;
			}

			void set_right() {
				flags |= RIGHT_CLICK;
			}

			void reset_right() {
				flags &= ~RIGHT_CLICK;
			}

			void set_mid() {
				flags |= MIDDLE_CLICK;
			}

			void reset_mid() {
				flags &= ~MIDDLE_CLICK;
			}

			void set_move() {
				flags |= MOVE;
			}

			void reset_move() {
				flags &= ~MOVE;
			}

			void set_wheel() {
				flags |= WHEEL;
			}

			void reset_wheel() {
				flags &= ~WHEEL;
			}
		};
	private :
		std::map< z_size_t, std::vector< std::function< void(const mouse_status&) > > > _ops;

		mouse_status _msg;
	public :

		// events binding
		// 事件绑定
		// Return : the event's function id in current event.
		// 返回 : 当前事件的绑定方法索引
		size_t on(z_size_t opt, std::function< void(const mouse_status&) > func) {
			size_t id = 0;
			if (!(_ops[opt].size())) {
				_ops[opt] = std::vector< std::function< void(const mouse_status&) > >{ func };
				id = 1;
			} else {
				_ops[opt].push_back(func);
				id = _ops[opt].size();
			}

			return id;
		}

		// events unbinding
		// 事件解绑
		// unbinding some event if id equal 0, or else unbinding event's functional
		// 当id为0是解绑事件，否则解绑绑定事件的某一方法
		size_t off(z_size_t opt, size_t id = 0) {
			size_t count = _ops[opt].size();

			if (count) {
				if (id) {
					_ops[opt].erase(_ops[opt].begin() + (id - 1U));
					if (count != _ops[opt].size()) {
						count = 1;
					}
				} else {
					_ops[opt].clear();
					// count = size of _ops[opt]
				}
			}

			return count;
		}

		// events monitoring
		// 事件监视
		void monitoring(std::function< mouse_status(mouse_status &) > monitor) {
			_msg = monitor(_msg);

			if (_ops[_msg.flags].size())
				for (auto op : _ops[_msg.flags])
					op(_msg);
		}
	};

_ZGL_END

#endif // !ZGL_MOUSE