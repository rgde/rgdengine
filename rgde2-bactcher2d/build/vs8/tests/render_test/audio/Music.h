#pragma once

#include "audio.h"
#include "base_audio.h"

namespace audio
{

	// The Music class is the base class for any streamed music.
	class Music : public internal::base_audio
	{
	public:
		Music();
		virtual ~Music();

		virtual unsigned int GetTotalTime() = 0;
	protected:
	};


	inline Music::Music()
		:	internal::base_audio(internal::base_audio::MUSIC)
	{
	}

	inline Music::~Music()
	{
	}
}