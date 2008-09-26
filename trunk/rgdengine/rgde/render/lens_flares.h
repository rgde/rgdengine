/// Class for rendering lens flare effect is declared here.
#pragma once

#include <rgde/render/manager.h>


namespace render
{
	typedef boost::shared_ptr<class LensFlares> PLensFlares;
	
	class LensFlares : public rendererable
	{
	public:
		static PLensFlares create(const std::string& file_name, const math::frame_ptr& pFrame);

		struct Flare
		{
			texture_ptr texture;
			float fDistance;
			float fImageScale;
			float fAngleScale;
			math::Color color;
		};

		//You can not to fill texture member of Flare. Then use strTextureName
		void addFlare(const Flare& flare, const std::string& strTextureName = "");

	protected:
		LensFlares(math::frame_ptr pFrame);//pFrame is light pos

	private:
		void loadFromXML(const std::string& file_name);
		void render();
		inline void progressFlare(Flare& flare, const math::Vec2f& toLightVector, float fToLightLength, float fAngle, float fAlphaScale);

	private:
		math::frame_ptr m_frame;
		std::vector<Flare> m_flares;
	};

	typedef boost::shared_ptr<class LensFlares> PLensFlares;
}