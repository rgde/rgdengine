//////////////////////////////////////////////////////////////////////////
// @author Guardian,  
// @date 02.01.06
// project: RGDE 
// description: XML property support
// @todo Add export of complex properties: CFrame, FloatInterp etc.
//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------
//									CFrame
//----------------------------------------------------------------------------------------

// TODO: add temporary export!

//----------------------------------------------------------------------------------------
//									XmlProperty
//----------------------------------------------------------------------------------------

inline TiXmlNode& operator<<(TiXmlNode& out, IProperty& prop)
{
	TiXmlElement* element = out.ToElement();
	element->SetValue(prop.getName());
	element->SetAttribute("type", prop.getType());
	
	TiXmlText text(prop.get());
	element->InsertEndChild(text);
	
	return out;
}

inline TiXmlNode& operator>>(TiXmlNode& in, IProperty& prop)
{
	TiXmlElement* element = in.ToElement();
	const std::string typestr = prop.getType();
	const std::string typenode = element->Attribute("type");
	if(typestr == typenode)
	{
		TiXmlText* text = element->FirstChild()->ToText();
		prop.set(text->Value());
	}

	return in;
}

//----------------------------------------------------------------------------------------
//									XmlFunction
//----------------------------------------------------------------------------------------

inline TiXmlNode& operator<<(TiXmlNode& out, IFunction& func)
{
	TiXmlText text(func.getName());
	out.InsertEndChild(text);

	return out;
}

inline TiXmlNode& operator>>(TiXmlNode& in, IFunction& )
{
	//unsupported!
	return in;
}

//----------------------------------------------------------------------------------------
//									Generic owners
//----------------------------------------------------------------------------------------

inline TiXmlNode& operator<<(TiXmlNode& out, CPropertyOwner& obj)
{
	TiXmlElement node("Properties");
	TiXmlNode* pProp = out.InsertEndChild(node);

	core::XmlClass::PropList& props = obj.getProperties();
	for (core::XmlClass::PropList::iterator iter = props.begin(); iter != props.end(); ++iter)
	{

		TiXmlElement property("property");
		property << (IProperty&) **iter;
		pProp->InsertEndChild(property);
	}

	return out;
}

inline TiXmlNode& operator>>(TiXmlNode& in, CPropertyOwner& obj)
{
	TiXmlNode* prop = NULL;
	TiXmlNode* parent = in.FirstChild("Properties");
	for( prop = parent->FirstChild(); prop; prop = prop->NextSibling() )
	{
		std::string propname = prop->Value();
		IProperty& p = obj.getProperty(propname);
		*prop >> p;
	}

	return in;
}

inline TiXmlNode& operator<<(TiXmlNode& out, CFunctionsOwner& obj)
{
	TiXmlElement node("Actions");
	TiXmlNode* pAct = out.InsertEndChild(node);

	core::XmlClass::FuncList& funcs = obj.getFunctions();
	for (core::XmlClass::FuncList::iterator iter = funcs.begin(); iter != funcs.end(); ++iter)
	{
		TiXmlElement function("action");
		function << (IFunction&) **iter;
		pAct->InsertEndChild(function);
	}

	return out;
}

inline TiXmlNode& operator>>(TiXmlNode& in, CFunctionsOwner& obj)
{
	// unsupported
	return in;
}

//----------------------------------------------------------------------------------------
//									XmlClass
//----------------------------------------------------------------------------------------

inline TiXmlNode& operator<<(TiXmlNode& out, core::XmlClass& obj)
{
	out << static_cast<CPropertyOwner&>(obj);
	// original object havn't any generic functions
	return out;
}

inline TiXmlNode& operator>>(TiXmlNode& in, core::XmlClass& obj)
{
	in >> static_cast<CPropertyOwner&>(obj);

	// actions is unchangeable :)
	return in;
}

//----------------------------------------------------------------------------------------
//									XmlNode
//----------------------------------------------------------------------------------------

//template <class T>
//inline TiXmlNode& operator<<(TiXmlNode& out, core::XmlNode<T>& obj)
//{
//	out << static_cast<core::XmlClass&>(obj);
//
//	core::XmlNode<T>::ChildrenList& children = obj.getChildren();
//	for (core::XmlNode<T>::ChildrenList::iterator it = children.begin(); it != children.end(); ++it)
//	{
//		TiXmlElement element((*it)->getName());
//		element << (core::XmlNode<T>&)(**it);
//		out.InsertEndChild(element);
//	}
//	return out;
//}
//
//template <class T>
//inline TiXmlNode& operator>>(TiXmlNode& in, core::XmlNode<T>& obj)
//{
//	// unsupported!
//	return in;
//}

namespace script{
}// end of script


//----------------------------------------------------------------------------------------
//									TAbstaractAdaptor
//----------------------------------------------------------------------------------------
