#include "stdafx.h"
#include "audiotag.h"
#include "audiomanager.h"
#include "sound3D.h"
#include "musicoggvorbis.h"


namespace audio
{

	////////////////////////////
	// audio_tag class members
	////////////////////////////

	audio_tag::audio_tag()
		:	m_priority(0),
		m_vol_adjust(0),
		m_vol_adjust_range(0),
		m_loop_delay(-1),
		m_loop_delay_range(0),
		m_loop_times(0),
		m_cur_loop(0)
	{
	}

	audio_tag::~audio_tag()
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

	bool audio_tag::load_tag(xml::node node)
	{
		xml::node el = node;

		safe_read(el, "PRIORITY", m_priority);

		float value = 0.0f;

		if (safe_read(el, "VOLUME_ADJUST", value))
			m_vol_adjust = (int)(value * 100);

		// randomization on the volume adjust
		if (safe_read(el, "VOLUME_ADJUST_RANGE", value))
			m_vol_adjust_range = (int)(value * 100);

		// number of times to loop - not specifying a value means no loop,
		// 0 means infinite, any other number is the number of times to repeat
		safe_read(el, "LOOP_TIMES", m_loop_times);

		// seconds to delay loopiung playback
		if (safe_read(el, "LOOP_DELAY", value))
			m_loop_delay = (int)(value*1000);	// convert sec to millisec

		// randomization for loop delay
		if (safe_read(el, "LOOP_DELAY_RANGE", value))
			m_loop_delay_range = (int)(value*1000);	// convert sec to millisec

		return true;
	}

	void audio_tag::audio_finished(internal::base_audio* audio)
	{
		// check if the tag is set to loop
		int delay = get_loop_delay();

		if (delay >= 0)
		{
			if (0 == m_loop_times)		// play infinitely
			{
				audio_manager::instance()->play(this, NULL, 0, delay, this);
			}
			else
			{
				if (m_cur_loop < m_loop_times - 1)
				{
					audio_manager::instance()->play(this, NULL, 0, delay, this);
					++m_cur_loop;
				}
				else
				{
					m_cur_loop = 0;
				}
			}
		}
	}

	int audio_tag::get_volume_adjust()
	{
		// use range to calculate random volume adjustment (volAdjust +/- range)
		if (m_vol_adjust_range > 0)
			return m_vol_adjust + ((rand() % (m_vol_adjust_range*2)) - m_vol_adjust_range);
		else
			return m_vol_adjust;
	}

	int audio_tag::get_loop_delay()
	{
		// calculate number of millseconds to delay looping of audio
		// if audio is not meant to be looped, return -1.

		int delay = -1;

		if (m_loop_delay >= 0)
		{
			if (0 == m_loop_delay_range)
			{
				delay = m_loop_delay;
			}
			else
			{
				delay = m_loop_delay + (rand() % (m_loop_delay_range*2) - m_loop_delay_range);
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
		:	audio_tag(),
		m_minDist(0.0f),
		m_maxDist(1000.0f),
		m_cascadeNum(0x7FFFFFFF),
		m_cascadeTag("")
	{
	}


	AudioEffectTag::~AudioEffectTag()
	{
	}


	internal::base_audio* AudioEffectTag::create_audio(world_object* obj,
		int ms_duration,
		int ms_delay,
		audio::listener* notify)
	{
		sound3d* snd = new sound3d(m_filename.c_str());
		if (!snd->get_wave_file())
		{
			ASSERT(!"Bad wav file name");
			delete snd;
			return NULL;
		}

		snd->add_listener(notify);
		snd->add_listener(this);
		return snd;
	}


	bool AudioEffectTag::load_tag(xml::node node)
	{
		audio_tag::load_tag(node);

		xml::node el = node;

		// .wav file for this effect
		if (const char* file_name = node["FILE"].value())
		{
			m_filename = file_name;
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
		:	audio_tag(),
		m_file("")
	{
	}


	AudioMusicTag::~AudioMusicTag()
	{
	}


	internal::base_audio* AudioMusicTag::create_audio(world_object* obj,
		int ms_duration,
		int ms_delay, 
		audio::listener* notify)
	{
		// currently only Ogg Vorbis files are supported for streaming music
		music_oggvorbis* pMusic = new music_oggvorbis(m_file.c_str());
		pMusic->add_listener(notify);
		if (notify != (audio::listener*)this)
			pMusic->add_listener(this);
		return pMusic;
	}

	bool AudioMusicTag::load_tag(xml::node node)
	{
		audio_tag::load_tag(node);

		// the .ogg file for this music tag
		const char* file_name = node["FILE"].value();
		if (NULL == file_name)
			return false;

		m_file = file_name;

		return true;
	}


	/////////////////////////////////////
	// AudioCompositionTag class members
	/////////////////////////////////////

	AudioCompositionTag::AudioCompositionTag()
		:	audio_tag(),
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


	internal::base_audio* AudioCompositionTag::create_audio(world_object* obj,
		int ms_duration,
		int ms_delay,
		audio::listener* notify)
	{
		// play the desired section of the composition
		switch (m_lastSection)
		{
		case SECTION_NONE:
		case SECTION_OUT:
			m_lastSection = SECTION_IN;
			audio_manager::instance()->play(m_inTag.c_str(), obj, ms_duration, ms_delay, this);
			break;

		case SECTION_IN:
			{
				int delay;
				audio_manager::instance()->play(m_loopTag.c_str(), obj, ms_duration, ms_delay, this);

				// set out music to fade in if we need it to
				if (ms_duration > 0)
				{
					delay = ms_duration - m_crossFadeToOutTime;
				}
				else
				{
					delay = audio_manager::instance()->GetRemainingMusicPlayback() - m_crossFadeToOutTime;
				}

				m_lastSection = SECTION_LOOP;
				audio_manager::instance()->play(m_outTag.c_str(), obj, 0, delay, this);
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


	bool AudioCompositionTag::load_tag(xml::node node)
	{
		audio_tag::load_tag(node);

		xml::node el = node;

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

	void AudioCompositionTag::audio_finished(internal::base_audio* audio)
	{
		if (SECTION_OUT == m_lastSection)
		{
			// repeat the composition tag if necessary 
			audio_tag::audio_finished(audio);
		}
		else
		{
			// otherwise we're not done with the composition, 
			// play the next section
			audio_manager::instance()->play(this, NULL, 0, 0, this);
		}
	}


	/////////////////////////////////
	// AudioRandomTag class members
	/////////////////////////////////

	AudioRandomTag::AudioRandomTag()
		:	audio_tag()
	{
	}


	AudioRandomTag::~AudioRandomTag()
	{
	}


	internal::base_audio* AudioRandomTag::create_audio(world_object* obj,
		int ms_duration,
		int ms_delay,
		audio::listener* notify)
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
			audio_manager::instance()->play(m_tags[i].c_str(), obj, ms_duration, ms_delay, this);
		}

		return NULL;
	}

	bool AudioRandomTag::load_tag(xml::node node)
	{
		audio_tag::load_tag(node);

		for (xml::node el = node.first_child(); el.next_sibling(); el = el.next_sibling())
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


	internal::base_audio* AudioAmbientTag::create_audio(world_object* obj,
		int ms_duration,
		int ms_delay,
		audio::listener* notify)
	{
		sound3d* snd = new sound3d(m_filename.c_str());
		snd->add_listener(notify);
		if (notify != (audio::listener*)this)
			snd->add_listener(this);
		snd->set_world_object(&m_obj);
		return snd;
	}

	bool AudioAmbientTag::load_tag(xml::node node)
	{
		AudioEffectTag::load_tag(node);
		xml::node el = node;

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

	void AudioAmbientTag::audio_finished(internal::base_audio* audio)
	{
		RandomizeWorldPosition();
		audio_tag::audio_finished(audio);
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
		:	audio_tag()
	{
	}


	AudioGroupTag::~AudioGroupTag()
	{
	}


	internal::base_audio* AudioGroupTag::create_audio(world_object* obj,
		int ms_duration,
		int ms_delay,
		audio::listener* notify)
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

			audio_manager::instance()->play(m_tags[i].c_str(), obj, ms_duration, ms_delay+delay, this);
		}

		return NULL;
	}

	bool AudioGroupTag::load_tag(xml::node node)
	{
		audio_tag::load_tag(node);

		for (xml::node el = node.first_child(); el.next_sibling(); el = el.next_sibling())
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
					m_delays.push_back((int)(delay*1000.0f));
				else
					m_delays.push_back(-1);

				// randomization for the delay
				float delay_range = 0;
				if (safe_read(el, "DELAY_RANGE", delay_range))
					m_delayRanges.push_back((int)(delay_range*1000.0f));
				else
					m_delayRanges.push_back(0);
			}
		}

		return true;
	}
}