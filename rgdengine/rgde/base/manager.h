#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace base
{
	template<class Param, class Resource> 
	class resource_manager
	{
	public:
		typedef boost::shared_ptr<Resource> resource_ptr;
		typedef boost::function <resource_ptr (const Param&)> creator_func;

		resource_manager(creator_func creator, bool hasDefault = false, const Param& p = Param())
			: m_creator(creator),m_default_param(p), m_has_default(hasDefault)
		{
		}

		resource_ptr get(const Param& p) 
		{
			resource_map::iterator it = m_resources.find(p);
			if (it != m_resources.end() && !it->second.expired())
			{
				return it->second.lock();
			}

			return create(p);
		}

	private:
		resource_ptr create(const Param& p)
		{
			if (resource_ptr r = m_creator(p))
			{
				m_resources[p] = r;
				return r;
			}
			else if (m_has_default && p != m_default_param)
			{
				return get(m_default_param);
			}
			else 
				return resource_ptr();
		}

	private:
		typedef boost::weak_ptr<Resource> resource_wptr;
		typedef std::map<Param, resource_wptr> resource_map;

		creator_func m_creator;
		Param		m_default_param; // для дефолтного ресурса
		bool		m_has_default;
		resource_map m_resources;
	};
}