#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace base
{
	template<class Param, class Resource> 
	class TResourceManager
	{
	public:
		typedef boost::shared_ptr<Resource> ResourcePtr;
		typedef boost::function <ResourcePtr (const Param&)> CreatorFunc;

		TResourceManager(CreatorFunc creator, bool hasDefault = false, const Param& p = Param())
			: m_creator(creator),m_defaultParam(p), m_hasDefault(hasDefault)
		{
		}

		ResourcePtr get(const Param& p) 
		{
			//ResourceMap::iterator it = m_resources.find(p);
			//if (it != m_resources.end() && !it->second.expired())
			//{
			//	return it->second.lock();
			//}

			return create(p);
		}

	private:
		ResourcePtr create(const Param& p)
		{
			if (ResourcePtr r = m_creator(p))
			{
				m_resources[p] = r;
				return r;
			}
			else if (m_hasDefault && p != m_defaultParam)
			{
				return get(m_defaultParam);
			}
			else 
				return ResourcePtr();
		}

	private:
		typedef boost::weak_ptr<Resource> ResourceWeakPtr;
		typedef std::map<Param, ResourceWeakPtr> ResourceMap;

		CreatorFunc m_creator;
		Param		m_defaultParam; // для дефолтного ресурса
		bool		m_hasDefault;
		ResourceMap m_resources;
	};
}