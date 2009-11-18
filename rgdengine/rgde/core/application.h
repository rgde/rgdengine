#pragma once

#include <rgde/event/events.h>

struct window_resize
{
    window_resize(int x, int y): width(x),height(y) {}
    int	width;
    int height;
};

struct mouse_move
{
    mouse_move(float _x, float _y): x(_x),y(_y) {}
    float x;
    float y;
};

struct mouse_whell
{
    mouse_whell(int _delta): delta(_delta) {}
    int delta;
};

struct mouse_button
{
    enum button_type
    {
        Left,
        Middle,
        Right
    };

    enum action_type
    {
        Down,
        Up,
        DoubleClick
    };

    mouse_button(button_type _button, action_type _click)
		: button(_button), click(_click) {}

    button_type button;
    action_type click;
};

typedef void *window_handle;

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
        virtual void			add(task_ptr t) = 0;
        virtual void			run() = 0;
        virtual bool			update() = 0;
        virtual void			close() = 0;
        virtual window_handle	get_handle() const = 0;

		template<typename TaskType, typename P1, typename P2, typename P3>
		application& add(const P1& p1, const P2& p2, const P3& p3)
		{
			task_ptr task(new TaskType(*this, p1, p2, p3));
			add(task);
			return *this;
		}

		template<typename TaskType, typename P1, typename P2>
		application& add(const P1& p1, const P2& p2)
		{
			task_ptr task(new TaskType(*this, p1, p2));
			add(task);
			return *this;
		}

		template<typename TaskType, typename P1>
		application& add(const P1& p1)
		{
			task_ptr task(new TaskType(*this, p1));
			add(task);
			return *this;
		}

		static application		*create(const std::wstring& window_name = L"");
        static application		*create(const std::wstring& window_name, int width, int height, bool fullscreen, bool resize_enable = true);
        static application		*create(window_handle parent_window);

        static application		*get();
    };
}