#pragma once

namespace core
{
	class IApplication;

	class ITask
	{
	public:
		ITask(const IApplication& app, int nPriority = 0);
		virtual ~ITask();

		int getPriority() const {return m_nPriority;}

		void start();
		void stop();
		void pause();
		void execute();

		const IApplication& getApplication() const {return m_application;}

	protected:
		virtual void run() = 0;

	private:
		bool m_bIsPaused;
		bool m_bIsStarted;
		int m_nPriority;
		const IApplication& m_application;
	};

	typedef boost::shared_ptr<ITask> PTask;

	inline bool operator<(const PTask& task1, const PTask& task2)
	{
		return task1->getPriority() < task2->getPriority();
	}
}