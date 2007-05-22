#pragma once

#include <Windows.h>
#include <MMReg.h>

#include <rgde/io/io.h>

namespace audio
{
	//-----------------------------------------------------------------------------
	// Name: class CWaveFile
	// Desc: Encapsulates reading or writing sound data to or from a wave file
	//-----------------------------------------------------------------------------
	class CWaveFile
	{
	public:
		CWaveFile(byte* pbData, uint ulDataSize, WAVEFORMATEX* pwfx, uint dwFlags);
		~CWaveFile();

		bool Read( byte* pBuffer, uint dwSizeToRead, uint* pdwSizeRead );
		uint GetSize();
		
		WAVEFORMATEX* GetFormat() { return m_pwfx; }

	protected:
		bool Reset();

	public:
		WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
		uint         m_dwSize;      // The size of the wave file
		uint         m_dwFlags;
		byte*         m_pbData;
		byte*         m_pbDataCur;
		uint         m_ulDataSize;
	};
}