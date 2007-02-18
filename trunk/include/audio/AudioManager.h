#pragma once

class Audio;
struct IAudioListener;
class AudioTag;
class Sound3D;
class Music;
class WorldObject;

// The Audio Manager class creates audio based on the XML
// tags it loads and handles all the sound buffer updates,
class AudioManager
{
    public:
		enum HRTF {	HRTF_FULL, HRTF_LIGHT, HRTF_NONE };

		enum //Constants
		{
			MAX_EFFECT_BUFFERS = 32,
			EFFECT_BUFFER_SIZE = 141120,
			EFFECT_INITIAL_READ_SIZE = 141120/2,
			EFFECT_READ_SIZE = 141120/2,

			MAX_MUSIC_BUFFERS = 2,
			MUSIC_BUFFER_SIZE = 1411200,
			MUSIC_INITIAL_READ_SIZE = 1411200/10,
			MUSIC_READ_SIZE = 1411200/10,
		};

		// Gets the instance of the singleton class
		static AudioManager* Instance();
		static void			 Destroy();

        AudioManager();
		virtual ~AudioManager() {}

        virtual bool Init(HWND hWnd,
				  DWORD dwPrimaryChannels = 2,
                  DWORD dwPrimaryFreq = 44100,
				  DWORD dwPrimaryBitRate = 16,
				  HRTF hrtf = HRTF_FULL) = 0;

		void ClearAudioTags();
		
		AudioTag* GetAudioTag(const char* szTagName);
		
		// Update needs to be called periodically to stream
		// new audio into the sound buffers
		virtual void Update(int msElapsed);

		void Play(const char* szTagName,
				  WorldObject* pObj = NULL,		// used for positional sound effects
				  int msDuration = 0,			// length of time to play sound
				  int msDelay = 0,				// time to wait before playing
				  IAudioListener* pNotify = NULL); // listener to receive notification when sound is done

		void Play(AudioTag* pTag,
				  WorldObject* pObj = NULL,		// used for positional sound effects
				  int msDuration = 0,			// length of time to play sound
				  int msDelay = 0,				// time to wait before playing
				  IAudioListener* pNotify = NULL);	// listener to receive notification when sound is done
		
		// Normally wouldn't need the following functions,
		// but they're exposed so the sample can list all
		// the audio tags for the simple testing interface.
		int GetNumAudioTags() {return static_cast<int>(m_tags.size());}
		const char* GetAudioTagName(unsigned int index);
		
		// The game would set this to the player's camera to affect 
		// how 3D audio is heard
		void setListenerCamera(math::PCamera& pCam) {m_pListenerCam = pCam;}
		math::PCamera& getListenerCamera() {return m_pListenerCam;}

		void SetDistanceFactor(float dF = 1.0f){m_distanceFactor = dF;}
		void SetRolloffFactor(float rF = 1.0f) {m_rolloffFactor = rF;}
		void SetDopplerFactor(float dF = 1.0f) {m_dopplerFactor = dF;}

		// The master volume that affects all sound buffers
		virtual void SetOverallVolume(int volume) = 0; 

		virtual void StopAll() = 0;

		// returns time remaining of current music playing
		virtual unsigned int GetRemainingMusicPlayback() = 0;	// in milliseconds

	protected:
		virtual void PlayEffect(AudioTag* pTag, Sound3D* pSound3D, int msDuration) = 0;
		virtual void PlayMusic(AudioTag* pTag, Music* pMusic, int msDuration) = 0;

    protected:
		// static singleton instance
		static AudioManager* s_pInstance;

		typedef std::map<std::string, AudioTag*> AudioTagMap;
		typedef AudioTagMap::iterator AudioTagIterator;

		struct AudioWaitingToBePlayed
		{
			AudioTag*				pTag;		// the tag waiting to be played
			WorldObject*			pObj;		// the obj we want to attach it to (for 3D sound effects)
			int		                msDuration; // time to play
			int                     msDelay;	// wait before playing
			IAudioListener*			pNotify;	// the listener to receive finished notification
		};

		typedef std::list<AudioWaitingToBePlayed> AudioWaitingList;
		typedef AudioWaitingList::iterator AudioWaitingIter;

	protected:
		math::PCamera m_pListenerCam;   // current game view to use for 3D Audio

		// DirectSound 3D Audio parameters
		float m_distanceFactor;
		float m_rolloffFactor;
		float m_dopplerFactor;		

		// initialization parameters
		DWORD m_frequency;			// sample rate (usually 44.1 kHz)
		DWORD m_bitRate;			// 
		int m_volume;				// in hundredths of decibels
		HRTF m_hrtf;				// Head related transfer function - how to calculate 3D audio

		AudioTagMap m_tags;				// tag database
		AudioWaitingList m_toBePlayed;  // queue of delayed audio
};