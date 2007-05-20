#pragma once


class AudioManagerImpl : public AudioManager
{
public:
							AudioManagerImpl() ;
	virtual					~AudioManagerImpl();

	virtual bool			Init(HWND hWnd, DWORD dwPrimaryChannels = 2, DWORD dwPrimaryFreq = 44100, DWORD dwPrimaryBitRate = 16, HRTF hrtf = HRTF_FULL);

	virtual void			Update(int msElapsed);
	virtual void			StopAll();
	virtual unsigned int	GetRemainingMusicPlayback();
	virtual void			SetOverallVolume(int volume);

private:
	struct Buffer
	{
		LPDIRECTSOUNDBUFFER		pDSBuf;			// DirectSound buffer for playback
		LPDIRECTSOUND3DBUFFER	pDSBuf3D;		// DirectSound 3D buffer for effects (positional audio)
		Audio					*pAudio;			// Audio created by the tag
		AudioTag				*pTag;			// Tag that decided to play this sound
		bool					bMoreInBuffer;	// if more needs to be streamed into buffer
		bool					bPlaying;		// if this is an active buffer or can it be reused
		DWORD					dwLastWritePos; // last place we updated the buffer
		DWORD					dwNextWritePos; // next place to update the buffer
		long					volume;			// volume for this buffer (taking into account tag volume adjustment)
		int						msDuration;		// desired duration, in milliseconds
		int						msLifetime;		// elapsed playback time, in milliseconds
		long					curVolume;  	// current volume (if we're fading in or out)
		float					volumeTransitionAdjust;	// amount to fade by per millsecond
	};

	// internal class functions
	HRESULT					SetPrimaryBufferFormat(DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate);

	bool					CreateBuffer(Buffer *pBuf, DWORD size, bool b3D);
	bool					RestoreBuffer(Buffer *pBuf);
	void					StopBuffer(Buffer *pBuf);
	void					UpdateBuffer(int msElapsed, Buffer *pBuf, DWORD dwBufSize, DWORD dwWriteAmt);
	void					UpdateListener();

	void					PlayEffect(AudioTag *pTag, Sound3D *pSound3D, int msDuration);
	void					PlayMusic(AudioTag *pTag, Music *pMusic, int msDuration);

private:
	LPDIRECTSOUND8			m_pDS;
	LPDIRECTSOUNDBUFFER		m_pDSBPrimary;
	LPDIRECTSOUND3DLISTENER	m_pDSListener;

	Buffer					m_effectBufs[MAX_EFFECT_BUFFERS];	
	Buffer					m_musicBufs[MAX_MUSIC_BUFFERS];
};

