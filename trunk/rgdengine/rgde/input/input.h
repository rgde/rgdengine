#pragma once

#include <rgde/core/application.h>

#include <boost/function.hpp>

namespace input
{
	class input_impl; //система ввода
	class device;    //устройство ввода
	class Control;   //элемент (кнопка или ось) ввода
	class Command;   //команда ввода

	typedef boost::shared_ptr<Command> command_ptr;

	class Helper;       //базовый класс обьекта-посредника для получения информации о событиях ввода
	class Button;       //кнопка (нажатие и отжатие клавиш)
	class Trigger;      //триггер (переключение состояния при каждом НАжатии клавиш)
	class key_up;        //ОТжатие клавиши
	class key_down;      //НАжатие на клавишу
	class RelativeAxis; //относительная ось (сдвиг оси)
	class AbsoluteAxis; //абсолютная ось (координата оси)
	class KeyStream;

	enum controls
	{
		KeyEscape,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,
		Key0,
		KeyMinus,
		KeyEquals,
		KeyBack,
		KeyTab,
		KeyQ,
		KeyW,
		KeyE,
		KeyR,
		KeyT,
		KeyY,
		KeyU,
		KeyI,
		KeyO,
		KeyP,
		KeyLBracket,
		KeyRBracket,
		KeyReturn,
		KeyLControl,
		KeyA,
		KeyS,
		KeyD,
		KeyF,
		KeyG,
		KeyH,
		KeyJ,
		KeyK,
		KeyL,
		KeySemicolon,
		KeyApostrophe,
		KeyGrave,
		KeyLShift,
		KeyBackSlash,
		KeyZ,
		KeyX,
		KeyC,
		KeyV,
		KeyB,
		KeyN,
		KeyM,
		KeyComma,
		KeyPeriod,
		KeySlash,
		KeyRShift,
		KeyMultiply,
		KeyLMenu,
		KeySpace,
		KeyCapital,
		KeyF1,
		KeyF2,
		KeyF3,
		KeyF4,
		KeyF5,
		KeyF6,
		KeyF7,
		KeyF8,
		KeyF9,
		KeyF10,
		KeyNumLock,
		KeyScroll,
		KeyNumPad7,
		KeyNumPad8,
		KeyNumPad9,
		KeySubtract,
		KeyNumPad4,
		KeyNumPad5,
		KeyNumPad6,
		KeyAdd,
		KeyNumPad1,
		KeyNumPad2,
		KeyNumPad3,
		KeyNumPad0,
		KeyDecimal,
		KeyOEM_102,
		KeyF11,
		KeyF12,
		KeyF13,
		KeyF14,
		KeyF15,
		KeyKana,
		KeyABNT_C1,
		KeyConvert,
		KeyNoConvert,
		KeyYen,
		KeyABNT_C2,
		KeyNumPadEquals,
		KeyPrevTrack,
		KeyAt,
		KeyColon,
		KeyUnderLine,
		KeyKANJI,
		KeyStop,
		KeyAx,
		KeyUnlabeled,
		KeyNextTrack,
		KeyNumPadEnter,
		KeyRControl,
		KeyMute,
		KeyCalculator,
		KeyPlayPause,
		KeyMediaStop,
		KeyVolumeDown,
		KeyVolumeUp,
		KeyWebHome,
		KeyNumPadComma,
		KeyDivide,
		KeySysRQ,
		KeyRMenu,
		KeyPause,
		KeyHome,
		KeyUp,
		KeyPrior,
		KeyLeft,
		KeyRight,
		KeyEnd,
		KeyDown,
		KeyNext,
		KeyInsert,
		KeyDelete,
		KeyLWin,
		KeyRWin,
		KeyApps,

		ButtonLeft,
		ButtonMid,
		ButtonRight,

		button0 = ButtonLeft,
		button1 = ButtonRight,
		button2 = ButtonMid,
		button4,
		button5,
		button6,
		button7,

		AxisX,
		AxisY,
		AxisWheel,

		InvalidControl
	};

	//input device
	class device
	{
	public:
		enum type_t
		{
			keyboard = 0,
			mouse	 = 1,
			joystick = 2,
			total_number = 3,
			invalid = 255,
		};

		device(type_t type, int index, input_impl &input_system);
		virtual ~device();

		//get device type
		type_t type () const {return m_type;}
		//get device index
		int get_device_index () const {return m_index;}

		input_impl& get_impl()       {return m_input;}

		// get control
		Control* get_control       (controls	contol_name);
		Control* get_control       (const std::wstring &contol_name);

		// is such control present in device
		bool is_control_present (controls	contol_name) const;
		bool is_control_present (const std::wstring &contol_name) const;

	protected:
		friend class input_impl;
		//add button
		void add_button (controls contol_name); 
		//add axis
		void add_axis   (controls contol_name); 
		//deattach command from all controls
		void detach_command (command_ptr command); 

	private:
		int			m_index;
		type_t		m_type;
		input_impl &m_input;

		typedef std::map<controls, Control*> controls_map;

		//owned input controls
		controls_map m_controls; 
	};


	std::wstring Device2String (device::type_t type);
	device::type_t String2Device (const std::wstring &name);

	std::wstring Control2String (controls type);
	controls String2Control (const std::wstring &name); 

	class input_impl;

	class Input: boost::noncopyable
	{
	public:
		static Input& get  ();
		static void destroy ();

		void init (input_impl*);

        static bool set_mode (bool exclusive=false, bool foreground=true);
        static void load_xml (const std::string &xml);
        static void load (const std::string &filename);
        static void update();
        static void save_xml(std::string &xml);

        static device* get_device (device::type_t device_type, int indx=0);
        static device* get_device (const std::wstring &sDeviceName, int indx=0);

        static bool is_present (device::type_t device_type, int indx=0);
        static bool is_present (const std::wstring &sDeviceName, int indx=0);

		static device* get_mouse (int indx = 0)
		{
			return get_device(device::mouse, indx);
		}

		static device* get_keyboard (int indx = 0)
		{
			return get_device(device::keyboard, indx);
		}

		static device* get_joystick (int indx = 0)
		{
			return get_device(device::joystick, indx);
		}

		static Control* get_control(device::type_t device_type, int dev_index, controls control);
		static Control* get_control(device::type_t device_type, controls control)
		{
			return get_control(device_type, 0, control);
		}

        static void			add_command       (const std::string &command_name);
        static command_ptr  get_command       (const std::string &command_name);
        static bool			is_command_present(const std::string &command_name);
        static void			detach_command    (command_ptr command);

	private:
		Input();
		~Input();

		static Input            *ms_instance;
		std::auto_ptr<input_impl> m_impl;
	};

	//элемент (кнопка, ось и т.п.) устройства ввода
	class Control
	{
	public:        
		//виды контролов
		enum type_t
		{
			Button,
			Axis
		};

		Control(controls name, type_t type, device &device);

		controls name () const {return m_name;}
		type_t    type () const {return m_type;}

		//добавить наблюдателя
		void bind (command_ptr command);
		void bind (const std::string &command_name);

		//удалить наблюдателя
		void unbind (command_ptr command);
		void unbind (const std::string &command_name);

		//добавлен ли такой наблюдатель
		bool is_bind (command_ptr command);
		bool is_bind (const std::string &command_name);

		//уведомить наблюдателей о своем изменении
		void notify_all (); 

		bool get_press() const {return m_press;}
		int get_delta() const {return m_delta;}
		int get_time() const {return m_time;}

	private:
		device    &m_device; //устройство, которому принадлежит контрол
		type_t      m_type;   //тип контрола
		controls  m_name;   //'имя' контрола

		friend class input_impl;
		int  m_time;
		bool m_press;
		int  m_delta;

		typedef std::list<command_ptr> CommandsList;
		typedef CommandsList::iterator commands_iter;

		//команды, к которой привязан контрол
		CommandsList m_commands;
	};

	//Helper

	//параметр для Helper::Handler
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

	//обьект-посредник для получения информации о событиях ввода
	class Helper
	{
	public:
		typedef boost::function<void(const helper_event&)> Handler;

		Helper ();
		explicit Helper (const std::string &commandName);
		virtual ~Helper();

		void attach (const std::string &commandName);
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

	// Button, обьект-посредник "кнопка"
	class Button: public Helper
	{
	public:
		typedef boost::function<void(bool)> ButtonHandler;
		Button ();
		explicit Button (const std::string &commandName);

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
		explicit Trigger (const std::string &commandName);

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

	// key_up, обьект-посредник "ОТжатие клавиши"
	class key_up : public Helper
	{
	public:
		typedef boost::function<void()> KeyUpHandler;
		key_up ();
		explicit key_up (const std::string &commandName);

		void operator += (KeyUpHandler handler);

	protected:
		friend class Command;
		virtual void notify (const Control &control);

	private:
		std::list<KeyUpHandler> m_keyupHandlers;
	};

	// key_down
	//обьект-посредник "НАжатие клавиши"
	class key_down: public Helper
	{
	public:
		typedef boost::function<void()> KeyDownHandler;
		key_down ();
		explicit key_down (const std::string &commandName);

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
		explicit RelativeAxis (const std::string &commandName);

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
		explicit AbsoluteAxis (const std::string &commandName);

		void operator += (AbsoluteAxisHandler handler);
		operator int () const {return m_pos;}

		int  getMin () const {return m_min;}
		void setMin (int value);

		int  getMax () const {return m_max;}
		void setMax (int value);

		int  position () const {return m_pos;}		
		void position (int value);

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
	class Cursor: public Helper, public event::listener
	{
	public:
		typedef boost::function<void(float,float)> CursorHandler;

		Cursor ();

		void operator += (CursorHandler handler);

		float getX () const {return m_x;}
		float getY () const {return m_y;}
		void  position (float x, float y);

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

	// mouse
	//обьект-посредник "мышь"
	class mouse: public Cursor
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

		mouse();

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
	//параметр для KeyStream::KeyStreamHandler
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

	//обьект-посредник "поток символов"
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

	//input system command
	class Command
	{
	public:
		Command(const std::string &name, input_impl &input);

		const std::string& name() const;
		void lock();
		void unlock();
		bool is_locked() const;

	protected:
		friend class Control;
		void notify_all (const Control &c);

	protected:
		friend class Helper;
		void attach (Helper *h);
		void detach (Helper *h);

	private:
		bool          m_is_locked;
		input_impl    &m_input;
		std::string   m_name;

		typedef std::list<Helper*> helpers_list;
		typedef helpers_list::iterator helpers_iter;

		helpers_list m_helpers;
	};

	inline const std::string& Command::name() const 
	{
		return m_name;
	}

	inline void Command::lock() 
	{
		m_is_locked = true;
	}

	inline void Command::unlock() 
	{
		m_is_locked = false;
	}

	inline bool Command::is_locked() const 
	{
		return m_is_locked;
	}
}