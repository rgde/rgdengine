/// Class for rendering lens flare effect is declared here.
#pragma once

#include <rgde/render/manager.h>


namespace render
{
	typedef boost::shared_ptr<class LensFlares> PLensFlares;
	
	class LensFlares : public IRendererable
	{
	public:
		static PLensFlares Create(const std::string& strFileName, const math::PFrame& pFrame);

		struct Flare
		{
			PTexture texture;
			float fDistance;
			float fImageScale;
			float fAngleScale;
			math::Color color;
		};

		//You can not to fill texture member of Flare. Then use strTextureName
		void addFlare(const Flare& flare, const std::string& strTextureName = "");

	protected:
		LensFlares(math::PFrame pFrame);//pFrame is light pos

	private:
		void loadFromXML(const std::string& strFileName);
		void render();
		inline void progressFlare(Flare& flare, const math::Vec2f& toLightVector, float fToLightLength, float fAngle, float fAlphaScale);

	private:
		math::PFrame m_pFrame;
		std::vector<Flare> m_flares;
	};

	typedef boost::shared_ptr<class LensFlares> PLensFlares;
}