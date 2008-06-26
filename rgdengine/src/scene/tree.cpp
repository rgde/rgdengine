#include "precompiled.h"

#include <rgde/scene/tree.h>

namespace scene
{	
	//struct node_deleter
	//{
	//	template<class T>
	//		void operator()(T* obj){delete obj;}
	//};

	void Node::clear()
	{
		//std::for_each(m_children.begin(), m_children.end(), node_deleter());
		m_children.clear();
	}

	void Node::update(double dTime, double dElapsedTime)
	{
		//for (ChildIterator it = m_children.begin(); it != m_children.end(); ++it)
		//{
		//	(*it)->update(dTime, dElapsedTime);
		//}
	}

	void Node::toStream(io::IWriteStream& wf)
	{
		//unsigned num_children = (unsigned)m_children.size();
		//wf << num_children;
		//for (ChildrenList::iterator it = m_children.begin(); it != m_children.end(); ++it)
		//{
		//	wf << (*it)->getTypeName();
		//	wf << *(*it);
		//}
	}
	void Node::fromStream(io::IReadStream& rf)
	{
		//unsigned num_children;
		//rf >> num_children;
		//for (unsigned i = 0; i < num_children; ++i)
		//{
		//	std::string type_name;
		//	rf >> type_name;
		//	Node *n = factory::TFactory<Node>::Create(type_name);
		//	rf >> *n;
		//	m_children.push_back(n);
		//}
	}


	void SceneTree::toStream(io::IWriteStream& wf)
	{
		//wf << m_root_node;
	}
	void SceneTree::fromStream(io::IReadStream& rf)
	{
		//rf >> m_root_node;
	}
}