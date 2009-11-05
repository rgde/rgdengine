#include "precompiled.h"

#include <rgde/render/flame_renderer.h>
#include <rgde/render/particles/includes.h>

#include <rgde/base/xml_helpers.h>

#include <rgde/scene/scene.h>

namespace render
{
	flame_fx::flame_fx(const std::string &tex, const math::Color &color, const math::vec3f &pos, const math::vec2f &size, uint fps, const std::vector<math::frame_ptr> &vVector)
		: rendererable(100000)
	{
		hide();

		m_renderInfo.render_func = boost::bind(&flame_fx::render, this);
		{
			io::path_add_scoped p	("common/");
			m_pParticleTexture = texture::create(tex);
			std::string strTNFName	= io::helpers::get_shot_filename(tex) + ".tnf";;
			readTNF(strTNFName);
		}

		m_pTank = particles::renderer_ptr(new particles::renderer());
		m_pTank->setTextureTiling(m_nRows, m_nTotalColumns, m_nTotalRows);

		int nFramesNum								= (int)vVector.size();
		float fFramesNum							= (float)nFramesNum;

		particles::renderer::ParticleArray &vArray	= m_pTank->getParticles();

		std::vector<math::frame_ptr>::const_iterator it= vVector.begin();

		for (; it != vVector.end(); it++)
		{
			particles::renderer::SParticle particle;
			particle.pos = (*it)->get_world_pos() + pos;
			particle.size = size;
			particle.spin = 0.0f;
			particle.color = color.color;
			float fRandom	= math::Math::rangeRandom(0.0f, fFramesNum);
			particle.nTile = (int)fRandom;
			vArray.push_back(particle);
		}

		m_pTank->update();

		m_fFPS = float(fps);
	}

	void flame_fx::update(float dt)
	{
		if (m_nFrames == 1)
			return;

		m_fFramesToAdd += dt * m_fFPS;

		int nFramesToAdd= (int)m_fFramesToAdd;

		if (nFramesToAdd != 0)
		{
			particles::renderer::ParticleArray &vParticles= m_pTank->getParticles();

			size_t nNumParticles						= vParticles.size();

			for (size_t i = 0; i < nNumParticles; i++)
			{
				unsigned int &nTile	= vParticles[i].nTile;

				nTile += nFramesToAdd;

				if (nTile >= (unsigned int)m_nFrames)
					nTile = nTile % (unsigned int)(m_nFrames - 1);
			}

			m_pTank->update();

			m_fFramesToAdd -= nFramesToAdd;
		}
	}

	void flame_fx::render()
	{
		m_pTank->render(m_pParticleTexture, *scene::TheScene::get().get_root().get());
	}


	PFlameRenderer flame_fx::create(const std::string &tex, const math::Color &color, const math::vec3f &pos, const math::vec2f &size, uint fps, const std::vector<math::frame_ptr> &vVector)
	{
		return PFlameRenderer(new flame_fx(tex, color, pos, size, fps, vVector));
	}


	PFlameRenderer flame_fx::create(const std::string &file_name, const std::vector<math::frame_ptr> &vVector)
	{
		//Load from XML
		//TODO: Make function load_from_xml
		TiXmlDocument flame;
		{
			io::path_add_scoped p	("Flames/");
			if (!base::load_xml(file_name, flame))
			{
				base::lerr << "Can't load flame \"" << file_name << "\".";
				return PFlameRenderer();
			}
		}

		TiXmlHandle root(&flame);

		root = root.FirstChildElement("flame");

		std::string strTextureName	= base::safe_read<std::string>(root, "texture_name", "");

		float x						= base::safe_read_attr(root, "size", "x", 0.0f);
		float y						= base::safe_read_attr(root, "size", "y", 0.0f);

		math::vec3f pos				(base::safe_read_attr(root, "pos", "x", 0.0f), base::safe_read_attr(root, "pos", "y", 0.0f), base::safe_read_attr(root, "pos", "z", 0.0f));

		math::vec2f size(x, y);

		char r	= base::safe_read_attr(root, "color", "r", 0);
		char g	= base::safe_read_attr(root, "color", "g", 0);
		char b	= base::safe_read_attr(root, "color", "b", 0);
		char a	= base::safe_read_attr(root, "color", "a", 0);

		math::Color color(r, g, b, a);

		float fps = base::safe_read(root, "fps", 0.0f);

		return create(strTextureName, color, pos, size, (int)fps, vVector);
	}

	void readValue(int &value, io::readstream_ptr in)
	{
		byte temp	= 0;
		in->read(&temp, sizeof(byte));
		value = temp;
	}

	void flame_fx::readTNF(const std::string &file_name)
	{
		m_nColumns = 1;
		m_nRows = 1;
		m_nFrames = 1;
		m_nTotalColumns = 1;
		m_nTotalRows = 1;
		m_fFPS = 1.0f;
		m_fFramesToAdd = 0.0f;

		if (file_name.empty())
			return;

		io::CFileSystem &fs		= io::TheFileSystem::get();
		io::ScopePathChange p	("./Media/Common/Textures/");
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