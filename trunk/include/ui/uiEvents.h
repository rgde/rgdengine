#pragma once

namespace ui
{
	class IControl;

	struct Event
	{
		enum EventType
		{
			KeyDown = 0,
			KeyUp,
			MouseMove,
			MouseBtnDown,
			MouseBtnUp,
			MouseClick,
			Update,
			Paint,
			Unknown = 0xFF
		};

		class IControl* pSender;
		EventType		eventType;
	};

	struct KeyEvent : public Event
	{
		enum KeyState {Down, Up};
		typedef int Key;

		KeyEvent(Key key, KeyState state)
		{
			switch(state)
			{
			case Down:
				eventType = KeyDown;
				break;
			case Up:
				eventType = KeyUp;
				break;
			default:
				eventType = Unknown;
			}

			this->key = key;
		}

		Key key;
	};

	struct MouseEvent : public Event
	{
		enum MouseState {BtnDown, BtnUp, Click, Move};
	};
}