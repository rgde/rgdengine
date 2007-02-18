//RGDE
#include "engine.h"
#include "test_tetris.h"

DWORD interval;

#define numboxes 225
#define numboxesinablock 16

class My : public game::IDynamicObject
{
public:
	// теперь использовать вместо массива моделей
	struct Stone
	{
		math::PFrame frame;
		uint model_index;		
	};


	My() :  m_spApp(core::IApplication::Create(L"Tetris",600,600,32,85,false))
	{
		m_spApp->addTask(core::PTask(new core::CInputTask(0, false)));
		m_spApp->addTask(core::PTask(new core::CGameTask(1)));
		m_spApp->addTask(core::PTask(new core::CRenderTask(2)));

		math::Vec3f vEyePt( 0.0f, 0.f, -8.f );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera = render::CRenderCamera::Create();
		m_camera->setProjection(math::Math::PI/2, 1.0f, 1.0f, 100.0f);
		render::TheCameraManager::Get().addCamera(m_camera);

		m_cTargetCamera.setCamera( m_camera );
		m_cTargetCamera.setPosition(vEyePt,vLookatPt,vUpVec);

		m_spFont = render::IFont::Create(18, L"Courier New", render::IFont::Heavy);
		m_spFontBig = render::IFont::Create(40,L"Arial", render::IFont::Medium);
		//инициализация ввода
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_cEsc.addHandler(this,&My::onEsc);
		m_cLeft.attachToControl(input::Keyboard, input::KeyLeft);
		m_cLeft.addHandler(this,&My::onLeft);
		m_cRight.attachToControl(input::Keyboard, input::KeyRight);
		m_cRight.addHandler(this,&My::onRight);
		m_cSpace.attachToControl(input::Keyboard, input::KeySpace);
		m_cSpace.addHandler(this,&My::onSpace);
		m_cDown.attachToControl(input::Keyboard, input::KeyDown);
		m_cDown.addHandler(this,&My::onDown);

		m_cTargetCamera.activate();

		render::TheRenderManager::Get().enableLighting(true);
		render::TheLightManager::Get().setAmbientColor(math::Color(20, 20, 20, 255));
				
		render::CPointLight *pLight = new render::CPointLight("point1");
		scene::TheScene::Get().getRootFrame()->addChild(pLight);		

		pLight->setPosition(math::Vec3f(-10,-10,-10));				
		pLight->setDiffuse(math::Color(235, 0, 0, 255));
		pLight->setRange(50.f);
		pLight->setEnabled(true);

		render::TheRenderManager::Get().enableVolumes(false);
		math::Vec3f scale(0.012f,0.012f,0.012f);
		for(int i=0;i < numboxes;i++) 
		{
			boxes[i] = render::CModel::Create("gems1");
			scene::TheScene::Get().getRootFrame()->addChild(boxes[i]);
			boxes[i]->setPosition(math::Vec3f((i%15)-7.f,(i/15)-7.f,0));
			boxes[i]->setScale(scale);
			boxes[i]->setVisible(false);
		}

		bl = boost::shared_ptr<CBlock>(new CBlock());

		for(int i=0;i < numboxesinablock;i++) 
		{
			blocks[i] = render::CModel::Create("gems1");
			scene::TheScene::Get().getRootFrame()->addChild(blocks[i]);
			blocks[i]->setScale(scale);
			blocks[i]->setPosition(math::Vec3f(bl->getx()+(i%4)-7,bl->gety()+(i/4)-7,0.f));
			blocks[i]->setVisible(false);
		}

		m_effect = render::TheRenderManager::Get().getDefaultEffect();
		
		interval = 500;
		m_spApp->Run();
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

			//render::CModel::Frames m_vFrames = blocks[i]->getFrames();
			//size_t nNumFrames = m_vFrames.size();
			//for(size_t j = 0; j < nNumFrames; j++)
			//  m_vFrames[j]->setPosition(math::Vec3f(bl->getx()+(i%4)-7,bl->gety()+(i/4)-7,0.f));

			blocks[i]->setPosition(math::Vec3f(bl->getx()+(i%4)-7,bl->gety()+(i/4)-7,0.f));
		}
		for(int i=0;i<=3;i++) 
			for(int j=0;j<=3;j++)
			{
				if(bl->m_mat[i][j]!=0)	blocks[i*4+j]->setVisible(true);
			}
	}

	void update(float dt)
	{
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
		render::TheDevice::Get().showFPS(m_spFont);
		
		render::TheDevice::Get().showWiredFloorGrid(15.0f, 15, math::Color(60, 60, 60, 255));
		renderMatr();
		renderBlock();
		switch(bl->m_ilastnum)
		{
			case 2 : 
				m_spFontBig->renderText(L"Double!",math::Rect(320,320,200,200),math::Color(10, 10, 255, 255).color,true);
				break;
			case 3 : 
				m_spFontBig->renderText(L"Triple!",math::Rect(320,320,200,200),math::Color(10, 255, 10, 255).color,true);
				break;
			case 4 : 
				m_spFontBig->renderText(L"Quadriple!",math::Rect(320,320,250,200),math::Color(255, 10, 10, 255).color,true);
				break;
		}
		WCHAR str[80];
		swprintf(str,L"Ваш счёт:   %d",bl->m_ipoints);
		m_spFont->renderText(str,math::Rect(400,1,200,200),0xEEEEEEEE,true);
	}

	//выход из программы
	void onEsc(const input::CButtonEvent&)
	{
		core::IApplication::Get()->close();
	}

	void onLeft(const input::CButtonEvent& event)
	{
		if(event.m_bPress) 
		{
			bl->move(0);
		}
	}
	void onRight(const input::CButtonEvent& event)
	{
		if(event.m_bPress) 
		{
			bl->move(1);
		}
	}
	void onDown(const input::CButtonEvent& event)
	{
		if(event.m_bPress) 
		{
			interval=100;
		} else
			interval=500;
	}
	void onSpace(const input::CButtonEvent& event)
	{
		if(event.m_bPress) 
		{
			bl->rotate();
		}
	}

	void renderStone(const Stone& stone)
	{
		render::PModel& model = m_models[stone.model_index]; // следить что бы не было выхода за пределы вектора
		render::PMesh& mesh = model->getMeshes()[0]; // мы знаем что у нас только 1 меш в модели. но нет проблем так сделать и для других вариантов
		render::CMesh::PGeometry geom = mesh->getGeometry();
		render::PMaterial& material = mesh->getMaterials()[0];		

		material->bind();
	}

protected:
	std::vector<render::PModel> m_models; // тут храним исходные модели камушков
	render::PEffect				m_effect;
	


	math::PCamera       m_camera;            //указатель на камеру
	::render::PFont m_spFont,m_spFontBig;

	//данные для ввода
	input::CButtonCommand       m_cEsc,m_cLeft,m_cRight,m_cDown,m_cSpace;
	
	//данные для камеры
	math::CTargetCamera      m_cTargetCamera;      //контроллер камеры "нацеленная камера"

	std::auto_ptr<core::IApplication> m_spApp;

	render::PModel boxes[numboxes];
	render::PModel blocks[numboxesinablock];

	// Надо сделать так:
	//Stone boxes[numboxes];
	//Stone blocks[numboxesinablock];

	boost::shared_ptr<CBlock> bl;
};


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	My heh;
}