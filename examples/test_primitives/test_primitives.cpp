#include <rgde/engine.h>

#include <boost/scoped_ptr.hpp>

// game has levels
// level has scene
// scene has worlds
// word knows about it's resources
// level knows about it's resources
// etc.

//or not? if we change level - we may not want to change currently rendering scene!
// to be or not to be.... ghm.

class SceneHelper : public render::rendererable, public math::frame
{
public:
	typedef render::Generator<vertex::PositionNormalColored>::geometry_ptr geometry_ptr;

	SceneHelper(geometry_ptr m_geometry);
	void render();

private:
	geometry_ptr m_geometry;
};

SceneHelper::SceneHelper(geometry_ptr geometry)
{
	m_geometry = geometry;
	m_render_info.frame = this;
	m_render_info.render_func = boost::bind( &SceneHelper::render, this );
}

void SceneHelper::render()
{
	render::render_device::get().draw_wired_floor(100.0f, 20, math::Color(150, 150, 150, 255));
	render::render_device::get().draw_wired_floor(100.0f, 2, math::Color(60, 60, 60, 255));

	const int num_primitives = (int)m_geometry->getIndexNum()/3;
	m_geometry->render(render::TriangleList,  num_primitives);
}


class SampleApp
{
public:
	SampleApp()
	{
		math::vec3f eye( 0.0f, 0, -30 );
		math::vec3f look_at( 0.0f, 0.0f, 0.0f );
		math::vec3f up_vec( 0.0f, 1.0f, 0.0f );

		m_camera = render::render_camera::create();
		m_camera->set_projection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		render::TheCameraManager::get().add_camera(m_camera);


		m_target_camera = math::target_camera::create(m_camera);
		m_target_camera->set_position(up_vec,eye,look_at);

        {
            using namespace input;

			m_esc  .attach("Quit");
			m_mouse_x.attach("Horz");
			m_mouse_y.attach("Vert");

			Input::get_control(device::keyboard, KeyEscape)->bind("Quit");
            Input::get_control(device::mouse, AxisX )->bind("Horz");
			Input::get_control(device::mouse, AxisY )->bind("Vert");

            m_esc   += boost::bind(&SampleApp::onEsc,   this);
            m_mouse_x += boost::bind(&SampleApp::onXAxis, this, _1);
            m_mouse_y += boost::bind(&SampleApp::onYAxis, this, _1);
        }

		m_target_camera->activate();

		m_pGeometry = render::Generator<vertex::PositionNormalColored>::CreateBox();

		m_pMySuper = boost::intrusive_ptr<SceneHelper>(new SceneHelper(m_pGeometry));
		scene::TheScene::get().get_root()->add(m_pMySuper);
		m_pMySuper->set_position(math::vec3f(0,0,-5));
	}
protected:

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

		m_target_camera->rotate_left(-angle);
	}

	//ось Y
	void onYAxis(int dy)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dy>accel ? dy*fast : dy*slow;

		m_target_camera->rotate_up(angle);
	}
protected:
	math::camera_ptr       m_camera;

	render::Generator<vertex::PositionNormalColored>::geometry_ptr m_pGeometry;

	boost::intrusive_ptr<SceneHelper> m_pMySuper;

	//данные для ввода
	input::key_down      m_esc;
	input::RelativeAxis m_mouse_x;
	input::RelativeAxis m_mouse_y;

	//данные для камеры
	//контроллер камеры "нацеленная камера"
	math::target_camera_ptr  m_target_camera;
};

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	using namespace core;

	boost::scoped_ptr<application> 
	app(application::create(L"Test_Lines Example", 800, 600, false, false));

	app->add<render_task>(1);
	app->add<input_task>(0);
	app->add<game_task>(2);

	{
		SampleApp r;
		app->run();
	}	
}