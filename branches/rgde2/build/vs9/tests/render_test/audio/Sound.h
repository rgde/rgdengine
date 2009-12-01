#pragma once

#include "audio.h"
#include "base_audio.h"

namespace audio
{
class wave_file;

// sound represents any .wav file audio
class sound : public internal::base_audio
{
    public:
        sound(const char* wave_filename);
        virtual ~sound();

		wave_file* get_wave_file() { return m_wave_file; }

		virtual bool fill_bufer(LPDIRECTSOUNDBUFFER pDSB,
								DWORD startIndex,
								DWORD amount,
								DWORD* pAmtRead = NULL);

		virtual void reset();

    protected:
        wave_file* m_wave_file;
		DWORD m_pos;
};
}