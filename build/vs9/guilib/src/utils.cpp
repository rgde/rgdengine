#include "stdafx.h"

#include "vector.h"
#include "utils.h"

#include "utf8.h"

namespace gui
{

// translate from UTF-8 to UTF-16
std::wstring UTF8ToUTF16(const std::string& text)
{	
	std::wstring out;
	utf8::utf8to16(text.begin(), text.end(), std::back_inserter(out));
	return out;
}

// translate from UTF-16 to UTF-8
std::string UTF16ToUTF8(const std::wstring& wtext)
{	
	std::string out;
	utf8::utf16to8(wtext.begin(), wtext.end(), std::back_inserter(out));
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
	if (str.size() == 1) {
		const char c = *str.begin();
		if (c == '1') return true;
		if (c == '0') return false;
	}

	// length of "true" is 4
	static const std::string true_str = "true";
	if (str.size() == 4) {
		for (size_t i = 0; i < true_str.length(); ++i)
		{
			if (tolower(str[i]) != true_str[i]) return false;
		}
		return true;
	}

	return false;
}

const std::string& BoolToString(bool val)
{
	static const std::string true_str("true");
	static const std::string false_str("false");
	return val ? true_str : false_str;
}

}