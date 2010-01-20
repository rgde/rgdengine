#include "stdafx.h"

#include "d3d9renderer.h"
#include "d3d9texture.h"
#include "d3d9shader.h"
#include "ftfont.h"

// все правильно. макрос. да именно так!
#define PixelAligned(x)	( ( (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) ) - 0.5f )

namespace gui
{
	namespace win32dx9platform
	{

		Renderer* CreateRenderer(IDirect3DDevice9* dev, unsigned buff)
		{
			return new DirectX9Renderer(dev, 1024);
		}
		
		
		struct QuadVertex
		{
			float x, y, tu1, tv1;		//!< The transformed position for the vertex.
			DWORD diffuse;			//!< colour of the vertex
		};

	
		D3DVERTEXELEMENT9 VERTEX_DECL[] =
		{
			{ 0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0}, 
			{ 0,  16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_COLOR, 0}, 
			D3DDECL_END()
		};

		const int			DirectX9Renderer::VERTEX_PER_QUAD = 6;
		const int			DirectX9Renderer::VERTEX_PER_TRIANGLE = 3;
		const int			DirectX9Renderer::VERTEXBUFFER_CAPACITY	= 8192;

		const unsigned long	DirectX9Renderer::VERTEX_FVF = (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);

		/*************************************************************************
		Constructor
		*************************************************************************/
		DirectX9Renderer::DirectX9Renderer(LPDIRECT3DDEVICE9 device, unsigned int max_quads)
		{
			m_device = device;
			m_needToAddCallback = false;
			Size size(getViewportSize());

			constructor_impl(device, size);
		}


		/*************************************************************************
		method to do work of constructor
		*************************************************************************/
		void DirectX9Renderer::constructor_impl(LPDIRECT3DDEVICE9 device, const Size& display_size)
		{
			m_device        = device;

			m_currTexture   = 0;
			m_buffer        = 0;
			m_bufferPos     = 0;

			m_originalsize = display_size;

			// Create a vertex buffer
			if (FAILED(m_device->CreateVertexBuffer(
				(VERTEXBUFFER_CAPACITY * sizeof(QuadVertex)), 
				D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY, 
				0, 
				D3DPOOL_DEFAULT, 
				&m_buffer,
				0)))
			{
				// Ideally, this would have been a RendererException, but we can't use that until the System object is created
				// and that requires a Renderer passed to the constructor, so we throw this instead.
				throw std::exception("Creation of VertexBuffer for Renderer object failed");
			}

			char s[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH,s);
			m_device->CreateVertexDeclaration(VERTEX_DECL, &m_vertexDeclaration);

			m_shader.reset(new D3D9ShaderEffect(m_device, L"gui\\shaders\\gui.fx"));

			// get the maximum available texture size.
			D3DCAPS9	devCaps;
			if (FAILED(device->GetDeviceCaps(&devCaps)))
			{
				// release vertex buffer
				m_buffer->Release();
			}

			// set max texture size the the smaller of max width and max height.
			m_maxTextureSize = devCaps.MaxTextureWidth < devCaps.MaxTextureHeight ? devCaps.MaxTextureWidth : devCaps.MaxTextureHeight;

			m_handleGuiTexture = m_shader->GetParameterByName(NULL, "guiTexture");
			m_handleViewPortSize = m_shader->GetParameterByName(NULL, "ViewPortSize");

			m_device->AddRef();
		}


		/*************************************************************************
		Destructor
		*************************************************************************/
		DirectX9Renderer::~DirectX9Renderer(void)
		{
			if (m_vertexDeclaration)
			{
				m_vertexDeclaration->Release();
				m_vertexDeclaration = NULL;
			}
		
			if (m_buffer)
			{
				m_buffer->Release();
			}

			if (m_device)
			{
				m_device->Release();
			}
		}

		void DirectX9Renderer::addCallback( AfterRenderCallbackFunc callback,
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

		void DirectX9Renderer::addQuad(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode)
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

		void DirectX9Renderer::setRenderStates()
		{
			// setup vertex stream
			m_device->SetStreamSource(0, m_buffer, 0, sizeof(QuadVertex));
			D3DVIEWPORT9 viewPortDesc;
			m_device->GetViewport(&viewPortDesc);
			m_device->SetVertexDeclaration(m_vertexDeclaration);
			m_shader->SetTechnique("Simple");
			D3DXVECTOR2 vec((float)viewPortDesc.Width, (float)viewPortDesc.Height);
			m_shader->SetFloatArray(m_handleViewPortSize,vec, 2 );
			m_shader->Begin(D3DXSPRITE_DONOTSAVESTATE );
			m_shader->BeginPass(0);
		}
		/*************************************************************************
		perform final rendering for all queued renderable quads.
		*************************************************************************/
		void DirectX9Renderer::doRender(void)
		{

			if (!m_buffer)
				return;

			setRenderStates();
			m_currTexture = 0;

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

				if (FAILED(m_buffer->Lock(UINT(s_quadOffset * 6 * sizeof(QuadVertex)), 
					UINT(6 * (m_batches[b].numQuads)* sizeof(QuadVertex)),
					(void**)&buffmem, s_quadOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
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

				m_buffer->Unlock();
				LPDIRECT3DTEXTURE9 tex = static_cast<DirectX9Texture*>(batch.texture)->getD3DTexture();
				m_device->SetTexture(0, tex);

				m_device->DrawPrimitive(D3DPT_TRIANGLELIST, s_quadOffset * 6, UINT(numQ * 2));
				s_quadOffset += (DWORD)numQ;


				if (batch.callbackInfo.window && batch.callbackInfo.afterRenderCallback)
				{
					m_shader->EndPass();
					m_shader->End();

					batch.callbackInfo.afterRenderCallback(batch.callbackInfo.window,
									batch.callbackInfo.dest, batch.callbackInfo.clip);

					// если это был не последний батч
					if (b < m_num_batches -1)
						setRenderStates();
				}
			}
			
			m_shader->EndPass();
			m_shader->End();
			

		}

		/*************************************************************************
		setup states etc
		*************************************************************************/
		void DirectX9Renderer::initPerFrameStates(void)
		{
			// setup vertex stream
			m_device->SetStreamSource(0, m_buffer, 0, sizeof(QuadVertex));
			m_device->SetFVF(VERTEX_FVF);
			m_device->SetVertexShader( 0 );
			m_device->SetPixelShader( 0 );

			// set device states
			m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			m_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
			m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


			// setup texture addressing settings
			m_device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			m_device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			// setup colour calculations
			m_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			m_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

			// setup alpha calculations
			m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			// setup filtering
			m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

			// disable texture stages we do not need.
			m_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			// setup scene alpha blending
			m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}


		/*************************************************************************
		renders whatever is in the vertex buffer
		*************************************************************************/
		void DirectX9Renderer::renderVBuffer(void)
		{
			// if bufferPos is 0 there is no data in the buffer and nothing to render
			if (m_bufferPos == 0)
			{
				return;
			}

			// render the sprites
			HRESULT hr = m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, (m_bufferPos / VERTEX_PER_TRIANGLE));

			// reset buffer position to 0...
			m_bufferPos = 0;
		}

		/*************************************************************************
		render a quad directly to the display
		*************************************************************************/
		void DirectX9Renderer::renderQuadDirect(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode)
		{			
			if (!m_buffer)
				return;

			m_device->SetStreamSource(0, m_buffer, 0, sizeof(QuadVertex));
			D3DVIEWPORT9 viewPortDesc;
			m_device->GetViewport(&viewPortDesc);

			//m_device->SetFVF(VERTEX_FVF);
			m_device->SetVertexDeclaration(m_vertexDeclaration);

			m_shader->SetTechnique("Simple");
			D3DXVECTOR2 vec((float)viewPortDesc.Width, (float)viewPortDesc.Height);
			m_shader->SetFloatArray("ViewPortSize",vec, 2 );
			m_shader->Begin(D3DXSPRITE_DONOTSAVESTATE );
			m_shader->BeginPass(0);


			QuadVertex*	buffmem;
			
			DirectX9Texture* tex = static_cast<DirectX9Texture*>(&img.getTexture());
			m_device->SetTexture(0, ((DirectX9Texture*)tex)->getD3DTexture());

			if (SUCCEEDED(m_buffer->Lock(0, VERTEX_PER_QUAD * sizeof(QuadVertex), (void**)&buffmem, D3DLOCK_DISCARD)))
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

				m_buffer->Unlock();
				m_bufferPos = VERTEX_PER_QUAD;

				renderVBuffer();
			}
			m_shader->EndPass();
			m_shader->End();

		}


		void DirectX9Renderer::OnLostDevice(void)
		{
			if (m_shader)
				m_shader->OnLostDevice();
			if(m_buffer)
			{
				// release the buffer prior to the reset call (will be re-created later)
				if (FAILED(m_buffer->Release()))
				{
					throw std::exception("DirectX9Renderer::onDeviceLost - Failed to release the VertexBuffer used by the DirectX9Renderer object.");
				}

				m_buffer = 0;
			}

			m_texmanager.onDeviceLost();
		}

		void DirectX9Renderer::OnResetDevice(void)
		{
			if(!m_buffer)
			{
				// Recreate a vertex buffer
				if (FAILED(m_device->CreateVertexBuffer((VERTEXBUFFER_CAPACITY * sizeof(QuadVertex)), D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY, VERTEX_FVF, D3DPOOL_DEFAULT, &m_buffer, 0)))
				{
					throw std::exception("DirectX9Renderer::onDeviceReset - Failed to create the VertexBuffer for use by the DirectX9Renderer object.");
				}
			}
			if (m_shader)
				m_shader->OnResetDevice();

			m_texmanager.onDeviceReset();
		}

		Size DirectX9Renderer::getViewportSize(void) const
		{
			// initialise renderer size
			D3DVIEWPORT9	vp;

			if (FAILED(m_device->GetViewport(&vp)))
			{
				return Size(0.f, 0.f);
			}
			else
			{
				return Size((float)vp.Width, (float)vp.Height);
			}
		}

		TexturePtr	DirectX9Renderer::createTexture(const std::string& filename)
		{
			return m_texmanager.createTexture(filename);
		}

		TexturePtr	DirectX9Renderer::createTextureInstance(const std::string& filename) 
		{
			TexturePtr tex;
			if(!filename.empty())
			{
				IDirect3DTexture9* dx_tex = NULL;
				HRESULT hr = D3DXCreateTextureFromFileA(m_device, filename.c_str(), &dx_tex);
				if(!FAILED(hr))
				{
					tex.reset(new DirectX9Texture(*this, dx_tex));
				}
			}
			return tex;
		}
		void DirectX9Renderer::destroyTexture(TexturePtr tex)
		{
			throw std::exception("Not implemented yet!");
		}

		TexturePtr DirectX9Renderer::createEmptyTexture(unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat)
		{
			TexturePtr tex;

			// calculate square size big enough for whole memory buffer
			unsigned int tex_size = buffWidth > buffHeight ? buffWidth : buffHeight;

			// create a texture
			// TODO: Check resulting pixel format and react appropriately.
			D3DFORMAT pixfmt;
			switch (pixFormat)
			{
			case Texture::PF_RGB:
				pixfmt = D3DFMT_R8G8B8;
				break;
			case Texture::PF_RGBA:
				pixfmt = D3DFMT_A8B8G8R8;
				break;
			default:
				throw std::exception("Failed to load texture from memory: Invalid pixelformat.");
				break;
			}

			IDirect3DTexture9* dx_tex = NULL;
			HRESULT hr = D3DXCreateTexture(m_device, tex_size, tex_size, 1, 0, pixfmt, D3DPOOL_MANAGED, &dx_tex);

			if (FAILED(hr))
			{
				throw std::exception("Failed to load texture from memory: D3D Texture creation failed.");
			}
			else
			{		
				tex.reset(new DirectX9Texture(*this, dx_tex));
				m_texmanager.pushTexture(tex);
			}

			return tex;
		}

		TexturePtr	DirectX9Renderer::reloadTextureFromBuffer(TexturePtr p, const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat)
		{
			IDirect3DTexture9* dx_tex = static_cast<DirectX9Texture&>(*p).getD3DTexture();
			// lock the D3D texture
			D3DLOCKED_RECT	rect = {0};
			HRESULT hr = dx_tex->LockRect(0, &rect, 0, 0);

			if (FAILED(hr))
			{
				dx_tex->Release();
				dx_tex = 0;

				throw std::exception("Failed to load texture from memory: IDirect3DTexture9::LockRect failed.");
			}
			else
			{
				// copy data from buffer into texture
				unsigned long* dst = (unsigned long*)rect.pBits;
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

							dst[j] = D3DCOLOR_ARGB(a, r, g, b);
						}

						dst += rect.Pitch / sizeof(unsigned long);
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

						dst += rect.Pitch / sizeof(unsigned long);
						src += buffWidth;
					}
				}

				dx_tex->UnlockRect(0);
			}
			return p;
		}

		TexturePtr DirectX9Renderer::loadFromMemory(const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat)
		{
			//using namespace std;
			TexturePtr tex;

			// calculate square size big enough for whole memory buffer
			unsigned int tex_size = buffWidth > buffHeight ? buffWidth : buffHeight;

			// create a texture
			// TODO: Check resulting pixel format and react appropriately.
			D3DFORMAT pixfmt;
			switch (pixFormat)
			{
			case Texture::PF_RGB:
				pixfmt = D3DFMT_R8G8B8;
				break;
			case Texture::PF_RGBA:
				pixfmt = D3DFMT_A8B8G8R8;
				break;
			default:
				throw std::exception("Failed to load texture from memory: Invalid pixelformat.");
				break;
			}

			IDirect3DTexture9* dx_tex = NULL;
			HRESULT hr = D3DXCreateTexture(m_device, tex_size, tex_size, 1, 0, pixfmt, D3DPOOL_MANAGED, &dx_tex);

			if (!FAILED(hr))
			{
				throw std::exception("Failed to load texture from memory: D3D Texture creation failed.");
			}
			else
			{
				// lock the D3D texture
				D3DLOCKED_RECT	rect;
				hr = dx_tex->LockRect(0, &rect, 0, 0);

				if (FAILED(hr))
				{
					dx_tex->Release();
					dx_tex = 0;

					throw std::exception("Failed to load texture from memory: IDirect3DTexture9::LockRect failed.");
				}
				else
				{
					// copy data from buffer into texture
					unsigned long* dst = (unsigned long*)rect.pBits;
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

								dst[j] = D3DCOLOR_ARGB(a, r, g, b);
							}

							dst += rect.Pitch / sizeof(unsigned long);
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

							dst += rect.Pitch / sizeof(unsigned long);
							src += buffWidth;
						}
					}

					dx_tex->UnlockRect(0);
				}

				tex.reset(new DirectX9Texture(*this, dx_tex));
				m_texmanager.pushTexture(tex);
			}

			return tex;
		}

		FontPtr	DirectX9Renderer::createFont(const std::string& name, const std::string& filename, unsigned int size)
		{
			return FontPtr(new FreeTypeFont(name, filename, size, *this));
		}

		void DirectX9Renderer::drawFromCache( BaseWindow* window )
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

		void DirectX9Renderer::beginBatching()
		{
			m_needToAddCallback = false;
			Renderer::beginBatching();
		}

		void DirectX9Renderer::endBatching()
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
