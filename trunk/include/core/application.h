#pragma once

#include "event/event.h"

struct SRect
{
	SRect(int x, int y)
	{
		width = x;
		height = y;
	}
	int	width, height;
};

class CWindowResize : public event::TEvent<SRect>
{
public:
	CWindowResize(int x, int y)
		: width(x),
		  height(y),
		  event::TEvent<SRect>(SRect(x, y))
	{
	}

	int	width, height;
};


typedef void *WindowHandle;

namespace core
{
	class ITask;
	typedef boost::shared_ptr<ITask> PTask;

	class IApplication
	{
	protected:
		IApplication()
		{
		}

	public:
		virtual ~IApplication()
		{
		}
		virtual void			addTask(PTask pTask) = 0;
		virtual void			Run() = 0;
		virtual bool			update() = 0;
		virtual void			close() = 0;
		virtual WindowHandle	getWindowHandle() const = 0;

		static IApplication		*Create();
		static IApplication		*Create(std::wstring Name, int Width, int Height, int ColorDepth, int DisplayFrequency, bool Fullscreen);
		static IApplication		*Create(WindowHandle hParentWindow);

		static IApplication		*Get();
	};
}