#pragma once

#include <rgde/event/events.h>

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
    class base_task;
    typedef boost::shared_ptr<base_task> task_ptr;

    class application
    {
    protected:
		application(){}

    public:
		virtual ~application() {}
        virtual void			addTask(task_ptr pTask) = 0;
        virtual void			Run() = 0;
        virtual bool			update() = 0;
        virtual void			close() = 0;
        virtual WindowHandle	getWindowHandle() const = 0;

		template<typename TaskType, typename P1, typename P2, typename P3>
		application& addTask(const P1& p1, const P2& p2, const P3& p3)
		{
			task_ptr task(new TaskType(*this, p1, p2, p3));
			addTask(task);
			return *this;
		}

		template<typename TaskType, typename P1, typename P2>
		application& addTask(const P1& p1, const P2& p2)
		{
			task_ptr task(new TaskType(*this, p1, p2));
			addTask(task);
			return *this;
		}

		template<typename TaskType, typename P1>
		application& addTask(const P1& p1)
		{
			task_ptr task(new TaskType(*this, p1));
			addTask(task);
			return *this;
		}

		static application		*Create(const std::wstring& window_name = L"");
        static application		*Create(const std::wstring& window_name, int width, int height, bool fullscreen, bool resize_enable = true);
        static application		*Create(WindowHandle parent_window);

        static application		*Get();
    };
}