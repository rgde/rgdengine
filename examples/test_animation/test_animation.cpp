//RGDE
#include <rgde/engine.h>


class AnimationTest : public game::dynamic_object
{
public:
	AnimationTest() 
		: m_camera(render::render_camera::create())
	{
		m_font = render::font::create(20, L"Arial", render::font::Heavy);

		m_mesh = render::mesh_ptr(new render::mesh);
		{
			io::path_add_scoped p("models/test_scene/");
			m_mesh->load( "Box01.xml" );
		}
		
		m_texture = render::texture::create("tiger.bmp");

		m_frame = math::frame::create();
		m_frame->add( m_mesh );

		TiXmlDocument xml( "media\\1111.XML" );

		if ( xml.LoadFile() )
		{
			m_controller.load( xml.FirstChild( "model" )->FirstChild( "node" )->FirstChild( "node" ) );
		}
		
		m_controller.atach( m_frame );
		m_controller.start();
		m_controller.speed( 1.0f );
		m_controller.looped( true );

		math::vec3f eye( 0.0f, 40, -400 );
		math::vec3f lookat( 0.0f, 0.0f, 0.0f );
		math::vec3f up_vec( 0.0f, 1.0f, 0.0f );

		m_camera->projection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);

		m_target_camera = math::target_camera::create(m_camera);
		m_target_camera->position(up_vec,eye,lookat);


		{//input init
			using namespace input;

			// избавить от строковых ID для команд
			Input::add_command("Quit");
			Input::add_command("Horz");
			Input::add_command("Vert");

			Input::get_control(device::mouse, AxisX)->bind("Horz");
			Input::get_control(device::mouse, AxisY)->bind("Vert");
			Input::get_control(device::keyboard, KeyEscape)->bind("Quit");

			m_esc  .attach("Quit");
			m_mouse_x.attach("Horz");
			m_mouse_y.attach("Vert");

			// переделать через систему эвентов
			// все обработчики инпут эвентов по сути получают только число.
			m_esc += boost::bind(&AnimationTest::onEsc, this);
			m_mouse_y += boost::bind(&AnimationTest::onYAxis, this, _1);
			m_mouse_x += boost::bind(&AnimationTest::onXAxis, this, _1);
		}
	}

protected:
	virtual void update(float dt)
	{
		m_controller.update( dt );
	}

	//application exit
	void onEsc()
	{
		core::application::get()->close();
	}

	//mouse X
	void onXAxis(int dx)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dx>accel ? dx*fast : dx*slow;

		m_target_camera->rotate_left(-angle);
	}

	//mouse Y
	void onYAxis(int dy)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dy>accel ? dy*fast : dy*slow;

		m_target_camera->rotate_up(angle);
	}
protected:
	render::font_ptr		m_font;	
	render::texture_ptr		m_texture;

	math::frame_animation	m_controller;
	math::frame_ptr			m_frame;
	render::mesh_ptr		m_mesh;

	//input handlers
	input::Button       m_esc;
	input::RelativeAxis m_mouse_x;
	input::RelativeAxis m_mouse_y;

	//camera:
	math::camera_ptr m_camera;
	//camera controller "targeted camera"
	math::target_camera_ptr      m_target_camera;
};


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	using namespace core;

	typedef std::auto_ptr<application> app_ptr;

	app_ptr app(application::create(L"Animation Test", 640, 480, false));

	app->add<render_task>(0);
	app->add<input_task>(1, false);
	app->add<game_task>(2);

	{
		AnimationTest r;
		app->run();
	}
}