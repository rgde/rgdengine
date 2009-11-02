#pragma once


namespace ui
{
	class control : public base_control						
	{
		children_list	m_children;
		math::Rect		m_rect;
	public:
		typedef std::list<base_control*> children_list;

		virtual void on_event(const Event& e)
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

		virtual const math::Rect& get_rect() { return m_rect; }
		virtual void set_rect(const math::Rect& rect) { m_rect = rect; }

		virtual const children_list& get_children()
		{
			return m_children;
		}

	protected:
		virtual void add(base_control* pControl)
		{
			m_children.push_back(pControl);
		}

		virtual void remove(base_control* pControl)
		{
			m_children.remove(pControl);
		}
	};
}
