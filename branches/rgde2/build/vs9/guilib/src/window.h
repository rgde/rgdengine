#pragma once

#include "rect.h"
#include "color.h"
#include "vector.h"
#include "input.h"
#include "events.h"
#include "eventtypes.h"
#include "scripthost.h"
#include "renderCallback.h"
#include "utils.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace xml 
{
class node;
};

namespace gui
{

template<class T>
struct type_name{ static const char* get(); };

class System;
class ScriptSystem;
class ScriptObject;

class  BaseWindow :
	public NamedObject,
	public TreeNode<BaseWindow>,
	public RefCounted,
	public ScriptObject,
	public events::Sender
{
public:
	typedef BaseWindow Self;
	BaseWindow(System& sys, const std::string& name = std::string());
	virtual ~BaseWindow();

	static const char* GetType() { return "BaseWindow"; }
	virtual const char* getType() { return Self::GetType(); }

	void suspendLayout() { m_suspended = true; onSuspendLayout(); }
	void resumeLayout() { m_suspended = false; onResumeLayout(); }

	void setArea(const Rect& rc);
	const Rect& getArea(void) const { return m_area; }

	void setPosition(const point& pt);
	const point getPosition(void) const { return m_area.getPosition(); }
	void setSize(const Size& sz);
	const Size getSize(void) const { return m_area.getSize(); }

	void setVisible(bool visible);
	bool getVisible() const { return m_visible; }

	void setEnabled(bool enable) { m_enabled = enable; }
	bool getEnabled() const { return m_enabled; }

	void setBackColor(Color cl)
	{
		m_backColor = cl;
		invalidate();
	}
	Color getBackColor() const { return m_backColor; }
	void setForeColor(Color cl)
	{
		m_foreColor = cl;
		invalidate();
	}
	Color getForeColor() const { return m_foreColor; }

	void setUserData(size_t ptr) { m_userData = ptr; }
	size_t getUserData() const { return m_userData; }

	void moveToFront(BaseWindow* child);
	void bringToBack(BaseWindow* child);
	void moveToFront();
	void bringToBack();
	virtual void rise();

	void setAlwaysOnTop(bool status) { m_alwaysOnTop = status; if(status) moveToFront(); }
	bool getAlwaysOnTop() const { return m_alwaysOnTop; }

	virtual bool isCanHaveChildren(void) const { return true; }
	BaseWindow* findChildWindow(const std::string& name);
	BaseWindow const* getParent() const { return m_parent; }
	void addChildWindow(BaseWindow* wnd);

	void setInputFocus(bool query);
	void resetInputFocus() { m_focus = false; }
	bool hasInputFocus() const { return m_focus; }
	bool isTabStop() const { return m_tabstop; }

	bool hitTest(const point& pt);

	bool isDragable() const { return m_dragable; }
	void setDragable(bool drag) { m_dragable = drag;}
	bool isAcceptDrop() const { return m_acceptDrop; }
	void setAcceptDrop(bool drop) { m_acceptDrop = drop;}

	void setIgnoreInputEvents(bool ignore) { m_ignoreInputEvents = ignore;}
	bool getIgnoreInputEvents() const { return m_ignoreInputEvents; }

	virtual void invalidate();
	virtual void render(const Rect& dest, const Rect& clip) {clip;dest;}

	void draw(const point& offset, const Rect& clip);

	// events
	virtual bool onMouseEnter(void);
	virtual bool onMouseLeave(void);
	virtual bool onMouseMove(void);
	virtual bool onMouseWheel(int delta);
	virtual bool onMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state);
	virtual bool onMouseDouble(EventArgs::MouseButtons btn);
	virtual bool onChar(const wchar_t* text);
	virtual bool onKeyboardButton(EventArgs::Keys key, EventArgs::ButtonState state);
	virtual bool onCaptureGained(void);
	virtual bool onCaptureLost(void);
	virtual bool onLoad(void);
	virtual bool onSized(bool update = true);
	virtual bool onMoved(void);
	virtual bool onTick(float delta);
	virtual bool onTooltipShow();
	virtual bool onTooltipHide();
	virtual bool onSuspendLayout();
	virtual bool onResumeLayout();
	virtual bool onFocusGained();
	virtual bool onFocusLost(BaseWindow* newFocus);

	// loading from XML
	virtual void init(xml::node& node);
	virtual void parseEventHandlers(xml::node& node);

	BaseWindow* nextSibling();
	BaseWindow* prevSibling();

	point transformToWndCoord(const point& global); // translate to parent coords!
	point transformToRootCoord(const point& local);

	void subscribeNamedEvent(std::string name, BaseWindow* sender, std::string script);
	void unsubscribeNamedEvent(std::string name, BaseWindow* sender);
	void sendNamedEvent(std::string name);
	void onNamedEvent(events::NamedEvent& e);

	void addScriptEventHandler(std::string name, std::string handler);
	void callHandler(EventArgs* arg);

	void startTick(void);
	void stopTick(void);
	bool isUnsubscribePending() { if(m_unsubscribePending) {m_unsubscribePending = false; return true;} return false; }

	bool hasTooltip(void) const { return m_tooltip; }
	void enableTooltip(bool b) { m_tooltip = b; }

	std::string getEventScript(const std::string& ev);
	virtual bool onGameEvent(const std::string& ev);

	bool isChildrenOf(const BaseWindow* wnd);

	
	void setAfterRenderCallback(AfterRenderCallbackFunc func)
	{
		m_afterRenderCallback = func;
	}

protected:

	void ExecuteScript(const std::string& env, const std::string& script);
	void thisset();
	BaseWindow& operator=(const BaseWindow&) { return *this; }

	void Align();
	void Stick();

	void CallAfterRenderCallback(const Rect& dest, const Rect& clip);

protected:
	bool			m_suspended;

	Rect			m_area;
	Rect			m_alignmentRect;
	unsigned int	m_alignment;
	Rect			m_stickRect;
	unsigned int	m_stick;

	bool			m_visible;
	bool			m_enabled;
	Color			m_backColor;
	Color			m_foreColor;

	size_t			m_userData;
	
	bool			m_alwaysOnTop;

	bool			m_focus;
	bool			m_ignoreInputEvents;
	bool			m_tabstop;

	bool			m_dragable;
	bool			m_acceptDrop;

	bool			m_tooltip;

	bool			m_unsubscribePending;

	bool			m_customDraw;
	bool			m_invalidated;

	std::string		m_drawhandler;

	System&			m_system;
	AfterRenderCallbackFunc m_afterRenderCallback;

	typedef std::pair<std::string, BaseWindow*> NamedEventEntry;
	typedef boost::unordered_map<NamedEventEntry, std::string> NamedEventsMap;
	NamedEventsMap m_scriptevents;
	
	typedef boost::unordered_map<std::string, std::string> HandlerMap;
	HandlerMap		m_handlers;

	struct seeker_{
		const BaseWindow* m_ptr;
		seeker_& operator=(const seeker_&) {}
		seeker_(const BaseWindow* ptr) : m_ptr(ptr){}
		bool operator()(PNode obj) 
		{
			return obj ? (obj.get() == m_ptr) : false;
		}
	};
	struct topmost_{
		bool operator()(PNode obj) 
		{
			return obj->getAlwaysOnTop();
		}
	};
	struct ntopmost_{
		bool operator()(PNode obj) 
		{
			return !obj->getAlwaysOnTop();
		}
	};
};

typedef boost::intrusive_ptr<BaseWindow> WindowPtr;

template <typename T> struct window_caster
{
	static T* apply (BaseWindow* w)
	{
		return dynamic_cast <T*> (w);
	}
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
