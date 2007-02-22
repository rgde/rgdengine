#pragma once
#include <engine.h>

class CSpriteTest : public game::IDynamicObject
{
public:
    CSpriteTest();

protected:
    void onEsc ();
    void update (float dt);

protected:
    render::CSpriteManager				m_sprite_renderer;
    render::CSpriteManager::TSprites	m_vSprites;
    input::CKeyDown                     m_cEsc;
    unsigned							m_nPriorities;				// Число приоритетов спрайтов
    unsigned							m_nSpritesPerPriority;		// Число спрайтов с одинаковым приоритетом
    bool								m_bDebugLog;				// Стоит ли выводить отладочный лог
    bool								m_bSortSpritesBeforeLog;	// Стоит ли сортировать спрайты перед логом
    math::Vec2f							m_vSpritesSeed;				// Область разброса спрайтов
    ::render::PFont						m_spFont;
    math::PCamera						m_camera;
};