#pragma once

#include "rgde/event/events.h"

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

struct CMouseWhell
{
    CMouseWhell(int _delta): delta(_delta) {}
    int delta;
};

struct CMouseButton
{
    enum ButtonType
    {
        Left,
        Middle,
        Right
    };

    enum ClickType
    {
        Down,
        Up,
        DoubleClick
    };

    CMouseButton(ButtonType _button, ClickType _click): button(_button), click(_click) {}
    ButtonType button;
    ClickType click;
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

		template<typename TaskType, typename P1, typename P2, typename P3>
		IApplication& addTask(const P1& p1, const P2& p2, const P3& p3)
		{
			PTask task(new TaskType(*this, p1, p2, p3));
			addTask(task);
			return *this;
		}

		template<typename TaskType, typename P1, typename P2>
		IApplication& addTask(const P1& p1, const P2& p2)
		{
			PTask task(new TaskType(*this, p1, p2));
			addTask(task);
			return *this;
		}

		template<typename TaskType, typename P1>
		IApplication& addTask(const P1& p1)
		{
			PTask task(new TaskType(*this, p1));
			addTask(task);
			return *this;
		}

		static IApplication		*Create(const std::wstring& window_name = L"");
        static IApplication		*Create(const std::wstring& window_name, int width, int height, bool fullscreen, bool resize_enable = true);
        static IApplication		*Create(WindowHandle parent_window);

        static IApplication		*Get();
    };
}