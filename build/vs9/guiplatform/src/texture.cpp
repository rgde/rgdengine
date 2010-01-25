#include "stdafx.h"
#include "texture.h"
#include "renderer.h"

namespace gui
{
namespace rgde_platform
{


/*************************************************************************
	Constructor
*************************************************************************/
texture::texture(renderer& owner, rgde::render::texture_ptr tex) :
	Texture(owner),
	m_texture(tex)
{
	calculateMetrics();
}

/*************************************************************************
	Destructor
*************************************************************************/
texture::~texture()
{
	//m_owner.
	free_platform_resource();
}

void texture::calculateMetrics()
{
	m_size.m_width = 0;
	m_size.m_height = 0;

	assert(m_texture);

	m_size.m_width = m_texture->get_width();
	m_size.m_height = m_texture->get_height();

	rgde::render::surface_ptr surface = m_texture->get_surface(0);

	if (!surface)
		throw std::exception("Failed to load texture from memory: Invalid texture.");

	//enum format
	//{
	//	unknown = 0,
	//	r8g8b8 = 20,
	//	a8r8g8b8 = 21,
	//	x8r8g8b8 = 22,

	//	dxt1 = MKFRCC('D','X','T','1'),
	//	dxt2 = MKFRCC('D','X','T','2'),
	//	dxt3 = MKFRCC('D','X','T','3'),
	//	dxt4 = MKFRCC('D','X','T','4'),
	//	dxt5 = MKFRCC('D','X','T','5'),

	//	d16_locable = 70,
	//	d32 = 71,
	//	d24s8 = 75,
	//	d24x8 = 77,
	//	d16 = 80,

	//	vertex_data,
	//	index16,
	//	index32,

	//	// -- Floating point surface formats --
	//	// s10e5 formats (16-bits per channel)
	//	r16f = 111,
	//	g16r16f = 113,
	//	a16b16g16r16f = 113,

	//	// IEEE s23e8 formats (32-bits per channel)
	//	r32f = 114,
	//	g32r32f = 115,
	//	a32b32g32r32f = 116
	//};

	using rgde::render::resource;

	switch (surface->get_format())
	{
	case resource::r8g8b8:
		m_format = Texture::PF_RGB;
	    break;
	case resource::x8r8g8b8:
	case resource::a8r8g8b8:
		m_format = Texture::PF_RGBA;
	    break;
	case resource::dxt1:
	case resource::dxt2:
	case resource::dxt3:
	case resource::dxt4:
	case resource::dxt5:
		m_format = Texture::PF_COMPRESSED;
		break;

	default:
		throw std::exception("Failed to load texture from memory: Invalid pixelformat.");
		break;
	}
}

void texture::free_platform_resource()
{
	m_texture.reset();
}

void texture::onDeviceLost()
{
}

void texture::onDeviceReset()
{
}

}
}