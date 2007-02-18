#include "precompiled.h"

#include "render/renderSprites.h"

namespace render
{
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
		//SSprite		newSprite	= pSprite;
		//math::Vec2f halfScreen	= m_cvScreenSize / 2.0f;
		//newSprite.pos += m_vOrigin +  halfScreen;

		//m_bSorted = false;
		m_bUpdated = false;
		m_vSprites.push_back(pSprite);
	}
	//-----------------------------------------------------------------------------------
	bool sortPred (const SSprite& pSprite1, SSprite& pSprite2)
	{
		return (pSprite1.uPriority < pSprite2.uPriority);
	}
	////-----------------------------------------------------------------------------------
	//void CSpriteManager::sortSprites()
	//{
	//if( m_vSprites.empty() || m_bSorted )
	//{
	//	m_bSorted = true;
	//	return;
	//}

	//// Сортировка по приоритетам
	//std::sort( m_vSprites.begin(), m_vSprites.end(), sortPred );
	//}

	//	// Сортировка по текстурам
	//	m_vTexGroups.resize(0);

	//	// Определяем размеры групп спрайтов с одинаковым приоритетом
	//	m_vEqualPrioritiesN.resize(0);

	//	//for( TSpritesIter it1 = m_vSprites.begin(); it1 != (m_vSprites.end()-1); ++it1 )
	//	TSpritesIter it1 = m_vSprites.begin();
	//	while( it1 != m_vSprites.end() )
	//	{
	//		// Цикл ищет конец блока спрайтов с одинаковым приоритетом
	//		for( TSpritesIter it2 = (it1 + 1); it2 != m_vSprites.end(); ++it2 )
	//			if( it1->uPriority != it2->uPriority )
	//				break;

	//		m_vEqualPrioritiesN.push_back(static_cast<unsigned>(it2-it1));
	//		it1 = it2;
	//	};

	//	unsigned uOffset = 0;
	//	for( std::vector<unsigned>::iterator it3 = m_vEqualPrioritiesN.begin(); 
	//		it3 != m_vEqualPrioritiesN.end(); ++it3 )
	//	{
	//		// Цикл перебора текстур внутри блока спрайтов одного приоритета
	//		for( unsigned i = uOffset; i < uOffset+(*it3); ++i )
	//		{
	//			PTexture pTexture0 = m_vSprites[i].pTexture;	// Сейчас группируются спрайты с такой текстурой
	//			unsigned nTextures = 1;							// Число сгруппированных спрайтов с одинаковыми текстурами
	//			unsigned iSwap = i + 1;

	//			// Цикл поиска текстур с одинаковым именем
	//			for( unsigned j = (i + 1); j < uOffset+(*it3); ++j )
	//			{
	//				PTexture pTexture1 = m_vSprites[j].pTexture;
	//				if( pTexture0 != pTexture1 )
	//					continue;

	//				if( j == iSwap )
	//				{
	//					++iSwap;
	//					++nTextures;
	//					++i;
	//					continue;
	//				}

	//				SSprite& t = m_vSprites[iSwap];
	//				m_vSprites[iSwap] = m_vSprites[j];
	//				m_vSprites[j] = t;

	//				++iSwap;
	//				++nTextures;
	//				++i;
	//			}

	//			m_vTexGroups.push_back( STexGroup(pTexture0, nTextures) );
	//		}
	//		uOffset += *it3;
	//	}

	//	// Наконец, завершающий проход, чтобы в ситуациях, когда 100 спрайтов с приоритетами 1-100 и одной
	//	// текстурой, они рисовались за один проход
	//	unsigned it4 = 0;
	//	for( TexGroupIter it5 = (m_vTexGroups.begin()+1); it5 != m_vTexGroups.end(); ++it5 )
	//	{
	//		STexGroup& group0 = m_vTexGroups[it4];

	//		if( group0.pTexture == it5->pTexture )
	//			group0.nSprites += it5->nSprites;
	//		else
	//			++it4;
	//	}
	//	m_vTexGroups.resize(it4+1);

	//	m_bSorted = true;
	//}

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

		//if(!m_bSorted)
		//	sortSprites();

		if (m_vSprites.empty())
		{
			m_bUpdated = true;
			return;
		}

		unsigned nSprites	= (unsigned)m_vSprites.size();
		// На случай, если число частиц в векторе больше, чем зарезервировано в буферах
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
		if (m_vSprites.empty())
			return;

		//if(!m_bUpdated)
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
			IEffect::IParameter *textureShaderParam	= m_pEffect->getParams()["spriteTexture"];

			// Отрисовываем сгруппированные по одинаковым текстурам спрайты
			//for( unsigned i = 0; i < m_vTexGroups.size(); ++i )
			uint start_sprite = 0;
	  		uint end_sprite = 0;
			PTexture cur_tex;
			for (uint i = 0; i < m_vSprites.size(); ++i)
			{
				SSprite &sprite = m_vSprites[i];

				if (cur_tex != sprite.pTexture || i == m_vSprites.size()-1)
				{
					if (i == m_vSprites.size()-1)
						end_sprite++;

					unsigned nSprites = end_sprite - start_sprite;					
					cur_tex = sprite.pTexture;
					textureShaderParam->set(cur_tex);
					m_pEffect->commitChanges();
					//m_Geometry.render(PrimTypeTriangleList, 0, 4 * nSpritesRendered, nSprites * 4, 6 * nSpritesRendered, nSprites * 2);
					m_Geometry.render(PrimTypeTriangleList, 0, 4 * start_sprite, nSprites * 4, 6 * start_sprite, nSprites * 2);
					nSpritesRendered += nSprites;
					start_sprite = end_sprite;
				}

				end_sprite++;
								
			}
			m_nSpritesRendered = nSpritesRendered;

			pTech->getPasses()[iPass]->end();
		}
		pTech->end();

		m_vSprites.resize(0);
	}
	//-----------------------------------------------------------------------------------
	//inline base::log& operator << ( base::log& l, CSpriteManager::STexGroup& g )
	//{
	//	//base::lmsg << "{" << g.pTexture->getName() << " , " << g.nSprites << "}";
	//	return l;
	//}

	//-----------------------------------------------------------------------------------
	//void CSpriteManager::debugLog (bool bSortSprites)
	//{		
	//	using namespace base;

	//	// что делает сортировка в логе ?? (korak)
	//	//if (bSortSprites)
	//	//	sortSprites();

	//	for( unsigned i = 0; i < m_vSprites.size(); ++i )
	//	{
	//		lmsg << "";
	//		lmsg << "\n\n--- Спрайт " << i << (m_vSprites[i]);
	//	}

	//	lmsg << "";
	//	lmsg << "--- Группы спрайтов с одинаковыми приоритетами" << m_vEqualPrioritiesN;
	//	lmsg << "";
	//	lmsg << "--- Группы спрайтов с одинаковыми текстурами" << m_vTexGroups;
	//}

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
} //~ namespace utility