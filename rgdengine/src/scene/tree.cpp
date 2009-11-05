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

	void node::update(double time, double elapsed_time)
	{
		//for (child_iter it = m_children.begin(); it != m_children.end(); ++it)
		//{
		//	(*it)->update(time, elapsed_time);
		//}
	}

	void node::to_stream(io::write_stream& wf)
	{
		//unsigned num_children = (unsigned)m_children.size();
		//wf << num_children;
		//for (children_list::iterator it = m_children.begin(); it != m_children.end(); ++it)
		//{
		//	wf << (*it)->getTypeName();
		//	wf << *(*it);
		//}
	}
	void node::from_stream(io::read_stream& rf)
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

	scene_tree::scene_tree()
	: m_root_node("Root") 
	{
	}

	scene_tree::~scene_tree()
	{
	}

	void scene_tree::to_stream(io::write_stream& wf)
	{
		//wf << m_root_node;
	}
	void scene_tree::from_stream(io::read_stream& rf)
	{
		//rf >> m_root_node;
	}
}