#pragma once

#include "base.h"
#include <boost/function.hpp>
#include <rgde/core/application.h>

namespace input
{
	//Helper

	//параметр для Helper::Handler
	struct HelperEvent
	{
		enum EType
		{
			Button,
			Axis
		};

		EType m_type;
		bool m_press;
		int  m_delta;
		int  m_time;
	};

	//обьект-посредник для получения информации о событиях ввода
	class Helper
	{
	public:
		typedef boost::function<void(const HelperEvent&)> Handler;

		Helper ();
		Helper (const std::wstring &commandName);
		virtual ~Helper();

		void attach (const std::wstring &commandName);
		void detach ();

		void operator += (Handler handler);

		CommandPtr getCommand() const {return m_command;}

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		CommandPtr         m_command;
		std::list<Handler> m_handlers;
	};

	// Button, обьект-посредник "кнопка"
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

	// Trigger, обьект-посредник "триггер"
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
		virtual void notify (const Control &rControl);

	private:
		bool m_is_active;
		std::list<TriggerHandler> m_triggerHandlers;
	};

	// KeyUp, обьект-посредник "ОТжатие клавиши"
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

	// KeyDown
	//обьект-посредник "НАжатие клавиши"
	class KeyDown: public Helper
	{
	public:
		typedef boost::function<void()> KeyDownHandler;
		KeyDown ();
		KeyDown (const std::wstring &commandName);

		void operator += (KeyDownHandler handler);

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		std::list<KeyDownHandler> m_keydownHandlers;
	};

	// RelativeAxis
	//обьект-посредник "относительная ось"
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
	//обьект-посредник "абсолютная ось"
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

		int  getPos () const {return m_pos;}		
		void setPos (int value);

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
	//обьект-посредник "курсор мыши"
    class Cursor: public Helper, public event::CListener
	{
	public:
		typedef boost::function<void(float,float)> CursorHandler;

		Cursor ();

		void operator += (CursorHandler handler);

        float getX () const {return m_x;}
		float getY () const {return m_y;}
        void  setPos (float x, float y);

	protected:
        void onCursorMove (CCursorMove e);
        virtual void notify (const Control &control);

	private:
        void adjustPosToWindow(float &x, float &y);
        void adjustPosToClient(float &x, float &y);

	private:
        float m_x;
        float m_y;
		std::list<CursorHandler> m_cursorHandlers;
	};

	// KeyStream

	//параметр для KeyStream::KeyStreamHandler
	struct KeyStreamEvent
	{
		enum EType
		{
			KeyUp,
			KeyDown,
			KeyDownAuto,
			Char
		};

		EType m_type;
		bool m_press;
		int  m_key_code;
		int  m_char;
	};

	//обьект-посредник "поток символов"
	class KeyStream: public Helper
	{
	public:
		typedef boost::function<void(const KeyStreamEvent&)> KeyStreamHandler;
		KeyStream () {}

		void operator += (KeyStreamHandler handler);

	protected:
		friend class Command;
		virtual void notify (const Control &rControl);

	private:
		std::list<KeyStreamHandler> m_ksHandlers;
	};
}