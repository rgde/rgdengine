/******************************************************************************
@file    collider.h
@author  Neonic
@mail    gaba@male.ru
@project RGDE
@date    02.04.2006
@desc    collider interface
 ******************************************************************************/

//Neonic: octree

#ifndef COLLIDER_H
#define COLLIDER_H
#ifdef _WIN32
#pragma once
#endif

#include <rgde/scene/LiveTree.h>
#include <rgde/base/singelton.h>

namespace render
{
	class CMesh;
};

namespace collision
{
	using namespace live_tree;

	class CCollisionObject;
	class CIndexTriTreeObject;
	class CIndexTriTreeRoot;
	typedef unsigned short TIndex;

	typedef boost::intrusive_ptr<CIndexTriTreeObject> PIndexTriTreeObject;
	typedef boost::intrusive_ptr<CIndexTriTreeRoot> PIndexTriTreeRoot;

	enum eCPrimitive {
		ECP_UNKNOWN				= -1,	// unknown state

		ECP_TRIANGLE			= 0,	// standard mesh
		ECP_SPHERE,						// 
		ECP_AABB,						// 
		ECP_RAY,						// 
	};

	AABoxf& calcAABB( const Matrix44f& m, AABoxf& result, const AABoxf& source );
	AABoxf& calcAABB( const Matrix44f& m, AABoxf& result );

	struct CCollisionData
	{
		CCollisionObject*	pFirstObject;
		CCollisionObject*	pSecondObject;
		Point3f				m_pos;
		Point3f				m_normal;
		float				m_fDepth;
	};

	typedef int CollisionCallback( CCollisionData& data );

	////////////////////////////////////////////////////////////////////////
	// 
	// CContact
	//	contact info. produced by collision functions. ode
	////////////////////////////////////////////////////////////////////////

	/*
	class CContact
	{
	public:

		CContact( void ) : m_depth(0)
		{
			m_pObj[0] = m_pObj[1] = 0;
		};

		Point3f					m_position;
		Point3f					m_normal;
		float					m_depth;

		CIndexTriTreeObject*	m_pObj[2];

	};
	*/

	////////////////////////////////////////////////////////////////////////
	// 
	// CIndexTriTreeRoot
	//	
	////////////////////////////////////////////////////////////////////////

	class CIndexTriTreeObject
		: public CTObject
	{
	public:
		CIndexTriTreeObject( const TIndex& index =0 ) : m_iIndex(index) {};

		const TIndex&	getIndex( void ) const			{ return m_iIndex; };
		void			setIndex( const TIndex& index )	{ m_iIndex = index; };

	private:
		TIndex	m_iIndex;
	};

	////////////////////////////////////////////////////////////////////////
	// 
	// CIndexTriTreeRoot
	//	root - placed at zero point (for global tree object)
	//	TODO: it an AABB-cube at object (mean OBB-cube), so form tests correctly
	////////////////////////////////////////////////////////////////////////

	class CIndexTriTreeRoot
		: public CTRoot
	{
	public:
		CIndexTriTreeRoot( unsigned _ext=2147483647, unsigned _base=8, int LimitDivisions =-1 );
	};

	////////////////////////////////////////////////////////////////////////
	// 
	// CCollisionObject
	// 
	////////////////////////////////////////////////////////////////////////

	class CCollisionObject
		: public CTObject				// reflect in global tree
	{
	public:
		CCollisionObject( void );
		~CCollisionObject( void );

		void						setPrim( eCPrimitive prim ) { m_Primitive = prim; };
		eCPrimitive					getPrim( void ) { return m_Primitive; };

		bool						createLocal( render::CMesh* _pMesh, const Point3f& ext, unsigned base =4 );
		bool						createLocal( render::CMesh* _pMesh, const float& side, unsigned base =4 );
		void						clearLocal( void );
		PIndexTriTreeRoot			getLocal( void ) { return m_pLocalTree; };

		int							getNum( void );

		render::CMesh*				getMesh( void ) { return m_pMesh; };

	// convertors for support types
		void						makeSphere( const Point3f& center, const float& radius );
		void						makeSphere( const Spheref& sphere );
		void						setRadius( float radius );
		float						getRadius( void );

		void						makeAABB( const Point3f& cnt, const Point3f& ext );

		void						makeRay( const Point3f& origin, const Point3f& dir );
		void						makeRay( const Rayf& ray );
		void						setDir( const Point3f& dir );
		const Point3f&				getDir( void );

	private:
		// ptr to local tree
		PIndexTriTreeRoot	m_pLocalTree;
		// primitive type - for fast support highlights
		eCPrimitive			m_Primitive;
		// primitive buffered data. not dynamic - simple & not too much
		Point3f				m_Sup;

		// mesh pointer
		render::CMesh*		m_pMesh;
	};

	////////////////////////////////////////////////////////////////////////
	// 
	// CCollider
	//	universal collider
	////////////////////////////////////////////////////////////////////////

	class CCollider
	{
	public:

		CCollider( void );

		//
		// stats
		//

		void				resetStat( void );
		unsigned			getNumOfPrimsTests( void )		{ return m_iPrimPrimTests; };
		unsigned			getNumOfVolumesTests( void )	{ return m_iVolumePrimTests; };

		// process collide in global dynamic world
		void				CollideWorld( CDynamicTreeRoot* p );
		// collide one object with global world
		bool				CollideGlobal( CCollisionObject& obj, CTRoot* p=0);
		// main test query for collision objects
		bool				Collide( CCollisionObject& _1, CCollisionObject& _2 );

		// 
		// Overlap tests
		// 

		// overlap triangle vs triangle test
		bool				OverlapTriTri(	const Trif& tri0, const Trif& tri1 );
		// overlap sphere vs sphere test
		bool				OverlapSphSph(	const Spheref& s0, const Spheref& s1 );
		// overlap AABB vs AABB tests
		bool				OverlapAABBs(	const Point3f& cnt0, const Point3f& ext0,
											const Point3f& cnt1, const Point3f& ext1 );
		bool				OverlapAABBs(	const AABoxf& aabb0, const AABoxf& aabb1 );
		// overlap sphere vs triangle test
		bool				OverlapSphTri(	const Spheref& s, const Trif& tri );
		// overlap sphere vs AABB test
		bool				OverlapSphAABB(	const Spheref& sph, const Point3f& cnt, const Point3f& ext );
		// overlap triangle vs AABB test
		bool				OverlapTriAABB( const Trif& tri, const Point3f& cnt, const Point3f& ext );
		// overlap ray segment vs AABB test
		bool				OverlapSegmentAABB(	const Rayf& ray,
												const Point3f& cnt, const Point3f& ext );
		// overlap ray vs AABB test
		bool				OverlapRayAABB(	const Rayf& ray, const Point3f& cnt, const Point3f& ext );
		// overlap ray vs triangle test -- usage in ODE - dCollideRTL
		bool				OverlapRayTri(	const Rayf& ray, const Trif& tri );
		// overlap ray vs sphere test
		bool				OverlapRaySph( const Rayf& ray, const Spheref& sph );

		bool				OverlapCCylinderBox();

		// discrete test level
		void				setDTL( unsigned dtl ) { m_iDiscreteTestLevel = dtl; };
		unsigned			getDTL( void ) { return m_iDiscreteTestLevel; };

	private:
		// collision object tests
		bool CollideTriTri( CCollisionObject& _1, CCollisionObject& _2 );
		bool CollideSphSph( CCollisionObject& _1, CCollisionObject& _2 );
		bool CollideAABBS( CCollisionObject& _1, CCollisionObject& _2 );
		bool CollideSphTri( CCollisionObject& sph, CCollisionObject& tri, const float& middle, const unsigned dtl =0 );
		bool CollideSphAABB( CCollisionObject& sph, CCollisionObject& aabb );
		bool CollideTriAABB( CCollisionObject& tri, CCollisionObject& aabb );
		bool CollideRayAABB( CCollisionObject& ray, CCollisionObject& aabb );
		bool CollideRayTri( CCollisionObject& ray, CCollisionObject& tri );
		bool CollideRaySph( CCollisionObject& ray, CCollisionObject& sph );

		unsigned		m_iDiscreteTestLevel;

	// stats
		unsigned		m_iPrimPrimTests;			// hardest tests
		unsigned		m_iVolumePrimTests;			// simplest tests

	// temporary data
		float		firstContact;
		float		secondContact;
		Point3f		pos1, pos2;						// current positions
		Point3f		pos11, pos22;					// old positions
		Point3f		ext1, ext2;						// extensions
		// reverse matrix. used to convert point in global space to local space of 2nd object
		Matrix44f	mat1;
		// mat2 not used
		Matrix44f	mat2;
	};

	typedef base::TSingelton<CCollider> TheCollider;

}; // collision

#endif // COLLIDER_H
