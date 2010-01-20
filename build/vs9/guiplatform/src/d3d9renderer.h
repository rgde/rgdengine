#pragma once
#include "shader.h"
namespace gui
{
	namespace win32dx9platform
	{
		class DirectX9Renderer : public Renderer
		{
		public:
			DirectX9Renderer(LPDIRECT3DDEVICE9 device, unsigned int max_quads);
			virtual ~DirectX9Renderer();

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

			LPDIRECT3DDEVICE9 getDevice() const {return m_device;}

		protected:
			virtual void renderQuadDirect(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode);
			virtual	TexturePtr	createTextureInstance(const std::string& filename);

			void	initPerFrameStates();
			void	renderVBuffer();
			void	constructor_impl(LPDIRECT3DDEVICE9 device, const Size& display_size);
			void	setRenderStates();

		protected:	
			LPDIRECT3DDEVICE9		m_device;
			LPDIRECT3DTEXTURE9		m_currTexture;
			LPDIRECT3DVERTEXBUFFER9	m_buffer;
			LPDIRECT3DVERTEXDECLARATION9 m_vertexDeclaration;
			int						m_bufferPos;	

			typedef boost::shared_ptr<IShaderEffect> ShaderPtr;
			ShaderPtr				m_shader;
		protected:
			static const int			VERTEX_PER_QUAD;							//!< number of vertices per quad
			static const int			VERTEX_PER_TRIANGLE;						//!< number of vertices for a triangle
			static const int			VERTEXBUFFER_CAPACITY;						//!< capacity of the allocated vertex buffer
			static const unsigned long	VERTEX_FVF;									//!< FVF specifier constant

			D3DXHANDLE m_handleViewPortSize;
			D3DXHANDLE m_handleGuiTexture;

			bool m_needToAddCallback;
			RenderCallbackInfo m_callbackInfo;				 
		};
	}
}

