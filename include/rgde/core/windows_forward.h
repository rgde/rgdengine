#pragma once

#include <rgde/core/math/forward.h>

namespace rgde
{
	namespace core
	{
		namespace windows
		{
			struct handle {
				union {
					void* vp;
					int ip;
				};

				operator bool() const {return vp ? true : false;}
			};

			struct input_event {
				enum event_type{
					mouse_move,
					mouse_button, 
					mouse_scrool,
					key_pressed, 
					text_input
				};
				enum action_type {
					up, down, move
				};
				// to handle several input devices case or multi touch events
				int device_index; 
				int keycode;
				int value;
				event_type event;
				action_type action;
			};

			class window;
			typedef boost::shared_ptr<window> window_ptr;
		}
	}
}
