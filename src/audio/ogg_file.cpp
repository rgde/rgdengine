#include "stdafx.h"

#include "ogg_file.h"

namespace rgde
{
	namespace audio
	{

		oggvorbis_file::oggvorbis_file()
			:	m_pVorbisFile(NULL),
			m_size(0),
			m_read(0)
		{
		}


		oggvorbis_file::~oggvorbis_file()
		{
			if (m_pVorbisFile)
			{
				ASSERT(!"oggvorbis_file: File not closed before deletion");
				close();
				delete m_pVorbisFile;
				m_pVorbisFile = NULL;
			}
		}


		bool oggvorbis_file::open(const char* file_name)
		{
			ASSERT(!m_pVorbisFile);

			if (m_pVorbisFile)
				return false;
			FILE* pFile = fopen (file_name, "rb");

			if (pFile)
			{
				m_pVorbisFile = new OggVorbis_File();
				if (0 == ov_open(pFile, m_pVorbisFile, NULL, 0))
				{
					vorbis_info *vi = ov_info(m_pVorbisFile, -1);
					m_size = ov_pcm_total(m_pVorbisFile, 0);
					m_read = 0;
					return true;
				}
				else
				{
					delete m_pVorbisFile;
					m_pVorbisFile = NULL;
					fclose(pFile);
				}
			}

			return false;
		}


		bool oggvorbis_file::read(char* buff, int& amount)
		{
			ASSERT(m_pVorbisFile);

			if (m_pVorbisFile)
			{
				int bitstream = 0;
				int currentRead = 0;
				int totalRead = 0;
				int leftToRead = amount;

				// Because ov_read only decodes AT MOST one vorbis packet, we may have
				// to read multiple times to get the desired amount. 
				do
				{
					currentRead = ov_read(m_pVorbisFile, buff+totalRead, leftToRead, 0, 2, 1, &bitstream);

					if (currentRead > 0)
					{
						totalRead += currentRead;
						m_read += currentRead;
						leftToRead -= currentRead;
					}
					else if (currentRead < 0)
					{
						ASSERT(false);
					}
				}
				while (currentRead > 0 && totalRead < amount);

				amount = totalRead;

				return ov_time_total(m_pVorbisFile, -1) - ov_time_tell(m_pVorbisFile) > 0.0f; //m_read < m_size;
			}
			else
			{
				amount = 0;
				return false;
			}
		}


		bool oggvorbis_file::close()
		{
			if (m_pVorbisFile)
			{
				int nRet = ov_clear(m_pVorbisFile);

				if (0 == nRet)
				{
					delete m_pVorbisFile;
					m_pVorbisFile = NULL;
					return true;
				}
			}

			return false;
		}


		bool oggvorbis_file::reset()
		{
			return (0 == ov_pcm_seek(m_pVorbisFile, 0));
		}


		unsigned int oggvorbis_file::get_total_time()
		{
			double secs = ov_time_total(m_pVorbisFile, -1);
			return (unsigned int)(secs*1000);
		}

	}
}