#include <rgde/engine.h>

#include <boost/scoped_ptr.hpp>

class SceneHelper : public render::rendererable, public math::frame
{
public:
	typedef render::Generator<vertex::PositionNormalColored>::PGeometry geometry_type;

	SceneHelper(geometry_type pGeometry);
	void render();

private:
	geometry_type m_pGeometry;
};

SceneHelper::SceneHelper(geometry_type pGeometry)
{
	m_pGeometry = pGeometry;
	m_renderInfo.frame = this;
	m_renderInfo.render_func = boost::bind( &SceneHelper::render, this );
}

void SceneHelper::render()
{
	render::render_device::get().draw_wired_floor(100.0f, 20, math::Color(150, 150, 150, 255));
	render::render_device::get().draw_wired_floor(100.0f, 2, math::Color(60, 60, 60, 255));

	m_pGeometry->render(render::PrimTypeTriangleList, (int) (m_pGeometry->getIndexNum()/3));
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

            Input::add_command(L"Quit");
            Input::add_command(L"Horz");
            Input::add_command(L"Vert");
            Input::get_device(types::Keyboard)->get_control(types::KeyEscape)->bind(L"Quit");
            Input::get_device(types::Mouse   )->get_control(types::AxisX    )->bind(L"Horz");
			Input::get_device(types::Mouse   )->get_control(types::AxisY    )->bind(L"Vert");
            m_esc  .attach(L"Quit");
            m_mouse_x.attach(L"Horz");
            m_mouse_y.attach(L"Vert");
            m_esc   += boost::bind(&SampleApp::onEsc,   this);
            m_mouse_x += boost::bind(&SampleApp::onXAxis, this, _1);
            m_mouse_y += boost::bind(&SampleApp::onYAxis, this, _1);
        }

		m_target_camera->activate();

		//render::TheLightManager::get().setAmbientColor(math::Color(50, 50, 50, 0));
	
		//render::CPointLight *pLight = new render::CPointLight("point1");
		//scene::TheScene::get().get_root()->add(pLight);
		//pLight->set_position(math::vec3f(0,0,5));

		//pLight->setDiffuse(math::Color(230, 170, 170, 0));
		//pLight->setRange(100);

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

	render::Generator<vertex::PositionNormalColored>::PGeometry m_pGeometry;

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