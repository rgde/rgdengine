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

        static device* get_device (types::device eDeviceName, int indx=0);
        static device* get_device (const std::wstring &sDeviceName, int indx=0);

        static bool is_present (types::device eDeviceName, int indx=0);
        static bool is_present (const std::wstring &sDeviceName, int indx=0);

		static Control* get_control(types::device device, int dev_index, types::control control);
		static Control* get_control(types::device device, types::control control)
		{
			return get_control(device, 0, control);
		}

        static void			add_command       (const std::wstring &command_name);
        static command_ptr  get_command       (const std::wstring &command_name);
        static bool			is_command_present(const std::wstring &command_name);
        static void			detach_command    (command_ptr pCommand);

	private:
		Input();
		~Input();

		static Input            *ms_instance;
		std::auto_ptr<input_impl> m_impl;
	};
}