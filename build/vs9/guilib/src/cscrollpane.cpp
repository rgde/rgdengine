#include "StdAfx.h"
#include "image.h"
#include "imageset.h"
#include "renderer.h"
#include "cslider.h"
#include "cscrollpane.h"
#include "system.h"

namespace gui
{

ScrollPane::ScrollPane(System& sys, const std::string& name) 
	: BaseWindow(sys, name)
	, m_scroll(0)
	, m_target(0)
{
}

ScrollPane::~ScrollPane(void)
{
}

void ScrollPane::setTarget(BaseWindow* target)
{
	m_target = target;
	if(target && m_scroll)
	{
		addChild(target);
		bringToBack(target);
		layoutTarget();
		subscribe<events::SizedEvent, ScrollPane> (&ScrollPane::onTargetSized, m_target);				
	}
	else if(!target)
	{
		unsubscribe<events::SizedEvent>();
	}

	invalidate();
}

void ScrollPane::layoutTarget()
{
	if(m_target && m_scroll)
	{
		float doc = m_target->getSize().height;
		m_scroll->setDocumentSize(doc);
		float h = m_area.getHeight();

		float pos = m_scroll->getScrollPosition();
		float height = doc > h ? doc - h : doc;
		m_target->setPosition(point(0.f, -pos*height));
	}
}

bool ScrollPane::onLoad(void)
{
	WindowPtr scroll = findChild("scrollbar");
	if(scroll)
	{
		m_scroll = dynamic_cast<ScrollBar*>(scroll.get());
		if(m_scroll)
		{
			subscribe<events::TrackEvent, ScrollPane> (&ScrollPane::onTrack, m_scroll);
			Size sz(m_area.getSize());
			sz.width = m_scroll->getSize().width;
			m_scroll->setSize(sz);
			m_scroll->setAlwaysOnTop(true);
		}
	}

	return BaseWindow::onLoad();
}

bool ScrollPane::onSized(bool update)
{
	BaseWindow::onSized();

	if(m_scroll)
	{
		if(m_target)
		{
			float doc = m_target->getSize().height;
			m_scroll->setDocumentSize(doc);
		}
	}

	return true;
}

bool ScrollPane::onMouseWheel(int delta)
{
	if(m_scroll)
	{
		return m_scroll->onMouseWheel(delta);
	}
	return false;
}

void ScrollPane::onTrack(const events::TrackEvent& e)
{
	if(m_target && m_scroll)
	{
		float pos = m_scroll->getScrollPosition();
		float doc = m_target->getSize().height - m_area.getHeight();
		m_target->setPosition(point(0.f, -pos*doc));		
	}
}

void ScrollPane::onTargetSized(const events::SizedEvent& e)
{
	layoutTarget();	
}

void ScrollPane::init(xml::node& node)
{
	BaseWindow::init(node);
	
	//TODO: ???
}

}