#include "tut.h"
#include "header.h"
namespace{
struct data
{
	std::vector<CXmlNodePtr> m_nodes;
	data()
	{
		{
			CXmlNodePtr p (new CSomeObject());
			m_nodes.push_back(p);
		}
		{	
			CXmlNodePtr p(new CSomeObject2());
			SetParentNode(p, m_nodes[0]);
			m_nodes.push_back(p);
		}
		{
			CXmlNodePtr p(new CSomeObject3());
			SetParentNode(p ,m_nodes[1]);
			m_nodes.push_back(p);
		}
	}
};
typedef tut::test_group<data> tf;
typedef tf::object object;
tf group("xml");
}//end namespace{


template<>
template<>
void object::test <1>()
{
	TiXmlDocument doc;
	TiXmlDeclaration dcl("1.0", std::string(), "no");
	doc.InsertEndChild(dcl);
	std::ostringstream os;
	os<<doc;
	tut::ensure_equals(os.str(), std::string("<?xml version=\"1.0\" standalone=\"no\" ?>"));
}
template<>
template<>
void object::test <2>()
{
	TiXmlDocument doc;
	TiXmlElement n("CSomeClass");
	TiXmlElement val("Value");
	TiXmlText val_data("100");
	val.InsertEndChild(val_data);
	n.InsertEndChild(val);
	doc.InsertEndChild(n);
	std::ostringstream os;
	os<<doc;
	tut::ensure_equals(os.str(), std::string("<CSomeClass><Value>100</Value></CSomeClass>"));
}
template<>
template<>
void object::test <3>()
{
	TiXmlDocument doc;
	doc << *m_nodes[2];
	std::ostringstream os;
	os<<doc;
	tut::ensure_equals(os.str(), std::string("<CSomeObject3><Name>CSomeObject3</Name><Value>315</Value><DValue>3.14159</DValue></CSomeObject3>"));
}
template<>
template<>
void object::test <4>()
{
const char* result = \
"<CSomeObject>\
<Name>CSomeObject</Name>\
<Value>115</Value>\
<CSomeObject2>\
<Name>CSomeObject2</Name>\
<Value>215</Value>\
<CSomeObject3>\
<Name>CSomeObject3</Name>\
<Value>315</Value>\
<DValue>3.14159</DValue>\
</CSomeObject3>\
</CSomeObject2>\
</CSomeObject>";
	TiXmlDocument doc;
	doc << *m_nodes[0];
	std::ostringstream os;
	os<<doc;
	tut::ensure_equals(os.str(), std::string(result));
}