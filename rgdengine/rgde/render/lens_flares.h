/// Class for rendering lens flare effect is declared here.
#pragma once

#include <rgde/render/manager.h>


namespace render
{
	typedef boost::shared_ptr<class lens_flares> lens_flares_ptr;
	
	class lens_flares : public rendererable
	{
	public:
		static lens_flares_ptr create(const std::string& file_name, const math::frame_ptr& frame);

		struct flare
		{
			texture_ptr texture;
			float distance;
			float image_scale;
			float angle_scale;
			math::Color color;
		};

		//You can not to fill texture member of flare. Then use strTextureName
		void add_flare(const flare& flare, const std::string& strTextureName = "");

	protected:
		lens_flares(math::frame_ptr frame);//frame is light pos

	private:
		void load_from_xml(const std::string& file_name);
		void render();
		inline void update_flare(flare& flare, const math::Vec2f& toLightVector, float fToLightLength, float angle, float alpha_scale);

	private:
		math::frame_ptr m_frame;
		std::vector<flare> m_flares;
	};

	typedef boost::shared_ptr<class lens_flares> lens_flares_ptr;
}