#include "stdafx.h"
#include "d3d9texture.h"
#include "d3d9renderer.h"

namespace gui
{
namespace win32dx9platform
{


/*************************************************************************
	Constructor
*************************************************************************/
DirectX9Texture::DirectX9Texture(DirectX9Renderer& owner, IDirect3DTexture9* tex) :
	Texture(owner),
	m_d3dtexture(tex)
{
	calculateMetrics();
}

/*************************************************************************
	Destructor
*************************************************************************/
DirectX9Texture::~DirectX9Texture()
{
	//m_owner.
	freeD3DTexture();
}

void DirectX9Texture::calculateMetrics()
{
	m_size.m_width = 0;
	m_size.m_height = 0;

	D3DSURFACE_DESC surfDesc;
	HRESULT hr = m_d3dtexture->GetLevelDesc(0, &surfDesc);
	if (SUCCEEDED(hr))
	{
		m_size.m_width = (float)surfDesc.Width;
		m_size.m_height = (float)surfDesc.Height;
	}

	switch (surfDesc.Format)
	{
	case D3DFMT_R8G8B8:
		m_format = Texture::PF_RGB;
	    break;
	case D3DFMT_X8R8G8B8:
	case D3DFMT_A8B8G8R8:
	case D3DFMT_A8R8G8B8:
		m_format = Texture::PF_RGBA;
	    break;
	case D3DFMT_DXT1:
	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
		m_format = Texture::PF_COMPRESSED;
		break;

	default:
		throw std::exception("Failed to load texture from memory: Invalid pixelformat.");
		break;
	}
}

void DirectX9Texture::freeD3DTexture(void)
{
	if (m_d3dtexture)
	{
		m_d3dtexture->Release();		
	}
	m_d3dtexture = NULL;
}

void DirectX9Texture::onDeviceLost(void)
{
}

void DirectX9Texture::onDeviceReset(void)
{
}

}
}