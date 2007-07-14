//RGDE
#include "engine.h"

// TODO: Интерполяторы
#include "render/particles/particlesMain.h"
#include "render/particles/particlesBoxEmitter.h"
#include "render/particles/particlesMayaEmitter.h"
#include "render/particles/particlesSphericalEmitter.h"
#include "render/particles/particlesParticlesProcessor.h"
#include "render/particles/particlesEffect.h"


class CParticleTest : public game::IDynamicObject
{
public:
	CParticleTest() : 
		spApp(core::IApplication::Create(L"Test Particles Example", 640, 480, 32, 85, false)),
		m_bDebugDraw (false),
		m_bSaveParticles (false),
		m_bLoadParticles (true)
	{
		spApp->addTask(core::PTask(new core::CRenderTask(2)));
		spApp->addTask(core::PTask(new core::CGameTask(1)));
		spApp->addTask(core::PTask(new core::CInputTask(0)));

		

		m_spFont = render::IFont::Create(20, L"Arial", render::IFont::Heavy);

		math::Vec3f vEyePt(40, 40, -40);
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 00.0f, 1.0f, 0.0f );

		m_pCamera  = render::CRenderCamera::Create();
		m_pCamera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		render::TheCameraManager::Get().addCamera(m_pCamera);


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
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		m_cRightButton.attachToControl(input::Mouse, input::ButtonRight);
		m_cZAxis.attachToControl(input::Mouse, input::AxisWheel);
		m_cEsc.addHandler(this,&CParticleTest::onEsc);
		m_cYAxis.addHandler(this,&CParticleTest::onYAxis);
		m_cXAxis.addHandler(this,&CParticleTest::onXAxis);
		m_cZAxis.addHandler(this,&CParticleTest::onWheelAxis);

		// Инициализация камеры
		m_cTargetCamera.setCamera(m_pCamera);
		m_cTargetCamera.setPosition(vEyePt,vLookatPt,vUpVec);
		m_cTargetCamera.activate();

		spApp->Run();
	}

	~CParticleTest()
	{
		deleteParticles();
	}

public:
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

		if (m_cRightButton)
			m_cTargetCamera.rotateLeft(-angle);
	}

	//ось Y
	void onYAxis(const input::CRelativeAxisEvent &event)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

		if (m_cRightButton)
			m_cTargetCamera.rotateUp(angle);
	}

	/*onWheelAxis*/
	void onWheelAxis(const input::CRelativeAxisEvent &event)
	{
		const float slow = .1f;
		m_cTargetCamera.goBackward(-event.m_nDelta*slow);
	}

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

			particles::IParticlesProcessor* pProcessor = new particles::IParticlesProcessor();
			pProcessor->setTextureName( "particles/Shot_Smoke.png" );
			pProcessor->setMaxParticles( 100 );
			pProcessor->setGlobal( false );
			pSphericalEmitter->addProcessor(pProcessor);
			pProcessor->load();


			pSphericalEmitter->getTransform().setPosition(math::Point3f( fDist, 0, -fDist/1.732f));

			// Создаём кубический эмитер
			particles::IBoxEmitter* pBoxEmitter = new particles::IBoxEmitter();
			m_pEffect->addEmitter(pBoxEmitter);

			pProcessor = new particles::IParticlesProcessor();
			pBoxEmitter->addProcessor(pProcessor);
			pProcessor->setTextureName( "particles/Shot_Smoke.png" );
			pProcessor->setMaxParticles( 100 );
			pProcessor->setGlobal( false );
			pProcessor->load();

			pBoxEmitter->getTransform().setPosition( math::Point3f( -fDist, 0, -fDist/1.732f) );

			// Создаём майя эмитер
			particles::IMayaEmitter* pMayaEmitter = new particles::IMayaEmitter("particles/cannonShot_smoke.prt", "particles/shot_smoke.png");
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
		particles::IMayaEmitter::ClearCachedData();
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
	std::auto_ptr<core::IApplication> spApp;

	//данные для ввода
	input::CButtonCommand       m_cEsc;
	input::CButtonCommand		m_cRightButton;
	input::CRelativeAxisCommand m_cXAxis;
	input::CRelativeAxisCommand m_cYAxis;
	input::CRelativeAxisCommand m_cZAxis;

	math::PCamera				m_pCamera;
	math::CTargetCamera			m_cTargetCamera;      //контроллер камеры "нацеленная камера"


	::render::PFont		m_spFont;

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