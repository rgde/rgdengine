#pragma once

namespace core
{
	class application;

	class base_task
	{
	public:
		base_task(const application& app, int nPriority = 0);
		virtual ~base_task();

		int get_priority() const {return m_nPriority;}

		void start();
		void stop();
		void pause();
		void execute();

		const application& get_application() const {return m_application;}

	protected:
		virtual void run() = 0;

	private:
		bool m_bIsPaused;
		bool m_bIsStarted;
		int m_nPriority;
		const application& m_application;
	};

	typedef boost::shared_ptr<base_task> task_ptr;

	inline bool operator<(const task_ptr& task1, const task_ptr& task2)
	{
		return task1->get_priority() < task2->get_priority();
	}
}