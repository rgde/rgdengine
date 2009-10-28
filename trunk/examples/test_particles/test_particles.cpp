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
		spApp->addTask<core::RenderTask>(2)
			  .addTask<core::CGameTask>(1)
			  .addTask<core::InputTask>(0, false);

		m_spFont = render::IFont::create(20, L"Arial", render::IFont::Heavy);

		math::Vec3f vEyePt(40, 40, -40);
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 00.0f, 1.0f, 0.0f );

		// Создаём эффект
		m_pEffect = new particles::IEffect();
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
		m_pCamera  = render::CRenderCamera::Create();
		m_pCamera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		m_cTargetCamera = math::CTargetCamera::Create(m_pCamera);
		render::TheCameraManager::Get().addCamera(m_pCamera);
		m_cTargetCamera->setPosition(vEyePt,vLookatPt,vUpVec);
		m_cTargetCamera->activate();

		spApp->Run();
	}

	void initInput()
	{
		using namespace input;

		//m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		//m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		//m_cRightButton.attachToControl(input::Mouse, input::ButtonRight);
		//m_cZAxis.attachToControl(input::Mouse, input::AxisWheel);
		//m_cEsc.addHandler(this,&CParticleTest::onEsc);
		//m_cYAxis.addHandler(this,&CParticleTest::onYAxis);
		//m_cXAxis.addHandler(this,&CParticleTest::onXAxis);
		//m_cZAxis.addHandler(this,&CParticleTest::onWheelAxis);

		//связываем команды с контролами
		Input::getDevice(types::Keyboard)->getControl(types::KeyEscape)->bind(L"Quit");
		//Input::getDevice(types::Keyboard)->getControl(types::KeyW     )->bind(L"Froward");
		//Input::getDevice(types::Keyboard)->getControl(types::KeyS     )->bind(L"Backward");
		//Input::getDevice(types::Keyboard)->getControl(types::KeyE     )->bind(L"CW");
		//Input::getDevice(types::Keyboard)->getControl(types::KeyQ     )->bind(L"CCW");
		Input::getDevice(types::Mouse   )->getControl(types::AxisX    )->bind(L"Horz");
		Input::getDevice(types::Mouse   )->getControl(types::AxisY    )->bind(L"Vert");

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
		//m_cR     += boost::bind(&TestInterpolator::onReset, this);
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
		core::application::Get()->close();
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
	//void onWheelAxis(const input::CRelativeAxisEvent &event)
	//{
	//	const float slow = .1f;
	//	m_cTargetCamera.goBackward(-event.m_nDelta*slow);
	//}

	//-----------------------------------------------------------------------------------
	virtual void update(float dt)
	{
		render::TheDevice::Get().showWiredFloorGrid(100.0f, 20, math::Color(150, 150, 150, 255));
		render::TheDevice::Get().showWiredFloorGrid(100.0f, 2, math::Color(60, 60, 60, 255));
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
			particles::ISphericalEmitter* pSphericalEmitter = new particles::ISphericalEmitter();
			m_pEffect->addEmitter(pSphericalEmitter);

			particles::IParticlesProcessor* proc = new particles::IParticlesProcessor();
			proc->setTextureName( "particles/Shot_Smoke.png" );
			proc->setMaxParticles( 100 );
			proc->setGlobal( false );
			pSphericalEmitter->addProcessor(proc);
			proc->load();


			pSphericalEmitter->getTransform().setPosition(math::Point3f( fDist, 0, -fDist/1.732f));

			// Создаём кубический эмитер
			particles::IBoxEmitter* pBoxEmitter = new particles::IBoxEmitter();
			m_pEffect->addEmitter(pBoxEmitter);

			proc = new particles::IParticlesProcessor();
			pBoxEmitter->addProcessor(proc);
			proc->setTextureName( "particles/Shot_Smoke.png" );
			proc->setMaxParticles( 100 );
			proc->setGlobal( false );
			proc->load();

			pBoxEmitter->getTransform().setPosition( math::Point3f( -fDist, 0, -fDist/1.732f) );

			// Создаём майя эмитер
			particles::static_emitter* pMayaEmitter = new particles::static_emitter("particles/cannonShot_smoke.prt", "particles/shot_smoke.png");
			m_pEffect->addEmitter( pMayaEmitter );

			pMayaEmitter->setCycling(true);
			pMayaEmitter->setVisible(true);

			pMayaEmitter->getTransform().setPosition( math::Point3f(0, 0, fDist ) );
		}
	}

	//-----------------------------------------------------------------------------------
	void deleteParticles()
	{
		//delete m_pEffect;
		particles::static_emitter::ClearCachedData();
		render::IEffect::ClearAll();
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
	//input::Button       m_cEsc;
	//input::Button		m_cRightButton;
	//input::RelativeAxis m_cZAxis;

	math::camera_ptr			m_pCamera;
	math::target_camera_ptr		m_cTargetCamera;      //контроллер камеры "нацеленная камера"

	render::font_ptr		m_spFont;

	bool m_bDebugDraw;				// Стоит ли проводить в тесте дебажную отрисовку
	bool m_bSaveParticles;			// Стоит ли сохранить эффект частиц в файл
	bool m_bLoadParticles;			// Стоит ли создавать эффект частиц программно или грузить из файла
	particles::IEffect*	m_pEffect;	// Эффект частиц
};

int main()
{
	CParticleTest particleTest;
	return 0;
}