#include "precompiled.h"

#include <rgde/render/font.h>
#include <rgde/render/manager.h>
#include <rgde/render/render_device.h>

#include <d3dx9.h>
extern LPDIRECT3DDEVICE9 g_d3d;

namespace render
{
	class font_manager : public rendererable
	{
	protected:
		font_manager() : rendererable(10002)
		{
			m_render_info.render_func = boost::bind(&font_manager::renderAll, this);
		}

	public:
		typedef boost::function<void(void)>	FontRenderCallback;
		typedef std::list<FontRenderCallback>	CBList;

		CBList texts;		

		void addText(FontRenderCallback cb)
		{
			// тут и вообще мы легко упадем, если у нас не будет рендера.
			// надо делать проверки.
			texts.push_back(cb);
		}		

		void renderAll()
		{
			for (CBList::iterator it = texts.begin(); it != texts.end(); ++it)
				(*it)();
			texts.clear();
		}		

		renderable_info & get_renderable_info()
		{
			return m_render_info;
		}
	};	

	typedef base::singelton<font_manager> TheFontRenderManager;

	static int fontsCreated	= 0;

	void font::render(const std::wstring &text, const math::Rect &rect, unsigned int color)
	{
		render(text, rect, color, false, Top | Left | WordBreak);
	}	

	void font::render(const std::wstring &text, const math::Rect &rect, unsigned int color, bool isDrawShadow)
	{
		render(text, rect, color, isDrawShadow, Top | Left | WordBreak);
	}	

	class font_impl : public font, public device_object
	{
		int				m_height;
		std::wstring	m_name;
		FontWeight		m_eFontWeght;
		bool			m_useDelayedRender;

		void disableDelayedRender(bool b)
		{
			m_useDelayedRender = !b;
		}

		void destroy()
		{
			if (m_font != NULL)
			{
				m_font->Release();
				m_font = 0;
				fontsCreated--;
			}
		}

		void create()
		{
			if (g_d3d == NULL || m_font != NULL)
				return;

			if (FAILED(D3DXCreateFont(g_d3d, -m_height, 0, m_eFontWeght, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 5, DEFAULT_PITCH | FF_DONTCARE, m_name.c_str(), &m_font)))
			{
				throw std::bad_exception("font_impl():Can't create device font object!");
			}			fontsCreated++;
		}

	public:
		font_impl(int height, const std::wstring &name, FontWeight font_weigh)
			: m_font(0),
			  m_height(height),
			  m_name(name),
			  m_eFontWeght(font_weigh),
			  m_useDelayedRender(true)
		{
			base::lmsg << "Creating Font:  \"" << std::string(name.begin(), name.end()) << "\"," << m_height;
			create();
		}		

		void doRender(const std::wstring text, RECT textLocation, unsigned int color, int flags)
		{
			if (m_font != NULL)
				m_font->DrawText(NULL, text.c_str(), -1, &textLocation, flags, color);
		}		

		math::Rect get_rect(const std::wstring &text, int flags)
		{
			RECT rc	= {0, 0, 0, 200};

			if (m_font != NULL)
				m_font->DrawText(NULL, text.c_str(), -1, &rc, DT_CALCRECT | flags, 0);

			math::Rect	ret((float)rc.left, (float)rc.top, (float)rc.right - rc.left, (float)rc.bottom - rc.top);
			return ret;
		}		

		virtual void render(const std::wstring &text, const math::Rect &rect, unsigned int color, bool isDrawShadow, int flags)
		{
			const math::vec2f virtSize(800, 600);

			unsigned	nShadowDistance	= 2;
			unsigned	nShadowColor	= 0xFF000000; //Black
			math::vec2f	screen_size		= render::render_device::get().getBackBufferSize();
			math::vec2f	ratio = screen_size / virtSize;

			font_manager& rm = TheFontRenderManager::get();

			if (isDrawShadow)
			{
				RECT	textShadowLocation;
				textShadowLocation.left = (LONG)((rect.position[0] + nShadowDistance) * ratio[0]);
				textShadowLocation.top = (LONG)((rect.position[1] + nShadowDistance) * ratio[1]);
				textShadowLocation.right = textShadowLocation.left + (LONG)(rect.size[0] * ratio[0]);
				textShadowLocation.bottom = textShadowLocation.top + (LONG)(rect.size[1] * ratio[1]);

				//doRender(text, textShadowLocation, nShadowColor, flags);
				rm.addText(boost::bind(&font_impl::doRender, this, text, textShadowLocation, nShadowColor, flags));
			}

			RECT	textLocation;
			textLocation.left = (LONG)(rect.position[0] * ratio[0]);
			textLocation.top = (LONG)(rect.position[1] * ratio[1]);
			textLocation.right = textLocation.left + (LONG)(rect.size[0] * ratio[0]);
			textLocation.bottom = textLocation.top + (LONG)(rect.size[1] * ratio[1]);

			
			//doRender(text, textLocation, color, flags);
			rm.addText(boost::bind(&font_impl::doRender, this, text, textLocation, color, flags));
		}		

		virtual void onLostDevice()
		{
			if (m_font != NULL)
			{
				m_font->OnLostDevice();
				destroy();
			}
		}		

		virtual void onResetDevice()
		{
			create();
			if (m_font != NULL) 
				m_font->OnResetDevice();
		}

		virtual ~font_impl()
		{
			destroy();

			if (0 == fontsCreated)
			{
				//TheFontRenderManager::destroy();
			}
		}	

	private:		
		ID3DXFont	*m_font;
	};

	font_ptr font::create(int height, const std::wstring &name, FontWeight font_weigh)
	{
		try
		{
			return font_ptr(new font_impl(height, name, font_weigh));
		}
		catch (...)
		{
			return font_ptr();
		}
	}
}