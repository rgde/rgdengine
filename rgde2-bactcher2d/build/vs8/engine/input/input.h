#pragma once

#include "base.h"

namespace input
{
	class input_impl;

	// основной класс системы ввода
	class Input: boost::noncopyable
	{
	public:
		static Input& get  ();
		static void Destroy ();

		void init (input_impl*);

        // функции самой системы ввода
        static bool SetMode (bool exclusive=false, bool foreground=true); //изменить режим работы устройств ввода
        static void LoadFromString (const std::string &sXml);      //загрузить раскладку
        static void LoadFromFile   (const std::string &sFileName); //загрузить раскладку
        static void update  ();                        //считать из буфера все событи€ от устройств ввода
        static void Save    (std::string &sXml);       //сохранить раскладку

        // доступ к устройствам ввода
        //получить устройство
        static Device* getDevice (types::EDevice eDeviceName, int indx=0);
        static Device* getDevice (const std::wstring &sDeviceName, int indx=0);
        //есть ли такое устройство
        static bool isDevicePresent (types::EDevice eDeviceName, int indx=0);
        static bool isDevicePresent (const std::wstring &sDeviceName, int indx=0);

		static Control* GetControl(types::EDevice device, int dev_index, types::EControl control);
		static Control* GetControl(types::EDevice device, types::EControl control)
		{
			return GetControl(device, 0, control);
		}

        // доступ к командам системы ввода
        static void      addCommand       (const std::wstring &sCommandName); //добавить команду
        static CommandPtr  getCommand       (const std::wstring &sCommandName); //получить команду
        static bool      isCommandPresent (const std::wstring &sCommandName); //есть ли така€ команда
        static void      detachCommand    (CommandPtr pCommand);                //отв€зать команду ото всех контролов

	private:
		Input();
		~Input();

		static Input            *ms_instance; //указатель на едиственный экземпл€р System
		std::auto_ptr<input_impl> m_pImpl;      //конкретна€ реализаци€ системы ввода
	};
}