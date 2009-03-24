#include "stdafx.h"
#include "AudioTag.h"
#include "AudioManager.h"
#include "sound3D.h"
#include "musicoggvorbis.h"


namespace audio
{

	////////////////////////////
	// AudioTag class members
	////////////////////////////

	AudioTag::AudioTag()
		:	m_priority(0),
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

	bool safe_read(xml::node el, const std::string& attr_name, int& out)
	{
		xml::attribute a = el[attr_name.c_str()];
		if (a)
		{
			out = a.as_int();
			return true;
		}

		return false;
	}


	bool safe_read(xml::node el, const std::string& attr_name, float& out)
	{
		xml::attribute a = el[attr_name.c_str()];
		if (a)
		{
			out = a.as_float();
			return true;
		}

		return false;
	}

	bool safe_read(xml::node el, const std::string& attr_name, std::string& out)
	{
		xml::attribute a = el[attr_name.c_str()];
		if (a)
		{
			out = a.value();
			return true;
		}

		return false;
	}

	bool AudioTag::LoadTag(xml::node pDOMElement)
	{
		xml::node el = pDOMElement;

		safe_read(el, "PRIORITY", m_priority);

		float value = 0.0f;

		if (safe_read(el, "VOLUME_ADJUST", value))
			m_volAdjust = (int)(value * 100);

		// randomization on the volume adjust
		if (safe_read(el, "VOLUME_ADJUST_RANGE", value))
			m_volAdjustRange = (int)(value * 100);

		// number of times to loop - not specifying a value means no loop,
		// 0 means infinite, any other number is the number of times to repeat
		safe_read(el, "LOOP_TIMES", m_loopTimes);

		// seconds to delay loopiung playback
		if (safe_read(el, "LOOP_DELAY", value))
			m_loopDelay = (int)(value*1000);	// convert sec to millisec

		// randomization for loop delay
		if (safe_read(el, "LOOP_DELAY_RANGE", value))
			m_loopDelayRange = (int)(value*1000);	// convert sec to millisec

		return true;
	}

	void AudioTag::audio_finished(internal::base_audio* pAudio)
	{
		// check if the tag is set to loop
		int delay = GetLoopDelay();

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
			return m_volAdjust + ((rand() % (m_volAdjustRange*2)) - m_volAdjustRange);
		else
			return m_volAdjust;
	}

	int AudioTag::GetLoopDelay()
	{
		// calculate number of millseconds to delay looping of audio
		// if audio is not meant to be looped, return -1.

		int delay = -1;

		if (m_loopDelay >= 0)
		{
			if (0 == m_loopDelayRange)
			{
				delay = m_loopDelay;
			}
			else
			{
				delay = m_loopDelay + (rand() % (m_loopDelayRange*2) - m_loopDelayRange);
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
		:	AudioTag(),
		m_minDist(0.0f),
		m_maxDist(1000.0f),
		m_cascadeNum(0x7FFFFFFF),
		m_cascadeTag("")
	{
	}


	AudioEffectTag::~AudioEffectTag()
	{
	}


	internal::base_audio* AudioEffectTag::CreateAudio(world_object* pObj,
		int msDuration,
		int msDelay,
		audio::listener* pNotify)
	{
		Sound3D* pSound3D = new Sound3D(m_fileName.c_str());
		if (!pSound3D->GetWaveFile())
		{
			ASSERT(!"Bad wav file name");
			delete pSound3D;
			return NULL;
		}

		pSound3D->AddListener(pNotify);
		pSound3D->AddListener(this);
		return pSound3D;
	}


	bool AudioEffectTag::LoadTag(xml::node pDOMElement)
	{
		AudioTag::LoadTag(pDOMElement);

		xml::node el = pDOMElement;

		// .wav file for this effect
		if (const char* file_name = pDOMElement["FILE"].value())
		{
			m_fileName = file_name;
		}
		else
			return false;

		// minimum required distance for 3D audio culling
		safe_read(el, "MINDIST", m_minDist);

		// maximum allowed distance for 3D audio culling
		safe_read(el, "MAXDIST", m_maxDist);

		// number of times to play this sound before we switch to a new one
		safe_read(el, "CASCADENUM", m_cascadeNum);

		// the new tag to play when we hit the cascadenum
		safe_read(el, "CASCADETAG", m_cascadeTag);

		return true;
	}

	/////////////////////////////////
	// AudioMusicTag class members
	/////////////////////////////////

	AudioMusicTag::AudioMusicTag()
		:	AudioTag(),
		m_file("")
	{
	}


	AudioMusicTag::~AudioMusicTag()
	{
	}


	internal::base_audio* AudioMusicTag::CreateAudio(world_object* pObj,
		int msDuration,
		int msDelay, 
		audio::listener* pNotify)
	{
		// currently only Ogg Vorbis files are supported for streaming music
		MusicOggVorbis* pMusic = new MusicOggVorbis(m_file.c_str());
		pMusic->AddListener(pNotify);
		if (pNotify != (audio::listener*)this)
			pMusic->AddListener(this);
		return pMusic;
	}

	bool AudioMusicTag::LoadTag(xml::node pDOMElement)
	{
		AudioTag::LoadTag(pDOMElement);

		// the .ogg file for this music tag
		const char* file_name = pDOMElement["FILE"].value();
		if (NULL == file_name)
			return false;

		m_file = file_name;

		return true;
	}


	/////////////////////////////////////
	// AudioCompositionTag class members
	/////////////////////////////////////

	AudioCompositionTag::AudioCompositionTag()
		:	AudioTag(),
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


	internal::base_audio* AudioCompositionTag::CreateAudio(world_object* pObj,
		int msDuration,
		int msDelay,
		audio::listener* pNotify)
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
			ASSERT(!"Bad Composition section type");
			break;
		}

		return NULL;
	}


	bool AudioCompositionTag::LoadTag(xml::node pDOMElement)
	{
		AudioTag::LoadTag(pDOMElement);

		xml::node el = pDOMElement;

		// the first music tag to play
		if (!safe_read(el, "IN", m_inTag))
			return false;

		// the music tag to loop for the duration of the composition
		if (!safe_read(el, "LOOP", m_loopTag))
			return false;

		// the music tag to play on the exit of the composition
		if (!safe_read(el, "OUT", m_outTag))
			return false;

		// the amount of time to crossfade between LOOP and OUT
		float value = 0.0f;
		if (safe_read(el, "CROSSFADE_TO_OUT_TIME", value))
			m_crossFadeToOutTime = (int)(value*1000);	// convert sec to millisec

		return true;
	}

	void AudioCompositionTag::audio_finished(internal::base_audio* pAudio)
	{
		if (SECTION_OUT == m_lastSection)
		{
			// repeat the composition tag if necessary 
			AudioTag::audio_finished(pAudio);
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
		:	AudioTag()
	{
	}


	AudioRandomTag::~AudioRandomTag()
	{
	}


	internal::base_audio* AudioRandomTag::CreateAudio(world_object* pObj,
		int msDuration,
		int msDelay,
		audio::listener* pNotify)
	{
		int percent = (rand() % 100) + 1;
		int size = m_probabilities.size();
		int runningPercent = 0;
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

	bool AudioRandomTag::LoadTag(xml::node pDOMElement)
	{
		AudioTag::LoadTag(pDOMElement);

		for (xml::node el = pDOMElement.first_child(); el.next_sibling(); el = el.next_sibling())
		{

			// The tag name of the random element
			std::string tag_name;
			if (safe_read(el, "TAG", tag_name))
			{
				m_tags.push_back(tag_name);

				// the probability of playing this tag
				int tag_prob = 0;
				if (safe_read(el, "PROB", tag_prob))
					m_probabilities.push_back(tag_prob);
			}
		}

		if (m_tags.size() != m_probabilities.size())
			return false;

		return true;
	}


	/////////////////////////////////
	// AudioAmbientTag class members
	/////////////////////////////////
	AudioAmbientTag::AudioAmbientTag()
		:	AudioEffectTag(),
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


	internal::base_audio* AudioAmbientTag::CreateAudio(world_object* pObj,
		int msDuration,
		int msDelay,
		audio::listener* pNotify)
	{
		Sound3D* pSound3D = new Sound3D(m_fileName.c_str());
		pSound3D->AddListener(pNotify);
		if (pNotify != (audio::listener*)this)
			pSound3D->AddListener(this);
		pSound3D->SetWorldObject(&m_obj);
		return pSound3D;
	}

	bool AudioAmbientTag::LoadTag(xml::node pDOMElement)
	{
		AudioEffectTag::LoadTag(pDOMElement);
		xml::node el = pDOMElement;

		// X position for 3D audio
		safe_read(el, "X", m_xPos);

		// Y position for 3D audio
		safe_read(el, "Y", m_yPos);

		// Z position for 3D audio
		safe_read(el, "Z", m_zPos);

		// Randomization for X position
		safe_read(el, "XRANGE", m_xRange);

		// Randomization for Y position
		safe_read(el, "YRANGE", m_yRange);

		// Randomization for Z position
		safe_read(el, "ZRANGE", m_zRange);

		RandomizeWorldPosition();
		return true;
	}

	void AudioAmbientTag::audio_finished(internal::base_audio* pAudio)
	{
		RandomizeWorldPosition();
		AudioTag::audio_finished(pAudio);
	}	

	void AudioAmbientTag::RandomizeWorldPosition()
	{
		// this function calculates a new world position for the ambient sound
		// based on the tag's randomization attributes

		D3DXVECTOR3 pos;
		if (m_xRange != 0.0f)
			pos.x = m_xPos + ((float)(rand() % ((int)(m_xRange*2000))))/1000.0f - m_xRange; 
		else
			pos.x = m_xPos;

		if (m_yRange != 0.0f)
			pos.y = m_yPos + ((float)(rand() % ((int)(m_yRange*2000))))/1000.0f - m_yRange; 
		else
			pos.y = m_yPos;

		if (m_zRange != 0.0f)
			pos.z = m_zPos + ((float)(rand() % ((int)(m_zRange*2000))))/1000.0f - m_zRange; 
		else
			pos.z = m_zPos;

		m_obj.set_position(pos.x, pos.y, pos.z);
	}

	/////////////////////////////////
	// AudioGroupTag class members
	/////////////////////////////////

	AudioGroupTag::AudioGroupTag()
		:	AudioTag()
	{
	}


	AudioGroupTag::~AudioGroupTag()
	{
	}


	internal::base_audio* AudioGroupTag::CreateAudio(world_object* pObj,
		int msDuration,
		int msDelay,
		audio::listener* pNotify)
	{
		int numTags = m_tags.size();
		int delay;

		for (int i=0; i<numTags; ++i)
		{
			// calculate number of millseconds to delay individual elements
			delay = -1;

			if (m_delays[i] > 0)
			{
				if (0 == m_delayRanges[i])
					delay = m_delays[i];
				else
					delay = m_delays[i] + (rand() % (m_delayRanges[i]*2) - m_delayRanges[i]);
			}

			if (delay < 0)
				delay = 0;

			AudioManager::Instance()->Play(m_tags[i].c_str(), pObj, msDuration, msDelay+delay, this);
		}

		return NULL;
	}

	bool AudioGroupTag::LoadTag(xml::node pDOMElement)
	{
		AudioTag::LoadTag(pDOMElement);

		for (xml::node el = pDOMElement.first_child(); el.next_sibling(); el = el.next_sibling())
		{
			xml::node pChildNodes = el;

			// the tag this element in the group refers to
			std::string tag_name;
			if (safe_read(el, "TAG", tag_name))
			{
				m_tags.push_back(tag_name);

				// the delay to play this tag when starting the group
				float delay = -1;
				if (safe_read(el, "DELAY", delay))
					m_delays.push_back(delay*1000.0f);
				else
					m_delays.push_back(-1);

				// randomization for the delay
				float delay_range = 0;
				if (safe_read(el, "DELAY_RANGE", delay_range))
					m_delayRanges.push_back(delay_range*1000.0f);
				else
					m_delayRanges.push_back(0);
			}
		}

		return true;
	}
}