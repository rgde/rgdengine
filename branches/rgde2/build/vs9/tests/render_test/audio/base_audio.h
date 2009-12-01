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
			enum type
			{
				sound,
				sound3d,
				music
			};

			base_audio(type type);
			virtual ~base_audio();

			// returns true if more data exists to be read
			virtual bool fill_bufer(LPDIRECTSOUNDBUFFER pDSB,
				DWORD startIndex,
				DWORD amount,
				DWORD* pAmtRead = NULL) = 0;

			// sets the buffer back to the beginning so the audio
			// manager can loop the sound
			virtual void reset() = 0;

			type get_type() const;

			void add_listener(listener* pListener);
			void finish();

		protected:
			type m_type;
			typedef std::vector<audio::listener*> listeners;
			listeners m_listeners;
		};

		inline base_audio::base_audio(type type)
			:	m_type(type)
		{
		}

		inline base_audio::~base_audio()
		{
		}

		inline base_audio::type base_audio::get_type() const
		{
			return m_type;
		}

		inline void base_audio::add_listener(audio::listener* pListener)
		{
			if (pListener)
				m_listeners.push_back(pListener);
		}

		inline void base_audio::finish()
		{
			// notify all our listeners that we're done
			size_t num_listeners = m_listeners.size();

			for (size_t i = 0; i < num_listeners; ++i)
			{
				m_listeners[i]->audio_finished(this);
			}
		}
	}
}