#include "precompiled.h"

#include <rgde/render/flame_renderer.h>
#include <rgde/render/particles/includes.h>

#include <rgde/base/xml_helpers.h>

#include <rgde/scene/scene.h>

namespace render
{
	FlameRenderer::FlameRenderer(const std::string &tex, const math::Color &color, const math::Vec3f &pos, const math::Vec2f &size, uint fps, const std::vector<math::PFrame> &vVector)
		: IRendererable(100000)
	{
		hide();

		m_renderInfo.pRenderFunc = boost::bind(&FlameRenderer::render, this);
		{
			io::ScopePathAdd p	("common/");
			m_pParticleTexture = ITexture::Create(tex);
			std::string strTNFName	= io::helpers::getFileNameWithoutExtension(tex) + ".tnf";;
			readTNF(strTNFName);
		}

		m_pTank = particles::PPTank(new particles::PTank());
		m_pTank->setTextureTiling(m_nRows, m_nTotalColumns, m_nTotalRows);

		int nFramesNum								= (int)vVector.size();
		float fFramesNum							= (float)nFramesNum;

		particles::PTank::ParticleArray &vArray	= m_pTank->getParticles();

		std::vector<math::PFrame>::const_iterator it= vVector.begin();

		for (; it != vVector.end(); it++)
		{
			particles::PTank::SParticle particle;
			particle.pos = (*it)->getGlobalPosition() + pos;
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

	void FlameRenderer::update(float dt)
	{
		if (m_nFrames == 1)
			return;

		m_fFramesToAdd += dt * m_fFPS;

		int nFramesToAdd= (int)m_fFramesToAdd;

		if (nFramesToAdd != 0)
		{
			particles::PTank::ParticleArray &vParticles= m_pTank->getParticles();

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

	void FlameRenderer::render()
	{
		m_pTank->render(m_pParticleTexture, *scene::TheScene::Get().getRootFrame().get());
	}


	PFlameRenderer FlameRenderer::Create(const std::string &tex, const math::Color &color, const math::Vec3f &pos, const math::Vec2f &size, uint fps, const std::vector<math::PFrame> &vVector)
	{
		return PFlameRenderer(new FlameRenderer(tex, color, pos, size, fps, vVector));
	}


	PFlameRenderer FlameRenderer::Create(const std::string &strFileName, const std::vector<math::PFrame> &vVector)
	{
		//Load from XML
		//TODO: Make function loadFromXML
		TiXmlDocument flame;
		{
			io::ScopePathAdd p	("Flames/");
			if (!base::loadXml(strFileName, flame))
			{
				base::lerr << "Can't load flame \"" << strFileName << "\".";
				return PFlameRenderer();
			}
		}

		TiXmlHandle root(&flame);

		root = root.FirstChildElement("flame");

		std::string strTextureName	= base::safeReadValue<std::string>(root, "texture_name", "");

		float x						= base::safeReadAttributeValue(root, "size", "x", 0.0f);
		float y						= base::safeReadAttributeValue(root, "size", "y", 0.0f);

		math::Vec3f pos				(base::safeReadAttributeValue(root, "pos", "x", 0.0f), base::safeReadAttributeValue(root, "pos", "y", 0.0f), base::safeReadAttributeValue(root, "pos", "z", 0.0f));

		math::Vec2f size(x, y);

		char r	= base::safeReadAttributeValue(root, "color", "r", 0);
		char g	= base::safeReadAttributeValue(root, "color", "g", 0);
		char b	= base::safeReadAttributeValue(root, "color", "b", 0);
		char a	= base::safeReadAttributeValue(root, "color", "a", 0);

		math::Color color(r, g, b, a);

		float fps = base::safeReadValue(root, "fps", 0.0f);

		return Create(strTextureName, color, pos, size, (int)fps, vVector);
	}

	void readValue(int &value, io::PReadStream in)
	{
		byte temp	= 0;
		in->readbuff(&temp, sizeof(byte));
		value = temp;
	}

	void FlameRenderer::readTNF(const std::string &strFileName)
	{
		m_nColumns = 1;
		m_nRows = 1;
		m_nFrames = 1;
		m_nTotalColumns = 1;
		m_nTotalRows = 1;
		m_fFPS = 1.0f;
		m_fFramesToAdd = 0.0f;

		if (strFileName.empty())
			return;

		io::CFileSystem &fs		= io::TheFileSystem::Get();
		io::ScopePathChange p	("./Media/Common/Textures/");
		io::PReadStream in		= fs.findFile(strFileName);

		if (!in)
		{
			base::lerr << "Can't open TNF file \"" << strFileName << "\".";
			return;
		}

		readValue(m_nColumns, in);
		readValue(m_nRows, in);
		readValue(m_nFrames, in);
		readValue(m_nTotalColumns, in);
		readValue(m_nTotalRows, in);
	}
}