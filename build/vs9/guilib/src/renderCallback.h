#pragma once

namespace gui
{
	typedef boost::function <void (BaseWindow* wnd, const Rect& dest, const Rect& clip)> AfterRenderCallbackFunc;

}