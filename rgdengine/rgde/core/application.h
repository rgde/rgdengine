#pragma once

#include "rgde/event/event.h"

struct CWindowResize
{
    CWindowResize(int x, int y): width(x),height(y) {}
    int	width;
    int height;
};

struct CCursorMove
{
    CCursorMove(float _x, float _y): x(_x),y(_y) {}
    float x;
    float y;
};

typedef void *WindowHandle;

namespace core
{
    class ITask;
    typedef boost::shared_ptr<ITask> PTask;

    class IApplication
    {
    protected:
		IApplication(){}

    public:
		virtual ~IApplication() {}
        virtual void			addTask(PTask pTask) = 0;
        virtual void			Run() = 0;
        virtual bool			update() = 0;
        virtual void			close() = 0;
        virtual WindowHandle	getWindowHandle() const = 0;

		static IApplication		*Create(const std::wstring& window_name = L"");
        static IApplication		*Create(const std::wstring& window_name, int width, int height, bool fullscreen, bool resize_enable = true);
        static IApplication		*Create(WindowHandle parent_window);

        static IApplication		*Get();
    };
}