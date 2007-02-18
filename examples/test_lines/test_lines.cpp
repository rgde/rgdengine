//RGDE
#include "engine.h"
#include "TriangleManager.h"

#include <boost/scoped_ptr.hpp>

class HelloMessage :  public game::IDynamicObject
{
public:
	HelloMessage() :  
	  spApp(core::IApplication::Create(L"Test_Lines Example", 640, 480, 32, 85, false)),
	  m_TrianglesManager(9)
	{
		spApp->addTask(core::PTask(new core::CRenderTask(*spApp, 1)));
		spApp->addTask(core::PTask(new core::CInputTask(*spApp,0)));
		spApp->addTask(core::PTask(new core::CGameTask(*spApp,2)));

		m_pFont = render::IFont::Create(20, L"Arial", render::IFont::Heavy);

		m_camera = render::CRenderCamera::Create();

		math::Vec3f vEyePt( 0.0f, 0, -50 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera->lookAt(vEyePt, vLookatPt, vUpVec);
		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_cEsc.addHandler(this,&HelloMessage::onEsc);

		render::TheCameraManager::Get().addCamera(m_camera);
		
		spApp->Run();
	}

protected:
	void update (float dt)
	{
		render::TheDevice::Get().showFPS(m_pFont);
		m_TrianglesManager.update();
		m_TrianglesManager.render();
	}

	void onEsc (const input::CButtonEvent&)
	{
		core::IApplication::Get()->close();
	}

protected:
	render::PFont					m_pFont;
	render::PCamera			m_camera;
	lines_test::CTriangleManager	m_TrianglesManager;
	boost::scoped_ptr<core::IApplication> spApp;
	input::CButtonCommand			m_cEsc;
};

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	HelloMessage r;
}