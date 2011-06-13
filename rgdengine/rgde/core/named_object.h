#pragma once

namespace core
{
	class named_object
	{
	protected:
		named_object(){}
		explicit named_object(const std::string& name) : m_name(name){}
	public:
		virtual ~named_object(){}

		const std::string& name()	const	{return m_name;}
		void name(const std::string& name)	{m_name = name;}

	protected:
		std::string m_name;
	};
}