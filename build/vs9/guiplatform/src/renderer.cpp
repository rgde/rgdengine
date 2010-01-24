#include "stdafx.h"

#include <rgde/render/device.h>

#include "renderer.h"
#include "texture.h"
#include "ftfont.h"

// fine tune :)
#define PixelAligned(x)	( ( (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) ) - 0.5f )

using namespace rgde::render;

namespace gui
{
	namespace rgde_platform
	{
		Renderer* CreateRenderer(rgde::render::device& dev, 
			rgde::core::vfs::system& vfs,
			unsigned buff)
		{
			return new renderer(dev, 1024, vfs);
		}
				
		struct QuadVertex
		{
			float x, y, tu1, tv1;		//!< The transformed position for the vertex.
			unsigned int diffuse;			//!< colour of the vertex
		};

		using rgde::render::vertex_element;
		vertex_element vertex_desc[] = 
		{
			{0, 0,  vertex_element::float4,   vertex_element::default_method, vertex_element::position, 0}, 
			{0, 12, vertex_element::color4ub, vertex_element::default_method, vertex_element::color,	0},
			vertex_element::end_element
		};

		const int			renderer::VERTEX_PER_QUAD = 6;
		const int			renderer::VERTEX_PER_TRIANGLE = 3;
		const int			renderer::VERTEXBUFFER_CAPACITY	= 8192;

		/*************************************************************************
		Constructor
		*************************************************************************/
		renderer::renderer(device& device, unsigned int max_quads, 
			rgde::core::vfs::system& vfs)
			: m_filesystem(vfs)
			, m_device(device)
		{
			m_needToAddCallback = false;
			Size size(getViewportSize());

			constructor_impl(size);
		}


		/*************************************************************************
		method to do work of constructor
		*************************************************************************/
		void renderer::constructor_impl(const Size& display_size)
		{
			m_bufferPos     = 0;

			m_originalsize = display_size;

			m_vertexDeclaration = vertex_declaration::create(m_device, vertex_desc, 3);

			m_buffer = vertex_buffer::create(
				m_device, 
				m_vertexDeclaration, 
				VERTEXBUFFER_CAPACITY * sizeof(QuadVertex),
				resource::default, 
				buffer::write_only | buffer::dynamic);

			// Create a vertex buffer
			if (!m_buffer)
			{
				// Ideally, this would have been a RendererException, but we can't use that until the System object is created
				// and that requires a Renderer passed to the constructor, so we throw this instead.
				throw std::exception("Creation of VertexBuffer for Renderer object failed");
			}

			char s[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH,s);
					

			assert(0 && "TODO!");
			//m_shader.reset(new shader_effect(m_device, L"gui\\shaders\\gui.fx"));

			// get the maximum available texture size.
			// set max texture size the the smaller of max width and max height.
			m_maxTextureSize = 2048;//devCaps.MaxTextureWidth < devCaps.MaxTextureHeight ? devCaps.MaxTextureWidth : devCaps.MaxTextureHeight;

			m_handleGuiTexture = m_shader->get_param("guiTexture");
			m_handleViewPortSize = m_shader->get_param("ViewPortSize");
		}


		/*************************************************************************
		Destructor
		*************************************************************************/
		renderer::~renderer(void)
		{
		}

		void renderer::addCallback( AfterRenderCallbackFunc callback,
											BaseWindow* window, const Rect& dest, const Rect& clip)
		{
			// если сразу должны были рисовать, то сразу запускаем коллбак
			if (!m_isQueueing)
			{
				if (window && callback)
					callback(window,dest, clip);
				return;
			}

			m_needToAddCallback = true;
			m_callbackInfo.afterRenderCallback = callback;
			m_callbackInfo.window = window;
			m_callbackInfo.dest = dest;
			m_callbackInfo.clip = clip;
		}

		void renderer::addQuad(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode)
		{
			Rect local_dest_rect = dest_rect;

			// if not queueing, render directly (as in, right now!)
			if (!m_isQueueing)
			{
				renderQuadDirect(local_dest_rect, tex_rect, 1.0f, img, colours, quad_split_mode);
			}
			else
			{
				if (m_num_quads >= m_quads.size())
				{
					m_quads.resize(m_num_quads*2);
				}


				QuadInfo& quad = m_quads[m_num_quads];
				quad.position		= local_dest_rect;
				quad.z				= z;
				quad.texture		= &img.getTexture();
				quad.texPosition	= tex_rect;
				quad.topLeftCol		= colours.m_top_left.getARGB();
				quad.topRightCol	= colours.m_top_right.getARGB();
				quad.bottomLeftCol	= colours.m_bottom_left.getARGB();
				quad.bottomRightCol	= colours.m_bottom_right.getARGB();

				// set quad split mode
				quad.splitMode = quad_split_mode;
				
				if (m_currentCapturing)
				{
					if (m_currentCapturing->num >= m_currentCapturing->m_vec.size())
						m_currentCapturing->m_vec.resize(m_currentCapturing->num * 2);
					m_currentCapturing->m_vec[m_currentCapturing->num] = quad;
					++(m_currentCapturing->num);
				}



				if (!m_num_quads  || m_quads[m_num_quads - 1].texture != quad.texture ||
					m_needToAddCallback || 
					(m_num_batches && (m_num_quads - m_batches[m_num_batches - 1].startQuad + 1)*VERTEX_PER_QUAD >= VERTEXBUFFER_CAPACITY))
				{
					// закончим предыдущий батч если он есть
					if (m_num_batches)
					{
						m_batches[m_num_batches - 1].numQuads = int(m_num_quads - m_batches[m_num_batches - 1].startQuad);
						
						if (!m_needToAddCallback)
						{
							m_callbackInfo.window = NULL;
							m_callbackInfo.afterRenderCallback = NULL;
						}
						m_needToAddCallback = false;
						m_batches[m_num_batches - 1].callbackInfo = m_callbackInfo;
					}

					// начнем следующий батч
					m_batches[m_num_batches].texture = quad.texture;
					m_batches[m_num_batches].startQuad = (int)m_num_quads;

					++m_num_batches;
				}

				++m_num_quads;

			}
		}

		void renderer::setRenderStates()
		{
			// setup vertex stream
			m_device.set_stream_source(0, m_buffer, sizeof(QuadVertex));
			
			view_port viewPortDesc;
			m_device.get_viewport(viewPortDesc);

			//m_device->SetVertexDeclaration(m_vertexDeclaration);

			m_shader->set_tech("Simple");
			rgde::math::vec2f vec((float)viewPortDesc.width, (float)viewPortDesc.height);

			m_shader->set(m_handleViewPortSize,(float*)&vec, 2 );
			m_shader->begin(0);
			m_shader->begin_pass(0);
		}
		/*************************************************************************
		perform final rendering for all queued renderable quads.
		*************************************************************************/
		void renderer::doRender(void)
		{

			if (!m_buffer)
				return;

			setRenderStates();
			m_currTexture.reset();

			float scaleX = 1.f;
			float scaleY = 1.f;
			if(m_autoScale)
			{
				Size& viewport = getViewportSize();
				scaleX = viewport.m_width / m_originalsize.m_width;
				scaleY = viewport.m_height / m_originalsize.m_height;
			}

			static DWORD s_quadOffset = 0;	// смещение от начала буффера в квадах
			QuadVertex*	buffmem;
			
			for (std::size_t b = 0; b < m_num_batches; ++b)
			{
				BatchInfo& batch = m_batches[b];
				if ( 6 * (batch.numQuads + s_quadOffset) >= VERTEXBUFFER_CAPACITY)
					s_quadOffset = 0;

				buffmem = (QuadVertex*)m_buffer->lock(UINT(s_quadOffset * 6 * sizeof(QuadVertex)), 
					UINT(6 * (m_batches[b].numQuads)* sizeof(QuadVertex)),
					s_quadOffset ? buffer::nooverwrite : buffer::discard);

				if (!buffmem )
					return;

				std::size_t numQ = batch.numQuads;
				for (std::size_t q = 0; q < numQ; ++q)
				{
					const QuadInfo& quad = m_quads[q + batch.startQuad];

					float left = quad.position.m_left * scaleX;
					float right = quad.position.m_right * scaleX;
					float top = quad.position.m_top * scaleY - 0.5f;
					float bottom = quad.position.m_bottom * scaleY - 0.5f;

					right = PixelAligned(right);
					left = PixelAligned(left);
					top = PixelAligned(top);
					bottom = PixelAligned(bottom);

					// setup Vertex 1...
					buffmem->x = left;
					buffmem->y = top;
					buffmem->diffuse = quad.topLeftCol;
					buffmem->tu1 = quad.texPosition.m_left;
					buffmem->tv1 = quad.texPosition.m_top;
					++buffmem;

					// setup Vertex 2...

					// top-left to bottom-right diagonal
					if (quad.splitMode == TopLeftToBottomRight)
					{
						buffmem->x = right;
						buffmem->y = bottom;
						buffmem->diffuse = quad.bottomRightCol;
						buffmem->tu1 = quad.texPosition.m_right;
						buffmem->tv1 = quad.texPosition.m_bottom;
					}
					// bottom-left to top-right diagonal
					else
					{
						buffmem->x = right;
						buffmem->y = top;
						buffmem->diffuse = quad.topRightCol;
						buffmem->tu1 = quad.texPosition.m_right;
						buffmem->tv1 = quad.texPosition.m_top;
					}
					++buffmem;

					// setup Vertex 3...
					buffmem->x = left;
					buffmem->y = bottom;
					buffmem->diffuse = quad.bottomLeftCol;
					buffmem->tu1 = quad.texPosition.m_left;
					buffmem->tv1 = quad.texPosition.m_bottom;
					++buffmem;

					// setup Vertex 4...
					buffmem->x = right;
					buffmem->y = top;
					buffmem->diffuse = quad.topRightCol;
					buffmem->tu1 = quad.texPosition.m_right;
					buffmem->tv1 = quad.texPosition.m_top;
					++buffmem;

					// setup Vertex 5...
					buffmem->x = right;
					buffmem->y = bottom;
					buffmem->diffuse = quad.bottomRightCol;
					buffmem->tu1 = quad.texPosition.m_right;
					buffmem->tv1 = quad.texPosition.m_bottom;
					++buffmem;

					// setup Vertex 6...

					// top-left to bottom-right diagonal
					if (quad.splitMode == TopLeftToBottomRight)
					{
						buffmem->x = left;
						buffmem->y = top;
						buffmem->diffuse = quad.topLeftCol;
						buffmem->tu1 = quad.texPosition.m_left;
						buffmem->tv1 = quad.texPosition.m_top;
					}
					// bottom-left to top-right diagonal
					else
					{
						buffmem->x = left;
						buffmem->y = bottom;
						buffmem->diffuse = quad.bottomLeftCol;
						buffmem->tu1 = quad.texPosition.m_left;
						buffmem->tv1 = quad.texPosition.m_bottom;
					}
					++buffmem;
				}

				m_buffer->unlock();

				gui::rgde_platform::texture* t = static_cast<gui::rgde_platform::texture*>(batch.texture);
				m_device.set_texture(t->get_platform_resource(), 0);

				m_device.draw(triangle_list, s_quadOffset * 6, UINT(numQ * 2));
				s_quadOffset += (DWORD)numQ;


				if (batch.callbackInfo.window && batch.callbackInfo.afterRenderCallback)
				{
					m_shader->end_pass();
					m_shader->end();

					batch.callbackInfo.afterRenderCallback(batch.callbackInfo.window,
									batch.callbackInfo.dest, batch.callbackInfo.clip);

					// если это был не последний батч
					if (b < m_num_batches -1)
						setRenderStates();
				}
			}
			
			m_shader->end_pass();
			m_shader->end();
			

		}

		/*************************************************************************
		setup states etc
		*************************************************************************/
		void renderer::initPerFrameStates(void)
		{
			// setup vertex stream
			m_device.set_stream_source(0, m_buffer, sizeof(QuadVertex));

			//m_device->SetFVF(VERTEX_FVF);
			//m_device->SetVertexShader( 0 );
			//m_device->SetPixelShader( 0 );

			// set device states
			//m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			//m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			//m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			//m_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			//m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
			//m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


			// setup texture addressing settings
			//m_device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			//m_device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			// setup colour calculations
			//m_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			//m_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			//m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

			// setup alpha calculations
			//m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			//m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			//m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			// setup filtering
			//m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			//m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

			// disable texture stages we do not need.
			//m_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			// setup scene alpha blending
			//m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			//m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			//m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}


		/*************************************************************************
		renders whatever is in the vertex buffer
		*************************************************************************/
		void renderer::renderVBuffer(void)
		{
			// if bufferPos is 0 there is no data in the buffer and nothing to render
			if (m_bufferPos == 0)
			{
				return;
			}

			// render the sprites
			m_device.draw(triangle_list, 0, (m_bufferPos / VERTEX_PER_TRIANGLE));

			// reset buffer position to 0...
			m_bufferPos = 0;
		}

		/*************************************************************************
		render a quad directly to the display
		*************************************************************************/
		void renderer::renderQuadDirect(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode)
		{			
			if (!m_buffer)
				return;

			m_device.set_stream_source(0, m_buffer, sizeof(QuadVertex));
			view_port viewPortDesc;
			m_device.get_viewport(viewPortDesc);

			m_device.set_decl(m_vertexDeclaration);

			m_shader->set_tech("Simple");
			rgde::math::vec2f vec((float)viewPortDesc.width, (float)viewPortDesc.height);
			m_shader->set("ViewPortSize",&vec, 2 );
			m_shader->begin(0 );
			m_shader->begin_pass(0);

			QuadVertex*	buffmem;
			
			texture* tex = static_cast<texture*>(&img.getTexture());
			m_device.set_texture(((texture*)tex)->get_platform_resource(), 0 );

			buffmem = (QuadVertex*)m_buffer->lock(0, VERTEX_PER_QUAD * sizeof(QuadVertex), 
				buffer::discard);

			if (buffmem)
			{
				float scaleX = 1.f;
				float scaleY = 1.f;
				if(m_autoScale)
				{
					Size& viewport = getViewportSize();
					scaleX = viewport.m_width / m_originalsize.m_width;
					scaleY = viewport.m_height / m_originalsize.m_height;
				}

				float left = dest_rect.m_left * scaleX;
				left = PixelAligned(left);
				float top = dest_rect.m_top * scaleY;
				top = PixelAligned(top);
				float right = dest_rect.m_right * scaleX;
				right = PixelAligned(right);
				float bottom = dest_rect.m_bottom * scaleY;
				bottom = PixelAligned(bottom);

				// setup Vertex 1...
				buffmem->x = left;
				buffmem->y = top;
				buffmem->diffuse = colours.m_top_left.getARGB();
				buffmem->tu1 = tex_rect.m_left;
				buffmem->tv1 = tex_rect.m_top;
				++buffmem;

				// setup Vertex 2...

				// top-left to bottom-right diagonal
				if (quad_split_mode == TopLeftToBottomRight)
				{
					buffmem->x = right;
					buffmem->y = bottom;
					buffmem->diffuse = colours.m_bottom_right.getARGB();
					buffmem->tu1 = tex_rect.m_right;
					buffmem->tv1 = tex_rect.m_bottom;
				}
				// bottom-left to top-right diagonal
				else
				{
					buffmem->x = right;
					buffmem->y = top;
					buffmem->diffuse = colours.m_top_right.getARGB();
					buffmem->tu1 = tex_rect.m_right;
					buffmem->tv1 = tex_rect.m_top;
				}
				++buffmem;

				// setup Vertex 3...
				buffmem->x = left;
				buffmem->y = bottom;
				buffmem->diffuse = colours.m_bottom_left.getARGB();
				buffmem->tu1 = tex_rect.m_left;
				buffmem->tv1 = tex_rect.m_bottom;
				++buffmem;

				// setup Vertex 4...
				buffmem->x = right;
				buffmem->y = top;
				buffmem->diffuse = colours.m_top_right.getARGB();
				buffmem->tu1 = tex_rect.m_right;
				buffmem->tv1 = tex_rect.m_top;
				++buffmem;

				// setup Vertex 5...
				buffmem->x = right;
				buffmem->y = bottom;
				buffmem->diffuse = colours.m_bottom_right.getARGB();
				buffmem->tu1 = tex_rect.m_right;
				buffmem->tv1 = tex_rect.m_bottom;
				++buffmem;

				// setup Vertex 6...

				// top-left to bottom-right diagonal
				if (quad_split_mode == TopLeftToBottomRight)
				{
					buffmem->x = left;
					buffmem->y = top;
					buffmem->diffuse = colours.m_top_left.getARGB();
					buffmem->tu1 = tex_rect.m_left;
					buffmem->tv1 = tex_rect.m_top;
				}
				// bottom-left to top-right diagonal
				else
				{
					buffmem->x = left;
					buffmem->y = bottom;
					buffmem->diffuse = colours.m_bottom_left.getARGB();
					buffmem->tu1 = tex_rect.m_left;
					buffmem->tv1 = tex_rect.m_bottom;
				}

				m_buffer->unlock();
				m_bufferPos = VERTEX_PER_QUAD;

				renderVBuffer();
			}
			m_shader->end_pass();
			m_shader->end();

		}


		void renderer::OnLostDevice()
		{
			if (m_shader)
				m_shader->on_device_lost();

			m_buffer.reset();

			m_texmanager.onDeviceLost();
		}

		void renderer::OnResetDevice(void)
		{
			if(!m_buffer)
			{
				// Recreate a vertex buffer
				m_buffer = vertex_buffer::create(
					m_device,
					m_vertexDeclaration,
					(VERTEXBUFFER_CAPACITY * sizeof(QuadVertex)), 
					resource::default,
					buffer::dynamic|buffer::write_only
					);

				if (!m_buffer)
				{
					throw std::exception("renderer::onDeviceReset - Failed to create the VertexBuffer for use by the renderer object.");
				}
			}
			if (m_shader)
				m_shader->on_device_reset();

			m_texmanager.onDeviceReset();
		}

		Size renderer::getViewportSize(void) const
		{
			// initialise renderer size
			view_port	vp;
			m_device.get_viewport(vp);
			return Size((float)vp.width, (float)vp.height);
		}

		TexturePtr	renderer::createTexture(const std::string& filename)
		{
			return m_texmanager.createTexture(filename);
		}

		TexturePtr	renderer::createTextureInstance(const std::string& filename) 
		{
			TexturePtr tex;
			if(!filename.empty())
			{
				texture_ptr t = rgde::render::texture::create(
					m_device, 
					m_filesystem.open_read(filename)
					);

				if (t)
				{
					tex.reset(new texture(*this, t));
				}
			}
			return tex;
		}

		TexturePtr renderer::createEmptyTexture(unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat)
		{
			TexturePtr tex;

			// calculate square size big enough for whole memory buffer
			unsigned int tex_size = buffWidth > buffHeight ? buffWidth : buffHeight;

			// create a texture
			// TODO: Check resulting pixel format and react appropriately.
			resource::format pixfmt;
			switch (pixFormat)
			{
			case Texture::PF_RGB:
				pixfmt = resource::r8g8b8;
				break;
			case Texture::PF_RGBA:
				pixfmt = resource::a8b8g8r8;
				break;
			default:
				throw std::exception("Failed to load texture from memory: Invalid pixelformat.");
				break;
			}

			rgde::render::texture_ptr platform_texture = rgde::render::texture::create(
				m_device, 
				tex_size, 
				tex_size, 
				1, 
				pixfmt
				);
			
			if (!platform_texture)
			{
				throw std::exception("Failed to load texture from memory: D3D Texture creation failed.");
			}
			else
			{		
				tex.reset(new texture(*this, platform_texture));
				m_texmanager.pushTexture(tex);
			}

			return tex;
		}

		TexturePtr	renderer::reloadTextureFromBuffer(TexturePtr p, const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat)
		{
			texture_ptr platform_tex = static_cast<texture&>(*p).get_platform_resource();

			surface_ptr surface = platform_tex->get_surface(0);
			surface::lock_data ld;
			bool res = surface->lock(ld);

			if (!res)
			{
				platform_tex.reset();
				throw std::exception("Failed to load texture from memory: IDirect3DTexture9::LockRect failed.");
			}
			else
			{
				// copy data from buffer into texture
				unsigned long* dst = (unsigned long*)ld.bytes;
				unsigned long* src = (unsigned long*)buffPtr;

				// RGBA
				if (pixFormat == Texture::PF_RGBA)
				{
					for (unsigned int i = 0; i < buffHeight; ++i)
					{
						for (unsigned int j = 0; j < buffWidth; ++j)
						{
							//TODO: check for endian safety on non-MS platforms
							unsigned char r = (unsigned char)(src[j] & 0xFF);
							unsigned char g = (unsigned char)((src[j] >> 8) & 0xFF);
							unsigned char b = (unsigned char)((src[j] >> 16)  & 0xFF);
							unsigned char a = (unsigned char)((src[j] >> 24) & 0xFF);

							dst[j] = rgde::math::color(a, r, g, b).data;
						}

						dst += ld.pitch / sizeof(unsigned long);
						src += buffWidth;
					}
				}
				// RGB
				else
				{
					for (unsigned int i = 0; i < buffHeight; ++i)
					{
						for (unsigned int j = 0; j < buffWidth; ++j)
						{
							dst[j] = src[j];
						}

						dst += ld.pitch / sizeof(unsigned long);
						src += buffWidth;
					}
				}

				surface->unlock();
			}
			return p;
		}

		TexturePtr renderer::loadFromMemory(const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat)
		{
			//using namespace std;
			TexturePtr tex;

			// calculate square size big enough for whole memory buffer
			unsigned int tex_size = buffWidth > buffHeight ? buffWidth : buffHeight;

			// create a texture
			// TODO: Check resulting pixel format and react appropriately.
			resource::format pixfmt;
			switch (pixFormat)
			{
			case Texture::PF_RGB:
				pixfmt = resource::r8g8b8;
				break;
			case Texture::PF_RGBA:
				pixfmt = resource::a8b8g8r8;
				break;
			default:
				throw std::exception("Failed to load texture from memory: Invalid pixelformat.");
				break;
			}

			texture_ptr platform_tex = rgde::render::texture::create(
				m_device, 
				tex_size, 
				tex_size, 
				1, 
				pixfmt);

			if (!platform_tex)
			{
				throw std::exception("Failed to load texture from memory: D3D Texture creation failed.");
			}
			else
			{
				// lock the texture
				surface_ptr surface = platform_tex->get_surface(0);
				rgde::render::surface::lock_data ld;
				bool res = surface->lock(ld);

				if (!res)
				{
					platform_tex.reset();
					throw std::exception("Failed to load texture from memory: IDirect3DTexture9::LockRect failed.");
				}
				else
				{
					// copy data from buffer into texture
					unsigned long* dst = (unsigned long*)ld.bytes;
					unsigned long* src = (unsigned long*)buffPtr;

					// RGBA
					if (pixFormat == Texture::PF_RGBA)
					{
						for (unsigned int i = 0; i < buffHeight; ++i)
						{
							for (unsigned int j = 0; j < buffWidth; ++j)
							{
								// we dont need endian safety on microsoft
								unsigned char r = (unsigned char)(src[j] & 0xFF);
								unsigned char g = (unsigned char)((src[j] >> 8) & 0xFF);
								unsigned char b = (unsigned char)((src[j] >> 16)  & 0xFF);
								unsigned char a = (unsigned char)((src[j] >> 24) & 0xFF);

								dst[j] = rgde::math::color(a, r, g, b).data;
							}

							dst += ld.pitch / sizeof(unsigned long);
							src += buffWidth;
						}
					}
					// RGB
					else
					{
						for (unsigned int i = 0; i < buffHeight; ++i)
						{
							for (unsigned int j = 0; j < buffWidth; ++j)
							{
								dst[j] = src[j];
							}

							dst += ld.pitch / sizeof(unsigned long);
							src += buffWidth;
						}
					}

					surface->unlock();
				}

				tex.reset(new texture(*this, platform_tex));
				m_texmanager.pushTexture(tex);
			}

			return tex;
		}

		FontPtr	renderer::createFont(const std::string& name, const std::string& filename, unsigned int size)
		{
			return FontPtr(new FreeTypeFont(name, filename, size, *this));
		}

		void renderer::drawFromCache( BaseWindow* window )
		{
			assert(window);
			QuadCacheMap::iterator i = m_mapQuadList.find(window);
			if (i == m_mapQuadList.end()) return;
			QuadCacheRecord& v = i->second;

			for (std::size_t a = 0; a < v.num; ++a)
			{
				if (m_num_quads >= m_quads.size())
					m_quads.resize(m_num_quads * 2);
				assert(v.m_vec.size() > a);

				m_quads[m_num_quads] = v.m_vec[a];
				if (!m_num_quads  || m_quads[m_num_quads - 1].texture != m_quads[m_num_quads].texture ||
					m_needToAddCallback ||
					(m_num_batches && (m_num_quads - m_batches[m_num_batches - 1].startQuad + 1)*VERTEX_PER_QUAD >= VERTEXBUFFER_CAPACITY))
				{
					// закончим предыдущий батч если он есть
					if (m_num_batches)
					{
						m_batches[m_num_batches - 1].numQuads = m_num_quads - m_batches[m_num_batches - 1].startQuad;
						if (!m_needToAddCallback)
						{
							m_callbackInfo.window = NULL;
							m_callbackInfo.afterRenderCallback = NULL;
						}
						m_needToAddCallback = false;
						m_batches[m_num_batches - 1].callbackInfo = m_callbackInfo;
					}
					// начнем следующий батч
					m_batches[m_num_batches].texture = m_quads[m_num_quads].texture;
					m_batches[m_num_batches].startQuad = m_num_quads;

					++m_num_batches;
				}

				++m_num_quads;
			}
		}

		void renderer::beginBatching()
		{
			m_needToAddCallback = false;
			Renderer::beginBatching();
		}

		void renderer::endBatching()
		{
			if (!m_num_batches) return;
			Renderer::endBatching();
			if (!m_needToAddCallback)
			{
				m_callbackInfo.window = NULL;
				m_callbackInfo.afterRenderCallback = NULL;
			}
			m_needToAddCallback = false;
			m_batches[m_num_batches - 1].callbackInfo = m_callbackInfo;
		}
	}
}
