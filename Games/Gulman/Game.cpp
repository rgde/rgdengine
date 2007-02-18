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
	m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
	m_cXAxis.attachToControl(input::Mouse, input::AxisX);
	m_cYAxis.attachToControl(input::Mouse, input::AxisY);
	m_cZAxis.attachToControl(input::Mouse, input::AxisWheel);
	m_cEsc.addHandler(this,&Game::onEsc);
	m_cYAxis.addHandler(this,&Game::onYAxis);
	m_cXAxis.addHandler(this,&Game::onXAxis);
	m_cZAxis.addHandler(this,&Game::onWheelAxis);
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
void Game::onEsc(const input::CButtonEvent&)
{
	core::IApplication::Get()->close();
}


//ось X
void Game::onXAxis(const input::CRelativeAxisEvent &event)
{
	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

	if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonLeft)->getPress())
	{
		math::Vec3f cam_up;
		math::Vec3f cam_pos;
		math::Vec3f cam_target;
		m_spTargetCamera->getPosition(cam_up,cam_pos,cam_target);

		math::Vec3f dir = cam_target - cam_pos;
		math::normalize(dir);
		math::normalize(cam_up);
		math::Vec3f cam_side = math::makeCross(cam_up, dir);
		cam_side *= -event.m_nDelta*0.1f;

		cam_pos += cam_side;
		cam_target += cam_side;
		m_spTargetCamera->setPosition(cam_up,cam_pos,cam_target);

	}
	else if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonRight)->getPress() == false)
		return;
	else
	{
		m_spTargetCamera->rotateLeft(-angle);
	}
}

//ось Y
void Game::onYAxis(const input::CRelativeAxisEvent &event)
{
	if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonLeft)->getPress())
	{
		math::Vec3f cam_up;
		math::Vec3f cam_pos;
		math::Vec3f cam_target;
		m_spTargetCamera->getPosition(cam_up, cam_pos, cam_target);

		math::Vec3f dir = cam_target - cam_pos;
		math::normalize(dir);
		dir *= event.m_nDelta;

		cam_pos += dir;
		cam_target += dir;
		m_spTargetCamera->setPosition(cam_up,cam_pos,cam_target);
		return;

	}
	else if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonRight)->getPress() == false)
		return;

	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

	m_spTargetCamera->rotateUp(angle);
}

/*onWheelAxis*/
void Game::onWheelAxis(const input::CRelativeAxisEvent &event)
{
	const float slow = .1f;
	m_spTargetCamera->goBackward(-event.m_nDelta*slow);
}
}