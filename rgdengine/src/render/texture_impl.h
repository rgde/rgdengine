#pragma once

#include <d3dx9.h>
#include <rgde/render/texture.h>

namespace render
{
	class texture_d3d9 : public texture
	{
	public:
							texture_d3d9(const std::string& filename);
							~texture_d3d9();

		static texture_ptr		create_from_file(const std::string& filename);

		void				createTextureFromFile(const std::string& filename);
		void				create_render_texture(const math::Vec2i &size, texture_format format);

		IDirect3DTexture9*	get_dx_texture();

		texture_format		get_format()	const { return m_format; }
		texture_usage		get_usage()		const { return m_usage;  }
		int					get_height()	const { return m_height; }
		int					width()		const { return m_width;  }
		texture_type		type()		const { return m_type;   }

		const std::string& get_filename() const {return m_filename;}

		bool				has_alpha()		const
		{
			return m_format == A32B32G32R32F || m_format == A16B16G16R16F || m_format == ARGB8 || m_format == A16B16G16R16 || m_format == A8B8G8R8 || m_format == A8R8G8B8 || m_format == DXT3;
		}

	protected:
		texture_d3d9() {}
		texture_type	convet_type(D3DRESOURCETYPE d3dType);
		
	protected:
		IDirect3DTexture9	*m_texture;	
		D3DSURFACE_DESC		m_desc;
		std::string			m_filename;

		texture_usage		m_usage;
		unsigned			m_height;
		unsigned			m_width;
		texture_format		m_format;
		texture_type		m_type;
	};
}