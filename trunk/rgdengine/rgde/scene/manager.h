#pragma once

namespace scene
{
	class ICollisionVolume;
	//class ISceneManager
	//{
	//public:
	//	virtual ~ISceneManager(){}

	//	virtual std::list<render::mesh*> getObjectInsideVolume( ICollisionVolume* volume ) = 0;

	//	virtual base_visitor* get_visitor()const = 0;
	//};

	//typedef boost::shared_ptr<ISceneManager> scene_manager_ptr;

	//class MeshVisitor: public base_visitor
	//{
	//private:
	//	std::list<render::mesh*>& m_Meshes;
	//public:
	//	MeshVisitor( std::list<render::mesh*>& meshes ): m_Meshes( meshes )
	//	{
	//	}

	//	~MeshVisitor()
	//	{
	//	}

	//	bool visit( math::frame* )
	//	{
	//		return false;
	//	}

	//	bool visit( render::mesh* mesh )
	//	{
	//		m_Meshes.push_back( mesh );
	//		return true;
	//	}
	//};

	//class ListSceneManager: public ISceneManager
	//{
	//private:
	//	std::list<render::mesh*> m_Meshes;
	//	MeshVisitor* m_Visitor;
	//public:
	//	ListSceneManager()
	//	{
	//		m_Visitor = new MeshVisitor( m_Meshes );
	//	}

	//	~ListSceneManager()
	//	{
	//	}

	//	base_visitor* get_visitor()const
	//	{
	//		return m_Visitor;
	//	}

	//	std::list<render::mesh*> getObjectInsideVolume( ICollisionVolume* volume )
	//	{
	//		return m_Meshes;
	//	}
	//};
}
