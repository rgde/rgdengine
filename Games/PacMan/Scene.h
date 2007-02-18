/**--------------------------------------------------------------------------------------

									RGDE Engine

----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru

Created:	15 june 2005
---------------------------------------------------------------------------------------*/
#pragma once

#include "Base.h"
#include "TypeInfo.h"

#include "render/renderManager.h"
#include "game/DynamicObject.h"



namespace test_PacMan
{

	//-----------------------------------------------------------------------------------
	// Класс сцены
	class CScene : public render::IRendererable//, public game::IDynamicObject
	{
	public:
		CScene();

		void createLevel();
		void update(float dt);
		void render();

		CBaseObject* create (CBaseObject::eType type, const math::Vec2f& pos);

		bool isMoveable (const math::Vec2f& pos);

	private:
		typedef std::list<CBaseObject> TObjects;
		typedef TObjects::iterator TObjectsIter;
		TObjects m_lObjects;						// Все объекты уровня

		/* Время, через которое закончится проигрывание вступительной музыки и начнётся игра.
		Вступительная музыка играется только в начале уровня */
		float		m_fIntroMusicTime;
		float		m_fPacManRespawnTime;			// Время респаунинга ПакМэна
		float		m_fPacManDeathTime;				// Время, за которое ПакМэн помирает
		math::Vec2f m_vPacManEmitterPos;			// Позиция появления ПакМэна

		math::Vec2f m_vGhostEmitterPos;				// Позиция появления призраков
		float		m_fGhostEmitterRate;			// Частота появления призраков
	};

	extern CScene g_Scene;							// Сцена

}