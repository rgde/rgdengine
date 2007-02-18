/**--------------------------------------------------------------------------------------

									RGDE Engine

----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru

Created:	15 june 2005
---------------------------------------------------------------------------------------*/
#pragma once

#include "Base.h"



namespace test_PacMan
{
	struct STypeInfoHelper;

	//-----------------------------------------------------------------------------------
	// Класс инфы по всем объектам какого-либо типа
	class CTypeInfo
	{
		friend struct STypeInfoHelper;

	public:
		typedef std::list<CTypeInfo*> TTypeInfoes;
		typedef TTypeInfoes::iterator TTypeInfoesIter;

		typedef std::list<CBaseObject*> TObjects;
		typedef TObjects::iterator TObjectsIter;


		CTypeInfo (CBaseObject::eType type) : m_eType(type), m_nAnimated(0) {}

		static CTypeInfo* AddObject (CBaseObject* pObject);
		static CTypeInfo::TObjects* GetObjects (CBaseObject::eType type);

		inline CBaseObject::eType getType() const { return m_eType; }
		inline TObjects& getObjects() { return m_lObjects; }

		static inline TTypeInfoes& GetTypeInfoes() { return m_vTypeInfoes; }


	protected:
		CBaseObject::eType	m_eType;	// Тип объектов, для которых класс хранит инфу
		TObjects	m_lObjects;			// Объекты, относящиеся к данному типу
		unsigned	m_nAnimated;		/* Число объектов данного типа, для которых играется анимация в текущий
										момент времени */
		static TTypeInfoes m_vTypeInfoes;// Список классов с данными по типам
	};
}