
namespace ui
{
	class uiButton: public IButton, public game::dynamic_object
	{
	protected:
		std::wstring m_text;
		math::Rect m_rect;

		render::font_ptr m_spFont;

		render::Sprite sprite;
		
		bool m_pressed;
		bool m_wait;
		bool m_over;
		float m_wait_time;
	public:

		uiButton()
		{
			m_rect = math::Rect( 200, 0, 200, 30 );

			m_text = L"button";

			//m_spFont = render::IFont::Create(16, L"Tahoma");

			m_pressed = false;
			m_wait = false;
			m_over = false;
		}

		virtual const std::wstring& getText()
		{
			return m_text;
		}

		virtual void setText(const std::wstring& txt)
		{
			m_text = txt;
		}

		virtual void onEvent(const Event& e)
		{
			switch(e.eventType) 
			{
			case Event::Paint:
				{
					draw();
					break;
				}
			case Event::MouseMove:
				{
					m_over = true;
				}
				break;
			case Event::MouseClick:
				{
					m_pressed = true;
					break;
				}
			case Event::Update:
				{
					break;
				}
			default:
				break;
			};
		}

		const IControl* get_parent()
		{
			return 0;
		}

		void update( float dt )
		{
			if ( m_pressed )
			{
				m_wait = true;
				m_wait_time = 0;
				m_pressed = false;
			}

			if ( m_wait_time > 0.5f )
			{
				m_wait_time = 0;
				m_wait = false;
			}

			if ( m_wait )
			{
				m_wait_time += dt;
			}
		}

		virtual const math::Rect& getRect()
		{
			return m_rect;
		}

		virtual void setRect(const math::Rect& r)
		{
			m_rect = r;
		}
	protected:
		void draw()
		{
			drawButton( m_rect, m_text, Border3D, false, m_wait );
			m_over = false;
		}

		enum BorderStyle
		{
			BorderFlat, 
			Border3D
		};

		void drawBorder(math::Rect rect, BorderStyle style = BorderFlat)
		{
			math::Color lightGrey(200,200,200,m_over?255:128);
			math::Color lightLightGrey(250,250,250,m_over?255:128);
			math::Color darkGrey(170,170,170,m_over?255:128);
			math::Color black(0,0,0,m_over?255:128);

			render::Line2dManager& lr = render::TheLine2dManager::get();

			math::Vec2f& position = rect.position;
			math::Vec2f& size = rect.size;

			switch(style)
			{
			case BorderFlat:
				{
					lr.addLine(position, math::Vec2f(position[0], position[1]+size[1]), black);
					lr.addLine(position, math::Vec2f(position[0]+size[0], position[1]), black);
					lr.addLine(math::Vec2f(position[0], position[1]+size[1]), size + position, black);
					lr.addLine(math::Vec2f(position[0]+size[0], position[1]), size + position, black);
				}
				break;
			case Border3D:
				{
					lr.addLine(position, math::Vec2f(position[0], position[1]+size[1]), lightLightGrey);
					lr.addLine(position, math::Vec2f(position[0]+size[0], position[1]), lightLightGrey);
					lr.addLine(math::Vec2f(position[0], position[1]+size[1]), size + position, darkGrey);
					lr.addLine(math::Vec2f(position[0]+size[0], position[1]), size + position, darkGrey);
				}
				break;
			}
		}

		void drawButton(math::Rect rect, std::wstring text = L"", BorderStyle style = BorderFlat, bool isMouseOver = false, bool isPressed = false)
		{
			math::Color lightGrey(200,200,200,m_over?255:128);
			math::Color lightLightGrey(250,250,250,m_over?255:128);
			math::Color darkGrey(170,170,170,m_over?255:128);

			if ( !m_wait )
				drawBorder(rect, style);

			//drawText(m_spFont, text, rect, !m_wait);

			sprite.pos = rect.position;
			sprite.size = rect.size;
			sprite.spin = 0;
			if (isPressed)
				sprite.color = darkGrey;
			else 
				sprite.color = lightGrey;
			sprite.uPriority = 1;

			render::TheSpriteManager::get().addSprite(sprite);
		}

		void drawText(render::font_ptr font, std::wstring text, math::Rect rect, bool bShowShadow = true)
		{
			int flag = render::IFont::Center | render::IFont::VCenter;

			math::Vec2f vFrontBufferSize = render::TheDevice::get().getBackBufferSize();
			math::Vec2f screenSize(800, 600);

			rect.position	= (rect.position * vFrontBufferSize) / screenSize;
			rect.size		= (rect.size * vFrontBufferSize) / screenSize;
			font->render(text, rect, 0xFFFFFFFF, bShowShadow, flag);
		}
	};


}