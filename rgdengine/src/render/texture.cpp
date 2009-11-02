#include "precompiled.h"

#include <rgde/math/types3d.h>

#include <rgde/io/io.h>

#include "texture_impl.h"
#include "../base/exception.h"

#include <rgde/base/manager.h>

extern IDirect3DDevice9 *g_pd3dDevice;


namespace render
{
	typedef ::base::resource_manager<std::string, texture> texture_manager;
	texture_manager manager(boost::bind(&texture_d3d9::create_from_file, _1), true, "default.jpg");


	texture_ptr texture::create(const std::string& file_name)
	{
		return manager.get(file_name);
	}

	//D3DUSAGE_AUTOGENMIPMAP during 
	//IDirect3DDevice9::CreateTexture, 
	//IDirect3DDevice9::CreateCubeTexture, 
	//IDirect3DDevice9::CreateVolumeTexture
	//For more information about usage constants, see D3DUSAGE.
	void texture_d3d9::createRenderTexture(const math::Vec2i &size, TextureFormat format)
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

	io::readstream_ptr findTexture(const std::string& file_name)
	{
		io::CFileSystem &fs	= io::TheFileSystem::get();
		if (io::readstream_ptr in	= fs.find(file_name))
			return in;

		{
			std::string file = io::helpers::get_shot_filename(file_name);
			if (io::readstream_ptr in	= fs.find(file + ".jpg"))
				return in;
		}

		{
			io::path_add_scoped p	("Textures/");
			if (io::readstream_ptr in	= fs.find(file_name))
				return in;
		}

		{
			io::ScopePathChange p	("Media/Common/Textures/");
			if (io::readstream_ptr in = fs.find(file_name))
				return in;
		}

		return io::readstream_ptr();
	}

	texture_ptr texture_d3d9::create_from_file(const std::string& file_name)
	{
		return texture_ptr(new texture_d3d9(file_name));
	}

	void texture_d3d9::createTextureFromFile(const std::string& file_name)
	{	
		if (NULL == g_pd3dDevice) return;

		std::vector<byte> data;
		io::readstream_ptr in = findTexture(file_name);

		if (!in)
		{
			base::lerr << "Can't load texture!" << "\"" << file_name << "\"";
			return; //false
		}

		unsigned int size	= in->get_size();
		io::stream_to_vector(data, in);

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
		m_type = convertType(m_texture->GetType());
		if (m_type == Unknown)
			base::lwrn << "Warning: texture \"" << file_name << "\" has unknown type";
	}

	texture_d3d9::texture_d3d9(const std::string& file_name)
		: m_texture(0),
		m_strFileName(file_name)
	{
		base::lmsg << "loading texture:  " << m_strFileName.c_str() << "";
		createTextureFromFile(m_strFileName);
	}

	IDirect3DTexture9 * texture_d3d9::getDxTexture()
	{
		return m_texture;
	}

	texture_d3d9::~texture_d3d9()
	{
		//std::string base_name = std::string("Media\\")+m_texName;
		//base::lmsg << "deleting texture.";
		SAFE_RELEASE(m_texture);
	}

	ETextureType texture_d3d9::convertType(D3DRESOURCETYPE d3dType)
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