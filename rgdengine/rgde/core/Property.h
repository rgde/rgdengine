#pragma once

#include "rgde/base/exceptions.h"
#include <boost/shared_ptr.hpp>

//TODO: 
// * сделать хранилище пропертей внешним, что бы избавитьс€ от отверхеда на каждом классе
// * сделать нормальную работу серелизатора с проперт€ми.

namespace core
{

	class base_property : boost::noncopyable
	{
	public:
		virtual std::string	get()	const			= 0;
		virtual void		set(const std::string&)	= 0;

		const std::wstring& get_name() const {return m_name;}
		const std::string& get_type() const {return m_type_name;}

		virtual bool is_read_only() = 0;

		base_property(const std::wstring& name, std::string type = "string") 
			: m_name(name), m_type_name(type)
		{
		}
		
		virtual ~base_property(){}

	protected:
		std::wstring m_name;
		std::string m_type_name;
	};

	typedef boost::shared_ptr<base_property> property_ptr;

	template <class T>
	class property : public base_property
	{
	public:
		typedef typename boost::call_traits<T>::param_type param_type;
		typedef boost::function<param_type ()>		getter;
		typedef boost::function<void (param_type)>	setter;

		property(const std::string& name, getter gf, setter sf = setter()) 
			: base_property(name, std::string(typeid(T).name())), 
			m_get_function(gf), 
			m_set_function(sf)
		{
		}

		property(const std::string& name, const std::string& type_name, getter gf, setter sf = setter()) 
			: base_property(name, type_name), 
			m_get_function(gf), 
			m_set_function(sf)
		{
		}

		getter get_getter() const {return m_get_function;}
		setter get_setter() const {return m_set_function;}

		virtual bool is_read_only() {return m_set_function ? false : true;}

		std::string get() const 
		{
			if (!m_get_function) return std::string();

			try
			{
				std::ostrstream out;
				out << m_get_function();
				return std::string(out.str(), out.pcount());
			}
			catch(...)
			{
				throw std::bad_exception("Property: Get Error!");
			}
		}

		void set(const std::string& str)
		{
			if (!m_set_function) return;

			try
			{
				std::istrstream in(str.c_str());
				T t;
				in >> t;
				m_set_function(t);

			}
			catch(...)
			{
				throw std::exception("Property: Invalid value");
			}
		}
		
	protected:
		getter m_get_function;
		setter	m_set_function;
	};

	// хранит свойства дл€ конкретного типа объекта дл€ того, что бы не сздавать
	// экземпл€ры пропертей в каждом контретном экземпл€ре объета
	template<class OwnerType>
	class property_storage
	{
		struct _seacher
		{
			const std::string& str;
			_seacher(const std::string& str_to_cmp) : str(str_to_cmp)	{}
			bool operator()(property_ptr p) { return p->get_name() == str; }
		};
	public:
		typedef std::vector<property_ptr> PropList;

		PropList&		get_properties()			{return m_properties;}
		const PropList& get_properties() const	{return m_properties;}

		property_ptr get_property(const std::string& property_name) const
		{
			PropList::const_iterator i = std::find_if(m_properties.begin(), m_properties.end(), _seacher(property_name));

			if(i != m_properties.end())
				return (*i);
			else
				return property_ptr();
		}

		/// если вернет True - кто-то уже скорей всего создал все проперт€.
		static bool is_created();
		static property_storage& GetInstance();

	private:
		PropList m_properties;
		static property_storage* m_spInstance;
	};

	/// вспомогательный класс дл€ регистрации пропертей в хранилище.
	class property_owner
	{

	public:
		virtual ~property_owner() {}

		template<class Derived, typename Value>
		void register_property(){}
	};

	//////////////////////////////////////////////////////////////////////////
	/// Helpers 
	#define DEFINE_PROPERTY(NAME, TYPE)\
	protected:\
		TYPE m_##NAME;\
	public:\
		inline void set##NAME(const TYPE& value) {m_##NAME = value;}\
		inline const TYPE& get##NAME() const {return m_##NAME;}
	//
	//#define REGISTER_PROPERTY(NAME, TYPE)\
	//	addProperty(new property<TYPE>(m_##NAME, #NAME, #TYPE));

	//////////////////////////////////////////////////////////////////////////



	class function
	{
	public:
		typedef boost::function<void(const std::string&)> Func;

		function(const std::string& name, Func f)
			: m_name(name), m_func(f)
		{}

		const std::string& get_name() const  { return m_name; }

		void call(const std::string& params) {if (m_func) m_func(params);}
		void operator()(const std::string& params) {call(params);}

	private:
		Func		m_func;
		std::string m_name;
	};

	class functions_owner
	{
		struct _seacher
		{
			const std::string& str;
			_seacher(const std::string& str_to_cmp) : str(str_to_cmp){}
			bool operator()(const function& p){return p.get_name() == str;}
		};
	public:
		typedef std::vector<function> FuncList;
		FuncList& get_functions() {return m_functions;}
		
		virtual ~functions_owner(){}

		function* getter(const std::string& function_name)
		{
			FuncList::iterator i = std::find_if(m_functions.begin(), m_functions.end(), _seacher(function_name));
			if (m_functions.end() == i)
				return 0;
			return &(*i);
		}

		void call_function(const std::string& function_name, const std::string& strParams = std::string())
		{
			function* f = getter(function_name);
			if (0 != f)
				(*f)(strParams);
		}

	protected:
		void add_function(function p){m_functions.push_back(p);}

	protected:
		FuncList m_functions;
	};

}