//RGDE
#include <rgde/engine.h>

#include "d3dx.h"

class ModelViewer :  public game::IDynamicObject   //и получать апдейты
{
	std::wstring getCommandLineParams()
	{
		std::wstring cmd_line = GetCommandLine();
		size_t first = cmd_line.find_first_of( '"' );
		size_t last = cmd_line.find_first_of( '"', first+1 );
		cmd_line.erase( first, last+2 );

		if ( cmd_line.size() > 0 )
		{
			first = cmd_line.find_first_of( '"' );
			last = cmd_line.find_first_of( '"', first+1 );
			if ( last != -1)
				cmd_line.erase( last, last+1 );
			if ( first != -1)
				cmd_line.erase( first, first+1 );
		}
		return cmd_line;
	}

	void initInput()
	{
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_c1.attachToControl(input::Keyboard, input::Key1);
		m_c2.attachToControl(input::Keyboard, input::Key2);
		m_c3.attachToControl(input::Keyboard, input::Key3);
		m_c4.attachToControl(input::Keyboard, input::Key4);
		m_c5.attachToControl(input::Keyboard, input::Key5);
		m_cO.attachToControl(input::Keyboard, input::KeyO);
		m_cL.attachToControl(input::Keyboard, input::KeyL);
		m_cK.attachToControl(input::Keyboard, input::KeyK);
		m_cB.attachToControl(input::Keyboard, input::KeyB);
		m_cG.attachToControl(input::Keyboard, input::KeyG);
		m_cW.attachToControl(input::Keyboard, input::KeyW);
		m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		m_cZAxis.attachToControl(input::Mouse, input::AxisWheel);
		m_cEsc.addHandler(this,&ModelViewer::onEsc);
		m_cO.addHandler(this,&ModelViewer::onO);
		m_c1.addHandler(this,&ModelViewer::on1);
		m_c2.addHandler(this,&ModelViewer::on2);
		m_c3.addHandler(this,&ModelViewer::on3);
		m_c4.addHandler(this,&ModelViewer::on4);
		m_c5.addHandler(this,&ModelViewer::on5);
		m_cL.addHandler(this,&ModelViewer::onL);
		m_cK.addHandler(this,&ModelViewer::onK);
		m_cB.addHandler(this,&ModelViewer::onB);
		m_cG.addHandler(this,&ModelViewer::onG);
		m_cW.addHandler(this,&ModelViewer::onW);
		m_cYAxis.addHandler(this,&ModelViewer::onYAxis);
		m_cXAxis.addHandler(this,&ModelViewer::onXAxis);
		m_cZAxis.addHandler(this,&ModelViewer::onWheelAxis);
	}

	void initCamera()
	{
		math::Vec3f vEyePt( 0.0f, 40, -300 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );
		
		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		m_cTargetCamera.setCamera( m_camera );
		m_cTargetCamera.setPosition(vEyePt,vLookatPt,vUpVec);
		m_cTargetCamera.activate();
	}

	void setAllMaterials(render::CModel::MaterialMap& maps, render::PMaterial& pMaterial)
	{
		for(render::CModel::MaterialMap::iterator it = maps.begin(); it != maps.end(); it++)
			it->second = pMaterial;
	}

	void setAllMaterials(render::CMesh::MaterialList& list, render::PMaterial& pMaterial)
	{
		for(render::CMesh::MaterialList::iterator it = list.begin(); it != list.end(); it++)
			*it = pMaterial;
	}

	void createModel(std::string strFileName)
	{
		if (m_spModel)
		{
			scene::TheScene::Get().getRootFrame()->removeChild(m_spModel);
		}

		std::string file = io::helpers::getFileNameWithoutExtension(strFileName);
		m_spModel = render::CModel::Create(file);

		if(m_spModel)
		{
			setAllMaterials(m_spModel->getMaterials(), m_pTestMaterial);

			render::CModel::Meshes& meshes = m_spModel->getMeshes();
			size_t nNumMeshes = meshes.size();
			for(size_t i = 0; i < nNumMeshes; i++)
				setAllMaterials(meshes[i]->getMaterials(), m_pTestMaterial);
				

			scene::TheScene::Get().getRootFrame()->addChild(m_spModel);
			m_spModel->play();
			m_spModel->setLooped( true );
		}

	}

	render::PTexture readTexture(const std::string& strFileName)
	{
		io::ScopePathAdd p("Common/");
		return render::ITexture::Create(strFileName);
	}

	void createMaterial()
	{
		m_pTestMaterial = render::CMaterial::Create(math::Color(255, 255, 255, 255), math::Color(170, 210, 170, 255), math::Color(255, 255, 255, 255), math::Black, 500.0f);

		
		m_diffuse = readTexture("RGDE.jpg");
		m_lightmap = readTexture("RGDE_Lightmap.jpg");
		m_illumination = readTexture("RGDE_Illumination.jpg");
		m_reflection = readTexture("Spheremap.jpg");
		m_bump = readTexture("RGDE_Bump.jpg");
		m_bDiffuse = false;
		m_bLightmap = false;
		m_bIllumination = false;
		m_bReflection = false;
		m_bBump = false;
	}

	void setupMap(render::CMaterial::CMaterialMap& map, bool bSetup, render::PTexture pTexture)
	{
		static render::PTexture pEmptyTexture;

		if(bSetup)
			map.setTexture(pTexture);
		else
			map.setTexture(pEmptyTexture);
	}

	void updateMaterial()
	{
		render::CMaterial::MaterialMaps& maps = m_pTestMaterial->getMaterialMaps();

		setupMap(maps["diffuse"], m_bDiffuse, m_diffuse);
		setupMap(maps["lightmap"], m_bLightmap, m_lightmap);
		setupMap(maps["illumination"], m_bIllumination, m_illumination);
		setupMap(maps["reflection"], m_bReflection, m_reflection);
		setupMap(maps["bump"], m_bBump, m_bump);

		m_pTestMaterial->setEffect(render::TheRenderManager::Get().getDefaultEffect());
	}

	void createLights()
	{
		render::TheLightManager::Get().setAmbientColor(math::Color(70, 70, 70, 255));

		render::CPointLight *pPointLight = new render::CPointLight("point1");
		pPointLight->setPosition(math::Vec3f(0, 40, 60));
		scene::TheScene::Get().getRootFrame()->addChild(pPointLight);

		pPointLight->setDiffuse(math::Color(255, 255, 255, 255));
		pPointLight->setSpecular(math::Color(255, 255, 255, 255));
		pPointLight->setAmbient(math::Color(0, 0, 0, 0));
		pPointLight->setRange(100);
		pPointLight->setEnabled(true);

		render::CPointLight *pPointLight2 = new render::CPointLight("point2");
		scene::TheScene::Get().getRootFrame()->addChild(pPointLight2);
		pPointLight2->setPosition(math::Vec3f(0, -40, -60));

		pPointLight2->setDiffuse(math::Color(255, 0, 0, 255));
		pPointLight2->setSpecular(math::Color(255, 255, 255, 255));
		pPointLight2->setAmbient(math::Color(0, 0, 0, 0));
		pPointLight2->setRange(100);
		pPointLight2->setEnabled(true);
	}
public:
	ModelViewer() :  m_spApp(core::IApplication::Create(L"Material Test", 800, 600, 32, 85, false))
	{
		m_spApp->addTask(core::PTask(new core::CInputTask(0, false)));
		m_spApp->addTask(core::PTask(new core::CGameTask(1)));
		m_spApp->addTask(core::PTask(new core::CRenderTask(2)));

		m_camera = render::CRenderCamera::Create();
		render::TheCameraManager::Get().addCamera(m_camera);

		render::TheRenderManager::Get().enableVolumes( true );

		m_spFont = render::IFont::Create(18, L"Arial", render::IFont::Heavy);

		createMaterial();

		std::wstring wcmd_line = getCommandLineParams();
		std::string cmd_line(wcmd_line.begin(), wcmd_line.end());

		if (cmd_line.size() > 0)
			createModel(cmd_line);
		
		initCamera();
		//инициализация ввода
		initInput();
		createLights();

		m_bRenderGrid = true;

		m_spApp->Run();		
	}

protected:

protected:
	virtual void update(float dt)
	{
//		if(m_spModel)
//			m_spModel->update( dt );
		render::TheDevice::Get().showFPS(m_spFont);

		if(m_bRenderGrid){
			render::TheDevice::Get().showWiredFloorGrid(300.0f, 20, math::Color(150, 150, 150, 255));
			render::TheDevice::Get().showWiredFloorGrid(300.0f, 2, math::Color(60, 60, 60, 255));
		}

		m_spFont->renderText(L"Клавиши:",math::Rect(1,446,400,400),0xEEEEEEEE,true);
		m_spFont->renderText(L"\"O\" - загрузка модели",math::Rect(1,464,400,400),0xEEEEEEEE,true);
		m_spFont->renderText(L"\"L\" - вкл/выкл отображения источников света",math::Rect(1,482,800,400),0xEEEEEEEE,true);
		m_spFont->renderText(L"\"K\" - вкл/выкл освещения",math::Rect(1,500,800,400),0xEEEEEEEE,true);
		m_spFont->renderText(L"\"B\" - вкл/выкл отображения AABBox-ов",math::Rect(1,518,400,400),0xEEEEEEEE,true);
		m_spFont->renderText(L"\"G\" - вкл/выкл отображения сетки",math::Rect(1,536,400,400),0xEEEEEEEE,true);
		m_spFont->renderText(L"\"W\" - вкл/выкл wireframe режима",math::Rect(1,554,400,400),0xEEEEEEEE,true);
		m_spFont->renderText(L"Мышь - вращение камерой",math::Rect(1,572,400,400),0xEEEEEEEE,true);
	}

	//выход из программы
	void onEsc(const input::CButtonEvent&)
	{
		core::IApplication::Get()->close();
	}

	void on1(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			m_bDiffuse = !m_bDiffuse;
		updateMaterial();
	}

	void on2(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			m_bLightmap = !m_bLightmap;
		updateMaterial();
	}

	void on3(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			m_bIllumination = !m_bIllumination;
		updateMaterial();
	}

	void on4(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			m_bReflection = !m_bReflection;
		updateMaterial();
	}

	void on5(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			m_bBump = !m_bBump;
		updateMaterial();
	}

	//opening a new file
	void onO(const input::CButtonEvent&)
	{
		std::wstring current_dir;
		current_dir.resize(256);
		GetCurrentDirectory( 256, (wchar_t*)current_dir.c_str() );
		std::wstring file; file.resize(256);
		std::wstring fileTitle; fileTitle.resize(256);
		
		// Set up the OPENFILENAME structure
		OPENFILENAME ofn;
		ofn.lStructSize       = sizeof(OPENFILENAME);
		ofn.hwndOwner         = 0;
		ofn.hInstance         = (HINSTANCE)GetModuleHandle( 0 );
		ofn.lpstrFilter       = L"Model files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";//(wchar_t*)filter.c_str();
		ofn.lpstrCustomFilter = (LPWSTR) NULL;
		ofn.nMaxCustFilter    = 0L;
		ofn.nFilterIndex      = 1L;
		ofn.lpstrFile         = (wchar_t*)file.c_str();
		ofn.nMaxFile          = (DWORD)file.size();
		ofn.lpstrFileTitle    = (wchar_t*)fileTitle.c_str();
		ofn.nMaxFileTitle     = (DWORD)fileTitle.size();
		ofn.lpstrInitialDir   = NULL;
		ofn.lpstrTitle        = L"Open a File";
		ofn.nFileOffset       = 0;
		ofn.nFileExtension    = 0;
		ofn.lpstrDefExt       = L"*.xml";
		ofn.lCustData         = 0;

		ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			SetCurrentDirectory( (wchar_t*)current_dir.c_str() );
			std::wstring t(ofn.lpstrFileTitle);

			createModel(std::string(t.begin(), t.end()));
		}		
	}

	void onL(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			render::TheLightManager::Get().setDebugRendering(!render::TheLightManager::Get().isDebugRendering());
	}

	void onK(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			render::TheRenderManager::Get().enableLighting(!render::TheRenderManager::Get().isLightingEnabled());
	}

	void onB(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			render::TheRenderManager::Get().enableVolumes(!render::TheRenderManager::Get().isVolumeDrawing());
	}

	void onG(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
			m_bRenderGrid = !m_bRenderGrid;
	}

	void onW(const input::CButtonEvent& event)
	{
		if(event.m_bPress)
		{
			int nFillMode = render::TheRenderManager::Get().getFillMode();

			switch(nFillMode)
			{
				case D3DFILL_SOLID:
					render::TheRenderManager::Get().setFillMode(D3DFILL_WIREFRAME);
				break;

				case D3DFILL_WIREFRAME:
					render::TheRenderManager::Get().setFillMode(D3DFILL_SOLID);
				break;
			};
		}
	}

	//ось X
	void onXAxis(const input::CRelativeAxisEvent &event)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

		m_cTargetCamera.rotateLeft(-angle);
	}

	//ось Y
	void onYAxis(const input::CRelativeAxisEvent &event)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

		m_cTargetCamera.rotateUp(angle);
	}

	/*onWheelAxis*/
	void onWheelAxis(const input::CRelativeAxisEvent &event)
	{
		const float slow = .1f;
		m_cTargetCamera.goBackward(-event.m_nDelta*slow);
	}

protected:
	render::PModel			m_spModel;
	render::PFont			m_spFont;
	math::PCamera			m_camera;            //указатель на камеру

	render::PMaterial       m_pTestMaterial;

	render::PTexture m_diffuse;
	render::PTexture m_lightmap;
	render::PTexture m_illumination;
	render::PTexture m_reflection;
	render::PTexture m_bump;

	bool m_bDiffuse;
	bool m_bLightmap;
	bool m_bIllumination;
	bool m_bReflection;
	bool m_bBump;

	bool m_bRenderGrid;

	//данные для ввода
	input::CButtonCommand       m_cEsc;
	input::CButtonCommand       m_c1;
	input::CButtonCommand       m_c2;
	input::CButtonCommand       m_c3;
	input::CButtonCommand       m_c4;
	input::CButtonCommand       m_c5;
	input::CButtonCommand       m_cO;
	input::CButtonCommand       m_cL;
	input::CButtonCommand       m_cK;
	input::CButtonCommand       m_cB;
	input::CButtonCommand       m_cG;
	input::CButtonCommand       m_cW;
	input::CRelativeAxisCommand m_cXAxis;
	input::CRelativeAxisCommand m_cYAxis;
	input::CRelativeAxisCommand m_cZAxis;

	//данные для камеры
	math::CTargetCamera      m_cTargetCamera;      //контроллер камеры "нацеленная камера"

	std::auto_ptr<core::IApplication> m_spApp;
};


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	ModelViewer r;
	return 0;
}