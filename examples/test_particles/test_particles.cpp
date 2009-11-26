//RGDE
#include <rgde/engine.h>

#include <rgde/render/particles/main.h>
#include <rgde/render/particles/box_emitter.h>
#include <rgde/render/particles/static_emitter.h>
#include <rgde/render/particles/spherical_emitter.h>
#include <rgde/render/particles/processor.h>
#include <rgde/render/particles/effect.h>



class CParticleTest : public game::dynamic_object
{
public:
	CParticleTest() 
		: m_debug_draw (false)
	{
		m_font = render::font::create(20, L"Arial", render::font::Heavy);

		math::vec3f eye(40, 40, -40);
		math::vec3f look_at( 0.0f, 0.0f, 0.0f );
		math::vec3f up_vec( 00.0f, 1.0f, 0.0f );
	
		initParticles();

		// init input
		init_input();

		// init camera
		m_camera  = render::render_camera::create();
		m_camera->set_projection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		m_target_camera = math::target_camera::create(m_camera);
		render::TheCameraManager::get().add_camera(m_camera);
		m_target_camera->set_position(up_vec, eye,look_at);
		m_target_camera->activate();
	}

	void init_input()
	{
		using namespace input;

		Input::add_command("Quit");
		Input::add_command("LookHoriz");
		Input::add_command("LookVert");
		Input::add_command("Scroll");

		//связываем команды с контролами
		Input::get_control(device::keyboard, KeyEscape)->bind("Quit");
		Input::get_control(device::mouse, AxisX )->bind("LookHoriz");
		Input::get_control(device::mouse, AxisY )->bind("LookVert");
		Input::get_control(device::mouse, AxisWheel)->bind("Scroll");
		

		//биндим хелперы с командами		
		m_key_quit.attach("Quit");
		m_mouse_x.attach("LookHoriz");
		m_mouse_y.attach("LookVert");
		m_mouse_wheel.attach("Scroll");

		//задаем для команд функции-обработчики		
		m_key_quit += boost::bind(&CParticleTest::onQuit, this);
		m_mouse_y += boost::bind(&CParticleTest::onYAxis, this, _1);
		m_mouse_x += boost::bind(&CParticleTest::onXAxis, this, _1);
		m_mouse_wheel += boost::bind(&CParticleTest::onWheelAxis, this, _1);
	}

	~CParticleTest()
	{
		deleteParticles();
	}

public:
	//выход из программы
	void onQuit()
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

	void onWheelAxis(int dw)
	{
		const float slow = .1f;
		m_target_camera->move_back(-dw*slow);
	}

	//-----------------------------------------------------------------------------------
	virtual void update(float dt)
	{
		render::render_device::get().draw_wired_floor(100.0f, 20, math::Color(150, 150, 150, 255));
		render::render_device::get().draw_wired_floor(100.0f, 2, math::Color(60, 60, 60, 255));

		if (m_debug_draw)
			m_effect->debug_draw();
	}
protected:
	//-----------------------------------------------------------------------------------
	void initParticles()
	{
		// Создаём эффект
		m_effect = new particles::effect();

		// create emitters
		for( int i = 0; i < 3; i++ )
		{
			float dist = i*10.0f;
			// create spherical emitter
			particles::spherical_emitter* sph_emitter = new particles::spherical_emitter();
			m_effect->add(sph_emitter);

			particles::processor* proc = new particles::processor();
			proc->set_texture_name( "particles/Shot_Smoke.png" );
			proc->setMaxParticles( 100 );
			proc->setGlobal( false );
			sph_emitter->addProcessor(proc);
			proc->load();

			math::point3f sph_pos(dist, 0, -dist/1.732f);
			sph_emitter->get_transform()->set_position(math::point3f(sph_pos));

			// create cube emitter
			particles::box_emitter* box_emitter = new particles::box_emitter();
			m_effect->add(box_emitter);

			proc = new particles::processor();
			box_emitter->addProcessor(proc);
			proc->set_texture_name( "particles/Shot_Smoke.png" );
			proc->setMaxParticles( 100 );
			proc->setGlobal( false );
			proc->load();

			math::point3f box_pos( -dist, 0, -dist/1.732f);
			box_emitter->get_transform()->set_position(box_pos);
		}
	}

	//-----------------------------------------------------------------------------------
	void deleteParticles()
	{
		delete m_effect;
		particles::static_emitter::clear_cached_data();
		render::effect::clear_all();
	}

protected:
	//данные для ввода
	input::key_up       m_key_quit;
	input::RelativeAxis m_mouse_x;
	input::RelativeAxis m_mouse_y;
	input::key_up       m_esc;
	input::RelativeAxis m_mouse_wheel;

	math::camera_ptr		m_camera;
	math::target_camera_ptr	m_target_camera;      //контроллер камеры "нацеленная камера"

	render::font_ptr		m_font;

	bool				m_debug_draw;			  // Стоит ли проводить в тесте дебажную отрисовку
	particles::effect*	m_effect;				  // Эффект частиц
};

int main()
{
	std::auto_ptr<core::application> app(core::application::create(L"Test Particles Example", 800, 600, false));

	app->add<core::input_task>(0);
	app->add<core::game_task>(1);
	app->add<core::render_task>(2);

	{
		CParticleTest particleTest;

		app->run();
	}

	return 0;
}