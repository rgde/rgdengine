#pragma once

#include <rgde/core/xml_class.h>

namespace core
{
	namespace exceptions
	{
		struct node_not_found : public std::exception
		{
			node_not_found(const std::string& node_name) : 
			  std::exception(std::string("node <" + node_name + "> not found!").c_str())
			  {
			  }
		};
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
			node->set_parrent((T*)this);
		}

		void remove(const node_ptr& node)
		{
			m_children.remove(node); 
			node->set_parrent(0);
		}

		const node_ptr& get_parent() const  {return m_parent;}
		node_ptr		get_parent()			{return m_parent;}

		//children_list& get_children() {return m_children;}
		const children_list& get_children() const {return m_children;}

		tree_node() : m_parent(0)
		{}

		virtual ~tree_node()
		{
			for (children_list::iterator it = m_children.begin(); it != m_children.end(); ++it)
				(*it)->set_parrent(0);

			m_children.clear();

			if (0 != m_parent)
			{
				m_parent->remove((T*)this);
			}
		}

	protected:
				void set_parrent(node* node){m_parent = node; on_parent_change();}
		virtual void on_parent_change(){}

	protected:
		children_list m_children;
		node_ptr		 m_parent;
	};

	template <class T>
	class meta_node : public meta_class, public tree_node<T>
	{
	public: 
		typedef boost::intrusive_ptr<T> _PT;

		struct _searcher
		{
			const std::string& m_name;
			_searcher(const std::string& name) : m_name(name){}
			bool operator()(const _PT& obj) const {return obj->get_name() == m_name;}
		};

		meta_node(const std::string& name) : meta_class(name)
		{
		}

		virtual ~meta_node()
		{
		}

		_PT find_node(const std::string& node_name)
		{
			try{
				return find_node(base::tokenize<char>(".", node_name));		
			}
			catch(exceptions::node_not_found&)
			{
				throw exceptions::node_not_found(node_name);
			}		
		}

		_PT find_node(const std::list<std::string>& nodes_names)
		{
			_PT node = find_child(*nodes_names.begin());
			if (nodes_names.size() == 1)
				return node;
			else 
			{
				std::list<std::string> name;
				name.resize(nodes_names.size()-1);
				std::copy((++nodes_names.begin()), nodes_names.end(), name.begin());
				return node->find_node(name);
			}
		}

		_PT find_child(const std::string& name)
		{
			children_list::iterator it = std::find_if(m_children.begin(), m_children.end(), _searcher(name));

			if (it == m_children.end())
				throw exceptions::node_not_found(name);
			else 
				return *it;
		}
	};
}