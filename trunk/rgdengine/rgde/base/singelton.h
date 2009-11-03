#pragma once

namespace base
{
	template<class T, int tag = 0>
	class singelton : T, boost::noncopyable
	{
	protected:
		singelton()				{}
		virtual ~singelton()	{}

	public:
		static T& get();
		static void destroy();
		static bool is_created();

	private:
		static singelton<T, tag>*	ms_instance;
	};

	template<class T, int tag>
	singelton<T, tag>* singelton<T, tag>::ms_instance = 0;

	template<class T, int tag>
	T&	singelton<T, tag>::get()
	{
		if (0 == ms_instance) 
		{
			ms_instance = new singelton<T, tag>();
		}

		return  *ms_instance;
	}

	template<class T, int tag>
	void	singelton<T, tag>::destroy()
	{
		if (0 != ms_instance)
		{
			delete ms_instance;
			ms_instance = 0;
		}
	}

	template<class T, int tag>
	bool singelton<T, tag>::is_created()
	{
		return 0 != ms_instance;
	}
}