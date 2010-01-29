#pragma once

#include "cursor.h"
#include "input.h"
#include "window.h"
#include "windowmanager.h"
#include "log.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

struct lua_State;

namespace gui
{

class BaseWindow;
class Renderer;
class LogHelper;
class RenderHelper;
class DragContainer;
class Tooltip;
class Menu;

#define SHIFT		1
#define RSHIFT		2
#define CONTROL		4
#define RCONTROL	8
#define MENU		16
#define RMENU		32

class LogHelper
{
public:
	LogHelper(LoggerCallback callback)
		: m_callback(callback)
	{}

	void warning(const std::string& text) { if(!m_callback.empty()) m_callback(LogWarning, text); }
	void message(const std::string& text) { if(!m_callback.empty()) m_callback(LogMessage, text); }
	void error(const std::string& text) { if(!m_callback.empty()) m_callback(LogError, text); }
	void crit_error(const std::string& text) { if(!m_callback.empty()) m_callback(LogCritical, text); }

private:
	LoggerCallback m_callback;
};

class  System
{
public:
	System(Renderer& render, const std::string& resourceDir, const std::string& scheme, LoggerCallback callback);
	~System(void);

	void reset();
	void reset(bool complete);

	// public interface
	bool handleMouseMove(int x, int y);
	bool handleMouseWheel(int diff);
	bool handleMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state);
	bool handleMouseDouble(EventArgs::MouseButtons btn);
	bool handleChar(unsigned int code);
	bool handleKeyboard(EventArgs::Keys key, EventArgs::ButtonState state);
	void handleFocusLost(void);

	void handleViewportChange();

	bool isSysKeyPressed(unsigned int key);
	size_t GetSystemKeys() { return m_sytemkeys; }

	bool isInputCaptured() { return m_captureWindow != 0; }

	Cursor& getCursor() { return m_cursor; }
	const Cursor& getCursorConst() const { return m_cursor; }
	void setShowCursor(bool status) { m_bShowCursor = status; }
	
	void setLogger(LoggerCallback callback) { m_logger = callback; }
	void logEvent(LogLevel level, const std::string& message);

	BaseWindow* find(const std::string& name);
	
	void render();

	void init();

	void tick(float delta);
	void subscribeTick(BaseWindow* wnd);
	void unsubscribeTick(BaseWindow* wnd);
	void setTickClear() { m_tickClear = true; }

	void draw();

	BaseWindow&		getRootWindow() const { return *(m_rootWindow.get()); }
	WindowPtr		getRootPtr();

	BaseWindow*		getWindowWithFocus() const { return m_focusWindow; }
	BaseWindow*		getCaptureWindow() const { return m_captureWindow; }
	bool			queryInputFocus(BaseWindow* wnd);
	void			queryCaptureInput(BaseWindow* wnd);

	void			EnterExclusiveInputMode(BaseWindow* wnd);
	void			LeaveExclusiveInputMode();

	Tooltip*		getTooltip() const;
	void			setTooltipDelay(float f)	{ m_ttdelay = f; }
	void			setTooltipLifetime(float f)	{ m_ttlifetime = f; }
	float			getTooltipDelay() const		{ return m_ttdelay; }
	float			getTooltipLifetime() const	{ return m_ttlifetime; }

	void			showTooltip(BaseWindow* wnd);
	void			hideTooltip(BaseWindow* wnd);

	DragContainer*	getDragContainer() const;
	bool			startDrag(BaseWindow* wnd, point offset);
	bool			stopDrag(void);
	void			freezeDrag() { m_dragfreeze = true; }
	void			unfreezeDrag() { m_dragfreeze = false; }
	bool			isDragFrozen() const { return m_dragfreeze; }

	Menu*			getMenu(void) const;
	void			setContextMenuState(bool s) { m_isContextMenu = s; }

	Renderer&		getRenderer() { return m_render; }
	WindowManager&	getWindowManager() { return *m_windowMgr; }

	void setAutoScale(bool status);

	void			executeScript(const std::string& filename);	
	
	BaseWindow*		createWindow(BaseWindow* parent, const std::string& name, const std::string& type_or_path);
	BaseWindow*		loadXml(BaseWindow& parent, const std::string& filename);
	BaseWindow*		loadXml(const std::string& filename);
	BaseWindow*		getTargetWindow(const point& pt) const;

	ScriptSystem&	getScriptSystem() { return m_scriptSys; }

	bool			isMouseInGui() const;
	bool			isMouseInGui(float x, float y) const;

protected:
	WindowPtr	getTargetWindow(const point& pt, BaseWindow::ChildrenList& list) const;
	BaseWindow* getTabstopWindow(BaseWindow::ChildrenList& list) const;
	bool		proceedSystemKey(EventArgs::Keys key, EventArgs::ButtonState state);
	WindowPtr	loadXml_(const std::string& filename);

	void		makeLuaBinding(void);
	void		updateSize(Size& sz);

protected:
	Renderer&		m_render;
	boost::scoped_ptr<WindowManager>	m_windowMgr;
	boost::scoped_ptr<LogHelper>		m_logHelper;
	boost::scoped_ptr<RenderHelper>		m_renderHelper;

	ScriptSystem	m_scriptSys;

	bool m_autoScale;

	Cursor		m_cursor;
	bool		m_bShowCursor;
	BaseWindow*		m_exclusiveInputWindow;

	WindowPtr		m_rootWindow;
	BaseWindow*		m_focusWindow;
	BaseWindow*		m_captureWindow;
	BaseWindow*		m_containsMouse;
	WindowPtr		m_dragContainer;
	WindowPtr		m_tooltipWindow;
	WindowPtr		m_menuWindow;

	bool			m_isContextMenu;
	
	float			m_ttdelay;
	float			m_ttlifetime;
	float			m_tttime;
	bool			m_activateTooltip;

	bool	m_dragging;
	bool	m_dragfreeze;
	bool	m_dragfired;
	point	m_dragOffset;
	float	m_dragThreshold;
	BaseWindow*	m_dragWindow;

	size_t			m_sytemkeys;

	LoggerCallback	m_logger;

	std::vector<BaseWindow*>	m_tickedWnd;
	std::vector<BaseWindow*>	m_subscribeTickWnd;
	bool						m_inTick;
	bool						m_tickClear;
	struct tickClear_{
		bool operator()(BaseWindow* obj) 
		{
			return obj->isUnsubscribePending();
		}
	};
};


}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
