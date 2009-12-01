#pragma once

struct OggVorbis_File;

namespace audio
{
// The oggvorbis_file wraps the vorbislib sdk for .ogg
// playback - for more info see the Ogg Vorbis SDK
// http://www.vorbis.com
class oggvorbis_file
{
    public:
        oggvorbis_file();
        virtual ~oggvorbis_file();

		bool open(const char* file_name);
		bool read(char* buff, int& amount);
		bool close();
		bool reset();

		unsigned int get_total_time(); // returns time remaining in playback
		__int64 get_size();

    protected:
		OggVorbis_File*	m_pVorbisFile;
		__int64 m_size, m_read;
};

inline __int64 oggvorbis_file::get_size()
{
	return m_size;
}
}