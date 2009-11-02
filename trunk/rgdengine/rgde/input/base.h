#pragma once

namespace input
{
    class input_impl; //система ввода
    class device_dx9;    //устройство ввода
    class Control;   //элемент (кнопка или ось) ввода
    class Command;   //команда ввода

    typedef boost::shared_ptr<Command> command_ptr;

	class Helper;       //базовый класс обьекта-посредника для получения информации о событиях ввода
	class Button;       //кнопка (нажатие и отжатие клавиш)
	class Trigger;      //триггер (переключение состояния при каждом НАжатии клавиш)
	class KeyUp;        //ОТжатие клавиши
	class key_down;      //НАжатие на клавишу
	class RelativeAxis; //относительная ось (сдвиг оси)
	class AbsoluteAxis; //абсолютная ось (координата оси)
	class KeyStream;

	namespace types
	{
		//перечисление существующих устройств ввода
		enum device
		{
			Keyboard,
			Mouse,
			Joystick,

			InvalidDevice,
		};

		//перечисление существующих элементов ввода
		enum control
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
			key_down,
			KeyNext,
			KeyInsert,
			KeyDelete,
			KeyLWin,
			KeyRWin,
			KeyApps,

			ButtonLeft,
			ButtonMid,
			ButtonRight,

			AxisX,
			AxisY,
			AxisWheel,

			InvalidControl
		};
	}

	//получение строки, соответствующей enum-значению
	std::wstring Device2String (types::device name);

	//получение enum-значения, соответствующего строке
    types::device String2Device (const std::wstring &str); 

	//получение строки, соответствующей enum-значению
    std::wstring Control2String (types::control name);

	//получение enum-значения, соответствующего строке
    types::control String2Control (const std::wstring &str); 
}