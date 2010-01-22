#pragma once

#include <rgde/render/effect.h>

namespace gui
{
	namespace rgde_platform
	{
		class renderer : public Renderer
		{
		public:
			renderer(rgde::render::device_ptr device, unsigned int max_quads);
			virtual ~renderer();

			virtual void addCallback( AfterRenderCallbackFunc callback,
				BaseWindow* window, const Rect& dest, const Rect& clip);
			virtual	void doRender();
			virtual void addQuad(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode);
		
			virtual void drawFromCache(BaseWindow* window);
			virtual	TexturePtr	createTexture(const std::string& filename);
			virtual	void		destroyTexture(TexturePtr tex);
			virtual	TexturePtr loadFromMemory(const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat);

			// temporary
			virtual	TexturePtr	createEmptyTexture(unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat);
			virtual TexturePtr	reloadTextureFromBuffer(TexturePtr p, const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat);

			virtual FontPtr		createFont(const std::string& name, const std::string& fontname, unsigned int size);

			virtual void	beginBatching();
			virtual void	endBatching();

			virtual void	OnResetDevice();
			virtual void	OnLostDevice();

			virtual Size	getViewportSize() const;

			rgde::render::device_ptr getDevice() const {return m_device;}

		protected:
			virtual void renderQuadDirect(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode);
			virtual	TexturePtr	createTextureInstance(const std::string& filename);

			void	initPerFrameStates();
			void	renderVBuffer();
			void	constructor_impl(rgde::render::device_ptr device, const Size& display_size);
			void	setRenderStates();

		protected:	
			rgde::render::device_ptr m_device;
			rgde::render::texture_ptr m_currTexture;
			rgde::render::vertex_buffer_ptr	m_buffer;
			rgde::render::vertex_declaration_ptr m_vertexDeclaration;
			int m_bufferPos;	

			typedef rgde::render::effects::effect shader_effect;
			typedef rgde::render::effects::param_ptr shader_handle;

			typedef boost::shared_ptr<shader_effect> shader_ptr;
			shader_ptr				m_shader;
		protected:
			static const int			VERTEX_PER_QUAD;							//!< number of vertices per quad
			static const int			VERTEX_PER_TRIANGLE;						//!< number of vertices for a triangle
			static const int			VERTEXBUFFER_CAPACITY;						//!< capacity of the allocated vertex buffer
			static const unsigned long	VERTEX_FVF;									//!< FVF specifier constant

			shader_handle m_handleViewPortSize;
			shader_handle m_handleGuiTexture;

			bool m_needToAddCallback;
			RenderCallbackInfo m_callbackInfo;				 
		};
	}
}
