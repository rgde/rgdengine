#pragma once

#include "music.h"

namespace audio
{
class oggvorbis_file;

// music_oggvorbis is an implementation of the abstract music
// class supporting .ogg playback
class music_oggvorbis : public music
{
    public:
        music_oggvorbis(const char* vorbis_filename);
        virtual ~music_oggvorbis();

		virtual bool fill_bufer(LPDIRECTSOUNDBUFFER pDSB,
								DWORD startIndex,
								DWORD amount,
								DWORD* pAmtRead = NULL);

		virtual void reset();

		virtual unsigned int get_total_time();


    protected:
		oggvorbis_file* m_ovfile;
};
}