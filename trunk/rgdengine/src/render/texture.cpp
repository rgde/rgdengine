#include "precompiled.h"

#include <rgde/math/types3d.h>

#include <rgde/io/io.h>

#include "texture_impl.h"
#include "../base/exception.h"

#include <rgde/base/manager.h>

extern IDirect3DDevice9 *g_pd3dDevice;


namespace render
{
	typedef ::base::TResourceManager<std::string, ITexture> TextureManager;
	TextureManager manager(boost::bind(&TextureImpl::CreateFromFile, _1), true, "default.jpg");


	PTexture ITexture::Create(const std::string& file_name)
	{
		return manager.get(file_name);
	}

	//D3DUSAGE_AUTOGENMIPMAP during 
	//IDirect3DDevice9::CreateTexture, 
	//IDirect3DDevice9::CreateCubeTexture, 
	//IDirect3DDevice9::CreateVolumeTexture
	//For more information about usage constants, see D3DUSAGE.
	void TextureImpl::createRenderTexture(const math::Vec2i &size, TextureFormat format)
	{
		m_eUsage = RenderTarget;

		if (format >= D16_LOCKABLE && format <= D24FS8)
			m_eUsage = DepthStencil;

		V(g_pd3dDevice->CreateTexture(size[0],			// Width
			size[1],			// Height
			1,					// Levels
			(DWORD)m_eUsage,	// Usage
			(D3DFORMAT)format,	// Format
			D3DPOOL_DEFAULT,	// Pool
			&m_texture,		// Texture pointer
			0					// Reserved
		));

		V(m_texture->GetLevelDesc(0, &m_desc));

		m_nHeight = size[1];
		m_nWidth = size[0];
		m_eFormat = format;
	}

	io::PReadStream findTexture(const std::string& file_name)
	{
		io::CFileSystem &fs	= io::TheFileSystem::Get();
		if (io::PReadStream in	= fs.findFile(file_name))
			return in;

		{
			std::string file = io::helpers::getFileNameWithoutExtension(file_name);
			if (io::PReadStream in	= fs.findFile(file + ".jpg"))
				return in;
		}

		{
			io::ScopePathAdd p	("Textures/");
			if (io::PReadStream in	= fs.findFile(file_name))
				return in;
		}

		{
			io::ScopePathChange p	("Media/Common/Textures/");
			if (io::PReadStream in = fs.findFile(file_name))
				return in;
		}

		return io::PReadStream();
	}

	PTexture TextureImpl::CreateFromFile(const std::string& strFileName)
	{
		return PTexture(new TextureImpl(strFileName));
	}

	void TextureImpl::createTextureFromFile(const std::string& strFileName)
	{	
		if (NULL == g_pd3dDevice) return;

		std::vector<byte> data;
		io::PReadStream in = findTexture(strFileName);

		if (!in)
		{
			base::lerr << "Не могу загрузить текстуру!" << "\"" << strFileName << "\"";
			return; //false
		}

		unsigned int size	= in->getSize();
		io::StreamToVector(data, in);

		m_eUsage = DefaultUsage;

		SAFE_RELEASE(m_texture);

		{
			V(D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, (const void*)&(data[0]), (uint)data.size(), D3DX_DEFAULT,	// from file
				D3DX_DEFAULT,	// from file
				D3DX_DEFAULT,	// compete mipmap chain
				0,				//DWORD Usage,
				D3DFMT_UNKNOWN, //D3DFORMAT Format,
				D3DPOOL_MANAGED,//D3DPOOL Pool,
				D3DX_DEFAULT,	//DWORD Filter,
				D3DX_DEFAULT,	//D3DX_DEFAULT,	//DWORD MipFilter,
				0,				//D3DCOLOR ColorKey,
				NULL,			//D3DXIMAGE_INFO *pSrcInfo,
				NULL,			//PALETTEENTRY *pPalette,
				&m_texture		//LPDIRECT3DTEXTURE9 *ppTexture
			)) ;
		}

		V(m_texture->GetLevelDesc(0, &m_desc));

		m_nHeight = m_desc.Height;
		m_nWidth = m_desc.Width;
		m_eFormat = (TextureFormat)m_desc.Format;
		m_eType = convertType(m_texture->GetType());
		if (m_eType == Unknown)
			base::lwrn << "Warning: texture \"" << strFileName << "\" has unknown type";
	}

	TextureImpl::TextureImpl(const std::string& file_name)
		: m_texture(0),
		m_strFileName(file_name)
	{
		base::lmsg << "loading texture:  " << m_strFileName.c_str() << "";
		createTextureFromFile(m_strFileName);
	}

	IDirect3DTexture9 * TextureImpl::getDxTexture()
	{
		return m_texture;
	}

	TextureImpl::~TextureImpl()
	{
		//std::string base_name = std::string("Media\\")+m_texName;
		//base::lmsg << "deleting texture.";
		SAFE_RELEASE(m_texture);
	}

	ETextureType TextureImpl::convertType(D3DRESOURCETYPE d3dType)
	{
		switch (d3dType)
		{
		case D3DRTYPE_TEXTURE:
			return Texture;
			break;

		case D3DRTYPE_VOLUMETEXTURE:
			return VolumeTexture;
			break;

		case D3DRTYPE_CUBETEXTURE:
			return CubeTexture;
			break;

		default:
			return Unknown;
			break;
		};
	}
}