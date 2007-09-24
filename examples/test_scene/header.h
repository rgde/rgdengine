#pragma once
#include <rgde/engine.h>
#include <boost/weak_ptr.hpp>
//////////////////////////////////////////////////////////////////////////

//#define DEFINE_PROPERTY(NAME, TYPE)\
//protected:\
//	TYPE m_##NAME;\
//public:\
//	inline void set##Name(const TYPE& value) {m_##NAME = value;}\
//	inline const TYPE& get##NAME() const {return m_##NAME;}
//
//#define REGISTER_PROPERTY(NAME, TYPE)\
//	addProperty(new TProperty<TYPE>(m_##NAME, #NAME, #TYPE));


class INamedObject : virtual public CPropertyOwner, virtual public CFunctionsOwner
{
public:
	INamedObject(std::string name) : m_Name(name)
	{
		REGISTER_PROPERTY(Name, std::string)
	}
	virtual ~INamedObject(){}

	DEFINE_PROPERTY(Name, std::string)
};

class CXmlClass : public INamedObject
{
public:
	CXmlClass() : INamedObject(""){}
	CXmlClass(std::string name) : INamedObject(name){}
	~CXmlClass(){}
};


namespace exceptions
{
	class NodeNotFound : public std::exception
	{
	public:
		NodeNotFound(std::string node_name) : 
		  std::exception(std::string("Node <" + node_name + "> not found!").c_str())
		  {
		  }
	};
}
class CXmlNode;
typedef boost::shared_ptr<CXmlNode> CXmlNodePtr;
typedef boost::weak_ptr<CXmlNode> CXmlNodeWeakPtr;
class CXmlNode : public CXmlClass
{
private:
	struct _searcher{
		std::string m_strName;
		_searcher(std::string name) : m_strName(name){}
		bool operator()(CXmlNodePtr node) {return node->getName() == m_strName;}
	};
public: 
	typedef std::list<CXmlNodePtr> ChildrenList;
	typedef ChildrenList::iterator ChildrenListIter;

	CXmlNode(std::string name)
	{
		this->setName(name);
	}	

	virtual ~CXmlNode(){}

	template <class T> 
	T get_Value(const std::string& path)
	{
		std::list<std::string> tok = base::tokenize<char>(".", path);
		std::string property_name (tok.back());
		tok.pop_back();
		CXmlNodePtr node = findNode(tok);
		std::string val = node->getProperty(property_name).get();
		return base::Lexical_cast<T>(val);
	}
	template <class T> 
	void set_Value(const std::string& path, const T& val)
	{
		std::list<std::string> tok = base::tokenize<char>(".", path);
		std::string property_name (tok.back());
		tok.pop_back();
		CXmlNodePtr node = findNode(tok);
		node->getProperty(property_name).set(base::Lexical_cast<std::string>(val));
	}
	void removeChild(CXmlNodePtr node)
	{
		m_lChildren.remove(node);
		node->resetParent();
	}

	ChildrenList& getChildrenList(){return m_lChildren;}

	CXmlNodePtr findNode(const std::string& node_name)
	{
		try{
			return findNode(base::tokenize<char>(".", node_name));		
		}
		catch(exceptions::NodeNotFound&)
		{
			throw exceptions::NodeNotFound(node_name);
		}		
	}

	CXmlNodePtr findNode(const std::list<std::string>& nodes_names)
	{
		CXmlNodePtr node = findChid(*nodes_names.begin());
		if (nodes_names.size() == 1)
			return node;
		else 
		{
			std::list<std::string> name(++nodes_names.begin(), nodes_names.end());
			return node->findNode(name);
		}
	}

	CXmlNodePtr findChid(const std::string& name)
	{
		ChildrenListIter it = std::find_if(m_lChildren.begin(), m_lChildren.end(), _searcher(name));

		if (it == m_lChildren.end())
			throw exceptions::NodeNotFound(name);
		else 
			return *it;
	}
protected:
	void resetParent(){m_pParent.reset();}
	void addChild(CXmlNodePtr node)		
	{
		m_lChildren.push_back(node);
	}
	void setParent(CXmlNodePtr node)
	{
		m_pParent = node;
	}
	friend void SetParentNode(CXmlNodePtr node, CXmlNodePtr parent);
protected:
	CXmlNodePtr	m_pParent;
	ChildrenList	m_lChildren;	
};
inline 
void SetParentNode(CXmlNodePtr node, CXmlNodePtr parent)
{
	node->setParent(parent);
	parent->addChild(node);
}


class CSomeObject : public CXmlNode{
public:
	CSomeObject() : CXmlNode("CSomeObject"), m_Value(115){
		REGISTER_PROPERTY(Value, int)
	}

	DEFINE_PROPERTY(Value, int)
};

class CSomeObject2 : public CXmlNode{
public:
	CSomeObject2() : CXmlNode("CSomeObject2"), m_Value(215){
		REGISTER_PROPERTY(Value, int)
	}

	DEFINE_PROPERTY(Value, int)
};

class CSomeObject3 : public CXmlNode{
public:
	CSomeObject3() : CXmlNode("CSomeObject3"), m_Value(315), m_DValue(3.14159){
		REGISTER_PROPERTY(Value, int)
		REGISTER_PROPERTY(DValue, double)
	}

	DEFINE_PROPERTY(DValue, double)
	DEFINE_PROPERTY(Value, int)
};

inline std::string getSpaces(int nSpaces = 10)
{
	std::string spaces;
	for (int i = 0; i < nSpaces; ++i)
		spaces += " ";
	return spaces;
}
inline void ShowNode(std::ostream& out, CXmlNodePtr obj, int nSpaces = 0)
{
	std::string spaces = getSpaces(nSpaces);

	CPropertyOwner::PropList &plist = obj->getProperties();
	out << getSpaces(nSpaces).c_str() << "Name: " << obj->getName().c_str()	<< "\n";
	int num = 0;
	for (CPropertyOwner::PropList::iterator it = plist.begin();
		it != plist.end(); ++it, ++num)
	{
		IProperty& p = *(*it);
		out << getSpaces(nSpaces+1).c_str() << p.getName() << "(" << p.getType()
			<< ") = " << p.get() << "\n";
	}

	CXmlNode::ChildrenList& list = obj->getChildrenList();
	for (CXmlNode::ChildrenListIter it = list.begin(); it != list.end(); ++it)
	{
		ShowNode(out, *it, nSpaces+5);
	}
}
