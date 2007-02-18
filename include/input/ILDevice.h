#pragma once

#include <map>

#include "input/ILBase.h"


namespace input
{
	class CControl;

	//////////////////////
	// устройство ввода //
	//////////////////////
	class CDevice
	{
		friend class ISystemImpl;

	public:
		~CDevice ();

		EDevice  getName () const {return m_eName;} //получить 'имя' устройства

		CControl* getControl       (EControl); //получить контрол
		bool      isControlPresent (EControl); //есть ли такой контрол

	protected:
		CDevice (EDevice eName): m_eName(eName) {}

	private:
		EDevice                      m_eName;       //'имя' устройства
		std::map<EControl,CControl*> m_mapControls; //контролы, которые есть у устройства
	};
}