#pragma once

namespace core
{
	class CNamedObject
	{
	public:
		CNamedObject(){}
		CNamedObject(const std::string& name) : m_Name(name){}
		virtual ~CNamedObject(){}

		const std::string& getName()	const	{return m_Name;}
		void setName(const std::string& name)	{m_Name = name;}

	protected:
		std::string m_Name;
	};
}