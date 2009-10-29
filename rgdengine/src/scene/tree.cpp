#include "precompiled.h"

#include <rgde/scene/tree.h>

namespace scene
{	
	//struct node_deleter
	//{
	//	template<class T>
	//		void operator()(T* obj){delete obj;}
	//};

	void node::clear()
	{
		//std::for_each(m_children.begin(), m_children.end(), node_deleter());
		m_children.clear();
	}

	void node::update(double dTime, double dElapsedTime)
	{
		//for (child_iter it = m_children.begin(); it != m_children.end(); ++it)
		//{
		//	(*it)->update(dTime, dElapsedTime);
		//}
	}

	void node::toStream(io::IWriteStream& wf)
	{
		//unsigned num_children = (unsigned)m_children.size();
		//wf << num_children;
		//for (children_list::iterator it = m_children.begin(); it != m_children.end(); ++it)
		//{
		//	wf << (*it)->getTypeName();
		//	wf << *(*it);
		//}
	}
	void node::fromStream(io::IReadStream& rf)
	{
		//unsigned num_children;
		//rf >> num_children;
		//for (unsigned i = 0; i < num_children; ++i)
		//{
		//	std::string type_name;
		//	rf >> type_name;
		//	node *n = core::factory<node>::create(type_name);
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