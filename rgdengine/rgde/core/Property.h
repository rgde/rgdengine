#pragma once

#include "rgde/base/exceptions.h"
#include <boost/shared_ptr.hpp>

//TODO: 
// * сделать хранилище пропертей внешним, что бы избавитьс€ от отверхеда на каждом классе
// * сделать нормальную работу серелизатора с проперт€ми.

namespace core
{

	class IProperty : boost::noncopyable
	{
	public:
		virtual std::string	get()	const			= 0;
		virtual void		set(const std::string&)	= 0;

		const std::wstring& getName() const {return m_name;}
		const std::string& getType() const {return m_typeName;}

		virtual bool isReadOnly() = 0;

		IProperty(const std::wstring& name, std::string type = "string") 
			: m_name(name), m_typeName(type)
		{
		}
		
		virtual ~IProperty(){}

	protected:
		std::wstring m_name;
		std::string m_typeName;
	};

	typedef boost::shared_ptr<IProperty> PProperty;

	template <class T>
	class TProperty : public IProperty
	{
	public:
		typedef typename boost::call_traits<T>::param_type param_type;
		typedef boost::function<param_type ()>		GetFunction;
		typedef boost::function<void (param_type)>	SetFunction;

		TProperty(const std::string& name, GetFunction gf, SetFunction sf = SetFunction()) 
			: IProperty(name, std::string(typeid(T).name())), 
			m_get_function(gf), 
			m_set_function(sf)
		{
		}

		TProperty(const std::string& name, const std::string& type_name, GetFunction gf, SetFunction sf = SetFunction()) 
			: IProperty(name, type_name), 
			m_get_function(gf), 
			m_set_function(sf)
		{
		}

		GetFunction getFunction() const {return m_get_function;}
		SetFunction setFunction() const {return m_set_function;}

		virtual bool isReadOnly() {return m_set_function ? false : true;}

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
		GetFunction m_get_function;
		SetFunction	m_set_function;
	};

	// хранит свойства дл€ конкретного типа объекта дл€ того, что бы не сздавать
	// экземпл€ры пропертей в каждом контретном экземпл€ре объета
	template<class OwnerType>
	class PropertyStorage
	{
		struct _seacher
		{
			const std::string& str;
			_seacher(const std::string& str_to_cmp) : str(str_to_cmp)	{}
			bool operator()(PProperty p) { return p->getName() == str; }
		};
	public:
		typedef std::vector<PProperty> PropList;

		PropList&		getProperties()			{return m_properties;}
		const PropList& getProperties() const	{return m_properties;}

		PProperty getProperty(const std::string& property_name) const
		{
			PropList::const_iterator i = std::find_if(m_properties.begin(), m_properties.end(), _seacher(property_name));

			if(i != m_properties.end())
				return (*i);
			else
				return PProperty();
		}

		/// если вернет True - кто-то уже скорей всего создал все проперт€.
		static bool IsCreated();
		static PropertyStorage& GetInstance();

	private:
		PropList m_properties;
		static PropertyStorage* m_spInstance;
	};

	/// вспомогательный класс дл€ регистрации пропертей в хранилище.
	class CPropertyOwner
	{

	public:
		virtual ~CPropertyOwner() {}

		template<class Derived, typename Value>
		void registerProperty(){}
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
	//	addProperty(new TProperty<TYPE>(m_##NAME, #NAME, #TYPE));

	//////////////////////////////////////////////////////////////////////////



	class Function
	{
	public:
		typedef boost::function<void(const std::string&)> Func;

		Function(const std::string& name, Func f)
			: m_name(name), m_func(f)
		{}

		const std::string& getName() const  { return m_name; }

		void call(const std::string& params) {if (m_func) m_func(params);}
		void operator()(const std::string& params) {call(params);}

	private:
		Func		m_func;
		std::string m_name;
	};

	class CFunctionsOwner
	{
		struct _seacher
		{
			const std::string& str;
			_seacher(const std::string& str_to_cmp) : str(str_to_cmp){}
			bool operator()(const Function& p){return p.getName() == str;}
		};
	public:
		typedef std::vector<Function> FuncList;
		FuncList& getFunctions() {return m_functions;}
		
		virtual ~CFunctionsOwner(){}

		Function* getFunction(const std::string& function_name)
		{
			FuncList::iterator i = std::find_if(m_functions.begin(), m_functions.end(), _seacher(function_name));
			if (m_functions.end() == i)
				return 0;
			return &(*i);
		}

		void callFunction(const std::string& function_name, const std::string& strParams = std::string())
		{
			Function* f = getFunction(function_name);
			if (0 != f)
				(*f)(strParams);
		}

	protected:
		void addFunction(Function p){m_functions.push_back(p);}

	protected:
		FuncList m_functions;
	};

}