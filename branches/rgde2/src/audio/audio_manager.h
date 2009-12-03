#pragma once

namespace rgde
{
	namespace audio
	{

		class camera;

		namespace internal
		{
			class base_audio;
		}

		class listener;
		class audio_tag;
		class sound3d;
		class music;
		class world_object;


		// The internal::base_audio Manager class creates audio based on the XML
		// tags it loads and handles all the sound buffer updates,
		class audio_manager
		{
		public:
			enum hrtf
			{
				hrtf_full,
				hrtf_light,
				hrtf_none
			};

			enum constants
			{
				max_effect_buffers = 32,
				effect_buffer_size = 141120,
				effect_initial_read_size = 141120/2,
				effect_read_size = 141120/2,

				max_music_buffers = 2,
				music_buffer_size = 1411200,
				music_initial_read_size = 1411200/10,
				music_read_size = 1411200/10,
			};

			// Gets the instance of the singleton class
			static audio_manager* instance();

			audio_manager(HWND hWnd,
				DWORD dwPrimaryChannels = 2,
				DWORD dwPrimaryFreq = 44100,
				DWORD dwPrimaryBitRate = 16,
				hrtf hrtf = hrtf_full);

			~audio_manager();

			// loads tags from an XML file
			bool load_audio_tags(const char* file_name);
			bool load_audio_tags(xml::node node);

			void clear_audio_tags();

			audio_tag* get_audio_tag(const char* tag_name);

			// update needs to be called periodically to stream
			// new audio into the sound buffers
			void update(int msElapsed);

			void play(const char* tag_name,
				world_object* obj = NULL,		// used for positional sound effects
				int ms_duration = 0,			// length of time to play sound
				int ms_delay = 0,				// time to wait before playing
				audio::listener* notify = NULL); // listener to receive notification when sound is done

			void play(audio_tag* tag,
				world_object* obj = NULL,		// used for positional sound effects
				int ms_duration = 0,			// length of time to play sound
				int ms_delay = 0,				// time to wait before playing
				audio::listener* notify = NULL);	// listener to receive notification when sound is done

			void StopAll();

			// The game would set this to the player's camera to affect 
			// how 3D audio is heard
			void set_listener_camera(camera* pCam);  

			void SetDistanceFactor(float dF = DS3D_DEFAULTDISTANCEFACTOR);
			void SetRolloffFactor(float rF = DS3D_DEFAULTROLLOFFFACTOR);
			void SetDopplerFactor(float dF = DS3D_DEFAULTDOPPLERFACTOR);

			// The master volume that affects all sound buffers
			void SetOverallVolume(int volume); 

			// Normally wouldn't need the following functions,
			// but they're exposed so the sample can list all
			// the audio tags for the simple testing interface.
			int get_num_tags();
			const char* get_tag_name(unsigned int index);

			// returns time remaining of current music playing
			unsigned int GetRemainingMusicPlayback();	// in milliseconds

		protected:
			// static singleton instance
			static audio_manager* sm_instance;

			typedef std::map<std::string, audio_tag*> AudioTagMap;
			typedef AudioTagMap::iterator AudioTagIterator;

			struct Buffer
			{
				LPDIRECTSOUNDBUFFER		pDSBuf;			// DirectSound buffer for playback
				LPDIRECTSOUND3DBUFFER	pDSBuf3D;		// DirectSound 3D buffer for effects (positional audio)
				internal::base_audio*					audio;			// internal::base_audio created by the tag
				audio_tag*				tag;			// Tag that decided to play this sound
				bool					bMoreInBuffer;	// if more needs to be streamed into buffer
				bool					bPlaying;		// if this is an active buffer or can it be reused
				DWORD					dwLastWritePos; // last place we updated the buffer
				DWORD					dwNextWritePos; // next place to update the buffer
				long					volume;			// volume for this buffer (taking into account tag volume adjustment)
				int                     ms_duration;		// desired duration, in milliseconds
				int						msLifetime;		// elapsed playback time, in milliseconds
				long					curVolume;      // current volume (if we're fading in or out)
				float					volumeTransitionAdjust;	// amount to fade by per millsecond
			};

			struct AudioWaitingToBePlayed
			{
				audio_tag*				tag;		// the tag waiting to be played
				world_object*			obj;		// the obj we want to attach it to (for 3D sound effects)
				int		                ms_duration; // time to play
				int                     ms_delay;	// wait before playing
				audio::listener*			notify;	// the listener to receive finished notification
			};

			typedef std::list<AudioWaitingToBePlayed> AudioWaitingList;
			typedef AudioWaitingList::iterator AudioWaitingIter;

			// internal class functions
			HRESULT SetPrimaryBufferFormat(DWORD dwPrimaryChannels,
				DWORD dwPrimaryFreq,
				DWORD dwPrimaryBitRate);
			bool CreateBuffer(Buffer* buff, DWORD size, bool b3D);
			bool RestoreBuffer(Buffer* buff);
			void StopBuffer(Buffer* buff);
			void UpdateBuffer(int msElapsed, Buffer* buff, DWORD dwBufSize, DWORD dwWriteAmt);
			void UpdateListener();

			void PlayEffect(audio_tag* tag, sound3d* pSound3D, int ms_duration);
			void PlayMusic(audio_tag* tag, music* pMusic, int ms_duration);

		protected:
			LPDIRECTSOUND8 m_pDS;
			LPDIRECTSOUNDBUFFER m_pDSBPrimary;
			LPDIRECTSOUND3DLISTENER m_pDSListener;

			camera* m_pListenerCam;   // current game view to use for 3D internal::base_audio

			// DirectSound 3D internal::base_audio parameters
			float m_distanceFactor;
			float m_rolloffFactor;
			float m_dopplerFactor;		

			// initialization parameters
			DWORD m_frequency;			// sample rate (usually 44.1 kHz)
			DWORD m_bitRate;			// 
			int m_volume;				// in hundredths of decibels
			hrtf m_hrtf;				// Head related transfer function - how to calculate 3D audio

			Buffer m_effectBufs[max_effect_buffers];	
			Buffer m_musicBufs[max_music_buffers];

			AudioTagMap m_tags;			// tag database
			AudioWaitingList m_toBePlayed; // queue of delayed audio
		};


		inline audio_manager* audio_manager::instance()
		{ 
			ASSERT(NULL != sm_instance);
			return sm_instance;
		}

		inline void audio_manager::SetDistanceFactor(float dF)
		{
			m_distanceFactor = dF;
		}

		inline void audio_manager::SetRolloffFactor(float rF)
		{
			m_rolloffFactor = rF;
		}

		inline void audio_manager::SetDopplerFactor(float dF)
		{
			m_dopplerFactor = dF;
		}
	}
}