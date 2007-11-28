#include "engine.h"

#include "SkinnedMesh.h"
#include "AnimationController.h"

const float CYLINDER_RADIUS = 20.0f;
const float CYLINDER_HEIGHT = CYLINDER_RADIUS*12.0f;
const float ANIMATION_TIME = 1.0f;
const int NUM_ANIMATIONS = 3;
const int CYLINDER_HEIGHT_SEGMENTS = 16;
const int CYLINDER_RADIUS_SEGMENTS = 16;

class CSkinningTest : public game::IDynamicObject   //и получать апдейты
{
	void initInput()
	{
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_c1.attachToControl(input::Keyboard, input::Key1);
		m_c2.attachToControl(input::Keyboard, input::Key2);
		m_c3.attachToControl(input::Keyboard, input::Key3);
		m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		m_cZAxis.attachToControl(input::Mouse, input::AxisWheel);
		m_cEsc.addHandler(this,&CSkinningTest::onEsc);
		m_c1.addHandler(this,&CSkinningTest::on1);
		m_c2.addHandler(this,&CSkinningTest::on2);
		m_c3.addHandler(this,&CSkinningTest::on3);
		m_cYAxis.addHandler(this,&CSkinningTest::onYAxis);
		m_cXAxis.addHandler(this,&CSkinningTest::onXAxis);
		m_cZAxis.addHandler(this,&CSkinningTest::onWheelAxis);
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

	void createLights()
	{
//		render::TheLightManager::Get().setAmbientColor(math::Color(200, 200, 200, 255));
		
//		render::TheRenderManager::Get().enableLighting(false);

		//render::CPointLight *pPointLight = new render::CPointLight("point1");
		//scene::TheScene::Get().getRootFrame()->addChild(pPointLight);
		//pPointLight->setPosition(math::Vec3f(0,40,60));

		//pPointLight->setDiffuse(math::Color(180, 180, 180, 250));
		//pPointLight->setSpecular(math::Color(30, 30, 30, 250));
		//pPointLight->setAmbient(math::Color(0, 0, 0, 0));
		//pPointLight->setRange(5500);
		//pPointLight->setEnabled(true);

//		render::TheRenderManager::Get().enableLighting(false);

/*		
		math::PFrame pSpotLightFrame(new math::CFrame());
		scene::TheScene::Get().getRootFrame()->addChild(pSpotLightFrame);
		pSpotLightFrame->setPosition(math::Vec3f(0,-40,-100));
		render::CSpotLight *pSpotLight = new render::CSpotLight("spot1", pSpotLightFrame);

		pSpotLight->setDiffuse(math::Color(180, 180, 180, 250));
		pSpotLight->setSpecular(math::Color(30, 30, 30, 250));
		pSpotLight->setAmbient(math::Color(0, 0, 0, 0));
		pSpotLight->setRange(5500);
		pSpotLight->setEnabled(true);
		pSpotLight->setTheta(0);
		pSpotLight->setPhi(45.0f/180.0f*3.14f);
		pSpotLight->setFalloff(1);


		math::PFrame pDirectionalLightFrame(new math::CFrame());
		scene::TheScene::Get().getRootFrame()->addChild(pDirectionalLightFrame);
		pDirectionalLightFrame->setPosition(math::Vec3f(0,-40,-100));
		render::CDirectionalLight *pDirectionalLight = new render::CDirectionalLight("directional1", pDirectionalLightFrame);

		pDirectionalLight->setDiffuse(math::Color(180, 180, 180, 250));
		pDirectionalLight->setSpecular(math::Color(30, 30, 30, 250));
		pDirectionalLight->setAmbient(math::Color(0, 0, 0, 0));
		pDirectionalLight->setEnabled(true);
		pDirectionalLight->setDirection(math::Vec3f(0, 0, 1));
*/
	}

	void setActiveBone(SkinnedMeshVertex& vertex, int bone)
	{
		vertex.weights[0] = 1.0f;
		vertex.indices[0] = bone;
		for(size_t i = 1; i < 4; ++i)
		{
			vertex.weights[i] = 0.0f;
			vertex.indices[i] = 0;
		}
	}

	static void generateSkinnedCylinder(PSkinnedMesh pMesh)
	{
		pMesh->getGeometry() = render::CGenerator<SkinnedMeshVertex, false>::CreateCylinder(CYLINDER_RADIUS_SEGMENTS, CYLINDER_HEIGHT_SEGMENTS, CYLINDER_RADIUS, CYLINDER_HEIGHT);
		pMesh->getSkinnedGeometry() = render::CGenerator<SkinnedMeshVertex, false>::CreateCylinder(CYLINDER_RADIUS_SEGMENTS, CYLINDER_HEIGHT_SEGMENTS, CYLINDER_RADIUS, CYLINDER_HEIGHT);

		//Set up bone weights
		{
			CSkinnedMesh::Geometry::Vertexes& verts = pMesh->getGeometry()->lockVB();

			for(size_t i = 0; i < verts.size(); i++)
			{
				SkinnedMeshVertex& vertex = verts[i];
				float parameter = (vertex.position[1] + CYLINDER_HEIGHT*0.5f)/CYLINDER_HEIGHT;

				vertex.weights[3] = 0.0f;
				vertex.indices[3] = 0;

				vertex.indices[0] = 0;
				vertex.indices[1] = 1;
				vertex.indices[2] = 2;

				if(parameter < 0.5f)
				{
					vertex.weights[0] = 1.0f - parameter*2.0f;
					vertex.weights[1] = parameter*2.0f;
					vertex.weights[2] = 0.0f;
				}
				else
				{
					vertex.weights[0] = 0.0f;
					vertex.weights[1] = 1.0f - (parameter - 0.5f)*2.0f;
					vertex.weights[2] = (parameter - 0.5f)*2.0f;
				}

				float summ = vertex.weights[0] + vertex.weights[1] + vertex.weights[2] + vertex.weights[3];
				if(summ != 0.0f)
					vertex.weights /= summ;

				vertex.color = math::Color(vertex.weights);
				vertex.color.a = 255;
//				vertex.color = math::Color(255, 255, 255, 255);
			}

			pMesh->getGeometry()->unlockVB();
		}
	}

	void createSkeleton()
	{
		m_bones.resize(4);

		for(int i = 0; i < 4; i++)
			m_bones[i] = math::PFrame(new math::CFrame());

		for(int i = 0; i < 4 - 1; i++)
			m_bones[4 - 1]->addChild(m_bones[i]);

		for(int i = 0; i < 4; i++)
			m_bones[i]->setScale(math::Vec3f(1.0f, 1.0f, 1.0f));

		m_bones[0]->setPosition(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));
		m_bones[2]->setPosition(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));
	}

	void blendTo(int to)
	{
		if(m_curAnim != to)
		{
			m_controller->setAnimation1(m_vAnimations[to]);
			m_controller->setAnimation2(m_vAnimations[m_curAnim]);
			m_controller->setBlendValue(0.0f);
			m_controller->setFullTime(m_vAnimations[to]->getFullTime());
			m_curAnim = to;
		}
	}

	void createMachine()
	{
		m_controller = PAnimationController(new CAnimationController(m_bones, ANIMATION_TIME));
		m_controller->setBlendingSpeed(0.2f);

		m_controller->setLooped(true);
		m_controller->start();
	}

	void initSkinnedMesh()
	{
		createSkeleton();

		m_pMesh = PSkinnedMesh(new CSkinnedMesh(m_bones));

		createAnimations();

		generateSkinnedCylinder(m_pMesh);

		createMachine();
		m_curAnim = 1;
		blendTo(0);
	}

	void createAnimations()
	{
		for(int i = 0; i < NUM_ANIMATIONS; i++)
			m_vAnimations[i] = PSkeletonAnimation(new CSkeletonAnimation(ANIMATION_TIME));

		createAnimation1(m_bones, m_vAnimations[0]);
		createAnimation2(m_bones, m_vAnimations[1]);
		createAnimation3(m_bones, m_vAnimations[2]);
	}

	static inline math::Quatf convertEylerToQuat(const math::Vec3f& angle)
	{
			math::EulerAngleXYZf ang(angle[0], angle[1], angle[2]);

			math::Quatf q;
			math::set(q, ang);

			return q;
	}

	static void createAnimation1(CSkeleton::Bones& bones, PSkeletonAnimation& animation)
	{
		for(int i = 0; i < 4; i++)
			animation->addBone(i);

		animation->getBoneAnimation(0).addKey(0.0f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(0).addKey(0.25f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(3.14f/2.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(0).addKey(0.75f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(-3.14f/2.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(0).addKey(1.0f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));


		animation->getBoneAnimation(2).addKey(0.0f, CBoneState(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(2).addKey(1.0f, CBoneState(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
	}

	static void createAnimation2(CSkeleton::Bones& bones, PSkeletonAnimation& animation)
	{
/*		PFrameAnimationController controllers[4];

		for(int i = 0; i < 4; i++)
		{
			animation->getBoneAnimation(i) = PFrameAnimationController(new CFrameAnimationController());
			animation->getBoneAnimation(i)->setFrame(bones[i]);

			animation->addController(animation->getBoneAnimation(i));

			animation->getBoneAnimation(i).getScaleInterpolyator().addKey(0.0f, math::Vec3f(1.0f, 1.0f, 1.0f));
			animation->getBoneAnimation(i).getScaleInterpolyator().addKey(1.0f, math::Vec3f(1.0f, 1.0f, 1.0f));
		}
/*
		animation->getBoneAnimation(0).getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));
		animation->getBoneAnimation(0).getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));

		animation->getBoneAnimation(0).getRotationInterpolyator().addKey(0.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(0).getRotationInterpolyator().addKey(0.25f, convertEylerToQuat(math::Vec3f(3.14f/2.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(0).getRotationInterpolyator().addKey(0.75f, convertEylerToQuat(math::Vec3f(-3.14f/2.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(0).getRotationInterpolyator().addKey(1.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));

		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.25f, convertEylerToQuat(math::Vec3f(-3.14f/2.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.75f, convertEylerToQuat(math::Vec3f(3.14f/2.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(1.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));

		animation->getBoneAnimation(2).getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));
		animation->getBoneAnimation(2).getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));

		controllers[3]->getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, 0.0f, 0.0f));
		controllers[3]->getPosInterpolyator().addKey(0.25f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.125f, 0.0f));
		controllers[3]->getPosInterpolyator().addKey(0.75f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.125f, 0.0f));
		controllers[3]->getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, 0.0f, 0.0f));
*/
/*		animation->getBoneAnimation(0).getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));
		animation->getBoneAnimation(0).getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));

		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.25f, convertEylerToQuat(math::Vec3f(3.14f/2.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.75f, convertEylerToQuat(math::Vec3f(-3.14f/2.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(1.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));

		animation->getBoneAnimation(2).getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));
		animation->getBoneAnimation(2).getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));

		controllers[3]->getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, 0.0f, 0.0f));
		controllers[3]->getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, 0.0f, 0.0f));
*/
		for(int i = 0; i < 4; i++)
			animation->addBone(i);

		animation->getBoneAnimation(0).addKey(0.0f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(0).addKey(1.0f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));


		animation->getBoneAnimation(2).addKey(0.0f, CBoneState(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(2).addKey(0.25f, CBoneState(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(3.14f/2.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(2).addKey(0.75f, CBoneState(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(-3.14f/2.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(2).addKey(1.0f, CBoneState(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
	}

	static void createAnimation3(CSkeleton::Bones& bones, PSkeletonAnimation& animation)
	{
/*		PFrameAnimationController controllers[4];

		for(int i = 0; i < 4; i++)
		{
			animation->getBoneAnimation(i) = PFrameAnimationController(new CFrameAnimationController());
			animation->getBoneAnimation(i)->setFrame(bones[i]);

			animation->addController(animation->getBoneAnimation(i));

			animation->getBoneAnimation(i).getScaleInterpolyator().addKey(0.0f, math::Vec3f(1.0f, 1.0f, 1.0f));
			animation->getBoneAnimation(i).getScaleInterpolyator().addKey(1.0f, math::Vec3f(1.0f, 1.0f, 1.0f));
		}
/*
		animation->getBoneAnimation(0).getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));
		animation->getBoneAnimation(0).getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));

		animation->getBoneAnimation(0).getRotationInterpolyator().addKey(0.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(0).getRotationInterpolyator().addKey(0.25f, convertEylerToQuat(math::Vec3f(3.14f, 0.0f, 0.0f)));
		animation->getBoneAnimation(0).getRotationInterpolyator().addKey(0.75f, convertEylerToQuat(math::Vec3f(-3.14f, 0.0f, 0.0f)));
		animation->getBoneAnimation(0).getRotationInterpolyator().addKey(1.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));

		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.25f, convertEylerToQuat(math::Vec3f(-3.14f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(0.75f, convertEylerToQuat(math::Vec3f(3.14f, 0.0f, 0.0f)));
		animation->getBoneAnimation(2).getRotationInterpolyator().addKey(1.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));

		animation->getBoneAnimation(2).getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));
		animation->getBoneAnimation(2).getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));
*/
/*		animation->getBoneAnimation(0).getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));
		animation->getBoneAnimation(0).getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f));

		animation->getBoneAnimation(1).getRotationInterpolyator().addKey(0.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));
		animation->getBoneAnimation(1).getRotationInterpolyator().addKey(0.25f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 3.14f/2.0f)));
		animation->getBoneAnimation(1).getRotationInterpolyator().addKey(0.75f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, -3.14f/2.0f)));
		animation->getBoneAnimation(1).getRotationInterpolyator().addKey(1.0f, convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)));

		animation->getBoneAnimation(2).getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));
		animation->getBoneAnimation(2).getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f));

		controllers[3]->getPosInterpolyator().addKey(0.0f, math::Vec3f(0.0f, 0.0f, 0.0f));
		controllers[3]->getPosInterpolyator().addKey(1.0f, math::Vec3f(0.0f, 0.0f, 0.0f));
*/
		for(int i = 0; i < 4; i++)
			animation->addBone(i);

		animation->getBoneAnimation(0).addKey(0.0f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(0).addKey(0.25f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(3.14f/2.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(0).addKey(0.75f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(-3.14f/2.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(0).addKey(1.0f, CBoneState(math::Vec3f(0.0f, -CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));


		animation->getBoneAnimation(2).addKey(0.0f, CBoneState(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
		animation->getBoneAnimation(2).addKey(1.0f, CBoneState(math::Vec3f(0.0f, CYLINDER_HEIGHT*0.25f, 0.0f),
													convertEylerToQuat(math::Vec3f(0.0f, 0.0f, 0.0f)),
													math::Vec3f(1.0f, 1.0f, 1.0f)));
	}


public:
	CSkinningTest() :  m_spApp(core::IApplication::Create(L"Skinning Test", 800, 600, 32, 85, false)) /*, m_controller(ANIMATION_TIME)*/
	{
		m_spApp->addTask(core::PTask(new core::CInputTask(0, false)));
		m_spApp->addTask(core::PTask(new core::CGameTask(1)));
		m_spApp->addTask(core::PTask(new core::CRenderTask(2)));

		m_camera = render::CRenderCamera::Create();
		render::TheCameraManager::Get().addCamera(m_camera);

		render::TheRenderManager::Get().enableVolumes( true );
		render::TheRenderManager::Get().enableLighting(false);

		m_spFont = render::IFont::Create(18, L"Arial", render::IFont::Heavy);

		//
		initCamera();
		////инициализация ввода
		initInput();
		createLights();

		initSkinnedMesh();

		m_bRenderGrid = true;
//		render::TheLightManager::Get().setDebugRendering(false);

		m_spApp->Run();
	}

	
	~CSkinningTest()
	{
	}


protected:
	virtual void update(float dt)
	{
		render::TheDevice::Get().showFPS(m_spFont);

		render::TheRenderManager::Get().setFillMode(render::WireFrame);

		if(m_bRenderGrid){
			render::TheDevice::Get().showWiredFloorGrid(300.0f, 20, math::Color(150, 150, 150, 255));
			render::TheDevice::Get().showWiredFloorGrid(300.0f, 2, math::Color(60, 60, 60, 255));
		}

//		m_vAnimations[m_curAnim]->update(dt);
		m_controller->update(dt);

		size_t num = m_bones.size();

		for(size_t i = 0; i < num; i++)
			m_bones[i]->debugDraw();
	}

	//выход из программы
	void onEsc(bool)
	{
		core::IApplication::Get()->close();
	}

	void on1(bool)
	{
		blendTo(0);
	}

	void on2(bool)
	{
		blendTo(1);
	}

	void on3(bool)
	{
		blendTo(2);
	}

	//ось X
	void onXAxis(int dx)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dx>accel ? dx*fast : dx*slow;

		m_cTargetCamera.rotateLeft(-angle);
	}

	//ось Y
	void onYAxis(int dy)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = dy>accel ? dy*fast : dy*slow;

		m_cTargetCamera.rotateUp(angle);
	}

	void onWheelAxis(int i)
	{
		const float slow = .1f;
		m_cTargetCamera.goBackward(-i*slow);
	}

protected:

	PSkinnedMesh m_pMesh;
	PAnimationController m_controller;
	PSkeletonAnimation        m_vAnimations[NUM_ANIMATIONS];
	CSkeleton::Bones         m_bones;//For debug rendering

	int m_curAnim;

	render::PFont			m_spFont;
	math::PCamera			m_camera;            //указатель на камеру

	bool m_bRenderGrid;

	//данные для ввода
	input::Button       m_cEsc;
	input::Button       m_c1;
	input::Button       m_c2;
	input::Button       m_c3;
	input::RelativeAxis m_cXAxis;
	input::RelativeAxis m_cYAxis;
	input::RelativeAxis m_cZAxis;

	//данные для камеры
	math::CTargetCamera      m_cTargetCamera;      //контроллер камеры "нацеленная камера"

	std::auto_ptr<core::IApplication> m_spApp;
};


// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	CSkinningTest r;
	return 0;
}