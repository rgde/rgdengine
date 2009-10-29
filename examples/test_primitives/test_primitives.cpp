#include <rgde/engine.h>


class SceneHelper : public render::rendererable, public math::Frame
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
	m_renderInfo.pFrame = this;
	m_renderInfo.pRenderFunc = boost::bind( &SceneHelper::render, this );
}

void SceneHelper::render()
{
	m_pGeometry->render(render::PrimTypeTriangleList, (int) (m_pGeometry->getIndexNum()/3));
}


class SampleApp
{
public:
	SampleApp() :  m_spApp(core::application::create())
	{
		m_spApp->add(core::task_ptr(new core::input_task(*m_spApp,0,false)));
		m_spApp->add(core::task_ptr(new core::render_task(*m_spApp,1)));
		m_spApp->add(core::task_ptr(new core::game_task(*m_spApp,2)));

		math::Vec3f vEyePt( 0.0f, 0, -30 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera = render::render_camera::create();
		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		render::TheCameraManager::get().addCamera(m_camera);


		m_spTargetCamera = math::target_camera::create(m_camera);
		m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);

        {
            using namespace input;

            Input::addCommand(L"Quit");
            Input::addCommand(L"Horz");
            Input::addCommand(L"Vert");
            Input::get_device(types::Keyboard)->get_control(types::KeyEscape)->bind(L"Quit");
            Input::get_device(types::Mouse   )->get_control(types::AxisX    )->bind(L"Horz");
			Input::get_device(types::Mouse   )->get_control(types::AxisY    )->bind(L"Vert");
            m_cEsc  .attach(L"Quit");
            m_cXAxis.attach(L"Horz");
            m_cYAxis.attach(L"Vert");
            m_cEsc   += boost::bind(&SampleApp::onEsc,   this);
            m_cXAxis += boost::bind(&SampleApp::onXAxis, this, _1);
            m_cYAxis += boost::bind(&SampleApp::onYAxis, this, _1);
        }

		m_spTargetCamera->activate();

		//render::TheLightManager::get().setAmbientColor(math::Color(50, 50, 50, 0));
	
		//render::CPointLight *pLight = new render::CPointLight("point1");
		//scene::TheScene::get().getRootFrame()->add(pLight);
		//pLight->setPosition(math::Vec3f(0,0,5));

		//pLight->setDiffuse(math::Color(230, 170, 170, 0));
		//pLight->setRange(100);

		m_pGeometry = render::Generator<vertex::PositionNormalColored>::CreateBox();

		m_pMySuper = boost::intrusive_ptr<SceneHelper>(new SceneHelper(m_pGeometry));
		scene::TheScene::get().getRootFrame()->add(m_pMySuper);
		m_pMySuper->setPosition(math::Vec3f(0,0,-5));

		m_spApp->run();
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
	math::camera_ptr       m_camera;

	::render::Generator<vertex::PositionNormalColored>::PGeometry m_pGeometry;

	boost::intrusive_ptr<SceneHelper> m_pMySuper;

	//данные для ввода
	input::KeyDown      m_cEsc;
	input::RelativeAxis m_cXAxis;
	input::RelativeAxis m_cYAxis;

	//данные для камеры
	//контроллер камеры "нацеленная камера"
	math::target_camera_ptr  m_spTargetCamera; 

	std::auto_ptr<core::application> m_spApp;
};

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	SampleApp r;
}