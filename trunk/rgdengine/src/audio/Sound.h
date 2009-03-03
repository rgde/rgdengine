#pragma once

#include <rgde/audio/audio.h>
#include "base_audio.h"

namespace audio
{
class WaveFile;

// Sound represents any .wav file audio
class Sound : public internal::base_audio
{
    public:
        Sound(const char* szWaveFile);
        virtual ~Sound();

		WaveFile* GetWaveFile() { return m_pWaveFile; }

		virtual bool FillBuffer(LPDIRECTSOUNDBUFFER pDSB,
								DWORD startIndex,
								DWORD amount,
								DWORD* pAmtRead = NULL);

		virtual void Reset();

    protected:
        WaveFile* m_pWaveFile;
		DWORD m_pos;
};
}