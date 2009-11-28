#pragma once

#include <d3dx9.h>
#include <rgde/render/texture.h>

namespace render
{
	class texture_d3d9 : public texture
	{
	public:
							texture_d3d9(const std::string& file_name);
							~texture_d3d9();

		static texture_ptr		create_from_file(const std::string& file_name);

		void				createTextureFromFile(const std::string& file_name);
		void				create_render_texture(const math::Vec2i &size, texture_format format);

		IDirect3DTexture9*	get_dx_texture();

		texture_format		get_format()		const { return m_eFormat; }
		texture_usage		get_usage()		const { return m_eUsage;  }
		int					get_height()		const { return m_nHeight; }
		int					get_width()		const { return m_nWidth;  }
		texture_type		get_type()		const { return m_type;   }

		const std::string& get_filename() const {return m_strFileName;}

		bool				has_alpha()		const
		{
			return m_eFormat == A32B32G32R32F || m_eFormat == A16B16G16R16F || m_eFormat == ARGB8 || m_eFormat == A16B16G16R16 || m_eFormat == A8B8G8R8 || m_eFormat == A8R8G8B8 || m_eFormat == DXT3;
		}

	protected:
		texture_d3d9() {}
		texture_type	convet_type(D3DRESOURCETYPE d3dType);
		
	protected:
		IDirect3DTexture9	*m_texture;	
		D3DSURFACE_DESC		m_desc;
		std::string			m_strFileName;

		texture_usage		m_eUsage;
		unsigned			m_nHeight;
		unsigned			m_nWidth;
		texture_format		m_eFormat;
		texture_type		m_type;
	};
}