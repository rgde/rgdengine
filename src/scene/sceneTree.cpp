#include "precompiled.h"

#include "scene/sceneTree.h"

namespace scene
{	
	//struct node_deleter
	//{
	//	template<class T>
	//		void operator()(T* obj){delete obj;}
	//};

	void CNode::clear()
	{
		//std::for_each(m_children.begin(), m_children.end(), node_deleter());
		m_children.clear();
	}

	void CNode::update(double dTime, double dElapsedTime)
	{
		//for (ChildIterator it = m_children.begin(); it != m_children.end(); ++it)
		//{
		//	(*it)->update(dTime, dElapsedTime);
		//}
	}

	void CNode::toStream(io::IWriteStream& wf)
	{
		//unsigned num_children = (unsigned)m_children.size();
		//wf << num_children;
		//for (ChildrenList::iterator it = m_children.begin(); it != m_children.end(); ++it)
		//{
		//	wf << (*it)->getTypeName();
		//	wf << *(*it);
		//}
	}
	void CNode::fromStream(io::IReadStream& rf)
	{
		//unsigned num_children;
		//rf >> num_children;
		//for (unsigned i = 0; i < num_children; ++i)
		//{
		//	std::string type_name;
		//	rf >> type_name;
		//	CNode *n = factory::TFactory<CNode>::Create(type_name);
		//	rf >> *n;
		//	m_children.push_back(n);
		//}
	}


	void CSceneTree::toStream(io::IWriteStream& wf)
	{
		//wf << m_root_node;
	}
	void CSceneTree::fromStream(io::IReadStream& rf)
	{
		//rf >> m_root_node;
	}
}