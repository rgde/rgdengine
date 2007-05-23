#pragma once

namespace input
{
    class InputImpl; //������� �����
    class Device;    //���������� �����
    class Control;   //������� (������ ��� ���) �����
    class Command;   //������� �����

    typedef boost::shared_ptr<Command> CommandPtr;

	class Helper;       //������� ����� �������-���������� ��� ��������� ���������� � �������� �����
	class Button;       //������ (������� � ������� ������)
	class Trigger;      //������� (������������ ��������� ��� ������ ������� ������)
	class KeyUp;        //������� �������
	class KeyDown;      //������� �� �������
	class RelativeAxis; //������������� ��� (����� ���)
	class AbsoluteAxis; //���������� ��� (���������� ���)
	class KeyStream;

	namespace types
	{
		//������������ ������������ ��������� �����
		enum EDevice
		{
			Keyboard,
			Mouse,
			Joystick,

			InvalidDevice,
		};

		//������������ ������������ ��������� �����
		enum EControl
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

			AxisX,
			AxisY,
			AxisWheel,

			InvalidControl
		};
	}

	//��������� ������, ��������������� enum-��������
	std::wstring Device2String (types::EDevice eName);

	//��������� enum-��������, ���������������� ������
    types::EDevice String2Device (const std::wstring &str); 

	//��������� ������, ��������������� enum-��������
    std::wstring Control2String (types::EControl eName);

	//��������� enum-��������, ���������������� ������
    types::EControl String2Control (const std::wstring &str); 
}