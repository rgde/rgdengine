//////////////////////////////////////////////////////////////////////////
// @date 06.01.06
// project: RGDE 
// description: Base XML script services
// @todo perform full export, import and update
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "core/xmlNode.h"
#include "base/singelton.h"

namespace script{
	
	// Базовый интерфейс
	class IAbstractAdaptor : public core::XmlNode<IAbstractAdaptor>
	{
	public:
		IAbstractAdaptor(const std::string& name) : core::XmlNode<IAbstractAdaptor>(name){};
		virtual ~IAbstractAdaptor(){};

		virtual const std::string	getType(void) const = 0;

		virtual core::XmlClass& getObject(void) = 0;
		// this call can be used to parse children
		virtual void parse(void){};
		
		// other public methods:
		// getProperty()
		// callFunction(), and so on...
	};
}// end script

inline TiXmlNode& operator<<(TiXmlNode& out, script::IAbstractAdaptor& obj)
{
	//TiXmlElement* pElem = out.ToElement();
	//pElem->SetValue(obj.getName());
	//pElem->SetAttribute("type", obj.getType());

	//out << obj.getObject();
	//out << static_cast<CFunctionsOwner&>(obj);

	//TiXmlElement node_c("Children");
	//TiXmlNode* pCategory = out.InsertEndChild(node_c);

	//core::XmlNode<script::IAbstractAdaptor>::ChildrenList& children = obj.getChildren();
	//for (core::XmlNode<script::IAbstractAdaptor>::ChildrenList::iterator it = children.begin(); it != children.end(); ++it)
	//{
	//	TiXmlElement element((*it)->getName());
	//	TiXmlNode* child = pCategory->InsertEndChild(element);

	//	*child << (script::IAbstractAdaptor&)(**it);
	//}

	return out;
}

inline TiXmlNode& operator>>(TiXmlNode& in, script::IAbstractAdaptor& obj)
{
	in >> obj.getObject();

	return in;
}

namespace script{

	template<class T>
	class TAbstractAdaptor : public IAbstractAdaptor
	{
	public:
		TAbstractAdaptor(T* p, const std::string& name) : IAbstractAdaptor(name), type_(typeid(T).name()) 
		{ 
			object_ = p;
		} 
		virtual ~TAbstractAdaptor(){};

		const std::string getType() const { return type_; }

		virtual core::XmlClass& getObject() { return static_cast<core::XmlClass&>(*object_); }

	protected:
		T* object_;
		std::string type_;
	};

	class ExportManager
	{
	protected:
		ExportManager(){}
		~ExportManager(){}

	public:
		void setRoot(IAbstractAdaptor* pAdaptor) { m_pRoot = pAdaptor; m_pRoot->parse(); }
		IAbstractAdaptor* getRoot(void) {return m_pRoot;};

		void export(TiXmlNode* node)
		{
			*node << (IAbstractAdaptor&)*m_pRoot;
		}

	protected:
		IAbstractAdaptor* m_pRoot;
	};
	typedef base::TSingelton<ExportManager> TheExportManager;
}// end script