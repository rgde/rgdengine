#pragma once

struct OggVorbis_File;

// The OggVorbisFile wraps the vorbislib sdk for .ogg
// playback - for more info see the Ogg Vorbis SDK
// http://www.vorbis.com
class OggVorbisFile
{
public:
					OggVorbisFile();
	virtual			~OggVorbisFile();

	bool			Open(const char *szFileName);
	bool			Read(char *pBuf, int &amount);
	bool			Close();
	bool			Reset();

	unsigned int	GetTotalTime(); // returns time remaining in playback
	__int64 GetSize()
	{
		return m_size;
	}

protected:
	OggVorbis_File	*m_pVorbisFile;
	__int64			m_size, m_read;
};