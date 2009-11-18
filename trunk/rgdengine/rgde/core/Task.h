#pragma once

namespace core
{
	class application;

	class base_task
	{
	public:
		base_task(const application& app, int priority = 0);
		virtual ~base_task();

		int get_priority() const {return m_priority;}

		void start();
		void stop();
		void pause();
		void execute();

		const application& get_application() const {return m_application;}
		application& get_application() {return m_application;}

	protected:
		virtual void run() = 0;

	private:
		bool m_is_paused;
		bool m_is_started;
		int m_priority;
		const application& m_application;
	};

	typedef boost::shared_ptr<base_task> task_ptr;

	inline bool operator<(const task_ptr& task1, const task_ptr& task2)
	{
		return task1->get_priority() < task2->get_priority();
	}
}