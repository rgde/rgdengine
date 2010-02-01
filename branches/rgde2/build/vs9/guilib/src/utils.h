#pragma once

namespace gui
{
	std::wstring UTF8ToUTF16(const std::string& text);
	std::string UTF16ToUTF8(const std::wstring& wtext);

	Size StringToSize(const std::string& str);
	std::string SizeToString(const Size& val);

	point StringToPoint(const std::string& str);
	std::string PointToString(const point& val);

	bool StringToBool(const std::string& str);
	const std::string& BoolToString(bool val);
}
