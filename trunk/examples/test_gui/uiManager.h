#pragma once

namespace ui
{
	class manager : public render::rendererable
	{
		input::Trigger      m_cLeftMouse;
		input::RelativeAxis m_cXMoveMouse;
		input::RelativeAxis m_cYMoveMouse;

		math::Vec2f m_mouse_pos;

		std::list<ui::base_control*> m_controls;

		bool m_bIsMouseMoved;
	public:
		manager() : m_mouse_pos(400, 300), m_bIsMouseMoved(false)
		{
			//m_cLeftMouse.attachToControl(input::Mouse,	input::ButtonLeft);
			//m_cXMoveMouse.attachToControl(input::Mouse, input::AxisX);
			//m_cYMoveMouse.attachToControl(input::Mouse, input::AxisY);

			//m_cLeftMouse.addHandler(this, &manager::onLeftMouseButton);
			//m_cXMoveMouse.addHandler(this,&manager::onXMouseMoveCommand);
			//m_cYMoveMouse.addHandler(this,&manager::onYMouseMoveCommand);

			ShowCursor(TRUE);		

			m_renderInfo.render_func = boost::bind( &manager::render, this );
		}

	protected:		
		//обработчики для команд
		//void onLeftMouseButton(const input::CTriggerEvent& e)
		//{
		//	std::list<ui::base_control*>::iterator i;
		//	for ( i = m_controls.begin(); i != m_controls.end(); ++i )
		//	{
		//		math::Rect r = (*i)->get_rect();
		//		if ( r.isPointInside( m_mouse_pos ) )
		//		{
		//			Event ev;
		//			ev.eventType = Event::MouseClick;
		//			(*i)->on_event( ev );
		//		}
		//	}
		//}

		//void onXMouseMoveCommand(const input::CRelativeAxisEvent& e)
		//{
		//	m_mouse_pos[0] += e.m_nDelta * 1.5f;/// 2.0f;
		//	if (m_mouse_pos[0] < 0) m_mouse_pos[0] = 0;
		//	if (m_mouse_pos[0] > 800) m_mouse_pos[0] = 800;

		//	m_bIsMouseMoved = true;
		//}

		//void onYMouseMoveCommand(const input::CRelativeAxisEvent& e)
		//{
		//	m_mouse_pos[1] += e.m_nDelta * 1.5f;/// 3.0f;
		//	if (m_mouse_pos[1] < 0) m_mouse_pos[1] = 0;
		//	if (m_mouse_pos[1] > 600) m_mouse_pos[1] = 600;

		//	m_bIsMouseMoved = true;
		//}

		void draw_cursor(const math::Vec2f& pos)
		{
			math::Vec2f dx(10,0);
			math::Vec2f dy(0,10);

			render::Line2dManager& lr = render::TheLine2dManager::get();
			lr.add_line(pos - dx, pos + dx, math::Green);
			lr.add_line(pos - dy, pos + dy, math::Green);
		}

		void render()
		{			
			std::list<ui::base_control*>::iterator i;
			for ( i = m_controls.begin(); i != m_controls.end(); ++i )
			{
				Event ev;

				if ( m_bIsMouseMoved )
				{
					math::Rect r = (*i)->get_rect();
					if ( r.isPointInside( m_mouse_pos ) )
					{
						ev.eventType = Event::MouseMove;
						(*i)->on_event( ev );
					}					
				}

				ev.eventType = Event::Paint;
				(*i)->on_event( ev );
			}
			draw_cursor( m_mouse_pos );
		}
	public:
		void addControl( base_control* cont )
		{
			m_controls.push_back( cont );
		}

		virtual render::renderable_info&	getRenderableInfo()
		{
			return m_renderInfo;
		}
	};

	typedef base::singelton<manager> TheUIManager;

}