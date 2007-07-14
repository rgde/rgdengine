//RGDE
#include "engine.h"


class HelloMessage : public game::IDynamicObject
{
public:
	HelloMessage() :  m_spApp(core::IApplication::Create(L"Test Loader", 800, 600, 32, 85, false))
	{
		m_spApp->addTask(core::PTask(new core::CRenderTask(1)));
		m_spApp->addTask(core::PTask(new core::CInputTask(0)));
		m_spApp->addTask(core::PTask(new core::CGameTask(2)));

		m_spFont = render::IFont::Create(20, L"Arial", render::IFont::Heavy);

		m_geometry = render::CModel::Create("bomber.skel");
		//m_geometry = render::CModel::Create(L"box_anim.xml");
		m_geometry->setLooped( true );
		m_geometry->play();
		
		scene::TheScene::Get().getRootFrame()->addChild( m_geometry);
		//m_pTexture = ::render::CTexture::Create(L"tiger.bmp");

		math::Vec3f vEyePt( 0.0f, 40, -400 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera = render::CRenderCamera::Create();
		render::TheCameraManager::Get().addCamera(m_camera);
		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		m_camera->activate();

		m_cTargetCamera.setCamera( m_camera );
		m_cTargetCamera.setPosition(vEyePt,vLookatPt,vUpVec);
		m_cTargetCamera.activate();

		//инициализаци€ ввода
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		m_cEsc.addHandler(this,&HelloMessage::onEsc);
		m_cYAxis.addHandler(this,&HelloMessage::onYAxis);
		m_cXAxis.addHandler(this,&HelloMessage::onXAxis);

		m_spApp->Run();
	}

	~HelloMessage()
	{
		//delete m_pTexture;
	}

protected:
	virtual void update(float dt)
	{
//		m_geometry->update( dt );
	}
	
	//выход из программы
	void onEsc(const input::CButtonEvent&)
	{
		core::IApplication::Get()->close();
	}

	//ось X
	void onXAxis(const input::CRelativeAxisEvent &event)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

		m_cTargetCamera.rotateLeft(-angle);
	}

	//ось Y
	void onYAxis(const input::CRelativeAxisEvent &event)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

		m_cTargetCamera.rotateUp(angle);
	}

protected:
	render::PFont		m_spFont;
	math::PCamera       m_camera;            //указатель на камеру
	render::PTexture  m_pTexture;
	
	render::PModel    m_geometry;

	//данные дл€ ввода
	input::CButtonCommand       m_cEsc;
	input::CRelativeAxisCommand m_cXAxis;
	input::CRelativeAxisCommand m_cYAxis;

	//данные дл€ камеры
	math::CTargetCamera      m_cTargetCamera;      //контроллер камеры "нацеленна€ камера"

	std::auto_ptr<core::IApplication> m_spApp;
};

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	HelloMessage r;
}