#pragma once

#include "texture.h"
#include <boost/weak_ptr.hpp>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace gui
{
	class Renderer;

	class  TextureManager
	{
		typedef boost::weak_ptr<Texture> TextureWeakPtr;
		typedef std::map<std::string, TextureWeakPtr> Textures;
		typedef Textures::iterator TexturesIter;

	public:
		TextureManager(Renderer& render);
		~TextureManager();	

		void	onDeviceReset();
		void	onDeviceLost();

		TexturePtr	createTexture(const std::string& filename);
		void		pushTexture(TexturePtr tex);

		void	cleanup();

	protected:
		TextureManager& operator=(const TextureManager&) { return *this; }
		Textures m_textures;
		Renderer& m_render;
	};
}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif