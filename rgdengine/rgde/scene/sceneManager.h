#pragma once

namespace scene
{
	class ICollisionVolume;
	//class ISceneManager
	//{
	//public:
	//	virtual ~ISceneManager(){}

	//	virtual std::list<render::Mesh*> getObjectInsideVolume( ICollisionVolume* volume ) = 0;

	//	virtual ISceneVisitor* get_visitor()const = 0;
	//};

	//typedef boost::shared_ptr<ISceneManager> PSceneManager;

	//class MeshVisitor: public ISceneVisitor
	//{
	//private:
	//	std::list<render::Mesh*>& m_Meshes;
	//public:
	//	MeshVisitor( std::list<render::Mesh*>& meshes ): m_Meshes( meshes )
	//	{
	//	}

	//	~MeshVisitor()
	//	{
	//	}

	//	bool visit( math::Frame* )
	//	{
	//		return false;
	//	}

	//	bool visit( render::Mesh* mesh )
	//	{
	//		m_Meshes.push_back( mesh );
	//		return true;
	//	}
	//};

	//class ListSceneManager: public ISceneManager
	//{
	//private:
	//	std::list<render::Mesh*> m_Meshes;
	//	MeshVisitor* m_Visitor;
	//public:
	//	ListSceneManager()
	//	{
	//		m_Visitor = new MeshVisitor( m_Meshes );
	//	}

	//	~ListSceneManager()
	//	{
	//	}

	//	ISceneVisitor* get_visitor()const
	//	{
	//		return m_Visitor;
	//	}

	//	std::list<render::Mesh*> getObjectInsideVolume( ICollisionVolume* volume )
	//	{
	//		return m_Meshes;
	//	}
	//};
}
