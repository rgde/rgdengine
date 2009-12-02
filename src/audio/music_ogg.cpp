#include "stdafx.h"
#include "music_ogg.h"
#include "ogg_file.h"



namespace rgde
{
	namespace audio
	{


		music_oggvorbis::music_oggvorbis(const char* file_name)
			:	music(),
			m_ovfile(NULL)
		{
			m_ovfile = new oggvorbis_file();
			if (m_ovfile)
			{
				if (!m_ovfile->open(file_name))
				{
					delete m_ovfile;
					m_ovfile = NULL;
				}
			}

			ASSERT(m_ovfile && "Couldn't find/open .ogg file.");
		}


		music_oggvorbis::~music_oggvorbis()
		{
			if (m_ovfile)
			{
				m_ovfile->close();
				delete m_ovfile;
				m_ovfile = NULL;
			}
		}


		bool music_oggvorbis::fill_bufer(LPDIRECTSOUNDBUFFER pDSB,
			DWORD startIndex,
			DWORD amount,
			DWORD* pAmtRead)
		{
			HRESULT hr; 
			VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
			DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
			VOID*   pDSLockedBuffer2      = NULL; // Pointer to locked buffer memory
			DWORD   dwDSLockedBufferSize2 = 0;    // Size of the locked DirectSound buffer

			if (pDSB == NULL)
				return false;

			if (FAILED(hr = pDSB->Lock(startIndex, amount, 
				&pDSLockedBuffer, &dwDSLockedBufferSize, 
				NULL, NULL, 0L)))
			{
				DXTRACE_ERR(TEXT("Lock"), hr);
				return false;
			}


			int read = dwDSLockedBufferSize;
			bool bMore = m_ovfile->read((char*)pDSLockedBuffer, read);

			// Fill Buffer with silence first because we won't get the exact
			// amount read back from ogg. Assume bit stream > 8bit, therefore
			// silence is 0, not 128.
			FillMemory(((BYTE*)pDSLockedBuffer)+read, dwDSLockedBufferSize-read, 0);

			// Unlock the buffer, we don't need it anymore.
			pDSB->Unlock(pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0);

			if (pAmtRead)
				*pAmtRead = read;

			return bMore;
		}

		void music_oggvorbis::reset()
		{
			if (m_ovfile)
			{
				m_ovfile->reset();
			}
		}

		unsigned int music_oggvorbis::get_total_time()
		{
			ASSERT(m_ovfile);
			return m_ovfile->get_total_time();
		}
	}
}