#include "test_physic.h"

namespace scene
{
	PManager Create( void );

	Scene::Scene( void )
		: m_manager(Create())
	{
		// 
	};

	Scene::setManager( PManager manager )
	{
		// 
	};

	class CSceneManager
		: public ISceneManager
	{
	public:
		void			onObjectAdd( SceneObject* );
		void			onObjectRemove( SceneObject* );

		void			getObjectsInside( render::PCamera cam, vecArray& objs);
		void			getObjectsInside( const math::Spheref& sphere, vecArray& objs);
		void			getObjectsInside( const math::AABoxf& box, vecArray& objs);
	};

	/** \brief Create default scene manager
	*/
	PManager Create( void )
	{
		return PManager(new CSceneManager());
	};

	/*
	////////////////////////////////////////////////////////////////////////
	// 
	// CGlobalUpdater
	// 
	////////////////////////////////////////////////////////////////////////

	void CGlobalUpdater::update( void )
	{
		if(m_array.size())
		{
			for(PSMArray::iterator vt=m_array.begin(); vt!=m_array.end(); vt++)
				(*vt)->update();
		}
	};


	////////////////////////////////////////////////////////////////////////
	// 
	// ISMObject
	// 
	////////////////////////////////////////////////////////////////////////

	void ISMObject::reg_updater( void )
	{
		scene::TheUpdater::Get().reg(this);
	};

	void ISMObject::unreg( void )
	{
		scene::TheUpdater::Get().unreg_updater(this);
	};
	*/

}; //namespace scene