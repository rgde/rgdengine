#include "precompiled.h"

#include <rgde/render/renderSprites.h>

namespace render
{
	//////////////////////////////////////////////////////////////////////////
	SSprite::SSprite()	: spin(0), uPriority(0), rect(0, 0, 1, 1)
	{
	}

	SSprite::SSprite( const math::Vec2f& pos_, const math::Vec2f& size_, 
		const math::Color& color_,render::PTexture pTexture_,
		float spin_, const math::Rect& rect_,
		unsigned long uPriority_) :
	rect (rect_), pos (pos_), size (size_), spin (spin_), uPriority (uPriority_), 
		pTexture (pTexture_), color (color_)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------------
	CSpriteManager::CSpriteManager(int priority)
		: m_cvScreenSize(800, 600),
		  m_nReservedSize(0),
		  m_nSpritesRendered(0),
		  m_bSorted(false),
		  m_bUpdated(true),
		  IRendererable(priority),
		  m_vOrigin(0, 0)
	{
		//base::lmsg << "CSpriteManager::CSpriteManager()";
		math::Vec2f vFrontBufferSize= render::TheDevice::Get().getBackBufferSize();
		m_vScale = vFrontBufferSize / m_cvScreenSize;


		m_bAditive = false;

		m_pEffect = IEffect::Create("SpriteManager.fxo");
		m_renderInfo.pRenderFunc = boost::bind(&CSpriteManager::render, this);
	}
	//-----------------------------------------------------------------------------------
	CSpriteManager::~CSpriteManager()
	{
	}
	//-----------------------------------------------------------------------------------
	void CSpriteManager::addSprite(const SSprite &pSprite)
	{
		m_bUpdated = false;
		m_vSprites.push_back(pSprite);
	}
	//-----------------------------------------------------------------------------------
	bool sortPred (const SSprite& pSprite1, SSprite& pSprite2)
	{
		return (pSprite1.uPriority < pSprite2.uPriority);
	}
	//-----------------------------------------------------------------------------------
	inline math::Vec2f rotatePos(float x, float y, float sina, float cosa)
	{
		return math::Vec2f(x * cosa - y * sina, x * sina + y * cosa);
	}
	//--------------------------------------------------------------------------------------
	void CSpriteManager::update()
	{
		using namespace math;

		std::sort( m_vSprites.begin(), m_vSprites.end(), sortPred );

		if (m_vSprites.empty())
		{
			m_bUpdated = true;
			return;
		}

		unsigned nSprites	= (unsigned)m_vSprites.size();
		// На случай, если число спрайтов в векторе больше, чем зарезервировано в буферах
		if ((nSprites > m_nReservedSize))
		{
			m_nReservedSize = nSprites;

			Geometry::Indexes &vIndices	= m_Geometry.lockIB();
			if (vIndices.size() < nSprites * 6)
				vIndices.resize(nSprites * 6);
			for (unsigned i = 0; i < nSprites; ++i)
			{
				vIndices[i * 6 + 0] = i * 4 + 0;
				vIndices[i * 6 + 1] = i * 4 + 1;
				vIndices[i * 6 + 2] = i * 4 + 2;
				vIndices[i * 6 + 3] = i * 4 + 0;
				vIndices[i * 6 + 4] = i * 4 + 2;
				vIndices[i * 6 + 5] = i * 4 + 3;
			}
			m_Geometry.unlockIB();
		}

		Geometry::Vertexes &vVertices	= m_Geometry.lockVB();
		if (vVertices.size() < nSprites * 4)
			vVertices.resize(nSprites * 4);
		unsigned i	= 0;
		for (TSpritesIter it = m_vSprites.begin(); it != m_vSprites.end(); ++it)
		{
			// Срайты масштабируются только при записи в буфер
			const SSprite &sprite	= *it;
			const math::Color &color= sprite.color;
			const math::Rect &rect	= sprite.rect;
			// Сразу же масштабируем позицию и размер
			math::Vec2f hsize		= math::Vec2f(sprite.size[0] * m_vScale[0], sprite.size[1] * m_vScale[1]) / 2.0f;
			math::Vec2f pos			= math::Vec2f(sprite.pos[0] * m_vScale[0], sprite.pos[1] * m_vScale[1]);

			float cosa				= ::cos(sprite.spin);
			float sina				= ::sin(sprite.spin);

			// Top left
			math::Vec2f rotPos		= rotatePos(-hsize[0], -hsize[1], sina, cosa) + pos;
			vVertices[i].position = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
			vVertices[i].tex = rect.getTopLeft();
			vVertices[i].color = color;
			++i;

			// Top right
			rotPos = rotatePos(hsize[0], -hsize[1], sina, cosa) + pos;
			vVertices[i].position = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
			vVertices[i].tex = rect.getTopRight();
			vVertices[i].color = color;
			++i;

			// Bottom right
			rotPos = rotatePos(hsize[0], hsize[1], sina, cosa) + pos;
			vVertices[i].position = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
			vVertices[i].tex = rect.getBottomRight();
			vVertices[i].color = color;
			++i;

			// Bottom left
			rotPos = rotatePos(-hsize[0], hsize[1], sina, cosa) + pos;
			vVertices[i].position = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
			vVertices[i].tex = rect.getBottomLeft();
			vVertices[i].color = color;
			++i;
		}
		m_Geometry.unlockVB();

		m_bUpdated = true;
	}

	//-----------------------------------------------------------------------------------
	void CSpriteManager::render()
	{
		if (m_vSprites.empty()) return;

		update();

		render::IEffect::ITechnique *pTech	= NULL;

		if (m_bAditive)
			pTech = m_pEffect->findTechnique("aditive");
		else
			pTech = m_pEffect->findTechnique("alpha");

		pTech->begin();

		for (unsigned iPass = 0; iPass < pTech->getPasses().size(); iPass++)
		{
			pTech->getPasses()[iPass]->begin();

			unsigned nSpritesRendered				= 0;
			IEffect::IParameter *textureShaderParam	= m_pEffect->getParams()["SpriteTexture"];

			uint start_sprite = 0;
			PTexture cur_tex = m_vSprites[0].pTexture;
			for (uint i = 0; i < m_vSprites.size(); ++i)
			{
				SSprite &sprite = m_vSprites[i];

				// если = то отрисовать
				if (cur_tex != sprite.pTexture)
				{
					int cur_sprite = i - 1;
					int nSprites = cur_sprite - start_sprite + 1;
					//i == m_vSprites.size()-1
					if (nSprites > 0)
					{
						textureShaderParam->set(cur_tex);
						m_pEffect->commitChanges();
						m_Geometry.render(PrimTypeTriangleList, 0, 4 * start_sprite, nSprites * 4, 6 * start_sprite, nSprites * 2);
						nSpritesRendered += nSprites;
					}
					cur_tex = sprite.pTexture;
					start_sprite = i;
				}

				if (i == m_vSprites.size()-1)
				{
					int cur_sprite = i;
					int nSprites = i - start_sprite + 1;
					if (nSprites > 0)
					{
						textureShaderParam->set(cur_tex);
						m_pEffect->commitChanges();
						m_Geometry.render(PrimTypeTriangleList, 0, 4 * start_sprite, nSprites * 4, 6 * start_sprite, nSprites * 2);
						nSpritesRendered += nSprites;
					}
				}
			}
			m_nSpritesRendered = nSpritesRendered;

			pTech->getPasses()[iPass]->end();
		}
		pTech->end();

		m_vSprites.resize(0);
	}
	//-----------------------------------------------------------------------------------
	void CSpriteManager::onLostDevice()
	{
	}
	//-----------------------------------------------------------------------------------
	void CSpriteManager::onResetDevice()
	{
		// Вычисляем коэффициенты масштабирования
		math::Vec2f vFrontBufferSize= render::TheDevice::Get().getBackBufferSize();
		m_vScale = vFrontBufferSize / m_cvScreenSize;
		update();
	}
}