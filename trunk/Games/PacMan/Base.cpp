/**--------------------------------------------------------------------------------------

									RGDE Engine

----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru

Created:	15 june 2005
---------------------------------------------------------------------------------------*/
#include "precomp.h"
#include "Base.h"
#include "TypeInfo.h"

#include "test_PacMan.h"



namespace test_PacMan
{

	math::Vec2f CBaseObject::m_vSize = math::Vec2f (40,40);


	//-----------------------------------------------------------------------------------
	CBaseObject::CBaseObject (const std::string& strTexName)
		 : m_eBaseType (eBaseTypeNotSet), m_eType (eTypeNotSet)
	{
		m_pTypeInfo = CTypeInfo::AddObject (this);

		m_pTexture = render::CTexture::Create (strTexName);
		m_pSprite = new render::SSprite ( m_pTexture, math::Rect (0, 0, 1, 1), m_vPos, m_vSize, 0 );
		g_pPacManTest->getSpriteManager()->addSprite (m_pSprite);
	}

}