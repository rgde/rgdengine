#include "precompiled.h"

#include <rgde/math/types3d.h>

#include <rgde/io/io.h>

#include "texture_impl.h"
#include "../base/exception.h"

#include <rgde/base/manager.h>

extern IDirect3DDevice9 *g_d3d;


namespace render
{
	typedef ::base::resource_manager<std::string, texture> texture_manager;
	texture_manager manager(boost::bind(&texture_d3d9::create_from_file, _1), true, "default.jpg");


	texture_ptr texture::create(const std::string& filename)
	{
		return manager.get(filename);
	}

	//D3DUSAGE_AUTOGENMIPMAP during 
	//IDirect3DDevice9::CreateTexture, 
	//IDirect3DDevice9::CreateCubeTexture, 
	//IDirect3DDevice9::CreateVolumeTexture
	//For more information about usage constants, see D3DUSAGE.
	void texture_d3d9::create_render_texture(const math::Vec2i &size, texture_format format)
	{
		m_usage = RenderTarget;

		if (format >= D16_LOCKABLE && format <= D24FS8)
			m_usage = DepthStencil;

		V(g_d3d->CreateTexture(size[0],			// Width
			size[1],			// Height
			1,					// Levels
			(DWORD)m_usage,	// Usage
			(D3DFORMAT)format,	// Format
			D3DPOOL_DEFAULT,	// Pool
			&m_texture,		// Texture pointer
			0					// Reserved
		));

		V(m_texture->GetLevelDesc(0, &m_desc));

		m_height = size[1];
		m_width = size[0];
		m_format = format;
	}

	io::readstream_ptr open_texture_file(const std::string& filename)
	{
		io::file_system &fs	= io::file_system::get();
		if (io::readstream_ptr in	= fs.find(filename))
			return in;

		{
			std::string file = io::helpers::get_shot_filename(filename);
			if (io::readstream_ptr in	= fs.find(file + ".jpg"))
				return in;
		}

		{
			io::path_add_scoped p	("Textures/");
			if (io::readstream_ptr in	= fs.find(filename))
				return in;
		}

		{
			io::scope_path p	("Media/Common/Textures/");
			if (io::readstream_ptr in = fs.find(filename))
				return in;
		}

		return io::readstream_ptr();
	}

	texture_ptr texture_d3d9::create_from_file(const std::string& filename)
	{
		return texture_ptr(new texture_d3d9(filename));
	}

	void texture_d3d9::createTextureFromFile(const std::string& filename)
	{	
		if (NULL == g_d3d) return;

		std::vector<byte> data;
		io::readstream_ptr in = open_texture_file(filename);

		if (!in)
		{
			base::lerr << "Can't load texture!" << "\"" << filename << "\"";
			return; //false
		}

		unsigned int size	= in->size();
		io::stream_to_vector(data, in);

		m_usage = DefaultUsage;

		SAFE_RELEASE(m_texture);

		{
			V(D3DXCreateTextureFromFileInMemoryEx(g_d3d, (const void*)&(data[0]), (uint)data.size(), D3DX_DEFAULT,	// from file
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

		m_height = m_desc.Height;
		m_width = m_desc.Width;
		m_format = (texture_format)m_desc.Format;
		m_type = convet_type(m_texture->GetType());
		if (m_type == Unknown)
			base::lwrn << "Warning: texture \"" << filename << "\" has unknown type";
	}

	texture_d3d9::texture_d3d9(const std::string& filename)
		: m_texture(0),
		m_filename(filename)
	{
		base::lmsg << "loading texture:  " << m_filename.c_str() << "";
		createTextureFromFile(m_filename);
	}

	IDirect3DTexture9 * texture_d3d9::get_dx_texture()
	{
		return m_texture;
	}

	texture_d3d9::~texture_d3d9()
	{
		//std::string base_name = std::string("Media\\")+m_texName;
		//base::lmsg << "deleting texture.";
		SAFE_RELEASE(m_texture);
	}

	texture_type texture_d3d9::convet_type(D3DRESOURCETYPE d3dType)
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