#pragma once

namespace base
{
	class CBaseRefCounted 
	{
	public:
		CBaseRefCounted() : m_refCount(0){}
		virtual ~CBaseRefCounted(){}

	private: 
		int	m_refCount;
		friend void intrusive_ptr_add_ref(CBaseRefCounted*);
		friend void intrusive_ptr_release(CBaseRefCounted*);
	};

	inline void intrusive_ptr_add_ref(CBaseRefCounted* p)
	{
		++(p->m_refCount);
	}
	inline void intrusive_ptr_release(CBaseRefCounted* p)
	{
		--(p->m_refCount);

		if (p->m_refCount == 0)
			delete p;
	}
}