//RGDE
#include <rgde/engine.h>

//WIN
//#include <windows.h> //дл€ HWND и SendMessage
//#include <sstream> 

class AnimationTest : public game::dynamic_object
{
public:
	AnimationTest() :  m_spApp(core::IApplication::Create(L"Animation Test", 640, 480, 32, 85, false)),
					   m_camera(render::CRenderCamera::Create())
	{
		m_spApp->addTask(core::PTask(new core::CRenderTask(1)));
		m_spApp->addTask(core::PTask(new core::CInputTask(0)));
		m_spApp->addTask(core::PTask(new core::CGameTask(2)));

		m_spFont = render::IFont::Create(20, L"Arial", render::IFont::Heavy);

		m_pMesh = render::PMesh(new render::CMesh);
		m_pMesh->load( "media\\meshes\\Box01.xml" );

		m_pTexture = ::render::ITexture::Create("tiger.bmp");

		m_spFrame = new math::CFrame();
		m_spFrame->addChild( m_pMesh );

		TiXmlDocument xml( "media\\1111.XML" ) ;

		if ( !xml.LoadFile() )
			return ;
		
		m_controller.load( xml.FirstChild( "model" )->FirstChild( "node" )->FirstChild( "node" ) );
		m_controller.atachToFrame( m_spFrame );
		m_controller.start();
		m_controller.setRate( 1.0f );
		m_controller.setLooped( true );

		math::Vec3f vEyePt( 0.0f, 40, -400 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		m_cTargetCamera.setCamera( m_camera );
		m_cTargetCamera.setPosition(vEyePt,vLookatPt,vUpVec);

		//инициализаци€ ввода
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		m_cEsc.addHandler(this,&AnimationTest::onEsc);
		m_cYAxis.addHandler(this,&AnimationTest::onYAxis);
		m_cXAxis.addHandler(this,&AnimationTest::onXAxis);

		m_spApp->Run();

		m_controller.atachToFrame(0);
	}

protected:

	virtual void update(float dt)
	{
		m_controller.update( dt );
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
	std::auto_ptr<core::IApplication> m_spApp;

	::render::PFont				m_spFont;
	math::PCamera				m_camera;            //указатель на камеру
	::render::PTexture			m_pTexture;

	math::CFrameAnimationController	m_controller;
	math::PFrame				m_spFrame;
	::render::PMesh				m_pMesh;

	//данные дл€ ввода
	input::Button       m_cEsc;
	input::RelativeAxis m_cXAxis;
	input::RelativeAxis m_cYAxis;

	//данные дл€ камеры
	math::CTargetCamera      m_cTargetCamera;      //контроллер камеры "нацеленна€ камера"
};


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	AnimationTest r;
}