/******************************************************************************
@file    SceneManager.h
@author  Neonic
@mail    gaba@male.ru
@project RGDE
@date    22.07.2006
@desc    scene manager
 ******************************************************************************/

#pragma once


namespace scene
{
	// объект сцены
	class SceneObject
		: public math::CFrame
	{
	public:

	};

	struct ISceneManager;
	typedef boost::shared_ptr<ISceneManager> PManager;

	class Scene
		: public SceneObject
	{
	public:
		Scene( void );

		void			setManager(PManager manager);
		PManager		getManager( void ) { return m_manager; };

	protected:
		// по умолчанию создается дефолтный менеджер который ничего не оптимизирует
		PManager	m_manager;
	};

	struct ISceneManager
	{
		typedef std::vector<SceneObject> vecArray;

		virtual void	onObjectAdd( SceneObject* ) = 0;
		virtual void	onObjectRemove( SceneObject* ) = 0;

		virtual void	getObjectsInside( render::PCamera cam, vecArray& objs) = 0;
		virtual void	getObjectsInside( const math::Spheref& sphere, vecArray& objs) = 0;
		virtual void	getObjectsInside( const math::AABoxf& box, vecArray& objs) = 0;
	};
 
}

/*
namespace scene
{
	////////////////////////////////////////////////////////////////////////
	// 
	// defs & deps
	// 
	////////////////////////////////////////////////////////////////////////

	// типы пространственных разбиений для менеджера сцены
	enum eSMTypes
	{
		ESM_NONE		= 0,

		ESM_OCTREE,				// oct-tree, Neonic's LiveTree (division-positioning algo)
		ESM_QUADTREE,			// quad-tree patritioning
		ESM_BSP,				// BSP (binary space partitioning)
		ESM_PORTAL,				// PSP (portal space partitioning)

		ESM_TOTAL,
	};

	// scene managed object
	class ISMObject;
	class ISceneManager;

	typedef boost::intrusive_ptr<ISMObject>		PSMObject;
	typedef boost::intrusive_ptr<ISceneManager> PSceneMgr;
	typedef std::list<PSMObject>				PSMArray;


	////////////////////////////////////////////////////////////////////////
	// 
	// CGlobalUpdater
	// 
	////////////////////////////////////////////////////////////////////////

	class CGlobalUpdater
	{
	public:
		void reg( PSMObject pObj )		{ m_array.push_back(pObj); };
		void unreg( PSMObject pObj )	{ m_array.remove(pObj); };

		// последовательный перебор массива с вызовом метода обновления
		void update( void );

	private:
		PSMArray		m_array;
	};

	typedef base::TSingelton<CGlobalUpdater> TheUpdater;


	////////////////////////////////////////////////////////////////////////
	// 
	// ISMObject
	// 
	////////////////////////////////////////////////////////////////////////

	class ISMObject
		: public base::CBaseRefCounted 
	{
		// я просто существую в сцене
	public:
		ISMObject( ISceneManager* pMan =0 ) : m_pTree(pMan) {};

		// метод обновления
		virtual void	update( void ) {};

		// register/unregister self
		void			reg_updater( void );
		void			unreg_updater( void );

		// manage

		PSceneMgr		getManager() const				{ return m_pSceneMgr; };
		ISceneManager*	getManager()					{ return m_pSceneMgr.get(); };

		void			setManager( PSceneMgr pMgr )	{ m_pSceneMgr = pMgr; };

	private:
		// наш менеджер сцены
		PSceneMgr m_pSceneMgr;
	};

	// testing purpose
	// 
	// 

	// example
	class CSMObject
		: public ISMObject
	{
	public:

		// apply forces to object on call
		void update( void )
		{
			math::Point3f move, rot;
			move = m_MoveForce + m_MoveAcc;
			rot = m_RotForce + m_RotAcc;

			// apply resulting forces
		};

		// instant force move
		void setMoveAcc( const math::Point3f& pt)	{ m_MoveAcc = pt; };
		void addMoveAcc( const math::Point3f& pt)	{ m_MoveAcc += pt; };
		const math::Point3f& getMoveAcc( void )		{ return m_MoveAcc; };

		// instant force rotation
		void setRotAcc( const math::Point3f& pt)	{ m_RotAcc = pt; };
		void addRotAcc( const math::Point3f& pt)	{ m_RotAcc += pt; };
		const math::Point3f& getRotAcc( void )		{ return m_RotAcc; };

		// constant force move
		void setMoveForce( const math::Point3f& pt)	{ m_MoveForce = pt; };
		void addMoveForce( const math::Point3f& pt)	{ m_MoveForce += pt; };
		const math::Point3f& getMoveForce( void )	{ return m_MoveForce; };

		// constant force rotation
		void setRotForce( const math::Point3f& pt)	{ m_RotForce = pt; };
		void addRotForce( const math::Point3f& pt)	{ m_RotForce += pt; };
		const math::Point3f& getRotForce( void )	{ return m_RotForce; };

		// acceleration - instant force
		math::Point3f	m_MoveAcc, m_RotAcc;
		// force - contant force
		math::Point3f	m_MoveForce, m_RotForce;

	};


	////////////////////////////////////////////////////////////////////////
	// 
	// ISceneManager
	//	scene manager interface
	////////////////////////////////////////////////////////////////////////

	class ISceneManager
		: public ISMObject
	{
		// я знаю о своих детях, как о позиционированных в пространстве объектах

	public:
		// request type
		virtual eSMTypes	getSMType( void ) { return ESM_NONE; };

		// clear
		virtual void		clear( void ) {};
		// debug draw
		virtual void		draw( void ) {};
		// inject object
		virtual void		inject( PSMObject pObj, AABoxf* pAABB =0 ) {};
		// eject object
		virtual void		eject( PSMObject pObj, AABoxf* pAABB =0 ) {};
		// eject object now in any case
		virtual void		ejectNow( PSMObject pObj ) {};
		// set object new position
		virtual void		setPos( PSMObject pObj, const Point3f pos ) {};
		// set object new extents
		virtual void		setExt( PSMObject pObj, const Point3f ext ) {};
		// move object to new position
		virtual void		move( PSMObject pObj, const Point3f& pos ) {};

		// collector
		virtual bool		collect( const AABoxf& aabb, PSMArray& coll, PSMObject exclude =0, PSMArray& coll ) { return 0; };
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// IHaveSceneManager
	// 
	////////////////////////////////////////////////////////////////////////

	class IHaveSceneManager
		: public ISMObject
	{
		// я могу иметь менеджер сцены, получать и назначать его,
		// поэтому используйте меня для выведения сцен (и других объектов)

	public:
		// constructor
		IHaveSceneManager( PSceneMgr pMgr )		{ setManager(pMgr); };

	};

}; // namespace scene
*/