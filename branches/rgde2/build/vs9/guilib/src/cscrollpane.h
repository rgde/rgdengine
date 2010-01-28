
#pragma once

#include "window.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace gui
{

class ScrollBar;
class ScrollPane;

template<>
struct type_name<ScrollPane>{
	static const char* get() {return "ScrollPane";} };

class  ScrollPane : public BaseWindow
{
public:
	typedef ScrollPane Self;
	ScrollPane(System& sys, const std::string& name = "");
	virtual ~ScrollPane();

	static const char* GetType() { return type_name<Self>::get(); }
	virtual const char* getType() { return type_name<Self>::get(); }

	virtual bool onLoad();
	virtual bool onSized(bool update = true);
	virtual bool onMouseWheel(int delta);
	virtual void init(xml::node& node);

	void setTarget(BaseWindow* target);

	void onTrack(const events::TrackEvent& e);
	void onTargetSized(const events::SizedEvent& e);

protected:
	void layoutTarget();

protected:
	ScrollBar*	m_scroll;
	BaseWindow* m_target;

};

}