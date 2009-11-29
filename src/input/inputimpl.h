#pragma once
#include <rgde/input/base.h>

#define DIRECTINPUT_VERSION 0x0800
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dinput.h>

namespace input
{
    class input_impl
    {
    public:
        input_impl();
       ~input_impl();

	    //изменить режим работы устройств ввода
        bool set_mode (bool exclusive=false, bool foreground=true);

		//проинициализировать систему ввода
        bool Init    (HWND hWnd, bool exclusive=false, bool foreground=true);

		//загрузить/сохранить раскладку
        void load(const std::string &sXml);                               
		void save(std::string &sXml);

		//считать из буфера все события от устройств ввода
        void update();
		//завершить работу системы ввода
        void done();

        //получить устройство
        device* get_device(types::EDevice      eDeviceName, int indx=0);
        device* get_device(const std::wstring &device_name, int indx=0);

        //есть ли такое устройство
        bool is_device_present(types::EDevice      eDeviceName, int indx=0) const;
        bool is_device_present(const std::wstring &device_name, int indx=0) const;


        // доступ к командам системы ввода
		//добавить команду
        command_ptr add_command(const std::wstring &command_name);
		//получить команду
        command_ptr get_command(const std::wstring &command_name);
		//есть ли такая команда
        bool isCommandPresent(const std::wstring &command_name) const;
		//отвязать команду ото всех контролов
        void detach_command(command_ptr pCommand);

	private:
		input_impl (const input_impl&);
		input_impl& operator= (const input_impl&);

		bool initDXInput(HWND hWnd, bool exclusive, bool foreground);
		void doneDXInput();

		//обработка клавиатурных событий
		void kProcess (DIDEVICEOBJECTDATA data);
		//обработка мышиных событий
		void mProcess (DIDEVICEOBJECTDATA data);

    private:
        std::list<device*> m_devices;
        std::list<command_ptr> m_commands;
		
		//вспомогательные переменные для более быстрого доступа
        device*            keyboard; 
        device*            mouse;    

        bool m_bInit;

        HWND m_hWnd;
        bool m_exclusive;
        bool m_foreground;

		LPDIRECTINPUT8       m_pDI;       //устройство DInput
		LPDIRECTINPUTDEVICE8 m_pKeyboard; //устройство ввода "клавиатура"
		LPDIRECTINPUTDEVICE8 m_pMouse;    //устройство ввода "мышь"
    };
}