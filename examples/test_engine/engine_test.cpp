#include <rgde/engine.h>

#include <boost/scoped_ptr.hpp>

namespace game
{
	class game_object : public game::IDynamicObject
	{
	public:
		game_object()
		{
			{
				using namespace math;
				m_cursor.pTexture = render::ITexture::Create("TestInput/cursor.png");
				m_cursor.uPriority = (uint)1000;
				m_cursor.size = Vec2f(32, 32);
				m_cursor.color = Color(255, 255, 255, 255);
				m_cursor.rect = Rect(0, 0, 1, 1);
				m_cursor.pos = Vec2f(m_mouse.getX(), m_mouse.getY());
				m_cursor.spin = 0;
			}
		}

	protected:
		void update(float dt)
		{
			if (m_mouse.isLeftPressed())
				m_cursor.color = math::Color(255,0,0,128);
			else
				m_cursor.color = math::Color(0,255,0,255);

			if (m_mouse.isRightPressed())
			{
				m_cursor.color = math::Color(0,0,255,128);
				//...
			}
			else
			{
				//...
			}
			using namespace math;
			m_cursor.pos = Vec2f(m_mouse.getX(), m_mouse.getY());
			m_sprite_renderer.addSprite(m_cursor);
		}

	protected:
		input::Mouse           m_mouse;
		render::SSprite        m_cursor;
		render::CSpriteManager m_sprite_renderer;
	};

	class base_application
	{
		typedef boost::scoped_ptr<core::IApplication> PApplication;
	public:
		base_application()
			: m_rgde_application(core::IApplication::Create(L"Engine test", 800, 600, false))
		{

			create_tasks();

			game_object obj;

			m_rgde_application->Run();
		}

	protected:
		void init_resources()
		{

		}

		void create_tasks()
		{
			using namespace core;

			IApplication& app = *m_rgde_application;

			app.addTask(PTask(new InputTask(app, 0, false)));
			app.addTask(PTask(new CGameTask(app, 1)));
			app.addTask(PTask(new RenderTask(app, 2)));
		}

	private:
		PApplication m_rgde_application;
	};
}


void main()
{
	game::base_application();
}
