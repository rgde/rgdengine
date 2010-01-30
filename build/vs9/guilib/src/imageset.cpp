#include "stdafx.h"

#include "imageset.h"

namespace gui
{
	Imageset::Imageset(const std::string& name, TexturePtr tex) :
	m_name(name),
	m_texture(tex)
	{
	}

	Imageset::Imageset(const std::string& filename, Renderer& render)
	{
		load(filename, render);
	}

	Imageset::~Imageset(void)
	{
	}

	/*************************************************************************
	load Imageset data from the specified file
	*************************************************************************/
	void Imageset::load(const std::string& filename, Renderer& render)
	{
		assert(false && "Imageset::load");
	}


	const Image* Imageset::getImage(const std::string& name) const
	{
		if(name.empty())
			return 0;

		ImagesCIter pos = m_images.find(name);

		if (pos != m_images.end())
		{
			return &(pos->second);
		}
		return 0; 
	}


	/*************************************************************************
	defines a new Image.
	*************************************************************************/
	const Image* Imageset::defineImage(const std::string& name, const Rect& image_rect)
	{
		ImagesIter i = m_images.find(name);
		if(i == m_images.end())
		{
			i = m_images.insert(i, ImagePair(name,Image(*m_texture, name, image_rect, Image::BLEND_MODULATE, this)));
		}
		return &i->second;
	}


	void Imageset::undefineImage(const std::string& name)
	{
		m_images.erase(name);
	}


	void Imageset::undefineAllImages(void)
	{
		m_images.clear();
	}

}