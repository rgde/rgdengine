//RGDE
#include <rgde/engine.h>
#include "tetris.h"

DWORD interval;

#define numboxes 225
#define numboxesinablock 16

class My : public game::dynamic_object
{
public:
	// теперь использовать вместо массива моделей
	struct stone
	{
		math::frame_ptr frame;
		uint model_index;		
	};


	My() :  m_spApp(core::application::create(L"Tetris",600,600,false))
	{
		m_spApp->add(core::task_ptr(new core::input_task(*m_spApp, 0, false)));
		m_spApp->add(core::task_ptr(new core::game_task(*m_spApp, 1)));
		m_spApp->add(core::task_ptr(new core::render_task(*m_spApp, 2)));

		math::Vec3f vEyePt( 0.0f, 0.f, -8.f );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera = render::render_camera::create();
		m_camera->set_projection(math::Math::PI/2, 1.0f, 1.0f, 100.0f);
		render::TheCameraManager::get().add_camera(m_camera);

		m_cTargetCamera = math::target_camera::create( m_camera );
		m_cTargetCamera->set_position(vUpVec,vEyePt,vLookatPt);

		m_spFont = render::font::create(18, L"Courier New", render::font::Heavy);
		m_spFontBig = render::font::create(40,L"Arial", render::font::Medium);

		//инициализация ввода
		{
			using namespace input;

			Input::add_command(L"Quit");
			Input::add_command(L"Left");
			Input::add_command(L"Right");
			Input::add_command(L"Down");
			Input::add_command(L"Drop");

			Input::get_device(types::Keyboard)->get_control(types::KeyEscape)->bind(L"Quit");
			Input::get_device(types::Keyboard)->get_control(types::KeyLeft)->bind(L"Left");
			Input::get_device(types::Keyboard)->get_control(types::KeyRight)->bind(L"Right");
			Input::get_device(types::Keyboard)->get_control(types::key_down)->bind(L"Down");
			Input::get_device(types::Keyboard)->get_control(types::KeySpace)->bind(L"Drop");
			
			m_cEsc  .attach(L"Quit");
			m_cLeft.attach(L"Left");
			m_cRight.attach(L"Right");
			m_cDown.attach(L"Down");
			m_cSpace.attach(L"Drop");

			m_cEsc   += boost::bind(&My::onEsc,   this);
			m_cLeft   += boost::bind(&My::onLeft,   this);
			m_cRight   += boost::bind(&My::onRight,   this);
			m_cDown   += boost::bind(&My::onDown,   this);
			m_cSpace   += boost::bind(&My::onSpace,   this);
		}

		m_cTargetCamera->activate();

		render::TheRenderManager::get().enableLighting(true);

		//render::TheLightManager::get().setAmbientColor(math::Color(20, 20, 20, 255));
		
		render::PointLight *pLight = new render::PointLight("point1");
		scene::TheScene::get().get_root()->add(pLight);		

		pLight->set_position(math::Vec3f(-10,-10,-10));				
		pLight->setDiffuse(math::Color(235, 0, 0, 255));
		pLight->setRange(50.f);
		pLight->setEnabled(true);

		render::TheRenderManager::get().enableVolumes(false);
		math::Vec3f scale(0.012f,0.012f,0.012f);
		for(int i=0;i < numboxes;i++) 
		{
			boxes[i] = render::model::create("gems1");
			scene::TheScene::get().get_root()->add(boxes[i]);
			boxes[i]->set_position(math::Vec3f((i%15)-7.f,(i/15)-7.f,0));
			boxes[i]->set_scale(scale);
			boxes[i]->setVisible(false);
		}

		bl = boost::shared_ptr<CBlock>(new CBlock());

		for(int i=0;i < numboxesinablock;i++) 
		{
			blocks[i] = render::model::create("gems1");
			scene::TheScene::get().get_root()->add(blocks[i]);
			blocks[i]->set_scale(scale);
			blocks[i]->set_position(math::Vec3f(bl->getx()+(i%4)-7,bl->gety()+(i/4)-7,0.f));
			blocks[i]->setVisible(false);
		}

		m_effect = render::TheRenderManager::get().getDefaultEffect();
		
		interval = 500;
		m_spApp->run();
	}

protected:

	void renderMatr()
	{
		int i;
		for(int i=0;i < numboxes;i++) boxes[i]->setVisible(false);
		for(i=0;i<=14;i++) 
			for(int j=0;j<=14;j++)
			{				
				if(bl->m_matr[i+1][j+1]!=0)	boxes[i*15+j]->setVisible(true);
			}
	}

	void renderBlock()
	{
		for(int i=0;i < numboxesinablock;i++) 
		{
			blocks[i]->setVisible(false);

			//render::CModel::frames_vector m_frames = blocks[i]->getFrames();
			//size_t nNumFrames = m_frames.size();
			//for(size_t j = 0; j < nNumFrames; j++)
			//  m_frames[j]->set_position(math::Vec3f(bl->getx()+(i%4)-7,bl->gety()+(i/4)-7,0.f));

			blocks[i]->set_position(math::Vec3f(bl->getx()+(i%4)-7,bl->gety()+(i/4)-7,0.f));
		}
		for(int i=0;i<=3;i++) 
			for(int j=0;j<=3;j++)
			{
				if(bl->m_mat[i][j]!=0)	blocks[i*4+j]->setVisible(true);
			}
	}

	void update(float dt)
	{
		//interval = m_cDown ? 100 : 500;
		interval = 250;

		//for(int i=0; i<=numboxes; i++) boxes[i]->update(dt);
		//for(i=0; i<=numboxesinablock; i++) blocks[i]->update(dt);
		static float elapsed=0;
		elapsed+=dt;
		if(elapsed>interval/10000.f)
		{ 
			bl->m_ilastnum=0;
			bl->move(2); 
			elapsed=0;
		}
		render::TheDevice::get().showFPS(m_spFont);
		
		render::TheDevice::get().showWiredFloorGrid(15.0f, 15, math::Color(60, 60, 60, 255));
		renderMatr();
		renderBlock();
		switch(bl->m_ilastnum)
		{
			case 2 : 
				m_spFontBig->render(L"Double!",math::Rect(320,320,200,200),math::Color(10, 10, 255, 255).color,true);
				break;
			case 3 : 
				m_spFontBig->render(L"Triple!",math::Rect(320,320,200,200),math::Color(10, 255, 10, 255).color,true);
				break;
			case 4 : 
				m_spFontBig->render(L"Quadriple!",math::Rect(320,320,250,200),math::Color(255, 10, 10, 255).color,true);
				break;
		}
		WCHAR str[80];
		swprintf(str,L"Ваш счёт:   %d",bl->m_ipoints);
		m_spFont->render(str,math::Rect(400,1,200,200),0xEEEEEEEE,true);
	}

	//выход из программы
	void onEsc()
	{
		core::application::get()->close();
	}

	void onLeft()
	{
		bl->move(0);
	}

	void onRight()
	{
		bl->move(1);
	}

	void onDown()
	{
		//if(e) 
			//interval=100;
		//else
		//	interval=500;
	}

	void onSpace()
	{
		bl->rotate();
	}

	void renderStone(const stone& stone)
	{
		render::model_ptr& model = m_models[stone.model_index]; // следить что бы не было выхода за пределы вектора
		render::mesh_ptr& mesh = model->getMeshes()[0]; // мы знаем что у нас только 1 меш в модели. но нет проблем так сделать и для других вариантов
		render::mesh::PGeometry geom = mesh->getGeometry();
		render::material_ptr& material = mesh->getMaterials()[0];		

		//material->bind();
	}

protected:
	std::vector<render::model_ptr> m_models; // тут храним исходные модели камушков
	render::effect_ptr				m_effect;

	math::camera_ptr       m_camera;            //указатель на камеру
	::render::font_ptr m_spFont,m_spFontBig;

	//данные для ввода
	input::key_down      m_cEsc,m_cLeft,m_cRight,m_cDown,m_cSpace;
	
	//данные для камеры
	math::target_camera_ptr      m_cTargetCamera;      //контроллер камеры "нацеленная камера"

	std::auto_ptr<core::application> m_spApp;

	render::model_ptr boxes[numboxes];
	render::model_ptr blocks[numboxesinablock];

	// Надо сделать так:
	//stone boxes[numboxes];
	//stone blocks[numboxesinablock];

	boost::shared_ptr<CBlock> bl;
};


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	My heh;
}