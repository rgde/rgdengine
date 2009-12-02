#include "stdafx.h"
#include "wavefile.h"
#include "wavefile_factory.h"

namespace rgde
{
	namespace audio
	{

		wave_file_factory* wave_file_factory::sm_instance = NULL;

		wave_file_factory::wave_file_factory()
		{
		}


		wave_file_factory::~wave_file_factory()
		{
			wave_iter iWav = m_wavs.begin();
			wave_iter endWav = m_wavs.end();

			for (; iWav != endWav; ++iWav)
			{
				delete iWav->second;
			}

			m_wavs.clear();
		}


		wave_file* wave_file_factory::get(const char* file_name)
		{
			wave_iter found = m_wavs.find(file_name);

			if (found != m_wavs.end())
			{
				return found->second;
			}
			else  // not found, we need to load the wav file
			{
				wave_file* pWaveFile = new wave_file();
				if (!pWaveFile->open(file_name, NULL))
				{
					delete pWaveFile;
					return NULL;
				}

				WAVEFORMATEX* pwfx = pWaveFile->get_format();

				// check for invalid format or too many channels in wave,
				// sound must be mono and PCM when using DSBCAPS_CTRL3D,
				if (pwfx == NULL || pwfx->nChannels > 1 || pwfx->wFormatTag != WAVE_FORMAT_PCM) 
				{
					delete pWaveFile;
					return NULL;
				}
				else
				{
					m_wavs.insert(wave_map::value_type(file_name, pWaveFile));
					return pWaveFile;
				}
			}
		}
	}
}