//input.h
#pragma once
#include <boost/noncopyable.hpp>
#include "input/inputimpl.h"

//синглтон системы ввода
namespace input
{
	//////////////////////////////////
	// основной класс системы ввода //
	//////////////////////////////////
	class CInput: boost::noncopyable
	{
	public:
		static CInput& Get  ();  //получить ссылку на синглтон системы ввода
		static void Destroy ();	 //удалить синглтон системы ввода

		void init (CInputImpl*); //инициализировать систему ввода

        /////////////////////////////////
        // функции самой системы ввода //
        /////////////////////////////////
        static bool SetMode (bool exclusive=false, bool foreground=true); //изменить режим работы устройств ввода
        static void LoadFromString (const std::string &sXml);      //загрузить раскладку
        static void LoadFromFile   (const std::string &sFileName); //загрузить раскладку
        static void Update  ();                        //считать из буфера все событи€ от устройств ввода
        static void Save    (std::string &sXml);       //сохранить раскладку

        ////////////////////////////////
        // доступ к устройствам ввода //
        ////////////////////////////////
        //получить устройство
        static CDevice* getDevice (EDevice             eDeviceName, int indx=0);
        static CDevice* getDevice (const std::wstring &sDeviceName, int indx=0);
        //есть ли такое устройство
        static bool isDevicePresent (EDevice             eDeviceName, int indx=0);
        static bool isDevicePresent (const std::wstring &sDeviceName, int indx=0);

        /////////////////////////////////////
        // доступ к командам системы ввода //
        /////////////////////////////////////
        static void      addCommand       (const std::wstring &sCommandName); //добавить команду
        static PCommand  getCommand       (const std::wstring &sCommandName); //получить команду
        static bool      isCommandPresent (const std::wstring &sCommandName); //есть ли така€ команда
        static void      detachCommand    (PCommand pCommand);                //отв€зать команду ото всех контролов

	private:
		CInput ();
		~CInput ();

		static CInput            *ms_pInstance; //указатель на едиственный экземпл€р System
		std::auto_ptr<CInputImpl> m_pImpl;      //конкретна€ реализаци€ системы ввода
	};
}
