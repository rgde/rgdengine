#pragma once

namespace base
{
	class BaseRefCounted 
	{
	public:
		BaseRefCounted() : m_refCount(0){}
		virtual ~BaseRefCounted(){}

	private: 
		int	m_refCount;
		friend void intrusive_ptr_add_ref(BaseRefCounted*);
		friend void intrusive_ptr_release(BaseRefCounted*);
	};

	inline void intrusive_ptr_add_ref(BaseRefCounted* p)
	{
		++(p->m_refCount);
	}
	inline void intrusive_ptr_release(BaseRefCounted* p)
	{
		--(p->m_refCount);

		if (p->m_refCount == 0)
			delete p;
	}
}