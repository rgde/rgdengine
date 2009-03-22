#pragma once

namespace core
{
	class named_object
	{
	public:
		named_object(){}
		named_object(const std::string& name) : m_name(name){}
		virtual ~named_object(){}

		const std::string& get_name()	const	{return m_name;}
		void set_name(const std::string& name)	{m_name = name;}

	protected:
		std::string m_name;
	};
}