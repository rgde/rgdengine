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
		: spApp(core::application::create(L"Test Particles Example", 640, 480, false))
		, m_bDebugDraw (false)
		, m_bSaveParticles (false)
		, m_bLoadParticles (false)
	{
		spApp->add<core::render_task>(2)
			  .add<core::game_task>(1)
			  .add<core::input_task>(0, false);

		m_spFont = render::base_font::create(20, L"Arial", render::base_font::Heavy);

		math::Vec3f vEyePt(40, 40, -40);
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 00.0f, 1.0f, 0.0f );

		// Создаём эффект
		m_pEffect = new particles::effect();
		//m_pEffect->setDebugDraw(m_bDebugDraw);

		if (m_bLoadParticles)
			loadParticles();
		else
			initParticles();

		if (m_bSaveParticles)
			saveParticles();

		// Инициализация ввода
		initInput();

		// Инициализация камеры
		m_pCamera  = render::render_camera::create();
		m_pCamera->set_projection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		m_cTargetCamera = math::target_camera::create(m_pCamera);
		render::TheCameraManager::get().add_camera(m_pCamera);
		m_cTargetCamera->set_position(vEyePt,vLookatPt,vUpVec);
		m_cTargetCamera->activate();

		spApp->run();
	}

	void initInput()
	{
		using namespace input;

		Input::add_command(L"Quit");
		Input::add_command(L"Horz");
		Input::add_command(L"Vert");
		Input::add_command(L"Froward");
		Input::add_command(L"Backward");
		Input::add_command(L"CW");
		Input::add_command(L"CCW");

		//m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		//m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		//m_cRightButton.attachToControl(input::Mouse, input::ButtonRight);
		//m_cZAxis.attachToControl(input::Mouse, input::AxisWheel);
		//m_cEsc.addHandler(this,&CParticleTest::onEsc);
		//m_cYAxis.addHandler(this,&CParticleTest::onYAxis);
		//m_cXAxis.addHandler(this,&CParticleTest::onXAxis);
		//m_cZAxis.addHandler(this,&CParticleTest::onWheelAxis);

		//связываем команды с контролами
		Input::get_device(types::Keyboard)->get_control(types::KeyEscape)->bind(L"Quit");
		Input::get_device(types::Keyboard)->get_control(types::KeyW     )->bind(L"Froward");
		Input::get_device(types::Keyboard)->get_control(types::KeyS     )->bind(L"Backward");
		Input::get_device(types::Keyboard)->get_control(types::KeyE     )->bind(L"CW");
		Input::get_device(types::Keyboard)->get_control(types::KeyQ     )->bind(L"CCW");
		Input::get_device(types::Mouse   )->get_control(types::AxisX    )->bind(L"Horz");
		Input::get_device(types::Mouse   )->get_control(types::AxisY    )->bind(L"Vert");

		//биндим хелперы с командами		
		//m_cR    .attach(L"Reset");
		//m_cW    .attach(L"Froward");
		//m_cS    .attach(L"Backward");
		//m_cE    .attach(L"CW");
		//m_cQ    .attach(L"CCW");
		m_keyupQuit.attach(L"Quit");
		m_cXAxis.attach(L"Horz");
		m_cYAxis.attach(L"Vert");

		//задаем для команд функции-обработчики		
		//m_cEsc   += boost::bind(&TestInterpolator::onEsc,   this);
		//m_cR     += boost::bind(&TestInterpolator::on_reset, this);
		m_keyupQuit += boost::bind(&CParticleTest::onQuit, this);
		m_cYAxis += boost::bind(&CParticleTest::onYAxis, this, _1);
		m_cXAxis += boost::bind(&CParticleTest::onXAxis, this, _1);
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

		//if (m_cRightButton)
			m_cTargetCamera->rotateLeft(-angle);
	}

	//ось Y
	void onYAxis(int dy)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dy>accel ? dy*fast : dy*slow;

		//if (m_cRightButton)
			m_cTargetCamera->rotateUp(angle);
	}

	///*onWheelAxis*/
	void onWheelAxis(int dw)
	{
		const float slow = .1f;
		m_cTargetCamera->goBackward(-dw*slow);
	}

	//-----------------------------------------------------------------------------------
	virtual void update(float dt)
	{
		render::TheDevice::get().showWiredFloorGrid(100.0f, 20, math::Color(150, 150, 150, 255));
		render::TheDevice::get().showWiredFloorGrid(100.0f, 2, math::Color(60, 60, 60, 255));
	}
protected:
	//-----------------------------------------------------------------------------------
	void initParticles()
	{
		// Создаём эмиттеры
		for( int i = 0; i < 3; i++ )
		{
			float fDist = (float)i*10;
			// Создаём сферический эмитер
			particles::spherical_emitter* sph_emitter = new particles::spherical_emitter();
			m_pEffect->add(sph_emitter);

			particles::processor* proc = new particles::processor();
			proc->setTextureName( "particles/Shot_Smoke.png" );
			proc->setMaxParticles( 100 );
			proc->setGlobal( false );
			sph_emitter->addProcessor(proc);
			proc->load();

			sph_emitter->getTransform().set_position(math::Point3f( fDist, 0, -fDist/1.732f));

			// Создаём кубический эмитер
			particles::box_emitter* box_emitter = new particles::box_emitter();
			m_pEffect->add(box_emitter);

			proc = new particles::processor();
			box_emitter->addProcessor(proc);
			proc->setTextureName( "particles/Shot_Smoke.png" );
			proc->setMaxParticles( 100 );
			proc->setGlobal( false );
			proc->load();

			box_emitter->getTransform().set_position( math::Point3f( -fDist, 0, -fDist/1.732f) );

			// Создаём майя эмитер
			particles::static_emitter* static_emitter = new particles::static_emitter("particles/cannonShot_smoke.prt", "particles/shot_smoke.png");
			m_pEffect->add( static_emitter );

			static_emitter->setCycling(true);
			static_emitter->setVisible(true);

			static_emitter->getTransform().set_position( math::Point3f(0, 0, fDist ) );
		}
	}

	//-----------------------------------------------------------------------------------
	void deleteParticles()
	{
		//delete m_pEffect;
		particles::static_emitter::ClearCachedData();
		render::effect::ClearAll();
	}

	//-----------------------------------------------------------------------------------
	void loadParticles()
	{
		//io::ReadFile in( 
		//	io::helpers::createFullFilePathA(L"Media/particles/particles_serialization.dat").c_str() );
		//if (!in.isOpened())
		//{
			initParticles();
			//saveParticles();
			//deleteParticles();

			// Создаём эффект
			//m_pEffect = new particles::IEffect();
			//m_pEffect->setDebugDraw(true);
		//	in.open( io::helpers::createFullFilePathA(L"Media/particles/particles_serialization.dat").c_str() );
		//}
		//in >> *m_pEffect;
	}

	//-----------------------------------------------------------------------------------
	void saveParticles()
	{
		//io::CWriteFileStream out( 
		//	io::helpers::createFullFilePathA(L"Media/particles/particles_serialization.dat").c_str() );
		//out << *m_pEffect;
	}


protected:
	std::auto_ptr<core::application> spApp;

	//данные для ввода
	input::KeyUp        m_keyupQuit;
	input::RelativeAxis m_cXAxis;
	input::RelativeAxis m_cYAxis;
	input::KeyUp       m_cEsc;
	input::KeyUp		m_cRightButton;
	input::RelativeAxis m_cZAxis;

	math::camera_ptr			m_pCamera;
	math::target_camera_ptr		m_cTargetCamera;      //контроллер камеры "нацеленная камера"

	render::font_ptr		m_spFont;

	bool m_bDebugDraw;				// Стоит ли проводить в тесте дебажную отрисовку
	bool m_bSaveParticles;			// Стоит ли сохранить эффект частиц в файл
	bool m_bLoadParticles;			// Стоит ли создавать эффект частиц программно или грузить из файла
	particles::effect*	m_pEffect;	// Эффект частиц
};

int main()
{
	CParticleTest particleTest;
	return 0;
}