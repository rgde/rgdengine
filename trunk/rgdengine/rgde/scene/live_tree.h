/******************************************************************************
@file    live_tree.h
@author  Neonic
@mail    gaba@male.ru
@project RGDE
@date    12.05.2006
@desc    Шаблон AABB дерева
 ******************************************************************************/

//Neonic: octree

#pragma once

#pragma warning(disable : 4244)	// type conversion warning.
#pragma warning(disable : 4305)	// truncation from 'const double' to 'float'
#pragma warning(disable : 4800)	// warning C4800: forcing value to bool 'true' or 'false' (performance warning)

#include <rgde/base/smart_ptr_helpers.h>

namespace live_tree
{
	using namespace math;

	// used axis
	enum eAxis {
		AXIS_X				= 1,
		AXIS_Y				= 2,
		AXIS_Z				= 4,
	};

	class CFlags {
	public:
		CFlags( unsigned flags=0 ) : m_Flags(flags) {};
		const unsigned&			getFlags( void ) const					{ return m_Flags; };
		bool					getFlags( unsigned flags )				{ return (m_Flags & flags); };
		void					setFlags( unsigned flags )				{ m_Flags = flags; };
		void					changeFlags( unsigned flags, bool b )	{
			if(b)	m_Flags |= flags;
			else	m_Flags &= ~flags;
		};

	private:
		unsigned			m_Flags;
	};

	class TreeObject;
	class CTBranch;
	class CTRoot;

	typedef boost::intrusive_ptr<TreeObject> PObject;

	typedef std::vector<PObject>			Array;
	typedef std::map<PObject, point3f>		Sorted;

	/// Найти ближайшее число в прогрессии(out), включающее заданное число(in)
	bool calc_progression( const int &in, int &out );


	////////////////////////////////////////////////////////////////////////
	// 
	// TreeObject
	//
	////////////////////////////////////////////////////////////////////////

	class TreeObject
		: virtual public base::refcounted
	{
	public:

		TreeObject( void );
		virtual ~TreeObject( void ) ;

		// get object AABB
		aaboxf					getAABB( void );
		// get object dynamic AABB. sure you call this func then object dynamic, otherwise it fail.
		aaboxf					getDynamicAABB( void );
		// set object dimensions as aabb
		void					setAABB( const aaboxf& aabb );
		// get object position
		const point3f&			get_pos( void ) const;
		// set object position
		void					set_pos( const point3f& pos );
		// get object extents
		const point3f&			getExt( void ) const;
		// set object extents
		void					setExt( const point3f& ext );
		// inject object into selected tree
		void					inject( CTRoot* p );
		// eject object from tree
		void					eject( void );
		// move object in tree. sure you inject object before, otherwise it fail
		void					move( const point3f& pos );
		// set object tree root
		void					setRoot( CTRoot* p );
		// get object tree root
		CTRoot*					getRoot( void );
		// debug draw
		void					draw( void );
		// eject object from tree
//		void					remSelf( void );
	// dynamic positioning
		// func make object dynamic. you must position object before make them dynamic.
		// in case of many changes in tree do next in query: create obj, set pos, make dynamic, inject
		void					makeDynamic( void );
		// convert back to static from dynamic. it can happen only before Approximate() in dynamic begin.
		bool					makeStatic( void );
		// test for dynamic enabled
		bool					is_dynamic( void );
		// get last updated pos
		point3f					getOldPos( void );
		// update pos from current
		void					updatePos( void );

	private:
		CTRoot*				m_root;

		point3f*			m_pPosOld;
		point3f				m_Position;
		point3f				m_Ext;

	};


	////////////////////////////////////////////////////////////////////////
	// 
	// CTBranch
	//
	////////////////////////////////////////////////////////////////////////

	class CTBranch
	{
	public:
		CTBranch( void );
		~CTBranch( void );

		// test branch for object containment
		bool					isContainer( void );
		// test branch for branches containment
		bool					isBranch( void );
		// test for both containments. also used as canDelete answer
		bool					isEmpty( void );
		// get object container
		Array&					getArray( void );
		// get parent branch
		CTBranch*				get_parent( void );
		// set parent branch
		void					set_parent( CTBranch* p );
		// get branch pointer. test them for existance before use
		CTBranch*				getBranch( unsigned iIndex );
		// call branch. create branch if it not exist
		CTBranch*				call( unsigned iIndex );
		// eject contained branch
		void					eject( CTBranch* pBranch );
		// clear both containments
		void					clear( void );
		// debug draw
		void					draw( void );

	private:

		//		union
//		{
			// все ветви
			CTBranch*			m_Branches[8];
			// Сортированные ветви - пары: upperlevel-downlevel, top-bottom, right-left
			// left equal X+ axis, top equal Y+ axis, upperlevel equal Z+ axis
			// 0 - ветвь все оси которой =0, 7 - ветвь все оси которой =1
			// Сортировка идет в порядке X>Y>Z
//			CTBase				*m_lbd, *m_rbd, *m_ltd, *m_rtd,
//								*m_lbu, *m_rbu, *m_ltu, *m_rtu;
//		};
		/// child nodes count
		int					m_iUsed;
		Array				m_Array;
		CTBranch*			m_pParent;
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// CTRoot
	//
	////////////////////////////////////////////////////////////////////////

	class CTRoot
		: virtual public base::refcounted
	{
	public:
		// ext is maximum tree extents, base is minimum container size
		CTRoot( unsigned _ext=2147483647, unsigned _base=8, int LimitDivisions =-1 );
		virtual ~CTRoot( void );

		// clear main (static) branch
		void					clear( void );
		// test object containment in selected branch. function is obsolete
//		bool					test_obj_in_branch( PObject pBranch, PObject obj );
		// get tree base
		unsigned				getBase( void );
		// get tree extents
		int						getExt( void );
		// get main branch
		CTBranch*				getBranch( void );
		// set main branch
		void					setBranch( CTBranch* p );
		// debug draw
		void					draw( void );

	// next funcs produce 'static/immediate' effect in action
	// overwrite all of them if you need 'Another' correct workflow
		// inject object
		virtual void			inject( PObject obj, aaboxf* pAABB =0 );
		// eject object
		virtual void			eject( PObject obj, aaboxf* pAABB =0 );
		// eject object now in any case
		virtual void			ejectNow( PObject obj );
		// set object new position
		virtual void			set_pos( PObject obj, const point3f pos );
		// set object new extents
		virtual void			setExt( PObject obj, const point3f ext );
		// move object to new position
		virtual void			move( PObject obj, const point3f& pos );

		// get current query status for dynamic. static tree always finalized
		virtual bool			isApproximated( void ) { return 0; };

		class CCollector
		{
		public:
			CCollector( CTRoot* pRoot, const aaboxf& aabb, PObject exclude =0);
			Array&				operator()( void );
		
		private:
			void				parse( CTBranch* pBranch, const point3f& cnt, const float& ext );
			void				collect( CTBranch* pBranch );
			void				call( CTBranch* pBranch, const unsigned& iIndex, const point3f& cnt, const float& ext );

			PObject		m_pExcludeObject;
			CTRoot*		m_root;
			aaboxf		m_AABB;

		public:
			Array		m_Array;
		};

		class CInjector
		{
		public:
			CInjector( PObject p, CTRoot* pRoot, const aaboxf& aabb, int LimitDivisions =-1);
			bool				operator()( void );
		
		private:
			bool				parse( CTBranch* pBranch, const point3f& cnt, const float& ext, const unsigned& division );
			bool				inject( CTBranch* pBranch, const unsigned& division );

			PObject		m_pObject;
			CTRoot*		m_root;
			aaboxf		m_AABB;
			int			m_LimitDivisions;
		};

		class CEjector
		{
		public:
			CEjector( PObject p, CTRoot* pRoot, const aaboxf& aabb, int LimitDivisions =-1 );
			bool				operator()( void );
		
		private:
			bool				parse( CTBranch* pBranch, const point3f& cnt, const float& ext, const unsigned& division );
			bool				eject( CTBranch* pBranch );

			PObject		m_pObject;
			CTRoot*		m_root;
			aaboxf		m_AABB;
			int			m_LimitDivisions;
		};

	private:
		unsigned		m_iBase;
		int				m_iExt,
						m_LimitDivisions;
		CTBranch*		m_pBranch;

		unsigned		m_iObjInTree,				// number of objects in tree
						m_iDeepLevel;				// most deep level
	};

	////////////////////////////////////////////////////////////////////////
	// 
	// CDynamicTreeRoot
	//	dynamic tree with query
	////////////////////////////////////////////////////////////////////////

	class CDynamicTreeRoot
		: public CTRoot
	{
	public:
		CDynamicTreeRoot( unsigned _ext=2147483647, unsigned _base=8, int LimitDivisions =-1 );

		// Neonic: write override for clear... if it needed

		// inject object
		void					inject( PObject obj );
		// eject object
		void					eject( PObject obj );
		// eject object now in any case. correctly eject dynamic object
		void					ejectNow( PObject obj );
		// set object new position
		void					set_pos( PObject obj, const point3f pos );
		// set object new extents
		void					setExt( PObject obj, const point3f ext );
		// move object to new position
		void					move( PObject obj, const point3f& pos );
		// process approximation with deferred queryes before asking position.
		virtual void			approximate( void );
		// process objects finalizationt. you can make object static after this query(use isApproximated())
		virtual void			finalize( void );
		// get current query status: approximated or finalized
		bool					isApproximated( void );
		// get container with object, who updated/moved at last cycle
		Array&					getDynamicObjects( void );

	private:
		// eject object in case of their state.
		void					ejectDynamic( PObject obj );
		// inject object in case of their state
		void					injectDynamic( PObject obj );

		bool			m_isApproximated;
		Array			m_arrayEject, m_arrayInject, m_arrayDynamic;
		Sorted			m_sortMove, m_sortChange;
	};

	////////////////////////////////////////////////////////////////////////
	// 
	// TStaticTreeRoot
	//	static tree without query
	////////////////////////////////////////////////////////////////////////

	class TStaticTreeRoot
		: public CTRoot
	{
	public:
	};

}; //live_tree