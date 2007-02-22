//base.h
#pragma once

#include <boost/shared_ptr.hpp>
#include <string>
#include <list>

namespace input
{

    class CInputImpl; //система ввода
    class CDevice;    //устройство ввода
    class CControl;   //элемент (кнопка или ось) ввода
    class CCommand;   //команда ввода

    typedef boost::shared_ptr<CCommand> PCommand;

    class CHelper;       //базовый класс обьекта-посредника для получения информации о событиях ввода
    class CButton;       //кнопка (нажатие и отжатие клавиш)
    class CTrigger;      //триггер (переключение состояния при каждом НАжатии клавиш)
    class CKeyUp;        //ОТжатие клавиши
    class CKeyDown;      //НАжатие на клавишу
    class CRelativeAxis; //относительная ось (сдвиг оси)
    class CAbsoluteAxis; //абсолютная ось (координата оси)

    //перечисление существующих устройств ввода
    enum EDevice
    {
        Keyboard,
        Mouse,
        Joystick,

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

    std::wstring Device2String (EDevice eName);           //получение строки, соответствующей enum-значению
    EDevice      String2Device (const std::wstring &str); //получение enum-значения, соответствующего строке

    std::wstring Control2String (EControl eName);          //получение строки, соответствующей enum-значению
    EControl     String2Control (const std::wstring &str); //получение enum-значения, соответствующего строке

};