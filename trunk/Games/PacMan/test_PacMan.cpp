/**--------------------------------------------------------------------------------------

									RGDE Engine

----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru

Created:	13 june 2005
---------------------------------------------------------------------------------------*/
#include "precomp.h"


#include "input/IL.h"

// game
#include "game/DynamicObject.h"

#include "test_PacMan.h"
#include "Scene.h"

//STL
#include <memory>

//WIN
#include <windows.h> //для HWND и SendMessage


namespace test_PacMan
{

	CPacManTest* g_pPacManTest;


	//-----------------------------------------------------------------------------------
	CPacManTest::CPacManTest()
		: render::IRendererable (10)
	{
		m_spFont = render::IFont::Create(20, L"Arial", render::IFont::FWHeavy);

		math::Vec3f vEyePt(0, 40, 0);
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 00.0f, 1.0f, 0.0f );

		m_camera.lookAt(vEyePt, vLookatPt, vUpVec);
		m_camera.setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);

		m_camera.activate();


		test_PacMan::g_pPacManTest = this;

		m_pSpriteManager = new render::CSpriteManager();

		g_Scene.createLevel();
	}

	//-----------------------------------------------------------------------------------
	void CPacManTest::render()
	{
		m_camera.activate();

		{
			math::Rect rect(30,30, 400, 400);
			m_spFont->renderText(L"RGDE - PacMan test", rect, 0xffffffff, true);
		}


		m_pSpriteManager->render();
	}

}



// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	std::auto_ptr<core::IApplication> spApp(core::IApplication::Create(L"Test_PacMan Example", 640, 480, 32, 85, false));

	new core::CRenderTask(2);
	new core::CGameTask(1);
	new core::CInputTask(0);

	new test_PacMan::CPacManTest();

	spApp->Run();

	delete test_PacMan::g_pPacManTest;
}