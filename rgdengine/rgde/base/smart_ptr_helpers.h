#pragma once

namespace base
{
	class refcounted 
	{
	public:
		refcounted() : m_refcount(0){}
		virtual ~refcounted(){}

	private: 
		int	m_refcount;
		friend void intrusive_ptr_add_ref(refcounted*);
		friend void intrusive_ptr_release(refcounted*);
	};

	inline void intrusive_ptr_add_ref(refcounted* p)
	{
		++(p->m_refcount);
	}
	inline void intrusive_ptr_release(refcounted* p)
	{
		--(p->m_refcount);

		if (p->m_refcount == 0)
			delete p;
	}
}