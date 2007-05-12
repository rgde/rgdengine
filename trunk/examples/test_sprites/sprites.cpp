/**--------------------------------------------------------------------------------------
									RGDE Engine
----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru
Created:	8 june 2005
---------------------------------------------------------------------------------------*/
//RGDE
#include "sprites.h"

namespace math
{
	using namespace Math;
}

CSpriteTest::CSpriteTest() : 
		m_nPriorities (1),
		m_nSpritesPerPriority (100),
		m_bDebugLog (false),
		m_bSortSpritesBeforeLog (true),
		m_vSpritesSeed (math::Vec2f (770, 570))		
	{
        {
            using namespace input;

            m_cEsc.attach(L"Quit");
			Input::getDevice(types::Keyboard)->getControl(types::KeyEscape)->bind(L"Quit");            
            m_cEsc += boost::bind(&CSpriteTest::onEsc, this);
        }

		m_spFont = render::IFont::Create(12, L"Arial", render::IFont::Heavy);

		std::vector<render::PTexture> vTextures;
		vTextures.push_back(render::ITexture::Create( "Sprites/test01.jpg" ));
		vTextures.push_back(render::ITexture::Create( "Sprites/test02.jpg" ));
		vTextures.push_back(render::ITexture::Create( "Sprites/test03.jpg" ));
		vTextures.push_back(render::ITexture::Create( "Sprites/test04.jpg" ));
		vTextures.push_back(render::ITexture::Create( "Sprites/test05.bmp" ));

		for( unsigned priority = m_nPriorities; priority > 0; --priority )
			for( unsigned sprite = m_nSpritesPerPriority; sprite > 0; --sprite )
			{
				float x = m_vSpritesSeed[0];
				float y = m_vSpritesSeed[1];

				math::Rect rect(0, 0, 1, 1);

				using math::unitRandom;
				using math::rangeRandom;

				for (uint i = 0; i < vTextures.size(); ++i)
				{
					math::Vec2f size(rangeRandom(0,80),rangeRandom(0,80));
					math::Vec2f pos(rangeRandom(0, x), rangeRandom(0, y));
					math::Color color;
					color.set(unitRandom(),	unitRandom(), unitRandom(),	unitRandom());

					float spin = rangeRandom (0, math::TWO_PI);

					render::SSprite sprite(pos, size, color, vTextures[i], spin, rect, priority);
					m_vSprites.push_back(sprite);
				}
			}
	}

	void CSpriteTest::onEsc ()
	{
		core::IApplication::Get()->close();
	}
	//-----------------------------------------------------------------------------------
	void CSpriteTest::update (float dt)
	{
		render::TheDevice::Get().showFPS(m_spFont);

		for (render::CSpriteManager::TSpritesIter it = m_vSprites.begin(); it != m_vSprites.end(); ++it)
		{
			it->spin += dt;
			m_sprite_renderer.addSprite(*it);
		}
	}