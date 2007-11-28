#include <rgde/engine.h>


class CTestShader : public game::IDynamicObject
{
public:
	CTestShader() : m_pApp(core::IApplication::Create(L"Effect test", 800, 600, false))
	{
		m_pApp->addTask(core::PTask(new core::RenderTask(*m_pApp, 2)));
		m_pApp->addTask(core::PTask(new core::CGameTask(*m_pApp, 1)));
		m_pApp->addTask(core::PTask(new core::InputTask(*m_pApp, 0, false)));


		m_pRenderCamera = render::CRenderCamera::Create();


		{
			math::Vec3f rc_vEyePt(0.0f, 4.0f, 75.0f);
			math::Vec3f rc_vLookatPt(0.0f, 0.75f, 0.0f);
			math::Vec3f rc_vUpVec(0.0f, 1.0f, 0.0f);

			m_pRenderCamera->lookAt(rc_vEyePt, rc_vLookatPt, rc_vUpVec);
			m_pRenderCamera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		}

		render::TheCameraManager::Get().addCamera(m_pRenderCamera);

		m_camera = render::CRenderCamera::Create();
		render::TheCameraManager::Get().addCamera(m_camera);
		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);

		m_pTargetCamera = math::CTargetCamera::Create(m_camera);
		
		{
			math::Vec3f vEyePt( 0.0f, 0, -300 );
			math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
			math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

			m_pTargetCamera->setPosition(vEyePt, vLookatPt, vUpVec);
			m_pTargetCamera->activate();
		}

		render::TheRenderManager::Get().enableVolumes( true );		
		
		{	// настроить освещение	
			//render::TheLightManager::Get().setAmbientColor(math::Color(50, 50, 50, 0));
			
			// источники света рисуются сейчас чайниками :) временно пока не будет более 
			// подходящей модельки
			/*render::CPointLight *pLight = new render::CPointLight("point1", m_spLightFrame);
			pLight->setDiffuse(math::Color(130, 100, 100, 0));
			pLight->setSpecular(math::Color(0, 0, 5, 0));
			pLight->setAmbient(math::Color(130, 100, 100, 0));
			pLight->setRange(1000);*/

			render::CDirectionalLight *pDirLight = new render::CDirectionalLight("dir1");
			pDirLight->setDiffuse(math::Color(200, 200, 200, 0));
			pDirLight->setSpecular(math::Color(50, 50, 50, 0));
			pDirLight->setAmbient(math::Color(100, 100, 100, 0));
			pDirLight->setEnabled(true);

			scene::TheScene::Get().getRootFrame()->addChild(pDirLight);
			pDirLight->setPosition(math::Vec3f(150,0,0));
			pDirLight->setScale(math::Vec3f(0.05f, 0.05f, 0.05f));
		}

		m_cSpace.attach(L"Space");
		m_cEsc  .attach(L"Escape");

		input::Input::getDevice(input::types::Keyboard)->getControl(input::types::KeySpace )->bind(L"Space");
		input::Input::getDevice(input::types::Keyboard)->getControl(input::types::KeyEscape)->bind(L"Escape");

		m_cSpace += boost::bind(&CTestShader::onSpace,   this);
		m_cEsc   += boost::bind(&CTestShader::onEsc,     this);


		m_cXAxis.attach(L"Horz");
		m_cYAxis.attach(L"Vert");

		input::Input::getDevice(input::types::Mouse)->getControl(input::types::AxisX )->bind(L"Horz");
		input::Input::getDevice(input::types::Mouse)->getControl(input::types::AxisY)->bind(L"Vert");

		m_cXAxis += boost::bind(&CTestShader::onXAxis, this, _1);
		m_cYAxis += boost::bind(&CTestShader::onYAxis, this, _1);

		m_spFont = ::render::IFont::Create(20, L"Arial", render::IFont::Heavy);


		//m_pGeometry = render::CGenerator<vertex::MeshVertex>::CreateCylinder(16, 20.0f, 20.0f, math::Vec3f(0.0f, 0.0f, 0.0f));
		// настраиваем рендер инфо структуру что бы правильно рендерить цилиндр

		//m_spGeomFrame = new math::CFrame();
		//scene::TheScene::Get().getRootFrame()->addChild(m_spGeomFrame);
		//m_spGeomFrame->setPosition(math::Vec3f(0,-50,0));

		m_Mode = Mode1;
		m_spEffect1 = render::IEffect::Create("graphics/shaders/default.fx");
		//m_spEffect2 = render::IEffect::Create(L"graphics/shaders/TestShader.fx");
		m_spEffect2 = render::IEffect::Create("graphics/shaders/default.fx");
		
		//m_renderInfo.pRenderFunc = boost::bind( &CTestShader::render, this );
		

		//m_spModel = render::CModel::Create("teapot.xml");
		m_spModel = render::CModel::Create("teapot");
		m_spModel->setPosition(math::Point3f(-55,0,0));
		scene::TheScene::Get().getRootFrame()->addChild(m_spModel);
		
		m_spMesh = render::PMesh(new render::CMesh);
		m_spMesh->load("media//meshes//teapot01.xml");
		render::PMaterial pmat = render::CMaterial::Create("media//materials//07 - default.xml");
		m_spMesh->getMaterials().push_back(pmat);
		m_spMesh->setEffect(m_spEffect1);
		m_spMesh->setPosition(math::Point3f(5,0,0));

		scene::TheScene::Get().getRootFrame()->addChild(m_spMesh);

		m_pApp->Run();		
	}

	void render()
	{
		//m_pGeometry->render(render::PrimTypeTriangleList, (int) (m_pGeometry->getIndexNum()/3));
	}

protected:
	virtual void update(float dt)
	{
		m_spFont->render(L"\"SPACE\" - переключение эффекта", math::Rect(20, 20, 800, 100));
		switch (m_Mode)
		{
		case Mode1:
			m_spFont->render(L"default.fx", math::Rect(40, 40, 100, 100));
			m_spMesh->setEffect(m_spEffect1);
			break;
		case Mode2:
			m_spFont->render(L"TestShader.fx", math::Rect(40, 40, 150, 100));
			m_spMesh->setEffect(m_spEffect2);
			break;
		}

		render::TheDevice::Get().showFPS(m_spFont);
	}

public:
	void onSpace()
	{
		m_Mode = (m_Mode == Mode1) ? Mode2 : Mode1;
	}

	void onEsc()
	{
		core::IApplication::Get()->close();
	}

	//ось X
	void onXAxis(int dx)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dx>accel ? dx*fast : dx*slow;

		m_pTargetCamera->rotateLeft(-angle);
	}

	//ось Y
	void onYAxis(int dy)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dy>accel ? dy*fast : dy*slow;

		m_pTargetCamera->rotateUp(angle);
	}

protected:
	input::KeyDown m_cSpace;
	input::KeyDown m_cEsc;

	render::PFont						m_spFont;
	std::auto_ptr<core::IApplication>	m_pApp;
	math::PCamera						m_camera;
	//math::CTargetCamera					m_TargetCamera;
	math::PTargetCamera m_pTargetCamera;
	::render::CGenerator<vertex::MeshVertex>::PGeometry m_pGeometry;

	render::PCamera   m_pRenderCamera;            //указатель на камеру

	render::PModel	m_spModel;
	render::PMesh	m_spMesh;

	math::PFrame m_spLightFrame;
	math::PFrame m_spGeomFrame;

	render::PEffect m_spEffect1;
	render::PEffect m_spEffect2;

	input::RelativeAxis m_cXAxis;
	input::RelativeAxis m_cYAxis;

	enum MODE {Mode1 = 1, Mode2};
	MODE m_Mode;
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	CTestShader ts;
}