#include "stdafx.h"
#include "sound.h"
#include "wavefile.h"
#include "wavefilefactory.h"
#include "audiomanager.h"

//#include <mmreg.h>
//#include <dxerr9.h>

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

namespace audio
{
//-----------------------------------------------------------------------------
// Name: sound::sound()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
sound::sound(const char* wave_filename)
:	internal::base_audio(internal::base_audio::sound),
	m_pos(0)
{
	m_wave_file = wave_file_factory::instance()->get(wave_filename);
}


sound::~sound()
{
	if (m_wave_file)
	{
		m_wave_file = NULL;
	}
}


bool sound::fill_bufer(LPDIRECTSOUNDBUFFER pDSB,
					   DWORD startIndex,
					   DWORD amount,
					   DWORD* pAmtRead)
{
    HRESULT hr; 
    VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

	if (pAmtRead)
		*pAmtRead = 0;

    if (pDSB == NULL)
        return false;

    // Lock the buffer down - assumes length defined by audio manager
    if (FAILED(hr = pDSB->Lock(startIndex, amount, 
                               &pDSLockedBuffer, &dwDSLockedBufferSize, 
                               NULL, NULL, 0L)))
	{
        DXTRACE_ERR(TEXT("Lock"), hr);
		return false;
	}

    // reset the wave file to the beginning 
    m_wave_file->reset_file();

    if (FAILED( hr = m_wave_file->read((BYTE*)pDSLockedBuffer,
									   m_pos,
                                       dwDSLockedBufferSize, 
                                       &dwWavDataRead)))        
	{
	    pDSB->Unlock(pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0);
		DXTRACE_ERR(TEXT("read"), hr);
		return false;
	}

    if (dwWavDataRead < amount)
    {
        // Wav is blank, so just fill with silence
        FillMemory((BYTE*) pDSLockedBuffer+dwWavDataRead, dwDSLockedBufferSize-dwWavDataRead-1, 
                   (BYTE)(m_wave_file->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }

	m_pos += dwWavDataRead;

    // Unlock the buffer, we don't need it anymore.
    pDSB->Unlock(pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0);

	if (pAmtRead)
		*pAmtRead = dwWavDataRead;

    return (m_pos < m_wave_file->get_size());
}

void sound::reset()
{
	// reset the reading position back to 0
	m_pos = 0;

	if (m_wave_file)
	{
		m_wave_file->reset_file();
	}
}
}