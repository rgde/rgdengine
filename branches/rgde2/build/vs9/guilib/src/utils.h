#pragma once

namespace gui
{
	std::wstring UTF8ToUTF16(const std::string& text);
	std::string UTF16ToUTF8(const std::wstring& wtext);

	Size StringToSize(const std::string& str);
	std::string SizeToString(const Size& val);
	Point StringToPoint(const std::string& str);
	std::string PointToString(const Point& val);
}
