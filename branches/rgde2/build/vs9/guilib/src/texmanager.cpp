#include "stdafx.h"

#include "texmanager.h"
#include "renderer.h"

namespace gui
{

TextureManager::TextureManager(Renderer& render) :
	m_render(render)
{
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::createTexture(const std::string& filename)
{	
	TexturesIter i = m_textures.find(filename);
	if(i == m_textures.end())
	{
		TexturePtr tex = m_render.createTextureInstance(filename);
		TextureWeakPtr weak_tex = tex;
		m_textures.insert(std::make_pair(filename, weak_tex));
		return tex;
	}

	TextureWeakPtr weak_tex = i->second;
	if (weak_tex.expired())
	{
		TexturePtr tex = m_render.createTextureInstance(filename);
		weak_tex = tex;
		return tex;
	}

	return weak_tex.lock();
}

void TextureManager::pushTexture(TexturePtr tex)
{
	//TextureListIter i = std::find(m_textures.begin(), m_textures.end(), tex);
	//if(i == m_textures.end())
	//{
	//	m_textures.push_back(tex);
	//}
}

void TextureManager::cleanup()
{
	m_textures.clear();
}

void TextureManager::onDeviceLost()
{
	//class devlost
	//{
	//public:
	//	void operator ()(TexturePtr& ptr)
	//	{
	//		ptr->onDeviceLost();
	//	}
	//};
	//devlost d;
	//std::for_each(m_textures.begin(), m_textures.end(), d);
	for(TexturesIter it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		TextureWeakPtr weak_tex = it->second;
		if (!weak_tex.expired())
		{
			TexturePtr t = weak_tex.lock();
			t->onDeviceLost();
		}
	}
}

void TextureManager::onDeviceReset()
{
	//class devreset
	//{
	//public:
	//	void operator ()(TexturePtr& ptr)
	//	{
	//		ptr->onDeviceReset();
	//	}
	//};
	//devreset d;
	//std::for_each(m_textures.begin(), m_textures.end(), d);

	for(TexturesIter it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		TextureWeakPtr weak_tex = it->second;
		if (!weak_tex.expired())
		{
			TexturePtr t = weak_tex.lock();
			t->onDeviceReset();
		}
	}
}

}