#include "game.h"

namespace gulman
{

Game::Game() 
{
	initCamera();

	render::TheRenderManager::Get().enableVolumes( false );
	render::TheRenderManager::Get().enableLighting(true);
//	render::TheLightManager::Get().setAmbientColor(math::Color(255, 255, 255, 255));

	m_spFont = render::IFont::Create(11, L"Arial", render::IFont::Heavy);

	createModel();	

	initInput();
}


Game::~Game()
{
}

void Game::initInput()
{
	m_cEsc.attach(L"Exit");
	m_cXAxis.attach(L"XAxis");
	m_cYAxis.attach(L"YAxis");
	m_cZAxis.attach(L"ZAxis");

	using namespace input;

	Input::GetControl(types::Keyboard, types::KeyEscape )->bind(m_cEsc.getCommand());
	Input::GetControl(types::Mouse, types::AxisX )->bind(m_cXAxis.getCommand());
	Input::GetControl(types::Mouse, types::AxisY )->bind(m_cYAxis.getCommand());
	Input::GetControl(types::Mouse, types::AxisWheel )->bind(m_cZAxis.getCommand());

	m_cEsc += boost::bind(&Game::onEsc, this);
	m_cYAxis += boost::bind(&Game::onYAxis, this, _1);
	m_cXAxis += boost::bind(&Game::onXAxis, this, _1);
	m_cZAxis += boost::bind(&Game::onWheelAxis, this, _1);
}

void Game::initCamera()
{
	m_camera = render::CRenderCamera::Create();
	render::TheCameraManager::Get().addCamera(m_camera);

	math::Vec3f vEyePt( 0.0f, 40, -300 );
	math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
	math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );
	
	m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);

    m_spTargetCamera = math::CTargetCamera::Create(m_camera);
	m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);
	m_spTargetCamera->activate();
}

void Game::createModel()
{
	m_spModel = render::CModel::Create("kauta");

	if(m_spModel)
	{
		scene::TheScene::Get().getRootFrame()->addChild(m_spModel);
		m_spModel->play();
		m_spModel->setLooped( true );
	}
}



void Game::update(float dt)
{
	render::TheDevice::Get().showFPS(m_spFont);
}

//выход из программы
void Game::onEsc()
{
	core::IApplication::Get()->close();
}


//ось X
void Game::onXAxis(int delta)
{
	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = delta > accel ? delta*fast : delta*slow;

	if (input::Input::GetControl(input::types::Mouse, input::types::ButtonLeft)->getPress())
	{
		math::Vec3f cam_up;
		math::Vec3f cam_pos;
		math::Vec3f cam_target;
		m_spTargetCamera->getPosition(cam_up,cam_pos,cam_target);

		math::Vec3f dir = cam_target - cam_pos;
		math::normalize(dir);
		math::normalize(cam_up);
		math::Vec3f cam_side = math::makeCross(cam_up, dir);
		cam_side *= -delta*0.1f;

		cam_pos += cam_side;
		cam_target += cam_side;
		m_spTargetCamera->setPosition(cam_up,cam_pos,cam_target);

	}
	else if (input::Input::GetControl(input::types::Mouse, input::types::ButtonRight)->getPress() == false)
		return;
	else
	{
		m_spTargetCamera->rotateLeft(-angle);
	}
}

//ось Y
void Game::onYAxis(int delta)
{
	using namespace input;
	if (Input::GetControl(types::Mouse, types::ButtonLeft)->getPress())
	{
		math::Vec3f cam_up;
		math::Vec3f cam_pos;
		math::Vec3f cam_target;
		m_spTargetCamera->getPosition(cam_up, cam_pos, cam_target);

		math::Vec3f dir = cam_target - cam_pos;
		math::normalize(dir);
		dir *= delta;

		cam_pos += dir;
		cam_target += dir;
		m_spTargetCamera->setPosition(cam_up,cam_pos,cam_target);
		return;

	}
	else if (Input::GetControl(types::Mouse, types::ButtonRight)->getPress() == false)
		return;

	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = delta>accel ? delta*fast : delta*slow;

	m_spTargetCamera->rotateUp(angle);
}

/*onWheelAxis*/
void Game::onWheelAxis(int delta)
{
	const float slow = .1f;
	m_spTargetCamera->goBackward(-delta*slow);
}
}