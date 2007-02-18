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

	class CGhost;

	//-----------------------------------------------------------------------------------
	// Класс ПакМэна
	class CPacMan : public CBaseObject
	{
		friend struct SPacManHelper;

	public:
		enum eDirection { eUp, eRight, eDown, eLeft	};

		CPacMan(); 

		void update (float dt);

		void eatBonus (CBaseBonus* pBonus);
		void eatGhost (CGhost* pGhost);


	private:
		unsigned	m_nLives;						// Число жизней
		unsigned	m_nScores;						// Число набранных очков
		eDirection	m_eDirection;					// Направление взгляда ПакМэна
		float		m_fMovingDelta;					// Приращение к позиции ПакМэна при движении


		// Переменные, связанные с пилюлей
		bool		m_isPillActive;					// Действует ли пилюля
		unsigned	m_nEatenPerPill;				/* Число монстров, съеденных за время действия пилюли
													(влияет на очки) */
		float		m_fPillTimeLeft;				// Время, которое ещё будет действовать пилюля
	};

}