#include "precompiled.h"

#include <rgde/render/renderLensFlares.h>
#include <rgde/render/renderSprites.h>

#include <rgde/base/xml_helpers.h>


namespace render
{
	typedef base::TSingelton<CSpriteManager, 1> TheSpriteManager2;

	PLensFlares LensFlares::Create(const std::string &strFileName, const math::PFrame &pFrame)
	{
		PLensFlares p	= PLensFlares(new LensFlares(pFrame));

		p->loadFromXML(strFileName);

		return p;
	}

	LensFlares::LensFlares(math::PFrame pFrame)
		: m_pFrame(pFrame)
	{
		m_renderInfo.pFrame = m_pFrame.get();
		m_renderInfo.pRenderFunc = boost::bind(&LensFlares::render, this);
		TheSpriteManager2::Get().setAditiveBlending(true);
	}

	void LensFlares::addFlare(const Flare &flare, const std::string &strTextureName)
	{
		Flare temp_flare	= flare;
		if ((temp_flare.texture == NULL) && (!strTextureName.empty()))
		{
			io::ScopePathAdd p	("common/");
			temp_flare.texture = render::ITexture::Create(strTextureName);
		}
		m_flares.push_back(temp_flare);
	}

	inline void LensFlares::progressFlare(Flare &flare, const math::Vec2f &toLightVector, float fToLightLength, float fAngle, float fAlphaScale)
	{
		Sprite sprite;
		sprite.color = flare.color;
		sprite.color.a = (char)((float)sprite.color.a * fAlphaScale);
		math::Vec2f screenCenter= math::Vec2f(400.0f, 300.0f);
		sprite.pos = toLightVector * flare.fDistance * fToLightLength + screenCenter;
		sprite.texture = flare.texture;
		sprite.rect = math::Rect(0.0f, 0.0f, 1.0f, 1.0f);
		sprite.size = math::Vec2f(flare.fImageScale, flare.fImageScale);
		sprite.spin = fAngle * flare.fAngleScale;
		sprite.uPriority = 1;

		TheSpriteManager2::Get().addSprite(sprite);
	}

	void progressNode(TiXmlNode *pNode, LensFlares *pLensFlares)
	{
		if (NULL == pNode)
			return;

		TiXmlElement *pElement	= pNode->ToElement();

		if (NULL == pElement)
			return;

		LensFlares::Flare flare;
		std::string strTextureName	= base::safeReadValue<std::string>(pElement, "texture", "");
		flare.fDistance = base::safeReadValue(pElement, "distance", 0.0f);
		flare.fImageScale = base::safeReadValue(pElement, "image_scale", 0.0f);
		flare.fAngleScale = base::safeReadValue(pElement, "angle_scale", 0.0f);
		base::read(flare.color, pElement, "color");

		if (pLensFlares)
			pLensFlares->addFlare(flare, strTextureName);
	}

	void LensFlares::loadFromXML(const std::string &strFileName)
	{
		TiXmlDocument lens;

		if (!base::loadXml(strFileName, lens))
		{
			io::ScopePathAdd p	("LensFlares/");
			if (!base::loadXml(strFileName, lens))
			{
				base::lerr << "Can't load Lens Flares effect \"" << strFileName << "\".";
				return;
			}
		}

		TiXmlHandle root(&lens);
		root = root.FirstChildElement("lens_flares");

		TiXmlNode *pRoot= root.Element();

		if (!pRoot)
			return;

		TiXmlNode *pFirst	= pRoot->FirstChild("flare"), *pCurrent = pFirst;

		for (; pCurrent != NULL; pCurrent = pCurrent->NextSibling("flare"))
			progressNode(pCurrent, this);
	}

	void LensFlares::render()
	{
		math::PCamera pCamera	= TheDevice::Get().getCurentCamera();

		math::Matrix44f projView= pCamera->getProjMatrix() * pCamera->getViewMatrix();

		math::Vec3f framePos3	= m_pFrame->getGlobalPosition();
		math::Vec4f framePos4	= math::Vec4f(framePos3[0], framePos3[1], framePos3[2], 1.0f);
		math::Vec4f lightPos4	= projView *framePos4;

		if (lightPos4[3] == 0.0f)
			return;

		math::Vec3f lightPos3		= math::Vec3f((lightPos4[0] / lightPos4[3] + 1.0f) / 2.0f, (lightPos4[1] / lightPos4[3] + 1.0f) / 2.0f, (lightPos4[2] / lightPos4[3] + 1.0f) / 2.0f);
		math::Vec2f lightPos		= math::Vec2f(lightPos3[0] * 800.0f, (1.0f - lightPos3[1]) * 600.0f);
		math::Vec2f screenCenter	= math::Vec2f(400.0f, 300.0f);
		math::Vec2f toLightVector	= lightPos - screenCenter;
		float fLength				= math::normalize(toLightVector);

		float fAngle				= math::Math::aCos(math::dot(toLightVector, math::Vec2f(1.0f, 0.0f)));

		/*
					math::Vec3f toLightVector3 = framePos3 - pCamera->getPosition();
					math::normalize(toLightVector3);
					math::Matrix44f mat = pCamera->getViewMatrix();
					math::Vec3f at = math::Vec3f(mat.mData[0], mat.mData[4], mat.mData[8]);
					math::normalize(at);
					float fAlphaScale = math::dot(at, toLightVector3);
			*/
		float fAlphaScale			= 1.0f;

		size_t nNumFlares			= m_flares.size();

		for (size_t i = 0; i < nNumFlares; i++)
			progressFlare(m_flares[i], toLightVector, fLength, fAngle, fAlphaScale);
	}
}