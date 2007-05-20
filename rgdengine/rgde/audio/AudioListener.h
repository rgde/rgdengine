#pragma once

class IAudio;

// Interface class to receive notifications that an audio
// piece has finished playing 
struct IAudioListener
{
	virtual void AudioFinished(IAudio* pAudio) = 0;
};