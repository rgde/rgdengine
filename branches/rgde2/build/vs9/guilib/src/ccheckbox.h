#pragma once
#include "cstatictext.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace gui
{

class  Checkbox : public StaticText
{
public:
	typedef Checkbox Self;
	Checkbox(System& sys, const std::string& name = "");
	virtual ~Checkbox();
	
	static const char* GetType() { return "Checkbox"; }
	virtual const char* getType() { return Self::GetType(); }

	virtual void render(const Rect& finalRect, const Rect& finalClip);

	bool isChecked() const { return m_checked; }
	void setChecked(bool check)
	{
		m_checked = check;
		invalidate();
	}

	virtual bool onMouseEnter();
	virtual bool onMouseLeave();
	virtual bool onMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state);
	virtual bool onKeyboardButton(EventArgs::Keys key, EventArgs::ButtonState state);

	virtual void init(xml::node& node);

protected:
	bool m_checked;
	bool m_hovered;

	ImagesetPtr m_imgset;
	const Image* m_imgUnchecked;
	const Image* m_imgChecked;
};
}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif