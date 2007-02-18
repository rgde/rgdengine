/**--------------------------------------------------------------------------------------

									RGDE Engine

----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru

Created:	15 june 2005
---------------------------------------------------------------------------------------*/
#include "precomp.h"
#include "PacMan.h"



namespace test_PacMan
{
	// Помощник при работе с ПакМэном (создан, чтобы упростить интерфейс ПакМэна для пользователя)
	struct SPacManHelper
	{
		static void StopPill (CPacMan& pacMan);
		static void StartPill (CPacMan& pacMan);
	};

	//-----------------------------------------------------------------------------------
	void SPacManHelper::StopPill (CPacMan& pacMan)
	{
		pacMan.m_isPillActive = false;
		pacMan.m_nEatenPerPill = 0;
		pacMan.m_fPillTimeLeft = 0;
	}

	//-----------------------------------------------------------------------------------
	void SPacManHelper::StartPill (CPacMan& pacMan)
	{
		pacMan.m_isPillActive = true;
		pacMan.m_nEatenPerPill = 0;
		pacMan.m_fPillTimeLeft = 60;
	}


	//-----------------------------------------------------------------------------------
	CPacMan::CPacMan()
		: m_nLives(3), m_nScores(0), m_fMovingDelta(10.0f)
	{
		m_eBaseType = ePacManBaseType;
		m_eType = ePacMan;
		SPacManHelper::StopPill(*this);
	}

	//-----------------------------------------------------------------------------------
	void CPacMan::update (float dt)
	{
		// Движение ПакМэна
		math::Vec2f vMovingDelta;
		switch (m_eDirection)
		{
		case eUp:		vMovingDelta = math::Vec2f (0, m_fMovingDelta); break;
		case eRight:	vMovingDelta = math::Vec2f (m_fMovingDelta, 0); break;
		case eDown:		vMovingDelta = math::Vec2f (0, -m_fMovingDelta); break;
		case eLeft:		vMovingDelta = math::Vec2f (-m_fMovingDelta, 0); break;
		}

		math::Vec2f vNewPos = m_vPos + vMovingDelta*dt;
		if (g_Scene.isMoveable (vNewPos))
		{
			// Поедание бонуса
			eatBonus (g_Scene.getObject (eBonus, vNewPos));
			// Движение ПакМэна
			m_vPos = vNewPos;
		}

		if (m_isPillActive)
		{
			// Поедание призрака
			eatGhost (g_Scene.getObject (eEnemy, m_vPos));

			m_fPillTimeLeft -= dt;
			if (m_fPillTimeLeft <= 0)
				SPacManHelper::StopPill (*this);
		}
	}

	//-----------------------------------------------------------------------------------
	void CPacMan::eatBonus (CBaseBonus* pBonus)
	{
		if (!pBonus) return;

		m_nScores += pBonus->getScores();
		
		if (pBonus->getType() == ePowerPill)
			SPacManHelper::StartPill (*this);

		// TODO: Отправить сообщение onKillObject
		// TODO: Убить объект
	}

	//-----------------------------------------------------------------------------------
	void CPacMan::eatGhost (CGhost* pGhost)
	{
		if (!pGhost) return;

		m_nEatenPerPill++;
		m_nScores += 100 * pow (2, m_nEatenPerPill);

		// TODO: Отправить сообщение onKillObject
		// TODO: Убить объект
	}

}