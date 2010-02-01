#include "StdAfx.h"
#include "imageset.h"
#include "renderer.h"
#include "renderhelper.h"
#include "window.h"
#include "dragcontainer.h"
#include "tooltip.h"
#include "menu.h"
#include "windowmanager.h"
#include "system.h"

namespace gui
{

System::System(Renderer& render, const std::string& resourceDir, const std::string& scheme, LoggerCallback callback) : 
	m_logger(callback),
	m_render(render),
	m_bShowCursor(true),
	m_focusWindow(0),
	m_exclusiveInputWindow(0),
	m_captureWindow(0),
	m_rootWindow(0),
	m_containsMouse(0),
	m_cursor(render),
	m_sytemkeys(0),
	m_autoScale(true),
	m_dragging(false),
	m_dragfired(false),
	m_dragfreeze(false),
	m_dragWindow(0),
	m_dragThreshold(8.f),
	m_inTick(false),
	m_tickClear(false),
	m_ttdelay(2.f),
	m_ttlifetime(0.f),
	m_tttime(0.f),
	m_isContextMenu(false),
	m_logHelper(new LogHelper(callback)),
	m_renderHelper(new RenderHelper(render))
{
	logEvent(LogSystem, "GUI system initialization started");
	m_windowMgr.reset(new WindowManager(*this, resourceDir, scheme));
	m_cursor.setImageset(m_windowMgr->loadImageset("Cursors"));
	
	makeLuaBinding();
	logEvent(LogSystem, "GUI system initialization ended");

	reset(true);
}

System::~System(void)
{
}

void System::reset()
{
	reset(false);
}

void System::reset(bool complete)
{
	m_render.cleanup(complete);
	m_windowMgr->reset(complete);
	
	logEvent(LogSystem, "Resetting window tree...");
	m_tickedWnd.clear();
	m_subscribeTickWnd.clear();
	m_dragContainer = 0;
	m_tooltipWindow = 0;
	m_dragWindow = 0;
	m_rootWindow = 0;
	m_containsMouse = 0;
	m_focusWindow = 0;
	m_exclusiveInputWindow = 0;
	m_captureWindow = 0;
	m_dragging = false;
	m_dragfired = false;
	m_dragfreeze = false;

	// правильно ли создавать базовое окошко в обход манагера? 
	BaseWindow* rootWindow = new (std::nothrow) BaseWindow(*this, "systemroot");
	m_rootWindow = rootWindow;
	if(!rootWindow)
		throw std::exception("Couldn't create root window!");

	DragContainer* drag = new (std::nothrow) DragContainer(*this, "systemdrag");
	m_dragContainer = drag;
	if(!drag)
		throw std::exception("Couldn't create drag window!");

	drag->reset();

	Tooltip* tooltip = new (std::nothrow) Tooltip(*this, "systemtooltip");
	m_tooltipWindow = tooltip;
	if(!tooltip)
		throw std::exception("Couldn't create tooltip window!");
	
	tooltip->reset();

	Menu* menu = new (std::nothrow) Menu(*this, "systemmenu");
	m_menuWindow = menu;
	if(!menu)
		throw std::exception("Couldn't create menu window!");
	m_windowMgr->loadLeafWindow(m_menuWindow, "base/menu.xml");
	
	menu->reset();

	rootWindow->setArea(Rect(point(0.f, 0.f), m_render.getSize())); // full window area
	rootWindow->setVisible(true);
	rootWindow->addChild(m_dragContainer);
	rootWindow->addChild(m_tooltipWindow);
	rootWindow->addChild(m_menuWindow);
	rootWindow->setAcceptDrop(true);
	
	logEvent(LogSystem, "Gui subsystem is ready");
}

WindowPtr System::getRootPtr()
{
	return m_rootWindow;
}

Tooltip* System::getTooltip() const
{ 
	return static_cast<Tooltip*>(m_tooltipWindow.get()); 
}

DragContainer* System::getDragContainer() const
{
	return static_cast<DragContainer*>(m_dragContainer.get()); 
}

Menu* System::getMenu() const
{ 
	return static_cast<Menu*>(m_menuWindow.get()); 
}

BaseWindow*	System::createWindow(BaseWindow* parent, const std::string& name, const std::string& type)
{
	WindowPtr p;
	if(parent)
	{
		p = m_windowMgr->createWindow(type, name);
		if(p)
			parent->addChild(p);
	}
	return p.get();
}

BaseWindow*	System::loadXml(BaseWindow& parent, const std::string& filename)
{
	WindowPtr p = loadXml_(filename);
	if(p)
	{
		assert(!p->getParent() && "This window must be parentless!");
		parent.addChild(p);
		return p.get();
	}
	return 0;
	
}
BaseWindow*	System::loadXml(const std::string& filename)
{
	WindowPtr p = loadXml_(filename);
	if(p)
	{
		assert(p.get() == &getRootWindow()  && "This window must have the root window as a parent!");
		return p.get();
	}
	return 0;	
}

WindowPtr System::loadXml_(const std::string& filename)
{
	logEvent(LogSystem, std::string("Loading ") + filename);
	WindowPtr p = m_windowMgr->loadXml(filename);
	if(p)
		logEvent(LogSystem, filename + " successfuly loaded.");
	else
		logEvent(LogWarning, filename + " failed to load.");
	
	return p;
}

void System::init()
{
	 getRootWindow().onGameEvent("On_Init");
}

bool System::handleMouseMove(int x, int y)
{
	if(m_exclusiveInputWindow)
		return m_exclusiveInputWindow->onMouseMove();

	const point pt((float)x, (float)y);
	m_cursor.setPosition(pt);

	if(m_rootWindow)
	{
		if(m_dragging && m_dragWindow != 0)
		{
			if(m_captureWindow)
				m_captureWindow = 0; // молча снимаем capture поскольку в данной ситуации оно лишнее
			
			if(!m_dragfired)
			{
				point t = m_cursor.getPosition() - m_dragOffset;
				if(sqrt(t.x*t.x + t.y*t.y) > m_dragThreshold)
				{
					point off = m_dragWindow->transformToWndCoord(m_dragOffset);
					off -= m_dragWindow->getPosition();
					if(!startDrag(m_dragWindow, off))
					{
						m_dragging = false;
						m_dragWindow = 0;
					}
				}
			}
		}
		
		BaseWindow* mouseWnd = getTargetWindow(m_cursor.getPosition());
		if(m_dragfired)
		{
			getDragContainer()->update(mouseWnd, m_cursor.getPosition());
		}
		if((mouseWnd != m_containsMouse) && !m_captureWindow)
		{
			// кем является новое окно по отношению к текущему
			//bool isChild = mouseWnd ? mouseWnd->isChildrenOf(m_containsMouse) : false;
			//bool isParent = m_containsMouse ? m_containsMouse->isChildrenOf(mouseWnd) : false;

			m_activateTooltip = false;
			
			if(m_containsMouse)
			{
				hideTooltip(m_containsMouse);

				//if(!isChild)
					m_containsMouse->onMouseLeave();
			}
			
			m_containsMouse = mouseWnd;

			if(m_containsMouse)
			{
				if(m_containsMouse->hasTooltip())
				{
					m_activateTooltip = true;
					m_tttime = 0.f;
				}
				//if(!isParent)
					m_containsMouse->onMouseEnter();
			}
		}

		BaseWindow* target = m_containsMouse;
		if(m_captureWindow)
		{
			target = m_captureWindow;
		}

		if(!target)
			target = m_rootWindow.get();
		
		while(target)
		{
			if(target->onMouseMove())
				return true;
			target = const_cast<BaseWindow*>(target->getParent());
		}
	}

	return false;
}

bool System::handleMouseWheel(int diff)
{
	if(m_exclusiveInputWindow)
		return m_exclusiveInputWindow->onMouseWheel(diff);

	m_activateTooltip = false;
	hideTooltip(m_containsMouse);

	if(m_rootWindow)
	{
		BaseWindow* target = m_containsMouse;
		if(m_captureWindow)
		{
			target = m_captureWindow;
		}
		
		if(!target)
			target = m_rootWindow.get();
		
		while(target)
		{
			if(target->onMouseWheel(diff))
				return true;
			target = const_cast<BaseWindow*>(target->getParent());
		}
	}

	return false;
}

bool System::handleMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state)
{
	if(m_exclusiveInputWindow)
		return m_exclusiveInputWindow->onMouseButton(btn, state);

	m_activateTooltip = false;
	hideTooltip(m_containsMouse);

	if(m_rootWindow)
	{
		BaseWindow* target = m_containsMouse;
		if(m_captureWindow)
		{
			target = m_captureWindow;
		}
		
		if(!target)
			target = m_rootWindow.get();
		
		{
			if(m_focusWindow != target)
			{
				if(target->isTabStop())
					queryInputFocus(target);
				else
					queryInputFocus(0);
			}

			// Drag'n'Drop support
			if(btn == EventArgs::Left)
			{
				if(!m_dragfired)
				{
					if(target != m_rootWindow.get() && target != m_captureWindow)
					{
						if(target->isDragable())
						{
							if(state == EventArgs::Down)
							{
								m_dragfired = false;
								m_dragOffset = m_cursor.getPosition();
								m_dragWindow = target;
								m_dragging = true;
							}
						}
					}
				}
				if(state == EventArgs::Up && m_dragging)
				{
					if(!m_dragfired)
					{
						m_dragging = false;
					}
				}
			}

			target->rise();

			if(target != m_menuWindow.get())
				getMenu()->reset();

			while(target)
			{
				if(target->onMouseButton(btn, state))
					return true;
				target = const_cast<BaseWindow*>(target->getParent());
			}
		}
	}
	
	return false;
}

bool System::handleMouseDouble(EventArgs::MouseButtons btn)
{
	if(m_exclusiveInputWindow)
		return m_exclusiveInputWindow->onMouseDouble(btn);

	m_activateTooltip = false;
	hideTooltip(m_containsMouse);

	if(m_rootWindow)
	{
		BaseWindow* target = m_containsMouse;
		if(m_captureWindow)
		{
			target = m_captureWindow;
		}
		
		if(!target)
			target = m_rootWindow.get();

		target->rise();

		while(target)
		{
			if(target->onMouseDouble(btn))
				return true;
			target = const_cast<BaseWindow*>(target->getParent());
		}
	}
	
	return false;

}

bool System::handleKeyboard(EventArgs::Keys key, EventArgs::ButtonState state)
{
	proceedSystemKey(key, state);

	if(m_exclusiveInputWindow)
		return m_exclusiveInputWindow->onKeyboardButton(key, state);	

	//if(!m_focusWindow)
	//	queryInputFocus(getTabstopWindow(m_rootWindow->getChildren()));

	if(m_focusWindow && m_focusWindow->getEnabled())
	{
		return m_focusWindow->onKeyboardButton(key, state);
	}
	else
		return m_rootWindow->onKeyboardButton(key, state);

	return false;
}

bool System::handleChar(unsigned int code)
{
	if(m_exclusiveInputWindow)
		return m_exclusiveInputWindow->onChar((const wchar_t*)&code);

	if(m_focusWindow)
	{
		if(code > 31) // skip ctrl codes
			return m_focusWindow->onChar((const wchar_t*)&code);
		else
			return true;
	}
	return false;
}

void System::handleViewportChange()
{
	if(!m_autoScale)
	{
		updateSize(m_render.getViewportSize());
	}
}

void System::handleFocusLost()
{
	queryCaptureInput(0);
	queryInputFocus(0);
}

bool System::proceedSystemKey(EventArgs::Keys key, EventArgs::ButtonState state)
{
	switch(key)
	{
		case EventArgs::K_TAB:
			if(m_focusWindow && state == EventArgs::Down)
			{
				if(m_sytemkeys & SHIFT)
				{
					BaseWindow* sibling = m_focusWindow->prevSibling();
					while(!sibling->isTabStop())
					{
						if(sibling == m_focusWindow)
							break;
						sibling = sibling->prevSibling();
					}
					queryInputFocus(sibling);
				}
				else
				{
					BaseWindow* sibling = m_focusWindow->nextSibling();
					while(!sibling->isTabStop())
					{
						if(sibling == m_focusWindow)
							break;
						sibling = sibling->nextSibling();
					}
					queryInputFocus(sibling);
				}
			}

			break;
		case EventArgs::K_SHIFT:
		case EventArgs::K_LSHIFT:
		case EventArgs::K_RSHIFT:
			state == EventArgs::Down ? m_sytemkeys |= SHIFT : m_sytemkeys &= ~SHIFT;
			break;
		case EventArgs::K_CONTROL:
		case EventArgs::K_LCONTROL:
		case EventArgs::K_RCONTROL:
			state == EventArgs::Down ? m_sytemkeys |= CONTROL : m_sytemkeys &= ~CONTROL;
			break;
		case EventArgs::K_MENU:
		case EventArgs::K_LMENU:
		case EventArgs::K_RMENU:
			state == EventArgs::Down ? m_sytemkeys |= MENU : m_sytemkeys &= ~MENU;
			break;
		case EventArgs::K_ESCAPE:
		case EventArgs::K_RETURN:
		default:
			return false;
			break;
	};
	return true;
}

bool System::isSysKeyPressed(unsigned int key)
{
	return (m_sytemkeys & key) > 0;
}

void System::logEvent(LogLevel level, const std::string& message)
{
	if(!m_logger.empty())
		m_logger(level, message);
}

BaseWindow* System::find(const std::string& name)
{
	if(m_rootWindow)
		return m_rootWindow->findChildWindow(name);

	return 0;
}

bool System::queryInputFocus(BaseWindow* wnd)
{
	if(m_focusWindow)
	{
		m_focusWindow->resetInputFocus();
		m_focusWindow->onFocusLost(wnd);
		m_focusWindow->invalidate();
	}
	m_focusWindow = wnd;
	
	if(m_focusWindow)
	{
		m_focusWindow->setInputFocus(false);
		m_focusWindow->onFocusGained();
		m_focusWindow->invalidate();
	}

	return true;
}

void System::queryCaptureInput(BaseWindow* wnd)
{
	if(m_captureWindow)
	{
		m_captureWindow->onCaptureLost();
	}
	m_captureWindow = wnd;

	if(wnd)
		m_captureWindow->onCaptureGained();
}

void System::EnterExclusiveInputMode(BaseWindow* wnd)
{
	if(wnd)
		m_exclusiveInputWindow = wnd;
}
void System::LeaveExclusiveInputMode()
{
	m_exclusiveInputWindow = 0;
}

BaseWindow* System::getTargetWindow(const point& pt) const
{
	return getTargetWindow(pt, m_rootWindow->getChildren()).get();
}

WindowPtr System::getTargetWindow(const point& pt, BaseWindow::ChildrenList& list) const
{
	WindowPtr ret;
	BaseWindow::ReverseChildrenIter i = list.rbegin();
	BaseWindow::ReverseChildrenIter end = list.rend();
	while(i != end)
	{
		WindowPtr p = (*i);
		if(p->hitTest(pt))
		{
			ret = getTargetWindow(pt, p->getChildren());
			if(!ret)
				ret = p;
			break;
		}
		++i;
	}
	return ret;
}

BaseWindow* System::getTabstopWindow(BaseWindow::ChildrenList& list) const
{
	BaseWindow* ret = NULL;
	BaseWindow::ReverseChildrenIter i = list.rbegin();
	BaseWindow::ReverseChildrenIter end = list.rend();
	while(i != end)
	{
		if(!(*i)->isTabStop())
		{
			ret = getTabstopWindow((*i)->getChildren());
			if(ret)
				break;
		}
		else
		{
			ret = (*i).get();
			break;
		}
		++i;
	}
	return ret;
	
}

void System::executeScript(const std::string& filename)
{
	if(!m_scriptSys.ExecuteFile(filename))
	{
		logEvent(LogError, std::string("Unable to execute Lua file: ") + m_scriptSys.GetLastError());
	}
}

void System::render()
{	
	m_render.doRender();
	if(m_bShowCursor)
		getCursor().render();
}

void System::tick(float delta)
{
	m_inTick = true;
	std::vector<BaseWindow*>::iterator i = m_tickedWnd.begin();
	std::vector<BaseWindow*>::iterator end = m_tickedWnd.end();
	std::vector<BaseWindow*>::size_type subscribeTickWndSize = m_subscribeTickWnd.size();
	while(i != end)
	{
		BaseWindow* wnd = (*i);
		if(wnd)
			wnd->onTick(delta);
		++i;
	}
	if(m_tickClear)
	{
		m_tickedWnd.erase(std::remove_if(m_tickedWnd.begin(), end, tickClear_()), end);
		m_tickClear = false;
	}

	if(subscribeTickWndSize)
	{
		std::vector<BaseWindow*>::size_type tickedWndSize = m_tickedWnd.size();
		m_tickedWnd.resize(tickedWndSize + subscribeTickWndSize);
		std::copy(m_subscribeTickWnd.begin(), m_subscribeTickWnd.end(), m_tickedWnd.begin() + tickedWndSize);
		m_subscribeTickWnd.clear();
	}

	if(m_activateTooltip)
	{
		m_tttime += delta;
		if(m_tttime >= m_ttdelay)
		{
			m_tttime = 0.f;
			m_activateTooltip = false;

			showTooltip(m_containsMouse);			
		}
	}

	if(m_tooltipWindow->getVisible() && m_ttlifetime > 1.f)
	{
		m_tttime += delta;
		if(m_tttime >= m_ttlifetime)
		{
			hideTooltip(m_containsMouse);
		}
	}

	m_inTick = false;
}

void System::draw()
{
	m_render.clearRenderList();

	m_render.beginBatching();

	getRootWindow().draw(
		point(0.f, 0.f),
		Rect(point(0.f, 0.f),
		m_render.getSize())
		);

	m_render.endBatching();

}
void System::showTooltip(BaseWindow* wnd)
{
	if(wnd && wnd->hasTooltip())
	{
		wnd->onTooltipShow();
	}
	static_cast<Tooltip*>(m_tooltipWindow.get())->show();
}
void System::hideTooltip(BaseWindow* wnd)
{
	if(wnd && wnd->hasTooltip())
	{
		wnd->onTooltipHide();
	}
	static_cast<Tooltip*>(m_tooltipWindow.get())->reset();
}

bool System::startDrag(BaseWindow* wnd, point offset)
{
	if(!wnd) return false;

	m_dragContainer->rise();
	DragContainer* dc = getDragContainer();
	if(wnd->isDragable() && dc->startDrag(wnd, offset))
	{
		point pt = m_cursor.getPosition() - offset;
		dc->setPosition(pt);
		m_dragfired = true;
		BaseWindow* mouseWnd = getTargetWindow(m_cursor.getPosition());
		dc->update(mouseWnd, pt);
		return true;
	}
	return false;
}

bool System::stopDrag(void)
{
	if (m_dragfreeze)
		return false;

	if(m_dragfired)
	{
		DragContainer* dc = getDragContainer();
		m_dragfired = !dc->stopDrag();
	}

	if (!m_dragfired)
	{
		m_dragfreeze = false;
		m_dragWindow = 0;
		m_dragging = false;
	}

	return !m_dragfired;
}

void System::subscribeTick(BaseWindow* wnd)
{
	if(!wnd) return;

	std::vector<BaseWindow*>::iterator i = std::find(m_tickedWnd.begin(), m_tickedWnd.end(), wnd);
	if(i != m_tickedWnd.end())
		return;
	
	if(m_inTick)
	{
		std::vector<BaseWindow*>::iterator i = std::find(m_subscribeTickWnd.begin(), m_subscribeTickWnd.end(), wnd);
		if(i != m_subscribeTickWnd.end())
			return;
		m_subscribeTickWnd.push_back(wnd);
	}
	else
	{	
		m_tickedWnd.push_back(wnd);
	}
}

void System::unsubscribeTick(BaseWindow* wnd)
{
	if(m_inTick) return;
	if(!wnd) return;

	std::vector<BaseWindow*>::iterator i = std::find(m_tickedWnd.begin(), m_tickedWnd.end(), wnd);
	if(i != m_tickedWnd.end())
		m_tickedWnd.erase(i);
}

void System::setAutoScale(bool status)
{
	m_autoScale = status;
	m_render.setAutoScale(status);
	m_rootWindow->setSize(m_render.getSize());
	m_rootWindow->onSized(true);
}

void System::updateSize(Size& sz)
{
	if(m_rootWindow)
	{
		m_rootWindow->setSize(sz);
		m_rootWindow->onSized(false);
	}
}

bool System::isMouseInGui() const
{
	if(m_rootWindow && m_containsMouse && m_containsMouse != m_rootWindow)
		return true;
	return false;
}

bool System::isMouseInGui(float x, float y) const
{
	if(m_exclusiveInputWindow)
		return true;

	const point pt(x, y);
	if(m_rootWindow)
	{
		BaseWindow* mouseWnd = getTargetWindow(pt);
		if(mouseWnd != m_rootWindow)
		{
			return true;
		}
	}
	return false;
}

}
