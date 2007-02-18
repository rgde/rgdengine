//ILBase.h

//IL - Input Library (by Dub)
//предварительные обьявления
//подключается во все остальные *.h файлы библиотеки IL

#ifndef _ILBase_
#define _ILBase_

#include <string>

namespace input
{

  ///////////////////
  // основные типы //
  ///////////////////
  class CCommand;
  class CButtonCommand;
  class CTriggerCommand;
  class CRelativeAxisCommand;
  class CAbsoluteAxisCommand;

  class CControl;
  class CButtonControl;
  class CTriggerControl;
  class CRelativeAxisControl;
  class CAbsoluteAxisControl;

  class CDevice;
  class CScheme;
  class CSystem;
  class ISchemeImpl;
  class ISystemImpl;

  class CEvent;
  class CButtonEvent;
  class CTriggerEvent;
  class CRelativeAxisEvent;
  class CAbsoluteAxisEvent;

  class IHandler;

  enum ECommand;

  ///////////////////////
  // глобальные enum'ы //
  ///////////////////////
  //перечисление существующих устройств ввода
  enum EDevice
  {
    Keyboard,
    Mouse,

    InvalidDevice,
  };

  //перечисление существующих элементов ввода
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


  const std::string& EDevice2String (EDevice eName);          //получение строки, соответствующей enum-значению
  EDevice            String2EDevice (const std::string &str); //получение enum-значения, соответствующего строке

  const std::string& EControl2String (EControl eName);         //получение строки, соответствующей enum-значению
  EControl           String2EControl (const std::string &str); //получение enum-значения, соответствующего строке

} //namespace input

#endif //_ILBase_