/******************************************************************************
@file    liveTree.cpp
@author  Neonic
@mail    gaba@male.ru
@project RGDE
@date    12.05.2006
@desc    
 ******************************************************************************/
#include "precompiled.h"

#include <rgde/base/smart_ptr_helpers.h>
#include <rgde/math/types3d.h>
#include <rgde/render/lines3d.h>
#include <rgde/render/render_device.h>

#include <rgde/scene/live_tree.h>

namespace live_tree
{
	bool calc_progression( const int &in, int &out)
	{
		int i, shift = 16, curr = 16, src = in, dst;
		//steps are: 16 8 4 2 1 - total 5
		for(i=0;i<5;i++)
		{
			curr = curr>>1;							// calc current shift
			dst = 1<<shift;							// calc test value
			if(::abs(dst)==::abs(src))					// equal - stop searching
				break;
			if(::abs(dst)>::abs(src))					// reduce shift
				shift -= curr;
			else									// enlarge shift
				shift += curr;
		}
		if(::abs(src)>::abs(dst))						// check for other bits
			dst = dst<<1;							// shift a bit
		if(::abs(src)>src)							// check for sign
			dst = -dst;
		out = dst;
		return 1;
	};

	Color g_colorBranch(0,255,0,255);
	Color g_colorObject(255,255,255,255);
	Color g_colorDynamic(0,0,255,255);

	void drawObjectCube( const aaboxf& aabb, int iColor =0 )
	{
		math::point3f max = aabb.getMax();
		math::point3f min = aabb.getMin();

		/*
		if(!bObject)
			color = g_colorBranch;
		else
		{
			if(obj->isEmpty())
			{
//				max = math::point3f(min[0]+1,min[1]+1,min[2]+1);
//				render::lines3d::get().add_line( min, max, color );
				// or
				render::ThePoint3dManager::get().addPoint( min, color );
				return;
			}
		}
		*/
		math::Color color = g_colorObject;
		switch(iColor) {
//			case 0:		color = g_colorObject;		break;
			case 1:		color = g_colorBranch;		break;
			case 2:		color = g_colorDynamic;		break;
		}

		static const int nVerts = 8;
		static vec3f box[nVerts];
		static bool isVertexesCreated = false;

		if (!isVertexesCreated)
		{
			isVertexesCreated = true;

			box[0] = vec3f( 0, 1, 0 );		// 0 mean min point, 1 mean max point
			box[1] = vec3f( 0, 1, 1 );
			box[2] = vec3f( 1, 1, 1 );
			box[3] = vec3f( 1, 1, 0 );

			box[4] = vec3f( 0, 0, 0 );
			box[5] = vec3f( 0, 0, 1 );
			box[6] = vec3f( 1, 0, 1 );
			box[7] = vec3f( 1, 0, 0 );
		}

		point3f SizeBox[nVerts];
		for (unsigned i = 0; i < nVerts; ++i)
			SizeBox[i] = point3f(box[i][0]?max[0]:min[0], box[i][1]?max[1]:min[1], box[i][2]?max[2]:min[2]);


		render::lines3d& line_manager = render::render_device::get().get_lines3d();
		// Добавляем линии
		// up size
		line_manager.add_line( SizeBox[0], SizeBox[1], color );
		line_manager.add_line( SizeBox[1], SizeBox[2], color );
		line_manager.add_line( SizeBox[2], SizeBox[3], color );
		line_manager.add_line( SizeBox[3], SizeBox[0], color );

		//bottom size
		line_manager.add_line( SizeBox[0 + 4], SizeBox[1 + 4], color );
		line_manager.add_line( SizeBox[1 + 4], SizeBox[2 + 4], color );
		line_manager.add_line( SizeBox[2 + 4], SizeBox[3 + 4], color );
		line_manager.add_line( SizeBox[3 + 4], SizeBox[0 + 4], color );

		// connections between up and bottom
		line_manager.add_line( SizeBox[0], SizeBox[0 + 4], color );
		line_manager.add_line( SizeBox[1], SizeBox[1 + 4], color );
		line_manager.add_line( SizeBox[2], SizeBox[2 + 4], color );
		line_manager.add_line( SizeBox[3], SizeBox[3 + 4], color );
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// TreeObject
	//
	////////////////////////////////////////////////////////////////////////

	TreeObject::TreeObject( void )
		: m_root(0), m_pPosOld(0)
	{
		// nothing
	};

	TreeObject::~TreeObject( void )
	{
		// at deletion if we are inserted - we must cleanup self NOW
		if(getRoot())
			getRoot()->ejectNow(this);
		if(m_pPosOld)
		{
			delete m_pPosOld;
			m_pPosOld = 0;
		}
	};

	aaboxf TreeObject::getAABB( void )
	{
		return aaboxf(get_pos()-getExt(), get_pos()+getExt());
	};

	aaboxf TreeObject::getDynamicAABB( void )
	{
		if(!m_pPosOld)
			return getAABB();

		aaboxf aabb(*m_pPosOld-getExt(), *m_pPosOld+getExt());
		extendVolume(aabb,getAABB());
		return aabb;
	};
	
	void TreeObject::setAABB( const aaboxf& aabb )
	{
		set_pos((aabb.getMin()+aabb.getMax())*0.5f);
		setExt((aabb.getMax()-aabb.getMin())*0.5f);
	};

	const point3f& TreeObject::get_pos( void ) const
	{
		return m_Position;
	};

	void TreeObject::set_pos( const point3f& pos )
	{
		if(getRoot())
			getRoot()->set_pos(this,pos);
		else
			m_Position = pos;
	};

	const point3f& TreeObject::getExt( void ) const
	{
		return m_Ext;
	};
	
	void TreeObject::setExt( const point3f& ext )
	{
		if(getRoot())
			getRoot()->setExt(this,ext);
		else
			m_Ext = ext;
	};

	void TreeObject::inject( CTRoot* p )
	{
		p->inject(this);
	};
	
	void TreeObject::eject( void )
	{
		if(getRoot())
			getRoot()->eject(this);
	};

	void TreeObject::move( const point3f& pos )
	{
		if(getRoot())
			getRoot()->move(this,pos);
	};

	void TreeObject::setRoot( CTRoot* p )
	{
		m_root = p;
	};

	CTRoot* TreeObject::getRoot( void )
	{
		return m_root;
	};

	void TreeObject::draw( void )
	{
		drawObjectCube(getDynamicAABB());
	};


//	void TreeObject::remSelf( void )
//	{
//		if(getRoot())
//			getRoot()->ejectNow(this);
//	};

	void TreeObject::makeDynamic( void )
	{
		if(!m_pPosOld)	
			m_pPosOld = new point3f();
		updatePos();
	};

	bool TreeObject::makeStatic( void )
	{
		if(!m_pPosOld)
			return 0;

		if(getRoot())
		{
			if(getRoot()->isApproximated())
				return 0;
			delete m_pPosOld;
			m_pPosOld = 0;
			return 1;
		}
		// object call to make static:
		//	not inserted - this mean reconvertation after eject or before eject (sad - not inserted).
		//	please tell me if assert happen here. gaba@male.ru
		assert(0);
		return 0;
	};
	
	bool TreeObject::isDynamic( void )
	{
		return m_pPosOld;
	};

	point3f TreeObject::getOldPos( void )
	{
		if(!m_pPosOld)
			return get_pos();
		return *m_pPosOld;
	};

	void TreeObject::updatePos( void )
	{
		assert(m_pPosOld);
		*m_pPosOld = get_pos();
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// CTBranch
	//
	////////////////////////////////////////////////////////////////////////

	CTBranch::CTBranch( void )
		: m_iUsed(0), m_pParent(0)
	{
		for( int i=0; i<8; i++ )
			m_Branches[i] = 0;
	};

	CTBranch::~CTBranch( void )
	{
		clear();
	};

	bool CTBranch::isContainer( void )
	{
		return m_Array.size();
	};

	bool CTBranch::isBranch( void )
	{
		return m_iUsed;
	};

	bool CTBranch::isEmpty( void )
	{
		return ( (!isBranch()) && (!isContainer()) );
	};

	Array&  CTBranch::getArray( void )
	{
		return m_Array;
	};

	CTBranch* CTBranch::get_parent( void )
	{
		return m_pParent;
	};

	void CTBranch::set_parrent( CTBranch* p )
	{
		m_pParent = p;
	};

	CTBranch* CTBranch::getBranch( unsigned iIndex )
	{
		assert(iIndex<8);
		return m_Branches[iIndex];
	};

	CTBranch* CTBranch::call( unsigned iIndex )
	{
		assert(iIndex<8);
		if(!m_Branches[iIndex])
		{
			m_Branches[iIndex] = new CTBranch();
			m_Branches[iIndex]->set_parrent(this);
			m_iUsed++;
		}
		return m_Branches[iIndex];
	};

	void CTBranch::eject( CTBranch* pBranch )
	{
		for( int i=0; i<8; i++ )
		{
			if( m_Branches[i]==pBranch )
			{
				m_Branches[i]->set_parrent(0);			// deselect their parent
				delete m_Branches[i];					// destroy branch here after parenthesis removal
				m_Branches[i] = 0;						// cleanup branch ptr
				m_iUsed--;								// calc removal
				break;
			}
		}

		if(isEmpty()&&get_parent())
			get_parent()->eject(this);
	};

	void CTBranch::clear( void )
	{
		size_t size = getArray().size();
		if(size)
		{
			Array::iterator it = getArray().begin();
			//Neonic: eject place iterator to next pointer
			for ( size_t current = 0; current < size; current++ /*it++*/ )
				(*it)->eject();
			//getArray().clear();
			// not needed
			assert(!getArray().size());
		}

		// cleanup branches
		if(m_iUsed>0)
		{
			for( int i=0; i<8; i++ )
			{
				if(m_Branches[i])
				{
					m_Branches[i]->set_parrent(0);
					delete m_Branches[i];
					m_Branches[i] = 0;
					m_iUsed--;
				}
			}
		}

		if(get_parent())
			get_parent()->eject(this);
	};

	void CTBranch::draw( void )
	{
		for( int i=0; i<8; i++ )
			if(m_Branches[i])
				m_Branches[i]->draw();
		for(Array::iterator it = getArray().begin(); it != getArray().end(); it++ )
			(*it)->draw();
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// CTRoot
	//
	////////////////////////////////////////////////////////////////////////

	CTRoot::CTRoot( unsigned _ext, unsigned _base, int LimitDivisions )
		: m_iBase(_base)
		, m_LimitDivisions(LimitDivisions)
		, m_iObjInTree(0)
		, m_iDeepLevel(0)
	{
		if(_ext==0)
			_ext = 2147483647;
		if(_base>_ext)
			_base = 8;

		m_iExt = (_ext/getBase())*getBase();

		setBranch(new CTBranch());
	};

	CTRoot::~CTRoot( void )
	{
		delete getBranch();
		setBranch(0);
	};

	void CTRoot::clear( void )
	{
		getBranch()->clear();
	};
	
	/*
	bool CTRoot::test_obj_in_branch( PObject pBranch, PObject obj )
	{
		aaboxf BranchAABB(pBranch->getAABB()), aabb;
		aabb = obj->getDynamicAABB();
		return intersect(BranchAABB,aabb);
	};
	*/

	unsigned CTRoot::getBase( void )
	{
		return m_iBase;
	};

	int CTRoot::getExt( void )
	{
		return m_iExt;
	};

	CTBranch* CTRoot::getBranch( void )
	{
		return m_pBranch;
	};

	void CTRoot::setBranch( CTBranch* p )
	{
		m_pBranch = p;
	};

	void CTRoot::draw( void )
	{
		if(getBranch())
			getBranch()->draw();
	};

	void CTRoot::inject( PObject obj, aaboxf* pAABB )
	{
		if(obj->getRoot())
			return;

		aaboxf aabb;
		if(pAABB)
			aabb = *pAABB;
		else
			aabb = obj->getAABB();

		CInjector injector(obj,this,aabb,m_LimitDivisions);
		if(injector())
		{
			obj->setRoot(this);
			// stat
			m_iObjInTree++;
		}
	};

	void CTRoot::eject( PObject obj, aaboxf* pAABB )
	{
		if(!obj->getRoot())
			return;

		aaboxf aabb = pAABB? *pAABB : obj->getAABB();

		CEjector ejector(obj,this,aabb,m_LimitDivisions);
		if(ejector())
		{
			obj->setRoot(0);
			// stat
			m_iObjInTree--;
		}
	};

	void CTRoot::ejectNow( PObject obj )
	{
		eject(obj);
	};

	void CTRoot::set_pos( PObject obj, const point3f pos )
	{
		if(obj->getRoot())
			CTRoot::move(obj,pos);
		else
			obj->set_pos(pos);
	};

	void CTRoot::setExt( PObject obj, const point3f ext )
	{
		if(obj->getRoot())
		{
			CTRoot::eject(obj);
			obj->setExt(ext);
			CTRoot::inject(obj);
		}
		else
			obj->setExt(ext);
	};

	void CTRoot::move( PObject obj, const point3f& pos )
	{
		if(obj->getRoot())
			CTRoot::eject(obj);
		obj->set_pos(pos);
		CTRoot::inject(obj);
	};


	// 
	// CCollector
	// 

	CTRoot::CCollector::CCollector( CTRoot* pRoot, const aaboxf& aabb, PObject exclude )
		: m_root(pRoot), m_AABB(aabb), m_pExcludeObject(exclude)
	{
		// nothing
	};

	Array& CTRoot::CCollector::operator ()( void )
	{
		parse(m_root->getBranch(),point3f(),m_root->getExt());
		
		// Neonic: if got object repeats - add here sort&duplicaton removals
		
		if(m_pExcludeObject)
			m_Array.erase(std::remove(m_Array.begin(),m_Array.end(),m_pExcludeObject), m_Array.end());

		return m_Array;
	};

	void CTRoot::CCollector::parse( CTBranch* pBranch, const point3f& cnt, const float& ext )
	{
		collect(pBranch);

		int iIndexMin =   (m_AABB.getMin()[2] > cnt[2]? AXIS_Z:0)
						+ (m_AABB.getMin()[1] > cnt[1]? AXIS_Y:0)
						+ (m_AABB.getMin()[0] > cnt[0]? AXIS_X:0);

		int iIndexMax =   (m_AABB.getMax()[2] > cnt[2]? AXIS_Z:0)
						+ (m_AABB.getMax()[1] > cnt[1]? AXIS_Y:0)
						+ (m_AABB.getMax()[0] > cnt[0]? AXIS_X:0);

		if(iIndexMin==iIndexMax)
		{
			// way 1: indexes are same. branches was divided (if it was)
			call(pBranch, iIndexMin, cnt, ext);
		}
		else
		{
			// way 2: indexes not same. process in all different dimensions
			int iDiff = ::abs(iIndexMin - iIndexMax);						// produce differential index
			// all are affected. sides 4+2+1
			if(iDiff==7)
			{
				for(iDiff=0; iDiff<8; iDiff++)
					call(pBranch,iDiff, cnt, ext);
			}
			else
			{
				//version 1 - simple different test
				int iCurrentZ = iIndexMin;
				for( int iZ=0; iZ<2; iZ++)
				{
					int iCurrentY = iCurrentZ;
					for( int iY=0; iY<2; iY++) 
					{
						int iCurrentX = iCurrentY;
						for( int iX=0; iX<2; iX++) 
						{
							call(pBranch, iCurrentX, cnt, ext);			// touch current
							if(iDiff&AXIS_X)
							{
								if(!iX)
									iCurrentX += AXIS_X;
							} else
								break;
						} //for( int iX=0; iX<2; iX++) 
						if(iDiff&AXIS_Y)
						{
							if(!iY)
								iCurrentY += AXIS_Y;
						} else
							break;
					} //for( int iY=0; iY<2; iY++)
					if(iDiff&AXIS_Z)
					{
						if(!iZ)
							iCurrentZ += AXIS_Z;
					}
					else
						break;
				} //for( int iZ=0; iZ<2; iZ++)
			} //if(iDiff==7)
		}
	};

	void CTRoot::CCollector::collect( CTBranch* pBranch )
	{
		if( pBranch->isContainer() )
			std::copy( pBranch->getArray().begin(), pBranch->getArray().end(), std::back_inserter(m_Array) );
	};

	void CTRoot::CCollector::call( CTBranch* pBranch, const unsigned& iIndex, const point3f& cnt, const float& ext )
	{
		assert(iIndex<8);
		if(pBranch->getBranch(iIndex))
		{
			point3f center;
			float fQuart = ext*0.5f;
			for(unsigned iStep = 0; iStep<3; iStep++)
			{
				bool is_exist = iIndex&(1<<iStep);
				center[iStep] = cnt[iStep]+(is_exist?fQuart:-fQuart);
			}
			parse(pBranch->getBranch(iIndex),center,fQuart);
		}
	};


	// 
	// CInjector
	// 

	CTRoot::CInjector::CInjector( PObject p, CTRoot* pRoot, const aaboxf& aabb, int LimitDivisions )
		: m_pObject(p), m_root(pRoot), m_AABB(aabb), m_LimitDivisions(LimitDivisions)
	{
		// nothing
	};

	bool CTRoot::CInjector::operator ()( void )
	{
		return parse(m_root->getBranch(),point3f(),m_root->getExt(),0);
	};

	bool CTRoot::CInjector::parse( CTBranch* pBranch, const point3f& cnt, const float& ext, const unsigned& division )
	{
		// test for limits
		if( division==m_LimitDivisions || ext <= m_root->getBase() )
			return inject(pBranch,division);

		int iIndexMin =   (m_AABB.getMin()[2] > cnt[2]? AXIS_Z:0)
						+ (m_AABB.getMin()[1] > cnt[1]? AXIS_Y:0)
						+ (m_AABB.getMin()[0] > cnt[0]? AXIS_X:0);

		int iIndexMax =   (m_AABB.getMax()[2] > cnt[2]? AXIS_Z:0)
						+ (m_AABB.getMax()[1] > cnt[1]? AXIS_Y:0)
						+ (m_AABB.getMax()[0] > cnt[0]? AXIS_X:0);

		if(iIndexMin==iIndexMax)
		{
			// way 1: indexes are same. branches divided
			point3f center;
			float fQuart = ext*0.5f;
			for(unsigned iStep = 0; iStep<3; iStep++)
			{
				bool is_exist = iIndexMin&(1<<iStep);
				center[iStep] = cnt[iStep]+(is_exist?fQuart:-fQuart);
			}
			return parse(pBranch->call(iIndexMin),center,fQuart,division+1);
		}
		else
		{
			// way 2: indexes different. insert object
			return inject(pBranch,division);
		}
	};

	bool CTRoot::CInjector::inject( CTBranch* pBranch, const unsigned& division )
	{
		pBranch->getArray().push_back(m_pObject);
		if(division > m_root->m_iDeepLevel)
			m_root->m_iDeepLevel = division;
		return 1;
	};


	// 
	// CEjector
	// 

	CTRoot::CEjector::CEjector( PObject p, CTRoot* pRoot, const aaboxf& aabb, int LimitDivisions )
		: m_pObject(p), m_root(pRoot), m_AABB(aabb), m_LimitDivisions(LimitDivisions)
	{
		// nothing
	};

	bool CTRoot::CEjector::operator ()( void )
	{
		return parse(m_root->getBranch(),point3f(),m_root->getExt(),0);
	};

	bool CTRoot::CEjector::parse( CTBranch* pBranch, const point3f& cnt, const float& ext, const unsigned& division )
	{
		// test for limits
		if( division==m_LimitDivisions || ext <= m_root->getBase() )
			return eject(pBranch);

		int iIndexMin =   (m_AABB.getMin()[2] > cnt[2]? AXIS_Z:0)
						+ (m_AABB.getMin()[1] > cnt[1]? AXIS_Y:0)
						+ (m_AABB.getMin()[0] > cnt[0]? AXIS_X:0);

		int iIndexMax =   (m_AABB.getMax()[2] > cnt[2]? AXIS_Z:0)
						+ (m_AABB.getMax()[1] > cnt[1]? AXIS_Y:0)
						+ (m_AABB.getMax()[0] > cnt[0]? AXIS_X:0);

		if(iIndexMin==iIndexMax)
		{
			// correct way, but always wrong if limits exist
//			if(pBranch->isContainer())
//				if(eject(pBranch, m_pObject))
//					return 1;

			// way 1: indexes are same. branches was divided (if it was)
			assert(pBranch->getBranch(iIndexMin));
//			if(pBranch->getBranch(iIndexMin))
//			{
				point3f center;
				float fQuart = ext*0.5f;
				for(unsigned iStep = 0; iStep<3; iStep++)
				{
					bool is_exist = iIndexMin&(1<<iStep);
					center[iStep] = cnt[iStep]+(is_exist?fQuart:-fQuart);
				}
				return parse(pBranch->getBranch(iIndexMin),center,fQuart,division+1);
//			}
			//else
			//{
			//	// branches there was not divided
			//	if(isContainer())
			//		return eject(pBranch);
			//}

//			return 0;
		}
		else
		{
			// way 2: indexes different. eject object
			return eject(pBranch);
		}
	};

	bool CTRoot::CEjector::eject( CTBranch* pBranch )
	{
		if(pBranch->isContainer())
		{
			size_t size = pBranch->getArray().size();
			// remove object from branch
			pBranch->getArray().erase(std::remove(pBranch->getArray().begin(),pBranch->getArray().end(),m_pObject), pBranch->getArray().end());
			// cleanup branch if needed
//			if(pBranch->isEmpty()&&pBranch->get_parent())
//			{
//				pBranch->get_parent()->eject(pBranch);
//				return 1;
//			}
//			if(size>pBranch->getArray().size())
//				return 1;

			// cleanup branch if needed
			if(size>pBranch->getArray().size())
			{
				if(pBranch->isEmpty()&&pBranch->get_parent())
					pBranch->get_parent()->eject(pBranch);
				return 1;
			}
		}
		return 0;
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// CDynamicTreeRoot
	//
	////////////////////////////////////////////////////////////////////////

	CDynamicTreeRoot::CDynamicTreeRoot( unsigned _ext, unsigned _base, int LimitDivisions )
		: CTRoot(_ext,_base,LimitDivisions), m_isApproximated(0)
	{
		// nothing
	};

	void CDynamicTreeRoot::inject( PObject obj )
	{
		m_arrayInject.push_back(obj);
	};

	void CDynamicTreeRoot::eject( PObject obj )
	{
		m_arrayEject.push_back(obj);
	};

	void CDynamicTreeRoot::ejectNow( PObject obj )
	{
		ejectDynamic(obj);
	};

	void CDynamicTreeRoot::set_pos( PObject obj, const point3f pos )
	{
		if(obj->getRoot())
			move(obj, pos);
		else
			m_sortMove[obj] = pos;
	};

	void CDynamicTreeRoot::setExt( PObject obj, const point3f ext )
	{
		m_sortChange[obj] = ext;
		if(obj->getRoot())
		{
			inject(obj);
			eject(obj);
		}
	};

	void CDynamicTreeRoot::move( PObject obj, const point3f& pos )
	{
		if(obj->getRoot())
			eject(obj);
		m_sortMove[obj] = pos;
		inject(obj);
	};

	void CDynamicTreeRoot::approximate()
	{
		if(m_isApproximated)
			return;

		m_isApproximated = 1;

		// 1) process eject query
		// eject static objects. ALL object there are STATIC
		if(m_arrayEject.size())
		{
			for(Array::iterator vt=m_arrayEject.begin(); vt!=m_arrayEject.end(); vt++)
				if((*vt)->getRoot())
					CTRoot::eject(*vt);
			m_arrayEject.clear();
		}

		// 2) reset dynamic objects, cleanup dynamic query
		if(m_arrayDynamic.size())
		{
			for(Array::iterator vt=m_arrayDynamic.begin(); vt!=m_arrayDynamic.end(); vt++)
				(*vt)->updatePos();
			m_arrayDynamic.clear();
		}

		// 3) process change query
		// set new extents
		if(m_sortChange.size())
		{
			for(Sorted::iterator vp=m_sortChange.begin(); vp!=m_sortChange.end(); vp++)
				(*vp).first->setExt((*vp).second);
			m_sortChange.clear();
		}

		// 4) process move query
		// set new pos
		if(m_sortMove.size())
		{
			for(Sorted::iterator vp=m_sortMove.begin(); vp!=m_sortMove.end(); vp++)
			{
				m_arrayDynamic.push_back((*vp).first);
				// call in this position ITTRee::set_pos(pos)
				(*vp).first->set_pos((*vp).second); 
			}
			m_sortMove.clear();
		}

		// 5) process inject query
		// this query create extended volume for dynamic object to sure what we inject them.
		// object what are not marked as dynamic injected as static
		if(m_arrayInject.size())
		{
			for(Array::iterator vt=m_arrayInject.begin(); vt!=m_arrayInject.end(); vt++)
				if(!(*vt)->getRoot())
					injectDynamic(*vt);
			m_arrayInject.clear();
		}

	};

	void CDynamicTreeRoot::finalize()
	{
		if(!m_isApproximated)
			return;

		m_isApproximated = 0;

		// 1) update dynamic objects in case of approximate changes
		if(m_arrayDynamic.size())
		{
			// extend eject query (we cant forget to cleanup our leafs!)
			std::copy(	m_arrayDynamic.begin(), m_arrayDynamic.end(), std::back_inserter(m_arrayEject) );
			// extend inject query ( we still got this object in tree!)
			std::copy(	m_arrayDynamic.begin(), m_arrayDynamic.end(), std::back_inserter(m_arrayInject) );
		}

		// 2) process eject query
		// this query create extended volume for dynamic object to sure what we clearly eject them.
		if(m_arrayEject.size())
		{
			for(Array::iterator vt=m_arrayEject.begin(); vt!=m_arrayEject.end(); vt++)
				if((*vt)->getRoot())
					ejectDynamic(*vt);
			m_arrayEject.clear();
		}

		// 3) cleanup dynamic objects old position.
		// update old pos from current position
		if(m_arrayDynamic.size())
		{
			for(Array::iterator vt=m_arrayDynamic.begin(); vt!=m_arrayDynamic.end(); vt++)
				(*vt)->updatePos();
		}

	// fixup objects

		// 4) process change query
		// set new extents
		if(m_sortChange.size())
		{
			for(Sorted::iterator vp=m_sortChange.begin(); vp!=m_sortChange.end(); vp++)
				(*vp).first->setExt((*vp).second);
			m_sortChange.clear();
		}

		// 5) process move query
		// set new pos
		if(m_sortMove.size())
		{
			for(Sorted::iterator vp=m_sortMove.begin(); vp!=m_sortMove.end(); vp++)
			{
//				m_arrayDynamic.push_back((*vp).first);
				// call in this position ITTRee::set_pos(pos)
				(*vp).first->set_pos((*vp).second); 
			}
			m_sortMove.clear();
		}

		// 6) process inject query
		// inject objects as static. ALL object there are stay as STATIC
		if(m_arrayInject.size())
		{
			for(Array::iterator vt=m_arrayInject.begin(); vt!=m_arrayInject.end(); vt++)
				if(!(*vt)->getRoot())
					CTRoot::inject(*vt);
			m_arrayInject.clear();
		}
	};

	bool CDynamicTreeRoot::isApproximated( void )
	{
		return m_isApproximated;
	};

	// get query with updated in last cycle objects
	Array& CDynamicTreeRoot::getDynamicObjects( void )
	{
		return m_arrayDynamic;
	};

	void  CDynamicTreeRoot::ejectDynamic( PObject obj )
	{
		if( !obj->isDynamic() )
			CTRoot::eject(obj);
		else
			CTRoot::eject(obj, &obj->getDynamicAABB());
	};

	void CDynamicTreeRoot::injectDynamic( PObject obj )
	{
		if( !obj->isDynamic() )
			CTRoot::inject(obj);
		else
			CTRoot::inject(obj, &obj->getDynamicAABB());
	};


}; //live_tree