/**--------------------------------------------------------------------------------------

									RGDE Engine

----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru

Created:	15 june 2005
---------------------------------------------------------------------------------------*/
#include "precomp.h"
#include "Scene.h"
#include "TypeInfo.h"
#include "Wall.h"



namespace test_PacMan
{
	CScene g_Scene;						// Сцена


	// Помощник, определяет, пересекаются ли объекты
	inline bool collisionHelper(const math::Vec2f& pos1, const math::Vec2f& pos2)
	{
		if ((pos1/CBaseObject::GetSize()) != (pos2/CBaseObject::GetSize()))
			return false;
		return true;
	}

	
	//-----------------------------------------------------------------------------------
	// Помощник сцены
	struct SSceneHelper
	{
	public:
		static void CreateLevel();
	};

	//-----------------------------------------------------------------------------------
	void SSceneHelper::CreateLevel()
	{
		unsigned levelMap[21][19] = {
		//			0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18
		/*0*/	{	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1 },
		/*1*/	{	1,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
		/*2*/	{	1,	0,	1,	1,	0,	1,	1,	1,	0,	1,	0,	1,	1,	1,	0,	1,	1,	0,	1 },
		/*3*/	{	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
		/*4*/	{	1,	0,	1,	1,	0,	1,	0,	1,	1,	1,	1,	1,	0,	1,	0,	1,	1,	0,	1 },
		/*5*/	{	1,	0,	0,	0,	0,	1,	0,	0,	1,	1,	1,	0,	0,	1,	0,	0,	0,	0,	1 },
		/*6*/	{	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	0,	0,	1,	0,	1,	1,	1,	1 },
		/*7*/	{	0,	0,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0,	0 },
		/*8*/	{	1,	1,	1,	1,	0,	1,	0,	1,	0,	0,	0,	1,	0,	1,	0,	1,	1,	1,	1 },
		/*9*/	{	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0 },
		/*10*/	{	1,	1,	1,	1,	0,	1,	0,	1,	1,	1,	1,	1,	0,	1,	0,	1,	1,	1,	1 },
		/*11*/	{	0,	0,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0,	0 },
		/*12*/	{	1,	1,	1,	1,	0,	1,	0,	1,	1,	1,	1,	1,	0,	1,	0,	1,	1,	1,	1 },
		/*13*/	{	1,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
		/*14*/	{	1,	0,	1,	1,	0,	1,	1,	1,	0,	1,	0,	1,	1,	1,	0,	1,	1,	0,	1 },
		/*15*/	{	1,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	1 },
		/*16*/	{	1,	1,	0,	1,	0,	1,	0,	1,	1,	1,	1,	1,	0,	1,	0,	1,	0,	1,	1 },
		/*17*/	{	1,	0,	0,	0,	0,	1,	0,	0,	0,	1,	0,	0,	0,	1,	0,	0,	0,	0,	1 },
		/*18*/	{	1,	0,	1,	1,	1,	1,	1,	1,	0,	1,	0,	1,	1,	1,	1,	1,	1,	0,	1 },
		/*19*/	{	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
		/*20*/	{	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1 }
		};

		for (unsigned i = 0; i < 21; ++i)
			for (unsigned j = 0; j < 19; ++j)
			{
				CBaseObject::eType eObjectType;
				switch (levelMap[i][j])
				{
				case 1:	eObjectType = CBaseObject::eWall; break;
				default: continue;
				}

				g_Scene.create ( eObjectType, math::Vec2f(static_cast<float> (j), static_cast<float> (i))*
					CBaseObject::GetSize() );
			}
	}


	//-----------------------------------------------------------------------------------
	CScene::CScene()
		 : m_fGhostEmitterRate(1.0f)//, render::IRendererable(20)
	{
	}

	//-----------------------------------------------------------------------------------
	void CScene::createLevel()
	{
		SSceneHelper::CreateLevel();
	}

	//-----------------------------------------------------------------------------------
	bool CScene::isMoveable (const math::Vec2f& pos)
	{
		CTypeInfo::TObjects* pObjects = CTypeInfo::GetObjects (CBaseObject::eWall);
		if (!pObjects) assert (false);

		for (CTypeInfo::TObjectsIter it = pObjects->begin(); it != pObjects->end(); ++it)
			if (collisionHelper ((*it)->getPos(), pos))
				return false;
		return true;
	}

	//-----------------------------------------------------------------------------------
	void CScene::update(float dt)
	{
		static float fDeltaTimeElapsed = 0.02f;

		fDeltaTimeElapsed += dt;
		if (fDeltaTimeElapsed < 0.02f) return;

		// Обновление объектов
		for (TObjectsIter it = m_lObjects.begin(); it != m_lObjects.end(); ++it)
			it->update (dt);

		fDeltaTimeElapsed = 0;
	}

	//-----------------------------------------------------------------------------------
	void CScene::render()
	{
		for (TObjectsIter it = m_lObjects.begin(); it != m_lObjects.end(); ++it)
			it->render();
	}

	//-----------------------------------------------------------------------------------
	CBaseObject* CScene::create (CBaseObject::eType type, const math::Vec2f& pos)
	{
		CBaseObject* pNewObject;
		switch (type)
		{
		case CBaseObject::eWall:	pNewObject = new CWall(); break;
		}

		pNewObject->setPos (pos);

		return pNewObject;
	}

}