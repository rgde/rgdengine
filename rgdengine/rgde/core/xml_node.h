#pragma once

#include <rgde/core/xml_class.h>

namespace core
{
	namespace exceptions
	{
		struct NodeNotFound : public std::exception
		{
			NodeNotFound(const std::string& node_name) : 
			  std::exception(std::string("Node <" + node_name + "> not found!").c_str())
			  {
			  }
		};
	}

	template <class T>
	class TreeNode
	{
	public:
		typedef T Node;
		typedef boost::intrusive_ptr<Node> PNode;
		typedef std::list<PNode> ChildrenList;
		typedef typename ChildrenList::const_iterator ChildIterator;

		void addChild(const PNode& node)
		{
			m_children.push_back(node); 
			node->setParent((T*)this);
		}

		void removeChild(const PNode& node)
		{
			m_children.remove(node); 
			node->setParent(0);
		}

		const PNode& getParent() const  {return m_parent;}
		PNode		 getParent()		{return m_parent;}

		//ChildrenList& getChildren() {return m_children;}
		const ChildrenList& getChildren() const {return m_children;}

		TreeNode() : m_parent(0)
		{}

		virtual ~TreeNode()
		{
			for (ChildrenList::iterator it = m_children.begin(); it != m_children.end(); ++it)
				(*it)->setParent(0);

			m_children.clear();

			if (0 != m_parent)
			{
				m_parent->removeChild((T*)this);
			}
		}

	protected:
				void setParent(Node* node){m_parent = node; onParentChange();}
		virtual void onParentChange(){}

	protected:
		ChildrenList m_children;
		PNode		 m_parent;
	};

	template <class T>
	class XmlNode : public meta_class, public TreeNode<T>
	{
	public: 
		typedef boost::intrusive_ptr<T> _PT;

		struct _searcher
		{
			const std::string& m_name;
			_searcher(const std::string& name) : m_name(name){}
			bool operator()(const _PT& obj) const {return obj->get_name() == m_name;}
		};

		XmlNode(const std::string& name) : meta_class(name)
		{
		}

		virtual ~XmlNode()
		{
		}

		_PT findNode(const std::string& node_name)
		{
			try{
				return findNode(base::tokenize<char>(".", node_name));		
			}
			catch(exceptions::NodeNotFound&)
			{
				throw exceptions::NodeNotFound(node_name);
			}		
		}

		_PT findNode(const std::list<std::string>& nodes_names)
		{
			_PT node = findChild(*nodes_names.begin());
			if (nodes_names.size() == 1)
				return node;
			else 
			{
				std::list<std::string> name;
				name.resize(nodes_names.size()-1);
				std::copy((++nodes_names.begin()), nodes_names.end(), name.begin());
				return node->findNode(name);
			}
		}

		_PT findChild(const std::string& name)
		{
			ChildrenList::iterator it = std::find_if(m_children.begin(), m_children.end(), _searcher(name));

			if (it == m_children.end())
				throw exceptions::NodeNotFound(name);
			else 
				return *it;
		}
	};
}