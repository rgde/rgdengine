#pragma once

#include "base.h"
#include <boost/function.hpp>
#include <rgde/core/application.h>

namespace input
{
	//Helper

	//�������� ��� Helper::Handler
	struct helper_event
	{
		enum type
		{
			Button,
			Axis
		};

		type m_type;
		bool m_press;
		int  m_delta;
		int  m_time;
	};

	//������-��������� ��� ��������� ���������� � �������� �����
	class Helper
	{
	public:
		typedef boost::function<void(const helper_event&)> Handler;

		Helper ();
		Helper (const std::wstring &commandName);
		virtual ~Helper();

		void attach (const std::wstring &commandName);
		void detach ();

		void operator += (Handler handler);

		command_ptr get_command() const {return m_command;}

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		command_ptr         m_command;
		std::list<Handler> m_handlers;
	};

	// Button, ������-��������� "������"
	class Button: public Helper
	{
	public:
		typedef boost::function<void(bool)> ButtonHandler;
		Button ();
		Button (const std::wstring &commandName);

		void operator += (ButtonHandler handler);
		operator bool () const {return m_press > 0;}

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		int m_press;
		std::list<ButtonHandler> m_buttonHandlers;
	};

	// Trigger, ������-��������� "�������"
	class Trigger: public Helper
	{
	public:
		typedef boost::function<void(bool)> TriggerHandler;
		Trigger ();
		Trigger (const std::wstring &commandName);

		void operator += (TriggerHandler handler);
		operator bool () const {return m_is_active;}

		void setState (bool state);

	protected:
		friend class CCommand;
		virtual void notify (const Control &c);

	private:
		bool m_is_active;
		std::list<TriggerHandler> m_triggerHandlers;
	};

	// KeyUp, ������-��������� "������� �������"
	class KeyUp: public Helper
	{
	public:
		typedef boost::function<void()> KeyUpHandler;
		KeyUp ();
		KeyUp (const std::wstring &commandName);

		void operator += (KeyUpHandler handler);

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		std::list<KeyUpHandler> m_keyupHandlers;
	};

	// key_down
	//������-��������� "������� �������"
	class key_down: public Helper
	{
	public:
		typedef boost::function<void()> KeyDownHandler;
		key_down ();
		key_down (const std::wstring &commandName);

		void operator += (KeyDownHandler handler);

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		std::list<KeyDownHandler> m_keydownHandlers;
	};

	// RelativeAxis
	//������-��������� "������������� ���"
	class RelativeAxis : public Helper
	{
	public:
		typedef boost::function<void(int)> RelativeAxisHandler;
		RelativeAxis ();
		RelativeAxis (const std::wstring &commandName);

		void operator += (RelativeAxisHandler handler);

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		std::list<RelativeAxisHandler> m_raxisHandlers;
	};

	// AbsoluteAxis
	//������-��������� "���������� ���"
	class AbsoluteAxis: public Helper
	{
	public:
		typedef boost::function<void(int)> AbsoluteAxisHandler;

		AbsoluteAxis ();
		AbsoluteAxis (const std::wstring &commandName);

		void operator += (AbsoluteAxisHandler handler);
		operator int () const {return m_pos;}

		int  getMin () const {return m_min;}
		void setMin (int value);

		int  getMax () const {return m_max;}
		void setMax (int value);

		int  get_pos () const {return m_pos;}		
		void set_pos (int value);

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		int m_min;
		int m_max;
		int m_pos;
		std::list<AbsoluteAxisHandler> m_aaxisHandlers;
	};

	// Cursor
	//������-��������� "������ ����"
    class Cursor: public Helper, public event::listener
	{
	public:
		typedef boost::function<void(float,float)> CursorHandler;

		Cursor ();

		void operator += (CursorHandler handler);

        float getX () const {return m_x;}
		float getY () const {return m_y;}
        void  set_pos (float x, float y);

	protected:
        void onCursorMove (mouse_move e);
        virtual void notify (const Control &control);

        void adjustPosToWindow (float &x, float &y);
        void adjustPosToClient (float &x, float &y);

	private:
        float m_x;
        float m_y;
		std::list<CursorHandler> m_cursorHandlers;
	};

    // Mouse
	//������-��������� "����"
    class Mouse: public Cursor
    {
    public:
        enum action_type
        {
            Down,
            Up,
            DoubleClick
        };

		typedef boost::function<void(int)>       WhellHandler;
		typedef boost::function<void(action_type)> ButtonHandler;

        Mouse ();

        void setMoveHandler         (CursorHandler handler);
		void setWhellHandler        (WhellHandler  handler);
        void setLeftButtonHandler   (ButtonHandler handler);
        void setMiddleButtonHandler (ButtonHandler handler);
        void setRightButtonHandler  (ButtonHandler handler);

        bool isLeftPressed   () const {return m_left;}
        bool isMiddlePressed () const {return m_middle;}
        bool isRightPressed  () const {return m_right;}

    protected:
        void onWhell (mouse_whell e);
        void onButton (mouse_button e);

    private:
        bool m_left;
        bool m_middle;
        bool m_right;

    	std::list<WhellHandler> m_whellHandlers;
    	std::list<ButtonHandler> m_leftButtonHandlers;
    	std::list<ButtonHandler> m_middleButtonHandlers;
    	std::list<ButtonHandler> m_rightButtonHandlers;
    };

	// KeyStream

	//�������� ��� KeyStream::KeyStreamHandler
	struct KeyStreamEvent
	{
		enum type
		{
			KeyUp,
			key_down,
			KeyDownAuto,
			Char
		};

		type m_type;
		bool m_press;
		int  m_key_code;
		int  m_char;
	};

	//������-��������� "����� ��������"
	class KeyStream: public Helper
	{
	public:
		typedef boost::function<void(const KeyStreamEvent&)> KeyStreamHandler;
		KeyStream () {}

		void operator += (KeyStreamHandler handler);

	protected:
		friend class Command;
		virtual void notify (const Control &c);

	private:
		std::list<KeyStreamHandler> m_ksHandlers;
	};
}