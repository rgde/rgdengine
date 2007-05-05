//inputimpl.cpp
#include "precompiled.h"
#include "input/inputimpl.h"
#include "input/device.h"
#include "input/control.h"
#include "input/command.h"
#include <sstream>

#define KEYBOARD_BUFFER_SIZE 1024
#define    MOUSE_BUFFER_SIZE 1024

#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

namespace input
{

    CInputImpl::CInputImpl ():
        m_bInit     (false), //считаем, что система ввода не инициализированна
        m_pDI       (NULL),  //устройство DInput
        m_pKeyboard (NULL),  //устройство ввода "клавиатура"
        m_pMouse    (NULL),  //устройство ввода "мышь"
        keyboard    (NULL),
        mouse       (NULL)
    {
    }

    CInputImpl::~CInputImpl ()
    {
        if (m_bInit)
            Done();
    }
 
    /////////////////////////////////////////////////
    // основные методы для работы с системой ввода //
    /////////////////////////////////////////////////

    //изменить режим работы устройств ввода
    bool CInputImpl::SetMode (bool exclusive/*=false*/, bool foreground/*=true*/)
    {
        if (!m_bInit)
            return false;

        //убить старое подключение к устройствам ввода
        doneDXInput();

        //выполнить подключение к устройствам ввода с новыми настройками
        if (!initDXInput(m_hWnd,exclusive,foreground))
        {
            //если не удалось, то подключится со старыми настройками
            initDXInput(m_hWnd,m_exclusive,m_foreground);
            return false;
        }

        return true;
    }

    //проинициализировать систему ввода
    bool CInputImpl::Init (HWND hWnd, bool exclusive/*=false*/, bool foreground/*=true*/)
    {
        if (m_bInit)
            Done();

        //инициализируем DXInput
        m_bInit = initDXInput (hWnd,exclusive,foreground);
        if (!m_bInit)
            return false;

        //заполним массив m_devices
        m_devices.push_back (new CDevice(Keyboard, 0, *this));
        m_devices.push_back (new CDevice(Mouse,    0, *this));

        //заполним список контролов
        keyboard = *(m_devices.begin());
        mouse    = *(++m_devices.begin());

        //keyboard
        keyboard->addButton(KeyEscape);
        keyboard->addButton(Key1);
        keyboard->addButton(Key2);
        keyboard->addButton(Key3);
        keyboard->addButton(Key4);
        keyboard->addButton(Key5);
        keyboard->addButton(Key6);
        keyboard->addButton(Key7);
        keyboard->addButton(Key8);
        keyboard->addButton(Key9);
        keyboard->addButton(Key0);
        keyboard->addButton(KeyMinus);
        keyboard->addButton(KeyEquals);
        keyboard->addButton(KeyBack);
        keyboard->addButton(KeyTab);
        keyboard->addButton(KeyQ);
        keyboard->addButton(KeyW);
        keyboard->addButton(KeyE);
        keyboard->addButton(KeyR);
        keyboard->addButton(KeyT);
        keyboard->addButton(KeyY);
        keyboard->addButton(KeyU);
        keyboard->addButton(KeyI);
        keyboard->addButton(KeyO);
        keyboard->addButton(KeyP);
        keyboard->addButton(KeyLBracket);
        keyboard->addButton(KeyRBracket);
        keyboard->addButton(KeyReturn);
        keyboard->addButton(KeyLControl);
        keyboard->addButton(KeyA);
        keyboard->addButton(KeyS);
        keyboard->addButton(KeyD);
        keyboard->addButton(KeyF);
        keyboard->addButton(KeyG);
        keyboard->addButton(KeyH);
        keyboard->addButton(KeyJ);
        keyboard->addButton(KeyK);
        keyboard->addButton(KeyL);
        keyboard->addButton(KeySemicolon);
        keyboard->addButton(KeyApostrophe);
        keyboard->addButton(KeyGrave);
        keyboard->addButton(KeyLShift);
        keyboard->addButton(KeyBackSlash);
        keyboard->addButton(KeyZ);
        keyboard->addButton(KeyX);
        keyboard->addButton(KeyC);
        keyboard->addButton(KeyV);
        keyboard->addButton(KeyB);
        keyboard->addButton(KeyN);
        keyboard->addButton(KeyM);
        keyboard->addButton(KeyComma);
        keyboard->addButton(KeyPeriod);
        keyboard->addButton(KeySlash);
        keyboard->addButton(KeyRShift);
        keyboard->addButton(KeyMultiply);
        keyboard->addButton(KeyLMenu);
        keyboard->addButton(KeySpace);
        keyboard->addButton(KeyCapital);
        keyboard->addButton(KeyF1);
        keyboard->addButton(KeyF2);
        keyboard->addButton(KeyF3);
        keyboard->addButton(KeyF4);
        keyboard->addButton(KeyF5);
        keyboard->addButton(KeyF6);
        keyboard->addButton(KeyF7);
        keyboard->addButton(KeyF8);
        keyboard->addButton(KeyF9);
        keyboard->addButton(KeyF10);
        keyboard->addButton(KeyNumLock);
        keyboard->addButton(KeyScroll);
        keyboard->addButton(KeyNumPad7);
        keyboard->addButton(KeyNumPad8);
        keyboard->addButton(KeyNumPad9);
        keyboard->addButton(KeySubtract);
        keyboard->addButton(KeyNumPad4);
        keyboard->addButton(KeyNumPad5);
        keyboard->addButton(KeyNumPad6);
        keyboard->addButton(KeyAdd);
        keyboard->addButton(KeyNumPad1);
        keyboard->addButton(KeyNumPad2);
        keyboard->addButton(KeyNumPad3);
        keyboard->addButton(KeyNumPad0);
        keyboard->addButton(KeyDecimal);
        keyboard->addButton(KeyOEM_102);
        keyboard->addButton(KeyF11);
        keyboard->addButton(KeyF12);
        keyboard->addButton(KeyF13);
        keyboard->addButton(KeyF14);
        keyboard->addButton(KeyF15);
        keyboard->addButton(KeyKana);
        keyboard->addButton(KeyABNT_C1);
        keyboard->addButton(KeyConvert);
        keyboard->addButton(KeyNoConvert);
        keyboard->addButton(KeyYen);
        keyboard->addButton(KeyABNT_C2);
        keyboard->addButton(KeyNumPadEquals);
        keyboard->addButton(KeyPrevTrack);
        keyboard->addButton(KeyAt);
        keyboard->addButton(KeyColon);
        keyboard->addButton(KeyUnderLine);
        keyboard->addButton(KeyKANJI);
        keyboard->addButton(KeyStop);
        keyboard->addButton(KeyAx);
        keyboard->addButton(KeyUnlabeled);
        keyboard->addButton(KeyNextTrack);
        keyboard->addButton(KeyNumPadEnter);
        keyboard->addButton(KeyRControl);
        keyboard->addButton(KeyMute);
        keyboard->addButton(KeyCalculator);
        keyboard->addButton(KeyPlayPause);
        keyboard->addButton(KeyMediaStop);
        keyboard->addButton(KeyVolumeDown);
        keyboard->addButton(KeyVolumeUp);
        keyboard->addButton(KeyWebHome);
        keyboard->addButton(KeyNumPadComma);
        keyboard->addButton(KeyDivide);
        keyboard->addButton(KeySysRQ);
        keyboard->addButton(KeyRMenu);
        keyboard->addButton(KeyPause);
        keyboard->addButton(KeyHome);
        keyboard->addButton(KeyUp);
        keyboard->addButton(KeyPrior);
        keyboard->addButton(KeyLeft);
        keyboard->addButton(KeyRight);
        keyboard->addButton(KeyEnd);
        keyboard->addButton(KeyDown);
        keyboard->addButton(KeyNext);
        keyboard->addButton(KeyInsert);
        keyboard->addButton(KeyDelete);
        keyboard->addButton(KeyLWin);
        keyboard->addButton(KeyRWin);
        keyboard->addButton(KeyApps);

        //mouse
        mouse->addButton (ButtonLeft);
        mouse->addButton (ButtonMid);
        mouse->addButton (ButtonRight);
        mouse->addAxis   (AxisX);
        mouse->addAxis   (AxisY);
        mouse->addAxis   (AxisWheel);

        return true;
    }

    //загрузить раскладку
    void CInputImpl::Load (const std::string &sXml)
    {
        if (!m_bInit)
            return;

		TiXmlDocument doc;
		doc.Parse(sXml.c_str());
		
		TiXmlNode *root = doc.FirstChild("input");
		if (root)
		{
			TiXmlNode *command = root->FirstChild("command");
			while (command)
			{
				TiXmlElement *cmd = command->ToElement();
				if (cmd)
				{
					std::string sCommandName(cmd->Attribute("name"));
					addCommand(std::wstring(sCommandName.begin(), sCommandName.end()));

					TiXmlNode *control = command->FirstChild("control");
					while (control)
					{
						TiXmlElement *ctrl = control->ToElement();
						if (ctrl)
						{
							std::string sDevice(ctrl->Attribute("device"));
							std::string sControl(ctrl->Attribute("name"));
							CDevice *d = getDevice(String2Device(std::wstring(sDevice.begin(), sDevice.end())));
							if (d)
							{
								CControl *c = d->getControl(String2Control(std::wstring(sControl.begin(), sControl.end())));
								if (c)
									c->bind(getCommand(std::wstring(sCommandName.begin(), sCommandName.end())));
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
    void CInputImpl::Update ()
    {
        if (!m_bInit)
            return;

        static DIDEVICEOBJECTDATA kbuf [KEYBOARD_BUFFER_SIZE]; //буфер для клавиатурных событий
        static DIDEVICEOBJECTDATA mbuf [   MOUSE_BUFFER_SIZE]; //буфер для событий от мыши
        DWORD                     kElements;                   //кол-во клавиатурных событий
        DWORD                     mElements;                   //кол-во событий от мыши
        HRESULT                   hr;

        //получим сообщения от клавиатуры
        kElements = KEYBOARD_BUFFER_SIZE;
        hr = m_pKeyboard->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), kbuf, &kElements, 0);
        if (hr != DI_OK) 
        {
            if (hr == DI_BUFFEROVERFLOW)
                ; //... keyboard buffer overflow
            else
                kElements = 0;
            hr = m_pKeyboard->Acquire();
            while (hr == DIERR_INPUTLOST) 
                hr = m_pKeyboard->Acquire();
        }

        //получим сообщения от мыши
        mElements = MOUSE_BUFFER_SIZE;
        hr = m_pMouse->GetDeviceData (sizeof(DIDEVICEOBJECTDATA), mbuf, &mElements, 0);
        if (hr != DI_OK) 
        {
            if (hr == DI_BUFFEROVERFLOW)
                ; //... mouse buffer overflow
            else
                mElements = 0;
            hr = m_pMouse->Acquire();
            while (hr == DIERR_INPUTLOST) 
                hr = m_pMouse->Acquire();
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
    void CInputImpl::Save (std::string &sXml)
    {
        if (!m_bInit)
            return;

        TiXmlDocument doc;
        TiXmlElement *input = (TiXmlElement*)(doc.InsertEndChild(TiXmlElement("input")));

        std::list<PCommand>::iterator i = m_commands.begin();
        while (i != m_commands.end())
        {
            PCommand pCommand = *i;

            TiXmlElement *command = (TiXmlElement*)(input->InsertEndChild(TiXmlElement("command")));
            std::wstring sCommandNameW = pCommand->getName();
            std::string sCommandName(sCommandNameW.begin(), sCommandNameW.end());
            command->SetAttribute("name", sCommandName.c_str());

            std::list<CDevice*>::iterator j = m_devices.begin();
            while (j != m_devices.end())
            {
                CDevice *pDevice = *j;

                std::wstring sDeviseNameW = Device2String(pDevice->getName());
                std::string sDeviceName(sDeviseNameW.begin(), sDeviseNameW.end());

                std::map<EControl, CControl*>::iterator k = pDevice->m_controls.begin();
                while (k != pDevice->m_controls.end())
                {
                    CControl *pControl = k->second;
                    if (pControl->isbind(pCommand))
                    {
                        std::wstring sControlNameW = Control2String(pControl->getName());
                        std::string sControlName(sControlNameW.begin(), sControlNameW.end());

                        TiXmlElement *control = (TiXmlElement*)(command->InsertEndChild(TiXmlElement("control")));
                        control->SetAttribute("device", sDeviceName.c_str());
                        control->SetAttribute("name", sControlName.c_str());
                    }
                    ++k;
                }
                ++j;
            }
            ++i;
        }

        std::stringstream ss;
        ss << doc;
        sXml = ss.str();
    }

    //завершить работу системы ввода
    void CInputImpl::Done ()
    {
        if (!m_bInit)
            return;

        doneDXInput ();
        m_commands.clear();

        while (!m_devices.empty())
        {
            delete *(m_devices.begin());
            m_devices.erase(m_devices.begin());
        }

        keyboard = NULL;
        mouse    = NULL;

        m_bInit = false;
    }
 
    ////////////////////////////////
    // доступ к устройствам ввода //
    ////////////////////////////////

    //получить устройство
    CDevice* CInputImpl::getDevice (EDevice eDeviceName, int indx/*=0*/)
    {
        if (!m_bInit)
            return 0;

        std::list<CDevice*>::iterator i = m_devices.begin();
        while (i != m_devices.end())
        {
            if ((*i)->getName() == eDeviceName && (*i)->getIndx() == indx)
                return *i;
            ++i;
        }

        return 0;
    }

    CDevice* CInputImpl::getDevice (const std::wstring &sDeviceName, int indx/*=0*/)
    {
        return getDevice(String2Device(sDeviceName), indx);
    }

    //есть ли такое устройство
    bool CInputImpl::isDevicePresent (EDevice eDeviceName, int indx/*=0*/) const
    {
        if (!m_bInit)
            return false;

        std::list<CDevice*>::const_iterator i = m_devices.begin();
        while (i != m_devices.end())
        {
            if ((*i)->getName() == eDeviceName && (*i)->getIndx() == indx)
                return true;
            ++i;
        }

        return false;
    }

    bool CInputImpl::isDevicePresent (const std::wstring &sDeviceName, int indx/*=0*/) const
    {
        return isDevicePresent(String2Device(sDeviceName), indx);
    }
 
    /////////////////////////////////////
    // доступ к командам системы ввода //
    /////////////////////////////////////

    //добавить команду
    void CInputImpl::addCommand (const std::wstring &sCommandName)
    {
        if (!m_bInit)
            return;

        if (!isCommandPresent(sCommandName))
        {
            PCommand pCommand(new CCommand(sCommandName, *this));
            m_commands.push_back(pCommand);
        }
    }

    //получить команду
    PCommand CInputImpl::getCommand (const std::wstring &sCommandName)
    {
        if (!m_bInit)
            return PCommand();

        std::list<PCommand>::iterator i = m_commands.begin();
        while (i != m_commands.end())
        {
            if ((*i)->getName() == sCommandName)
                return (*i);
            ++i;
        }

        return PCommand();
    }

    //есть ли такая команда
    bool CInputImpl::isCommandPresent (const std::wstring &sCommandName) const
    {
        if (!m_bInit)
            return false;

        std::list<PCommand>::const_iterator i = m_commands.begin();
        while (i != m_commands.end())
        {
            if ((*i)->getName() == sCommandName)
                return true;
            ++i;
        }

        return false;
    }

    //отвязать команду ото всех контролов
    void CInputImpl::detachCommand (PCommand pCommand)
    {
        if (!m_bInit)
            return;

        std::list<CDevice*>::iterator i = m_devices.begin();

        while (i != m_devices.end())
        {
            (*i)->detachCommand(pCommand);
            ++i;
        }
    }

    //инициализация DXInput
    bool CInputImpl::initDXInput (HWND hWnd, bool exclusive, bool foreground)
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

        m_pDI       = NULL; //устройство DInput
        m_pKeyboard = NULL; //устройство ввода "клавиатура"
        m_pMouse    = NULL; //устройство ввода "мышь"

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
                    (VOID**)&m_pDI,
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
        if (FAILED (hr = m_pDI->CreateDevice (GUID_SysKeyboard, &m_pKeyboard, NULL)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't create keyboard device
        }

        //устанавливаем формат данных для устройства ввода "клавиатура"
        if (FAILED (hr = m_pKeyboard->SetDataFormat (&c_dfDIKeyboard)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set data format for keyboard device
        }

        //устанавливаем способ доступа к клавиатуре
        if (FAILED (hr = m_pKeyboard->SetCooperativeLevel (hWnd, flags)))
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

        if (FAILED (hr = m_pKeyboard->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set buffered device data for keyboard device
        }

        //захватываем клавиатуру
        m_pKeyboard->Acquire();

        /////////////////////////////////////
        // создаем устройство ввода "мышь" //
        /////////////////////////////////////
        if (FAILED (hr = m_pDI->CreateDevice (GUID_SysMouse, &m_pMouse, NULL)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't create mouse device
        }

        //устанавливаем формат данных для устройства ввода "мышь"
        if (FAILED (hr = m_pMouse->SetDataFormat (&c_dfDIMouse2)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set data format for mouse device
        }

        //устанавливаем способ доступа к мыши
        if (FAILED (hr = m_pMouse->SetCooperativeLevel (hWnd,flags)))
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

        if (FAILED (hr = m_pMouse->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
        {
            doneDXInput(); //assert(0);
            return false; //... can't set buffered device data for mouse device
        }

        //захватываем мышь
        m_pMouse->Acquire();

        //запомним параметры инициализации
        m_hWnd = hWnd;
        m_exclusive = exclusive;
        m_foreground = foreground;

        return true;
    }

    //ДЕинициализация DXInput
    void CInputImpl::doneDXInput()
    {
        if (m_pMouse)    m_pMouse   ->Unacquire();
        if (m_pKeyboard) m_pKeyboard->Unacquire();

        if (m_pMouse)    m_pMouse   ->Release();
        if (m_pKeyboard) m_pKeyboard->Release();
        if (m_pDI)       m_pDI      ->Release();

        m_pMouse    = NULL; //устройство ввода "мышь"
        m_pKeyboard = NULL; //устройство ввода "клавиатура"
        m_pDI       = NULL; //устройство DInput
    }

    //обработка клавиатурных событий
    void CInputImpl::kProcess (DIDEVICEOBJECTDATA dod)
    {
		//определим, от какого контрола пришло сообщение
		EControl eControl = InvalidControl;
		switch (dod.dwOfs)
		{
		    case DIK_ESCAPE       : eControl = KeyEscape       ; break;
		    case DIK_1            : eControl = Key1            ; break;
		    case DIK_2            : eControl = Key2            ; break;
		    case DIK_3            : eControl = Key3            ; break;
		    case DIK_4            : eControl = Key4            ; break;
		    case DIK_5            : eControl = Key5            ; break;
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

        CControl *pControl = keyboard->getControl(eControl);
		if (!pControl)
			return; //... такой контрол для клавиатуры не предусмотрен 

		pControl->m_nTime  = dod.dwTimeStamp;
		pControl->m_bPress = (dod.dwData & 0x80) ? true : false;
		pControl->m_nDelta = 0;

		pControl->notifyAllObservers();
    }

    //обработка мышиных событий
    void CInputImpl::mProcess (DIDEVICEOBJECTDATA dod)
    {
        //определим, от какого контрола пришло сообщение
        EControl eControl = InvalidControl;
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

        CControl *pControl = mouse->getControl(eControl);
		if (!pControl)
			return; //... такой контрол для клавиатуры не предусмотрен 

        pControl->m_nTime = dod.dwTimeStamp;

        if (pControl->getType() == CControl::Axis)
        {
            pControl->m_bPress = false;
            pControl->m_nDelta = dod.dwData;
        }
        else if (pControl->getType() == CControl::Button)
        {
            pControl->m_bPress = (dod.dwData & 0x80) ? true : false;
            pControl->m_nDelta = 0;
        }

        pControl->notifyAllObservers();
    }

}