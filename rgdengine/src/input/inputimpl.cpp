#include "precompiled.h"

#include <rgde/input/input.h>
#include "inputimpl.h"

#define KEYBOARD_BUFFER_SIZE 1024
#define    MOUSE_BUFFER_SIZE 1024

//#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

namespace input
{
    input_impl::input_impl ():
        m_inited     (false), //считаем, что система ввода не инициализированна
        m_dxinput       (NULL),  //устройство DInput
        m_keyboard (NULL),  //устройство ввода "клавиатура"
        m_mouse    (NULL),  //устройство ввода "мышь"
        keyboard    (NULL),
        mouse       (NULL)
    {
    }

    input_impl::~input_impl ()
    {
        if (m_inited)
            Done();
    }
 
    /////////////////////////////////////////////////
    // основные методы для работы с системой ввода //
    /////////////////////////////////////////////////

    //изменить режим работы устройств ввода
    bool input_impl::set_mode (bool exclusive/*=false*/, bool foreground/*=true*/)
    {
        if (!m_inited)
            return false;

        //убить старое подключение к устройствам ввода
        doneDXInput();

        //выполнить подключение к устройствам ввода с новыми настройками
        if (!init_input(m_hwnd,exclusive,foreground))
        {
            //если не удалось, то подключится со старыми настройками
            init_input(m_hwnd,m_exclusive,m_foreground);
            return false;
        }

        return true;
    }

    //проинициализировать систему ввода
    bool input_impl::init (HWND hWnd, bool exclusive/*=false*/, bool foreground/*=true*/)
    {
        if (m_inited)
            Done();

        //инициализируем DXInput
        m_inited = init_input (hWnd,exclusive,foreground);
        if (!m_inited)
            return false;

		keyboard = new device(device::keyboard, 0, *this);
		mouse = new device(device::mouse, 0, *this);

        //заполним массив m_devices
		m_devices[device::keyboard].push_back(keyboard);
		m_devices[device::mouse].push_back(mouse);

        //заполним список контролов
        keyboard = m_devices[device::keyboard].front();
        mouse    = m_devices[device::mouse].front();

        //keyboard
        keyboard->add_button(KeyEscape);
        keyboard->add_button(Key1);
        keyboard->add_button(Key2);
        keyboard->add_button(Key3);
        keyboard->add_button(Key4);
        keyboard->add_button(Key5);
        keyboard->add_button(Key6);
        keyboard->add_button(Key7);
        keyboard->add_button(Key8);
        keyboard->add_button(Key9);
        keyboard->add_button(Key0);
        keyboard->add_button(KeyMinus);
        keyboard->add_button(KeyEquals);
        keyboard->add_button(KeyBack);
        keyboard->add_button(KeyTab);
        keyboard->add_button(KeyQ);
        keyboard->add_button(KeyW);
        keyboard->add_button(KeyE);
        keyboard->add_button(KeyR);
        keyboard->add_button(KeyT);
        keyboard->add_button(KeyY);
        keyboard->add_button(KeyU);
        keyboard->add_button(KeyI);
        keyboard->add_button(KeyO);
        keyboard->add_button(KeyP);
        keyboard->add_button(KeyLBracket);
        keyboard->add_button(KeyRBracket);
        keyboard->add_button(KeyReturn);
        keyboard->add_button(KeyLControl);
        keyboard->add_button(KeyA);
        keyboard->add_button(KeyS);
        keyboard->add_button(KeyD);
        keyboard->add_button(KeyF);
        keyboard->add_button(KeyG);
        keyboard->add_button(KeyH);
        keyboard->add_button(KeyJ);
        keyboard->add_button(KeyK);
        keyboard->add_button(KeyL);
        keyboard->add_button(KeySemicolon);
        keyboard->add_button(KeyApostrophe);
        keyboard->add_button(KeyGrave);
        keyboard->add_button(KeyLShift);
        keyboard->add_button(KeyBackSlash);
        keyboard->add_button(KeyZ);
        keyboard->add_button(KeyX);
        keyboard->add_button(KeyC);
        keyboard->add_button(KeyV);
        keyboard->add_button(KeyB);
        keyboard->add_button(KeyN);
        keyboard->add_button(KeyM);
        keyboard->add_button(KeyComma);
        keyboard->add_button(KeyPeriod);
        keyboard->add_button(KeySlash);
        keyboard->add_button(KeyRShift);
        keyboard->add_button(KeyMultiply);
        keyboard->add_button(KeyLMenu);
        keyboard->add_button(KeySpace);
        keyboard->add_button(KeyCapital);
        keyboard->add_button(KeyF1);
        keyboard->add_button(KeyF2);
        keyboard->add_button(KeyF3);
        keyboard->add_button(KeyF4);
        keyboard->add_button(KeyF5);
        keyboard->add_button(KeyF6);
        keyboard->add_button(KeyF7);
        keyboard->add_button(KeyF8);
        keyboard->add_button(KeyF9);
        keyboard->add_button(KeyF10);
        keyboard->add_button(KeyNumLock);
        keyboard->add_button(KeyScroll);
        keyboard->add_button(KeyNumPad7);
        keyboard->add_button(KeyNumPad8);
        keyboard->add_button(KeyNumPad9);
        keyboard->add_button(KeySubtract);
        keyboard->add_button(KeyNumPad4);
        keyboard->add_button(KeyNumPad5);
        keyboard->add_button(KeyNumPad6);
        keyboard->add_button(KeyAdd);
        keyboard->add_button(KeyNumPad1);
        keyboard->add_button(KeyNumPad2);
        keyboard->add_button(KeyNumPad3);
        keyboard->add_button(KeyNumPad0);
        keyboard->add_button(KeyDecimal);
        keyboard->add_button(KeyOEM_102);
        keyboard->add_button(KeyF11);
        keyboard->add_button(KeyF12);
        keyboard->add_button(KeyF13);
        keyboard->add_button(KeyF14);
        keyboard->add_button(KeyF15);
        keyboard->add_button(KeyKana);
        keyboard->add_button(KeyABNT_C1);
        keyboard->add_button(KeyConvert);
        keyboard->add_button(KeyNoConvert);
        keyboard->add_button(KeyYen);
        keyboard->add_button(KeyABNT_C2);
        keyboard->add_button(KeyNumPadEquals);
        keyboard->add_button(KeyPrevTrack);
        keyboard->add_button(KeyAt);
        keyboard->add_button(KeyColon);
        keyboard->add_button(KeyUnderLine);
        keyboard->add_button(KeyKANJI);
        keyboard->add_button(KeyStop);
        keyboard->add_button(KeyAx);
        keyboard->add_button(KeyUnlabeled);
        keyboard->add_button(KeyNextTrack);
        keyboard->add_button(KeyNumPadEnter);
        keyboard->add_button(KeyRControl);
        keyboard->add_button(KeyMute);
        keyboard->add_button(KeyCalculator);
        keyboard->add_button(KeyPlayPause);
        keyboard->add_button(KeyMediaStop);
        keyboard->add_button(KeyVolumeDown);
        keyboard->add_button(KeyVolumeUp);
        keyboard->add_button(KeyWebHome);
        keyboard->add_button(KeyNumPadComma);
        keyboard->add_button(KeyDivide);
        keyboard->add_button(KeySysRQ);
        keyboard->add_button(KeyRMenu);
        keyboard->add_button(KeyPause);
        keyboard->add_button(KeyHome);
        keyboard->add_button(KeyUp);
        keyboard->add_button(KeyPrior);
        keyboard->add_button(KeyLeft);
        keyboard->add_button(KeyRight);
        keyboard->add_button(KeyEnd);
        keyboard->add_button(KeyDown);
        keyboard->add_button(KeyNext);
        keyboard->add_button(KeyInsert);
        keyboard->add_button(KeyDelete);
        keyboard->add_button(KeyLWin);
        keyboard->add_button(KeyRWin);
        keyboard->add_button(KeyApps);
							
        //mouse
        mouse->add_button (ButtonLeft);
        mouse->add_button (ButtonMid);
        mouse->add_button (ButtonRight);
        mouse->add_axis   (AxisX);
        mouse->add_axis   (AxisY);
        mouse->add_axis   (AxisWheel);

        return true;
    }

    void input_impl::load (const std::string &xml)
    {
        if (!m_inited)
            return;

		TiXmlDocument doc;
		doc.Parse(xml.c_str());
		
		TiXmlNode *root = doc.FirstChild("input");
		if (root)
		{
			TiXmlNode *command = root->FirstChild("command");
			while (command)
			{
				TiXmlElement *cmd = command->ToElement();
				if (cmd)
				{
					std::string command_name(cmd->Attribute("name"));
					add_command(command_name);

					TiXmlNode *control = command->FirstChild("control");
					while (control)
					{
						TiXmlElement *ctrl = control->ToElement();
						if (ctrl)
						{
							std::string sDevice(ctrl->Attribute("device"));
							std::string sControl(ctrl->Attribute("name"));
							device *d = get_device(String2Device(std::wstring(sDevice.begin(), sDevice.end())));
							if (d)
							{
								Control *c = d->get_control(String2Control(std::wstring(sControl.begin(), sControl.end())));
								if (c)
									c->bind(get_command(command_name));
							}
						}

						control = control->NextSibling("control");
					}
				}

				command = command->NextSibling("command");
			}
		}
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
        hr = m_keyboard->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), kbuf, &kElements, 0);
        if (hr != DI_OK) 
        {
            if (hr == DI_BUFFEROVERFLOW)
                ; //... keyboard buffer overflow
            else
                kElements = 0;
            hr = m_keyboard->Acquire();
            while (hr == DIERR_INPUTLOST) 
                hr = m_keyboard->Acquire();
        }

        //получим сообщения от мыши
        mElements = MOUSE_BUFFER_SIZE;
        hr = m_mouse->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), mbuf, &mElements, 0);
        if (hr != DI_OK) 
        {
            if (hr == DI_BUFFEROVERFLOW)
                ; //... mouse buffer overflow
            else
                mElements = 0;
            hr = m_mouse->Acquire();
            while (hr == DIERR_INPUTLOST) 
                hr = m_mouse->Acquire();
        }

        //обработаем сообщения от устройств ввода в порядке их возникновения
        unsigned int k = 0,
            m = 0;
        while (!(k>=kElements && m>=mElements))
            if (k<kElements && m<mElements)
            {
                if (DISEQUENCE_COMPARE(kbuf[k].dwSequence,<=,mbuf[m].dwSequence))
                    kProcess(kbuf[k++]);
                else
                    mProcess(mbuf[m++]);
            }
            else
            {
                if (k<kElements)
                    kProcess(kbuf[k++]);
                else
                    mProcess(mbuf[m++]);
            }
    }

    //сохранить раскладку
    void input_impl::save (std::string &sXml)
    {
        if (!m_inited)
            return;

        TiXmlDocument doc;
        TiXmlElement *input = (TiXmlElement*)(doc.InsertEndChild(TiXmlElement("input")));

        std::list<command_ptr>::iterator i = m_commands.begin();
        while (i != m_commands.end())
        {
            command_ptr pCommand = *i;

            TiXmlElement *command = (TiXmlElement*)(input->InsertEndChild(TiXmlElement("command")));
            std::string command_name = pCommand->name();

            command->SetAttribute("name", command_name.c_str());

			for(size_t device_type = 0; device_type < device::total_number; ++device_type)
            {
				devices_vector& devices = m_devices[device_type];
				devices_vector::iterator j = devices.begin();
				
				while (j != devices.end())
				{
					device *pDevice = *j;

					std::wstring sDeviseNameW = Device2String(pDevice->type());
					std::string sDeviceName(sDeviseNameW.begin(), sDeviseNameW.end());

					std::map<controls, Control*>::iterator k = pDevice->m_controls.begin();
					while (k != pDevice->m_controls.end())
					{
						Control *pControl = k->second;
						if (pControl->is_bind(pCommand))
						{
							std::wstring sControlNameW = Control2String(pControl->name());
							std::string contol_name(sControlNameW.begin(), sControlNameW.end());

							TiXmlElement *control = (TiXmlElement*)(command->InsertEndChild(TiXmlElement("control")));
							control->SetAttribute("device", sDeviceName.c_str());
							control->SetAttribute("name", contol_name.c_str());
						}
						++k;
					}
					++j;
				}
            }
            ++i;
        }

        std::stringstream ss;
        ss << doc;
        sXml = ss.str();
    }

    //завершить работу системы ввода
    void input_impl::Done ()
    {
        if (!m_inited)
            return;

        doneDXInput ();
        m_commands.clear();

		keyboard = NULL;
		mouse    = NULL;

		for(size_t device_type = 0; device_type < device::total_number; ++device_type)
		{
			devices_vector& devices = m_devices[device_type];
			while (!devices.empty())
			{
				delete *(devices.begin());
				devices.erase(devices.begin());
			}
		}

        m_inited = false;
    }
 
    device* input_impl::get_device (device::type_t type, size_t indx/*=0*/) const
    {
		if (!m_inited)
			return 0;

		if (type >= device::total_number)
			return 0;

		if (m_devices[type].empty() || indx >= m_devices[type].size())
			return 0;
		
		return m_devices[type][indx];
    }

    device* input_impl::get_device (const std::wstring &sDeviceName, size_t indx/*=0*/) const
    {
        return get_device(String2Device(sDeviceName), indx);
    }

    bool input_impl::is_present (device::type_t type, size_t indx/*=0*/) const
    {
		return get_device(type, indx) ? true : false;
    }

    bool input_impl::is_present (const std::wstring &sDeviceName, size_t indx/*=0*/) const
    {
        return is_present(String2Device(sDeviceName), indx);
    }

	//////////////////////////////////////////////////////////////////////////

    command_ptr input_impl::add_command (const std::string &command_name)
    {
        if (m_inited && !is_command_present(command_name))
        {
            command_ptr command(new Command(command_name, *this));
            m_commands.push_back(command);
			return command;
        }

		return command_ptr();
    }

    command_ptr input_impl::get_command (const std::string &command_name)
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

    bool input_impl::is_command_present (const std::string &command_name) const
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
    void input_impl::detach_command (command_ptr pCommand)
    {
        if (!m_inited)
            return;

		for(size_t device_type = 0; device_type < device::total_number; ++device_type)
		{
			devices_vector& devices = m_devices[device_type];
			for(size_t i = 0; i < devices.size(); ++i)
			{
				devices[i]->detach_command(pCommand);

			}
		}
    }

    //инициализация DXInput
    bool input_impl::init_input (HWND hWnd, bool exclusive, bool foreground)
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

        m_dxinput       = NULL; //устройство DInput
        m_keyboard = NULL; //устройство ввода "клавиатура"
        m_mouse    = NULL; //устройство ввода "мышь"

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
                    (VOID**)&m_dxinput,
                    NULL
                )
            )
        )
        {
            doneDXInput(); //assert(0);
            return false; //... can't create DInput object
        }

        ///////////////////////////////////////////
        // создаем устройство ввода "клавиатура" //
        ///////////////////////////////////////////
        if (FAILED (hr = m_dxinput->CreateDevice (GUID_SysKeyboard, &m_keyboard, NULL)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't create keyboard device
        }

        //устанавливаем формат данных для устройства ввода "клавиатура"
        if (FAILED (hr = m_keyboard->SetDataFormat (&c_dfDIKeyboard)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set data format for keyboard device
        }

        //устанавливаем способ доступа к клавиатуре
        if (FAILED (hr = m_keyboard->SetCooperativeLevel (hWnd, flags)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set cooperative level for keyboard device
        }

        //задаем буферизованный доступ к клавиатуре
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = KEYBOARD_BUFFER_SIZE;

        if (FAILED (hr = m_keyboard->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set buffered device data for keyboard device
        }

        //захватываем клавиатуру
        m_keyboard->Acquire();

        /////////////////////////////////////
        // создаем устройство ввода "мышь" //
        /////////////////////////////////////
        if (FAILED (hr = m_dxinput->CreateDevice (GUID_SysMouse, &m_mouse, NULL)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't create mouse device
        }

        //устанавливаем формат данных для устройства ввода "мышь"
        if (FAILED (hr = m_mouse->SetDataFormat (&c_dfDIMouse2)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set data format for mouse device
        }

        //устанавливаем способ доступа к мыши
        if (FAILED (hr = m_mouse->SetCooperativeLevel (hWnd,flags)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set cooperative level for mouse device
        }

        //задаем буферизованный доступ к мыши
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = MOUSE_BUFFER_SIZE;

        if (FAILED (hr = m_mouse->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set buffered device data for mouse device
        }

        //захватываем мышь
        m_mouse->Acquire();

        //запомним параметры инициализации
        m_hwnd = hWnd;
        m_exclusive = exclusive;
        m_foreground = foreground;

        return true;
    }

    //ДЕинициализация DXInput
    void input_impl::doneDXInput()
    {
        if (m_mouse)    m_mouse   ->Unacquire();
        if (m_keyboard) m_keyboard->Unacquire();

        if (m_mouse)    m_mouse   ->Release();
        if (m_keyboard) m_keyboard->Release();
        if (m_dxinput)       m_dxinput      ->Release();

        m_mouse    = NULL; //устройство ввода "мышь"
        m_keyboard = NULL; //устройство ввода "клавиатура"
        m_dxinput       = NULL; //устройство DInput
    }

    //обработка клавиатурных событий
    void input_impl::kProcess (DIDEVICEOBJECTDATA dod)
    {
		//определим, от какого контрола пришло сообщение
		controls eControl = InvalidControl;
		switch (dod.dwOfs)
		{
		    case DIK_ESCAPE       : eControl = KeyEscape	; break;
		    case DIK_1            : eControl = Key1		; break;
		    case DIK_2            : eControl = Key2		; break;
		    case DIK_3            : eControl = Key3		; break;
		    case DIK_4            : eControl = Key4		; break;
		    case DIK_5            : eControl = Key5		; break;
		    case DIK_6            : eControl = Key6            ; break;
		    case DIK_7            : eControl = Key7            ; break;
		    case DIK_8            : eControl = Key8            ; break;
		    case DIK_9            : eControl = Key9            ; break;
		    case DIK_0            : eControl = Key0            ; break;
		    case DIK_MINUS        : eControl = KeyMinus        ; break;
		    case DIK_EQUALS       : eControl = KeyEquals       ; break;
		    case DIK_BACK         : eControl = KeyBack         ; break;
		    case DIK_TAB          : eControl = KeyTab          ; break;
		    case DIK_Q            : eControl = KeyQ            ; break;
		    case DIK_W            : eControl = KeyW            ; break;
		    case DIK_E            : eControl = KeyE            ; break;
		    case DIK_R            : eControl = KeyR            ; break;
		    case DIK_T            : eControl = KeyT            ; break;
		    case DIK_Y            : eControl = KeyY            ; break;
		    case DIK_U            : eControl = KeyU            ; break;
		    case DIK_I            : eControl = KeyI            ; break;
		    case DIK_O            : eControl = KeyO            ; break;
		    case DIK_P            : eControl = KeyP            ; break;
		    case DIK_LBRACKET     : eControl = KeyLBracket     ; break;
		    case DIK_RBRACKET     : eControl = KeyRBracket     ; break;
		    case DIK_RETURN       : eControl = KeyReturn       ; break;
		    case DIK_LCONTROL     : eControl = KeyLControl     ; break;
		    case DIK_A            : eControl = KeyA            ; break;
		    case DIK_S            : eControl = KeyS            ; break;
		    case DIK_D            : eControl = KeyD            ; break;
		    case DIK_F            : eControl = KeyF            ; break;
		    case DIK_G            : eControl = KeyG            ; break;
		    case DIK_H            : eControl = KeyH            ; break;
		    case DIK_J            : eControl = KeyJ            ; break;
		    case DIK_K            : eControl = KeyK            ; break;
		    case DIK_L            : eControl = KeyL            ; break;
		    case DIK_SEMICOLON    : eControl = KeySemicolon    ; break;
		    case DIK_APOSTROPHE   : eControl = KeyApostrophe   ; break;
		    case DIK_GRAVE        : eControl = KeyGrave        ; break;
		    case DIK_LSHIFT       : eControl = KeyLShift       ; break;
		    case DIK_BACKSLASH    : eControl = KeyBackSlash    ; break;
		    case DIK_Z            : eControl = KeyZ            ; break;
		    case DIK_X            : eControl = KeyX            ; break;
		    case DIK_C            : eControl = KeyC            ; break;
		    case DIK_V            : eControl = KeyV            ; break;
		    case DIK_B            : eControl = KeyB            ; break;
		    case DIK_N            : eControl = KeyN            ; break;
		    case DIK_M            : eControl = KeyM            ; break;
		    case DIK_COMMA        : eControl = KeyComma        ; break;
		    case DIK_PERIOD       : eControl = KeyPeriod       ; break;
		    case DIK_SLASH        : eControl = KeySlash        ; break;
		    case DIK_RSHIFT       : eControl = KeyRShift       ; break;
		    case DIK_MULTIPLY     : eControl = KeyMultiply     ; break;
		    case DIK_LMENU        : eControl = KeyLMenu        ; break;
		    case DIK_SPACE        : eControl = KeySpace        ; break;
		    case DIK_CAPITAL      : eControl = KeyCapital      ; break;
		    case DIK_F1           : eControl = KeyF1           ; break;
		    case DIK_F2           : eControl = KeyF2           ; break;
		    case DIK_F3           : eControl = KeyF3           ; break;
		    case DIK_F4           : eControl = KeyF4           ; break;
		    case DIK_F5           : eControl = KeyF5           ; break;
		    case DIK_F6           : eControl = KeyF6           ; break;
		    case DIK_F7           : eControl = KeyF7           ; break;
		    case DIK_F8           : eControl = KeyF8           ; break;
		    case DIK_F9           : eControl = KeyF9           ; break;
		    case DIK_F10          : eControl = KeyF10          ; break;
		    case DIK_NUMLOCK      : eControl = KeyNumLock      ; break;
		    case DIK_SCROLL       : eControl = KeyScroll       ; break;
		    case DIK_NUMPAD7      : eControl = KeyNumPad7      ; break;
		    case DIK_NUMPAD8      : eControl = KeyNumPad8      ; break;
		    case DIK_NUMPAD9      : eControl = KeyNumPad9      ; break;
		    case DIK_SUBTRACT     : eControl = KeySubtract     ; break;
		    case DIK_NUMPAD4      : eControl = KeyNumPad4      ; break;
		    case DIK_NUMPAD5      : eControl = KeyNumPad5      ; break;
		    case DIK_NUMPAD6      : eControl = KeyNumPad6      ; break;
		    case DIK_ADD          : eControl = KeyAdd          ; break;
		    case DIK_NUMPAD1      : eControl = KeyNumPad1      ; break;
		    case DIK_NUMPAD2      : eControl = KeyNumPad2      ; break;
		    case DIK_NUMPAD3      : eControl = KeyNumPad3      ; break;
		    case DIK_NUMPAD0      : eControl = KeyNumPad0      ; break;
		    case DIK_DECIMAL      : eControl = KeyDecimal      ; break;
		    case DIK_OEM_102      : eControl = KeyOEM_102      ; break;
		    case DIK_F11          : eControl = KeyF11          ; break;
		    case DIK_F12          : eControl = KeyF12          ; break;
		    case DIK_F13          : eControl = KeyF13          ; break;
		    case DIK_F14          : eControl = KeyF14          ; break;
		    case DIK_F15          : eControl = KeyF15          ; break;
		    case DIK_KANA         : eControl = KeyKana         ; break;
		    case DIK_ABNT_C1      : eControl = KeyABNT_C1      ; break;
		    case DIK_CONVERT      : eControl = KeyConvert      ; break;
		    case DIK_NOCONVERT    : eControl = KeyNoConvert    ; break;
		    case DIK_YEN          : eControl = KeyYen          ; break;
		    case DIK_ABNT_C2      : eControl = KeyABNT_C2      ; break;
		    case DIK_NUMPADEQUALS : eControl = KeyNumPadEquals ; break;
		    case DIK_PREVTRACK    : eControl = KeyPrevTrack    ; break;
		    case DIK_AT           : eControl = KeyAt           ; break;
		    case DIK_COLON        : eControl = KeyColon        ; break;
		    case DIK_UNDERLINE    : eControl = KeyUnderLine    ; break;
		    case DIK_KANJI        : eControl = KeyKANJI        ; break;
		    case DIK_STOP         : eControl = KeyStop         ; break;
		    case DIK_AX           : eControl = KeyAx           ; break;
		    case DIK_UNLABELED    : eControl = KeyUnlabeled    ; break;
		    case DIK_NEXTTRACK    : eControl = KeyNextTrack    ; break;
		    case DIK_NUMPADENTER  : eControl = KeyNumPadEnter  ; break;
		    case DIK_RCONTROL     : eControl = KeyRControl     ; break;
		    case DIK_MUTE         : eControl = KeyMute         ; break;
		    case DIK_CALCULATOR   : eControl = KeyCalculator   ; break;
		    case DIK_PLAYPAUSE    : eControl = KeyPlayPause    ; break;
		    case DIK_MEDIASTOP    : eControl = KeyMediaStop    ; break;
		    case DIK_VOLUMEDOWN   : eControl = KeyVolumeDown   ; break;
		    case DIK_VOLUMEUP     : eControl = KeyVolumeUp     ; break;
		    case DIK_WEBHOME      : eControl = KeyWebHome      ; break;
		    case DIK_NUMPADCOMMA  : eControl = KeyNumPadComma  ; break;
		    case DIK_DIVIDE       : eControl = KeyDivide       ; break;
		    case DIK_SYSRQ        : eControl = KeySysRQ        ; break;
		    case DIK_RMENU        : eControl = KeyRMenu        ; break;
		    case DIK_PAUSE        : eControl = KeyPause        ; break;
		    case DIK_HOME         : eControl = KeyHome         ; break;
		    case DIK_UP           : eControl = KeyUp           ; break;
		    case DIK_PRIOR        : eControl = KeyPrior        ; break;
		    case DIK_LEFT         : eControl = KeyLeft         ; break;
		    case DIK_RIGHT        : eControl = KeyRight        ; break;
		    case DIK_END          : eControl = KeyEnd          ; break;
		    case DIK_DOWN         : eControl = KeyDown         ; break;
		    case DIK_NEXT         : eControl = KeyNext         ; break;
		    case DIK_INSERT       : eControl = KeyInsert       ; break;
		    case DIK_DELETE       : eControl = KeyDelete       ; break;
		    case DIK_LWIN         : eControl = KeyLWin         ; break;
		    case DIK_RWIN         : eControl = KeyRWin         ; break;
		    case DIK_APPS         : eControl = KeyApps         ; break;
		}

		if (eControl == InvalidControl)
			return; //... такой контрол не поддерживается

        Control *pControl = keyboard->get_control(eControl);
		if (!pControl)
			return; //... такой контрол для клавиатуры не предусмотрен 

		pControl->m_time  = dod.dwTimeStamp;
		pControl->m_press = (dod.dwData & 0x80) ? true : false;
		pControl->m_delta = 0;

		pControl->notify_all();
    }

    //обработка мышиных событий
    void input_impl::mProcess (DIDEVICEOBJECTDATA dod)
    {
        //определим, от какого контрола пришло сообщение
        controls eControl = InvalidControl;
        switch (dod.dwOfs)
        {
            case DIMOFS_BUTTON0: eControl = ButtonLeft  ; break;
            case DIMOFS_BUTTON1: eControl = ButtonRight ; break;
            case DIMOFS_BUTTON2: eControl = ButtonMid   ; break;
            case DIMOFS_X:       eControl = AxisX       ; break;
            case DIMOFS_Y:       eControl = AxisY       ; break;
            case DIMOFS_Z:       eControl = AxisWheel   ; break;
        }

		if (eControl == InvalidControl)
			return; //... такой контрол не поддерживается

        Control *pControl = mouse->get_control(eControl);
		if (!pControl)
			return; //... такой контрол для мыши не предусмотрен 

        pControl->m_time = dod.dwTimeStamp;

        if (pControl->type() == Control::Axis)
        {
            pControl->m_press = false;
            pControl->m_delta = dod.dwData;
        }
        else if (pControl->type() == Control::Button)
        {
            pControl->m_press = (dod.dwData & 0x80) ? true : false;
            pControl->m_delta = 0;
        }

        pControl->notify_all();
    }
}