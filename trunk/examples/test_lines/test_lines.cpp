//RGDE
#include <rgde/engine.h>
#include "TriangleManager.h"

#include <boost/scoped_ptr.hpp>

class HelloMessage :  public game::dynamic_object
{
public:
	HelloMessage(core::application& app) :  
	  m_app(app),
	  m_TrianglesManager(9)
	{
		m_font = render::font::create(20, L"Arial", render::font::Heavy);

		m_camera = render::render_camera::create();

		math::Vec3f vEyePt( 0.0f, 0, -50 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera->look_at(vEyePt, vLookatPt, vUpVec);
		m_camera->set_projection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);

        {
            using namespace input;

            m_esc.attach(L"Quit");
			Input::get_device(types::Keyboard)->get_control(types::KeyEscape)->bind(L"Quit");
            
            m_esc += boost::bind(&HelloMessage::onEsc, this);
        }

		render::TheCameraManager::get().add_camera(m_camera);		
	}

protected:
	void update (float dt)
	{
		float abs_time = game::game_system::get().get_timer().get_absolute_time();
		render::render_device::get().draw_fps(abs_time, m_font);

		m_TrianglesManager.update();
		m_TrianglesManager.render();
	}

	void onEsc ()
	{
		m_app.close();
	}

protected:
	render::font_ptr                     m_font;
	render::camera_ptr                   m_camera;
	lines_test::triangle_manager         m_TrianglesManager;
	core::application&					 m_app;
	input::key_down                      m_esc;
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
		HelloMessage r(*app.get());
		app->run();
	}
}