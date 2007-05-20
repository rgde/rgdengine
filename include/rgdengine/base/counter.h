#ifndef COUNTER_H
#define COUNTER_H

namespace base{

class counter
{
	public:
		counter();
		virtual ~counter();

		// инициализация
		static bool init();

		// инициализация
		static double getCurrentTime();

		// инициализация
		void startTiming();

		// инициализация
		double getTimeElapsed();

	protected:
		static double frequency;
		double startTime;
};

} // namespace base

#endif // COUNTER_H