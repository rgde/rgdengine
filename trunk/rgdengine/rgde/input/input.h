#pragma once

#include <rgde/input/base.h>

namespace input
{
	class input_impl;

	// основной класс системы ввода
	class Input: boost::noncopyable
	{
	public:
		static Input& get  ();
		static void destroy ();

		void init (input_impl*);

        // функции самой системы ввода
        static bool set_mode (bool exclusive=false, bool foreground=true); //изменить режим работы устройств ввода
        static void LoadFromString (const std::string &sXml);      //загрузить раскладку
        static void LoadFromFile   (const std::string &sFileName); //загрузить раскладку
        static void update  ();                        //считать из буфера все событи€ от устройств ввода
        static void Save    (std::string &sXml);       //сохранить раскладку

        // доступ к устройствам ввода
        //получить устройство
        static device_dx9* get_device (types::device eDeviceName, int indx=0);
        static device_dx9* get_device (const std::wstring &sDeviceName, int indx=0);
        //есть ли такое устройство
        static bool is_present (types::device eDeviceName, int indx=0);
        static bool is_present (const std::wstring &sDeviceName, int indx=0);

		static Control* GetControl(types::device device, int dev_index, types::control control);
		static Control* GetControl(types::device device, types::control control)
		{
			return GetControl(device, 0, control);
		}

        // доступ к командам системы ввода
        static void      add_command       (const std::wstring &command_name); //добавить команду
        static command_ptr  getCommand       (const std::wstring &command_name); //получить команду
        static bool      isCommandPresent (const std::wstring &command_name); //есть ли така€ команда
        static void      detach_command    (command_ptr pCommand);                //отв€зать команду ото всех контролов

	private:
		Input();
		~Input();

		static Input            *ms_instance; //указатель на едиственный экземпл€р System
		std::auto_ptr<input_impl> m_pImpl;      //конкретна€ реализаци€ системы ввода
	};
}
