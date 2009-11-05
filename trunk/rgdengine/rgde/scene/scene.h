#pragma once

#include <rgde/io/io.h>
#include <rgde/base/singelton.h>

//Neonic: octree
#include <rgde/scene/live_tree.h>

namespace math
{
	typedef boost::intrusive_ptr<class frame> frame_ptr;
}

namespace scene
{
	class ISceneManager;
	typedef boost::shared_ptr<ISceneManager> scene_manager_ptr;

	class Scene : public io::serialized_object				
				//, public live_tree::CDynamicTreeRoot //Neonic: octree
	{
	protected:
		Scene();
		virtual ~Scene();

	public:
		virtual math::frame_ptr& get_root();

		virtual void add_manager(const scene_manager_ptr& manager);
		virtual void removeManager( const scene_manager_ptr& manager );

		void debug_draw( );

	protected:
		void aux_draw( math::frame_ptr frame );
		virtual void to_stream(io::write_stream& wf) const;
		virtual void from_stream(io::read_stream& rf);

	private:
		std::list<scene_manager_ptr> m_managers;
		math::frame_ptr			 m_root;
	};

	typedef base::singelton<Scene> TheScene;
}