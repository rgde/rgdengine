//RGDE
#include <rgde/engine.h>

//WIN
//#include <windows.h> //дл€ HWND и SendMessage
//#include <sstream> 

class AnimationTest : public game::dynamic_object
{
public:
	AnimationTest() :  m_spApp(core::application::create(L"Animation Test", 640, 480, false)),
					   m_camera(render::render_camera::create())
	{
		m_spApp->add(core::task_ptr(new core::render_task(*m_spApp, 1)));
		m_spApp->add(core::task_ptr(new core::input_task(*m_spApp, 0)));
		m_spApp->add(core::task_ptr(new core::game_task(*m_spApp, 2)));

		m_spFont = render::IFont::create(20, L"Arial", render::IFont::Heavy);

		m_pMesh = render::mesh_ptr(new render::mesh);
		m_pMesh->load( "media\\meshes\\Box01.xml" );

		m_pTexture = render::texture::create("tiger.bmp");

		m_spFrame = new math::frame();
		m_spFrame->add( m_pMesh );

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

		m_spTargetCamera = math::target_camera::create(m_camera);
		m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);

		{//инициализаци€ ввода
			using namespace input;

			Input::get_device(types::Mouse   )->get_control(types::AxisX    )->bind(L"Horz");
			Input::get_device(types::Mouse   )->get_control(types::AxisY    )->bind(L"Vert");
			Input::get_device(types::Keyboard)->get_control(types::KeyEscape)->bind(L"Quit");

			m_cEsc  .attach(L"Quit");
			m_cXAxis.attach(L"Horz");
			m_cYAxis.attach(L"Vert");


			m_cEsc += boost::bind(&AnimationTest::onEsc, this);
			m_cYAxis += boost::bind(&AnimationTest::onYAxis, this, _1);
			m_cXAxis += boost::bind(&AnimationTest::onXAxis, this, _1);
		}


		m_spApp->run();

		m_controller.atachToFrame(0);
	}

protected:

	virtual void update(float dt)
	{
		m_controller.update( dt );
	}

	//выход из программы
	void onEsc()
	{
		core::application::get()->close();
	}

	//ось X
	void onXAxis(int dx)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dx>accel ? dx*fast : dx*slow;

		m_spTargetCamera->rotateLeft(-angle);
	}

	//ось Y
	void onYAxis(int dy)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dy>accel ? dy*fast : dy*slow;

		m_spTargetCamera->rotateUp(angle);
	}
protected:
	std::auto_ptr<core::application> m_spApp;

	render::font_ptr			m_spFont;
	math::camera_ptr			m_camera;            //указатель на камеру
	render::texture_ptr			m_pTexture;

	math::frame_anim_controller	m_controller;
	math::frame_ptr				m_spFrame;
	render::mesh_ptr				m_pMesh;

	//данные дл€ ввода
	input::Button       m_cEsc;
	input::RelativeAxis m_cXAxis;
	input::RelativeAxis m_cYAxis;

	//данные дл€ камеры
	math::target_camera_ptr      m_spTargetCamera;      //контроллер камеры "нацеленна€ камера"
};


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	AnimationTest r;
}