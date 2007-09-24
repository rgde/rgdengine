#pragma once

namespace ui
{
	class UIManager : public render::IRendererable
	{
		input::Trigger      m_cLeftMouse;
		input::RelativeAxis m_cXMoveMouse;
		input::RelativeAxis m_cYMoveMouse;

		math::Vec2f m_vMousePos;

		std::list<ui::IChildControl*> m_controls;

		bool m_bIsMouseMoved;
	public:
		UIManager() : m_vMousePos(400, 300), m_bIsMouseMoved(false)
		{
			//m_cLeftMouse.attachToControl(input::Mouse,	input::ButtonLeft);
			//m_cXMoveMouse.attachToControl(input::Mouse, input::AxisX);
			//m_cYMoveMouse.attachToControl(input::Mouse, input::AxisY);

			//m_cLeftMouse.addHandler(this, &UIManager::onLeftMouseButton);
			//m_cXMoveMouse.addHandler(this,&UIManager::onXMouseMoveCommand);
			//m_cYMoveMouse.addHandler(this,&UIManager::onYMouseMoveCommand);

			ShowCursor(TRUE);		

			m_renderInfo.pRenderFunc = boost::bind( &UIManager::render, this );
		}

	protected:		
		//обработчики для команд
		//void onLeftMouseButton(const input::CTriggerEvent& e)
		//{
		//	std::list<ui::IChildControl*>::iterator i;
		//	for ( i = m_controls.begin(); i != m_controls.end(); ++i )
		//	{
		//		math::Rect r = (*i)->getRect();
		//		if ( r.isPointInside( m_vMousePos ) )
		//		{
		//			Event ev;
		//			ev.eventType = Event::MouseClick;
		//			(*i)->onEvent( ev );
		//		}
		//	}
		//}

		//void onXMouseMoveCommand(const input::CRelativeAxisEvent& e)
		//{
		//	m_vMousePos[0] += e.m_nDelta * 1.5f;/// 2.0f;
		//	if (m_vMousePos[0] < 0) m_vMousePos[0] = 0;
		//	if (m_vMousePos[0] > 800) m_vMousePos[0] = 800;

		//	m_bIsMouseMoved = true;
		//}

		//void onYMouseMoveCommand(const input::CRelativeAxisEvent& e)
		//{
		//	m_vMousePos[1] += e.m_nDelta * 1.5f;/// 3.0f;
		//	if (m_vMousePos[1] < 0) m_vMousePos[1] = 0;
		//	if (m_vMousePos[1] > 600) m_vMousePos[1] = 600;

		//	m_bIsMouseMoved = true;
		//}

		void drawMouseCursor(const math::Vec2f& pos)
		{
			math::Vec2f dx(10,0);
			math::Vec2f dy(0,10);

			render::CLine2dManager& lr = render::TheLine2dManager::Get();
			lr.addLine(pos - dx, pos + dx, math::Green);
			lr.addLine(pos - dy, pos + dy, math::Green);
		}

		void render()
		{			
			std::list<ui::IChildControl*>::iterator i;
			for ( i = m_controls.begin(); i != m_controls.end(); ++i )
			{
				Event ev;

				if ( m_bIsMouseMoved )
				{
					math::Rect r = (*i)->getRect();
					if ( r.isPointInside( m_vMousePos ) )
					{
						ev.eventType = Event::MouseMove;
						(*i)->onEvent( ev );
					}					
				}

				ev.eventType = Event::Paint;
				(*i)->onEvent( ev );
			}
			drawMouseCursor( m_vMousePos );
		}
	public:
		void addControl( IChildControl* cont )
		{
			m_controls.push_back( cont );
		}

		virtual render::SRenderableInfo&	getRenderableInfo()
		{
			return m_renderInfo;
		}
	};

	typedef base::TSingelton<UIManager> TheUIManager;

}