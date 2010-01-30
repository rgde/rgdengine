#include "StdAfx.h"
#include "image.h"
#include "imageset.h"
#include "renderer.h"
#include "cstaticimage.h"

#include "system.h"

namespace gui
{

StaticImage::StaticImage(System& sys, const std::string& name) :
	BaseWindow(sys, name),
	m_img(0),
	m_vformat(Image::Stretch),
	m_hformat(Image::Stretch)
{
}

StaticImage::~StaticImage(void)
{
}

void StaticImage::setImageset(const std::string& set)
{
	m_imgset = m_system.getWindowManager().loadImageset(set);
	invalidate();
}

const std::string StaticImage::getImageset() const
{
	if(m_imgset)
		return m_imgset->getName();

	return std::string();
}

void StaticImage::setImage(const std::string& image)
{
	if(m_imgset && !image.empty())
	{
		const Imageset& set = *m_imgset;
		m_img = set[image];
	}
	else
		m_img = 0;

	invalidate();
}

const std::string StaticImage::getImage() const
{
	if(m_img)
		return m_img->getName();

	return std::string();
}

void StaticImage::render(const Rect& finalRect, const Rect& finalClip)
{
	Renderer& r = m_system.getRenderer();

	if (m_img)
    {
        r.draw(*m_img, finalRect, 1.f, finalClip,  m_backColor, m_hformat, m_vformat);
    }
}

void StaticImage::init(xml::node& node)
{
	BaseWindow::init(node);

	xml::node setting = node("Imageset");
	if(!setting.empty())
	{
		setImageset(setting.first_child().value());
	}

	setting = node("VertFormatting");
	if(!setting.empty())
	{
		m_vformat = StringToImageOps(setting.first_child().value());
	}
	
	setting = node("HorzFormatting");
	if(!setting.empty())
	{
		m_hformat = StringToImageOps(setting.first_child().value());
	}

	setting = node("Image");
	if(!setting.empty())
	{
		setImage(setting.first_child().value());
	}
}

}