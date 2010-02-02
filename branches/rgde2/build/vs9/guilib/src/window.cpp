#include "StdAfx.h"
#include "window.h"

#include "utils.h"
#include "imageset.h"
#include "system.h"
#include "renderer.h"

namespace gui
{

BaseWindow::BaseWindow(System& sys, const std::string& name) : 
	NamedObject(name),
	ScriptObject(sys.getScriptSystem()),
	m_system(sys),
	m_visible(false),
	m_enabled(true),
	m_area(0.f, 0.f, 1.f, 1.f),
	m_userData(0),
	m_focus(false),
	m_tabstop(false),
	m_ignoreInputEvents(false),
	m_alwaysOnTop(false),
	m_dragable(false),
	m_acceptDrop(false),
	m_customDraw(false),
	m_tooltip(false),
	m_unsubscribePending(false),
	m_alignmentRect(0.f, 0.f, 0.f, 0.f),
	m_invalidated(true),
	m_alignment(0),
	m_stick(0),
	m_stickRect(0.f, 0.f, 0.f, 0.f),
	m_afterRenderCallback(NULL),
	m_suspended(false)
{
	m_backColor = Color(1.f, 1.f, 1.f);
	m_foreColor = Color(0.f, 0.f, 0.f);
}

BaseWindow::~BaseWindow()
{
	m_system.getRenderer().clearCache(this);
}

void BaseWindow::setArea(const Rect& rc) 
{ 
	m_area = rc;
	onMoved();
	onSized();
}

void BaseWindow::setPosition(const point& pt) 
{
	if(pt != m_area.getPosition())
	{
		invalidate();
		m_area.setPosition(pt);
		onMoved();
		send_event(events::MovedEvent());
	}
}

void BaseWindow::setSize(const Size& sz) 
{
	if(sz != m_area.getSize())
	{
		invalidate();
		m_area.setSize(sz);
		onSized();
		send_event(events::SizedEvent());
	}
}

void BaseWindow::invalidate()	
{
	//m_system.getRenderer().clearCache(this);

	m_invalidated = true;	
	ChildrenIter i = m_children.begin();
	ChildrenIter end = m_children.end();
	while(i != end)
	{
		(*i)->invalidate();
		++i;
	}
}

void BaseWindow::setVisible(bool visible) 
{
	EventArgs a;
	
	if(visible)
	{
		a.name = "On_Show";
		callHandler(&a);
	}
	else
	{
		a.name = "On_Hide";
		callHandler(&a);
	}

	m_visible = visible; 
}

void BaseWindow::setInputFocus(bool query)
{
	if(query)
		m_focus = m_system.queryInputFocus(this);
	else
		m_focus = true;
}

bool BaseWindow::hitTest(const point& pt)
{
	if(!m_visible)
		return false;

	if(!m_ignoreInputEvents)
		return m_area.isPointInRect(transformToWndCoord(pt));
	
	return false;
}

void BaseWindow::rise()
{
	if(m_parent)
	{
		m_parent->rise();
	}
}

void BaseWindow::moveToFront(BaseWindow* child)
{
	if(m_children.size() <= 1)
		return;

	ChildrenIter it = std::find_if(m_children.begin(), m_children.end(), seeker_(child));
	if(it != m_children.end())
	{
		if(child->getAlwaysOnTop())
		{
			m_children.splice(m_children.end(), m_children, it);
		}
		else
		{
			ChildrenIter topmost = std::find_if(m_children.begin(), m_children.end(), topmost_());
			m_children.splice(topmost, m_children, it);
		}
	}
	
		
}

void BaseWindow::bringToBack(BaseWindow* child)
{
	if(m_children.size() <= 1)
		return;

	ChildrenIter it = std::find_if(m_children.begin(), m_children.end(), seeker_(child));
	if(it != m_children.end())
	{
		if(child->getAlwaysOnTop())
		{
			ChildrenIter topmost = std::find_if(m_children.begin(), m_children.end(), ntopmost_());
			m_children.splice(topmost, m_children, it);
		}
		else
		{
			m_children.splice(m_children.begin(), m_children, it);
		}
	}
}

void BaseWindow::moveToFront()
{
	if(m_parent)
		m_parent->moveToFront(this);	
}

void BaseWindow::bringToBack()
{
	if(m_parent)
		m_parent->bringToBack(this);
}


BaseWindow* BaseWindow::findChildWindow(const std::string& name)
{
	WindowPtr p = findNode(name);
	return p.get();
}

void BaseWindow::addChildWindow(BaseWindow* wnd)
{
	if(wnd) addChild(wnd);
}

void BaseWindow::callHandler(EventArgs* arg)
{
	if(!arg)
		return;

	HandlerMap::iterator it = m_handlers.find(arg->name);
	if(it != m_handlers.end())
	{
		std::string& handler = it->second;
		if(!handler.empty())
		{
			luabind::globals (m_ref_script.LuaState())["eventArgs"] = arg;
			ExecuteScript(arg->name, handler);
			luabind::globals (m_ref_script.LuaState())["eventArgs"] = 0;
		}
	}
}

void BaseWindow::ExecuteScript(const std::string& env, const std::string& script)
{
	if(!m_ref_script.ExecuteString(script, this, env))
	{
		m_system.logEvent(LogError, std::string("Unable to execute Lua handler '")+ env + std::string("' in object ") + getName());
		m_system.logEvent(LogError, m_ref_script.GetLastError());
	}
}


bool BaseWindow::onMouseEnter(void)
{
	MouseEventArgs m;
	m.name = "On_MouseEnter";	
	callHandler(&m);
	return m.handled; 
}
bool BaseWindow::onMouseLeave(void)
{
	MouseEventArgs m;
	m.name = "On_MouseLeave";
	callHandler(&m);
	return m.handled; 
}
bool BaseWindow::onMouseMove(void)
{
	MouseEventArgs m;
	m.name = "On_MouseMove";
	point pt = m_system.getCursor().getPosition();
	m.mouseX = pt.x;
	m.mouseY = pt.y;
	callHandler(&m);
	return m.handled; 
}
bool BaseWindow::onMouseWheel(int delta)
{
	MouseEventArgs m;
	m.name = "On_MouseWheel";
	m.wheelDelta = delta;
	callHandler(&m);
	return m.handled; 
}
bool BaseWindow::onMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state)
{
	MouseEventArgs m;
	m.name = "On_MouseButton";
	m.mbutton = btn;
	m.mstate = state;
	callHandler(&m);
	return m.handled; 
}
bool BaseWindow::onMouseDouble(EventArgs::MouseButtons btn)
{
	MouseEventArgs m;
	m.name = "On_MouseDouble";
	m.mbutton = btn;
	callHandler(&m);
	return m.handled;
}
bool BaseWindow::onChar(const wchar_t* text)
{
	KeyEventArgs k;
	k.name = "On_Char";
	k.ch = text;
	callHandler(&k);
	return k.handled; 
}
bool BaseWindow::onKeyboardButton(EventArgs::Keys key, EventArgs::ButtonState state)
{
	KeyEventArgs k;
	k.name = "On_KeyboardButton";
	k.button = key;
	k.state = state;
	callHandler(&k);
	return k.handled; 
}
bool BaseWindow::onCaptureGained(void)
{
	EventArgs a;
	a.name = "On_CaptureGained";
	callHandler(&a);
	return a.handled; 
}
bool BaseWindow::onCaptureLost(void)
{
	EventArgs a;
	a.name = "On_CaptureLost";
	callHandler(&a);
	return a.handled; 
}

bool BaseWindow::onSuspendLayout(void)
{
	EventArgs a;
	a.name = "On_SuspendLayout";
	callHandler(&a);
	return a.handled; 
}
bool BaseWindow::onResumeLayout(void)
{
	EventArgs a;
	a.name = "On_ResumeLayout";
	callHandler(&a);
	return a.handled; 
}

bool BaseWindow::onFocusGained(void)
{
	EventArgs a;
	a.name = "On_FocusGained";
	callHandler(&a);
	return a.handled; 
}
bool BaseWindow::onFocusLost(BaseWindow* newFocus)
{
	EventArgs a;
	a.name = "On_FocusLost";
	callHandler(&a);
	return a.handled; 
}

bool BaseWindow::onLoad(void)
{
	onMoved();
	if(m_alwaysOnTop)
		moveToFront();

	HandlerMap::iterator it = m_handlers.find("On_Draw");
	if(it != m_handlers.end())
	{
		m_drawhandler = it->second;
		if(!m_drawhandler.empty())
		{
			m_customDraw = true;
		}
	}

	EventArgs a;
	a.name = "On_Load";
	callHandler(&a);
	return a.handled; 
}

bool BaseWindow::onSized(bool update)
{
	if(update)
	{
		Align();
		if(m_alignment)
			onMoved();
	}

	ChildrenIter i = m_children.begin();
	ChildrenIter end = m_children.end();
	while(i != end)
	{
		(*i)->onSized();
		++i;
	}
	EventArgs a;
	a.name = "On_Sized";
	callHandler(&a);

	return true;
}

void BaseWindow::Align()
{
	if(!m_parent)
		return;
	if(!m_alignment)
		return;

	point pos = m_area.getPosition();
	Size sz = m_area.getSize();
	Size parent_size(m_parent->getSize());

	if(m_alignment & HCenter)
	{
		float original_width = m_alignmentRect.m_left + m_alignmentRect.m_right;
		float x = 0.f;
		if(original_width > 0.f)
		{
			float width = parent_size.width - sz.width;
			x = width * m_alignmentRect.m_left / original_width;
		}
		m_area.setPosition(point(x, pos.y));
	}
	else
	{
		if(m_alignment & Left)
		{
			m_area.setPosition(point(m_alignmentRect.m_left, pos.y));
		}
		else if(m_alignment & Right)
		{
			m_area.setPosition(point(parent_size.width - sz.width - m_alignmentRect.m_right, pos.y));
		}
	}

	pos = m_area.getPosition();

	if(m_alignment & VCenter)
	{
		float original_height = m_alignmentRect.m_top + m_alignmentRect.m_bottom;
		float y = 0.f;
		if(original_height > 0.f)
		{
			float height = parent_size.height - sz.height;
			y = height * m_alignmentRect.m_top / original_height;
		}
		m_area.setPosition(point(pos.x, y));
	}
	else
	{
		if(m_alignment & Top)
		{
			m_area.setPosition(point(pos.x, m_alignmentRect.m_top));
		}
		else if(m_alignment & Bottom)
		{
			m_area.setPosition(point(pos.x, parent_size.height - sz.height - m_alignmentRect.m_bottom));
		}
	}
}

void BaseWindow::Stick()
{
	if(!m_parent)
		return;
	if(!m_stick)
		return;

	Size sz = m_area.getSize();
	Size parent_size(m_parent->getSize());

	if(m_stick & HCenter)
	{
		m_area.setPosition(point((parent_size.width - sz.width) / 2, m_area.m_top));
	}
	else
	{
		if(m_stick & Left)
		{
			m_area.setPosition(point(m_stickRect.m_left, m_area.m_top));
		}
		else if(m_stick & Right)
		{
			m_area.setPosition(point(parent_size.width - sz.width - m_stickRect.m_right, m_area.m_top));
		}
	}

	if(m_stick & VCenter)
	{
		m_area.setPosition(point(m_area.m_left, (parent_size.height - sz.height) / 2));
	}
	else
	{
		if(m_stick & Top)
		{
			m_area.setPosition(point(m_area.m_left, m_stickRect.m_top));
		}
		else if(m_stick & Bottom)
		{
			m_area.setPosition(point(m_area.m_left, parent_size.height - sz.height - m_stickRect.m_bottom));
		}
	}
}

bool BaseWindow::onMoved(void)
{
	if(m_parent)
	{
		Stick();
		const Rect& pr = m_parent->getArea();
		Rect area(m_area);
		area.offset(pr.getPosition());

		m_alignmentRect = Rect(
			area.m_left - pr.m_left,
			area.m_top - pr.m_top,
			pr.m_right - area.m_right,
			pr.m_bottom - area.m_bottom);
	}
	invalidate();
	EventArgs a;
	a.name = "On_Moved";
	callHandler(&a);
	return true;
}

bool BaseWindow::onTick(float delta)
{
	TickEventArgs a;
	a.name = "On_Tick";
	a.delta = delta;
	callHandler(&a);
	return a.handled;
}

bool BaseWindow::onTooltipShow()
{
	EventArgs a;
	a.name = "On_TooltipShow";
	callHandler(&a);
	return a.handled;
}

bool BaseWindow::onTooltipHide()
{
	EventArgs a;
	a.name = "On_TooltipHide";
	callHandler(&a);
	return a.handled;
}

void BaseWindow::startTick(void)
{
	m_unsubscribePending = false;
	m_system.subscribeTick(this);
}

void BaseWindow::stopTick(void)
{
	m_unsubscribePending = true;
	m_system.setTickClear();
}

void BaseWindow::init(xml::node& node)
{	
	xml::node setting = node("Visible");
	if(!setting.empty())
	{
		m_visible = StringToBool(setting.first_child().value());
	}
	setting = node("Align");
	if(!setting.empty())
	{
		m_alignment = StringToAlignment(setting.first_child().value());
	}
	setting = node("Stick");
	if(!setting.empty())
	{
		m_stick = StringToAlignment(setting.first_child().value());
	}
	setting = node("StickRect");
	if(!setting.empty())
	{
		m_stickRect = StringToRect(setting.first_child().value());
	}
	setting = node("Area");
	if(!setting.empty())
	{
		m_area = StringToArea(setting.first_child().value());
	}
	setting = node("Backcolor");
	if(!setting.empty())
	{
		m_backColor = StringToColor(setting.first_child().value());
	}
	setting = node("Forecolor");
	if(!setting.empty())
	{
		m_foreColor = StringToColor(setting.first_child().value());
	}
	setting = node("TabStop");
	if(!setting.empty())
	{
		m_tabstop = StringToBool(setting.first_child().value());
	}
	setting = node("Dragable");
	if(!setting.empty())
	{
		m_dragable = StringToBool(setting.first_child().value());
	}
	setting = node("AcceptDrop");
	if(!setting.empty())
	{
		m_acceptDrop = StringToBool(setting.first_child().value());
	}
	setting = node("AlwaysOnTop");
	if(!setting.empty())
	{
		m_alwaysOnTop = StringToBool(setting.first_child().value());
	}
	setting = node("Tooltip");
	if(!setting.empty())
	{
		m_tooltip = StringToBool(setting.first_child().value());
	}
	setting = node("Size");
	if(!setting.empty())
	{
		setSize(StringToSize(setting.first_child().value()));
	}
	setting = node("Pos");
	if(!setting.empty())
	{
		setPosition(StringToPoint(setting.first_child().value()));
	}
	setting = node("IgnoreInputEvents");
	if(!setting.empty())
	{
		setIgnoreInputEvents(StringToBool(setting.first_child().value()));
	}
	onMoved();
}

void BaseWindow::parseEventHandlers(xml::node& node)
{
	if(!node.empty())
	{
		xml::node handler = node.first_child();
		while(!handler.empty())
		{
			std::string name(handler.name());
			m_handlers[name] = handler.first_child().value();
			handler = handler.next_sibling();
		}
	}
}

void BaseWindow::addScriptEventHandler(std::string name, std::string handler)
{
	if(!name.empty() && !handler.empty())
	{
		m_handlers[name] = handler;
	}
}

void BaseWindow::CallAfterRenderCallback(const Rect& dest, const Rect& clip)
{
	if (m_afterRenderCallback)
	{
		Renderer& r = m_system.getRenderer();
		r.addCallback(m_afterRenderCallback, this, dest, clip);
	}
}

void BaseWindow::draw(const point& offset, const Rect& clip)
{
	if(m_visible)
	{
		if(m_area.getWidth() < 1.f)
			return;

		Rect destrect(m_area);
		destrect.offset(offset);
		Rect cliprect(destrect);
		cliprect = cliprect.getIntersection(clip);

		if (m_invalidated)
		{


			m_system.getRenderer().startCaptureForCache(this);			
			if(m_customDraw && !m_drawhandler.empty())
			{
				EventArgs a;
				a.name = "On_Draw";
				luabind::globals (m_ref_script.LuaState())["eventArgs"] = &a;
				ExecuteScript(a.name, m_drawhandler);
				luabind::globals (m_ref_script.LuaState())["eventArgs"] = 0;
			}

			render(destrect, cliprect); // render self first
			m_system.getRenderer().endCaptureForCache(this);		
			m_invalidated = false;
		}
		else 
		{
			//if (!m_system.getRenderer().isExistInCache(this))
			//{
			//	m_system.getRenderer().startCaptureForCache(this);			
			//	if(m_customDraw && !m_drawhandler.empty())
			//	{
			//		EventArgs a;
			//		a.name = "On_Draw";
			//		luabind::globals (m_ref_script.LuaState())["eventArgs"] = &a;
			//		ExecuteScript(a.name, m_drawhandler);
			//		luabind::globals (m_ref_script.LuaState())["eventArgs"] = 0;
			//	}

			//	render(destrect, cliprect); // render self first
			//	m_system.getRenderer().endCaptureForCache(this);
			//}
			//else 
				m_system.getRenderer().drawFromCache(this);
		}

		ChildrenIter i = m_children.begin();
		ChildrenIter end = m_children.end();
		while(i != end)
		{
			(*i)->draw(destrect.getPosition(), cliprect);
			++i;
		}

		// теперь скажем, что тут коллбак при отрисовке нужно сделать
		CallAfterRenderCallback(destrect,cliprect);
	}	
}

point BaseWindow::transformToWndCoord(const point& global)
{
	BaseWindow* parent = m_parent;
	point out(global);
	if(parent)
	{
		out -= parent->getArea().getPosition();
		out = parent->transformToWndCoord(out);
	}
	return out;
}

point BaseWindow::transformToRootCoord(const point& local)
{
	BaseWindow* parent = m_parent;
	point out(local);
	if(parent)
	{
		out += parent->getArea().getPosition();
		out = parent->transformToRootCoord(out);
	}
	return out;
}

BaseWindow* BaseWindow::nextSibling()
{
	if(m_parent)
	{
		ChildrenList& list = m_parent->getChildren();
		if(list.size() <= 1)
			return this;
		ChildrenIter it = std::find_if(list.begin(), list.end(), seeker_(this));
		if(it == list.end())
		{
			assert(false && "Link to parent is invalid!");
		}
		return (++it != list.end()) ? (*it).get() : (*list.begin()).get();
	}
	else
	{
		return this;
	}
}

BaseWindow* BaseWindow::prevSibling()
{
	if(m_parent)
	{
		ChildrenList& list = m_parent->getChildren();
		if(list.size() <= 1)
			return this;
		ChildrenIter it = std::find_if(list.begin(), list.end(), seeker_(this));
		if(it == list.end())
		{
			assert(false && "Link to parent is invalid!");
		}
		if(it == list.begin())
			return (*list.rbegin()).get();
		--it;
		return (*it).get();
	}
	else
	{
		return this;
	}

}

void BaseWindow::thisset()
{
	if(m_ref_script.LuaState())
		luabind::globals(m_ref_script.LuaState())["this"] = this;
}

void BaseWindow::subscribeNamedEvent(std::string name, BaseWindow* sender, std::string script)
{
	if(script.empty())
		return; //nothing to do!
	
	if(!name.empty())
	{
		NamedEventEntry entry = std::make_pair<std::string, BaseWindow*>(name, sender);
		
		m_scriptevents.insert(std::make_pair(entry,script));

		// support for a script events
		subscribe<events::NamedEvent, BaseWindow> (&BaseWindow::onNamedEvent, sender);
		
	}
}

void BaseWindow::unsubscribeNamedEvent(std::string name, BaseWindow* sender)
{
	if(!name.empty())
	{
		NamedEventEntry entry = std::make_pair<std::string, BaseWindow*>(name, sender);
		NamedEventsMap::iterator it = m_scriptevents.find(entry);
		if(it != m_scriptevents.end())
		{			
			m_scriptevents.erase(it);
			if(sender != 0)
				unsubscribe<events::NamedEvent>(sender);

			if(m_scriptevents.size() == 0)
				unsubscribe<events::NamedEvent>();
		}
	}
}

void BaseWindow::sendNamedEvent(std::string name)
{
	if(!name.empty())
		send_event(events::NamedEvent(name, this));
}

void BaseWindow::onNamedEvent(events::NamedEvent& e)
{
	NamedEventEntry entry = std::make_pair<std::string, BaseWindow*>(e.m_name, e.m_sender);
	NamedEventsMap::iterator it = m_scriptevents.find(entry);
	if(it != m_scriptevents.end())
	{
		std::string& script = it->second;
		if(!script.empty())
		{
			EventArgs arg;
			arg.name = "On_ScriptEvent";
			luabind::globals (m_ref_script.LuaState())["eventArgs"] = &arg;
			ExecuteScript(arg.name, script);
			luabind::globals (m_ref_script.LuaState())["eventArgs"] = 0;			
		}
	}
}

std::string BaseWindow::getEventScript(const std::string& ev)
{
	HandlerMap::iterator it = m_handlers.find(ev);
	if(it != m_handlers.end())
	{
		std::string& handler = it->second;
		if(!handler.empty())
		{
			return std::string(handler);
		}
	}
	return "";
}

bool BaseWindow::onGameEvent(const std::string& ev)
{
	HandlerMap::iterator it = m_handlers.find(ev);
	if(it != m_handlers.end())
	{
		std::string& handler = it->second;
		if(!handler.empty())
		{
			EventArgs arg;
			arg.name = ev.c_str();
			luabind::globals (m_ref_script.LuaState())["gameEventArgs"] = &arg;
			ExecuteScript(ev, handler);
			luabind::globals (m_ref_script.LuaState())["gameEventArgs"] = 0;
			return arg.handled;
		}
	}
	return false;
}

bool BaseWindow::isChildrenOf(const BaseWindow* wnd)
{
	const BaseWindow* parent = m_parent;

	while(parent)
	{
		if(parent == wnd)
			return true;
		parent = parent->getParent();
	}
	return false;
}
}
