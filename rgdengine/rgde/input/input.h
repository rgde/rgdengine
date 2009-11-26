#pragma once

#include <rgde/input/base.h>

namespace input
{
	class input_impl;

	class Input: boost::noncopyable
	{
	public:
		static Input& get  ();
		static void destroy ();

		void init (input_impl*);

        static bool set_mode (bool exclusive=false, bool foreground=true);
        static void load_xml (const std::string &xml);
        static void load (const std::string &file_name);
        static void update();
        static void save_xml(std::string &xml);

        static device* get_device (devices device_type, int indx=0);
        static device* get_device (const std::wstring &sDeviceName, int indx=0);

        static bool is_present (devices device_type, int indx=0);
        static bool is_present (const std::wstring &sDeviceName, int indx=0);

		static device* get_mouse (int indx = 0)
		{
			return get_device(Mouse, indx);
		}

		static device* get_keyboard (int indx = 0)
		{
			return get_device(Keyboard, indx);
		}

		static device* get_joystick (int indx = 0)
		{
			return get_device(Joystick, indx);
		}

		static Control* get_control(devices device_type, int dev_index, controls control);
		static Control* get_control(devices device_type, controls control)
		{
			return get_control(device_type, 0, control);
		}

        static void			add_command       (const std::wstring &command_name);
        static command_ptr  get_command       (const std::wstring &command_name);
        static bool			is_command_present(const std::wstring &command_name);
        static void			detach_command    (command_ptr command);

	private:
		Input();
		~Input();

		static Input            *ms_instance;
		std::auto_ptr<input_impl> m_impl;
	};
}