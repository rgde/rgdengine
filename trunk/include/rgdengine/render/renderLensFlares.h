/// Class for rendering lens flare effect is declared here.
#pragma once

#include "render/renderManager.h"


namespace render
{
	typedef boost::shared_ptr<class CLensFlares> PLensFlares;
	
	class CLensFlares : public IRendererable
	{
	public:
		static PLensFlares Create(const std::string& strFileName, const math::PFrame& pFrame);

		struct SFlare
		{
			PTexture pTexture;
			float fDistance;
			float fImageScale;
			float fAngleScale;
			math::Color color;
		};

		//You can not to fill pTexture member of SFlare. Then use strTextureName
		void addFlare(const SFlare& flare, const std::string& strTextureName = "");

	protected:
		CLensFlares(math::PFrame pFrame);//pFrame is light pos

	private:
		void loadFromXML(const std::string& strFileName);
		void render();
		inline void progressFlare(SFlare& flare, const math::Vec2f& toLightVector, float fToLightLength, float fAngle, float fAlphaScale);

	private:
		math::PFrame m_pFrame;
		std::vector<SFlare> m_flares;
	};

	typedef boost::shared_ptr<class CLensFlares> PLensFlares;
}