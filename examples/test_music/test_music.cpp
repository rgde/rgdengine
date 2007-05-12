//RGDE
#include "engine.h"

//WIN
//#include <windows.h> //для HWND и SendMessage

//
//class HelloMessage
//{
//public:
//	HelloMessage() : m_camera(render::CRenderCamera::Create())
//	{
//		m_pFont = render::IFont::Create(20, L"Arial", render::IFont::Heavy);
//
//		math::Vec3f vEyePt( 0.0f, 0, -50 );
//		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
//		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );
//
//		m_camera->lookAt(vEyePt, vLookatPt, vUpVec);
//		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
//
//		m_camera->activate();
//	}
//
//protected:
//	::render::PFont	m_pFont;
//	math::PCamera	m_camera;
//};
//
//class CSound3dTest : public game::IDynamicObject
//{
//public:
//	CSound3dTest()
//	{
//		CoInitialize(0);
//
//		m_pSound3d = sound::CSound3d::Create (L"helicopter.wav");
//		m_pSound3d->setPos (math::Vec3f (0, 10, 0));
//		m_pSound3d->setLoop (true);
//		m_pSound3d->play();
//
//		sound::Sound3dManager::Get().setPos (math::Vec3f (0, 0, 0));
//	}
//
//	~CSound3dTest()
//	{
//		CoUninitialize();
//	}
//
//	void update (float dt)
//	{
//		static float fAngle = 0.0f;
//		fAngle += math::Math::PI / 100;
//
//		m_pSound3d->setPos (math::Vec3f ( 100.0f + 100.0f*cosf(fAngle), 0, 100.0f*sinf(fAngle) ));
//
//		sound::Sound3dManager::Get().update (dt);
//	}
//
//protected:
//	sound::Sound3dPtr	m_pSound3d;							///< 3d Sound
//};
//
//class CMusicTest : public game::IDynamicObject
//{
//public:
//	CMusicTest()
//	{
//		CoInitialize(0);
//		m_pMusic1 = sound::CMusic::Create (L"music/music1.ogg");
//		m_pMusic2 = sound::CMusic::Create (L"music/music2.ogg");
//	}
//
//	void update (float dt)
//	{
//		static bool bAmbient = false;
//		static bool bAction = false;
//		static float fStartTime = static_cast<float> (GetTickCount()) / 1000.0f;
//		float curTime = static_cast<float> (GetTickCount())/1000.0f - fStartTime;
//
//		// Вначале играет спокойная музыка
//		if ( !bAmbient && (curTime < 3) )
//		{
//			m_pMusic1->play ();
//			bAmbient = true;
//		}
//
//		// Затем переход на экшн
//		if ( !bAction && (curTime > 3) )
//		{
//			m_pMusic2->play (5);
//			bAction = true;
//		}
//
//		sound::MusicManager::Get().update (dt);
//	}
//
//protected:
//	sound::MusicPtr		m_pMusic1;						///< Music 1
//	sound::MusicPtr		m_pMusic2;						///< Music 2
//};
//
//class SimpleInput
//{
//public:
//	SimpleInput()
//	{
//		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
//		m_cEsc.addHandler(this,&SimpleInput::onEsc);
//	}
//
//private:
//	input::CButtonCommand m_cEsc;
//	void onEsc (const input::CButtonEvent&)
//	{
//		core::IApplication::Get()->close();
//	}
//};

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
//	std::auto_ptr<core::IApplication> spApp(core::IApplication::Create(L"Test Window", 640, 480, 32, 85, false));
//
//	new core::RenderTask(0);
//	new core::CGameTask (2);
//	new core::InputTask (1);
//
//	HelloMessage r;
//	SimpleInput i;
//	CSound3dTest test;
////	CMusicTest test;
//
//
//	spApp->Run();
}