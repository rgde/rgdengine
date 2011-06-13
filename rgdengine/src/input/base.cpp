//base.cpp
#include "precompiled.h"

#include <rgde/input/input.h>

namespace input
{
	struct InputMap
	{
		std::map<std::wstring, device::type_t>  m_mapString2Device;
		std::map<std::wstring, controls> m_mapString2Control;

		InputMap()
		{
			m_mapString2Device[L"Keyboard"] = device::keyboard;
			m_mapString2Device[L"Mouse"]    = device::mouse;

			m_mapString2Control[L"KeyEscape"]       = KeyEscape;
			m_mapString2Control[L"Key1"]            = Key1;
			m_mapString2Control[L"Key2"]            = Key2;
			m_mapString2Control[L"Key3"]            = Key3;
			m_mapString2Control[L"Key4"]            = Key4;
			m_mapString2Control[L"Key5"]            = Key5;
			m_mapString2Control[L"Key6"]            = Key6;
			m_mapString2Control[L"Key7"]            = Key7;
			m_mapString2Control[L"Key8"]            = Key8;
			m_mapString2Control[L"Key9"]            = Key9;
			m_mapString2Control[L"Key0"]            = Key0;
			m_mapString2Control[L"KeyMinus"]        = KeyMinus;
			m_mapString2Control[L"KeyEquals"]       = KeyEquals;
			m_mapString2Control[L"KeyBack"]         = KeyBack;
			m_mapString2Control[L"KeyTab"]          = KeyTab;
			m_mapString2Control[L"KeyQ"]            = KeyQ;
			m_mapString2Control[L"KeyW"]            = KeyW;
			m_mapString2Control[L"KeyE"]            = KeyE;
			m_mapString2Control[L"KeyR"]            = KeyR;
			m_mapString2Control[L"KeyT"]            = KeyT;
			m_mapString2Control[L"KeyY"]            = KeyY;
			m_mapString2Control[L"KeyU"]            = KeyU;
			m_mapString2Control[L"KeyI"]            = KeyI;
			m_mapString2Control[L"KeyO"]            = KeyO;
			m_mapString2Control[L"KeyP"]            = KeyP;
			m_mapString2Control[L"KeyLBracket"]     = KeyLBracket;
			m_mapString2Control[L"KeyRBracket"]     = KeyRBracket;
			m_mapString2Control[L"KeyReturn"]       = KeyReturn;
			m_mapString2Control[L"KeyLControl"]     = KeyLControl;
			m_mapString2Control[L"KeyA"]            = KeyA;
			m_mapString2Control[L"KeyS"]            = KeyS;
			m_mapString2Control[L"KeyD"]            = KeyD;
			m_mapString2Control[L"KeyF"]            = KeyF;
			m_mapString2Control[L"KeyG"]            = KeyG;
			m_mapString2Control[L"KeyH"]            = KeyH;
			m_mapString2Control[L"KeyJ"]            = KeyJ;
			m_mapString2Control[L"KeyK"]            = KeyK;
			m_mapString2Control[L"KeyL"]            = KeyL;
			m_mapString2Control[L"KeySemicolon"]    = KeySemicolon;
			m_mapString2Control[L"KeyApostrophe"]   = KeyApostrophe;
			m_mapString2Control[L"KeyGrave"]        = KeyGrave;
			m_mapString2Control[L"KeyLShift"]       = KeyLShift;
			m_mapString2Control[L"KeyBackSlash"]    = KeyBackSlash;
			m_mapString2Control[L"KeyZ"]            = KeyZ;
			m_mapString2Control[L"KeyX"]            = KeyX;
			m_mapString2Control[L"KeyC"]            = KeyC;
			m_mapString2Control[L"KeyV"]            = KeyV;
			m_mapString2Control[L"KeyB"]            = KeyB;
			m_mapString2Control[L"KeyN"]            = KeyN;
			m_mapString2Control[L"KeyM"]            = KeyM;
			m_mapString2Control[L"KeyComma"]        = KeyComma;
			m_mapString2Control[L"KeyPeriod"]       = KeyPeriod;
			m_mapString2Control[L"KeySlash"]        = KeySlash;
			m_mapString2Control[L"KeyRShift"]       = KeyRShift;
			m_mapString2Control[L"KeyMultiply"]     = KeyMultiply;
			m_mapString2Control[L"KeyLMenu"]        = KeyLMenu;
			m_mapString2Control[L"KeySpace"]        = KeySpace;
			m_mapString2Control[L"KeyCapital"]      = KeyCapital;
			m_mapString2Control[L"KeyF1"]           = KeyF1;
			m_mapString2Control[L"KeyF2"]           = KeyF2;
			m_mapString2Control[L"KeyF3"]           = KeyF3;
			m_mapString2Control[L"KeyF4"]           = KeyF4;
			m_mapString2Control[L"KeyF5"]           = KeyF5;
			m_mapString2Control[L"KeyF6"]           = KeyF6;
			m_mapString2Control[L"KeyF7"]           = KeyF7;
			m_mapString2Control[L"KeyF8"]           = KeyF8;
			m_mapString2Control[L"KeyF9"]           = KeyF9;
			m_mapString2Control[L"KeyF10"]          = KeyF10;
			m_mapString2Control[L"KeyNumLock"]      = KeyNumLock;
			m_mapString2Control[L"KeyScroll"]       = KeyScroll;
			m_mapString2Control[L"KeyNumPad7"]      = KeyNumPad7;
			m_mapString2Control[L"KeyNumPad8"]      = KeyNumPad8;
			m_mapString2Control[L"KeyNumPad9"]      = KeyNumPad9;
			m_mapString2Control[L"KeySubtract"]     = KeySubtract;
			m_mapString2Control[L"KeyNumPad4"]      = KeyNumPad4;
			m_mapString2Control[L"KeyNumPad5"]      = KeyNumPad5;
			m_mapString2Control[L"KeyNumPad6"]      = KeyNumPad6;
			m_mapString2Control[L"KeyAdd"]          = KeyAdd;
			m_mapString2Control[L"KeyNumPad1"]      = KeyNumPad1;
			m_mapString2Control[L"KeyNumPad2"]      = KeyNumPad2;
			m_mapString2Control[L"KeyNumPad3"]      = KeyNumPad3;
			m_mapString2Control[L"KeyNumPad0"]      = KeyNumPad0;
			m_mapString2Control[L"KeyDecimal"]      = KeyDecimal;
			m_mapString2Control[L"KeyOEM_102"]      = KeyOEM_102;
			m_mapString2Control[L"KeyF11"]          = KeyF11;
			m_mapString2Control[L"KeyF12"]          = KeyF12;
			m_mapString2Control[L"KeyF13"]          = KeyF13;
			m_mapString2Control[L"KeyF14"]          = KeyF14;
			m_mapString2Control[L"KeyF15"]          = KeyF15;
			m_mapString2Control[L"KeyKana"]         = KeyKana;
			m_mapString2Control[L"KeyABNT_C1"]      = KeyABNT_C1;
			m_mapString2Control[L"KeyConvert"]      = KeyConvert;
			m_mapString2Control[L"KeyNoConvert"]    = KeyNoConvert;
			m_mapString2Control[L"KeyYen"]          = KeyYen;
			m_mapString2Control[L"KeyABNT_C2"]      = KeyABNT_C2;
			m_mapString2Control[L"KeyNumPadEquals"] = KeyNumPadEquals;
			m_mapString2Control[L"KeyPrevTrack"]    = KeyPrevTrack;
			m_mapString2Control[L"KeyAt"]           = KeyAt;
			m_mapString2Control[L"KeyColon"]        = KeyColon;
			m_mapString2Control[L"KeyUnderLine"]    = KeyUnderLine;
			m_mapString2Control[L"KeyKANJI"]        = KeyKANJI;
			m_mapString2Control[L"KeyStop"]         = KeyStop;
			m_mapString2Control[L"KeyAx"]           = KeyAx;
			m_mapString2Control[L"KeyUnlabeled"]    = KeyUnlabeled;
			m_mapString2Control[L"KeyNextTrack"]    = KeyNextTrack;
			m_mapString2Control[L"KeyNumPadEnter"]  = KeyNumPadEnter;
			m_mapString2Control[L"KeyRControl"]     = KeyRControl;
			m_mapString2Control[L"KeyMute"]         = KeyMute;
			m_mapString2Control[L"KeyCalculator"]   = KeyCalculator;
			m_mapString2Control[L"KeyPlayPause"]    = KeyPlayPause;
			m_mapString2Control[L"KeyMediaStop"]    = KeyMediaStop;
			m_mapString2Control[L"KeyVolumeDown"]   = KeyVolumeDown;
			m_mapString2Control[L"KeyVolumeUp"]     = KeyVolumeUp;
			m_mapString2Control[L"KeyWebHome"]      = KeyWebHome;
			m_mapString2Control[L"KeyNumPadComma"]  = KeyNumPadComma;
			m_mapString2Control[L"KeyDivide"]       = KeyDivide;
			m_mapString2Control[L"KeySysRQ"]        = KeySysRQ;
			m_mapString2Control[L"KeyRMenu"]        = KeyRMenu;
			m_mapString2Control[L"KeyPause"]        = KeyPause;
			m_mapString2Control[L"KeyHome"]         = KeyHome;
			m_mapString2Control[L"KeyUp"]           = KeyUp;
			m_mapString2Control[L"KeyPrior"]        = KeyPrior;
			m_mapString2Control[L"KeyLeft"]         = KeyLeft;
			m_mapString2Control[L"KeyRight"]        = KeyRight;
			m_mapString2Control[L"KeyEnd"]          = KeyEnd;
			m_mapString2Control[L"KeyDown"]			= KeyDown;
			m_mapString2Control[L"KeyNext"]         = KeyNext;
			m_mapString2Control[L"KeyInsert"]       = KeyInsert;
			m_mapString2Control[L"KeyDelete"]       = KeyDelete;
			m_mapString2Control[L"KeyLWin"]         = KeyLWin;
			m_mapString2Control[L"KeyRWin"]         = KeyRWin;
			m_mapString2Control[L"KeyApps"]         = KeyApps;
			m_mapString2Control[L"ButtonLeft"]      = ButtonLeft;
			m_mapString2Control[L"ButtonMid"]       = ButtonMid;
			m_mapString2Control[L"ButtonRight"]     = ButtonRight;
			m_mapString2Control[L"AxisX"]           = AxisX;
			m_mapString2Control[L"AxisY"]           = AxisY;
			m_mapString2Control[L"AxisWheel"]       = AxisWheel;
		}
	};
	static InputMap s_map;

    //получение строки, соответствующей enum-значению
	std::wstring Device2String (device::type_t name)
    {
		std::map<std::wstring, device::type_t>::iterator i = s_map.m_mapString2Device.begin();
		while (i != s_map.m_mapString2Device.end())
		{
			if (i->second == name)
				return i->first;
			++i;
		}

        return L"InvalidDevice";
    }

    //получение enum-значения, соответствующего строке
    device::type_t String2Device (const std::wstring &str)
    {
		std::map<std::wstring, device::type_t>::iterator i = s_map.m_mapString2Device.find(str);
		if (i != s_map.m_mapString2Device.end())
			return i->second;

		return device::invalid;
    }

    //получение строки, соответствующей enum-значению
    std::wstring Control2String (controls name)
    {
		std::map<std::wstring, controls>::iterator i = s_map.m_mapString2Control.begin();
		while (i != s_map.m_mapString2Control.end())
		{
			if (i->second == name)
				return i->first;
			++i;
		}

        return L"InvalidControl";
    }

    //получение enu m-значения, соответствующего строке
    controls String2Control (const std::wstring &str)
    {
		std::map<std::wstring, controls>::iterator i = s_map.m_mapString2Control.find(str);
		if (i != s_map.m_mapString2Control.end())
			return i->second;

        return InvalidControl;
    }
}