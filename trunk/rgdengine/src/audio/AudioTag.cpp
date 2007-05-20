#include "precompiled.h"
//#include "AudioTag.h"
//#include "AudioManager.h"
//#include "sound3D.h"
//#include "musicoggvorbis.h"

#include <boost/lexical_cast.hpp>

using boost::lexical_cast;
template <typename T>
T getAttribute(TiXmlElement *pEl, const std::string &name, const T &def_val = T())
{
	std::string value	= pEl->Attribute(name.c_str());
	if (value.empty())
		return def_val;
	return lexical_cast<T>(value);
}


////////////////////////////
// AudioTag class members
////////////////////////////
AudioTag::AudioTag()
	: m_priority(0),
	  m_volAdjust(0),
	  m_volAdjustRange(0),
	  m_loopDelay(-1),
	  m_loopDelayRange(0),
	  m_loopTimes(0),
	  m_curLoop(0)
{
}

AudioTag::~AudioTag()
{
}

bool AudioTag::LoadTag(TiXmlElement *pEl)
{
	// tag priority
	m_priority = getAttribute<int>(pEl, "PRIORITY");

	// amount to adjust audio volume from the overall system volume
	m_volAdjust = getAttribute<float>(pEl, "VOLUME_ADJUST") * 100;

	// randomization on the volume adjust
	m_volAdjustRange = getAttribute<float>(pEl, "VOLUME_ADJUST_RANGE") * 100;

	// number of times to loop - not specifying a value means no loop,
	// 0 means infinite, any other number is the number of times to repeat
	m_loopTimes = getAttribute<int>(pEl, "LOOP_TIMES");

	// seconds to delay loopiung playback
	m_loopDelay = getAttribute<int>(pEl, "LOOP_DELAY") * 1000; // convert sec to millisec

	// randomization for loop delay
	m_loopDelayRange = getAttribute<int>(pEl, "LOOP_DELAY_RANGE") * 1000; // convert sec to millisec

	return true;
}

void AudioTag::AudioFinished(Audio *pAudio)
{
	// check if the tag is set to loop
	int delay	= GetLoopDelay();

	if (delay >= 0)
	{
		if (0 == m_loopTimes)		// play infinitely
		{
			AudioManager::Instance()->Play(this, NULL, 0, delay, this);
		}
		else
		{
			if (m_curLoop < m_loopTimes - 1)
			{
				AudioManager::Instance()->Play(this, NULL, 0, delay, this);
				++m_curLoop;
			}
			else
			{
				m_curLoop = 0;
			}
		}
	}
}

int AudioTag::GetVolumeAdjust()
{
	// use range to calculate random volume adjustment (volAdjust +/- range)
	if (m_volAdjustRange > 0)
		return m_volAdjust + ((rand() % (m_volAdjustRange * 2)) - m_volAdjustRange);
	else
		return m_volAdjust;
}

int AudioTag::GetLoopDelay()
{
	// calculate number of millseconds to delay looping of audio
	// if audio is not meant to be looped, return -1.

	int delay	= -1;

	if (m_loopDelay >= 0)
	{
		if (0 == m_loopDelayRange)
		{
			delay = m_loopDelay;
		}
		else
		{
			delay = m_loopDelay + (rand() % (m_loopDelayRange * 2) - m_loopDelayRange);
		}

		if (delay < 0)
		{
			delay = 0;
		}
	}

	return delay;
}

/////////////////////////////////
// AudioEffectTag class members
/////////////////////////////////
AudioEffectTag::AudioEffectTag()
	: AudioTag(),
	  m_minDist(0.0f),
	  m_maxDist(1000.0f),
	  m_cascadeNum(0x7FFFFFFF),
	  m_cascadeTag("")
{
}


AudioEffectTag::~AudioEffectTag()
{
}


Audio * AudioEffectTag::CreateAudio(WorldObject *pObj, int msDuration, int msDelay, IAudioListener *pNotify)
{
	Sound3D *pSound3D	= new Sound3D(m_fileName.c_str());
	if (!pSound3D->GetWaveFile())
	{
		//ASSERT(!"Bad wav file name");
		delete pSound3D;
		return NULL;
	}

	pSound3D->AddListener(pNotify);
	pSound3D->AddListener(this);
	return pSound3D;
}

bool AudioEffectTag::LoadTag(TiXmlElement *pEl)
{
	AudioTag::LoadTag(pEl);

	// .wav file for this effect
	m_fileName = getAttribute<std::string>(pEl, "FILE");

	// minimum required distance for 3D audio culling
	m_minDist = getAttribute<int>(pEl, "MINDIST");

	// maximum allowed distance for 3D audio culling
	m_maxDist = getAttribute<int>(pEl, "MAXDIST");

	// number of times to play this sound before we switch to a new one
	m_cascadeNum = getAttribute<int>(pEl, "CASCADENUM");

	// the new tag to play when we hit the cascadenum
	m_cascadeTag = getAttribute<std::string>(pEl, "CASCADETAG");

	return true;
}

/////////////////////////////////
// AudioMusicTag class members
/////////////////////////////////
AudioMusicTag::AudioMusicTag()
	: AudioTag(),
	  m_file("")
{
}


AudioMusicTag::~AudioMusicTag()
{
}


Audio * AudioMusicTag::CreateAudio(WorldObject *pObj, int msDuration, int msDelay, IAudioListener *pNotify)
{
	// currently only Ogg Vorbis files are supported for streaming music
	MusicOggVorbis *pMusic	= new MusicOggVorbis(m_file.c_str());
	pMusic->AddListener(pNotify);
	if (pNotify != (IAudioListener *)this)
		pMusic->AddListener(this);
	return pMusic;
}

bool AudioMusicTag::LoadTag(TiXmlElement *pEl)
{
	AudioTag::LoadTag(pEl);

	// the .ogg file for this music tag
	m_file = getAttribute<std::string>(pEl, "FILE");

	return true;
}

/////////////////////////////////////
// AudioCompositionTag class members
/////////////////////////////////////
AudioCompositionTag::AudioCompositionTag()
	: AudioTag(),
	  m_inTag(""),
	  m_loopTag(""),
	  m_outTag(""),
	  m_lastSection(SECTION_NONE),
	  m_crossFadeToOutTime(0)
{
}


AudioCompositionTag::~AudioCompositionTag()
{
}


Audio * AudioCompositionTag::CreateAudio(WorldObject *pObj, int msDuration, int msDelay, IAudioListener *pNotify)
{
	// play the desired section of the composition
	switch (m_lastSection)
	{
	case SECTION_NONE:
	case SECTION_OUT:
		m_lastSection = SECTION_IN;
		AudioManager::Instance()->Play(m_inTag.c_str(), pObj, msDuration, msDelay, this);
		break;

	case SECTION_IN:
		{
			int delay;
			AudioManager::Instance()->Play(m_loopTag.c_str(), pObj, msDuration, msDelay, this);

			// set out music to fade in if we need it to
			if (msDuration > 0)
			{
				delay = msDuration - m_crossFadeToOutTime;
			}
			else
			{
				delay = AudioManager::Instance()->GetRemainingMusicPlayback() - m_crossFadeToOutTime;
			}

			m_lastSection = SECTION_LOOP;
			AudioManager::Instance()->Play(m_outTag.c_str(), pObj, 0, delay, this);
			break;
		}

	case SECTION_LOOP:
		m_lastSection = SECTION_OUT;
		// already played out piece above, with delay
		break;

	default:
		//ASSERT(!"Bad Composition section type");
		break;
	}

	return NULL;
}


bool AudioCompositionTag::LoadTag(TiXmlElement *pEl)
{
	AudioTag::LoadTag(pEl);

	// the first music tag to play
	m_inTag = getAttribute<std::string>(pEl, "IN");

	// the music tag to loop for the duration of the composition
	m_loopTag = getAttribute<std::string>(pEl, "LOOP");

	// the music tag to play on the exit of the composition
	m_outTag = getAttribute<std::string>(pEl, "OUT");

	// the amount of time to crossfade between LOOP and OUT
	m_crossFadeToOutTime = getAttribute<float>(pEl, "CROSSFADE_TO_OUT_TIME") * 1000;	// convert sec to millisec

	return true;
}

void AudioCompositionTag::AudioFinished(Audio *pAudio)
{
	if (SECTION_OUT == m_lastSection)
	{
		// repeat the composition tag if necessary 
		AudioTag::AudioFinished(pAudio);
	}
	else
	{
		// otherwise we're not done with the composition, 
		// play the next section
		AudioManager::Instance()->Play(this, NULL, 0, 0, this);
	}
}


/////////////////////////////////
// AudioRandomTag class members
/////////////////////////////////
AudioRandomTag::AudioRandomTag()
	: AudioTag()
{
}


AudioRandomTag::~AudioRandomTag()
{
}


Audio * AudioRandomTag::CreateAudio(WorldObject *pObj, int msDuration, int msDelay, IAudioListener *pNotify)
{
	int percent			= (rand() % 100) + 1;
	int size			= m_probabilities.size();
	int runningPercent	= 0;
	int i;

	// determine which tag we've randomly selected, according to
	// each tag's percentage chance
	for (i = 0; i < size; ++i)
	{
		runningPercent += m_probabilities[i]; 
		if (percent <= runningPercent)
		{
			break;
		}
	}

	// play the tag, if we've selected one
	if (i < size)
	{
		AudioManager::Instance()->Play(m_tags[i].c_str(), pObj, msDuration, msDelay, this);
	}

	return NULL;
}


bool AudioRandomTag::LoadTag(TiXmlElement *pEl)
{
	AudioTag::LoadTag(pEl);

	for (TiXmlElement *n = pEl->FirstChildElement("ITEM"); 0 != n; n = n->NextSiblingElement("ITEM"))
	{
		// The tag name of the random element
		std::string tag		= getAttribute<std::string>(n, "TAG");
		unsigned int prob	= getAttribute<unsigned int>(n, "PROB");
		if (!tag.empty())
		{
			m_tags.push_back(tag);
			m_probabilities.push_back(prob);
		}
	}
	return true;
}



/////////////////////////////////
// AudioAmbientTag class members
/////////////////////////////////
AudioAmbientTag::AudioAmbientTag()
	: AudioEffectTag(),
	  m_xPos(0.0f),
	  m_yPos(0.0f),
	  m_zPos(0.0f),
	  m_xRange(0.0f),
	  m_yRange(0.0f),
	  m_zRange(0.0f),
	  m_obj()
{
}


AudioAmbientTag::~AudioAmbientTag()
{
}


Audio * AudioAmbientTag::CreateAudio(WorldObject *pObj, int msDuration, int msDelay, IAudioListener *pNotify)
{
	Sound3D *pSound3D	= new Sound3D(m_fileName.c_str());
	pSound3D->AddListener(pNotify);
	if (pNotify != (IAudioListener *)this)
		pSound3D->AddListener(this);
	pSound3D->SetWorldObject(&m_obj);
	return pSound3D;
}


bool AudioAmbientTag::LoadTag(TiXmlElement *n)
{
	AudioEffectTag::LoadTag(n);

	// X position for 3D audio
	float m_xPos	= getAttribute<float>(n, "X");

	// Y position for 3D audio
	float m_yPos	= getAttribute<float>(n, "Y");

	// Z position for 3D audio
	float m_zPos	= getAttribute<float>(n, "Z");

	// Randomization for X position
	float m_xRange	= getAttribute<float>(n, "XRANGE");

	// Randomization for Y position
	float m_yRange	= getAttribute<float>(n, "YRANGE");

	// Randomization for Z position
	float m_zRange	= getAttribute<float>(n, "ZRANGE");

	RandomizeWorldPosition();
	return true;
}

void AudioAmbientTag::AudioFinished(Audio *pAudio)
{
	RandomizeWorldPosition();
	AudioTag::AudioFinished(pAudio);
}	

void AudioAmbientTag::RandomizeWorldPosition()
{
	// this function calculates a new world position for the ambient sound
	// based on the tag's randomization attributes

	math::Vec3f pos;
	if (m_xRange != 0.0f)
		pos[0] = m_xPos + ((float)(rand() % ((int)(m_xRange * 2000)))) / 1000.0f - m_xRange;
	else
		pos[0] = m_xPos;

	if (m_yRange != 0.0f)
		pos[1] = m_yPos + ((float)(rand() % ((int)(m_yRange * 2000)))) / 1000.0f - m_yRange;
	else
		pos[1] = m_yPos;

	if (m_zRange != 0.0f)
		pos[2] = m_zPos + ((float)(rand() % ((int)(m_zRange * 2000)))) / 1000.0f - m_zRange;
	else
		pos[2] = m_zPos;

	m_obj.SetPosition(pos[0], pos[1], pos[2]);
}

/////////////////////////////////
// AudioGroupTag class members
/////////////////////////////////
AudioGroupTag::AudioGroupTag()
	: AudioTag()
{
}


AudioGroupTag::~AudioGroupTag()
{
}


Audio * AudioGroupTag::CreateAudio(WorldObject *pObj, int msDuration, int msDelay, IAudioListener *pNotify)
{
	int numTags	= m_tags.size();
	int delay;

	for (int i = 0; i < numTags; ++i)
	{
		// calculate number of millseconds to delay individual elements
		delay = -1;

		if (m_delays[i] > 0)
		{
			if (0 == m_delayRanges[i])
				delay = m_delays[i];
			else
				delay = m_delays[i] + (rand() % (m_delayRanges[i] * 2) - m_delayRanges[i]);
		}

		if (delay < 0)
			delay = 0;

		AudioManager::Instance()->Play(m_tags[i].c_str(), pObj, msDuration, msDelay + delay, this);
	}

	return NULL;
}



bool AudioGroupTag::LoadTag(TiXmlElement *pEl)
{
	AudioTag::LoadTag(pEl);

	for (TiXmlElement *n = pEl->FirstChildElement("ITEM"); 0 != n; n = n->NextSiblingElement("ITEM"))
	{
		// the tag this element in the group refers to
		std::string tag	= getAttribute<std::string>(n, "TAG");
		if (!tag.empty())
		{
			m_tags.push_back(tag);

			// the delay to play this tag when starting the group
			float delay	= getAttribute<float>(n, "DELAY", -0.001f);
			m_delays.push_back(delay * 1000.0f);

			// randomization for the delay
			float delay_range	= getAttribute<float>(n, "DELAY_RANGE");
			m_delayRanges.push_back(delay_range * 1000.0f);
		}
	}

	return true;
}