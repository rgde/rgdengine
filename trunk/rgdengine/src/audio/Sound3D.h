#pragma once

#include "sound.h"

namespace audio
{
class world_object;

// Sound3D is used for any positional audio
class Sound3D : public Sound
{
    public:
        Sound3D(const char* szWaveFileName);
        virtual ~Sound3D();

		void SetWorldObject(world_object* pObj) { m_pObj = pObj; }

		world_object* GetWorldObject() { return m_pObj; }

    protected:
		world_object* m_pObj;	// the object that tells us where the sound is
};
}