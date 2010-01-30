#pragma once

#include <rgde/render/effect.h>
#include <rgde/core/file_system.h>

namespace gui
{
	namespace rgde_platform
	{
		class renderer : public Renderer
		{
		public:
			renderer(rgde::render::device& device, unsigned int max_quads, 
				rgde::core::vfs::system& vfs);
			virtual ~renderer();

			virtual void addCallback( AfterRenderCallbackFunc callback,
				BaseWindow* window, const Rect& dest, const Rect& clip);
			virtual	void doRender();
			virtual void addQuad(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours);
			virtual void addQuad(const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours);
		
			virtual void drawFromCache(BaseWindow* window);
			virtual	TexturePtr createTexture(const std::string& filename);
			virtual	TexturePtr createTexture(const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat);

			// temporary
			virtual	TexturePtr	createTexture(unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat);
			virtual TexturePtr	reloadTexture(TexturePtr p, const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat);

			virtual FontPtr		createFont(const std::string& name, const std::string& fontname, unsigned int size);

			virtual void	beginBatching();
			virtual void	endBatching();

			virtual void	OnResetDevice();
			virtual void	OnLostDevice();

			virtual Size	getViewportSize() const;

			rgde::render::device& getDevice() const {return m_device;}

		protected:
			virtual void renderQuadDirect(const QuadInfo& q);
			virtual	TexturePtr	createTextureInstance(const std::string& filename);

			void	initPerFrameStates();
			void	constructor_impl(const Size& display_size);
			void	setRenderStates();

		protected:	
			rgde::render::device& m_device;
			rgde::render::texture_ptr m_currTexture;
			rgde::render::vertex_buffer_ptr	m_buffer;
			rgde::render::index_buffer_ptr	m_ibuffer;
			rgde::render::vertex_declaration_ptr m_vertexDeclaration;
			int m_bufferPos;	

			rgde::core::vfs::system& m_filesystem;

			typedef rgde::render::effects::effect shader_effect;
			typedef rgde::render::effects::param_ptr shader_handle;

			typedef boost::shared_ptr<shader_effect> shader_ptr;
			shader_ptr				m_shader;

		protected:
			shader_handle m_handleViewPortSize;
			shader_handle m_handleGuiTexture;

			bool m_needToAddCallback;
			RenderCallbackInfo m_callbackInfo;				 
		};
	}
}

