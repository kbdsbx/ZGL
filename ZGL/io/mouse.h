#include "../inc.h"
#include <map>
#include <functional>
#include <vector>

#ifndef ZGL_MOUSE
#define ZGL_MOUSE

_ZGL_BEGIN

	class mouse {
		typedef unsigned int _Tflag;
		typedef int _Tpx;
	public :
		struct mouse_status {
			_Tpx x;
			_Tpx y;
			_Tpx wheel;
			_Tflag flags;

			void set_left() {
				flags |= 0x01;
			}

			void reset_left() {
				flags &= ~0x01;
			}

			void set_right() {
				flags |= 0x02;
			}

			void reset_right() {
				flags &= ~0x02;
			}

			void set_mid() {
				flags |= 0x04;
			}

			void reset_right() {
				flags &= ~0x04;
			}

			void set_move() {
				flags |= 0x08;
			}

			void reset_move() {
				flags &= ~0x08;
			}

			void set_wheel() {
				flags |= 0x10;
			}

			void reset_wheel() {
				flags &= ~0x10;
			}
		};
		struct mouse_msg {
			_Tpx			x;
			_Tpx			y;
			_Tpx			wheel;
			enum mouse_msg_type {
				mouse_msg_down = 0x10,
				mouse_msg_up = 0x20,
				mouse_msg_move = 0x40,
				mouse_msg_wheel = 0x80,
			} msg;
			_Tflag			flags;
		};
	private :
		std::map< z_size_t, std::vector< std::function< void(const mouse_msg&) > > > _ops;

		mouse_msg _msg;
	public :

		int on(z_size_t opt, std::function< void(const mouse_msg&) > func) {
			if (!(_ops[opt].size())) {
				_ops[opt] = std::vector< std::function< void(const mouse_msg&) > >{ func };
			} else {
				_ops[opt].push_back(func);
			}

			return 1;
		}

		void monitoring(std::function< mouse_msg(void) > monitor) {
			_msg = monitor();
			_Tflag _flag = _msg.flags & (_Tflag)_msg.msg;

			if (!(_ops[_msg.flags].size()))
				for (auto op : _ops[_msg.flags])
					op(_msg);
		}
	};

_ZGL_END

#endif // !ZGL_MOUSE