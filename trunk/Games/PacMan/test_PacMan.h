/**--------------------------------------------------------------------------------------

									RGDE Engine

----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru

Created:	13 june 2005
---------------------------------------------------------------------------------------*/
#pragma once

//RGDE
#include "core/application.h"
#include "core/coreRenderDevice.h"
#include "core/RenderTask.h"
#include "core/InputTask.h"
#include "core/GameTask.h"

#include "math/mathTypes3d.h"
#include "math/mathCamera.h"
#include "math/mathInterpolyator.h"

#include "render/renderManager.h"
#include "render/renderFont.h"
#include "render/renderMesh.h"
#include "render/renderTexture.h"
#include "render/renderDevice.h"
#include "render/renderEffect.h"
#include "render/renderSprites.h"



namespace test_PacMan
{

	//-----------------------------------------------------------------------------------
	class CPacManTest : public render::IRendererable
	{
	public:
		CPacManTest();

		void render();

		inline render::CSpriteManager* getSpriteManager() { return m_pSpriteManager; }

	protected:
		::render::PFont			m_spFont;
		math::CBaseCamera		m_camera;
		render::CSpriteManager*	m_pSpriteManager;
	};

	extern CPacManTest* g_pPacManTest;

	/*
	//-----------------------------------------------------------------------------------
	// Класс точки
	class CDot : public CBaseBonus
	{
	public:
		CDot()
		{
			m_eBaseType = eBonus;
			m_eType = eDot;
		}
	};


	//-----------------------------------------------------------------------------------
	// Класс пилюли
	class CPowerPill : public CBaseBonus
	{
	public:
		CPowerPill()
		{
			m_eBaseType = eBonus;
			m_eType = ePowerPill;
		}
	};


	//-----------------------------------------------------------------------------------
	// Класс стены
	class CWall : public CBaseObject
	{
	public:
		CWall()
		{
			m_eBaseType = eObstacle;
			m_eType = eWall;
		}
	};
	*/
}