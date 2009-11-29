#pragma once

#include <rgde/input/base.h>

namespace input
{
	class input_impl;

	// основной класс системы ввода
	class system: boost::noncopyable
	{
	public:
		static system& get  ();
		static void destroy ();

		void init (input_impl*);

        // функции самой системы ввода
        static bool set_mode (bool exclusive=false, bool foreground=true); //изменить режим работы устройств ввода
        static void load_from_string (const std::string &xml_str);      //загрузить раскладку
        static void load_from_file   (const std::string &file_name); //загрузить раскладку
        static void update  ();                        //считать из буфера все событи€ от устройств ввода
        static void save    (std::string &xml_str);       //сохранить раскладку

        // доступ к устройствам ввода
        //получить устройство
        static device* get_device (types::EDevice device_type, int indx=0);
        static device* get_device (const std::wstring &device_name, int indx=0);
        //есть ли такое устройство
        static bool is_device_present (types::EDevice device_type, int indx=0);
        static bool is_device_present (const std::wstring &device_name, int indx=0);

		static control* get_control(types::EDevice device, int dev_index, types::EControl control);
		static control* get_control(types::EDevice device, types::EControl control)
		{
			return get_control(device, 0, control);
		}

        // доступ к командам системы ввода
        static void			add_command       (const std::wstring &command_name); //добавить команду
        static command_ptr  get_command       (const std::wstring &command_name); //получить команду
        static bool			is_command_present (const std::wstring &command_name); //есть ли така€ команда
        static void			detach_command    (command_ptr command);                //отв€зать команду ото всех контролов

	private:
		system();
		~system();

		static system            *ms_instance; //указатель на едиственный экземпл€р System
		std::auto_ptr<input_impl> m_impl;      //конкретна€ реализаци€ системы ввода
	};
}