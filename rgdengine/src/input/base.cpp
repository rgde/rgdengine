//base.cpp
#include "precompiled.h"
#include "input/base.h"
#include <map>

namespace input
{
	struct InputMap
	{
		std::map<std::wstring, types::EDevice>  m_mapString2Device;
		std::map<std::wstring, types::EControl> m_mapString2Control;

		InputMap()
		{
			m_mapString2Device[L"Keyboard"] = types::Keyboard;
			m_mapString2Device[L"Mouse"]    = types::Mouse;

			m_mapString2Control[L"KeyEscape"]       = types::KeyEscape;
			m_mapString2Control[L"Key1"]            = types::Key1;
			m_mapString2Control[L"Key2"]            = types::Key2;
			m_mapString2Control[L"Key3"]            = types::Key3;
			m_mapString2Control[L"Key4"]            = types::Key4;
			m_mapString2Control[L"Key5"]            = types::Key5;
			m_mapString2Control[L"Key6"]            = types::Key6;
			m_mapString2Control[L"Key7"]            = types::Key7;
			m_mapString2Control[L"Key8"]            = types::Key8;
			m_mapString2Control[L"Key9"]            = types::Key9;
			m_mapString2Control[L"Key0"]            = types::Key0;
			m_mapString2Control[L"KeyMinus"]        = types::KeyMinus;
			m_mapString2Control[L"KeyEquals"]       = types::KeyEquals;
			m_mapString2Control[L"KeyBack"]         = types::KeyBack;
			m_mapString2Control[L"KeyTab"]          = types::KeyTab;
			m_mapString2Control[L"KeyQ"]            = types::KeyQ;
			m_mapString2Control[L"KeyW"]            = types::KeyW;
			m_mapString2Control[L"KeyE"]            = types::KeyE;
			m_mapString2Control[L"KeyR"]            = types::KeyR;
			m_mapString2Control[L"KeyT"]            = types::KeyT;
			m_mapString2Control[L"KeyY"]            = types::KeyY;
			m_mapString2Control[L"KeyU"]            = types::KeyU;
			m_mapString2Control[L"KeyI"]            = types::KeyI;
			m_mapString2Control[L"KeyO"]            = types::KeyO;
			m_mapString2Control[L"KeyP"]            = types::KeyP;
			m_mapString2Control[L"KeyLBracket"]     = types::KeyLBracket;
			m_mapString2Control[L"KeyRBracket"]     = types::KeyRBracket;
			m_mapString2Control[L"KeyReturn"]       = types::KeyReturn;
			m_mapString2Control[L"KeyLControl"]     = types::KeyLControl;
			m_mapString2Control[L"KeyA"]            = types::KeyA;
			m_mapString2Control[L"KeyS"]            = types::KeyS;
			m_mapString2Control[L"KeyD"]            = types::KeyD;
			m_mapString2Control[L"KeyF"]            = types::KeyF;
			m_mapString2Control[L"KeyG"]            = types::KeyG;
			m_mapString2Control[L"KeyH"]            = types::KeyH;
			m_mapString2Control[L"KeyJ"]            = types::KeyJ;
			m_mapString2Control[L"KeyK"]            = types::KeyK;
			m_mapString2Control[L"KeyL"]            = types::KeyL;
			m_mapString2Control[L"KeySemicolon"]    = types::KeySemicolon;
			m_mapString2Control[L"KeyApostrophe"]   = types::KeyApostrophe;
			m_mapString2Control[L"KeyGrave"]        = types::KeyGrave;
			m_mapString2Control[L"KeyLShift"]       = types::KeyLShift;
			m_mapString2Control[L"KeyBackSlash"]    = types::KeyBackSlash;
			m_mapString2Control[L"KeyZ"]            = types::KeyZ;
			m_mapString2Control[L"KeyX"]            = types::KeyX;
			m_mapString2Control[L"KeyC"]            = types::KeyC;
			m_mapString2Control[L"KeyV"]            = types::KeyV;
			m_mapString2Control[L"KeyB"]            = types::KeyB;
			m_mapString2Control[L"KeyN"]            = types::KeyN;
			m_mapString2Control[L"KeyM"]            = types::KeyM;
			m_mapString2Control[L"KeyComma"]        = types::KeyComma;
			m_mapString2Control[L"KeyPeriod"]       = types::KeyPeriod;
			m_mapString2Control[L"KeySlash"]        = types::KeySlash;
			m_mapString2Control[L"KeyRShift"]       = types::KeyRShift;
			m_mapString2Control[L"KeyMultiply"]     = types::KeyMultiply;
			m_mapString2Control[L"KeyLMenu"]        = types::KeyLMenu;
			m_mapString2Control[L"KeySpace"]        = types::KeySpace;
			m_mapString2Control[L"KeyCapital"]      = types::KeyCapital;
			m_mapString2Control[L"KeyF1"]           = types::KeyF1;
			m_mapString2Control[L"KeyF2"]           = types::KeyF2;
			m_mapString2Control[L"KeyF3"]           = types::KeyF3;
			m_mapString2Control[L"KeyF4"]           = types::KeyF4;
			m_mapString2Control[L"KeyF5"]           = types::KeyF5;
			m_mapString2Control[L"KeyF6"]           = types::KeyF6;
			m_mapString2Control[L"KeyF7"]           = types::KeyF7;
			m_mapString2Control[L"KeyF8"]           = types::KeyF8;
			m_mapString2Control[L"KeyF9"]           = types::KeyF9;
			m_mapString2Control[L"KeyF10"]          = types::KeyF10;
			m_mapString2Control[L"KeyNumLock"]      = types::KeyNumLock;
			m_mapString2Control[L"KeyScroll"]       = types::KeyScroll;
			m_mapString2Control[L"KeyNumPad7"]      = types::KeyNumPad7;
			m_mapString2Control[L"KeyNumPad8"]      = types::KeyNumPad8;
			m_mapString2Control[L"KeyNumPad9"]      = types::KeyNumPad9;
			m_mapString2Control[L"KeySubtract"]     = types::KeySubtract;
			m_mapString2Control[L"KeyNumPad4"]      = types::KeyNumPad4;
			m_mapString2Control[L"KeyNumPad5"]      = types::KeyNumPad5;
			m_mapString2Control[L"KeyNumPad6"]      = types::KeyNumPad6;
			m_mapString2Control[L"KeyAdd"]          = types::KeyAdd;
			m_mapString2Control[L"KeyNumPad1"]      = types::KeyNumPad1;
			m_mapString2Control[L"KeyNumPad2"]      = types::KeyNumPad2;
			m_mapString2Control[L"KeyNumPad3"]      = types::KeyNumPad3;
			m_mapString2Control[L"KeyNumPad0"]      = types::KeyNumPad0;
			m_mapString2Control[L"KeyDecimal"]      = types::KeyDecimal;
			m_mapString2Control[L"KeyOEM_102"]      = types::KeyOEM_102;
			m_mapString2Control[L"KeyF11"]          = types::KeyF11;
			m_mapString2Control[L"KeyF12"]          = types::KeyF12;
			m_mapString2Control[L"KeyF13"]          = types::KeyF13;
			m_mapString2Control[L"KeyF14"]          = types::KeyF14;
			m_mapString2Control[L"KeyF15"]          = types::KeyF15;
			m_mapString2Control[L"KeyKana"]         = types::KeyKana;
			m_mapString2Control[L"KeyABNT_C1"]      = types::KeyABNT_C1;
			m_mapString2Control[L"KeyConvert"]      = types::KeyConvert;
			m_mapString2Control[L"KeyNoConvert"]    = types::KeyNoConvert;
			m_mapString2Control[L"KeyYen"]          = types::KeyYen;
			m_mapString2Control[L"KeyABNT_C2"]      = types::KeyABNT_C2;
			m_mapString2Control[L"KeyNumPadEquals"] = types::KeyNumPadEquals;
			m_mapString2Control[L"KeyPrevTrack"]    = types::KeyPrevTrack;
			m_mapString2Control[L"KeyAt"]           = types::KeyAt;
			m_mapString2Control[L"KeyColon"]        = types::KeyColon;
			m_mapString2Control[L"KeyUnderLine"]    = types::KeyUnderLine;
			m_mapString2Control[L"KeyKANJI"]        = types::KeyKANJI;
			m_mapString2Control[L"KeyStop"]         = types::KeyStop;
			m_mapString2Control[L"KeyAx"]           = types::KeyAx;
			m_mapString2Control[L"KeyUnlabeled"]    = types::KeyUnlabeled;
			m_mapString2Control[L"KeyNextTrack"]    = types::KeyNextTrack;
			m_mapString2Control[L"KeyNumPadEnter"]  = types::KeyNumPadEnter;
			m_mapString2Control[L"KeyRControl"]     = types::KeyRControl;
			m_mapString2Control[L"KeyMute"]         = types::KeyMute;
			m_mapString2Control[L"KeyCalculator"]   = types::KeyCalculator;
			m_mapString2Control[L"KeyPlayPause"]    = types::KeyPlayPause;
			m_mapString2Control[L"KeyMediaStop"]    = types::KeyMediaStop;
			m_mapString2Control[L"KeyVolumeDown"]   = types::KeyVolumeDown;
			m_mapString2Control[L"KeyVolumeUp"]     = types::KeyVolumeUp;
			m_mapString2Control[L"KeyWebHome"]      = types::KeyWebHome;
			m_mapString2Control[L"KeyNumPadComma"]  = types::KeyNumPadComma;
			m_mapString2Control[L"KeyDivide"]       = types::KeyDivide;
			m_mapString2Control[L"KeySysRQ"]        = types::KeySysRQ;
			m_mapString2Control[L"KeyRMenu"]        = types::KeyRMenu;
			m_mapString2Control[L"KeyPause"]        = types::KeyPause;
			m_mapString2Control[L"KeyHome"]         = types::KeyHome;
			m_mapString2Control[L"KeyUp"]           = types::KeyUp;
			m_mapString2Control[L"KeyPrior"]        = types::KeyPrior;
			m_mapString2Control[L"KeyLeft"]         = types::KeyLeft;
			m_mapString2Control[L"KeyRight"]        = types::KeyRight;
			m_mapString2Control[L"KeyEnd"]          = types::KeyEnd;
			m_mapString2Control[L"KeyDown"]         = types::KeyDown;
			m_mapString2Control[L"KeyNext"]         = types::KeyNext;
			m_mapString2Control[L"KeyInsert"]       = types::KeyInsert;
			m_mapString2Control[L"KeyDelete"]       = types::KeyDelete;
			m_mapString2Control[L"KeyLWin"]         = types::KeyLWin;
			m_mapString2Control[L"KeyRWin"]         = types::KeyRWin;
			m_mapString2Control[L"KeyApps"]         = types::KeyApps;
			m_mapString2Control[L"ButtonLeft"]      = types::ButtonLeft;
			m_mapString2Control[L"ButtonMid"]       = types::ButtonMid;
			m_mapString2Control[L"ButtonRight"]     = types::ButtonRight;
			m_mapString2Control[L"AxisX"]           = types::AxisX;
			m_mapString2Control[L"AxisY"]           = types::AxisY;
			m_mapString2Control[L"AxisWheel"]       = types::AxisWheel;
		}
	};
	static InputMap s_map;

    //получение строки, соответствующей enum-значению
	std::wstring Device2String (types::EDevice eName)
    {
		std::map<std::wstring, types::EDevice>::iterator i = s_map.m_mapString2Device.begin();
		while (i != s_map.m_mapString2Device.end())
		{
			if (i->second == eName)
				return i->first;
			++i;
		}

        return L"InvalidDevice";
    }

    //получение enum-значения, соответствующего строке
    types::EDevice String2Device (const std::wstring &str)
    {
		std::map<std::wstring, types::EDevice>::iterator i = s_map.m_mapString2Device.find(str);
		if (i != s_map.m_mapString2Device.end())
			return i->second;

        return types::InvalidDevice;
    }

    //получение строки, соответствующей enum-значению
    std::wstring Control2String (types::EControl eName)
    {
		std::map<std::wstring, types::EControl>::iterator i = s_map.m_mapString2Control.begin();
		while (i != s_map.m_mapString2Control.end())
		{
			if (i->second == eName)
				return i->first;
			++i;
		}

        return L"InvalidControl";
    }

    //получение enu m-значения, соответствующего строке
    types::EControl String2Control (const std::wstring &str)
    {
		std::map<std::wstring, types::EControl>::iterator i = s_map.m_mapString2Control.find(str);
		if (i != s_map.m_mapString2Control.end())
			return i->second;

        return types::InvalidControl;
    }
}