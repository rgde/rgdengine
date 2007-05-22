#include "precompiled.h"

#include <rgde/physic/physic.h>

#pragma warning(disable : 4800)	// warning C4800: forcing value to bool 'true' or 'false' (performance warning)

//there physic begin...

#ifndef PHYSIC_CUSTOM
//bullet libs
#ifdef _DEBUG
	#pragma comment (lib, "Bullet_d.lib")
	#pragma comment (lib, "BulletDynamics_d.lib")
	#pragma comment (lib, "CcdPhysics_d.lib")
	#pragma comment (lib, "PhysicsInterfaceCommon_d.lib")

#else
	#pragma comment (lib, "Bullet.lib")
	#pragma comment (lib, "BulletDynamics.lib")
	#pragma comment (lib, "CcdPhysics.lib")
	#pragma comment (lib, "PhysicsInterfaceCommon.lib")

#endif

namespace physic
{
	CPhysicObject::CPhysicObject( void ) :
		m_pcController(0), m_IsChanged(false)
	{
		m_worldTransform.setIdentity();
	};

	CPhysicObject::~CPhysicObject( void )
	{
		//
	};

	void CPhysicObject::registerController( void )
	{
		Physic::Get().addCcdPhysicsController(m_pcController);
	};

	Matrix44f CPhysicObject::getTransform( void ) const
	{
		return m_transform;
	};

	void CPhysicObject::getWorldPosition( Point3f& pos )
	{
		setTrans(pos, m_transform);
	};
	void CPhysicObject::getWorldScaling( Vec3f& scale )
	{
		scale = Vec3f(1,1,1);
	};

	void CPhysicObject::getWorldOrientation( Quatf& rot )
	{
		setRot(rot, m_transform);
	};

	void CPhysicObject::setWorldPosition( const Point3f& pos )
	{
		//m_IsChanged = true;
		setTrans(m_transform, pos);
	};
	void CPhysicObject::setWorldOrientation( const Quatf& rot )
	{
		//m_IsChanged = true;
		setRot(m_transform, rot);
	};

	void CPhysicObject::getWorldPosition( float& posX, float& posY, float& posZ )
	{
		//getWorldPosition(Point3f(posX,posY,posZ));
		posX = m_worldTransform.getOrigin().x();
		posY = m_worldTransform.getOrigin().y();
		posZ = m_worldTransform.getOrigin().z();
	};

	void CPhysicObject::getWorldScaling( float& scaleX, float& scaleY, float& scaleZ )
	{
		//getWorldScaling(Vec3f(scaleX,scaleY,scaleZ));
		scaleX = 1.;
		scaleY = 1.;
		scaleZ = 1.;
	};

	void CPhysicObject::getWorldOrientation( float& quatIma0, float& quatIma1,
											float& quatIma2, float& quatReal )
	{
		//getWorldOrientation(Quatf(quatIma0,quatIma1,quatIma2,quatReal));
		quatIma0 = m_worldTransform.getRotation().x();
		quatIma1 = m_worldTransform.getRotation().y();
		quatIma2 = m_worldTransform.getRotation().z();
		quatReal = m_worldTransform.getRotation()[3];
	};
	
	void CPhysicObject::setWorldPosition( float posX, float posY, float posZ)
	{
		//setWorldPosition(Point3f(posX,posY,posZ));
		SimdPoint3 pos(posX,posY,posZ);
		m_worldTransform.setOrigin( pos );
	};
	void CPhysicObject::setWorldOrientation( float quatIma0, float quatIma1,
											float quatIma2, float quatReal )
	{
		//setWorldOrientation(Quatf(quatIma0,quatIma1,quatIma2,quatReal));
		SimdQuaternion orn(quatIma0,quatIma1,quatIma2,quatReal);
		m_worldTransform.setRotation( orn );		
	};
	
	void CPhysicObject::calculateWorldTransformations( void )
	{
		//m_IsChanged = false;
	};

		
		
	/** \brief Отрисовка отладочной информации средствами DX
	*/
	class DXDebugDrawer : public IDebugDraw
	{
	private:

		// can be 0-off, 1-lines or IDebugDraw::DBG_DrawContactPoints(8)
		int m_debugMode;

	public:

		DXDebugDrawer() : m_debugMode(0) {};

		virtual void	DrawLine(	const SimdVector3& from,
									const SimdVector3& to,
									const SimdVector3& color )
		{
			if (m_debugMode > 0)
			{
				math::Color mColor;
				mColor.set(color.getX(), color.getY(), color.getZ(), 1);
				render::TheLine3dManager::Get().addLine(	gmtl::Vec3f(from.getX(), from.getY(), from.getZ()),
														gmtl::Vec3f(to.getX(), to.getY(), to.getZ()),
														mColor);
			}
		};

		virtual void	DrawContactPoint(	const SimdVector3& pointOnB,
											const SimdVector3& normalOnB,
											float distance,
											int lifeTime,
											const SimdVector3& color)
		{
			if (m_debugMode & IDebugDraw::DBG_DrawContactPoints)
			{
				SimdVector3 to=pointOnB+normalOnB*distance;
				const SimdVector3&from = pointOnB;

				math::Color mColor;
				mColor.set(color.getX(), color.getY(), color.getZ(), 1);
				render::TheLine3dManager::Get().addLine(	gmtl::Vec3f(from.getX(), from.getY(), from.getZ()),
														gmtl::Vec3f(to.getX(), to.getY(), to.getZ()),
														mColor);
			
				//TODO : project point to screen & draw text - use D3DXVec3Project?
				/*
				glRasterPos3f(from.x(),  from.y(),  from.z());
				char buf[12];
				sprintf(buf," %d",lifeTime);
				BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
				*/
			}
		};

		virtual void	SetDebugMode( int debugMode ) { m_debugMode = debugMode; };
		virtual int		GetDebugMode() const { return m_debugMode;}
	};

	typedef base::TSingelton<DXDebugDrawer>	PhysicDebugDrawer;

	CPhysic::CPhysic( void ) : m_fDeltaTime(0.0f), m_iDebugPhysic(0), m_bIdle(false)
	{
		// creation
	};

	CPhysic::~CPhysic( void )
	{
		for (TVecPhysObjPtr::iterator it = m_vecObjects.begin(); it != m_vecObjects.end(); it++)
			delete *it;
	};

	void CPhysic::init( void )
	{
		// replace default solver with SimpleConstraintSolver
		setSolverType(1);

		// set defaults
		setDeactivationTime(2.f);
		setGravity(0.0f,0.0f,-10.0f);

		setDebugDrawer(&PhysicDebugDrawer::Get());
	};

	void CPhysic::update( void )
	{
		// не обновляем вспять или если ничего не произошло
		if(m_fDeltaTime<0.0f || m_bIdle)
			return;

		proceedDeltaTime(0.f, m_fDeltaTime);
	};

	void CPhysic::drawDebug( void )
	{
		// Эту операцию нужно повторять каждый цикл, иначе AABB не рисуется
		PhysicDebugDrawer::Get().SetDebugMode(m_iDebugPhysic);

		for (TVecPhysObjPtr::iterator it = m_vecObjects.begin(); it != m_vecObjects.end(); it++)
		{
			Matrix44f transform;
			Vec3f vPos;
			Quatf qRot;
			
			(*it)->getWorldPosition(vPos[0],vPos[1],vPos[2]);
			(*it)->getWorldOrientation(qRot[0],qRot[1],qRot[2],qRot[3]);
			
			setRot(transform, qRot);
			setTrans(transform, vPos);
			//Matrix44f transform((*it)->getTransform());
			

			//wants deactivation color
			math::Color mColor(0,0,255,255);

			// color differently for active, sleeping, wants deactivation states
			int state = (*(*it))()->GetRigidBody()->GetActivationState();
			
			if ( state == 1)	//active
				mColor.set((unsigned char)255, 128, 0, 255);

			if ( state == 2)	//ISLAND_SLEEPING
				mColor.set((unsigned char)0, 255, 0, 255);

			// draw shape
			CollisionShape* shape = (*(*it))()->GetRigidBody()->GetCollisionShape();
			if (shape->IsPolyhedral())
			{
				PolyhedralConvexShape* polyshape = (PolyhedralConvexShape*) shape;
				SimdPoint3 a,b;
				math::Point3f vA, vB;
				for (int j=0;j<polyshape->GetNumEdges();j++)
				{
					polyshape->GetEdge(j,a,b);
					// copy points data
					memcpy(&vA[0],&a[0],12);//sizeof(float)*3
					memcpy(&vB[0],&b[0],12);

					render::TheLine3dManager::Get().addLine(transform*vA, transform*vB, mColor);
				}
			}

			/*
			// this changes shape types
			if (m_iDebugPhysic!=0 && (i>0))
			{
				if ((*(*it))()->GetRigidBody()->GetCollisionShape()->GetShapeType() == EMPTY_SHAPE_PROXYTYPE)
				{
					(*(*it))()->GetRigidBody()->SetCollisionShape(g_shapePtr[1]);
				
					//remove the persistent collision pairs that were created based on the previous shape

					BroadphaseProxy* bpproxy = (BroadphaseProxy*)(*(*it))()->m_broadphaseHandle;

					bpproxy->SetClientObjectType((*(*it))()->GetRigidBody()->GetCollisionShape()->GetShapeType());

					Physic::Get().GetBroadphase()->CleanProxyFromPairs(bpproxy);

					SimdVector3 newinertia;
					SimdScalar newmass = 10.f;
					(*(*it))()->GetRigidBody()->GetCollisionShape()->CalculateLocalInertia(newmass,newinertia);
					(*(*it))()->GetRigidBody()->setMassProps(newmass,newinertia);
					(*(*it))()->GetRigidBody()->updateInertiaTensor();
				}
			}
			*/
		}
	};

	void CPhysic::switchDebug( void )
	{
		switch(m_iDebugPhysic) {
			case IDebugDraw::DBG_DrawAabb:			m_iDebugPhysic = IDebugDraw::DBG_DrawContactPoints;	break;
			case IDebugDraw::DBG_DrawContactPoints:	m_iDebugPhysic = 0;									break;
			default:								m_iDebugPhysic = IDebugDraw::DBG_DrawAabb;			break;
			}
	};

	CPhysicObject* CPhysic::createObject( void )
	{
		//PPhysicObject pObj(new CPhysicObject());
		CPhysicObject* pObj = new CPhysicObject();
		//m_vecObjects.push_back(pObj.get());
		m_vecObjects.push_back(pObj);
		//return pObj.get();
		return pObj;
	}
}
#endif // PHYSIC_CUSTOM

namespace physic2
{
	CPhController::CPhController( void ) : m_pData(0), m_Flags(0)
	{
	};

	void CPhController::changeWorld( PPhWorld p )
	{
		PPhWorld tmp = m_pWorld;
		m_pWorld = p;
		if(tmp)
			tmp->m_Controllers.erase(this);
	};

	CPhWorld::CPhWorld( void )
	{
		m_vGravity = Point3f(0.0f,0.0f,-10.0f);
	}

	void CPhWorld::update( float dt )
	{
		// use updater with contacts inside
		update(dt, &m_Contacts);
	}

	void CPhWorld::update( float, TPhContacts* )
	{
		// update world
	}

	CPhSystem::CPhSystem( void )  : m_fDeltaTime(0.0f), m_iDebugPhysic(0),
		m_bIdle(false), m_fDeactivationTime(2.0f)
	{
		//
	}

	void CPhSystem::init( void )
	{
		//TODO: init system
	}

	void CPhSystem::update( void )
	{
		// не обновляем вспять или если ничего не произошло
		if(m_fDeltaTime<0.0f || m_bIdle)
			return;

		//TODO: update system
	};

	void CPhSystem::drawDebug( void )
	{
		//TODO: draw debug
	};

	void CPhSystem::setDeactivationTime( float f )
	{
		m_fDeactivationTime = f;
	};

	void CPhSystem::setGravity( PPhWorld w, Point3f &pt)
	{
		w->m_vGravity = pt;
	};

	Point3f CPhSystem::getGravity( PPhWorld w )
	{
		return w->m_vGravity;
	};

	PPhWorld CPhSystem::createWorld( void )
	{
		return m_Worlds.create();
	};

	void CPhSystem::eraseWorld( PPhWorld p )
	{
		m_Worlds.erase(&*p);
	};

	void CPhSystem::cleanWorlds( void )
	{
		m_Worlds.clear();
	};

	PPhController CPhSystem::createController( PPhWorld p )
	{
		assert(p);
		PPhController pCon = p->m_Controllers.create();
		pCon->changeWorld(p);
		return pCon;
	};

	int CPhSystem::getDebug( void )
	{
		return m_iDebugPhysic;
	};

	void CPhSystem::setDebug( int dbg )
	{
		m_iDebugPhysic = dbg;
	};

	void CPhSystem::switchDebug( void )
	{
		//
	};

	void CPhSystem::reserve( unsigned num )
	{
		m_Worlds.reserve(num);
	};

	TPhWorlds* CPhSystem::getWorlds( void )
	{
		return &m_Worlds;
	};
}