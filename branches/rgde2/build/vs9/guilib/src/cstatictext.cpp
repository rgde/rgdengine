#include "StdAfx.h"
#include "cstatictext.h"
#include "utils.h"

#include "system.h"


namespace gui
{

StaticText::StaticText(System& sys, const std::string& name) :
	BaseWindow(sys, name),
	m_centred(true),
	m_format(Font::LeftAligned),
	m_spacing(1.f)
{
}

StaticText::~StaticText(void)
{
}

void StaticText::setText(const std::string& text)
{
	m_text = text;
	if (!m_centred)
	{
		size_t count = m_font->getFormattedLineCount(m_text, m_area, m_format);
		float height = m_font->getLineSpacing() * (float)count;
		m_area.setHeight(height);
	}
	invalidate();
}

void StaticText::setSpacing(float spacing)
{
	m_spacing = spacing;
	if(m_font)
		m_font->setSpacing(m_spacing);invalidate();
}

void StaticText::appendText(const std::string& text)
{
	m_text += text;
	if (!m_centred)
	{
		size_t count = m_font->getFormattedLineCount(m_text, m_area, m_format);
		float height = m_font->getLineSpacing() * (float)count;
		m_area.setHeight(height);
	}
	invalidate();
}

void StaticText::render(const Rect& finalRect, const Rect& finalClip)
{
	if(m_font)
	{
		Rect rc(finalRect);
		if(m_centred)
		{
			size_t count = m_font->getFormattedLineCount(m_text, finalRect, m_format);
			float height = m_font->getLineSpacing() * (float)count;
			float offset = (finalRect.getHeight() - height) / 2;
			rc.offset(point(0.f, offset));
		}
		m_font->drawText(m_text, rc, 1.0f, finalClip, m_format, m_foreColor, 1.f, 1.f);
	}
}

void StaticText::setFont(const std::string& font)
{
	if(font.empty())
		return;

	m_font = m_system.getWindowManager().loadFont(font);
	if(m_font)
		m_font->setSpacing(m_spacing);

	invalidate();
}

void StaticText::init(xml::node& node)
{
	BaseWindow::init(node);

	xml::node setting = node("Font");
	if(!setting.empty())
	{
		m_font = m_system.getWindowManager().loadFont(setting.first_child().value());
	}

	setting = node("Formatting");
	if(!setting.empty())
	{
		m_format = StringToFormatType(setting.first_child().value());
	}

	setting = node("Spacing");
	if(!setting.empty())
	{
		m_spacing = (float)atof(setting.first_child().value());
		if(m_font)
			m_font->setSpacing(m_spacing);
	}

	setting = node("Centred");
	if(!setting.empty())
	{
		m_centred = StringToBool(setting.first_child().value());
	}

	setting = node("Text");
	if(!setting.empty())
	{
		m_text = setting.first_child().value();
	}
}

}