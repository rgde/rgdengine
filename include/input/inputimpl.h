//inputimpl.h
#pragma once
#include "base.h"

#define DIRECTINPUT_VERSION 0x0800
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dinput.h>

namespace input
{
    //система ввода
    class InputImpl
    {
    public:
        InputImpl();
       ~InputImpl();

        /////////////////////////////////////////////////
        // основные методы для работы с системой ввода //
        /////////////////////////////////////////////////
        bool SetMode (bool exclusive=false, bool foreground=true);            //изменить режим работы устройств ввода
        bool Init    (HWND hWnd, bool exclusive=false, bool foreground=true); //проинициализировать систему ввода

        void Load    (const std::string &sXml);                               //загрузить раскладку
		void Save    (std::string &sXml);                                     //сохранить раскладку

        void Update  ();                                                      //считать из буфера все события от устройств ввода        
        void Done    ();                                                      //завершить работу системы ввода

        ////////////////////////////////
        // доступ к устройствам ввода //
        ////////////////////////////////
        //получить устройство
        Device* getDevice (types::EDevice      eDeviceName, int indx=0);
        Device* getDevice (const std::wstring &sDeviceName, int indx=0);
        //есть ли такое устройство
        bool isDevicePresent (types::EDevice      eDeviceName, int indx=0) const;
        bool isDevicePresent (const std::wstring &sDeviceName, int indx=0) const;

        /////////////////////////////////////
        // доступ к командам системы ввода //
        /////////////////////////////////////
        void      addCommand       (const std::wstring &sCommandName);       //добавить команду
        CommandPtr  getCommand       (const std::wstring &sCommandName);       //получить команду
        bool      isCommandPresent (const std::wstring &sCommandName) const; //есть ли такая команда
        void      detachCommand    (CommandPtr pCommand);                      //отвязать команду ото всех контролов

    private:
        std::list<Device*> m_devices;
        std::list<CommandPtr> m_commands;
        Device*            keyboard; //вспомогательные переменные
        Device*            mouse;    //для более быстрого доступа

        bool m_bInit;

    private:
        InputImpl (const InputImpl&);
        InputImpl& operator= (const InputImpl&);

    private:
        HWND m_hWnd;
        bool m_exclusive;
        bool m_foreground;

        bool initDXInput(HWND hWnd, bool exclusive, bool foreground);
        void doneDXInput();

		LPDIRECTINPUT8       m_pDI;       //устройство DInput
		LPDIRECTINPUTDEVICE8 m_pKeyboard; //устройство ввода "клавиатура"
		LPDIRECTINPUTDEVICE8 m_pMouse;    //устройство ввода "мышь"

		void kProcess (DIDEVICEOBJECTDATA dod); //обработка клавиатурных событий
		void mProcess (DIDEVICEOBJECTDATA dod); //обработка мышиных событий
    };
}