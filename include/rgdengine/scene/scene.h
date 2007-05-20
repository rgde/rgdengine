#pragma once

#include "io/io.h"
#include "base/singelton.h"

//Neonic: octree
#include "scene/livetree.h"

namespace math
{
	typedef boost::intrusive_ptr<class CFrame> PFrame;
}

namespace scene
{
	class ISceneManager;
	typedef boost::shared_ptr<ISceneManager> PSceneManager;

	class CScene
		: public io::ISerializedObject
		//Neonic: octree
		, public live_tree::CDynamicTreeRoot
	{
	protected:
		CScene();
		virtual ~CScene();

	public:
		virtual math::PFrame& getRootFrame();

		virtual void addManager(const PSceneManager& manager);
		virtual void removeManager( const PSceneManager& manager );

		void debugDraw( );

	protected:
		void auxDraw( math::PFrame frame );
		virtual void toStream(io::IWriteStream& wf) const;
		virtual void fromStream(io::IReadStream& rf);

	private:
		std::list<PSceneManager> m_Managers;
		math::PFrame			 m_pRoot;
	};

	typedef base::TSingelton<CScene> TheScene;
}