#pragma once

#include <vector>
#include "AudioListener.h"


// The Audio class is the base class for any sound we'll play
class IAudio
{
    public:
		enum Type {	SOUND, SOUND3D, MUSIC };

		IAudio(Type type) :	m_type(type) {}
		virtual ~IAudio(){}

		// sets the buffer back to the beginning so the audio
		// manager can loop the sound
		virtual void Reset() = 0;

		Type GetType() const	{return m_type;	}

		void AddListener(IAudioListener* pListener);
		void Finish();

	protected:
		Type m_type;
		std::vector<IAudioListener*> m_listeners;
};

inline void IAudio::AddListener(IAudioListener* pListener)
{
	if (pListener)
		m_listeners.push_back(pListener);
}

inline void IAudio::Finish()
{
	// notify all our listeners that we're done
	for (unsigned int i = 0; i < m_listeners.size(); ++i)
		m_listeners[i]->AudioFinished(this);
}

struct IDirectSoundBuffer;

class Audio : public IAudio
{
public:
	Audio(Type type) :	IAudio(type) {}
	// returns true if more data exists to be read
	virtual bool FillBuffer(IDirectSoundBuffer* pDSB, DWORD startIndex, DWORD amount, DWORD* pAmtRead = NULL) = 0;
};

