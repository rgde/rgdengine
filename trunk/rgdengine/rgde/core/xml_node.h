#pragma once

#include <rgde/core/xml_class.h>

namespace core
{
	namespace exceptions
	{
		struct node_not_found : public std::exception
		{
			explicit node_not_found(const std::string& node_name);
		};

		inline node_not_found::node_not_found(const std::string& node_name)
			: std::exception(std::string("node <" + node_name + "> not found!").c_str())
		{
		}
	}

	template <class T>
	class tree_node
	{
	public:
		typedef T node;
		typedef boost::intrusive_ptr<node> node_ptr;
		typedef std::list<node_ptr> children_list;
		typedef typename children_list::const_iterator child_iter;

		void add(const node_ptr& node)
		{
			m_children.push_back(node); 
			node->parent((T*)this);
		}

		void remove(const node_ptr& node)
		{
			m_children.remove(node); 
			node->parent(0);
		}

		const node*		parent() const  {return m_parent;}
		node*			parent()		{return m_parent;}

		const children_list& children() const {return m_children;}

		tree_node() : m_parent(0)
		{}

		virtual ~tree_node()
		{
			for (children_list::iterator it = m_children.begin(); it != m_children.end(); ++it)
				(*it)->parent(0);

			m_children.clear();

			if (0 != m_parent)
			{
				m_parent->remove((T*)this);
				m_parent = 0;
			}
		}

		void parent(node* ptr)	{m_parent = ptr; on_parent_change();}

	protected:				
		virtual void on_parent_change(){}

	protected:
		children_list	m_children;
		node*			m_parent;
	};

	template <class node_type>
	class meta_node : public meta_class
					, public tree_node<node_type>
	{
	public: 
		typedef boost::intrusive_ptr<node_type> node_type_ptr;

		struct _searcher
		{			
			explicit _searcher(const std::string& name) 
				: m_name(name)
			{}

			bool operator()(const node_type_ptr& obj) const 
			{
				return obj->name() == m_name;
			}

			const std::string& m_name;
		};

		meta_node(const std::string& name) 
			: meta_class(name)
		{
		}

		virtual ~meta_node()
		{
		}

		node_type_ptr find(const std::string& node_name)
		{
			try
			{
				return find(base::tokenize<char>(".", node_name));		
			}
			catch(exceptions::node_not_found&)
			{
				throw exceptions::node_not_found(node_name);
			}		
		}

		node_type_ptr find(const std::list<std::string>& nodes_names)
		{
			node_type_ptr node = child(*nodes_names.begin());

			if (nodes_names.size() == 1)
				return node;
			else 
			{
				std::list<std::string> name;
				name.resize(nodes_names.size()-1);
				std::copy((++nodes_names.begin()), nodes_names.end(), name.begin());
				return node->find(name);
			}
		}

		node_type_ptr child(const std::string& name)
		{
			children_list::iterator it = std::find_if(m_children.begin(), m_children.end(), _searcher(name));

			if (it == m_children.end())
				throw exceptions::node_not_found(name);
			else 
				return *it;
		}
	};
}