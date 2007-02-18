/**--------------------------------------------------------------------------------------

									RGDE Engine

----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru

Created:	15 june 2005
---------------------------------------------------------------------------------------*/
#include "precomp.h"
#include "TypeInfo.h"



namespace test_PacMan
{
	CTypeInfo::TTypeInfoes CTypeInfo::m_vTypeInfoes;


	//-----------------------------------------------------------------------------------
	// Помощник при работе с типами
	struct STypeInfoHelper
	{
		// Автоматическое освобождение памяти
		~STypeInfoHelper();

		CTypeInfo* findType (CBaseObject::eType type);
	};
	STypeInfoHelper typeInfoHelper;

	//-----------------------------------------------------------------------------------
	STypeInfoHelper::~STypeInfoHelper()
	{
		CTypeInfo::TTypeInfoes& m_vTypeInfoes = CTypeInfo::GetTypeInfoes();
		for (CTypeInfo::TTypeInfoesIter it = m_vTypeInfoes.begin(); it != m_vTypeInfoes.end(); ++it)
			delete *it;
	}

	//-----------------------------------------------------------------------------------
	CTypeInfo* STypeInfoHelper::findType (CBaseObject::eType type)
	{
		if (CTypeInfo::m_vTypeInfoes.size() == 0) return 0;

		for (CTypeInfo::TTypeInfoesIter it = CTypeInfo::m_vTypeInfoes.begin(); 
			it != CTypeInfo::m_vTypeInfoes.end(); ++it)
		{
			if ((*it)->getType() == type)
				return *it;
		}
		return 0;
	}


	//-----------------------------------------------------------------------------------
	CTypeInfo* CTypeInfo::AddObject (CBaseObject* pObject)
	{
		CTypeInfo* pTypeInfo = typeInfoHelper.findType (pObject->getType());
		if (!pTypeInfo)	pTypeInfo = new CTypeInfo (pObject->getType());

		pTypeInfo->m_lObjects.push_back (pObject);

		return pTypeInfo;
	}

	
	//-----------------------------------------------------------------------------------
	CTypeInfo::TObjects* CTypeInfo::GetObjects (CBaseObject::eType type)
	{
		CTypeInfo* pTypeInfo = typeInfoHelper.findType (type);
		if (!pTypeInfo) return 0;

		return &pTypeInfo->getObjects();
	}

	/*
	//-----------------------------------------------------------------------------------
	CBaseObject* CScene::getObject (CBaseObject::eBaseType eBaseType, const math::Vec2f& pos)
	{
		CTypeInfo::TObjects* pObjects = getObjects (eBaseType);
		if (!pObjects) return 0;

		for (CTypeInfo::TObjectsIter it = pObjects->begin(); it != pObjects->end(); ++it)
			if (collisionHelper ((*it)->getPos(), pos))
				return *it;
		return 0;
	}
	*/

}