//RGDE
#include "engine.h"


class MySuperPuperClass : public render::IRendererable, public math::CFrame
{
public:

	MySuperPuperClass(render::CGenerator<vertex::PositionNormalColored>::PGeometry pGeometry)
	{
		m_pGeometry = pGeometry;
		m_renderInfo.pFrame = this;
		m_renderInfo.pRenderFunc = boost::bind( &MySuperPuperClass::render, this );
	}

	void render()
	{
		m_pGeometry->render(render::PrimTypeTriangleList, (int) (m_pGeometry->getIndexNum()/3));
	}

private:
	render::CGenerator<vertex::PositionNormalColored>::PGeometry m_pGeometry;
};


class HelloMessage
{
public:
	HelloMessage() :  m_spApp(core::IApplication::Create())
	{
		m_spApp->addTask(core::PTask(new core::CInputTask(*m_spApp,0,false)));
		m_spApp->addTask(core::PTask(new core::CRenderTask(*m_spApp,1)));
		m_spApp->addTask(core::PTask(new core::CGameTask(*m_spApp,2)));

		math::Vec3f vEyePt( 0.0f, 0, -30 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera = render::CRenderCamera::Create();
		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		render::TheCameraManager::Get().addCamera(m_camera);


		m_spTargetCamera = math::CTargetCamera::Create(m_camera);
		m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);

        {
            using namespace input;

            CInput::addCommand(L"Quit");
            CInput::addCommand(L"Horz");
            CInput::addCommand(L"Vert");
            CInput::getDevice(Keyboard)->getControl(KeyEscape)->bind(L"Quit");
            CInput::getDevice(Mouse   )->getControl(AxisX    )->bind(L"Horz");
            CInput::getDevice(Mouse   )->getControl(AxisY    )->bind(L"Vert");
            m_cEsc  .attach(L"Quit");
            m_cXAxis.attach(L"Horz");
            m_cYAxis.attach(L"Vert");
            m_cEsc   += boost::bind(&HelloMessage::onEsc,   this);
            m_cXAxis += boost::bind(&HelloMessage::onXAxis, this, _1);
            m_cYAxis += boost::bind(&HelloMessage::onYAxis, this, _1);
        }

		m_spTargetCamera->activate();

		//render::TheLightManager::Get().setAmbientColor(math::Color(50, 50, 50, 0));
	
		//render::CPointLight *pLight = new render::CPointLight("point1");
		//scene::TheScene::Get().getRootFrame()->addChild(pLight);
		//pLight->setPosition(math::Vec3f(0,0,5));

		//pLight->setDiffuse(math::Color(230, 170, 170, 0));
		//pLight->setRange(100);

		m_pGeometry = render::CGenerator<vertex::PositionNormalColored>::CreateBox();

		m_pMySuper = boost::intrusive_ptr<MySuperPuperClass>(new MySuperPuperClass(m_pGeometry));
		scene::TheScene::Get().getRootFrame()->addChild(m_pMySuper);
		m_pMySuper->setPosition(math::Vec3f(0,0,-5));

		m_spApp->Run();
	}
protected:
	//выход из программы
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
	math::PCamera       m_camera;            //указатель на камеру

	::render::CGenerator<vertex::PositionNormalColored>::PGeometry m_pGeometry;

	boost::intrusive_ptr<MySuperPuperClass> m_pMySuper;

	//данные для ввода
	input::CKeyDown      m_cEsc;
	input::CRelativeAxis m_cXAxis;
	input::CRelativeAxis m_cYAxis;

	//данные для камеры
	math::PTargetCamera  m_spTargetCamera; //контроллер камеры "нацеленная камера"

	std::auto_ptr<core::IApplication> m_spApp;
};

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	HelloMessage r;
}