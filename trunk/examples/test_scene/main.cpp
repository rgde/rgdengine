#include <windows.h>
#include "header.h"
 
namespace scene
{
	//class CScene
	//{
	//public:
	//};

	//class CNode : public core::CNamedObject
	//{
	//public:
	//	// path в виде root.child1.child2.child3
	//	CNode* getChildNodeByPath(std::wstring path);
	//};

	//typedef base::TSingelton<CScene> Scene;
}

class ExampleObject //: public ::render::IRendererable
{
public:
	ExampleObject() : m_camera(render::CRenderCamera::Create()) //: ::render::IRendererable(10)
	{
		m_spFont = render::IFont::Create(20, L"Arial", render::IFont::Heavy);

		m_mesh.load("teapot.xml");
		m_pTexture = ::render::ITexture::Create("tiger.bmp");

		math::Vec3f vEyePt( 5.0f, 4, 0 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_camera->lookAt(vEyePt, vLookatPt, vUpVec);
		m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);

		//input 
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_cEsc.addHandler(this, &ExampleObject::onEsc);
	}

protected:
	virtual void render() 
	{
		m_camera->activate();

		{
			math::Rect rect(30,30, 400, 400);
			m_spFont->renderText(L"Welcome to RGDE !", rect, 0xFFFF0000, true);
		}
	}

protected:
	void onEsc (const input::CButtonEvent&)
	{
		core::IApplication::Get()->close();
	}

protected:
	render::CMesh		m_mesh;
	render::PTexture	m_pTexture;
	render::PFont		m_spFont;
	math::PCamera		m_camera;

	input::Button m_cEsc;
};


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	//std::auto_ptr<core::IApplication> spApp(core::IApplication::Create(L"Test Window", 640, 480, 32, 85, false));
	//new core::CRenderTask(0);
	//new core::CInputTask(1);
	//ExampleObject* p = new ExampleObject;
	//spApp->Run();
	//delete p;

	try
	{
		//CSomeObject* obj = new CSomeObject();

		//CSomeObject2* pObj2 = new CSomeObject2();
		//obj->addChild(pObj2);

		//CSomeObject3* pObj3 = new CSomeObject3();
		//pObj2->addChild(pObj3);

		//CXmlNode& node = obj->findNode("CSomeObject2.CSomeObject3");
		//std::ostringstream os;
		//ShowNode(os, *obj);
		//std::string res = os.str();

		//int i = 5;

	}
	catch(std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}