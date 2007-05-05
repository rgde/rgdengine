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
    class CInputImpl
    {
    public:
        CInputImpl();
       ~CInputImpl();

        /////////////////////////////////////////////////
        // основные методы для работы с системой ввода //
        /////////////////////////////////////////////////
        bool SetMode (bool exclusive=false, bool foreground=true);            //изменить режим работы устройств ввода
        bool Init    (HWND hWnd, bool exclusive=false, bool foreground=true); //проинициализировать систему ввода
        void Load    (const std::string &sXml);                               //загрузить раскладку
        void Update  ();                                                      //считать из буфера все события от устройств ввода
        void Save    (std::string &sXml);                                     //сохранить раскладку
        void Done    ();                                                      //завершить работу системы ввода

        ////////////////////////////////
        // доступ к устройствам ввода //
        ////////////////////////////////
        //получить устройство
        CDevice* getDevice (EDevice             eDeviceName, int indx=0);
        CDevice* getDevice (const std::wstring &sDeviceName, int indx=0);
        //есть ли такое устройство
        bool isDevicePresent (EDevice             eDeviceName, int indx=0) const;
        bool isDevicePresent (const std::wstring &sDeviceName, int indx=0) const;

        /////////////////////////////////////
        // доступ к командам системы ввода //
        /////////////////////////////////////
        void      addCommand       (const std::wstring &sCommandName);       //добавить команду
        PCommand  getCommand       (const std::wstring &sCommandName);       //получить команду
        bool      isCommandPresent (const std::wstring &sCommandName) const; //есть ли такая команда
        void      detachCommand    (PCommand pCommand);                      //отвязать команду ото всех контролов

    private:
        std::list<CDevice*> m_devices;
        std::list<PCommand> m_commands;
        CDevice*            keyboard; //вспомогательные переменные
        CDevice*            mouse;    //для более быстрого доступа

        bool m_bInit;

    private:
        CInputImpl (const CInputImpl&);
        CInputImpl& operator= (const CInputImpl&);

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