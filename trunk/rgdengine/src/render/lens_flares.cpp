#include "precompiled.h"

#include <rgde/render/lens_flares.h>
#include <rgde/render/sprites.h>

#include <rgde/base/xml_helpers.h>


namespace render
{
	typedef base::singelton<sprite_manager, 1> TheSpriteManager2;

	lens_flares_ptr lens_flares::create(const std::string &file_name, const math::frame_ptr &frame)
	{
		lens_flares_ptr p	= lens_flares_ptr(new lens_flares(frame));

		p->load_from_xml(file_name);

		return p;
	}

	lens_flares::lens_flares(math::frame_ptr frame)
		: m_frame(frame)
	{
		m_renderInfo.frame = m_frame.get();
		m_renderInfo.render_func = boost::bind(&lens_flares::render, this);
		TheSpriteManager2::get().setAditiveBlending(true);
	}

	void lens_flares::add_flare(const flare &f, const std::string &strTextureName)
	{
		flare temp_flare	= f;
		if ((temp_flare.texture == NULL) && (!strTextureName.empty()))
		{
			io::path_add_scoped p	("common/");
			temp_flare.texture = render::texture::create(strTextureName);
		}
		m_flares.push_back(temp_flare);
	}

	inline void lens_flares::update_flare(flare &flare, const math::Vec2f &toLightVector, float fToLightLength, float angle, float alpha_scale)
	{
		sprite sprite;
		sprite.color = flare.color;
		sprite.color.a = (char)((float)sprite.color.a * alpha_scale);
		math::Vec2f screenCenter= math::Vec2f(400.0f, 300.0f);
		sprite.pos = toLightVector * flare.distance * fToLightLength + screenCenter;
		sprite.texture = flare.texture;
		sprite.rect = math::Rect(0.0f, 0.0f, 1.0f, 1.0f);
		sprite.size = math::Vec2f(flare.image_scale, flare.image_scale);
		sprite.spin = angle * flare.angle_scale;
		sprite.uPriority = 1;

		TheSpriteManager2::get().addSprite(sprite);
	}

	void progressNode(TiXmlNode *pNode, lens_flares *pLensFlares)
	{
		if (NULL == pNode)
			return;

		TiXmlElement *pElement	= pNode->ToElement();

		if (NULL == pElement)
			return;

		lens_flares::flare flare;
		std::string strTextureName	= base::safe_read<std::string>(pElement, "texture", "");
		flare.distance = base::safe_read(pElement, "distance", 0.0f);
		flare.image_scale = base::safe_read(pElement, "image_scale", 0.0f);
		flare.angle_scale = base::safe_read(pElement, "angle_scale", 0.0f);
		base::read(flare.color, pElement, "color");

		if (pLensFlares)
			pLensFlares->add_flare(flare, strTextureName);
	}

	void lens_flares::load_from_xml(const std::string &file_name)
	{
		TiXmlDocument lens;

		if (!base::load_xml(file_name, lens))
		{
			io::path_add_scoped p	("lens_flares/");
			if (!base::load_xml(file_name, lens))
			{
				base::lerr << "Can't load Lens Flares effect \"" << file_name << "\".";
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

	void lens_flares::render()
	{
		math::camera_ptr camera	= TheDevice::get().get_camera();

		math::Matrix44f projView= camera->get_proj_matrix() * camera->get_view_matrix();

		math::Vec3f framePos3	= m_frame->getGlobalPosition();
		math::Vec4f framePos4	= math::Vec4f(framePos3[0], framePos3[1], framePos3[2], 1.0f);
		math::Vec4f lightPos4	= projView *framePos4;

		if (lightPos4[3] == 0.0f)
			return;

		math::Vec3f lightPos3		= math::Vec3f((lightPos4[0] / lightPos4[3] + 1.0f) / 2.0f, (lightPos4[1] / lightPos4[3] + 1.0f) / 2.0f, (lightPos4[2] / lightPos4[3] + 1.0f) / 2.0f);
		math::Vec2f lightPos		= math::Vec2f(lightPos3[0] * 800.0f, (1.0f - lightPos3[1]) * 600.0f);
		math::Vec2f screenCenter	= math::Vec2f(400.0f, 300.0f);
		math::Vec2f toLightVector	= lightPos - screenCenter;
		float length				= math::normalize(toLightVector);

		float angle				= math::Math::aCos(math::dot(toLightVector, math::Vec2f(1.0f, 0.0f)));

		/*
					math::Vec3f toLightVector3 = framePos3 - camera->getPosition();
					math::normalize(toLightVector3);
					math::Matrix44f mat = camera->get_view_matrix();
					math::Vec3f at = math::Vec3f(mat.mData[0], mat.mData[4], mat.mData[8]);
					math::normalize(at);
					float alpha_scale = math::dot(at, toLightVector3);
			*/
		float alpha_scale			= 1.0f;

		size_t num_flares			= m_flares.size();

		for (size_t i = 0; i < num_flares; i++)
			update_flare(m_flares[i], toLightVector, length, angle, alpha_scale);
	}
}