/******************************************************************************
@file    collider.cpp
@author  Neonic
@mail    gaba@male.ru
@project RGDE
@date    02.04.2006
@desc    some code from [opcode v.1.3] from Pierre Terdiman
 ******************************************************************************/
//#include <rgde/engine.h>

#include "precompiled.h"

#include <rgde/render/renderMesh.h>
#include <rgde/physic/collider.h>

#include <float.h>

/*
collide concept:
	01. detect objects in global tree

	02. process objects in collider
	03. detect overlap area in AABB's

	04. grab primitives from 1st object (overlap, unique) return no collision if no prims
	05. grab primitives from 2nd object (overlap, unique) return no collision if no prims

	06. test 1st object (per primitive) with 2nd (by type)

	collision types: mesh - triangle; support utility - aabb, sphere, ray

	07. create contact pair
	08. -store pairs?
	09. -object are in collide state if stored in collided pairs?

	trick: to solve pass-throught problem (ODE bypass this - use fixed range) try to
	test convex hull with 2 AABB's (start/end). see qhull

algo support usage (sphere\cam):
	01. create collision object
	02. add data & flags
	03. call collider in update collision objects (see update query theme)
	04. grab area in global tree
	05. detect overlap
	06. grab primitive from 1st (sphere data)
	07. grab primitives from 2nd
	08. test 1st with second (per primitive)

	09. create event...

	10. see events. if exist - move sphere\cam back to last allowed (contact) position

update query theme:
	1. update model animations, object moves, physic forces, joints. [reflect updated pos]
	2. update global scene tree with queryes [reflect updated pos after movements for test]
	3. process collider queryes [detect collided objects].
	   generate events with contact pairs.
	4. process game/programmed area.
	   browse events for collided objects, do responses, cleanup unneeded events
	5. update physic. process physic simulation for rigid objects.

motors?: cars,mechanics
schemes for connections? (convert animated model to rag doll or something)



	Смысл существования дерева будет только в том случае, если число проверок, необходимое
	для осуществления выбора, будет существенно ниже (в разы) исходного числа проверок,
	производимого на основе примитивов опрашиваемых объектов.

	Вторая основа существования дерева заключена в его объеме. Если объем создаваемого
	дерева превышает объем исходного материала в разы - то это явный перерасход ресурсов,
	делающий существование дерева бессмысленным. Сюда идут методы воксельной детализации,
	где точность предоставлена хоть и двоичным, но чересчур подробным (октарным) деревом.
	Ограничение объемом - дерево не должно занимать места больше чем в три раза от
	оригинального объема.

	Броское название живого дерева на самом деле означает концепт положения объектов,
	динамичный для двигающихся объектов и статичный для недвижимых.
*/

namespace collision
{
	typedef render::CMesh						TMesh;
	typedef render::CMesh::PGeometry			TGeom;
//	typedef render::TIndexedGeometry<vertex::MeshVertex, false>::Vertexes	TVerts;
//	typedef render::TIndexedGeometry<vertex::MeshVertex, false>::Indexes	TInds;
	typedef render::CMesh::Geometry::Vertexes	TVerts;
	typedef render::CMesh::Geometry::Indexes	TInds;

	////////////////////////////////////////////////////////////////////////
	// 
	// Suport
	//	some helper functions to reduce code massive
	////////////////////////////////////////////////////////////////////////

	//! Use CPU comparisons (comment that line to use standard FPU compares)
	#define OPC_CPU_COMPARE
	
	//! Use epsilon value in tri-tri overlap test
	#define OPC_TRITRI_EPSILON_TEST

	//! Use FCOMI / FCMOV on Pentium-Pro based processors (comment that line to use plain C++)
	#define OPC_USE_FCOMI


	// operator| for Point3f
	inline float DotProd(const Point3f& p0, const Point3f& p1)
	{
		return p0[0]*p1[0] + p0[1]*p1[1] + p0[2]*p1[2];
	};

	// operator^ for Point3f
	inline Point3f VecProd(const Point3f& p0, const Point3f& p1)
	{
		return Point3f(	p0[1] * p1[2] - p0[2] * p1[1],
						p0[2] * p1[0] - p0[0] * p1[2],
						p0[0] * p1[1] - p0[1] * p1[0] );
	};

	//! Computes square distance to another point
	inline float SquareDistance(const Point3f& p0, const Point3f& p1)
	{
		return (  (p0[0] - p1[0])*(p0[0] - p1[0])
				+ (p0[1] - p1[1])*(p0[0] - p1[1])
				+ (p0[2] - p1[2])*(p0[2] - p1[2]));
	}

	//! Computes square magnitude
	inline float SquareMagnitude(const Point3f& p)
	{
		return p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
	};

	#define FCOMI_ST1	_asm	_emit	0xdb	_asm	_emit	0xf1
	#define FCOMIP_ST1	_asm	_emit	0xdf	_asm	_emit	0xf1
	#define FCMOVB_ST1	_asm	_emit	0xda	_asm	_emit	0xc1
	#define FCMOVNB_ST1	_asm	_emit	0xdb	_asm	_emit	0xc1

	#define FCOMI_ST2	_asm	_emit	0xdb	_asm	_emit	0xf2
	#define FCOMIP_ST2	_asm	_emit	0xdf	_asm	_emit	0xf2
	#define FCMOVB_ST2	_asm	_emit	0xda	_asm	_emit	0xc2
	#define FCMOVNB_ST2	_asm	_emit	0xdb	_asm	_emit	0xc2

	//! A global function to find MAX(a,b,c) using FCOMI/FCMOV
	inline float FCMax3(float a, float b, float c)
	{
		float Res;
		_asm	fld		[a]
		_asm	fld		[b]
		_asm	fld		[c]
		FCOMI_ST1
		FCMOVB_ST1
		FCOMI_ST2
		FCMOVB_ST2
		_asm	fstp	[Res]
		_asm	fcompp
		return Res;
	}

	//! A global function to find MIN(a,b,c) using FCOMI/FCMOV
	inline float FCMin3(float a, float b, float c)
	{
		float Res;
		_asm	fld		[a]
		_asm	fld		[b]
		_asm	fld		[c]
		FCOMI_ST1
		FCMOVNB_ST1
		FCOMI_ST2
		FCMOVNB_ST2
		_asm	fstp	[Res]
		_asm	fcompp
		return Res;
	}

	//! if OPC_TRITRI_EPSILON_TEST is true then we do a check (if |dv|<EPSILON then dv=0.0;) else no check is done (which is less robust, but faster)
	#define LOCAL_EPSILON 0.000001f

	//! Integer representation of a floating-point value.
	#define IR(x)					((unsigned&)(x))

	//! Absolute integer representation of a floating-point value
	#define AIR(x)					(IR(x)&0x7fffffff)

	#define IEEE_1_0				0x3f800000					//!< integer representation of 1.0

	#ifdef OPC_CPU_COMPARE
		#define GREATER(x, y)	AIR(x) > IR(y)
	#else
		#define GREATER(x, y)	fabsf(x) > (y)
	#endif

	//! Integer-based comparison of a floating point value.
	//! Don't use it blindly, it can be faster or slower than the FPU comparison, depends on the context.
	#define IS_NEGATIVE_FLOAT(x)	(IR(x)&0x80000000)

	//Neonic: produce overlap area AABB between 2 AABB
	void makeOverlapAABB( const AABoxf& _1, const AABoxf& _2, AABoxf& result )
	{
		// if AABBs overlap - at least 1 point are in another volume
		for( int i=0; i<3; i++ )
		{
			Point3f current;
			{
				current = result.getMin();
				current[i]=_1.getMin()[i]>_2.getMin()[i] ? _1.getMin()[i] : _2.getMin()[i];
				result.setMin(current);
			}
			{
				current = result.getMax();
				current[i]=_1.getMax()[i]<_2.getMax()[i] ? _1.getMax()[i] : _2.getMax()[i];
				result.setMax(current);
			}
		}
		// Neonic: correct overlapped AABB produced only if overlapped AABB got on enter. near result of incorrect AABB's
		/*
		sph = pos(-6,-4,-8) + radius(1)
		tri = pos(0,0,0) + ext(100,100,0)
		overlap = min(-7,-5,0) + max(-5,-3,-7)
		cause
		min				max
		[-7] > -100		[-5] < -100
		[-5] > -100		[-3] < -100
		-9   > [0]		[-7] < 0
		last line is incorrect for true AABB.
		so it mean what AABBs not overlapping
		*/
	};

	AABoxf& calcAABB( const Matrix44f& m, AABoxf& result, const AABoxf& source )
	{
		// calc global AABB from OOB

		// in case &result is == &source... :(
		float	Xmax = source.getMax()[0],
				Ymax = source.getMax()[1],
				Zmax = source.getMax()[2],
				Xmin = source.getMin()[0],
				Ymin = source.getMin()[1],
				Zmin = source.getMin()[2];
	//(MAX/min)
//		for( int iX =0; iX<2; iX++)
//			for( int iY =0; iY<2; iY++)
//				for( int iZ =0; iZ<2; iZ++)
//				{
//					Point3f pt( iX? src.getMax()[0] : src.getMin()[0],
//								iY? src.getMax()[1] : src.getMin()[1],
//								iZ? src.getMax()[2] : src.getMin()[2] );
//					extendVolume(result,m*pt);
//				}

		result = AABoxf();	// identity
		extendVolume(result,m*Point3f(Xmax,Ymax,Zmax));	// XYZ
		extendVolume(result,m*Point3f(Xmax,Ymax,Zmin));	// XYz
		extendVolume(result,m*Point3f(Xmax,Ymin,Zmax));	// XyZ
		extendVolume(result,m*Point3f(Xmax,Ymin,Zmin));	// Xyz
		extendVolume(result,m*Point3f(Xmin,Ymax,Zmax));	// xYZ
		extendVolume(result,m*Point3f(Xmin,Ymax,Zmin));	// xYz
		extendVolume(result,m*Point3f(Xmin,Ymin,Zmax));	// xyZ
		extendVolume(result,m*Point3f(Xmin,Ymin,Zmin));	// xyz

		return result;
	};

	AABoxf& calcAABB( const Matrix44f& m, AABoxf& aabb )
	{
		return calcAABB(m,aabb,aabb);
	};

	////////////////////////////////////////////////////////////////////////
	// 
	// CIndexTriTreeRoot
	// 
	////////////////////////////////////////////////////////////////////////
	
	CIndexTriTreeRoot::CIndexTriTreeRoot( unsigned _ext, unsigned _base, int LimitDivisions )
		: CTRoot(_ext,_base,LimitDivisions)
	{
		// nothing
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// CCollisionObject
	// 
	////////////////////////////////////////////////////////////////////////

	CCollisionObject::CCollisionObject( void )
		: m_Primitive(ECP_UNKNOWN), m_pMesh(0)
	{
	};

	CCollisionObject::~CCollisionObject( void )
	{
		clearLocal();
	};

//	#include "strsafe.h"

	bool CCollisionObject::createLocal( render::CMesh* _pMesh, const Point3f& ext, unsigned base )
	{
		int side=0;
		for(unsigned i=0; i<3; i++)
			if(ext[i]>side)
				side = ext[i];
		return createLocal(_pMesh, side, base);
	};

	bool CCollisionObject::createLocal( render::CMesh* _pMesh, const float& side, unsigned base )
	{
		assert(_pMesh);
		clearLocal();

		m_pMesh = _pMesh;

		int extent;
		calcProgression(side, extent);

		m_pLocalTree = new CIndexTriTreeRoot(extent,base);
		setPrim(ECP_TRIANGLE);

		// create tree structure from mesh
		TMesh *pMesh = getMesh();
		const TGeom geom = pMesh->getGeometry();
		const TVerts &verts = geom->lockVB();
		const TInds &inds = geom->lockIB();
		unsigned limit_divisions = 8, divisions = 0;

		if(inds.size())
		{
			unsigned total = pMesh->getNumPrimitives();
			// allocate massive once
//			CIndexTriTreeObject* objects = new CIndexTriTreeObject[total];
			for( size_t index=0; index<total; index++ )
			{
				//CIndexTriTreeObject* pObj = new CIndexTriTreeObject(index);
//				CIndexTriTreeObject* pObj = &objects[index];
				PIndexTriTreeObject pObj(new CIndexTriTreeObject());
				unsigned curr = (unsigned)(index*3);
				pObj->setIndex(curr);
				AABoxf aabb;
				for(int tri =0; tri<3; tri++)
					extendVolume(aabb,(Point3f)verts[inds[curr+tri]].position);
				pObj->setAABB(aabb);
				getLocal()->inject(pObj);
			}

	//		base::lmsg << "inserted " << inds.size() << " objects...";
		}
		return 1;
	};

	void CCollisionObject::clearLocal( void )
	{
		if(m_pLocalTree.get())
		{
			m_pLocalTree = 0;
			m_pMesh = 0;
			setPrim(ECP_UNKNOWN);
		}
	};

	void CCollisionObject::makeSphere( const Point3f& center, const float& radius )
	{
		clearLocal();
		m_Primitive = ECP_SPHERE;
		setPos(center);
		setRadius(radius);
	};

	void CCollisionObject::makeSphere( const Spheref& sphere )
	{
		clearLocal();
		m_Primitive = ECP_SPHERE;
		setPos(sphere.getCenter());
		setRadius(sphere.getRadius());
	};

	void CCollisionObject::setRadius( float radius )
	{
		setExt(Point3f(radius,radius,radius));
	};

	float CCollisionObject::getRadius( void )
	{
		return getExt()[0];
	};

	void CCollisionObject::makeAABB( const Point3f& cnt, const Point3f& ext )
	{
		clearLocal();
		m_Primitive = ECP_AABB;
		setPos(cnt);
		setExt(ext);
	};

	void CCollisionObject::makeRay( const Point3f& origin, const Point3f& dir )
	{
		clearLocal();
		m_Primitive = ECP_RAY;
		setPos(origin);
		setDir(dir);
	};

	void CCollisionObject::makeRay( const Rayf& ray )
	{
		clearLocal();
		m_Primitive = ECP_RAY;
		setPos(ray.getOrigin());
		setDir(ray.getDir());
	};

	void CCollisionObject::setDir( const Point3f& dir )
	{
		m_Sup=dir;
	};
	
	const Point3f& CCollisionObject::getDir( void )
	{
		return m_Sup;
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// CCollider
	// 
	////////////////////////////////////////////////////////////////////////

	CCollider::CCollider( void )
		: m_iDiscreteTestLevel(3) // [level-num_of_tests]:[0-0][1-1][2-3][3-7]...
		, m_iPrimPrimTests(0)
		, m_iVolumePrimTests(0)
	{
		// 
	};

	void CCollider::resetStat( void )
	{
		m_iPrimPrimTests = 0;
		m_iVolumePrimTests = 0;	
	};

	void CCollider::CollideWorld( CDynamicTreeRoot* p )
	{
		// sure we got something to test
		assert(p);
		// sure we test is after
		assert(p->isApproximated());

		Array dynamics = p->getDynamicObjects();

		if(!dynamics.size())
			return;

		// collide objects one by one.
		// TODO: possible error - object can touched twice (if both collided object in move)
		for(Array::iterator it=dynamics.begin(); it!=dynamics.end(); it++)
		{
			//Neonic: bad bad glue - possible casting errors - (CCollisionObject*) cast
			CollideGlobal( *((CCollisionObject*)(*it).get()), p );
		}
	};

	bool CCollider::CollideGlobal( CCollisionObject& obj, CTRoot* p)
	{
		// sure we got something to test
		if(!p)
			p=obj.getRoot();

		assert(p);

		CTRoot::CCollector coll(p,obj.getDynamicAABB(),&obj);
		coll();

//		// sort all objects, then remove duplicates
//		if(vecObj.size()) {
//			std::sort(vecObj.begin(), vecObj.end());
//			vecObj.erase(std::unique(vecObj.begin(), vecObj.end()),vecObj.end());
//			vecObj.erase(std::remove(vecObj.begin(), vecObj.end(), &obj), vecObj.end());
//		}
		if(!coll.m_Array.size())
			return 0;

		for(Array::iterator it=coll.m_Array.begin(); it!=coll.m_Array.end(); it++)
		{
			//Neonic: bad bad glue - possible casting errors - (CCollisionObject*) cast
			// Neonic: next bad glue
			if( Collide(obj, *((CCollisionObject*)(*it).get())) )
				return 1;
		}
		return 0;
	};

	// Neonic: Collide
	bool CCollider::Collide( CCollisionObject& _1, CCollisionObject& _2 )
	{
	#ifdef _DEBUG
		if(_2.getPrim()==ECP_TRIANGLE)
		{
			assert(_2.getLocal().get());
			assert(_2.getMesh());
		}
	#endif

		// Тест пересечения динамических AABB объектов - устранение ошибки коллектора
		if( !OverlapAABBs( _1.getDynamicAABB(), _2.getDynamicAABB()) )
			return 0;

		if(getDTL()==0)
		{
			// Neonic: simple collision here
			return 1;
		}

		pos1	= _1.getPos();
		pos2	= _2.getPos();
		pos11	= _1.getOldPos();
		pos22	= _2.getOldPos();
		ext1	= _1.getExt();
		ext2	= _2.getExt();

		AABoxf box1(pos11-ext1,pos11+ext1);
		AABoxf box2(pos22-ext2,pos22+ext2);

		// Тест пересечения AABB объектов в движении
		if( !intersect(	box1, (Vec3f)pos1,
						box2, (Vec3f)pos2,
						firstContact, secondContact) )
			return 0;

		if(getDTL()==1)
		{
			// Neonic: simple collision here
			return 1;
		}

		// Вычисляем текущее значение для циклического теста столкновения
		float middle = (firstContact+secondContact)*0.5f;

		switch(_1.getPrim())
		{
	/*	case ECP_TRIANGLE:
	#ifdef _DEBUG
			assert(_1.getLocal());
			assert(_1.getMesh());
	#endif
			switch(_2.getPrim())
			{
			case ECP_TRIANGLE:	return CollideTriTri(_1,_2);
			case ECP_SPHERE:	return CollideSphTri(_2,_1);
			case ECP_AABB:		return CollideTriAABB(_1,_2);
			case ECP_RAY:		return CollideRayTri(_2,_1);
			}
			break;												*/

		case ECP_SPHERE:
			switch(_2.getPrim())
			{
			case ECP_TRIANGLE:
				// calc inversion matrix for reverse operations at space of second object OBB
				gmtl::invert(mat1, _2.getMesh()->getFullTransform());
				return CollideSphTri( _1, _2, middle );
	/*		case ECP_SPHERE:	return CollideSphSph(_1,_2);
			case ECP_AABB:		return CollideSphAABB(_1,_2);
			case ECP_RAY:		return CollideRaySph(_2,_1);	*/
			}
			break;

	/*	case ECP_AABB:
			switch(_2.getPrim())
			{
			case ECP_TRIANGLE:	return CollideTriAABB(_2,_1);
			case ECP_SPHERE:	return CollideSphAABB(_2,_1);
			case ECP_AABB:		return CollideAABBS(_1,_2);
			case ECP_RAY:		return CollideRayAABB(_2,_1);
			}
			break;

		case ECP_RAY:
			switch(_2.getPrim())
			{
			case ECP_TRIANGLE:	return CollideRayTri(_1,_2);
			case ECP_SPHERE:	return CollideRaySph(_1,_2);
			case ECP_AABB:		return CollideRayAABB(_1,_2);
			//Neonic: ray never collide with another ray
			case ECP_RAY:		return 0;
	
			}
			break;												*/
		}
		// got an error... stop work. report... some error in primitive assign process...
		assert(0);

		return 0;
	};

	//! sort so that a<=b
	#define SORT(a,b)			\
		if(a>b)					\
		{						\
			const float c=a;	\
			a=b;				\
			b=c;				\
		}

	//! Edge to edge test based on Franlin Antonio's gem: "Faster Line Segment Intersection", in Graphics Gems III, pp. 199-202
	#define EDGE_EDGE_TEST(V0, U0, U1)						\
		Bx = U0[i0] - U1[i0];								\
		By = U0[i1] - U1[i1];								\
		Cx = V0[i0] - U0[i0];								\
		Cy = V0[i1] - U0[i1];								\
		f  = Ay*Bx - Ax*By;									\
		d  = By*Cx - Bx*Cy;									\
		if((f>0.0f && d>=0.0f && d<=f) || (f<0.0f && d<=0.0f && d>=f))	\
		{													\
			const float e=Ax*Cy - Ay*Cx;					\
			if(f>0.0f)										\
			{												\
				if(e>=0.0f && e<=f) return 1;				\
			}												\
			else											\
			{												\
				if(e<=0.0f && e>=f) return 1;				\
			}												\
		}

	//! TO BE DOCUMENTED
	#define EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2)		\
	{														\
		float Bx,By,Cx,Cy,d,f;								\
		const float Ax = V1[i0] - V0[i0];					\
		const float Ay = V1[i1] - V0[i1];					\
		/* test edge U0,U1 against V0,V1 */					\
		EDGE_EDGE_TEST(V0, U0, U1);							\
		/* test edge U1,U2 against V0,V1 */					\
		EDGE_EDGE_TEST(V0, U1, U2);							\
		/* test edge U2,U1 against V0,V1 */					\
		EDGE_EDGE_TEST(V0, U2, U0);							\
	}

	//! TO BE DOCUMENTED
	#define POINT_IN_TRI(V0, U0, U1, U2)					\
	{														\
		/* is T1 completly inside T2? */					\
		/* check if V0 is inside tri(U0,U1,U2) */			\
		float a  = U1[i1] - U0[i1];							\
		float b  = -(U1[i0] - U0[i0]);						\
		float c  = -a*U0[i0] - b*U0[i1];					\
		float d0 = a*V0[i0] + b*V0[i1] + c;					\
															\
		a  = U2[i1] - U1[i1];								\
		b  = -(U2[i0] - U1[i0]);							\
		c  = -a*U1[i0] - b*U1[i1];							\
		const float d1 = a*V0[i0] + b*V0[i1] + c;			\
															\
		a  = U0[i1] - U2[i1];								\
		b  = -(U0[i0] - U2[i0]);							\
		c  = -a*U2[i0] - b*U2[i1];							\
		const float d2 = a*V0[i0] + b*V0[i1] + c;			\
		if(d0*d1>0.0f)										\
		{													\
			if(d0*d2>0.0f) return 1;						\
		}													\
	}

	//! TO BE DOCUMENTED
	bool CoplanarTriTri(	const Point3f& n,
							const Point3f& v0, const Point3f& v1, const Point3f& v2,
							const Point3f& u0, const Point3f& u1, const Point3f& u2)
	{
		float A[3];
		short i0,i1;
		/* first project onto an axis-aligned plane, that maximizes the area */
		/* of the triangles, compute indices: i0,i1. */
		A[0] = fabsf(n[0]);
		A[1] = fabsf(n[1]);
		A[2] = fabsf(n[2]);
		if(A[0]>A[1])
		{
			if(A[0]>A[2])
			{
				i0=1;      /* A[0] is greatest */
				i1=2;
			}
			else
			{
				i0=0;      /* A[2] is greatest */
				i1=1;
			}
		}
		else   /* A[0]<=A[1] */
		{
			if(A[2]>A[1])
			{
				i0=0;      /* A[2] is greatest */
				i1=1;
			}
			else
			{
				i0=0;      /* A[1] is greatest */
				i1=2;
			}
		}

		/* test all edges of triangle 1 against the edges of triangle 2 */
		EDGE_AGAINST_TRI_EDGES(v0, v1, u0, u1, u2);
		EDGE_AGAINST_TRI_EDGES(v1, v2, u0, u1, u2);
		EDGE_AGAINST_TRI_EDGES(v2, v0, u0, u1, u2);

		/* finally, test if tri1 is totally contained in tri2 or vice versa */
		POINT_IN_TRI(v0, u0, u1, u2);
		POINT_IN_TRI(u0, v0, v1, v2);

		return 0;
	};

	//! TO BE DOCUMENTED
	#define NEWCOMPUTE_INTERVALS(VV0, VV1, VV2, D0, D1, D2, D0D1, D0D2, A, B, C, X0, X1)	\
	{																						\
		if(D0D1>0.0f)																		\
		{																					\
			/* here we know that D0D2<=0.0 */												\
			/* that is D0, D1 are on the same side, D2 on the other or on the plane */		\
			A=VV2; B=(VV0 - VV2)*D2; C=(VV1 - VV2)*D2; X0=D2 - D0; X1=D2 - D1;				\
		}																					\
		else if(D0D2>0.0f)																	\
		{																					\
			/* here we know that d0d1<=0.0 */												\
			A=VV1; B=(VV0 - VV1)*D1; C=(VV2 - VV1)*D1; X0=D1 - D0; X1=D1 - D2;				\
		}																					\
		else if(D1*D2>0.0f || D0!=0.0f)														\
		{																					\
			/* here we know that d0d1<=0.0 or that D0!=0.0 */								\
			A=VV0; B=(VV1 - VV0)*D0; C=(VV2 - VV0)*D0; X0=D0 - D1; X1=D0 - D2;				\
		}																					\
		else if(D1!=0.0f)																	\
		{																					\
			A=VV1; B=(VV0 - VV1)*D1; C=(VV2 - VV1)*D1; X0=D1 - D0; X1=D1 - D2;				\
		}																					\
		else if(D2!=0.0f)																	\
		{																					\
			A=VV2; B=(VV0 - VV2)*D2; C=(VV1 - VV2)*D2; X0=D2 - D0; X1=D2 - D1;				\
		}																					\
		else																				\
		{																					\
			/* triangles are coplanar */													\
			return CoplanarTriTri(N1, tri0[0], tri0[1], tri0[2], tri1[0], tri1[1], tri1[2]);\
		}																					\
	}

	//Neonic: perform tests with opcode 1.3 [BOOL AABBTreeCollider::TriTriOverlap]
	bool CCollider::OverlapTriTri(	const Trif& tri0, const Trif& tri1 )
	{
		// Stats
//		mNbPrimPrimTests++;

		// Compute plane equation of triangle0
		Point3f E1 = tri0[1] - tri0[0];
		Point3f E2 = tri0[2] - tri0[0];
		const Point3f N1 = VecProd(E1,E2);
		const float d1 =- DotProd(N1,tri0[0]);
		// Plane equation 1: N1.X+d1=0

		// Put tri1[0],tri1[1],tri1[2] into plane equation 1 to compute signed distances to the plane
		float du0 = (DotProd(N1,tri1[0])) + d1;
		float du1 = (DotProd(N1,tri1[1])) + d1;
		float du2 = (DotProd(N1,tri1[2])) + d1;

		// Coplanarity robustness check
	#ifdef OPC_TRITRI_EPSILON_TEST
		if(fabsf(du0)<LOCAL_EPSILON) du0 = 0.0f;
		if(fabsf(du1)<LOCAL_EPSILON) du1 = 0.0f;
		if(fabsf(du2)<LOCAL_EPSILON) du2 = 0.0f;
	#endif

		const float du0du1 = du0 * du1;
		const float du0du2 = du0 * du2;

		if(du0du1>0.0f && du0du2>0.0f)	// same sign on all of them + not equal 0 ?
			return 0;				// no intersection occurs

		// Compute plane of triangle (tri1[0],tri1[1],tri1[2])
		E1 = tri1[1] - tri1[0];
		E2 = tri1[2] - tri1[0];
		const Point3f N2 = VecProd(E1,E2);
		const float d2=- DotProd(N2,tri1[0]);
		// plane equation 2: N2.X+d2=0

		// put tri0[0],tri0[1],tri0[2] into plane equation 2
		float dv0 = (DotProd(N2,tri0[0])) + d2;
		float dv1 = (DotProd(N2,tri0[1])) + d2;
		float dv2 = (DotProd(N2,tri0[2])) + d2;

	#ifdef OPC_TRITRI_EPSILON_TEST
		if(fabsf(dv0)<LOCAL_EPSILON) dv0 = 0.0f;
		if(fabsf(dv1)<LOCAL_EPSILON) dv1 = 0.0f;
		if(fabsf(dv2)<LOCAL_EPSILON) dv2 = 0.0f;
	#endif

		const float dv0dv1 = dv0 * dv1;
		const float dv0dv2 = dv0 * dv2;

		if(dv0dv1>0.0f && dv0dv2>0.0f)	// same sign on all of them + not equal 0 ?
			return 0;				// no intersection occurs

		// Compute direction of intersection line
		const Point3f D = VecProd(N1,N2);

		// Compute and index to the largest component of D
		float max=fabsf(D[0]);
		short index=0;
		float bb=fabsf(D[1]);
		float cc=fabsf(D[2]);
		if(bb>max) max=bb,index=1;
		if(cc>max) max=cc,index=2;

		// This is the simplified projection onto L
		const float vp0 = tri0[0][index];
		const float vp1 = tri0[1][index];
		const float vp2 = tri0[2][index];

		const float up0 = tri1[0][index];
		const float up1 = tri1[1][index];
		const float up2 = tri1[2][index];

		// Compute interval for triangle 1
		float a,b,c,x0,x1;
		NEWCOMPUTE_INTERVALS(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,a,b,c,x0,x1);

		// Compute interval for triangle 2
		float d,e,f,y0,y1;
		NEWCOMPUTE_INTERVALS(up0,up1,up2,du0,du1,du2,du0du1,du0du2,d,e,f,y0,y1);

		const float xx=x0*x1;
		const float yy=y0*y1;
		const float xxyy=xx*yy;

		float isect1[2], isect2[2];

		float tmp=a*xxyy;
		isect1[0]=tmp+b*x1*yy;
		isect1[1]=tmp+c*x0*yy;

		tmp=d*xxyy;
		isect2[0]=tmp+e*xx*y1;
		isect2[1]=tmp+f*xx*y0;

		SORT(isect1[0],isect1[1]);
		SORT(isect2[0],isect2[1]);

		if(isect1[1]<isect2[0] || isect2[1]<isect1[0])
			return 0;
		return 1;
	};

	//Neonic: perform tests with opcode 1.3 [Sphere:: Intersect(const Sphere& sphere)]
	bool CCollider::OverlapSphSph( const Spheref& s0, const Spheref& s1 )
	{
		float r = s0.getRadius() + s0.getRadius();
		return SquareDistance(s0.getCenter(),s1.getCenter()) <= r*r;
	};

	bool CCollider::OverlapAABBs(	const Point3f& cnt0, const Point3f& ext0,
									const Point3f& cnt1, const Point3f& ext1 )
	{
		return intersect(AABoxf(cnt0-ext0, cnt0+ext0),AABoxf(cnt1-ext1, cnt1+ext1));
	};

	bool CCollider::OverlapAABBs(	const AABoxf& aabb0, const AABoxf& aabb1 )
	{
		return intersect(aabb0,aabb1);
	};

	//Neonic: perform tests with opcode 1.3 [AABB:: Intersect(const AABB& a)] & points types
	/*
	bool CCollider::OverlapAABBs(	const Point3f& c0, const Point3f& e0,
									const Point3f& c1, const Point3f& e1)
	{
		// test on point center+extents
		float tx = c0[0] - c1[0];	float ex = e0[0] + e1[0];	if(AIR(tx) > IR(ex))	return 0;
		float ty = c0[1] - c1[1];	float ey = e0[1] + e1[1];	if(AIR(ty) > IR(ey))	return 0;
		float tz = c0[2] - c1[2];	float ez = e0[2] + e1[2];	if(AIR(tz) > IR(ez))	return 0;
		return 1;
	};
	*/

	//Neonic: perform tests with opcode 1.3 BOOL SphereCollider::SphereTriOverlap]
	bool CCollider::OverlapSphTri(	const Spheref& sph, const Trif& tri )
	{
		// Stats
//		mNbVolumePrimTests++;

		// - Precompute R^2
		float mRadius2 = sph.getRadius() * sph.getRadius();

		// Early exit if one of the vertices is inside the sphere
		Point3f kDiff = tri[2] - sph.getCenter();
		float fC = SquareMagnitude(kDiff);
		if(fC <= mRadius2)
			return 1;

		kDiff = tri[1] - sph.getCenter();
		fC =  SquareMagnitude(kDiff);
		if(fC <= mRadius2)
			return 1;

		kDiff = tri[0] - sph.getCenter();
		fC = SquareMagnitude(kDiff);
		if(fC <= mRadius2)
			return 1;

		// Else do the full distance test
		Point3f TriEdge0 = tri[1] - tri[0];
		Point3f TriEdge1 = tri[2] - tri[0];

		float fA00	= SquareMagnitude(TriEdge0);
		float fA01	= DotProd(TriEdge0,TriEdge1);
		float fA11	= SquareMagnitude(TriEdge1);
		float fB0	= DotProd(kDiff,TriEdge0);
		float fB1	= DotProd(kDiff,TriEdge1);

		float fDet	= fabsf(fA00*fA11 - fA01*fA01);
		float u		= fA01*fB1-fA11*fB0;
		float v		= fA01*fB0-fA00*fB1;
		float SqrDist;

		if(u + v <= fDet)
		{
			if(u < 0.0f)
			{
				if(v < 0.0f)  // region 4
				{
					if(fB0 < 0.0f)
					{
						if(-fB0>=fA00)			{ 					SqrDist = fA00+2.0f*fB0+fC;	}
						else					{ u = -fB0/fA00;	SqrDist = fB0*u+fC;			}
					}
					else
					{
						if(fB1>=0.0f)			{					SqrDist = fC;				}
						else if(-fB1>=fA11)		{					SqrDist = fA11+2.0f*fB1+fC;	}
						else					{ v = -fB1/fA11;	SqrDist = fB1*v+fC;			}
					}
				}
				else  // region 3
				{
					if(fB1>=0.0f)				{					SqrDist = fC;				}
					else if(-fB1>=fA11)			{					SqrDist = fA11+2.0f*fB1+fC;	}
					else						{ v = -fB1/fA11;	SqrDist = fB1*v+fC;			}
				}
			}
			else if(v < 0.0f)  // region 5
			{
				if(fB0>=0.0f)					{					SqrDist = fC;				}
				else if(-fB0>=fA00)				{					SqrDist = fA00+2.0f*fB0+fC;	}
				else							{ u = -fB0/fA00;	SqrDist = fB0*u+fC;			}
			}
			else  // region 0
			{
				// minimum at interior point
				if(fDet==0.0f)
				{
					SqrDist = FLT_MAX;
				}
				else
				{
					float fInvDet = 1.0f/fDet;
					u *= fInvDet;
					v *= fInvDet;
					SqrDist = u*(fA00*u+fA01*v+2.0f*fB0) + v*(fA01*u+fA11*v+2.0f*fB1)+fC;
				}
			}
		}
		else
		{
			float fTmp0, fTmp1, fNumer, fDenom;

			if(u < 0.0f)  // region 2
			{
				fTmp0 = fA01 + fB0;
				fTmp1 = fA11 + fB1;
				if(fTmp1 > fTmp0)
				{
					fNumer = fTmp1 - fTmp0;
					fDenom = fA00-2.0f*fA01+fA11;
					if(fNumer >= fDenom)
					{
						SqrDist = fA00+2.0f*fB0+fC;
					}
					else
					{
						u = fNumer/fDenom;
						v = 1.0f - u;
						SqrDist = u*(fA00*u+fA01*v+2.0f*fB0) + v*(fA01*u+fA11*v+2.0f*fB1)+fC;
					}
				}
				else
				{
					if(fTmp1 <= 0.0f)		{					SqrDist = fA11+2.0f*fB1+fC;	}
					else if(fB1 >= 0.0f)	{					SqrDist = fC;				}
					else					{ v = -fB1/fA11;	SqrDist = fB1*v+fC;			}
				}
			}
			else if(v < 0.0f)  // region 6
			{
				fTmp0 = fA01 + fB1;
				fTmp1 = fA00 + fB0;
				if(fTmp1 > fTmp0)
				{
					fNumer = fTmp1 - fTmp0;
					fDenom = fA00-2.0f*fA01+fA11;
					if(fNumer >= fDenom)
					{
						SqrDist = fA11+2.0f*fB1+fC;
					}
					else
					{
						v = fNumer/fDenom;
						u = 1.0f - v;
						SqrDist = u*(fA00*u+fA01*v+2.0f*fB0) + v*(fA01*u+fA11*v+2.0f*fB1)+fC;
					}
				}
				else
				{
					if(fTmp1 <= 0.0f)		{					SqrDist = fA00+2.0f*fB0+fC;	}
					else if(fB0 >= 0.0f)	{					SqrDist = fC;				}
					else					{ u = -fB0/fA00;	SqrDist = fB0*u+fC;			}
				}
			}
			else  // region 1
			{
				fNumer = fA11 + fB1 - fA01 - fB0;
				if(fNumer <= 0.0f)
				{
					SqrDist = fA11+2.0f*fB1+fC;
				}
				else
				{
					fDenom = fA00-2.0f*fA01+fA11;
					if(fNumer >= fDenom)
					{
						SqrDist = fA00+2.0f*fB0+fC;
					}
					else
					{
						u = fNumer/fDenom;
						v = 1.0f - u;
						SqrDist = u*(fA00*u+fA01*v+2.0f*fB0) + v*(fA01*u+fA11*v+2.0f*fB1)+fC;
					}
				}
			}
		}

		return fabsf(SqrDist) < mRadius2;
	};

	//Neonic: perform tests with opcode 1.3 [SphereCollider::SphereAABBOverlap]
	bool CCollider::OverlapSphAABB(	const Spheref& sph, const Point3f& cnt, const Point3f& ext )
	{
		// Stats
//		mNbVolumeBVTests++;

		// - Precompute R^2
		float mRadius2 = sph.getRadius() * sph.getRadius();

		float d = 0.0f;

		float tmp,s;

		tmp = sph.getCenter()[0] - cnt[0];
		s = tmp + ext[0];

		if(s<0.0f)
		{
			d += s*s;
			if(d>mRadius2)
				return 0;
		}
		else
		{
			s = tmp - ext[0];
			if(s>0.0f)
			{
				d += s*s;
				if(d>mRadius2)
					return 0;
			}
		}

		tmp = sph.getCenter()[1] - cnt[1];
		s = tmp + ext[1];

		if(s<0.0f)
		{
			d += s*s;
			if(d>mRadius2)
				return 0;
		}
		else
		{
			s = tmp - ext[1];
			if(s>0.0f)
			{
				d += s*s;
				if(d>mRadius2)
					return 0;
			}
		}

		tmp = sph.getCenter()[2] - cnt[2];
		s = tmp + ext[1];

		if(s<0.0f)
		{
			d += s*s;
			if(d>mRadius2)
				return 0;
		}
		else
		{
			s = tmp - ext[2];
			if(s>0.0f)
			{
				d += s*s;
				if(d>mRadius2)
					return 0;
			}
		}

		return d <= mRadius2;
	};

	//! This macro quickly finds the min & max values among 3 variables
	#define FINDMINMAX(x0, x1, x2, min, max)	\
		min = max = x0;							\
		if(x1<min) min=x1;						\
		if(x1>max) max=x1;						\
		if(x2<min) min=x2;						\
		if(x2>max) max=x2;

	//! TO BE DOCUMENTED
	inline bool planeBoxOverlap(const Point3f& normal, const float d, const Point3f& maxbox)
	{
		Point3f vmin, vmax;
		for(unsigned q=0;q<=2;q++)
		{
			if(normal[q]>0.0f)	{ vmin[q]=-maxbox[q]; vmax[q]=maxbox[q]; }
			else				{ vmin[q]=maxbox[q]; vmax[q]=-maxbox[q]; }
		}
		if((DotProd(normal,vmin))+d>0.0f) return 0;
		if((DotProd(normal,vmax))+d>=0.0f) return 1;

		return 0;
	}

	//! TO BE DOCUMENTED
	#define AXISTEST_X01(a, b, fa, fb)							\
		min = a*v0[1] - b*v0[2];								\
		max = a*v2[1] - b*v2[2];								\
		if(min>max) {const float tmp=max; max=min; min=tmp;	}	\
		rad = fa * ext[1] + fb * ext[2];						\
		if(min>rad || max<-rad) return 0;

	//! TO BE DOCUMENTED
	#define AXISTEST_X2(a, b, fa, fb)							\
		min = a*v0[1] - b*v0[2];								\
		max = a*v1[1] - b*v1[2];								\
		if(min>max) {const float tmp=max; max=min; min=tmp;	}	\
		rad = fa * ext[1] + fb * ext[2];						\
		if(min>rad || max<-rad) return 0;

	//! TO BE DOCUMENTED
	#define AXISTEST_Y02(a, b, fa, fb)							\
		min = b*v0[2] - a*v0[0];								\
		max = b*v2[2] - a*v2[0];								\
		if(min>max) {const float tmp=max; max=min; min=tmp;	}	\
		rad = fa * ext[0] + fb * ext[2];						\
		if(min>rad || max<-rad) return 0;

	//! TO BE DOCUMENTED
	#define AXISTEST_Y1(a, b, fa, fb)							\
		min = b*v0[2] - a*v0[0];								\
		max = b*v1[2] - a*v1[0];								\
		if(min>max) {const float tmp=max; max=min; min=tmp;	}	\
		rad = fa * ext[0] + fb * ext[2];						\
		if(min>rad || max<-rad) return 0;

	//! TO BE DOCUMENTED
	#define AXISTEST_Z12(a, b, fa, fb)							\
		min = a*v1[0] - b*v1[1];								\
		max = a*v2[0] - b*v2[1];								\
		if(min>max) {const float tmp=max; max=min; min=tmp;	}	\
		rad = fa * ext[0] + fb * ext[1];						\
		if(min>rad || max<-rad) return 0;

	//! TO BE DOCUMENTED
	#define AXISTEST_Z0(a, b, fa, fb)							\
		min = a*v0[0] - b*v0[1];								\
		max = a*v1[0] - b*v1[1];								\
		if(min>max) {const float tmp=max; max=min; min=tmp;	}	\
		rad = fa * ext[0] + fb * ext[1];						\
		if(min>rad || max<-rad) return 0;

	// compute triangle edges
	// - edges lazy evaluated to take advantage of early exits
	// - fabs precomputed (half less work, possible since extents are always >0)
	// - customized macros to take advantage of the null component
	// - axis vector discarded, possibly saves useless movs
	#define IMPLEMENT_CLASS3_TESTS						\
		float rad;										\
		float min, max;									\
														\
		const float fey0 = fabsf(e0[1]);				\
		const float fez0 = fabsf(e0[2]);				\
		AXISTEST_X01(e0[2], e0[1], fez0, fey0);			\
		const float fex0 = fabsf(e0[0]);				\
		AXISTEST_Y02(e0[2], e0[0], fez0, fex0);			\
		AXISTEST_Z12(e0[1], e0[0], fey0, fex0);			\
														\
		const float fey1 = fabsf(e1[1]);				\
		const float fez1 = fabsf(e1[2]);				\
		AXISTEST_X01(e1[2], e1[1], fez1, fey1);			\
		const float fex1 = fabsf(e1[0]);				\
		AXISTEST_Y02(e1[2], e1[0], fez1, fex1);			\
		AXISTEST_Z0(e1[1], e1[0], fey1, fex1);			\
														\
		const Point3f e2 = tri[0] - tri[2];				\
		const float fey2 = fabsf(e2[1]);				\
		const float fez2 = fabsf(e2[2]);				\
		AXISTEST_X2(e2[2], e2[1], fez2, fey2);			\
		const float fex2 = fabsf(e2[0]);				\
		AXISTEST_Y1(e2[2], e2[0], fez2, fex2);			\
		AXISTEST_Z12(e2[1], e2[0], fey2, fex2);

	//Neonic: perform tests with opcode 1.3 [BOOL AABBTreeCollider::TriBoxOverlap]
	bool CCollider::OverlapTriAABB( const Trif& tri, const Point3f& cnt, const Point3f& ext )
	{
		// Stats
//		mNbBVPrimTests++;

		//Neonic: hardcode
		bool mFullPrimBoxTest=1;	//!< Perform full Primitive-BV tests (true) or SAT-lite tests (false)


		// use separating axis theorem to test overlap between triangle and box 
		// need to test for overlap in these directions: 
		// 1) the {x,y,z}-directions (actually, since we use the AABB of the triangle 
		//    we do not even need to test these) 
		// 2) normal of the triangle 
		// 3) crossproduct(edge from tri, {x,y,z}-directin) 
		//    this gives 3x3=9 more tests 

		// move everything so that the boxcenter is in (0,0,0) 
		Point3f v0, v1, v2;
		v0[0] = tri[0][0] - cnt[0];
		v1[0] = tri[1][1] - cnt[0];
		v2[0] = tri[2][2] - cnt[0];

		// First, test overlap in the {x,y,z}-directions
	#ifdef OPC_USE_FCOMI
		// find min, max of the triangle in x-direction, and test for overlap in X
		if(FCMin3(v0[0], v1[0], v2[0])>ext[0])	return 0;
		if(FCMax3(v0[0], v1[0], v2[0])<-ext[0])	return 0;

		// same for Y
		v0[1] = tri[0][1] - cnt[1];
		v1[1] = tri[1][1] - cnt[1];
		v2[1] = tri[2][1] - cnt[1];

		if(FCMin3(v0[1], v1[1], v2[1])>ext[1])	return 0;
		if(FCMax3(v0[1], v1[1], v2[1])<-ext[1])	return 0;

		// same for Z
		v0[2] = tri[0][2] - cnt[2];
		v1[2] = tri[1][2] - cnt[2];
		v2[2] = tri[2][2] - cnt[2];

		if(FCMin3(v0[2], v1[2], v2[2])>ext[2])	return 0;
		if(FCMax3(v0[2], v1[2], v2[2])<-ext[2])	return 0;
	#else
		float min,max;
		// Find min, max of the triangle in x-direction, and test for overlap in X
		FINDMINMAX(v0[0], v1[0], v2[0], min, max);
		if(min>ext[0] || max<-ext[0]) return 0;

		// Same for Y
		v0[1] = tri[0][1] - cnt[1];
		v1[1] = tri[1][1] - cnt[1];
		v2[1] = tri[2][1] - cnt[1];

		FINDMINMAX(v0[1], v1[1], v2[1], min, max);
		if(min>ext[1] || max<-ext[1]) return 0;

		// Same for Z
		v0[2] = tri[0][2] - cnt[2];
		v1[2] = tri[1][2] - cnt[2];
		v2[2] = tri[2][2] - cnt[2];

		FINDMINMAX(v0[2], v1[2], v2[2], min, max);
		if(min>extents[2] || max<-ext[2]) return 0;
	#endif
		// 2) Test if the box intersects the plane of the triangle
		// compute plane equation of triangle: normal*x+d=0
		// ### could be precomputed since we use the same leaf triangle several times
		const Point3f e0 = v1 - v0;
		const Point3f e1 = v2 - v1;
		const Point3f normal = VecProd(e0,e1);
		const float d = -DotProd(normal,v0);
		if(!planeBoxOverlap(normal, d, ext)) return 0;

		// 3) "Class III" tests
		if(mFullPrimBoxTest)
		{
			IMPLEMENT_CLASS3_TESTS
		}
		return 1;
	};

	//Neonic: perform tests with opcode 1.3 [BOOL RayCollider::SegmentAABBOverlap]
	bool CCollider::OverlapSegmentAABB(	const Rayf& ray,
										const Point3f& cnt, const Point3f& ext )
	{
		// Stats
//		mNbRayBVTests++;

		// For Segment-AABB overlap
		//Neonic: hardcode
		float mMaxDist = FLT_MAX;
		Point3f mData = 0.5f * ray.getDir() * mMaxDist;
		Point3f mData2 = ray.getOrigin() + mData;

		// Precompute mFDir;
		Point3f mFDir(fabsf(mData[0]), fabsf(mData[1]), fabsf(mData[2]));

		//Special case: 1-triangle meshes ???

		// code

		float Dx = mData2[0] - cnt[0];
		if(fabsf(Dx) > ext[0] + mFDir[0])
			return 0;

		float Dy = mData2[1] - cnt[1];
		if(fabsf(Dy) > ext[1] + mFDir[1])
			return 0;

		float Dz = mData2[2] - cnt[2];
		if(fabsf(Dz) > ext[2] + mFDir[2])
			return 0;

		float f;
		f = mData[1] * Dz - mData[2] * Dy;
		if(fabsf(f) > ext[1]*mFDir[2] + ext[2]*mFDir[1])
			return 0;

		f = mData[2] * Dx - mData[0] * Dz;
		if(fabsf(f) > ext[0]*mFDir[2] + ext[2]*mFDir[0])
			return 0;

		f = mData[0] * Dy - mData[1] * Dx;
		if(fabsf(f) > ext[0]*mFDir[1] + ext[1]*mFDir[0])
			return 0;

		return 1;
	};

	//Neonic: perform tests with opcode 1.3 [BOOL RayCollider::RayAABBOverlap]
	bool CCollider::OverlapRayAABB(	const Rayf& ray, const Point3f& cnt, const Point3f& ext )
	{
		// Stats
//		mNbRayBVTests++;

		//Special case: 1-triangle meshes ???

		// Precompute mFDir;
		Point3f mFDir(fabsf(ray.getDir()[0]), fabsf(ray.getDir()[1]), fabsf(ray.getDir()[2]));

		float Dx = ray.getOrigin()[0] - cnt[0];
		if(GREATER(Dx, ext[0]) && Dx*ray.getDir()[0]>=0.0f)
			return 0;

		float Dy = ray.getOrigin()[1] - cnt[1];
		if(GREATER(Dy, ext[1]) && Dy*ray.getDir()[1]>=0.0f)
			return 0;

		float Dz = ray.getOrigin()[2] - cnt[2];
		if(GREATER(Dz, ext[2]) && Dz*ray.getDir()[2]>=0.0f)
			return 0;

		float f;
		f = ray.getDir()[1] * Dz - ray.getDir()[2] * Dy;
		if(fabsf(f) > ext[1]*mFDir[2] + ext[2]*mFDir[1])
			return 0;

		f = ray.getDir()[2] * Dx - ray.getDir()[0] * Dz;
		if(fabsf(f) > ext[0]*mFDir[2] + ext[2]*mFDir[0])
			return 0;

		f = ray.getDir()[0] * Dy - ray.getDir()[1] * Dx;
		if(fabsf(f) > ext[0]*mFDir[1] + ext[1]*mFDir[0])
			return 0;

		return 1;
	};

	//Neonic: perform tests with opcode 1.3 [BOOL RayCollider::RayTriOverlap]
	bool CCollider::OverlapRayTri(	const Rayf& ray, const Trif& tri )
	{
		// Stats
//		mNbRayPrimTests++;

		//Neonic: hardcode
		class CollisionFace
		{
			public:
			//! Constructor
			inline				CollisionFace()			{}
			//! Destructor
			inline				~CollisionFace()		{}

					unsigned	mFaceID;				//!< Index of touched face
					float		mDistance;				//!< Distance from collider to hitpoint
					float		mU, mV;					//!< Impact barycentric coordinates
		};

		CollisionFace mStabbedFace;

		//Neonic: hardcode
		bool mCulling = 1;

		// Find vectors for two edges sharing v0
		Point3f edge1 = tri[1] - tri[0];
		Point3f edge2 = tri[2] - tri[0];

		// Begin calculating determinant - also used to calculate U parameter
		Point3f pvec = VecProd(ray.getDir(),edge2);

		// If determinant is near zero, ray lies in plane of triangle
		float det = DotProd(edge1,pvec);

		if(mCulling)
		{
			if(det<LOCAL_EPSILON)
				return 0;
			// From here, det is > 0. So we can use integer cmp.

			// Calculate distance from v0 to ray origin
			Point3f tvec = ray.getOrigin() - tri[0];

			// Calculate U parameter and test bounds
			mStabbedFace.mU = DotProd(tvec,pvec);
	//		if(IR(u)&0x80000000 || u>det)					return FALSE;
			if(IS_NEGATIVE_FLOAT(mStabbedFace.mU) || IR(mStabbedFace.mU)>IR(det))
				return 0;

			// Prepare to test V parameter
			Point3f qvec = VecProd(tvec,edge1);

			// Calculate V parameter and test bounds
			mStabbedFace.mV = DotProd(ray.getDir(),qvec);
			if(IS_NEGATIVE_FLOAT(mStabbedFace.mV) || mStabbedFace.mU+mStabbedFace.mV>det)
				return 0;

			// Calculate t, scale parameters, ray intersects triangle
			mStabbedFace.mDistance = DotProd(edge2,qvec);
			// Det > 0 so we can early exit here
			// Intersection point is valid if distance is positive (else it can just be a face behind the orig point)
			if(IS_NEGATIVE_FLOAT(mStabbedFace.mDistance))
				return 0;
			// Else go on
			float OneOverDet = 1.0f / det;
			mStabbedFace.mDistance *= OneOverDet;
			mStabbedFace.mU *= OneOverDet;
			mStabbedFace.mV *= OneOverDet;
		}
		else
		{
			// the non-culling branch
			if(det>-LOCAL_EPSILON && det<LOCAL_EPSILON)
				return 0;
			float OneOverDet = 1.0f / det;

			// Calculate distance from v0 to ray origin
			Point3f tvec = ray.getOrigin() - tri[0];

			// Calculate U parameter and test bounds
			mStabbedFace.mU = (DotProd(tvec,pvec)) * OneOverDet;
			if(IS_NEGATIVE_FLOAT(mStabbedFace.mU) || IR(mStabbedFace.mU)>IEEE_1_0)
				return 0;

			// prepare to test V parameter
			Point3f qvec = VecProd(tvec,edge1);

			// Calculate V parameter and test bounds
			mStabbedFace.mV = (DotProd(ray.getDir(),qvec)) * OneOverDet;
			if(IS_NEGATIVE_FLOAT(mStabbedFace.mV) || mStabbedFace.mU+mStabbedFace.mV>1.0f)
				return 0;

			// Calculate t, ray intersects triangle
			mStabbedFace.mDistance = (DotProd(edge2,qvec)) * OneOverDet;
			// Intersection point is valid if distance is positive (else it can just be a face behind the orig point)
			if(IS_NEGATIVE_FLOAT(mStabbedFace.mDistance))
				return 0;
		}
		return 1;
	};

	bool CCollider::OverlapRaySph( const Rayf& ray, const Spheref& sph )
	{
		//Neonic: hardcode
		int numhits;
		float t0, t1;
		return intersect(sph, ray, numhits, t0, t1);
	};

	//Neonic: perform tests with ODE [dCollideCCylinderBox]
	bool CCollider::OverlapCCylinderBox()
	{
		/*
		dxCCylinder *cyl = (dxCCylinder*) o1;
		dxBox *box = (dxBox*) o2;

		contact->g1 = o1;
		contact->g2 = o2;

		// get p1,p2 = cylinder axis endpoints, get radius
		dVector3 p1,p2;
		dReal clen = cyl->lz * REAL(0.5);
		p1[0] = o1->pos[0] + clen * o1->R[2];
		p1[1] = o1->pos[1] + clen * o1->R[6];
		p1[2] = o1->pos[2] + clen * o1->R[10];
		p2[0] = o1->pos[0] - clen * o1->R[2];
		p2[1] = o1->pos[1] - clen * o1->R[6];
		p2[2] = o1->pos[2] - clen * o1->R[10];
		dReal radius = cyl->radius;

		// copy out box center, rotation matrix, and side array
		dReal *c = o2->pos;
		dReal *R = o2->R;
		const dReal *side = box->side;

		// get the closest point between the cylinder axis and the box
		dVector3 pl,pb;
		dClosestLineBoxPoints (p1,p2,c,R,side,pl,pb);

		// generate contact point
		return dCollideSpheres (pl,radius,pb,0,contact);
		*/

		return 0;
	};

	bool CCollider::CollideTriTri( CCollisionObject& _1, CCollisionObject& _2 )
	{
		// TODO: CCollider::CollideTriTri
		/*
		assert(_1.getLocal()&&_2.getLocal());
		assert(_1.getMesh()&&_2.getMesh());

		if( !OverlapAABBs( _1.getPos(), _1.getExt(), _2.getPos(), _2.getExt()) )
			return 0;

//		03. detect overlap area in AABB's
		AABoxf overlap;
		makeOverlapAABB(_1.getAABB(),_2.getAABB(), overlap);
		Point3f pos;
		
//		04. grab primitives from 1st object (overlap, unique) return no collision if no prims
		pos = _1.getPos();
		CIndexTriAABBCollector c1(_1.getLocal(),overlap.getMin()-pos,overlap.getMax()-pos);
		if(!c1.m_vecObjs.size())
			return 0;
//		05. grab primitives from 2nd object (overlap, unique) return no collision if no prims
		pos = _2.getPos();
		CIndexTriAABBCollector c2(_2.getLocal(),overlap.getMin()-pos,overlap.getMax()-pos);
		if(!c2.m_vecObjs.size())
			return 0;

		std::unique(c1.m_vecObjs.begin(), c1.m_vecObjs.end());
		std::unique(c2.m_vecObjs.begin(), c2.m_vecObjs.end());

//		06. test 1st object with 2nd
		
		Trif tri0;
		unsigned int i1, i2, repos;
		VectorTIndexObject::iterator it1, it2;
		TMesh	*m1 = _1.getMesh(),
				*m2 = _2.getMesh();
		TGeom	g1 = m1->getGeometry(),
				g2 = m2->getGeometry();
		TVerts	&v1 = g1->lockVB(),
				&v2 = g2->lockVB();
		TInds	&in1 = g1->lockIB(),
				&in2 = g2->lockIB();
		
		pos = _1.getPos() - _2.getPos();
		for(it1=c1.m_vecObjs.begin(); it1!=c1.m_vecObjs.end(); it1++)
		{
			i1=*it1;
			for(repos=0; repos<3; repos++)
				tri0[repos] = v1[in1[i1+repos]].position + pos;
			for(it2=c2.m_vecObjs.begin(); it2!=c2.m_vecObjs.end(); it2++)
			{
				i2=*it2;
				// detect 1st intesection - detection as fact for now
				if( OverlapTriTri( tri0,
					Trif(	v2[in2[i2]].position,
							v2[in2[i2+1]].position,
							v2[in2[i2+2]].position) ) )
					return 1;
			}
		}
		*/

		return 0;
	};


//see void SphereCollider::_CollideNoPrimitiveTest(const AABBCollisionNode* node)
	bool CCollider::CollideSphSph( CCollisionObject& _1, CCollisionObject& _2 )
	{
		return OverlapSphSph(Spheref(_1.getPos(),_1.getRadius()),Spheref(_2.getPos(),_2.getRadius()));
	};

	bool CCollider::CollideAABBS( CCollisionObject& _1, CCollisionObject& _2 )
	{
		return OverlapAABBs(_1.getPos(),_1.getExt(),_2.getPos(),_2.getExt());
	};

	bool CCollider::CollideSphTri( CCollisionObject& sph, CCollisionObject& tri, const float& middle, const unsigned dtl )
	{
		//TODO:	CCollider::CollideSphTri ...

		if(dtl>getDTL())
			return 0;

// Циклический тест столкновения - производится внутри конкретной функции
//	определяем первую точку столкновения. Чем больше вариантов перепробуем - тем меньше вероятность
//	ошибки. Собственно, важно обнаружить конечные пересекающиеся объекты и вернуть точное время пересечения

		// старое положение + текущее положение
		Point3f	path1 = pos11+(pos1 - pos11)*middle,
				path2 = pos22+(pos2 - pos22)*middle;

		// Создание перекрывающегося объема для обeих глобальных AABB
		AABoxf	overlap, local;
		makeOverlapAABB(AABoxf(path1-ext1,path1+ext1), AABoxf(path2-ext2,path2+ext2), overlap);

	// Сбор примитивов из 2-го объекта(перекрытые, уникальные)

		// trans matrix to current (middle) position - used to test with dynamic meshes
//		setTrans(mat1, path2);
		calcAABB(mat1, local, overlap);

		CTRoot::CCollector coll(tri.getLocal().get(),local);
		coll();

		// При отсутствии примитивов - нет столкновения
		if(coll.m_Array.size())
		{

		// Тест столкновения 1-го объекта со вторым на основе примитивов

			// Создаем сферу в локальных координатах меша
			Spheref s( mat1*pos1, sph.getRadius() );

			TMesh *pMesh = tri.getMesh();
			TGeom g = pMesh->getGeometry();
			TVerts &v = g->lockVB();
			TInds &in = g->lockIB();
			for(Array::iterator it=coll.m_Array.begin(); it!=coll.m_Array.end(); it++)
			{
				//Neonic: bad bad glue - possible casting errors - (CIndexTriTreeObject*) cast
				CIndexTriTreeObject* itto = (CIndexTriTreeObject*) (*it).get();
				TIndex iIndex = itto->getIndex();
				// detect 1st intesection - detection as fact for now
				if( OverlapSphTri( s,
						Trif(	v[in[iIndex]].position,
								v[in[iIndex+1]].position,
								v[in[iIndex+2]].position) ) )
				{
					// Neonic: primitive collision here
					return 1;
				}
			}
		}

	// here we got no collision after test all of the primitives, so do another (discrete) tests

		// lookup area at half before our current middle
		if( CollideSphTri( sph, tri, middle/2, dtl+1 ) )
			return 1;

		// lookup area at half after our current middle
		if( CollideSphTri( sph, tri, middle+middle/2, dtl+1 ) )
			return 1;

		return 0;
	};

	bool CCollider::CollideSphAABB( CCollisionObject& sph, CCollisionObject& aabb )
	{
		return OverlapSphAABB(Spheref(sph.getPos(),sph.getRadius()),aabb.getPos(),aabb.getExt());
	};
		
	bool CCollider::CollideTriAABB( CCollisionObject& tri, CCollisionObject& aabb )
	{
		// TODO: CCollider::CollideTriAABB
		/*
		assert(tri.getLocal());
		assert(tri.getMesh());

		if( !OverlapAABBs( tri.getPos(), tri.getExt(), aabb.getPos(), aabb.getExt()) )
			return 0;

//		03. detect overlap area in AABB's
		AABoxf overlap;
		makeOverlapAABB(tri.getAABB(),aabb.getAABB(), overlap);
		
//		05. grab primitives from 2nd object (overlap, unique) return no collision if no prims
		Point3f pos = tri.getPos();
		CIndexTriAABBCollector c(tri.getLocal(),overlap.getMin()-pos,overlap.getMax()-pos);
		if(!c.m_vecObjs.size())
			return 0;

		std::unique(c.m_vecObjs.begin(), c.m_vecObjs.end());

//		06. test 1st object with 2nd

		unsigned int i;
		TMesh *m = tri.getMesh();
		TGeom g = m->getGeometry();
		TVerts &v = g->lockVB();
		TInds &in = g->lockIB();
		// move aabb to local coords
		Point3f cnt = aabb.getPos()-pos;
		for(VectorTIndexObject::iterator it=c.m_vecObjs.begin(); it!=c.m_vecObjs.end(); it++)
		{
			i = *it;
			// detect 1st intesection - detection as fact for now
			if( OverlapTriAABB( 
								Trif(	v[in[i]].position,
										v[in[i+1]].position,
										v[in[i+2]].position ),
								cnt, aabb.getExt()) )
				return 1;
		}
		*/

		return 0;
	};

	bool CCollider::CollideRayAABB( CCollisionObject& ray, CCollisionObject& aabb )
	{
		return OverlapRayAABB(Rayf(ray.getPos(),ray.getDir()),aabb.getPos(),aabb.getExt());
	};

	bool RecurseCollideRayTri(	const Rayf& ray, CTBranch* pBase,
											const TVerts &v, const TInds &in )
	{
		// TODO: RecurseCollideRayTri
//		assert(pBase);

		/*
		if( OverlapRayAABB(ray, pBase->getPos(), pBase->getExt()) )
		{
			if( pBase->isLeaf())
			{
				// collide
				CIndexTriTreeLeaf *pLeaf = (CIndexTriTreeLeaf*)pBase;
				unsigned int i;
				CIndexTriTreeLeaf::TChildrenList::iterator it;
				for(it=pLeaf->getChilds().begin(); it!=pLeaf->getChilds().end(); it++)
				{
					i = *it;
					// detect 1st intesection - detection as fact for now
					if( OverlapRayTri(	ray,
															Trif(	v[in[i]].position,
															v[in[i+1]].position,
															v[in[i+2]].position ) ) )
						return 1;
				}
			}
			else
			{
				// move in
				CIndexTriTreeBranch *pBranch = (CIndexTriTreeBranch*)pBase;
				IIndexTriTreeBase *p;
				for(unsigned i=0; i<8; i++)
				{
					p = pBranch->getBranch(i);
					if(p)
						if(RecurseCollideRayTri(ray,p,v,in))
							return 1;
				}
			}
		}
		*/
		return 0;
	};

	bool CCollider::CollideRayTri( CCollisionObject& ray, CCollisionObject& tri )
	{
		// optimize: do plane test - exclude unnessesary "inbound"(move in,test,return) tests

		// do recurse parsing - come, test, exit or continue
		assert(tri.getLocal().get());
		assert(tri.getMesh());
		// create ray at local coords
		Rayf r( ray.getPos()-tri.getPos(),ray.getDir()-tri.getPos());

		TMesh *m = tri.getMesh();
		TGeom g = m->getGeometry();
		TVerts &v = g->lockVB();
		TInds &in = g->lockIB();

		return RecurseCollideRayTri(r,tri.getLocal()->getBranch(),v,in);
	};

	bool CCollider::CollideRaySph( CCollisionObject& ray, CCollisionObject& sph )
	{
		return OverlapRaySph( Rayf(ray.getPos(),ray.getDir()),Spheref(sph.getPos(),sph.getRadius()) );
	};


}; // collision