#include "precompiled.h"

#include "input/ILSystemImplDx.h"
#include "input/ILControl.h"
#include "input/ILSystem.h"

#define KEYBOARD_BUFFER_SIZE 1024
#define    MOUSE_BUFFER_SIZE 1024


namespace input
{

	CSystemImplDx::CSystemImplDx (HWND hwnd, bool exclusive/*=false*/, bool foreground/*=true*/)
	{
		//////////////////////////////////////////////////////////////////////////
		//			!!!	HACK  !!! TO OBTAIN TOP LEVEL WINDOW					//
		//////////////////////////////////////////////////////////////////////////
		{
			HWND cur = hwnd;
			for(;;)
			{
				HWND temp_hwnd = GetParent(cur);
				if (temp_hwnd == NULL) 
				{
					hwnd = cur;
					break;
				}
				cur = temp_hwnd;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		DWORD test = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;

		DWORD flags = 0;
		flags |= exclusive  ? DISCL_EXCLUSIVE  : DISCL_NONEXCLUSIVE;
		flags |= foreground ? DISCL_FOREGROUND : DISCL_BACKGROUND;

		HRESULT     hr;     //результат успеха/ошибки
		DIPROPDWORD dipdw;  //для задания буферизованного ввода клавиатуры и мыши

		m_pDI        = NULL; //устройство DInput
		m_pKeyboard  = NULL; //устройство ввода "клавиатура"
		m_pMouse     = NULL; //устройство ввода "мышь"
		//... другие устройства ввода

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
			assert(0);
			return; //... can't create DInput object
		}

		///////////////////////////////////////////
		// создаем устройство ввода "клавиатура" //
		///////////////////////////////////////////
		if (FAILED (hr = m_pDI->CreateDevice (GUID_SysKeyboard, &m_pKeyboard, NULL)))
		{
			assert(0);
			return; //... can't create keyboard device
		}

		//устанавливаем формат данных для устройства ввода "клавиатура"
		if (FAILED (hr = m_pKeyboard->SetDataFormat (&c_dfDIKeyboard)))
		{
			assert(0);
			return; //... can't set data format for keyboard device
		}

		//устанавливаем способ доступа к клавиатуре
		if (FAILED (hr = m_pKeyboard->SetCooperativeLevel (hwnd, flags)))
		{
			assert(0);
			return; //... can't set cooperative level for keyboard device
		}

		//задаем буферизованный доступ к клавиатуре
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = KEYBOARD_BUFFER_SIZE;

		if (FAILED (hr = m_pKeyboard->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
		{
			assert(0);
			return; //... can't set buffered device data for keyboard device
		}

		//захватываем клавиатуру
		m_pKeyboard->Acquire();

		/////////////////////////////////////
		// создаем устройство ввода "мышь" //
		/////////////////////////////////////
		if (FAILED (hr = m_pDI->CreateDevice (GUID_SysMouse, &m_pMouse, NULL)))
		{
			assert(0);
			return; //... can't create mouse device
		}

		//устанавливаем формат данных для устройства ввода "мышь"
		if (FAILED (hr = m_pMouse->SetDataFormat (&c_dfDIMouse2)))
		{
			assert(0);
			return; //... can't set data format for mouse device
		}

		//устанавливаем способ доступа к мыши
		if (FAILED (hr = m_pMouse->SetCooperativeLevel (hwnd,flags)))
		{
			assert(0);
			return; //... can't set cooperative level for mouse device
		}

		//задаем буферизованный доступ к мыши
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = MOUSE_BUFFER_SIZE;

		if (FAILED (hr = m_pMouse->SetProperty (DIPROP_BUFFERSIZE, &dipdw.diph)))
		{
			assert(0);
			return; //... can't set buffered device data for mouse device
		}

		//захватываем мышь
		m_pMouse->Acquire();

		//////////////////////////////
		// заполним карту устройств //
		//////////////////////////////
		CDevice *keyboard = newDevice(Keyboard);
		CDevice *mouse    = newDevice(Mouse);

		/////////////////////////////////////////
		// заполним карту контролов клавиатуры //
		/////////////////////////////////////////
		addControl (Keyboard, new CControl(KeyEscape,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key1,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key2,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key3,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key4,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key5,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key6,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key7,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key8,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key9,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(Key0,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyMinus,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyEquals,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyBack,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyTab,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyQ,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyW,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyE,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyR,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyT,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyY,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyU,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyI,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyO,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyP,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyLBracket,     CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyRBracket,     CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyReturn,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyLControl,     CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyA,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyS,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyD,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyG,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyH,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyJ,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyK,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyL,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeySemicolon,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyApostrophe,   CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyGrave,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyLShift,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyBackSlash,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyZ,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyX,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyC,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyV,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyB,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyN,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyM,            CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyComma,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyPeriod,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeySlash,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyRShift,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyMultiply,     CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyLMenu,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeySpace,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyCapital,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF1,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF2,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF3,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF4,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF5,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF6,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF7,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF8,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF9,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF10,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumLock,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyScroll,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad7,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad8,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad9,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeySubtract,     CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad4,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad5,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad6,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyAdd,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad1,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad2,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad3,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPad0,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyDecimal,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyOEM_102,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF11,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF12,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF13,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF14,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyF15,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyKana,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyABNT_C1,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyConvert,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNoConvert,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyYen,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyABNT_C2,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPadEquals, CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyPrevTrack,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyAt,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyColon,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyUnderLine,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyKANJI,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyStop,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyAx,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyUnlabeled,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNextTrack,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPadEnter,  CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyRControl,     CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyMute,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyCalculator,   CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyPlayPause,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyMediaStop,    CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyVolumeDown,   CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyVolumeUp,     CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyWebHome,      CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNumPadComma,  CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyDivide,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeySysRQ,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyRMenu,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyPause,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyHome,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyUp,           CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyPrior,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyLeft,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyRight,        CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyEnd,          CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyDown,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyNext,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyInsert,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyDelete,       CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyLWin,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyRWin,         CControl::Button, *keyboard));
		addControl (Keyboard, new CControl(KeyApps,         CControl::Button, *keyboard));

		//////////////////////////////////
		// заполним карту котролов мыши //
		//////////////////////////////////
		addControl (Mouse, new CControl(ButtonLeft,  CControl::Button, *mouse));
		addControl (Mouse, new CControl(ButtonMid,   CControl::Button, *mouse));
		addControl (Mouse, new CControl(ButtonRight, CControl::Button, *mouse));
		addControl (Mouse, new CControl(AxisX,       CControl::Axis,   *mouse));
		addControl (Mouse, new CControl(AxisY,       CControl::Axis,   *mouse));
		addControl (Mouse, new CControl(AxisWheel,   CControl::Axis,   *mouse));
	}

	CSystemImplDx::~CSystemImplDx ()
	{
		//... отсоединяем другие устройства ввода
		if (m_pMouse)    m_pMouse   ->Unacquire();
		if (m_pKeyboard) m_pKeyboard->Unacquire();

		//... освобождаем другие устройства ввода
		if (m_pMouse)    m_pMouse   ->Release();
		if (m_pKeyboard) m_pKeyboard->Release();
		if (m_pDI)       m_pDI      ->Release();
	}

	//извлечь из буфера все события от устройств ввода
	void CSystemImplDx::update ()
	{
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

			//->
			//очистим очередь оконных сообщений
			/*
			MSG msg;
			while (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
			{
			GetMessage(&msg,NULL,0,0);
			DispatchMessage(&msg);
			}
			*/
			//-<
	}

	//обработка клавиатурных событий
	void CSystemImplDx::kProcess (DIDEVICEOBJECTDATA dod)
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

		CControl *pControl = CSystem::GetControl(Keyboard,eControl);
		if (!pControl)
			return; //... такой контрол в IL для клавиатуры не предусмотрен 

		pControl->setTime(dod.dwTimeStamp);
		pControl->setPress((dod.dwData & 0x80) ? true : false);
		pControl->setDelta(0);

		pControl->notifyAllObservers();
	}

	//обработка мышиных событий
	void CSystemImplDx::mProcess (DIDEVICEOBJECTDATA dod)
	{
		//определим, от какого контрола пришло сообщение
		EControl eControl = InvalidControl;
		switch (dod.dwOfs)
		{
		case DIMOFS_BUTTON0: eControl = ButtonLeft;  break;
		case DIMOFS_BUTTON1: eControl = ButtonRight; break;
		case DIMOFS_BUTTON2: eControl = ButtonMid;   break;
		case DIMOFS_X:       eControl = AxisX;       break;
		case DIMOFS_Y:       eControl = AxisY;       break;
		case DIMOFS_Z:       eControl = AxisWheel;   break;
		}

		if (eControl == InvalidControl)
			return; //... такой контрол не поддерживается

		CControl *pControl = CSystem::GetControl(Mouse,eControl);
		if (!pControl)
			return; //... такой контрол для в IL для клавиатуры не предусмотрен 

		pControl->setTime(dod.dwTimeStamp);

		if (pControl->getType() == CControl::Axis)
		{
			pControl->setPress(false);
			pControl->setDelta(dod.dwData);
		}
		else if (pControl->getType() == CControl::Button)
		{
			pControl->setPress((dod.dwData & 0x80) ? true : false);
			pControl->setDelta(0);
		}

		pControl->notifyAllObservers();
	}

} //namespace input