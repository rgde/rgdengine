/*
ISimpleTrigger is base class for all triggers.
@author PC
*/

#pragma once

namespace scene
{
	template<class T>
	class base_trigger
	{
	public:
		base_trigger() : m_is_triggered(false)
		{
		}

		virtual ~base_trigger(){}

		typedef boost::function<void(const T&)> ListenerCallBack;

		void addListener(const ListenerCallBack& callBack)
		{
			m_vCallBacks.push_back(callBack);
		}

		bool isTriggered() const {return m_is_triggered;}

	protected:
		//triggers the trigger
		void trigger(T t) const
		{
			size_t num = m_vCallBacks.size();
			for(size_t i = 0; i < num; i++)
				m_vCallBacks[i](t);
		}

	protected:
		typedef std::vector<ListenerCallBack> ListenerCallBacks;
		ListenerCallBacks m_vCallBacks;

		bool m_is_triggered;
	};
}