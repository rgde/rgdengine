#pragma once
#include "audio.h"

// The Music class is the base class for any streamed music.
class Music : public Audio
{
    public:
		Music() : Audio(Audio::MUSIC) {}
		virtual ~Music() {}

		virtual unsigned int GetTotalTime() = 0;
    protected:
};