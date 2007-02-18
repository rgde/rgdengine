#include "precompiled.h"

#include "input/ILBase.h"


namespace input
{

  //локальный класс для создания единственного статического экземпляра
  //этого класса, который нужен для того, чтобы в его конструкторе заполнить
  //std::map<EDevice, std::string>
  class LocalClassDeviceEnum2String
  {
    typedef std::map<EDevice, std::string> Enum2String;
    Enum2String enum2string;

  public:
    //коструктор, в котором для каждого enum-значения задается соответствующая ему строка
    LocalClassDeviceEnum2String ()
    {
      enum2string[Keyboard] = "Keyboard";
      enum2string[Mouse]    = "Mouse";
      //... другие устройства ввода
    }

    //получение строки, соответствующей enum-значению
    const std::string& EDevice2String (EDevice eDevice)
    {
      Enum2String::const_iterator i = enum2string.find(eDevice);
      if (i != enum2string.end())
        return i->second;

      //... сообщить, что для такого enum-значения строки не найдено
      static std::string empty = "";
      return empty;
    }

    //получение enum-значения, соответствующего строке
    EDevice String2EDevice (const std::string &str)
    {
			Enum2String::const_iterator	i	= enum2string.begin();
			Enum2String::const_iterator end	= enum2string.end();
      while (i != end)
      {
        if (i->second == str)
          return i->first;
        ++i;
      }

      //... сообщить, что для такой строки нет соответствующего ему enum-значения
      return InvalidDevice;
    }
  };

  //локальный класс для создания единственного статического экземпляра
  //этого класса, который нужен для того, чтобы в его кострукторе заполнить
  //std::map<ControlEnum, std::string>
  class LocalClassControlEnum2String
  {
    typedef std::map<EControl, std::string> Enum2String;
    Enum2String enum2string;

  public:
    //коструктор, в котором для каждого enum-значения задается соответствующая ему строка
    LocalClassControlEnum2String ()
    {
      enum2string [KeyEscape      ] = "KeyEscape";
      enum2string [Key1           ] = "Key1";
      enum2string [Key2           ] = "Key2";
      enum2string [Key3           ] = "Key3";
      enum2string [Key4           ] = "Key4";
      enum2string [Key5           ] = "Key5";
      enum2string [Key6           ] = "Key6";
      enum2string [Key7           ] = "Key7";
      enum2string [Key8           ] = "Key8";
      enum2string [Key9           ] = "Key9";
      enum2string [Key0           ] = "Key0";
      enum2string [KeyMinus       ] = "KeyMinus";
      enum2string [KeyEquals      ] = "KeyEquals";
      enum2string [KeyBack        ] = "KeyBack";
      enum2string [KeyTab         ] = "KeyTab";
      enum2string [KeyQ           ] = "KeyQ";
      enum2string [KeyW           ] = "KeyW";
      enum2string [KeyE           ] = "KeyE";
      enum2string [KeyR           ] = "KeyR";
      enum2string [KeyT           ] = "KeyT";
      enum2string [KeyY           ] = "KeyY";
      enum2string [KeyU           ] = "KeyU";
      enum2string [KeyI           ] = "KeyI";
      enum2string [KeyO           ] = "KeyO";
      enum2string [KeyP           ] = "KeyP";
      enum2string [KeyLBracket    ] = "KeyLBracket";
      enum2string [KeyRBracket    ] = "KeyRBracket";
      enum2string [KeyReturn      ] = "KeyReturn";
      enum2string [KeyLControl    ] = "KeyLControl";
      enum2string [KeyA           ] = "KeyA";
      enum2string [KeyS           ] = "KeyS";
      enum2string [KeyD           ] = "KeyD";
      enum2string [KeyF           ] = "KeyF";
      enum2string [KeyG           ] = "KeyG";
      enum2string [KeyH           ] = "KeyH";
      enum2string [KeyJ           ] = "KeyJ";
      enum2string [KeyK           ] = "KeyK";
      enum2string [KeyL           ] = "KeyL";
      enum2string [KeySemicolon   ] = "KeySemicolon";
      enum2string [KeyApostrophe  ] = "KeyApostrophe";
      enum2string [KeyGrave       ] = "KeyGrave";
      enum2string [KeyLShift      ] = "KeyLShift";
      enum2string [KeyBackSlash   ] = "KeyBackSlash";
      enum2string [KeyZ           ] = "KeyZ";
      enum2string [KeyX           ] = "KeyX";
      enum2string [KeyC           ] = "KeyC";
      enum2string [KeyV           ] = "KeyV";
      enum2string [KeyB           ] = "KeyB";
      enum2string [KeyN           ] = "KeyN";
      enum2string [KeyM           ] = "KeyM";
      enum2string [KeyComma       ] = "KeyComma";
      enum2string [KeyPeriod      ] = "KeyPeriod";
      enum2string [KeySlash       ] = "KeySlash";
      enum2string [KeyRShift      ] = "KeyRShift";
      enum2string [KeyMultiply    ] = "KeyMultiply";
      enum2string [KeyLMenu       ] = "KeyLMenu";
      enum2string [KeySpace       ] = "KeySpace";
      enum2string [KeyCapital     ] = "KeyCapital";
      enum2string [KeyF1          ] = "KeyF1";
      enum2string [KeyF2          ] = "KeyF2";
      enum2string [KeyF3          ] = "KeyF3";
      enum2string [KeyF4          ] = "KeyF4";
      enum2string [KeyF5          ] = "KeyF5";
      enum2string [KeyF6          ] = "KeyF6";
      enum2string [KeyF7          ] = "KeyF7";
      enum2string [KeyF8          ] = "KeyF8";
      enum2string [KeyF9          ] = "KeyF9";
      enum2string [KeyF10         ] = "KeyF10";
      enum2string [KeyNumLock     ] = "KeyNumLock";
      enum2string [KeyScroll      ] = "KeyScroll";
      enum2string [KeyNumPad7     ] = "KeyNumPad7";
      enum2string [KeyNumPad8     ] = "KeyNumPad8";
      enum2string [KeyNumPad9     ] = "KeyNumPad9";
      enum2string [KeySubtract    ] = "KeySubtract";
      enum2string [KeyNumPad4     ] = "KeyNumPad4";
      enum2string [KeyNumPad5     ] = "KeyNumPad5";
      enum2string [KeyNumPad6     ] = "KeyNumPad6";
      enum2string [KeyAdd         ] = "KeyAdd";
      enum2string [KeyNumPad1     ] = "KeyNumPad1";
      enum2string [KeyNumPad2     ] = "KeyNumPad2";
      enum2string [KeyNumPad3     ] = "KeyNumPad3";
      enum2string [KeyNumPad0     ] = "KeyNumPad0";
      enum2string [KeyDecimal     ] = "KeyDecimal";
      enum2string [KeyOEM_102     ] = "KeyOEM_102";
      enum2string [KeyF11         ] = "KeyF11";
      enum2string [KeyF12         ] = "KeyF12";
      enum2string [KeyF13         ] = "KeyF13";
      enum2string [KeyF14         ] = "KeyF14";
      enum2string [KeyF15         ] = "KeyF15";
      enum2string [KeyKana        ] = "KeyKana";
      enum2string [KeyABNT_C1     ] = "KeyABNT_C1";
      enum2string [KeyConvert     ] = "KeyConvert";
      enum2string [KeyNoConvert   ] = "KeyNoConvert";
      enum2string [KeyYen         ] = "KeyYen";
      enum2string [KeyABNT_C2     ] = "KeyABNT_C2";
      enum2string [KeyNumPadEquals] = "KeyNumPadEquals";
      enum2string [KeyPrevTrack   ] = "KeyPrevTrack";
      enum2string [KeyAt          ] = "KeyAt";
      enum2string [KeyColon       ] = "KeyColon";
      enum2string [KeyUnderLine   ] = "KeyUnderLine";
      enum2string [KeyKANJI       ] = "KeyKANJI";
      enum2string [KeyStop        ] = "KeyStop";
      enum2string [KeyAx          ] = "KeyAx";
      enum2string [KeyUnlabeled   ] = "KeyUnlabeled";
      enum2string [KeyNextTrack   ] = "KeyNextTrack";
      enum2string [KeyNumPadEnter ] = "KeyNumPadEnter";
      enum2string [KeyRControl    ] = "KeyRControl";
      enum2string [KeyMute        ] = "KeyMute";
      enum2string [KeyCalculator  ] = "KeyCalculator";
      enum2string [KeyPlayPause   ] = "KeyPlayPause";
      enum2string [KeyMediaStop   ] = "KeyMediaStop";
      enum2string [KeyVolumeDown  ] = "KeyVolumeDown";
      enum2string [KeyVolumeUp    ] = "KeyVolumeUp";
      enum2string [KeyWebHome     ] = "KeyWebHome";
      enum2string [KeyNumPadComma ] = "KeyNumPadComma";
      enum2string [KeyDivide      ] = "KeyDivide";
      enum2string [KeySysRQ       ] = "KeySysRQ";
      enum2string [KeyRMenu       ] = "KeyRMenu";
      enum2string [KeyPause       ] = "KeyPause";
      enum2string [KeyHome        ] = "KeyHome";
      enum2string [KeyUp          ] = "KeyUp";
      enum2string [KeyPrior       ] = "KeyPrior";
      enum2string [KeyLeft        ] = "KeyLeft";
      enum2string [KeyRight       ] = "KeyRight";
      enum2string [KeyEnd         ] = "KeyEnd";
      enum2string [KeyDown        ] = "KeyDown";
      enum2string [KeyNext        ] = "KeyNext";
      enum2string [KeyInsert      ] = "KeyInsert";
      enum2string [KeyDelete      ] = "KeyDelete";
      enum2string [KeyLWin        ] = "KeyLWin";
      enum2string [KeyRWin        ] = "KeyRWin";
      enum2string [KeyApps        ] = "KeyApps";
      enum2string [ButtonLeft     ] = "ButtonLeft";
      enum2string [ButtonMid      ] = "ButtonMid";
      enum2string [ButtonRight    ] = "ButtonRight";
      enum2string [AxisX          ] = "AxisX";
      enum2string [AxisY          ] = "AxisY";
      enum2string [AxisWheel      ] = "AxisWheel";
      //... другие контролы устройств ввода
    }

    //получение строки, соответствующей enum-значению
    const std::string& EControl2String (EControl eControl)
    {
      Enum2String::const_iterator i = enum2string.find(eControl);
      if (i != enum2string.end())
        return i->second;

      //... сообщить, что для такого enum-значения строки не найдено
      static std::string empty = "";
      return empty;
    }

    //получение enum-значения, соответствующего строке
    EControl String2EControl (const std::string &str)
    {
			Enum2String::const_iterator	i	= enum2string.begin();
			Enum2String::const_iterator	end	= enum2string.end();
      while (i != end)
      {
        if (i->second == str)
          return i->first;
        ++i;
      }

      //... сообщить, что для такой строки нет соответствующего ему enum-значения
      return InvalidControl;
    }
  };

  //единственный экземпляр класса LocalClassDeviceEnum2String
  //(используется только в этом файле)
  LocalClassDeviceEnum2String de2s; //'D'evice'E'num to 'S'tring

  //единственный экземпляр класса LocalClassControlEnum2String
  //(используется только в этом файле)
  LocalClassControlEnum2String ce2s; //'C'ontrol'E'num to 'S'tring

  //получение строки, соответствующей enum-значению
  const std::string& EDevice2String (EDevice eDevice)
  {
    return de2s.EDevice2String(eDevice);
  }

  //получение enum-значения, соответствующего строке
  EDevice String2EDevice (const std::string &str)
  {
    return de2s.String2EDevice(str);
  }

  //получение строки, соответствующей enum-значению
  const std::string& EControl2String (EControl eControl)
  {
    return ce2s.EControl2String(eControl);
  }

  //получение enum-значения, соответствующего строке
  EControl String2EControl (const std::string &str)
  {
    return ce2s.String2EControl(str);
  }

} //namespace input