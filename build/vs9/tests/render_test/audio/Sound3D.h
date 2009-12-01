#pragma once

#include "sound.h"

namespace audio
{
class world_object;

// sound3d is used for any positional audio
class sound3d : public sound
{
    public:
        sound3d(const char* wave_filename);
        virtual ~sound3d();

		void set_world_object(world_object* obj) { m_obj = obj; }
		world_object* get_world_object() { return m_obj; }

    protected:
		world_object* m_obj;	// the object that tells us where the sound is
};
}