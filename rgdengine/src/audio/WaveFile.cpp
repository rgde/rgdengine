#include "precompiled.h"

#include "wavefile.h"

#include <dxerr9.h>

#include "io/io.h"

#define SAFE_RELEASE(p) 	 { if(p) { (p)->Release(); (p)=NULL; } }

namespace audio
{

	bool GetWaveData(void * pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, int& cbWaveSize) 
	//bool GetWaveData(void * pRes, WAVEFORMATEX& pWaveHeader, void * & pbWaveData, int& cbWaveSize) 
	{
		pWaveHeader = 0;
		pbWaveData = 0;
		cbWaveSize = 0;

		DWORD * pdw = (DWORD *)pRes;
		DWORD dwRiff = *pdw++;
		DWORD dwLength = *pdw++;
		DWORD dwType = *pdw++;

		if( dwRiff != mmioFOURCC('R', 'I', 'F', 'F') )
			return FALSE;      // not even RIFF

		if( dwType != mmioFOURCC('W', 'A', 'V', 'E') )
			return FALSE;      // not a WAV

		DWORD * pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

		while( pdw < pdwEnd ) {
			dwType = *pdw++;
			dwLength = *pdw++;

			switch( dwType ) {
			case mmioFOURCC('f', 'm', 't', ' '):
				if( !pWaveHeader ) {
					if( dwLength < sizeof(WAVEFORMAT) )
						return FALSE;      // not a WAV

					pWaveHeader = (WAVEFORMATEX *)pdw;

					if( pbWaveData && cbWaveSize )
						return TRUE;
				}
				break;

			case mmioFOURCC('d', 'a', 't', 'a'):
				pbWaveData = LPVOID(pdw);
				cbWaveSize = dwLength;

				if( pWaveHeader )
					return TRUE;
				break;
			}
			pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
		}

		return FALSE;
	}

	//-----------------------------------------------------------------------------
	// Name: CWaveFile::CWaveFile()
	// Desc: Constructs the class.  Call Open() to open a wave file for reading.
	//       Then call Read() as needed.  Calling the destructor or Close()
	//       will close the file.
	//-----------------------------------------------------------------------------
	CWaveFile::CWaveFile(BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags)
	{
		m_pwfx    = NULL;
		m_hmmio   = NULL;
		m_pResourceBuffer = NULL;
		m_dwSize  = 0;
		m_bIsReadingFromMemory = TRUE;

		OpenFromMemory(pbData, ulDataSize, pwfx, dwFlags);
	}


	//-----------------------------------------------------------------------------
	// Name: CWaveFile::~CWaveFile()
	// Desc: Destructs the class
	//-----------------------------------------------------------------------------
	CWaveFile::~CWaveFile()
	{
	}


	//-----------------------------------------------------------------------------
	// Name: CWaveFile::OpenFromMemory()
	// Desc: copy data to CWaveFile member variable from memory
	//-----------------------------------------------------------------------------
	HRESULT CWaveFile::OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags )
	{
		m_pwfx       = pwfx;
		m_ulDataSize = ulDataSize;
		m_pbData     = pbData;
		m_pbDataCur  = m_pbData;
		m_bIsReadingFromMemory = TRUE;

		if( dwFlags != WAVEFILE_READ )
			return E_NOTIMPL;

		return S_OK;
	}

	//-----------------------------------------------------------------------------
	// Name: CWaveFile::GetSize()
	// Desc: Retuns the size of the read access wave file
	//-----------------------------------------------------------------------------
	DWORD CWaveFile::GetSize()
	{
		return m_dwSize;
	}


	//-----------------------------------------------------------------------------
	// Name: CWaveFile::ResetFile()
	// Desc: Resets the internal m_ck pointer so reading starts from the
	//       beginning of the file again
	//-----------------------------------------------------------------------------
	HRESULT CWaveFile::Reset()
	{
		m_pbDataCur = m_pbData;
		return S_OK;
	}


	//-----------------------------------------------------------------------------
	// Name: CWaveFile::Read()
	// Desc: Reads section of data from a wave file into pBuffer and returns
	//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
	//       This uses m_ck to determine where to start reading from.  So
	//       subsequent calls will be continue where the last left off unless
	//       Reset() is called.
	//-----------------------------------------------------------------------------
	HRESULT CWaveFile::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
	{
		if( m_pbDataCur == NULL )
			return CO_E_NOTINITIALIZED;
		if( pdwSizeRead != NULL )
			*pdwSizeRead = 0;

		if( (BYTE*)(m_pbDataCur + dwSizeToRead) >
			(BYTE*)(m_pbData + m_ulDataSize) )
		{
			dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
		}

		CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );

		if( pdwSizeRead != NULL )
			*pdwSizeRead = dwSizeToRead;

		return S_OK;
	}

}