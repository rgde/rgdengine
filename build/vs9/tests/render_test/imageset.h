#pragma once

#include <map>
#include <string>

namespace game
{
	namespace gui
	{
		class imageset;

		class image
		{
		public:
			image(imageset& parent, rgde::math::rect uv);
			image();

			imageset& get_imageset() {return m_parent;}
			const rgde::math::rect& get_uv() const {return m_uv_rect;}

		private:
			imageset* m_parent;
			rgde::math::rect m_uv_rect;
		};

		class imageset
		{
		public:
			typedef std::map<std::string, image> images_map;
			typedef images_map::iterator image_itr;
			typedef images_map::const_iterator image_citr;

			imageset(const std::wstring& file);

			rgde::render::texture_ptr get_texture() {return m_texture;}
			const image& get_image(const std::string& name) const;

			const std::string get_name() const {return m_name;}

		protected:
			std::string m_name;
			images_map m_images;
			rgde::render::texture_ptr m_texture;
		};
	}
}