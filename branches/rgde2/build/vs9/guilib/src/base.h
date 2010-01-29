#pragma once

#include <boost/intrusive_ptr.hpp>

//-----------------------------------------------------------------
template<class Ch>
std::list<std::basic_string<Ch> > tokenize(const std::basic_string<Ch>& delim, const std::basic_string<Ch> &src)
{
	std::list<std::basic_string<Ch> > out_tokens;

	for (size_t end	= 0; end != std::basic_string<Ch>::npos;)
	{
		size_t start = src.find_first_not_of(delim, end);
		end = src.find_first_of(delim, start);

		if (start != std::string::npos)
		{
			size_t end_pos = end == std::string::npos ? end : end - start;
			out_tokens.push_back(src.substr(start, end_pos));
		}
	}
	return out_tokens;
}
//-----------------------------------------------------------------
class  RefCounted 
{
public:
	RefCounted():m_refCount(0){}
	virtual ~RefCounted(){}
private: 
	int	m_refCount;
	friend void intrusive_ptr_add_ref(RefCounted*);
	friend void intrusive_ptr_release(RefCounted*);
};

inline void intrusive_ptr_add_ref(RefCounted* p)
{
	++(p->m_refCount);
}

inline void intrusive_ptr_release(RefCounted* p)
{
	--(p->m_refCount);

	if (p->m_refCount == 0)
		delete p;
}
//-----------------------------------------------------------------
class  NamedObject
{
public:
	NamedObject(const std::string& name);
	virtual ~NamedObject(){}

	const std::string& getName() const;
	void setName(const std::string& name);

private:
	std::string m_strName;
};
//-----------------------------------------------------------------
template <class T>
class TreeNode 
{
public:
	typedef T Node;
	typedef boost::intrusive_ptr<Node> PNode;
	typedef std::list<PNode> ChildrenList;
	typedef typename ChildrenList::iterator ChildrenIter;
	typedef typename ChildrenList::reverse_iterator ReverseChildrenIter;
	typedef typename ChildrenList::const_iterator ChildrenIterConst;

	virtual bool isCanHaveChildren() const = 0;

	void addChild(const PNode& node)
	{
		addChild(node, m_children.end());
	}

	void addChild(const PNode& node, ChildrenIter iwhere)
	{
		if(!isCanHaveChildren()) return;

		if (node && node != this && node->getParent() != this)
		{
			node->resetParent();

			PNode n = node;			
			m_children.insert(iwhere, n);
			onChildAdd(n);
			n->setParent((T*)this);
		}
	}

	void removeChild(const PNode& node)
	{
		if(!isCanHaveChildren()) return;

		if (node && node != this && node->getParent() == this)
		{
			PNode n = node;
			m_children.remove(n);
			onChildRemove(n);
			node->setParent(0);
		}
	}

	ChildrenIter getOrder(const PNode& node) 
	{
		ChildrenIter f = std::find(m_children.begin(), m_children.end(), node);
		if(f == m_children.begin())
			++f;
		else
			--f;
		return f;
	}

	const T* getParent() const  {return m_parent;}
	T*		 getParent()		{return m_parent;}
	T*		 getParentConst() const		{return m_parent;}

	ChildrenList& getChildren() {return m_children;}
	const ChildrenList& getChildren() const {return m_children;}

	TreeNode() : m_parent(0){}
	virtual ~TreeNode()
	{
		for (ChildrenList::iterator it = m_children.begin(); it != m_children.end(); ++it) 
		{
			(*it)->setParent(0);
		}
	}

	struct _searcher{
		const std::string& m_strName;
		_searcher& operator=(const _searcher&) {}
		_searcher(const std::string& name) : m_strName(name){}
		bool operator()(PNode obj) 
		{
			return obj ? (obj->getName() == m_strName) : false;
		}
	};

	PNode findNode(const std::string& node_name)
	{
		return findNode(tokenize<char>(".", node_name));
	}

	PNode findNode(const std::list<std::string>& nodes_names)
	{
		PNode node = findChild(*nodes_names.begin());

		if (!node) return node; // returns PNode();
		
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

	PNode findChild(const std::string& name)
	{
		ChildrenList::iterator it = std::find_if(m_children.begin(), m_children.end(), _searcher(name));

		if (it == m_children.end())
			return PNode();
		else 
			return *it;
	}

protected:
	void setParent(T* node)
	{
		m_parent = node; 
		onParentChange();
	}

	void resetParent()
	{
		if(m_parent)
			m_parent->removeChild(static_cast<T*>(this));
	}

	virtual void onParentChange()			{}
	virtual void onChildAdd(PNode& node)	{node;}
	virtual void onChildRemove(PNode& node) {node;}

protected:
	ChildrenList m_children;
	T* m_parent;
};
