#pragma once

#include <rgde/io/io.h>
#include <rgde/base/singelton.h>

//Neonic: octree
#include <rgde/scene/live_tree.h>

namespace math
{
	typedef boost::intrusive_ptr<class Frame> frame_ptr;
}

namespace scene
{
	class ISceneManager;
	typedef boost::shared_ptr<ISceneManager> scene_manager_ptr;

	class Scene
		: public io::serialized_object
		//Neonic: octree
		, public live_tree::CDynamicTreeRoot
	{
	protected:
		Scene();
		virtual ~Scene();

	public:
		virtual math::frame_ptr& getRootFrame();

		virtual void addManager(const scene_manager_ptr& manager);
		virtual void removeManager( const scene_manager_ptr& manager );

		void debugDraw( );

	protected:
		void auxDraw( math::frame_ptr frame );
		virtual void toStream(io::write_stream& wf) const;
		virtual void fromStream(io::read_stream& rf);

	private:
		std::list<scene_manager_ptr> m_Managers;
		math::frame_ptr			 m_pRoot;
	};

	typedef base::singelton<Scene> TheScene;
}