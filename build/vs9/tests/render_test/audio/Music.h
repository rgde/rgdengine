#pragma once

#include "audio.h"
#include "base_audio.h"

namespace audio
{

	// The music class is the base class for any streamed music.
	class music : public internal::base_audio
	{
	public:
		music();
		virtual ~music();

		virtual unsigned int get_total_time() = 0;
	protected:
	};


	inline music::music()
		:	internal::base_audio(internal::base_audio::music)
	{
	}

	inline music::~music()
	{
	}
}