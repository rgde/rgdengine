#include "stdafx.h"

#include <rgde/input/device.h>
#include <rgde/input/control.h>
#include <rgde/input/command.h>
#include <rgde/input/input.h>

#include <assert.h>

#include "inputimpl.h"

#define KEYBOARD_BUFFER_SIZE 1024
#define    MOUSE_BUFFER_SIZE 1024

//#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

namespace rgde
{
namespace input
{
    input_impl::input_impl (system& sys, core::windows::window_ptr window, int flags):
        m_inited     (false), //считаем, что система ввода не инициализированна
        m_direct_input (NULL),  //устройство DInput
        m_dx_keyboard (NULL),  //устройство ввода "клавиатура"
        m_dx_mouse    (NULL),  //устройство ввода "мышь"
        keyboard    (NULL),
        mouse       (NULL),
		m_system(sys)
    {
		HWND hWnd = (HWND)window->system_handle().vp;

		bool exclusive = flags && system::mode_exlusive;
		bool foreground = flags && system::mode_foreground;
		
		//init DirecX Input system
		m_inited = init_dx_input (hWnd,exclusive,foreground);
		if (!m_inited)
			return;

		// add standart devices
		m_devices.push_back (new device(types::Keyboard, 0, sys));
		m_devices.push_back (new device(types::Mouse,    0, sys));

		//заполним список контролов
		keyboard = *(m_devices.begin());
		mouse    = *(++m_devices.begin());

		//keyboard
		keyboard->add_button(types::KeyEscape);
		keyboard->add_button(types::Key1);
		keyboard->add_button(types::Key2);
		keyboard->add_button(types::Key3);
		keyboard->add_button(types::Key4);
		keyboard->add_button(types::Key5);
		keyboard->add_button(types::Key6);
		keyboard->add_button(types::Key7);
		keyboard->add_button(types::Key8);
		keyboard->add_button(types::Key9);
		keyboard->add_button(types::Key0);
		keyboard->add_button(types::KeyMinus);
		keyboard->add_button(types::KeyEquals);
		keyboard->add_button(types::KeyBack);
		keyboard->add_button(types::KeyTab);
		keyboard->add_button(types::KeyQ);
		keyboard->add_button(types::KeyW);
		keyboard->add_button(types::KeyE);
		keyboard->add_button(types::KeyR);
		keyboard->add_button(types::KeyT);
		keyboard->add_button(types::KeyY);
		keyboard->add_button(types::KeyU);
		keyboard->add_button(types::KeyI);
		keyboard->add_button(types::KeyO);
		keyboard->add_button(types::KeyP);
		keyboard->add_button(types::KeyLBracket);
		keyboard->add_button(types::KeyRBracket);
		keyboard->add_button(types::KeyReturn);
		keyboard->add_button(types::KeyLControl);
		keyboard->add_button(types::KeyA);
		keyboard->add_button(types::KeyS);
		keyboard->add_button(types::KeyD);
		keyboard->add_button(types::KeyF);
		keyboard->add_button(types::KeyG);
		keyboard->add_button(types::KeyH);
		keyboard->add_button(types::KeyJ);
		keyboard->add_button(types::KeyK);
		keyboard->add_button(types::KeyL);
		keyboard->add_button(types::KeySemicolon);
		keyboard->add_button(types::KeyApostrophe);
		keyboard->add_button(types::KeyGrave);
		keyboard->add_button(types::KeyLShift);
		keyboard->add_button(types::KeyBackSlash);
		keyboard->add_button(types::KeyZ);
		keyboard->add_button(types::KeyX);
		keyboard->add_button(types::KeyC);
		keyboard->add_button(types::KeyV);
		keyboard->add_button(types::KeyB);
		keyboard->add_button(types::KeyN);
		keyboard->add_button(types::KeyM);
		keyboard->add_button(types::KeyComma);
		keyboard->add_button(types::KeyPeriod);
		keyboard->add_button(types::KeySlash);
		keyboard->add_button(types::KeyRShift);
		keyboard->add_button(types::KeyMultiply);
		keyboard->add_button(types::KeyLMenu);
		keyboard->add_button(types::KeySpace);
		keyboard->add_button(types::KeyCapital);
		keyboard->add_button(types::KeyF1);
		keyboard->add_button(types::KeyF2);
		keyboard->add_button(types::KeyF3);
		keyboard->add_button(types::KeyF4);
		keyboard->add_button(types::KeyF5);
		keyboard->add_button(types::KeyF6);
		keyboard->add_button(types::KeyF7);
		keyboard->add_button(types::KeyF8);
		keyboard->add_button(types::KeyF9);
		keyboard->add_button(types::KeyF10);
		keyboard->add_button(types::KeyNumLock);
		keyboard->add_button(types::KeyScroll);
		keyboard->add_button(types::KeyNumPad7);
		keyboard->add_button(types::KeyNumPad8);
		keyboard->add_button(types::KeyNumPad9);
		keyboard->add_button(types::KeySubtract);
		keyboard->add_button(types::KeyNumPad4);
		keyboard->add_button(types::KeyNumPad5);
		keyboard->add_button(types::KeyNumPad6);
		keyboard->add_button(types::KeyAdd);
		keyboard->add_button(types::KeyNumPad1);
		keyboard->add_button(types::KeyNumPad2);
		keyboard->add_button(types::KeyNumPad3);
		keyboard->add_button(types::KeyNumPad0);
		keyboard->add_button(types::KeyDecimal);
		keyboard->add_button(types::KeyOEM_102);
		keyboard->add_button(types::KeyF11);
		keyboard->add_button(types::KeyF12);
		keyboard->add_button(types::KeyF13);
		keyboard->add_button(types::KeyF14);
		keyboard->add_button(types::KeyF15);
		keyboard->add_button(types::KeyKana);
		keyboard->add_button(types::KeyABNT_C1);
		keyboard->add_button(types::KeyConvert);
		keyboard->add_button(types::KeyNoConvert);
		keyboard->add_button(types::KeyYen);
		keyboard->add_button(types::KeyABNT_C2);
		keyboard->add_button(types::KeyNumPadEquals);
		keyboard->add_button(types::KeyPrevTrack);
		keyboard->add_button(types::KeyAt);
		keyboard->add_button(types::KeyColon);
		keyboard->add_button(types::KeyUnderLine);
		keyboard->add_button(types::KeyKANJI);
		keyboard->add_button(types::KeyStop);
		keyboard->add_button(types::KeyAx);
		keyboard->add_button(types::KeyUnlabeled);
		keyboard->add_button(types::KeyNextTrack);
		keyboard->add_button(types::KeyNumPadEnter);
		keyboard->add_button(types::KeyRControl);
		keyboard->add_button(types::KeyMute);
		keyboard->add_button(types::KeyCalculator);
		keyboard->add_button(types::KeyPlayPause);
		keyboard->add_button(types::KeyMediaStop);
		keyboard->add_button(types::KeyVolumeDown);
		keyboard->add_button(types::KeyVolumeUp);
		keyboard->add_button(types::KeyWebHome);
		keyboard->add_button(types::KeyNumPadComma);
		keyboard->add_button(types::KeyDivide);
		keyboard->add_button(types::KeySysRQ);
		keyboard->add_button(types::KeyRMenu);
		keyboard->add_button(types::KeyPause);
		keyboard->add_button(types::KeyHome);
		keyboard->add_button(types::KeyUp);
		keyboard->add_button(types::KeyPrior);
		keyboard->add_button(types::KeyLeft);
		keyboard->add_button(types::KeyRight);
		keyboard->add_button(types::KeyEnd);
		keyboard->add_button(types::KeyDown);
		keyboard->add_button(types::KeyNext);
		keyboard->add_button(types::KeyInsert);
		keyboard->add_button(types::KeyDelete);
		keyboard->add_button(types::KeyLWin);
		keyboard->add_button(types::KeyRWin);
		keyboard->add_button(types::KeyApps);

		//mouse
		mouse->add_button (types::ButtonLeft);
		mouse->add_button (types::ButtonMid);
		mouse->add_button (types::ButtonRight);
		mouse->add_axis   (types::AxisX);
		mouse->add_axis   (types::AxisY);
		mouse->add_axis   (types::AxisWheel);
    }

    input_impl::~input_impl ()
    {
        if (m_inited)
            done();
    }
 
    /////////////////////////////////////////////////
    // основные методы для работы с системой ввода //
    /////////////////////////////////////////////////

    //изменить режим работы устройств ввода
    bool input_impl::set_mode (int flags)
    {
        if (!m_inited)
            return false;

		bool exclusive = flags && system::mode_exlusive;
		bool foreground = flags && system::mode_foreground;

        //убить старое подключение к устройствам ввода
        done_dx_input();

        //выполнить подключение к устройствам ввода с новыми настройками
        if (!init_dx_input(m_hwnd,exclusive,foreground))
        {
            //если не удалось, то подключится со старыми настройками
            init_dx_input(m_hwnd,m_exclusive,m_foreground);
            return false;
        }

        return true;
    }

    ////проинициализировать систему ввода
    //bool input_impl::init ()
    //{
    //    if (m_inited)
    //        done();

    //}

    //загрузить раскладку
    void input_impl::load (const std::string &xml_str)
    {
        if (!m_inited)
            return;

		//TiXmlDocument doc;
		//doc.Parse(xml_str.c_str());
		//
		//TiXmlNode *root = doc.FirstChild("input");
		//if (root)
		//{
		//	TiXmlNode *command = root->FirstChild("command");
		//	while (command)
		//	{
		//		TiXmlElement *cmd = command->ToElement();
		//		if (cmd)
		//		{
		//			std::string command_name(cmd->Attribute("name"));
		//			add_command(std::wstring(command_name.begin(), command_name.end()));

		//			TiXmlNode *control = command->FirstChild("control");
		//			while (control)
		//			{
		//				TiXmlElement *ctrl = control->ToElement();
		//				if (ctrl)
		//				{
		//					std::string sDevice(ctrl->Attribute("device"));
		//					std::string sControl(ctrl->Attribute("name"));
		//					device *d = get_device(string_to_device(std::wstring(sDevice.begin(), sDevice.end())));
		//					if (d)
		//					{
		//						control *c = d->get_control(String2Control(std::wstring(sControl.begin(), sControl.end())));
		//						if (c)
		//							c->bind(get_command(std::wstring(command_name.begin(), command_name.end())));
		//					}
		//				}

		//				control = control->NextSibling("control");
		//			}
		//		}

		//		command = command->NextSibling("command");
		//	}
		//}
    }

    //считать из буфера все события от устройств ввода
    void input_impl::update ()
    {
        if (!m_inited)
            return;

        static DIDEVICEOBJECTDATA kbuf [KEYBOARD_BUFFER_SIZE]; //буфер для клавиатурных событий
        static DIDEVICEOBJECTDATA mbuf [   MOUSE_BUFFER_SIZE]; //буфер для событий от мыши
        DWORD                     kElements;                   //кол-во клавиатурных событий
        DWORD                     mElements;                   //кол-во событий от мыши
        HRESULT                   hr;

        //получим сообщения от клавиатуры
        kElements = KEYBOARD_BUFFER_SIZE;
        hr = m_dx_keyboard->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), kbuf, &kElements, 0);
        if (hr != DI_OK) 
        {
            if (hr == DI_BUFFEROVERFLOW)
                ; //... keyboard buffer overflow
            else
                kElements = 0;
            hr = m_dx_keyboard->Acquire();
            while (hr == DIERR_INPUTLOST) 
                hr = m_dx_keyboard->Acquire();
        }

        //получим сообщения от мыши
        mElements = MOUSE_BUFFER_SIZE;
        hr = m_dx_mouse->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), mbuf, &mElements, 0);
        if (hr != DI_OK) 
        {
            if (hr == DI_BUFFEROVERFLOW)
                ; //... mouse buffer overflow
            else
                mElements = 0;
            hr = m_dx_mouse->Acquire();
            while (hr == DIERR_INPUTLOST) 
                hr = m_dx_mouse->Acquire();
        }

        //обработаем сообщения от устройств ввода в порядке их возникновения
        unsigned int k = 0,
            m = 0;
        while (!(k>=kElements && m>=mElements))
            if (k<kElements && m<mElements)
            {
                if (DISEQUENCE_COMPARE(kbuf[k].dwSequence,<=,mbuf[m].dwSequence))
                    process_keyboard(kbuf[k++]);
                else
                    process_mouse(mbuf[m++]);
            }
            else
            {
                if (k<kElements)
                    process_keyboard(kbuf[k++]);
                else
                    process_mouse(mbuf[m++]);
            }
    }

    //сохранить раскладку
    void input_impl::save (std::string &xml_str)
    {
        if (!m_inited)
            return;

    //    TiXmlDocument doc;
    //    TiXmlElement *input = (TiXmlElement*)(doc.InsertEndChild(TiXmlElement("input")));

    //    std::list<command_ptr>::iterator i = m_commands.begin();
    //    while (i != m_commands.end())
    //    {
    //        command_ptr command = *i;

    //        TiXmlElement *command = (TiXmlElement*)(input->InsertEndChild(TiXmlElement("command")));
    //        std::wstring sCommandNameW = command->get_name();
    //        std::string command_name(sCommandNameW.begin(), sCommandNameW.end());
    //        command->SetAttribute("name", command_name.c_str());

    //        std::list<device*>::iterator j = m_devices.begin();
    //        while (j != m_devices.end())
    //        {
    //            device *pDevice = *j;

    //            std::wstring sDeviseNameW = device_to_string(pDevice->get_name());
    //            std::string device_name(sDeviseNameW.begin(), sDeviseNameW.end());

				//std::map<types::EControl, control*>::iterator k = pDevice->m_controls.begin();
    //            while (k != pDevice->m_controls.end())
    //            {
    //                control *pControl = k->second;
    //                if (pControl->is_bind(command))
    //                {
    //                    std::wstring sControlNameW = Control2String(pControl->get_name());
    //                    std::string control_name(sControlNameW.begin(), sControlNameW.end());

    //                    TiXmlElement *control = (TiXmlElement*)(command->InsertEndChild(TiXmlElement("control")));
    //                    control->SetAttribute("device", device_name.c_str());
    //                    control->SetAttribute("name", control_name.c_str());
    //                }
    //                ++k;
    //            }
    //            ++j;
    //        }
    //        ++i;
    //    }

    //    std::stringstream ss;
    //    ss << doc;
    //    xml_str = ss.str();
    }

    //завершить работу системы ввода
    void input_impl::done ()
    {
        if (!m_inited)
            return;

        done_dx_input ();
        m_commands.clear();

        while (!m_devices.empty())
        {
            delete *(m_devices.begin());
            m_devices.erase(m_devices.begin());
        }

        keyboard = NULL;
        mouse    = NULL;

        m_inited = false;
    }
 
    ////////////////////////////////
    // доступ к устройствам ввода //
    ////////////////////////////////

    //получить устройство
    device* input_impl::get_device (types::EDevice device_type, int indx/*=0*/)
    {
        if (!m_inited)
            return 0;

        std::list<device*>::iterator i = m_devices.begin();
        while (i != m_devices.end())
        {
            if ((*i)->get_name() == device_type && (*i)->get_index() == indx)
                return *i;
            ++i;
        }

        return 0;
    }

    device* input_impl::get_device (const std::wstring &device_name, int indx/*=0*/)
    {
		return get_device(string_to_type::device(device_name), indx);
    }

    //есть ли такое устройство
    bool input_impl::is_device_present (types::EDevice device_type, int indx/*=0*/) const
    {
        if (!m_inited)
            return false;

        std::list<device*>::const_iterator i = m_devices.begin();
        while (i != m_devices.end())
        {
            if ((*i)->get_name() == device_type && (*i)->get_index() == indx)
                return true;
            ++i;
        }

        return false;
    }

    bool input_impl::is_device_present (const std::wstring &device_name, int indx/*=0*/) const
    {
		return is_device_present(string_to_type::device(device_name), indx);
    }
 
    /////////////////////////////////////
    // доступ к командам системы ввода //
    /////////////////////////////////////

    //добавить команду
    command_ptr input_impl::add_command (const std::wstring &command_name)
    {
        if (m_inited && !is_command_present(command_name))
        {
            command_ptr command(new command(command_name, m_system));
            m_commands.push_back(command);
			return command;
        }

		return command_ptr();
    }

    //получить команду
    command_ptr input_impl::get_command (const std::wstring &command_name)
    {
        if (!m_inited)
            return command_ptr();

        std::list<command_ptr>::iterator i = m_commands.begin();
        while (i != m_commands.end())
        {
            if ((*i)->name() == command_name)
                return (*i);
            ++i;
        }

        return command_ptr();
    }

    //есть ли такая команда
    bool input_impl::is_command_present (const std::wstring &command_name) const
    {
        if (!m_inited)
            return false;

        std::list<command_ptr>::const_iterator i = m_commands.begin();
        while (i != m_commands.end())
        {
            if ((*i)->name() == command_name)
                return true;
            ++i;
        }

        return false;
    }

    //отвязать команду ото всех контролов
    void input_impl::detach_command (command_ptr command)
    {
        if (!m_inited)
            return;

        std::list<device*>::iterator i = m_devices.begin();

        while (i != m_devices.end())
        {
            (*i)->detach_command(command);
            ++i;
        }
    }

    //инициализация DXInput
    bool input_impl::init_dx_input (HWND hWnd, bool exclusive, bool foreground)
    {
        //получаем окно верхнего уровня
        {
            HWND cur = hWnd;
            for(;;)
            {
                HWND temp_hwnd = GetParent(cur);
                if (temp_hwnd == NULL) 
                {
                    hWnd = cur;
                    break;
                }
                cur = temp_hwnd;
            }
        }

        DWORD test = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;

        DWORD flags = 0;
        flags |= exclusive  ? DISCL_EXCLUSIVE  : DISCL_NONEXCLUSIVE;
        flags |= foreground ? DISCL_FOREGROUND : DISCL_BACKGROUND;

        HRESULT     hr;     //результат успеха/ошибки
        DIPROPDWORD dipdw;  //для задания буферизованного ввода клавиатуры и мыши

        m_direct_input       = NULL; //устройство DInput
        m_dx_keyboard = NULL; //устройство ввода "клавиатура"
        m_dx_mouse    = NULL; //устройство ввода "мышь"

        ///////////////////////////
        // создаем обьект DInput //
        ///////////////////////////
        if
        (
            FAILED
            (
                hr = DirectInput8Create
                (
                    GetModuleHandle(NULL),
                    DIRECTINPUT_VERSION,
                    IID_IDirectInput8,
                    (VOID**)&m_direct_input,
                    NULL
                )
            )
        )
        {
            done_dx_input(); //assert(0);
            return false; //... can't create DInput object
        }

        ///////////////////////////////////////////
        // создаем устройство ввода "клавиатура" //
        ///////////////////////////////////////////
        if (FAILED (hr = m_direct_input->CreateDevice (GUID_SysKeyboard, &m_dx_keyboard, NULL)))
        {
            done_dx_input(); //assert(0);
            return false; //... can't create keyboard device
        }

        //устанавливаем формат данных для устройства ввода "клавиатура"
        if (FAILED (hr = m_dx_keyboard->SetDataFormat (&c_dfDIKeyboard)))
        {
            done_dx_input(); //assert(0);
            return false; //... can't set data format for keyboard device
        }

        //устанавливаем способ доступа к клавиатуре
        if (FAILED (hr = m_dx_keyboard->SetCooperativeLevel (hWnd, flags)))
        {
            done_dx_input(); //assert(0);
            return false; //... can't set cooperative level for keyboard device
        }

        //задаем буферизованный доступ к клавиатуре
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = KEYBOARD_BUFFER_SIZE;

        if (FAILED (hr = m_dx_keyboard->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
        {
            done_dx_input(); //assert(0);
            return false; //... can't set buffered device data for keyboard device
        }

        //захватываем клавиатуру
        m_dx_keyboard->Acquire();

        /////////////////////////////////////
        // создаем устройство ввода "мышь" //
        /////////////////////////////////////
        if (FAILED (hr = m_direct_input->CreateDevice (GUID_SysMouse, &m_dx_mouse, NULL)))
        {
            done_dx_input(); //assert(0);
            return false; //... can't create mouse device
        }

        //устанавливаем формат данных для устройства ввода "мышь"
        if (FAILED (hr = m_dx_mouse->SetDataFormat (&c_dfDIMouse2)))
        {
            done_dx_input(); //assert(0);
            return false; //... can't set data format for mouse device
        }

        //устанавливаем способ доступа к мыши
        if (FAILED (hr = m_dx_mouse->SetCooperativeLevel (hWnd,flags)))
        {
            done_dx_input(); //assert(0);
            return false; //... can't set cooperative level for mouse device
        }

        //задаем буферизованный доступ к мыши
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = MOUSE_BUFFER_SIZE;

        if (FAILED (hr = m_dx_mouse->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
        {
            done_dx_input(); //assert(0);
            return false; //... can't set buffered device data for mouse device
        }

        //захватываем мышь
        m_dx_mouse->Acquire();

        //запомним параметры инициализации
        m_hwnd = hWnd;
        m_exclusive = exclusive;
        m_foreground = foreground;

        return true;
    }

    //ДЕинициализация DXInput
    void input_impl::done_dx_input()
    {
        if (m_dx_mouse)    m_dx_mouse   ->Unacquire();
        if (m_dx_keyboard) m_dx_keyboard->Unacquire();

        if (m_dx_mouse)    m_dx_mouse   ->Release();
        if (m_dx_keyboard) m_dx_keyboard->Release();
        if (m_direct_input)       m_direct_input      ->Release();

        m_dx_mouse    = NULL; //устройство ввода "мышь"
        m_dx_keyboard = NULL; //устройство ввода "клавиатура"
        m_direct_input       = NULL; //устройство DInput
    }

    //обработка клавиатурных событий
    void input_impl::process_keyboard (DIDEVICEOBJECTDATA dod)
    {
		//определим, от какого контрола пришло сообщение
		types::EControl eControl = types::InvalidControl;
		switch (dod.dwOfs)
		{
		    case DIK_ESCAPE       : eControl = types::KeyEscape	; break;
		    case DIK_1            : eControl = types::Key1		; break;
		    case DIK_2            : eControl = types::Key2		; break;
		    case DIK_3            : eControl = types::Key3		; break;
		    case DIK_4            : eControl = types::Key4		; break;
		    case DIK_5            : eControl = types::Key5		; break;
		    case DIK_6            : eControl = types::Key6            ; break;
		    case DIK_7            : eControl = types::Key7            ; break;
		    case DIK_8            : eControl = types::Key8            ; break;
		    case DIK_9            : eControl = types::Key9            ; break;
		    case DIK_0            : eControl = types::Key0            ; break;
		    case DIK_MINUS        : eControl = types::KeyMinus        ; break;
		    case DIK_EQUALS       : eControl = types::KeyEquals       ; break;
		    case DIK_BACK         : eControl = types::KeyBack         ; break;
		    case DIK_TAB          : eControl = types::KeyTab          ; break;
		    case DIK_Q            : eControl = types::KeyQ            ; break;
		    case DIK_W            : eControl = types::KeyW            ; break;
		    case DIK_E            : eControl = types::KeyE            ; break;
		    case DIK_R            : eControl = types::KeyR            ; break;
		    case DIK_T            : eControl = types::KeyT            ; break;
		    case DIK_Y            : eControl = types::KeyY            ; break;
		    case DIK_U            : eControl = types::KeyU            ; break;
		    case DIK_I            : eControl = types::KeyI            ; break;
		    case DIK_O            : eControl = types::KeyO            ; break;
		    case DIK_P            : eControl = types::KeyP            ; break;
		    case DIK_LBRACKET     : eControl = types::KeyLBracket     ; break;
		    case DIK_RBRACKET     : eControl = types::KeyRBracket     ; break;
		    case DIK_RETURN       : eControl = types::KeyReturn       ; break;
		    case DIK_LCONTROL     : eControl = types::KeyLControl     ; break;
		    case DIK_A            : eControl = types::KeyA            ; break;
		    case DIK_S            : eControl = types::KeyS            ; break;
		    case DIK_D            : eControl = types::KeyD            ; break;
		    case DIK_F            : eControl = types::KeyF            ; break;
		    case DIK_G            : eControl = types::KeyG            ; break;
		    case DIK_H            : eControl = types::KeyH            ; break;
		    case DIK_J            : eControl = types::KeyJ            ; break;
		    case DIK_K            : eControl = types::KeyK            ; break;
		    case DIK_L            : eControl = types::KeyL            ; break;
		    case DIK_SEMICOLON    : eControl = types::KeySemicolon    ; break;
		    case DIK_APOSTROPHE   : eControl = types::KeyApostrophe   ; break;
		    case DIK_GRAVE        : eControl = types::KeyGrave        ; break;
		    case DIK_LSHIFT       : eControl = types::KeyLShift       ; break;
		    case DIK_BACKSLASH    : eControl = types::KeyBackSlash    ; break;
		    case DIK_Z            : eControl = types::KeyZ            ; break;
		    case DIK_X            : eControl = types::KeyX            ; break;
		    case DIK_C            : eControl = types::KeyC            ; break;
		    case DIK_V            : eControl = types::KeyV            ; break;
		    case DIK_B            : eControl = types::KeyB            ; break;
		    case DIK_N            : eControl = types::KeyN            ; break;
		    case DIK_M            : eControl = types::KeyM            ; break;
		    case DIK_COMMA        : eControl = types::KeyComma        ; break;
		    case DIK_PERIOD       : eControl = types::KeyPeriod       ; break;
		    case DIK_SLASH        : eControl = types::KeySlash        ; break;
		    case DIK_RSHIFT       : eControl = types::KeyRShift       ; break;
		    case DIK_MULTIPLY     : eControl = types::KeyMultiply     ; break;
		    case DIK_LMENU        : eControl = types::KeyLMenu        ; break;
		    case DIK_SPACE        : eControl = types::KeySpace        ; break;
		    case DIK_CAPITAL      : eControl = types::KeyCapital      ; break;
		    case DIK_F1           : eControl = types::KeyF1           ; break;
		    case DIK_F2           : eControl = types::KeyF2           ; break;
		    case DIK_F3           : eControl = types::KeyF3           ; break;
		    case DIK_F4           : eControl = types::KeyF4           ; break;
		    case DIK_F5           : eControl = types::KeyF5           ; break;
		    case DIK_F6           : eControl = types::KeyF6           ; break;
		    case DIK_F7           : eControl = types::KeyF7           ; break;
		    case DIK_F8           : eControl = types::KeyF8           ; break;
		    case DIK_F9           : eControl = types::KeyF9           ; break;
		    case DIK_F10          : eControl = types::KeyF10          ; break;
		    case DIK_NUMLOCK      : eControl = types::KeyNumLock      ; break;
		    case DIK_SCROLL       : eControl = types::KeyScroll       ; break;
		    case DIK_NUMPAD7      : eControl = types::KeyNumPad7      ; break;
		    case DIK_NUMPAD8      : eControl = types::KeyNumPad8      ; break;
		    case DIK_NUMPAD9      : eControl = types::KeyNumPad9      ; break;
		    case DIK_SUBTRACT     : eControl = types::KeySubtract     ; break;
		    case DIK_NUMPAD4      : eControl = types::KeyNumPad4      ; break;
		    case DIK_NUMPAD5      : eControl = types::KeyNumPad5      ; break;
		    case DIK_NUMPAD6      : eControl = types::KeyNumPad6      ; break;
		    case DIK_ADD          : eControl = types::KeyAdd          ; break;
		    case DIK_NUMPAD1      : eControl = types::KeyNumPad1      ; break;
		    case DIK_NUMPAD2      : eControl = types::KeyNumPad2      ; break;
		    case DIK_NUMPAD3      : eControl = types::KeyNumPad3      ; break;
		    case DIK_NUMPAD0      : eControl = types::KeyNumPad0      ; break;
		    case DIK_DECIMAL      : eControl = types::KeyDecimal      ; break;
		    case DIK_OEM_102      : eControl = types::KeyOEM_102      ; break;
		    case DIK_F11          : eControl = types::KeyF11          ; break;
		    case DIK_F12          : eControl = types::KeyF12          ; break;
		    case DIK_F13          : eControl = types::KeyF13          ; break;
		    case DIK_F14          : eControl = types::KeyF14          ; break;
		    case DIK_F15          : eControl = types::KeyF15          ; break;
		    case DIK_KANA         : eControl = types::KeyKana         ; break;
		    case DIK_ABNT_C1      : eControl = types::KeyABNT_C1      ; break;
		    case DIK_CONVERT      : eControl = types::KeyConvert      ; break;
		    case DIK_NOCONVERT    : eControl = types::KeyNoConvert    ; break;
		    case DIK_YEN          : eControl = types::KeyYen          ; break;
		    case DIK_ABNT_C2      : eControl = types::KeyABNT_C2      ; break;
		    case DIK_NUMPADEQUALS : eControl = types::KeyNumPadEquals ; break;
		    case DIK_PREVTRACK    : eControl = types::KeyPrevTrack    ; break;
		    case DIK_AT           : eControl = types::KeyAt           ; break;
		    case DIK_COLON        : eControl = types::KeyColon        ; break;
		    case DIK_UNDERLINE    : eControl = types::KeyUnderLine    ; break;
		    case DIK_KANJI        : eControl = types::KeyKANJI        ; break;
		    case DIK_STOP         : eControl = types::KeyStop         ; break;
		    case DIK_AX           : eControl = types::KeyAx           ; break;
		    case DIK_UNLABELED    : eControl = types::KeyUnlabeled    ; break;
		    case DIK_NEXTTRACK    : eControl = types::KeyNextTrack    ; break;
		    case DIK_NUMPADENTER  : eControl = types::KeyNumPadEnter  ; break;
		    case DIK_RCONTROL     : eControl = types::KeyRControl     ; break;
		    case DIK_MUTE         : eControl = types::KeyMute         ; break;
		    case DIK_CALCULATOR   : eControl = types::KeyCalculator   ; break;
		    case DIK_PLAYPAUSE    : eControl = types::KeyPlayPause    ; break;
		    case DIK_MEDIASTOP    : eControl = types::KeyMediaStop    ; break;
		    case DIK_VOLUMEDOWN   : eControl = types::KeyVolumeDown   ; break;
		    case DIK_VOLUMEUP     : eControl = types::KeyVolumeUp     ; break;
		    case DIK_WEBHOME      : eControl = types::KeyWebHome      ; break;
		    case DIK_NUMPADCOMMA  : eControl = types::KeyNumPadComma  ; break;
		    case DIK_DIVIDE       : eControl = types::KeyDivide       ; break;
		    case DIK_SYSRQ        : eControl = types::KeySysRQ        ; break;
		    case DIK_RMENU        : eControl = types::KeyRMenu        ; break;
		    case DIK_PAUSE        : eControl = types::KeyPause        ; break;
		    case DIK_HOME         : eControl = types::KeyHome         ; break;
		    case DIK_UP           : eControl = types::KeyUp           ; break;
		    case DIK_PRIOR        : eControl = types::KeyPrior        ; break;
		    case DIK_LEFT         : eControl = types::KeyLeft         ; break;
		    case DIK_RIGHT        : eControl = types::KeyRight        ; break;
		    case DIK_END          : eControl = types::KeyEnd          ; break;
		    case DIK_DOWN         : eControl = types::KeyDown         ; break;
		    case DIK_NEXT         : eControl = types::KeyNext         ; break;
		    case DIK_INSERT       : eControl = types::KeyInsert       ; break;
		    case DIK_DELETE       : eControl = types::KeyDelete       ; break;
		    case DIK_LWIN         : eControl = types::KeyLWin         ; break;
		    case DIK_RWIN         : eControl = types::KeyRWin         ; break;
		    case DIK_APPS         : eControl = types::KeyApps         ; break;
		}

		if (eControl == types::InvalidControl)
			return; //... такой контрол не поддерживается

        control *pControl = keyboard->get_control(eControl);
		if (!pControl)
			return; //... такой контрол для клавиатуры не предусмотрен 

		pControl->m_time  = dod.dwTimeStamp;
		pControl->m_press = (dod.dwData & 0x80) ? true : false;
		pControl->m_delta = 0;

		pControl->notify_all();
    }

    //обработка мышиных событий
    void input_impl::process_mouse (DIDEVICEOBJECTDATA dod)
    {
        //определим, от какого контрола пришло сообщение
        types::EControl eControl = types::InvalidControl;
        switch (dod.dwOfs)
        {
            case DIMOFS_BUTTON0: eControl = types::ButtonLeft  ; break;
            case DIMOFS_BUTTON1: eControl = types::ButtonRight ; break;
            case DIMOFS_BUTTON2: eControl = types::ButtonMid   ; break;
            case DIMOFS_X:       eControl = types::AxisX       ; break;
            case DIMOFS_Y:       eControl = types::AxisY       ; break;
            case DIMOFS_Z:       eControl = types::AxisWheel   ; break;
        }

		if (eControl == types::InvalidControl)
			return; //... такой контрол не поддерживается

        control *pControl = mouse->get_control(eControl);
		if (!pControl)
			return; //... такой контрол для мыши не предусмотрен 

        pControl->m_time = dod.dwTimeStamp;

		switch(pControl->get_type())
		{
		case control::axis:
			pControl->m_press = false;
			pControl->m_delta = dod.dwData;
			break;
		case control::button:
			pControl->m_press = (dod.dwData & 0x80) ? true : false;
			pControl->m_delta = 0;
			break;
		default:
			assert (false && "Invalid Control Type!");
		}

        pControl->notify_all();
    }
}
}