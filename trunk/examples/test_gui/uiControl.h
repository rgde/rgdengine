#pragma once


namespace ui
{
	class CControl :	public virtual IControl						
	{
		Children	m_lChildren;
		math::Rect		m_rect;
	public:
		typedef std::list<IControl*> Children;

		virtual void onEvent(const Event& e)
		{
			switch(e.eventType) 
			{
			case Event::Paint:
				{
					draw();
					break;
				}
			case Event::Update:
				{
					idle();
					break;
				}
			default:
				break;
			};
		}

		virtual void idle(){}
		virtual void draw(){}

		virtual const math::Rect& getRect() { return m_rect; }
		virtual void setRect(const math::Rect& rect) { m_rect = rect; }

		virtual const Children& get_children()
		{
			return m_lChildren;
		}

	protected:
		virtual void add(IControl* pControl)
		{
			m_lChildren.push_back(pControl);
		}

		virtual void remove(IControl* pControl)
		{
			m_lChildren.remove(pControl);
		}
	};
}
