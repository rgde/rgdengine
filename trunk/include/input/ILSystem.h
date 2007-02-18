#pragma once

#include <boost/noncopyable.hpp>

#include "input/ILBase.h"

//синглтон системы ввода
namespace input
{
	//////////////////////////////////
	// основной класс системы ввода //
	//////////////////////////////////
	class CSystem : boost::noncopyable
	{
	public:
		static CSystem& Get (); //получить ссылку на синглтон системы ввода
		static void Destroy ();	//удалить синглтон системы ввода

		void init (ISystemImpl*);	//инициализировать систему ввода

		//доступ к устройствам ввода
		static CDevice* GetDevice       (EDevice); //получить устройство
		static bool     IsDevicePresent (EDevice); //проверить, существует ли указанное устройство

		//доступ к контролам
		static CControl* GetControl       (EDevice, EControl); //получить контрол
		static bool      IsControlPresent (EDevice, EControl); //проверить, существует ли указанныйы контрол

		//апдейт системы ввода (получение информации от
		//устройств ввода и рассылка их по контролам)
		static void Update ();

	private:
		CSystem ();
		~CSystem ();

		static CSystem            *ms_pInstance; //указатель на едиственный экземпл€р System
		std::auto_ptr<ISystemImpl> m_pImpl;      //конкретна€ реализаци€ системы ввода
	};
}