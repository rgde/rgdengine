#pragma once
#include "window.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace gui
{
class GUILIB_API DragContainer : public BaseWindow
{
public:
	typedef DragContainer Self;
	DragContainer(System& sys, const std::string& name = "");
	virtual ~DragContainer(void);

	static const char* GetType() { return "DragContainer"; }
	virtual const char* getType() { return Self::GetType(); }

	virtual void rise();

	void update(BaseWindow* target, const Point& pt);
	
	bool startDrag(BaseWindow* subj, const Point& off);
	bool stopDrag(void);

	void reset(void);


protected:
	BaseWindow* m_dragSubject;
	
	BaseWindow* m_dropTarget;
	Point		m_childOffset;
	
};
}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif