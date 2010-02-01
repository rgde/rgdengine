#include "stdafx.h"

#include <windows.h>
#include "vector.h"
#include "utils.h"

namespace gui
{

#pragma message ("Use portable UTF conversion from http://sourceforge.net/projects/utfcpp/ !!!")

std::wstring UTF8ToUTF16(const std::string& text)
{
	// translate from UTF-8 to UTF-16
	std::wstring out;

	size_t len = text.length();
	char* src = new char[len+1];
	memset(src, 0, len+1);
	memcpy(src, text.c_str(), len);

	size_t needed = ::MultiByteToWideChar(CP_UTF8, NULL, src, (int)len+1, NULL, NULL);
	if(needed)
	{
		wchar_t* dst = new wchar_t[needed+1];
		memset(dst, 0, needed+1);
		if(::MultiByteToWideChar(CP_UTF8, NULL, src, (int)len+1, dst, (int)needed))
		{
			out = dst;
		}
		delete [] dst;
	}
	delete [] src;
	return out;
}

std::string UTF16ToUTF8(const std::wstring& wtext)
{
	// translate from UTF-16 to UTF-8
	std::string out;
	size_t len = wtext.length();

	size_t needed = ::WideCharToMultiByte(CP_UTF8, NULL, wtext.c_str(), (int)len, NULL, NULL, NULL, NULL);
	if(needed)
	{
		char* dst = new char[needed+1];
		memset(dst, 0, needed+1);
		if(::WideCharToMultiByte(CP_UTF8, NULL, wtext.c_str(), (int)len, dst, (int)needed, NULL, NULL))
		{
			out = dst;
		}
		delete [] dst;
	}
	return out;	
}

point StringToPoint(const std::string& str)
{
	int x, y;
	sscanf(str.c_str(), "%d %d", &x, &y);
	return point((float)x, (float)y);
}

std::string PointToString(const point& val)
{
	char buff[128] = {0};
	_snprintf(buff, sizeof (buff), "%d %d", val.x, val.y);
	return std::string(buff);
}

bool StringToBool(const std::string& str)
{
	return (str == "true" || str == "True");
}

const std::string& BoolToString(bool val)
{
	static const std::string true_str("true");
	static const std::string false_str("false");
	return val ? true_str : false_str;
}

}