#pragma once

namespace xml {
class node;
};

namespace gui
{

class System;
class BaseWindow;
typedef boost::intrusive_ptr<BaseWindow> WindowPtr;

struct ICreator
{
	ICreator(System& sys, const char* type)
		: m_system(sys), m_typename(type)
	{
	}
	virtual ~ICreator(){}
	virtual WindowPtr Create(const std::string& name) = 0;

	System& m_system;
	const char* m_typename;
};

template<class T>
struct Creator : public ICreator
{
	Creator(System& sys) : ICreator(sys, T::GetType()) {}
	virtual WindowPtr Create(const std::string& name)
	{
		return WindowPtr(new T(m_system, name));
	}
};

struct RootCreator : public ICreator
{
	RootCreator(System& sys, const char* type_name) : ICreator(sys, type_name) {}
	virtual WindowPtr Create(const std::string& name);	
};

typedef boost::shared_ptr<ICreator> CreatorPtr;
class WindowFactory
{
public:
	typedef std::map<std::string, CreatorPtr> CreatorsMap;
	typedef CreatorsMap::const_iterator CreatorsConstIt;
	typedef std::list<std::string> TypesList;

	WindowFactory(System& sys);
	virtual ~WindowFactory();

	void RegisterCreator(const std::string& type, CreatorPtr creator);

	template<typename T>
	void RegisterCreator()
	{
		RegisterCreator(T::GetType(), CreatorPtr(new Creator<T>(m_system)));
	}

	WindowPtr Create(const std::string& type, const std::string& name);
	
	TypesList GetTypesList();
	
protected:
	CreatorsMap m_creators;
	System& m_system;
};

}