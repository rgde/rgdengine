#include "stdafx.h"
#include "sound3D.h"
#include "wavefile.h"


#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

namespace audio
{
//-----------------------------------------------------------------------------
// Name: sound::sound()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
sound3d::sound3d(const char* wave_filename)
:	sound(wave_filename),
	m_obj(0)
{
	m_type = internal::base_audio::sound3d;
}


sound3d::~sound3d()
{
}
}