#pragma once

namespace base
{
	template<class T, int ___v = 0>
	class TSingelton : T, boost::noncopyable
	{
	private:
		TSingelton()				{}
		virtual ~TSingelton()		{}

	public:
		static T& Get();
		static void Destroy();
		static bool IsCreated();

	private:
		static TSingelton<T, ___v>*	ms_pInstance;
	};

	template<class T, int ___v>
	TSingelton<T, ___v>* TSingelton<T, ___v>::ms_pInstance = 0;

	template<class T, int ___v>
	T&	TSingelton<T, ___v>::Get()
	{
		if (0 == ms_pInstance) 
		{
			ms_pInstance = new TSingelton<T, ___v>();
		}

		return  *ms_pInstance;
	}

	template<class T, int ___v>
	void	TSingelton<T, ___v>::Destroy()
	{
		if (0 != ms_pInstance)
		{
			delete ms_pInstance;
			ms_pInstance = 0;
		}
	}

	template<class T, int ___v>
	bool TSingelton<T, ___v>::IsCreated()
	{
		return 0 != ms_pInstance;
	}
}