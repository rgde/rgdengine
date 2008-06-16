#pragma once
#include "base.h"

#define DIRECTINPUT_VERSION 0x0800
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dinput.h>

namespace input
{
    class InputImpl
    {
    public:
        InputImpl();
       ~InputImpl();

	    //изменить режим работы устройств ввода
        bool SetMode (bool exclusive=false, bool foreground=true);

		//проинициализировать систему ввода
        bool Init    (HWND hWnd, bool exclusive=false, bool foreground=true);

		//загрузить/сохранить раскладку
        void Load(const std::string &sXml);                               
		void Save(std::string &sXml);

		//считать из буфера все события от устройств ввода
        void Update();
		//завершить работу системы ввода
        void Done();

        //получить устройство
        Device* getDevice(types::EDevice      eDeviceName, int indx=0);
        Device* getDevice(const std::wstring &sDeviceName, int indx=0);

        //есть ли такое устройство
        bool isDevicePresent(types::EDevice      eDeviceName, int indx=0) const;
        bool isDevicePresent(const std::wstring &sDeviceName, int indx=0) const;


        // доступ к командам системы ввода
		//добавить команду
        CommandPtr addCommand(const std::wstring &sCommandName);
		//получить команду
        CommandPtr getCommand(const std::wstring &sCommandName);
		//есть ли такая команда
        bool isCommandPresent(const std::wstring &sCommandName) const;
		//отвязать команду ото всех контролов
        void detachCommand(CommandPtr pCommand);

	private:
		InputImpl (const InputImpl&);
		InputImpl& operator= (const InputImpl&);

		bool initDXInput(HWND hWnd, bool exclusive, bool foreground);
		void doneDXInput();

		//обработка клавиатурных событий
		void kProcess (DIDEVICEOBJECTDATA data);
		//обработка мышиных событий
		void mProcess (DIDEVICEOBJECTDATA data);

    private:
        std::list<Device*> m_devices;
        std::list<CommandPtr> m_commands;
		
		//вспомогательные переменные для более быстрого доступа
        Device*            keyboard; 
        Device*            mouse;    

        bool m_bInit;

        HWND m_hWnd;
        bool m_exclusive;
        bool m_foreground;

		LPDIRECTINPUT8       m_pDI;       //устройство DInput
		LPDIRECTINPUTDEVICE8 m_pKeyboard; //устройство ввода "клавиатура"
		LPDIRECTINPUTDEVICE8 m_pMouse;    //устройство ввода "мышь"
    };
}