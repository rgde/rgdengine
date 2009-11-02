#pragma once

#include <dsound.h>

namespace audio
{


	namespace internal
	{
		// The internal::base_audio class is the base class for any sound we'll play
		class base_audio
		{
		public:
			enum Type
			{
				SOUND,
				SOUND3D,
				MUSIC
			};

			base_audio(Type type);
			virtual ~base_audio();

			// returns true if more data exists to be read
			virtual bool FillBuffer(LPDIRECTSOUNDBUFFER pDSB,
				DWORD startIndex,
				DWORD amount,
				DWORD* pAmtRead = NULL) = 0;

			// sets the buffer back to the beginning so the audio
			// manager can loop the sound
			virtual void Reset() = 0;

			Type GetType() const;

			void AddListener(audio::listener* listener);
			void Finish();

		protected:
			Type m_type;
			typedef std::vector<audio::listener*> Listeners;
			Listeners m_listeners;
		};

		inline base_audio::base_audio(Type type)
			:	m_type(type)
		{
		}

		inline base_audio::~base_audio()
		{
		}

		inline base_audio::Type base_audio::GetType() const
		{
			return m_type;
		}

		inline void base_audio::AddListener(audio::listener* listener)
		{
			if (listener)
				m_listeners.push_back(listener);
		}

		inline void base_audio::Finish()
		{
			// notify all our listeners that we're done
			size_t numListeners = m_listeners.size();

			for (size_t i = 0; i < numListeners; ++i)
			{
				m_listeners[i]->audio_finished(this);
			}
		}
	}
}