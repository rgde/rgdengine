#include "precompiled.h"

#include <rgde/render/font.h>
#include <rgde/render/manager.h>
#include <rgde/render/device.h>

#include <d3dx9.h>
extern LPDIRECT3DDEVICE9 g_pd3dDevice;

namespace render
{
	class CFontRenderManager : public IRendererable
	{
	protected:
		CFontRenderManager() : IRendererable(10002)
		{
			m_renderInfo.pRenderFunc = boost::bind(&CFontRenderManager::renderAll, this);
		}

	public:
		typedef boost::function<void(void)>	FontRenderCallback;

		struct STextInfo
		{
			STextInfo() {}
			STextInfo(PFont pFont, FontRenderCallback rcb) : cb(rcb), m_pFont(pFont) {}

			PFont m_pFont;
			FontRenderCallback	cb;
		};		

		typedef std::list<STextInfo>	TextList;

		TextList texts;		

		void addText(PFont font, FontRenderCallback cb)
		{
			// тут и вообще мы легко упадем, если у нас не будет рендера.
			// надо делать проверки.
			texts.push_back(STextInfo(font, cb));
		}		

		void renderAll()
		{
			for (TextList::iterator it = texts.begin(); it != texts.end(); ++it)
				it->cb();
			texts.clear();
		}		

		SRenderableInfo & getRenderableInfo()
		{
			return m_renderInfo;
		}
	};	

	typedef base::TSingelton<CFontRenderManager> TheFontRenderManager;

	static int fontsCreated	= 0;

	void IFont::render(const std::wstring &text, const math::Rect &rect, unsigned int color)
	{
		render(text, rect, color, false, Top | Left | WordBreak);
	}	

	void IFont::render(const std::wstring &text, const math::Rect &rect, unsigned int color, bool isDrawShadow)
	{
		render(text, rect, color, isDrawShadow, Top | Left | WordBreak);
	}	

	class FontImpl : public IFont, public IDeviceObject
	{
		int				m_nHeight;
		std::wstring	m_name;
		FontWeight		m_eFontWeght;
		bool			m_useDelayedRender;

		void disableDelayedRender(bool b)
		{
			m_useDelayedRender = !b;
		}

		void Destroy()
		{
			if (m_pFont != NULL)
			{
				m_pFont->Release();
				m_pFont = 0;
				fontsCreated--;
			}
		}

		void Create()
		{
			if (g_pd3dDevice == NULL || m_pFont != NULL)
				return;

			if (FAILED(D3DXCreateFont(g_pd3dDevice, -m_nHeight, 0, m_eFontWeght, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 5, DEFAULT_PITCH | FF_DONTCARE, m_name.c_str(), &m_pFont)))
			{
				throw std::bad_exception("FontImpl():Can't create device font object!");
			}			fontsCreated++;
		}

	public:
		FontImpl(int height, const std::wstring &name, FontWeight font_weigh)
			: m_pFont(0),
			  m_nHeight(height),
			  m_name(name),
			  m_eFontWeght(font_weigh),
			  m_useDelayedRender(true)
		{
			base::lmsg << "Creating Font:  \"" << std::string(name.begin(), name.end()) << "\"," << m_nHeight;
			Create();
		}		

		void doRender(const std::wstring text, RECT textLocation, unsigned int color, int flags)
		{
			if (m_pFont != NULL)
				m_pFont->DrawText(NULL, text.c_str(), -1, &textLocation, flags, color);
		}		

		math::Rect getRect(const std::wstring &text, int flags)
		{
			RECT rc	= {0, 0, 0, 200};

			if (m_pFont != NULL)
				m_pFont->DrawText(NULL, text.c_str(), -1, &rc, DT_CALCRECT | flags, 0);

			math::Rect	ret((float)rc.left, (float)rc.top, (float)rc.right - rc.left, (float)rc.bottom - rc.top);
			return ret;
		}		

		virtual void render(const std::wstring &text, const math::Rect &rect, unsigned int color, bool isDrawShadow, int flags)
		{
			const math::Vec2f virtSize(800, 600);

			unsigned	nShadowDistance	= 2;
			unsigned	nShadowColor	= 0xFF000000; //Black
			math::Vec2f	screen_size		= render::TheDevice::Get().getBackBufferSize();
			math::Vec2f	ratio = screen_size / virtSize;

			if (isDrawShadow)
			{
				RECT	textShadowLocation;
				textShadowLocation.left = (LONG)((rect.position[0] + nShadowDistance) * ratio[0]);
				textShadowLocation.top = (LONG)((rect.position[1] + nShadowDistance) * ratio[1]);
				textShadowLocation.right = textShadowLocation.left + (LONG)(rect.size[0] * ratio[0]);
				textShadowLocation.bottom = textShadowLocation.top + (LONG)(rect.size[1] * ratio[1]);
			}

			RECT	textLocation;
			textLocation.left = (LONG)(rect.position[0] * ratio[0]);
			textLocation.top = (LONG)(rect.position[1] * ratio[1]);
			textLocation.right = textLocation.left + (LONG)(rect.size[0] * ratio[0]);
			textLocation.bottom = textLocation.top + (LONG)(rect.size[1] * ratio[1]);
		}		

		virtual void onLostDevice()
		{
			if (m_pFont != NULL)
			{
				m_pFont->OnLostDevice();
				Destroy();
			}
		}		

		virtual void onResetDevice()
		{
			Create();
			if (m_pFont != NULL) 
				m_pFont->OnResetDevice();
		}

		virtual ~FontImpl()
		{
			Destroy();

			if (0 == fontsCreated)
			{
				//TheFontRenderManager::Destroy();
			}
		}	

	private:		
		ID3DXFont	*m_pFont;
	};

	PFont IFont::Create(int height, const std::wstring &name, FontWeight font_weigh)
	{
		try
		{
			return PFont(new FontImpl(height, name, font_weigh));
		}
		catch (...)
		{
			return PFont();
		}
	}
}