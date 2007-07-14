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
tf group("props");
}//end namespace{
template<>
template<>
void object::test <1>()
{
	CXmlNodePtr p (new CSomeObject());
	CXmlNodePtr p2 (new CSomeObject2());
	SetParentNode(p2, p);
}
template<>
template<>
void object::test <2>()
{
	{
		CXmlNode::ChildrenList& list = m_nodes[0]->getChildrenList();
		tut::ensure_equals((unsigned int)list.size(), 1);
		tut::ensure_equals(list.front(), m_nodes[1]);
	}
	{
		CXmlNode::ChildrenList& list = m_nodes[1]->getChildrenList();
		tut::ensure_equals((unsigned int)list.size(), 1);
		tut::ensure_equals(list.front(), m_nodes[2]);
	}
}
template<>
template<>
void object::test <3>()
{
	{
		CXmlNodePtr node = m_nodes[0]->findNode("CSomeObject2");
		tut::ensure_equals(node, m_nodes[1]);
	}
	{
		CXmlNodePtr node = m_nodes[0]->findNode("CSomeObject2.CSomeObject3");
		tut::ensure_equals(node, m_nodes[2]);
	}
}
template<>
template<>
void object::test <4>()
{
	CXmlNodePtr root = m_nodes[0];
	int res = root->get_Value<int>("CSomeObject2.CSomeObject3.Value");
	root = m_nodes[1];
	double d = root->get_Value<double>("CSomeObject3.DValue");
	CSomeObject3 mock;
	tut::ensure_equals(res, mock.getValue()  );
	tut::ensure_equals(d,   mock.getDValue() );
}
template<>
template<>
void object::test <5>()
{
	CXmlNodePtr root = m_nodes[0];
	int val = root->get_Value<int>("CSomeObject2.CSomeObject3.Value");
	root->set_Value("CSomeObject2.CSomeObject3.Value", val+10);
	CSomeObject3 mock;
	root = m_nodes[1];
	tut::ensure_equals(root->get_Value<int>("CSomeObject3.Value") , mock.getValue()+10  );
}