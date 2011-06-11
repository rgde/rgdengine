#include "stdafx.h"
#include "texture.h"
#include "renderer.h"

namespace gui
{
namespace rgde_platform
{

texture::texture(renderer& owner, rgde::render::texture_ptr tex) :
	Texture(owner),
	m_texture(tex)
{
	calculateMetrics();
}

texture::~texture()
{
	free_platform_resource();
}

void texture::calculateMetrics()
{
	m_size.width = 0;
	m_size.height = 0;

	assert(m_texture);

	m_size.width = m_texture->get_width()*1.0f;
	m_size.height = m_texture->get_height()*1.0f;

	rgde::render::surface_ptr surface = m_texture->get_surface(0);

	if (!surface)
		throw std::exception("Failed to load texture from memory: Invalid texture.");
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