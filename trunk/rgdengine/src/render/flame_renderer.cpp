#include "precompiled.h"

#include <rgde/render/flame_renderer.h>
#include <rgde/render/particles/includes.h>

#include <rgde/base/xml_helpers.h>

#include <rgde/scene/scene.h>

namespace render
{
	flame_fx::flame_fx(const std::string &tex, const math::Color &color, const math::vec3f &pos, const math::vec2f &size, 
					   uint fps, const frames_vector &vector)
		: rendererable(100000)
	{
		hide();

		m_renderInfo.render_func = boost::bind(&flame_fx::render, this);
		{
			io::path_add_scoped p("common/");
			m_particle_texture = texture::create(tex);
			std::string strTNFName	= io::helpers::get_shot_filename(tex) + ".tnf";;
			read_tnf(strTNFName);
		}

		m_tank = particles::renderer_ptr(new particles::renderer());
		m_tank->setTextureTiling(m_nRows, m_nTotalColumns, m_nTotalRows);

		int nFramesNum = (int)vector.size();
		float fFramesNum = (float)nFramesNum;

		particles::renderer::particles_vector &particles = m_tank->getParticles();

		std::vector<math::frame_ptr>::const_iterator it= vector.begin();

		for (; it != vector.end(); it++)
		{
			particles::renderer::SParticle particle;
			particle.pos = (*it)->get_world_pos() + pos;
			particle.size = size;
			particle.spin = 0.0f;
			particle.color = color.color;
			float fRandom = math::Math::rangeRandom(0.0f, fFramesNum);
			particle.nTile = (int)fRandom;
			particles.push_back(particle);
		}

		m_tank->update();

		m_fps = float(fps);
	}

	void flame_fx::update(float dt)
	{
		if (m_nFrames == 1)
			return;

		m_fFramesToAdd += dt * m_fps;

		int nFramesToAdd= (int)m_fFramesToAdd;

		if (nFramesToAdd != 0)
		{
			particles::renderer::particles_vector &vParticles= m_tank->getParticles();

			size_t nNumParticles = vParticles.size();

			for (size_t i = 0; i < nNumParticles; i++)
			{
				unsigned int &nTile	= vParticles[i].nTile;

				nTile += nFramesToAdd;

				if (nTile >= (unsigned int)m_nFrames)
					nTile = nTile % (unsigned int)(m_nFrames - 1);
			}

			m_tank->update();

			m_fFramesToAdd -= nFramesToAdd;
		}
	}

	void flame_fx::render()
	{
		m_tank->render(m_particle_texture, scene::TheScene::get().get_root());
	}


	flame_fx_ptr flame_fx::create(const std::string &tex, const math::Color &color, const math::vec3f &pos, 
								  const math::vec2f &size, uint fps, const frames_vector &vector)
	{
		return flame_fx_ptr(new flame_fx(tex, color, pos, size, fps, vector));
	}


	flame_fx_ptr flame_fx::create(const std::string &file_name, const frames_vector &vector)
	{
		//Load from XML
		//TODO: Make function load_from_xml
		TiXmlDocument flame;
		{
			io::path_add_scoped p("Flames/");
			if (!base::load_xml(file_name, flame))
			{
				base::lerr << "Can't load flame \"" << file_name << "\".";
				return flame_fx_ptr();
			}
		}

		TiXmlHandle root(&flame);

		root = root.FirstChildElement("flame");

		std::string strTextureName	= base::safe_read<std::string>(root, "texture_name", "");

		float x = base::safe_read_attr(root, "size", "x", 0.0f);
		float y = base::safe_read_attr(root, "size", "y", 0.0f);

		math::vec3f pos(
			base::safe_read_attr(root, "pos", "x", 0.0f), 
			base::safe_read_attr(root, "pos", "y", 0.0f), 
			base::safe_read_attr(root, "pos", "z", 0.0f)
			);

		math::vec2f size(x, y);

		char r	= base::safe_read_attr(root, "color", "r", 0);
		char g	= base::safe_read_attr(root, "color", "g", 0);
		char b	= base::safe_read_attr(root, "color", "b", 0);
		char a	= base::safe_read_attr(root, "color", "a", 0);

		math::Color color(r, g, b, a);

		float fps = base::safe_read(root, "fps", 0.0f);

		return create(strTextureName, color, pos, size, (int)fps, vector);
	}

	void readValue(int &value, io::readstream_ptr in)
	{
		byte temp	= 0;
		in->read(&temp, sizeof(byte));
		value = temp;
	}

	void flame_fx::read_tnf(const std::string &file_name)
	{
		m_nColumns = 1;
		m_nRows = 1;
		m_nFrames = 1;
		m_nTotalColumns = 1;
		m_nTotalRows = 1;
		m_fps = 1.0f;
		m_fFramesToAdd = 0.0f;

		if (file_name.empty())
			return;

		io::file_system &fs		= io::file_system::get();
		io::scope_path p	("./Media/Common/Textures/");
		io::readstream_ptr in		= fs.find(file_name);

		if (!in)
		{
			base::lerr << "Can't open TNF file \"" << file_name << "\".";
			return;
		}

		readValue(m_nColumns, in);
		readValue(m_nRows, in);
		readValue(m_nFrames, in);
		readValue(m_nTotalColumns, in);
		readValue(m_nTotalRows, in);
	}
}