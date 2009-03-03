#include "precompiled.h"
#include "sound3D.h"
#include "wavefile.h"


#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

namespace audio
{
//-----------------------------------------------------------------------------
// Name: Sound::Sound()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
Sound3D::Sound3D(const char* szWaveFile)
:	Sound(szWaveFile),
	m_pObj(0)
{
	m_type = internal::base_audio::SOUND3D;
}


Sound3D::~Sound3D()
{
}
}