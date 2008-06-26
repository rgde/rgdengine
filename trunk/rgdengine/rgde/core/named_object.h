#pragma once

namespace core
{
	class NamedObject
	{
	public:
		NamedObject(){}
		NamedObject(const std::string& name) : m_name(name){}
		virtual ~NamedObject(){}

		const std::string& getName()	const	{return m_name;}
		void setName(const std::string& name)	{m_name = name;}

	protected:
		std::string m_name;
	};
}