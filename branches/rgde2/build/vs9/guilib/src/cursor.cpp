#include "StdAfx.h"
#include "imageset.h"
#include "renderer.h"
#include "cursor.h"

namespace gui
{


Cursor::Cursor(Renderer& r) :
	m_render(r),
	m_currentCursor(0)
{
}

Cursor::~Cursor(void)
{
}

void Cursor::setPosition(Point pt) 
{
	m_cursorPos = pt;
}

Point Cursor::getPosition() const
{	
	Size vp = m_render.getViewportSize();
	if(m_render.isAutoScale() && vp.m_width && vp.m_height)
	{
		Size sz = m_render.getOriginalSize();
		float cx = sz.m_width / vp.m_width;
		float cy = sz.m_height / vp.m_height;
		
		return Point(m_cursorPos.m_x * cx, m_cursorPos.m_y * cy);
	}
	else
	{
		return m_cursorPos;
	}
}


void Cursor::setType(const std::string& curname)
{
	if(m_cursors)
	{
		m_currentCursor = (*m_cursors)[curname];
	}
}

std::string Cursor::getType(void) const
{
	if(m_currentCursor)
	{
		return m_currentCursor->getName();
	}
	return "";
}

void Cursor::render()
{
	if(m_currentCursor)
	{
		Rect dest(getPosition(), m_currentCursor->getPixelRect().getSize());
		m_render.immediateDraw(*m_currentCursor, dest, 0.f, Rect(Point(0.f,0.f), m_render.getViewportSize()), Color(1.f, 1.f, 1.f), TopLeftToBottomRight);
	}
}

}