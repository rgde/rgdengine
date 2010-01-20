#pragma once

#include "image.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace gui
{
	class Texture;
	typedef boost::shared_ptr<Texture> TexturePtr;
	class Renderer;

	class  Imageset
	{
		typedef std::map<std::string, Image> Images;
		typedef Images::iterator ImagesIter;
		typedef std::pair<std::string, Image> ImagePair;
		typedef Images::const_iterator ImagesCIter;

	public:
		Imageset(const std::string& name, TexturePtr tex);
		Imageset(const std::string& filename, Renderer& render);

		~Imageset();

		const TexturePtr& getTexture() const	{return m_texture;}
		const std::string& getName() const	{return m_name;}

		bool isImageDefined(const std::string& name) const	{return m_images.find(name) != m_images.end();}
		const Image* getImage(const std::string& name) const;
		void undefineImage(const std::string& name);
		void undefineAllImages();

		const Image* defineImage(const std::string& name, const Rect& image_rect);

		const Image* operator[](const std::string& name) const { return getImage(name); }
		const Image* getImagePtr(std::string name) const { return getImage(name); }

	protected:
		void load(const std::string& filename, Renderer& render);

	protected:
		TexturePtr m_texture;
		std::string m_name;

		Images m_images;
	};

	typedef boost::shared_ptr<Imageset> ImagesetPtr;
}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif