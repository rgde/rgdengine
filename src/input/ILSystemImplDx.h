#pragma once
//реализация системы ввода посредством DInput

#define DIRECTINPUT_VERSION 0x0800

#include "input/ILBase.h"
#include "ILSystemImpl.h"
#include <windows.h>
#include <dinput.h>

namespace input
{

	class CSystemImplDx: public ISystemImpl
	{
	public:
		CSystemImplDx (HWND hwnd, bool exclusive=false, bool foreground=true);
		~CSystemImplDx ();

		//апдейт системы ввода (получение информации от
		//устройств ввода и рассылка их по контролам)
		void update ();

	private:
		LPDIRECTINPUT8       m_pDI;        //устройство DInput
		LPDIRECTINPUTDEVICE8 m_pKeyboard;  //устройство ввода "клавиатура"
		LPDIRECTINPUTDEVICE8 m_pMouse;     //устройство ввода "мышь"

		void kProcess (DIDEVICEOBJECTDATA dod); //обработка клавиатурных событий
		void mProcess (DIDEVICEOBJECTDATA dod); //обработка мышиных событий
	};

}