#include "precompiled.h"

#include <rgde/render/sprites.h>

namespace render
{
	sprite::sprite()
		: spin(0), 
		uPriority(0), 
		rect(0, 0, 1, 1)
	{
	}

	sprite::sprite( const math::Vec2f& pos_, const math::Vec2f& size_, 
		const math::Color& color_,render::texture_ptr pTexture_,
		float spin_, const math::Rect& rect_,
		unsigned long uPriority_) 
		:
		rect (rect_), 
		pos (pos_), 
		size (size_), 
		spin (spin_), 
		uPriority (uPriority_), 
		texture (pTexture_), 
		color (color_)
	{
	}

	SpriteManager::SpriteManager(int priority)
		: m_cvScreenSize(800, 600),
		  m_nReservedSize(0),
		  m_nSpritesRendered(0),
		  m_bSorted(false),
		  m_bUpdated(true),
		  rendererable(priority),
		  m_vOrigin(0, 0)
	{
		//base::lmsg << "SpriteManager::SpriteManager()";
		math::Vec2f vFrontBufferSize= render::TheDevice::get().getBackBufferSize();
		m_vScale = vFrontBufferSize / m_cvScreenSize;


		m_bAditive = false;

		m_effect = effect::create("SpriteManager.fx");
		m_renderInfo.pRenderFunc = boost::bind(&SpriteManager::render, this);
	}

	SpriteManager::~SpriteManager()
	{
	}

	void SpriteManager::addSprite(const sprite &pSprite)
	{
		m_bUpdated = false;
		m_sprites.push_back(pSprite);
	}

	bool sortPred (const sprite& pSprite1, sprite& pSprite2)
	{
		return (pSprite1.uPriority < pSprite2.uPriority);
	}

	inline math::Vec2f rotatePos(float x, float y, float sina, float cosa)
	{
		return math::Vec2f(x * cosa - y * sina, x * sina + y * cosa);
	}

	void SpriteManager::update()
	{
		using namespace math;

		std::sort( m_sprites.begin(), m_sprites.end(), sortPred );

		if (m_sprites.empty())
		{
			m_bUpdated = true;
			return;
		}

		unsigned nSprites	= (unsigned)m_sprites.size();
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
		for (SpritesIter it = m_sprites.begin(); it != m_sprites.end(); ++it)
		{
			// Срайты масштабируются только при записи в буфер
			const sprite &sprite	= *it;
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

	void SpriteManager::render()
	{
		if (m_sprites.empty()) return;

		update();

		render::effect::ITechnique *pTech	= NULL;

		if (m_bAditive)
			pTech = m_effect->findTechnique("aditive");
		else
			pTech = m_effect->findTechnique("alpha");

		assert(0 != pTech && "SpriteManager::render(): Can't find effect technique!");
		pTech->begin();

		for (unsigned iPass = 0; iPass < pTech->getPasses().size(); iPass++)
		{
			effect::ITechnique::IPass& pass = *pTech->getPasses()[iPass];
			pass.begin();

			unsigned nSpritesRendered = 0;
			effect::IParameter *textureShaderParam	= m_effect->getParams()["spriteTexture"];

			assert(0 != textureShaderParam && "m_effect->getParams()[\"spriteTexture\"] == NULL !");

			uint start_sprite = 0;
			texture_ptr cur_tex = m_sprites[0].texture;
			for (uint i = 0; i < m_sprites.size(); ++i)
			{
				sprite &sprite = m_sprites[i];

				// если = то отрисовать
				if (cur_tex != sprite.texture)
				{
					int cur_sprite = i - 1;
					int nSprites = cur_sprite - start_sprite + 1;
					//i == m_sprites.size()-1
					if (nSprites > 0)
					{
						textureShaderParam->set(cur_tex);
						m_effect->commitChanges();
						m_Geometry.render(PrimTypeTriangleList, 0, 4 * start_sprite, nSprites * 4, 6 * start_sprite, nSprites * 2);
						nSpritesRendered += nSprites;
					}
					cur_tex = sprite.texture;
					start_sprite = i;
				}

				if (i == m_sprites.size()-1)
				{
					int cur_sprite = i;
					int nSprites = i - start_sprite + 1;
					if (nSprites > 0)
					{
						textureShaderParam->set(cur_tex);
						m_effect->commitChanges();
						m_Geometry.render(PrimTypeTriangleList, 0, 4 * start_sprite, nSprites * 4, 6 * start_sprite, nSprites * 2);
						nSpritesRendered += nSprites;
					}
				}
			}
			m_nSpritesRendered = nSpritesRendered;

			pass.end();
		}
		pTech->end();

		m_sprites.resize(0);
	}

	void SpriteManager::onLostDevice()
	{
	}

	void SpriteManager::onResetDevice()
	{
		// Вычисляем коэффициенты масштабирования
		math::Vec2f vFrontBufferSize= render::TheDevice::get().getBackBufferSize();
		m_vScale = vFrontBufferSize / m_cvScreenSize;
		update();
	}
}